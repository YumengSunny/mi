#include "../h/pmc.h"
#include "../h/pio.h"
#include "../h/usart.h"
#include "../h/tc.h"
#include "../h/aic.h"
#include <stdio.h>

int baudrate = 38400;
volatile int c1 = 2000;
volatile int c2 = 0;

#define TC4_INIT  TC_CLKS_MCK2 | TC_LDBSTOP | TC_CAPT | TC_LDRA_RISING_EDGE | TC_LDRB_RISING_EDGE
#define TC5_INIT  TC_CLKS_MCK2 | TC_LDBSTOP | TC_CAPT | TC_LDRA_RISING_EDGE | TC_LDRB_RISING_EDGE
#define DEFAULT_BAUD = baudrate

volatile StructPMC* pmcbase  = PMC_BASE;
volatile StructPIO* piobaseA = PIOA_BASE;
volatile StructPIO* piobaseB = PIOB_BASE;
volatile StructTC* tcbase4   = TCB4_BASE;
volatile StructTC* tcbase5   = TCB5_BASE;
volatile StructAIC* aicbase  = AIC_BASE;

void pump_stop(void)
{
  StructPIO* piobaseA = PIOA_BASE;        // Base address PIOA (for pump pins)
  piobaseA->PIO_CODR  = (1<<PIOTIOA3);     // Clear (set to low) to prevent permanent high
  piobaseA->PIO_PER   = (1<<PIOTIOA3);   // Port enable (switch to PIOB)
}

void init_messung(void){
    pmcbase->PMC_PCER = 0x06f80;    // Clock PIOA, PIOB, Timer5, Timer4, Timer1 einschalten

    // Periodendauer der Waagensignale messen
    // Signal aud TIOA4 ca. 16kHz entspricht ca. einer Periodendauer von 62,5us
    // durch den Teiler von 32 ergeben sich ca. 2ms
    // ZÃ¤hler mit positiver Flanke starten

    piobaseA->PIO_PDR = 0x090;
    tcbase4->TC_CCR   = TC_CLKDIS;            //Channel Control Register =  Counter Clock Disable Command (Disables Clock)
    tcbase4->TC_CMR   = TC4_INIT;            //Channel Mode Register = siehe define
    tcbase4->TC_CCR   = TC_CLKEN;            //Counter Clock Enable Comman Enables the clock if CLKDIS is not 1
    tcbase4->TC_CCR   = TC_SWTRG;            //Software Trigger Command: Counter reset and Clock start

    piobaseA->PIO_PDR = 0x090;
    tcbase5->TC_CCR   = TC_CLKDIS;            //Channel Control Register =  Counter Clock Disable Command (Disables Clock)
    tcbase5->TC_CMR   = TC5_INIT;            //Channel Mode Register = siehe define
    tcbase5->TC_CCR   = TC_CLKEN;            //Counter Clock Enable Comman Enables the clock if CLKDIS is not 1
    tcbase5->TC_CCR   = TC_SWTRG;            //Software Trigger Command: Counter reset and Clock start

}

float MessungDerMasse(void){
    volatile int    captureRA1;
    volatile int    captureRB1;
    volatile int    capturediff1;
    volatile int    captureRA2;
    volatile int    captureRB2;
    volatile int    capturediff2;
    volatile float  Periodendauer1;
    volatile float  Periodendauer2;

    int temp = 0;
    temp = tcbase4->TC_SR & 0x40; // vor SWTRG TC_SR(timer control status register) lesen
    temp = tcbase5->TC_SR & 0x40;
    tcbase4->TC_CCR    =    TC_SWTRG;
    while (!( tcbase4->TC_SR & 0x40));        // Status Register and TC Channel 1 //Capture Register B wurde geladen Messung abgeschlossen
        captureRA1    = tcbase4->TC_RA;                //
        captureRB1    = tcbase4->TC_RB;
        capturediff1    =    abs(captureRB1) - abs(captureRA1);
        Periodendauer1 = abs(capturediff1);    // Zeit in us

    tcbase5->TC_CCR    =    TC_SWTRG;
    while (!( tcbase5->TC_SR & 0x40));        // Status Register and TC Channel 1 //Capture Register B wurde geladen Messung abgeschlossen
        captureRA2    = tcbase5->TC_RA;
        captureRB2    = tcbase5->TC_RB;
        capturediff2    =    abs(captureRB2) - abs(captureRA2);
        Periodendauer2 = abs(capturediff2);    // Zeit in us

    return (c1* ((Periodendauer1/Periodendauer2)-1)-c2 );
}

int MasseMitGenauigkeit(void){      // Die Funktion misst 3 Mal das Gewicht auf der Wage und gibt den gerundeten Wert zurück
    float erg = MessungDerMasse();
    erg = erg + MessungDerMasse();
    erg = erg + MessungDerMasse();
    return ((erg/3)+0.5);
}



// Timer3 initialisieren
void pump_start( void )
{
    StructTC* timerbase3  = TCB3_BASE;        // Basisadressse TC Block 3
    StructPIO* piobaseA   = PIOA_BASE;        // Basisadresse PIO A

    timerbase3->TC_CCR = TC_CLKDIS;            // Disable Clock

    // Initialize the mode of the timer 3
    timerbase3->TC_CMR =          //Channel Mode Register
       TC_ACPC_CLEAR_OUTPUT  |    //ACPC    : Register C clear TIOA
       TC_ACPA_SET_OUTPUT    |    //ACPA    : Register A set TIOA
       TC_WAVE               |    //WAVE    : Waveform mode
       TC_CPCTRG             |    //CPCTRG  : Register C compare trigger enable
       TC_CLKS_MCK1024;           //TCCLKS  : MCKI / 1024 = 24 414,0625

    // Initialize the counter:
    timerbase3->TC_RA = 244;    // Set
    timerbase3->TC_RC = 488;    // Clear

    // Start the timer :
    timerbase3->TC_CCR = TC_CLKEN ;        // CCR = Channel Control Register -> Clock enable
    timerbase3->TC_CCR = TC_SWTRG ;        // ZÃ¤hler reset, Clock starten
    piobaseA->PIO_PDR  = (1<<PIOTIOA3) ;    // (Port Disable Register) TIOA Port deaktivieren -> Timer bekommt Kontrolle ueber Pin
    piobaseA->PIO_OER  = (1<<PIOTIOA3) ;    // (Output Enable Register) TIOA Output aktivieren
    piobaseA->PIO_CODR = (1<<PIOTIOA3) ;    // (Clear Output Data Register)

}

void intOutput (int var)
{
    long int a = 10;
    int b = 0;            //b legt später fest and welche Stelle des arrays der char geschrieben wird
    char carray[22];        //20 Stellen fue eine 64bit Zahl + eine Stelle fuer '-' und eine fuer '\0'

    if(var < 0){            //prüfen ob var kleiner 0, wenn ja, ein minus in das array schreiben und var mit minus 1 multiplizieren
        var = var * -1;
        carray[0]='-';
        b=b+1;
        }

    while((var % a) != var){    //a so oft mit 10 multiplizieren, bis var%a = 0
    a=a*10;                //später wird var durch a geteilt um die erste Stelle rauszukriegen
    }
    a=a/10;                //am Ende der Schleife is a um eine Größenordnung größer als var, deswegen /10

    while(a>0){            // var stellenweise in das array schreiben
    carray[b]=((var/a)+48);        // Die Ziffern 0-9 fangen bei 48 im ASCII-Code an var/a = ergibt die jeweils nächste Stelle der Zahl
    var = var - ((var/a)*a);    // Entfernt die herausgeschriebene Ziffer. z.B. 253/100 = 2, 2*100 = 200, 253-200 = 53
    a = a/10;            // a um eine Größenordnung verringern
    b = b+1;            // nächste Stelle des arrays
    }
    carray[b]='\0';                 //array "abschließen"
    puts(carray);            //array ausgeben
}


int main(void)
{
    pump_stop();
    int test1 = 0;
    int test = 50;
    // Serielle initialisieren
    inits();
    init_ser();

    puts("Herzlich willkommen zur Ausschankstation. Bitte Folgen Sie den Anweisungen.\n");

    while(1){
    pump_stop();                            //falls die Pumpe noch läuft
    int fehlertoleranz = 7;
    int voll = 50;                            // Welche Menge soll abgefüllt werden
    puts("Bitte stellen Sie zur Kalibrierung den Becher auf und drücken Sie SW1 \n");
    init_messung();
    while(piobaseB->PIO_PDSR & KEY1){}                              //polling (KEY1)
    int becher = 0;
    becher = MasseMitGenauigkeit();                                 //Bechergewicht messen
    puts("Der Becher hat folgende Masse in Gramm: ");
    intOutput(becher);
    puts("\n");
    puts("Drücken Sie SW2 um den Becher zu befüllen. \n");

    while(piobaseB->PIO_PDSR & KEY2){}                              //polling (KEY2)
        if (MasseMitGenauigkeit() < becher+fehlertoleranz && MasseMitGenauigkeit() > becher -fehlertoleranz){
        // pump_init();
            pump_start();
            int alteMasse = becher;
            int neueMasse = becher;
            int testMasse = becher;
            int ausgabe = 1;
             while ((neueMasse - becher) < voll){         // Solange Masse minus Bechergewicht kleiner als die gewünschte Menge ist, Schleife ausführen
             test1++;
             if(test1%test==0){
             if(testMasse>=neueMasse){
                 pump_stop();
                puts("keine Gewichtszunahme !! \n");
                break;
                }
                 testMasse = neueMasse;
             }
             alteMasse = neueMasse;
             neueMasse = MasseMitGenauigkeit();
             if((neueMasse-becher) >= ausgabe){
             puts(" im Becher sind ");
             intOutput(neueMasse-becher);
             puts(" Gramm \n");
             ausgabe = neueMasse-becher+1;
             }
             if (neueMasse < 1 || (alteMasse+6) < neueMasse || (alteMasse-6) > neueMasse){        // Falls Becher weg genommen wird oder Gewichtsveränderung zu groß ist
             pump_stop();
             puts("Fehler: Gewichtsveränderung zu groß \n");
             break;
             }
            }
            pump_stop();
            puts("Vorgang abgeschlossen \n\n");
        } else{
            pump_stop();
            puts("Fehler: Gewichtsveränderung an Becher zu groß \n");
        }
    }
    return 0;
}



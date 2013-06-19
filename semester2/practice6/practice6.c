/* Abfüllanlage
 *
 * Arne Bahlo
 * Johannes Wahl
 *
 * 19. Juni 2013
 *
 */

#include "pio.h"
#include "pmc.h"
#include "aic.h"
#include "tc.h"

#define TC4_INIT  TC_CLKS_MCK2 | TC_LDBSTOP | TC_CAPT | TC_LDRA_RISING_EDGE | TC_LDRB_RISING_EDGE // TimerClock für Timer 4
#define TC5_INIT  TC_CLKS_MCK2 | TC_LDBSTOP | TC_CAPT | TC_LDRA_RISING_EDGE | TC_LDRB_RISING_EDGE // TimerClock für Timer 5
#define PIOB_PER	((volatile unsigned int *) 0xFFFF0000)	// Port Enable Register
#define PIOB_OER	((volatile unsigned int *) 0xFFFF0010)	// Output enable Register
#define PIOB_SODR	((volatile unsigned int *) 0xFFFF0030)	// Set output Data Register
#define PIOB_CODR	((volatile unsigned int *) 0xFFFF0034)	// Clear output Register

void taste_irq_handler(void) __attribute__((interrupt));

// Initialisierungsfunktionen
void keys_init(void) {
    *PIOB_PER = 0x100; // aktiviere Register LED's und Taster an PortB
    *PIOB_OER = 0x100; // aktiviere das output-enable Register
    StructAIC* aicbase = AIC_BASE; // Basisiadresse für AIC
    StructPMC* pmcbase = PMC_BASE; // Basisiadresse für PMC
    // Basisadresse des PMC
    StructPIO* piobaseB = PIOB_BASE; // Basisadresse PIO B
    pmcbase->PMC_PCER = 0x4200; // Peripheral Clocks aktiv für PIOB,

    //piobaseB->PIO_PER = LED1 | LED2;
    //piobaseB->PIO_OER = LED1 | LED2;

    piobaseB->PIO_PER = KEY1 | KEY2; // KEY1&2 PinEnable
    piobaseB->PIO_OER = KEY1 | KEY2; // KEY1&2 OutputEnable

    aicbase->AIC_IDCR = 0x4000;
    aicbase->AIC_ICCR = 0x4000;

    aicbase-> AIC_SVR[14] = taste_irq_handler;
    aicbase->AIC_IECR = 0x4000;

    piobaseB->PIO_IER = ALL_KEYS;
}

void pio_init(void) {
    StructPMC* pmcbase = PMC_BASE;
    StructPIO* piobaseA = PIOA_BASE;
    pmcbase->PMC_PCER = 0x06f80; // Clock PIOA, PIOB, Timer5, Timer4, Timer1 einschalten
    piobaseA->PIO_PDR = 0x090;
}

void timer_init(void) {
    StructTC* timerbase3 = TCB3_BASE; // Basisadressse TC Block 1
    StructPIO* piobaseA = PIOA_BASE; // Basisadresse PIO B

    timerbase3->TC_CCR = TC_CLKDIS; // Disable Clock

    // Initialize the mode of the timer 3
    timerbase3->TC_CMR =
            TC_ACPC_CLEAR_OUTPUT | //ACPC    : Register C clear TIOA
            TC_ACPA_SET_OUTPUT | //ACPA    : Register A set TIOA
            TC_WAVE | //WAVE    : Waveform mode
            TC_CPCTRG | //CPCTRG  : Register C compare trigger enable
            TC_CLKS_MCK8; //TCCLKS  : MCKI / 1024

    // Initialize the counter:
    timerbase3->TC_RA = 31250; //positive Taktflanke
    timerbase3->TC_RC = 62500; //negative Taktflanke

    // Start the timer :
    timerbase3->TC_CCR = TC_CLKEN; //Clockenable
    timerbase3->TC_CCR = TC_SWTRG; //Softwaretrigger intitialisieren
    piobaseA->PIO_PER = (1 << PIOTIOA3); //PinEnableReg
    piobaseA->PIO_OER = (1 << PIOTIOA3); //OutputEnableReg
    piobaseA->PIO_CODR = (1 << PIOTIOA3); //CurrentOutputDataReg

    StructTC* tcbase4 = TCB4_BASE;
    StructTC* tcbase5 = TCB5_BASE;

    // Timer 4 Initialisierung
    tcbase4->TC_CCR = TC_CLKDIS;
    tcbase4->TC_CMR = TC4_INIT;
    tcbase4->TC_CCR = TC_CLKEN;
    tcbase4->TC_CCR = TC_SWTRG;

    // Timer 5 Initialisierung
    tcbase5->TC_CCR = TC_CLKDIS;
    tcbase5->TC_CMR = TC5_INIT;
    tcbase5->TC_CCR = TC_CLKEN;
    tcbase5->TC_CCR = TC_SWTRG;
}

// Pumpe
void pumpeAn(void) {
    StructPIO* piobaseA = PIOA_BASE; // Basisadresse PIO A
    piobaseA->PIO_PDR = (1 << PIOTIOA3); // PinDisableReg für Pumpenpin
}

void pumpeAus(void) {
    StructPIO* piobaseA = PIOA_BASE; // Basisadresse PIO A
    piobaseA->PIO_PER = (1 << PIOTIOA3); // PinEnableReg für Pumpenpin
}

// Waage
int MessungderMassemitGenauigkeit(volatile int genauigkeit) {
    volatile int counter = 0;
    volatile int summe = 0;
    volatile int current = 0;
    while (counter < genauigkeit) {
        current = MessungderMasse();
        summe = summe + current;
        ++counter;
    }
    return abs(summe / genauigkeit); // Durschnitt von mehreren Messungen zurückgeben
}

int MessungderMasse(void) {
    volatile int Masse;
    volatile int captureRA1;
    volatile int captureRB1;
    volatile int capturediff1;
    volatile float Periodendauer1;

    volatile int captureRA2;
    volatile int captureRB2;
    volatile int capturediff2;
    volatile float Periodendauer2;

    StructPMC* pmcbase = PMC_BASE;
    StructPIO* piobaseA = PIOA_BASE;
    StructPIO* piobaseB = PIOB_BASE;
    StructTC* tcbase4 = TCB4_BASE;
    StructTC* tcbase5 = TCB5_BASE;

    tcbase4->TC_SR;
    tcbase5->TC_SR;
    tcbase4->TC_CCR = TC_SWTRG; // Timer 4 Softwaretrigger
    tcbase5->TC_CCR = TC_SWTRG; // Timer 5 Softwaretrigger

    while (!(tcbase4->TC_SR & 0x40)); // Capture Register B wurde geladen Messung abgeschlossen
    captureRA1 = tcbase4->TC_RA;
    captureRB1 = tcbase4->TC_RB;
    capturediff1 = abs(captureRB1) - abs(captureRA1);
    Periodendauer1 = abs(capturediff1); // Zeit in microsek

    while (!(tcbase5->TC_SR & 0x40)); // Capture Register A wurde geladen Messung abgeschlossen
    captureRA2 = tcbase5->TC_RA;
    captureRB2 = tcbase5->TC_RB;
    capturediff2 = abs(captureRB2) - abs(captureRA2);
    Periodendauer2 = abs(capturediff2); // Zeit in microsek

    Masse = 2000 * ((Periodendauer2 / Periodendauer1) - 1) - 0; // Masseberechnung

    if (Masse < 0) return 0; // Wenn Masse kleiner als Null ist, Null zurückgeben
    return Masse;
}

// Interrupt requests
void taste_irq_handler(void) {
    StructPIO* piobaseA = PIOA_BASE; // Basisadresse PIO A
    StructPIO* piobaseB = PIOB_BASE; // Basisadresse PIO B
    StructAIC* aicbase = AIC_BASE; // Basisadresse AIC
    /*if (!(piobaseB->PIO_PDSR & KEY1)) {
        piobaseA->PIO_PDR = (1 << PIOTIOA3);
    }

    if (!(piobaseB->PIO_PDSR & KEY2)) {
        piobaseA->PIO_PER = (1 << PIOTIOA3);
    }*/
    aicbase->AIC_EOICR = piobaseB->PIO_ISR;
}

// Terminal
void puti(int i) { // Integerausgabe
    char s[12];
    int count = 10;
    int t = 0;
    int tmp = 0;
    if (i < 0) {
        putc('-');
        i = i*-1;
    }
    if (i == 0) {
        putc('0');
    }
    while (i != 0) {
        tmp = i % count;
        s[t] = tmp + 0x30;
        t++;
        i = i / 10;
    }
    t--;
    for (t; t >= 0; t--) {
        putc(s[t]);
    }
}


// Ablauf des Programmes (Abfuellvorgang)
void programm_init() { // Aufruf der Initialisierungsfunktionen
    inits();
    pio_init();
    timer_init();
    keys_init();
}
void programm_start() { // Start des Programms
    StructPIO* piobaseB = PIOB_BASE;
    // Begüßung über die serielle Schnittstelle
    puts("Hallo! \nStellen Sie einen Bechher auf die Abfuellanlage. Danach drüchen Sie die Taste 1 um den Abfüllvorgang zu starten. \n");

    while (1) {
        if (!(piobaseB->PIO_PDSR & KEY1)) {
            programm_pumpen();
        }
    }
}
void programm_pumpen() {
    volatile int Masse = 0;
    volatile int BecherGewicht = MessungderMassemitGenauigkeit(3);
    puts("Das Gewicht de Bechers betraegt: ");
    puti(BecherGewicht);
    puts(" Gramm \n");
    volatile int MasseAlt = 0;
	puts("Betaetigen Sie Taste 2 um den Abfuellvorgang zu starten.\n");
    while (1) {
        if (!(piobaseB->PIO_PDSR & KEY2)) {
            pumpeAn();
            for (Masse = MessungderMassemitGenauigkeit(3) - BecherGewicht; Masse < 50; Masse = MessungderMassemitGenauigkeit(3) - BecherGewicht) { // Abfuellen von 50 ml
                if (Masse + 4 < MasseAlt) {
                    pumpeAus();
                    puts("Der Becher wurde entfernt, Vorgang abgebrochen!\n");
                    programm_start();
                    break;
                }
                if (MessungderMassemitGenauigkeit(3) > 999) {
                    pumpeAus();
                    puts("Das zulässige Maximalgewicht für die Waage ist erreicht. Der Pumpvorgang wurde abgebrochen!\n");
                    programm_start();
                    break;
                }
                MasseAlt = Masse;
            }
            pumpeAus();
            puts("Der Abfuellvorgang wurde beendet.\n");
            // programm_start();
        }
    }
}

int main(void) {
    programm_init(); // Aufruf der Initialisierungen
    programm_start(); // Start des Programms
    return 0;
}

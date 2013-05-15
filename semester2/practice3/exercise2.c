// Lösung zu Termin3
// Aufgabe 1
// von:
// vom: 
// 

#include "../h/pmc.h"
#include "../h/tc.h"
#include "../h/pio.h"
#include "../h/aic.h"

void taste_irq_handler (void) __attribute__ ((interrupt));

// PUMPIN
void pumpe_init(void) 
{
  StructPIO* piobaseA  = PIOA_BASE;	// Basisadresse PIO A
  piobaseA->PIO_OER  = (1<<PIOTIOA3) ;	//enable individiual i/0 signals
  piobaseA->PIO_CODR = (1<<PIOTIOA3) ;	//pin is used as output, high or low level 
}

// PUMPEN
void pumpe_on(void) 
{
  StructPIO* piobaseA  = PIOA_BASE;	// Basisadresse PIO A
  piobaseA->PIO_PDR  = (1<<PIOTIOA3) ;
}

// PUMPDIS
void pumpe_off(void) 
{
  StructPIO* piobaseA  = PIOA_BASE;	// Basisadresse PIO A
  piobaseA->PIO_CODR = (1<<PIOTIOA3) ;	//pin is used as output, high or low level 
  piobaseA->PIO_PER  = (1<<PIOTIOA3) ;
}

// Interruptserviceroutine für die Tasten SW1 und SW2
void taste_irq_handler (void)
{
  StructPIO* piobaseB   = PIOB_BASE;	// Basisadresse PIO B
  StructPIO* piobaseA  = PIOA_BASE;	// Basisadresse PIO A
  StructAIC* aicbase  = AIC_BASE;	//__
	
  // KEYS
  piobaseB->PIO_PER = KEY1 | KEY2 | KEY3; // Port enable
  piobaseB->PIO_IER = KEY1 | KEY2 | KEY3; // Input enable
      
  if( !( piobaseB->PIO_PDSR & KEY1 ) ) {
      // Key 1 gedrÃ¼ckt, Pumpe aus
      pumpe_off();
  } else if( !( piobaseB->PIO_PDSR & KEY2 ) ) {
      // Key 2 gedrÃ¼ckt, PUMPE an
      pumpe_on();
  }

   aicbase->AIC_EOICR = piobaseB->PIO_ISR;	//end of interrupt command register(handles priorities), updates value of interrupt status register
}

// Timer3 initialisieren
void Timer3_init( void )
{
  StructTC* timerbase3  = TCB3_BASE;	// Basisadressse TC Block 1
  StructPIO* piobaseA   = PIOA_BASE;	// Basisadresse PIO A

  timerbase3->TC_CCR = TC_CLKDIS;	// Disable timer, Channel Control Register 
 
  // Initialize the mode of the timer 3
  timerbase3->TC_CMR =
    TC_ACPC_CLEAR_OUTPUT  |    //ACPC    : Register C clear TIOA
    TC_ACPA_SET_OUTPUT    |    //ACPA    : Register A set TIOA
    TC_WAVE               |    //WAVE    : Waveform mode
    TC_CPCTRG             |    //CPCTRG  : Register C compare trigger enable
    TC_CLKS_MCK8;           //TCCLKS  : MCKI / 1024

  // Initialize the counter:
  timerbase3->TC_RA = 31250;	// cmp register a
  timerbase3->TC_RC = 62500;	// cmp register c
  
  //TC_CCR: Software trigger, resets counter and starts counter clock

  // Start the timer :
  timerbase3->TC_CCR = TC_CLKEN ;	// Enable timer again  
  timerbase3->TC_CCR = TC_SWTRG ;	//set software trigger
 
  pumpe_init();
  pumpe_on();
}

int main(void)
{

	StructPMC* pmcbase	= PMC_BASE;	// Basisadresse des PMC
	StructPIO* piobaseA   	= PIOA_BASE;	// Basisadresse PIO A
	StructPIO* piobaseB   	= PIOB_BASE;	// Basisadresse PIO B
	StructAIC* aicbase 	= AIC_BASE; 	// Basisadresse AIC

	pmcbase->PMC_PCER	= 0x6200;	// Peripheral Clocks einschalten für PIOB, PIOA, Timer3
	  
        // Interrupt initialisieren
	aicbase->AIC_IDCR = 0x4000;
	aicbase->AIC_ICCR = 0x4000;
	aicbase->AIC_SMR[14] = AIC_PRIOR;
	aicbase->AIC_SVR[14] = taste_irq_handler;
	aicbase->AIC_IECR = 0x4000;
	
	piobaseB->PIO_IER = ALL_KEYS;

	// Timer initialisieren
	Timer3_init();
	
	// Programm Beendung verhindern
	while(1)
	{
			
	}
	
	return 0;
}

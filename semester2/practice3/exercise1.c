// Lösung zu Termin3
// Aufgabe 1
//

#include "../h/pmc.h"
#include "../h/tc.h"
#include "../h/pio.h"
#include "../h/aic.h"

void taste_irq_handler (void) __attribute__ ((interrupt));

// Interruptserviceroutine für die Tasten SW1 und SW2
void taste_irq_handler (void)
{
  StructPIO* piobaseB   = PIOB_BASE;		// Basisadresse PIO B
  StructAIC* aicbase  = AIC_BASE;		//__

  aicbase->AIC_EOICR = piobaseB->PIO_ISR;	//end of interrupt command register(handles priorities), updates value of interrupt status register
}

// Timer3 initialisieren
void Timer3_init( void )
{
  StructTC* timerbase3  = TCB3_BASE;	// Basisadressse TC Block 1
  StructPIO* piobaseA   = PIOA_BASE;	// Basisadresse PIO A

  timerbase3->TC_CCR	= TC_CLKDIS;	// Disable Clock

  // Initialize the mode of the timer 3
  timerbase3->TC_CMR =
    TC_ACPC_CLEAR_OUTPUT  |    //ACPC    : Register C clear TIOA
    TC_ACPA_SET_OUTPUT    |    //ACPA    : Register A set TIOA
    TC_WAVE               |    //WAVE    : Waveform mode
    TC_CPCTRG             |    //CPCTRG  : Register C compare trigger enable
    TC_CLKS_MCK1024;           //TCCLKS  : MCKI / 1024

  // Initialize the counter:
  timerbase3->TC_RA = 244;	// cmp register a
  timerbase3->TC_RC = 488;	// cmp register c

  //TC_CCR: Software trigger, resets counter and starts counter clock

  // Start the timer :
  timerbase3->TC_CCR = TC_CLKEN ;	//enable clock
  timerbase3->TC_CCR = TC_SWTRG ;	//set software trigger

  piobaseA->PIO_OER  = (1<<PIOTIOA3) ;	//enable individiual i/0 signals
  piobaseA->PIO_CODR = (1<<PIOTIOA3) ;	//pin is used as output, high or low level
  piobaseA->PIO_PDR  = (1<<PIOTIOA3) ;	//disable
}

int main(void)
{

	StructPMC* pmcbase	= PMC_BASE;	// Basisadresse des PMC
	StructPIO* piobaseA   	= PIOA_BASE;	// Basisadresse PIO A
	StructPIO* piobaseB   	= PIOB_BASE;	// Basisadresse PIO B

	// Clock kram
	pmcbase->PMC_PCER	= 0x4000;	// Peripheral Clocks einschalten für PIOB, _____
	pmcbase->PMC_PCER	= 0x200; 	// Clock für PMC auch einschalten!

	// Timer initialisieren
	Timer3_init();


	while(1)
	{

	}

	return 0;
}

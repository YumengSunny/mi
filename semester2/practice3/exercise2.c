/**
 * Provides a signal for the pump
 * Key 1 stops pumpin
 * Key 2 starts pumpin
 */

#include "../h/pmc.h"
#include "../h/tc.h"
#include "../h/pio.h"
#include "../h/aic.h"

void taste_irq_handler (void) __attribute__ ((interrupt));

/**
  * Initializes pump
  */
void pump_init(void)
{
  StructPIO* piobaseA  = PIOA_BASE;	    // Base address PIOA (for pump pins)
  piobaseA->PIO_OER  = (1<<PIOTIOA3);	  // Output enable
  piobaseA->PIO_CODR = (1<<PIOTIOA3);	  // Clear
}

/**
  * Start pump
  */
void pump_start(void)
{
  StructPIO* piobaseA  = PIOA_BASE;	    // Base address PIOA (for pump pins)
  piobaseA->PIO_PDR  = (1<<PIOTIOA3);   // Port disable (switch to pump)
}

/**
  * Stop pump
  */
void pump_stop(void)
{
  StructPIO* piobaseA  = PIOA_BASE;	    // Base address PIOA (for pump pins)
  piobaseA->PIO_CODR = (1<<PIOTIOA3);	  // Clear (set to low) to prevent permanent high
  piobaseA->PIO_PER  = (1<<PIOTIOA3);   // Port enable (switch to PIOB)
}

/**
 * Interrupt
 */
void taste_irq_handler (void)
{
  StructPIO* piobaseB   = PIOB_BASE;	  // Base address PIOB (for controls)
  StructPIO* piobaseA  = PIOA_BASE;	    // Base address PIOA (for pump pins)
  StructAIC* aicbase  = AIC_BASE;	      // Base address AIC (for interrupts)

  piobaseB->PIO_PER = KEY1 | KEY2 | KEY3; // Port enable Keys
  piobaseB->PIO_IER = KEY1 | KEY2 | KEY3; // Input enable keys

  if( !( piobaseB->PIO_PDSR & KEY1 ) ) {
      // Key 1 was pressed, stop pump
      pump_stop();
  } else if( !( piobaseB->PIO_PDSR & KEY2 ) ) {
      // Key 2 was pressed, start pump again
      pump_start();
  }

   aicbase->AIC_EOICR = piobaseB->PIO_ISR;	// End of interrupt command register (handles priorities), updates value of interrupt status register
}

/**
 * Initialize timer
 */
void timer3_init( void )
{
  StructTC* timerbase3  = TCB3_BASE;	// Base address TC Block 1
  StructPIO* piobaseA   = PIOA_BASE;	// Base address PIOA (for pump pins)

  timerbase3->TC_CCR = TC_CLKDIS;	// Disable timer, Channel Control Register

  // Initialize the mode of the timer 3
  timerbase3->TC_CMR =
    TC_ACPC_CLEAR_OUTPUT  |    //ACPC    : Register C clear TIOA
    TC_ACPA_SET_OUTPUT    |    //ACPA    : Register A set TIOA
    TC_WAVE               |    //WAVE    : Waveform mode
    TC_CPCTRG             |    //CPCTRG  : Register C compare trigger enable
    TC_CLKS_MCK8;              //TCCLKS  : MCKI / 1024

  // Initialize the counter:
  timerbase3->TC_RA = 31250;	// cmp register a
  timerbase3->TC_RC = 62500;	// cmp register c

  // Start the timer :
  timerbase3->TC_CCR = TC_CLKEN;	// Enable timer again
  timerbase3->TC_CCR = TC_SWTRG;	// Set software trigger

  pump_init();
  pump_start();
}

int main(void)
{

	StructPMC* pmcbase	= PMC_BASE;  // Base address PMC
	StructPIO* piobaseA = PIOA_BASE; // Base address PIOA (for pump pins)
	StructPIO* piobaseB = PIOB_BASE; // Base address PIO B
	StructAIC* aicbase  = AIC_BASE;  // Base address AIC

	pmcbase->PMC_PCER	= 0x6200;	// Turn peripheral clocks on for PIOB, PIOA, Timer3

  // Initialize interrupt
	aicbase->AIC_IDCR = 0x4000;
	aicbase->AIC_ICCR = 0x4000;
	aicbase->AIC_SMR[14] = AIC_PRIOR;
	aicbase->AIC_SVR[14] = taste_irq_handler;
	aicbase->AIC_IECR = 0x4000;
	piobaseB->PIO_IER = ALL_KEYS;

	// Initialize timer
	timer3_init();

	// Prevent exit
	while(1)
	{

	}

	return 0;
}

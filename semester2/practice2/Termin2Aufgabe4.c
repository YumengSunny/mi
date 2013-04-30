// Lösung zu Termin2
// Aufgabe 4
// Namen: __________; ___________
// Matr.: __________; ___________
// vom :  __________
// 

#include "pio.h"
#include "aic.h"

#include "pmc.h"
#include "ser_io.h"
#include "std_c.h"
#include "tc.h"
#include "usart.h"

void taste_irq_handler (void) __attribute__ ((interrupt));

void taste_irq_handler (void)
{
  StructPIO* piobaseB = PIOB_BASE; // Basisdadresse PIO B
  StructAIC* aicbase = AIC_BASE;   // Basisadresse AIC
  
  if( !(piobaseB->PIO_PDSR & KEY1)) { // Pin status register abfragen
    piobaseB->PIO_CODR = LED1; // Clear output data = LED an
  }
  if ( !( piobaseB->PIO_PDSR & KEY2 )) {
    piobaseB->PIO_SODR = LED1; // Set output data = LED aus
  }
  
  // Interrupt beenden
  aicbase->AIC_EOICR = piobaseB->PIO_ISR;
}

int main(void)
{
	StructPIO* piobaseB = PIOB_BASE; // Basisdadresse PIO B
	StructAIC* aicbase = AIC_BASE;   // Basisadresse AIC
  
        // Interrupt initialisieren
	aicbase->AIC_IDCR = 0x4000; // Disable PIOB
	aicbase->AIC_ICCR = 0x4000; // Clear interrupt Register PIOB
	
	aicbase->AIC_SMR[14] = AIC_PRIOR; // Höchste Priorität 
	aicbase->AIC_SVR[14] = taste_irq_handler; // Wenn interrupt, taste_irq_handler() aufrufen
	aicbase->AIC_IECR = 0x4000; // Aktiviert PIOB wieder
	
	piobaseB->PIO_IER = ALL_KEYS; // Aktiviert alle Tasten
	
	// Schleife
	int x = 0;
	while (1) {
	  for( x = 1; x < 250000; x++)
	      piobaseB->PIO_SODR = LED2; // Set Ouput data register von LED2 = LED aus
	  for( x = 1; x < 250000; x++)
	      piobaseB->PIO_CODR = LED2; // Clear Output data register von LED2 = LED an
	}
	
	return 0;
}


// Lösung zu Termin2
// Aufgabe 2
// Namen: __________; ___________
// Matr.: __________; ___________
// vom :  __________
// 

#include "pio.h"
#include "aic.h"
#include "pmc.h"

int main(void)
{
 	StructPIO* piobaseB = PIOB_BASE; // Basisdadresse 
	piobaseB->PIO_PER = LED1|KEY1|KEY2;	// aktiviere Register LED's und Taster an PortB
	piobaseB->PIO_OER = LED1;// Otput enable auf LED 
	piobaseB->PIO_ODR = KEY1|KEY2; // Input enable auf KEYs
	
	StructPMC* pmcbase = PMC_BASE; // Clock erstellen (nicht unbedingt notwendig)
	pmcbase->PMC_PCER = 0x4000; // PIOB auf 1 setzen im PMC
	
	
	// Schleife
	while (1) {
  		if( !( piobaseB->PIO_PDSR & KEY1 ) ) { // Pin status register abfragen
  		  piobaseB->PIO_CODR = LED1; // Set output data = LED an
  		}
  		if ( !( piobaseB->PIO_PDSR & KEY2 ) ) {
		    piobaseB->PIO_SODR = LED1; // Clear output data = LED aus
  		}
	}
	
	return 0;
}

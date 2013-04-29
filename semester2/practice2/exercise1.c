// Lösung zu Termin2
// Aufgabe 1
// Namen: __________; ___________
// Matr.: __________; ___________
// vom :  __________
// 

int main(void)
{
// Beispiel des Anlegens und der Nutzung einer Zeigervariablen
#define PIOB_PER	((volatile unsigned int *) 0xFFFF0000)
#define PIOB_OER	((volatile unsigned int *) 0xFFFF0010)
#define PIOB_SODR	((volatile unsigned int *) 0xFFFF0030)
#define PIOB_CODR	((volatile unsigned int *) 0xFFFF0034)

 	*PIOB_PER = 0x100;	// aktiviere Register LED's und Taster an PortB
	*PIOB_OER = 0x100;      // Output enable
	
	*PIOB_SODR = 0x100;
	*PIOB_CODR = 0x100;
	*PIOB_SODR = 0x100;
	*PIOB_CODR = 0x100;
	*PIOB_SODR = 0x100;
	*PIOB_CODR = 0x100;
	*PIOB_SODR = 0x100;
	*PIOB_CODR = 0x100;

	return 0;
}

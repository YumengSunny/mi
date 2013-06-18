// Programmrahmen zur Aufgabe Termin5
// Aufgabe 1
//************************************
// 
// von: Manfred Pester
// vom: 06. August 2003
// letzte Änderung:	30. November 2004
// von: Manfred Pester

void intOutput (int var)
{

long int a = 10;
int b = 0;
char carray[22];
										
if(var < 0){			//prüfen ob var kleiner 0, wenn ja, ein minus in das array schreiben und var mit minus 1 multiplizieren
	var = var * -1;
	carray[0]='-';
	b=b+1;
}

while((var % a) != var){	//a so oft mit 10 multiplizieren, bis var%a = 0
	a=a*10;
}
a=a/10;

while(a>0){			//var stellenweise in das array schreiben
carray[b]=(var/a+48);
var = var - ((var/a)*a);
a = a/10;
b = b+1;
}
carray[b]='\0';			//array "abschließen"
puts(carray);			//array ausgeben
}


int main(void)
{
//	char i;
// Serielle initialisieren  
	inits();
	init_ser();
// CR und LF auf das Terminal ausgeben
	putc (0xd);
	putc (0xa);
// ein Zeichen von der seriellen abholen	
//	i=getc();
	putc('a');
// String ausgeben
	puts("Hallo! \n");
	
//	intOutput(0x80000);	
	intOutput(0x80000000); // diese zahl max mit 20 stellen (- zeichen zählt als stelle), falls mehr benötigt, muss im quellcode die größe von "carray" entsprechend geändert werden
	
	return 0;
}

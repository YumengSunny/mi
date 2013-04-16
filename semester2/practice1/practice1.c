// Lösung zu Termin1
// Aufgabe 1
// Namen: ___________; ____________
// Matr.: ___________; ____________
// vom:   ___________

int a = 0x1;

int addition(int* e, int* f, int* g) { 
	return *e + *f + *g;
}

int main (void)
{
	int b = 0x3;
	int c = 0x4;
	
	int* d = &b;
	int* h = &c;
	int* i = &a;
	
	int e = addition(d, h, i);
	
	return (0);
	
}

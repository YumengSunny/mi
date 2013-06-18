@----------------------------------------------------------------------------
@ File Name           : ser_io.S
@ Object              : Ein- Ausgabe-Funktionen der seriellen Schnittstelle
@			welche ueber den Supervisor-Mode gehen
@			
@
@ Namen :		Matr.-Nr.:
@			Matr.-Nr.:
@
@----------------------------------------------------------------------------

@ Debuginformationen
	.file	"ser_io.S"

@ Funktion
	.text
	.align	2
	.global	inits
	.type	inits,function
inits:
	swi	0x100
	bx	lr	@ Rücksprung

@ Funktion
	.text
	.align	2
	.global	putc
	.type	inits,function
putc:
	mov	r1, r0		@ Zeichen (char) nach r1
	ldr	r0, =Zeichen	@ Zeiger holen
	str	r1, [r0]	@ Zeichen (char) unter Zeiger ablegen
	swi 	0x200		@ 
	ldr	r1, =Zeichen	@ Zeiger holen
	ldr	r0, [r1]	@ Zeichen (char) aus Zeiger holen
	bx 	lr

@ Funktion
	.text
	.align	2
	.global	getc
	.type	inits,function
getc:
	ldr 	r0, =Zeichen	@ Zeiger holen
	swi 	0x300		
	ldr	r0, =Zeichen	@ Zeiger holen		
	ldr 	r0, [r0]	@ empfangenes Zeichen zurueck geben
	bx 	lr
	
@ Funktion
	.text
	.align	2
	.global	puts
	.type	puts,function
puts:
	stmfd sp!,{lr}	@ Retten der Register

	mov	r1, r0		@ Zeichen (char) nach r1
	ldr	r0, =Zeichen	@ Zeiger holen
	
Start:
	ldrb	r2, [r1]	@ Läd erstes Zeichen des Strings in r2
	cmp	r2, #0		@ Prüft ob string zu ende
	beq	Ende		@ Falls string zu ende springe zu Ende
	cmp	r2, #0x0a
	beq	CarriageReturn
	str	r2, [r0]	@ Zeichen (char) unter Zeiger ablegen
	swi 	0x200		@ Gibt Zeichen aus
	ldr	r2, [r0]	@ Speichert das vom SWI zurückgegebene Zeichen in r2
	cmp	r2, #0		@ Prüft ob SWI rückgabe 0 war, also swi noch nicht fertig oder fehlerhaft
	beq	Start		@ Springt zurück an den Start
	addne	r1, r1, #1
	b	Start
	
CarriageReturn: @ Cursor an den Anfang der Zeile setzen
	mov	r3, #0xd
	str	r3, [r0]	@ Zeichen (char) unter Zeiger ablegen
	swi 	0x200
	ldr	r3, [r0]	@ Speichert das vom SWI zurückgegebene Zeichen in r2
	cmp	r3, #0
	beq	CarriageReturn
	b	Linefeed
	
Linefeed: @ Absatz
	mov	r3, #0xa
	str	r3, [r0]	@ Zeichen (char) unter Zeiger ablegen
	swi 	0x200
	ldr	r3, [r0]	@ Speichert das vom SWI zurückgegebene Zeichen in r2
	cmp	r3, #0
	beq	Linefeed
	addne	r1, r1, #1
	b	Start
	
Ende:

	ldmfd sp!,{pc}	@ Rücksprung
	
@ Funktion
	.text
	.align	2
	.global	puti
	.type	puti,function
puti:
	stmfd sp!,{lr}	@ Retten der Register

	mov	r1, r0		@ Zeichen (char) nach r1
	ldr	r0, =Zeichen	@ Zeiger holen
	
IStart:
	str	r1, [r0]	@ Zeichen (char) unter Zeiger ablegen
	swi 	0x200		@ Gibt Zeichen aus
	ldr	r2, [r0]	@ Speichert das vom SWI zurückgegebene Zeichen in r2
	cmp	r2, #0		@ Prüft ob SWI rückgabe 0 war, also swi noch nicht fertig oder fehlerhaft
	beq	IStart		@ Springt zurück an den Start
	ldmfd sp!,{pc}	@ Rücksprung

@ Funktion
	.text
	.align	2
	.global	gets
	.type	gets,function
gets:
	stmfd	sp!,{lr}	@ Retten der Register
	
// Hier könnte Ihr Code eingefügt werden!

	ldmfd 	sp!,{pc}	@ Rücksprung
	
	.data
Zeichen:	.word 0			

.end

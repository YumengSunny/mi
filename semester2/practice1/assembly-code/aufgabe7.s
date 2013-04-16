	.file	"Termin1Aufgabe1.c"
	.global	a
	.data
	.align	2
	.type	a, %object
	.size	a, 4
a:
	.word	1
	.text
	.align	2
	.global	addition
	.type	addition, %function
addition:
	@ args = 0, pretend = 0, frame = 12
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	
	str	r0, [fp, #-4]
	str	r1, [fp, #-8]
	str	r2, [fp, #-12]
	
	ldr	r3, [fp, #-4]
	ldr	r2, [r3, #0]
	
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #0]

	add	r2, r2, r3
	
	ldr	r3, [fp, #-12]
	ldr	r3, [r3, #0]
	
	add	r3, r2, r3
	
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	addition, .-addition
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #24
	
	
	mov	r3, #3
	str	r3, [fp, #-24]
	
	mov	r3, #4
	str	r3, [fp, #-28]
	
	sub	r3, fp, #24
	str	r3, [fp, #-20]
	
	sub	r3, fp, #28
	str	r3, [fp, #-16]
	
	ldr	r3, .L5
	str	r3, [fp, #-12]
	
	ldr	r0, [fp, #-20]
	ldr	r1, [fp, #-16]
	ldr	r2, [fp, #-12]
	
	bl	addition
	mov	r3, r0
	str	r3, [fp, #-8]
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
.L6:
	.align	2
.L5:
	.word	a
	.size	main, .-main
	.ident	"GCC: (crosstool-NG 1.14.1) 4.4.6"
	
	
	
	
	
1
	.file	"Termin1Aufgabe1.c"
	.text
	.align	2
	.global	addition
	.type	addition, %function
addition:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r1, #0]
	ldr	r1, [r0, #0]
	add	r3, r3, r1
	ldr	r0, [r2, #0]
	add	r0, r3, r0
	bx	lr
	.size	addition, .-addition
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r0, #0
	bx	lr
	.size	main, .-main
	.global	a
	.data
	.align	2
	.type	a, %object
	.size	a, 4
a:
	.word	1
	.ident	"GCC: (crosstool-NG 1.14.1) 4.4.6"


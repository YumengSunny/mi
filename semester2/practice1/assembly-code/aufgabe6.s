	.file	"practice1.c"
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

	ldr	r2, [fp, #-4]
	ldr	r3, [fp, #-8]

	add	r2, r2, r3
	ldr	r3, [fp, #-12]
	add	r3, r2, r3

	mov	r0, r3 @gib Ergebnis zurück
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	addition, .-addition
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 12
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #12

	mov	r3, #3
	str	r3, [fp, #-16]

	mov	r3, #4
	str	r3, [fp, #-12]

	ldr	r3, .L5
	ldr	r3, [r3, #0]

	mov	r0, r3
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

 	.file	"practice1.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]! @ fp = sp = sp - 4
	add	fp, sp, #0 @ fp = sp
	sub	sp, sp, #8 @ sp = sp -8
	mov	r3, #1 @ r3 = 1
	str	r3, [fp, #-8] @ fp-8 = [r3]
	mov	r3, #2 @ r3 = 2
	str	r3, [fp, #-4] @ fp-4 = [r3]
	mov	r3, #0 @ r3 = 0
	mov	r0, r3 @ r0 = r3
	add	sp, fp, #0 @ sp = fp + 0
	ldmfd	sp!, {fp} @ load sp to fp and increment
	bx	lr
	.size	main, .-main
	.ident	"GCC: (crosstool-NG 1.14.1) 4.4.6"

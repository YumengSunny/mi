	.file	"seriell.c"
	.text
	.align	2
	.global	init_ser
	.type	init_ser, %function
init_ser:
	@ args = 0, pretend = 0, frame = 12
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	mov	r3, #-1342177280
	mov	r3, r3, asr #14
	str	r3, [fp, #-12]
	mov	r3, #-1610612736
	mov	r3, r3, asr #15
	str	r3, [fp, #-8]
	mov	r3, #-2147483648
	mov	r3, r3, asr #13
	str	r3, [fp, #-4]
	ldr	r3, [fp, #-8]
	mov	r2, #4
	str	r2, [r3, #16]
	ldr	r3, [fp, #-12]
	mov	r2, #98304
	str	r2, [r3, #4]
	ldr	r3, [fp, #-4]
	mov	r2, #160
	str	r2, [r3, #0]
	ldr	r3, [fp, #-4]
	mov	r2, #41
	str	r2, [r3, #32]
	ldr	r3, [fp, #-4]
	mov	r2, #2240
	str	r2, [r3, #4]
	ldr	r3, [fp, #-4]
	mov	r2, #80
	str	r2, [r3, #0]
	mov	r3, #0
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	init_ser, .-init_ser
	.align	2
	.global	putch
	.type	putch, %function
putch:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #8
	mov	r3, r0
	strb	r3, [fp, #-8]
	mov	r3, #-2147483648
	mov	r3, r3, asr #13
	str	r3, [fp, #-4]
	ldr	r3, [fp, #-4]
	ldr	r3, [r3, #20]
	and	r3, r3, #2
	cmp	r3, #0
	beq	.L4
	ldrb	r2, [fp, #-8]	@ zero_extendqisi2
	ldr	r3, [fp, #-4]
	str	r2, [r3, #28]
	ldrb	r3, [fp, #-8]	@ zero_extendqisi2
	b	.L5
.L4:
	mov	r3, #0
.L5:
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	putch, .-putch
	.align	2
	.global	getch
	.type	getch, %function
getch:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #8
	mov	r3, #-2147483648
	mov	r3, r3, asr #13
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #20]
	and	r3, r3, #1
	and	r3, r3, #255
	cmp	r3, #0
	beq	.L8
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #24]
	and	r3, r3, #255
	b	.L9
.L8:
	mov	r3, #0
.L9:
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	getch, .-getch
	.ident	"GCC: (crosstool-NG 1.14.1) 4.4.6"

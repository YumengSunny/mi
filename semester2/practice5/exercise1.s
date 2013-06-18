	.file	"Termin5Aufgabe1.c"
	.global	__modsi3
	.global	__divsi3
	.text
	.align	2
	.global	intOutput
	.type	intOutput, %function
intOutput:
	@ args = 0, pretend = 0, frame = 36
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #36
	str	r0, [fp, #-44]
	mov	r3, #10
	str	r3, [fp, #-16]
	mov	r3, #0
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-44]
	cmp	r3, #0
	bge	.L8
	ldr	r3, [fp, #-44]
	rsb	r3, r3, #0
	str	r3, [fp, #-44]
	mov	r3, #45
	strb	r3, [fp, #-40]
	ldr	r3, [fp, #-12]
	add	r3, r3, #1
	str	r3, [fp, #-12]
	b	.L3
.L4:
	ldr	r2, [fp, #-16]
	mov	r3, r2
	mov	r3, r3, asl #2
	add	r3, r3, r2
	mov	r3, r3, asl #1
	str	r3, [fp, #-16]
	b	.L3
.L8:
	mov	r0, r0	@ nop
.L3:
	ldr	r3, [fp, #-44]
	mov	r0, r3
	ldr	r1, [fp, #-16]
	bl	__modsi3
	mov	r3, r0
	mov	r2, r3
	ldr	r3, [fp, #-44]
	cmp	r2, r3
	bne	.L4
	ldr	r3, [fp, #-16]
	ldr	r1, .L9
	smull	r0, r2, r1, r3
	mov	r2, r2, asr #2
	mov	r3, r3, asr #31
	rsb	r3, r3, r2
	str	r3, [fp, #-16]
	b	.L5
.L6:
	ldr	r4, [fp, #-12]
	ldr	r0, [fp, #-44]
	ldr	r1, [fp, #-16]
	bl	__divsi3
	mov	r3, r0
	and	r3, r3, #255
	add	r3, r3, #48
	and	r2, r3, #255
	mvn	r3, #31
	sub	r0, fp, #8
	add	r1, r0, r4
	add	r3, r1, r3
	strb	r2, [r3, #0]
	ldr	r3, [fp, #-44]
	mov	r0, r3
	ldr	r1, [fp, #-16]
	bl	__modsi3
	mov	r3, r0
	str	r3, [fp, #-44]
	ldr	r3, [fp, #-16]
	ldr	r1, .L9
	smull	r0, r2, r1, r3
	mov	r2, r2, asr #2
	mov	r3, r3, asr #31
	rsb	r3, r3, r2
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-12]
	add	r3, r3, #1
	str	r3, [fp, #-12]
.L5:
	ldr	r3, [fp, #-16]
	cmp	r3, #0
	bgt	.L6
	ldr	r2, [fp, #-12]
	mvn	r3, #31
	sub	r1, fp, #8
	add	r2, r1, r2
	add	r3, r2, r3
	mov	r2, #0
	strb	r2, [r3, #0]
	sub	r3, fp, #40
	mov	r0, r3
	bl	puts
	sub	sp, fp, #8
	ldmfd	sp!, {r4, fp, pc}
.L10:
	.align	2
.L9:
	.word	1717986919
	.size	intOutput, .-intOutput
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Hallo! \012\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	bl	inits
	bl	init_ser
	mov	r0, #13
	bl	putc
	mov	r0, #10
	bl	putc
	mov	r0, #97
	bl	putc
	ldr	r0, .L13
	bl	puts
	mov	r0, #524288
	bl	intOutput
	mov	r3, #0
	mov	r0, r3
	ldmfd	sp!, {fp, pc}
.L14:
	.align	2
.L13:
	.word	.LC0
	.size	main, .-main
	.ident	"GCC: (crosstool-NG 1.14.1) 4.4.6"

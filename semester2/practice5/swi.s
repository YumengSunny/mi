	.file	"swi.c"
	.text
	.align	2
	.global	SWIHandler
	.type	SWIHandler, %function
SWIHandler:
	@ ARM Exception Handler.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r0, r1, r2, r3, r4, fp, ip, lr}
	add	fp, sp, #28
	mov	r3, lr
	sub	r3, r3, #4
	ldr	r3, [r3, #0]
	bic	r3, r3, #-16777216
	cmp	r3, #512
	beq	.L4
	cmp	r3, #768
	beq	.L5
	cmp	r3, #256
	bne	.L6
.L3:
	bl	init_ser
	b	.L6
.L4:
	mov	r4, r0
	mov	r3, r0
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	mov	r0, r3
	bl	putch
	mov	r3, r0
	and	r3, r3, #255
	strb	r3, [r4, #0]
	b	.L6
.L5:
	mov	r4, r0
	bl	getch
	mov	r3, r0
	and	r3, r3, #255
	strb	r3, [r4, #0]
.L6:
	sub	sp, fp, #28
	ldmfd	sp!, {r0, r1, r2, r3, r4, fp, ip, pc}^
	.size	SWIHandler, .-SWIHandler
	.ident	"GCC: (crosstool-NG 1.14.1) 4.4.6"

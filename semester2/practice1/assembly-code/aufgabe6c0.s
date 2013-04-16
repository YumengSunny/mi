    .file   "practice1.c"
    .text
    .align  2
    .global addition
    .type   addition, %function
addition:
    @ args = 0, pretend = 0, frame = 0
    @ frame_needed = 0, uses_anonymous_args = 0
    @ link register save eliminated.
    add r0, r1, r0
    add r0, r0, r2
    bx  lr
    .size   addition, .-addition
    .align  2
    .global main
    .type   main, %function
main:
    @ args = 0, pretend = 0, frame = 0
    @ frame_needed = 0, uses_anonymous_args = 0
    @ link register save eliminated.
    mov r0, #0
    bx  lr
    .size   main, .-main
    .global a
    .data
    .align  2
    .type   a, %object
    .size   a, 4
a:
    .word   1
    .ident  "GCC: (crosstool-NG 1.14.1) 4.4.6"

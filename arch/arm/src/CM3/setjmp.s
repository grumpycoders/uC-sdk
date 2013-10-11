.cpu cortex-m3
.syntax unified

.section ".text.setjmp","x",%progbits
.align 2
.thumb
.thumb_func
.globl setjmp
.type setjmp, %function
setjmp:
.fnstart
    mov ip, sp
    stmea a1!, {v1-v7, fp, ip, lr}
    mov a1, #0
    bx lr
.fnend
.size setjmp,.-setjmp

.section ".text.longjmp","x",%progbits
.align 2
.thumb
.thumb_func
.globl longjmp
.type longjmp, %function
longjmp:
.fnstart
    ldmfd a1!, {v1-v7, fp, ip, lr}
    mov sp, ip
    movs a1, a2
    it eq
    moveq a1, #1
.fnend
.size longjmp,.-longjmp

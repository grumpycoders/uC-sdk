.cpu cortex-m4
.syntax unified
.thumb

.section ".text.Semihost_SYS_OPEN", "x", %progbits
.thumb_func
.globl Semihost_SYS_OPEN
.type Semihost_SYS_OPEN, %function
Semihost_SYS_OPEN:
    mov r1, r0
    mov r0, #1
    bkpt 0xAB
    bx lr

.section ".text.Semihost_SYS_CLOSE", "x", %progbits
.thumb_func
.globl Semihost_SYS_CLOSE
.type Semihost_SYS_CLOSE, %function
Semihost_SYS_CLOSE:
    mov r1, r0
    mov r0, #2
    bkpt 0xAB
    bx lr

.section ".text.Semihost_SYS_WRITE", "x", %progbits
.thumb_func
.globl Semihost_SYS_WRITE
.type Semihost_SYS_WRITE, %function
Semihost_SYS_WRITE:
    mov r1, r0
    mov r0, #5
    bkpt 0xAB
    bx lr

.section ".text.Semihost_SYS_READ", "x", %progbits
.thumb_func
.globl Semihost_SYS_READ
.type Semihost_SYS_READ, %function
Semihost_SYS_READ:
    mov r1, r0
    mov r0, #6
    bkpt 0xAB
    bx lr

.section ".text.Semihost_SYS_SEEK", "x", %progbits
.thumb_func
.globl Semihost_SYS_SEEK
.type Semihost_SYS_SEEK, %function
Semihost_SYS_SEEK:
    mov r1, r0
    mov r0, #10
    bkpt 0xAB
    bx lr

.section ".text.Semihost_SYS_FLEN", "x", %progbits
.thumb_func
.globl Semihost_SYS_FLEN
.type Semihost_SYS_FLEN, %function
Semihost_SYS_FLEN:
    mov r1, r0
    mov r0, #12
    bkpt 0xAB
    bx lr

.end

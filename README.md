uC-sdk - a lightweight development kit for embedded micro-controllers
=====================================================================

This SDK revolves around FreeRTOS in order to provide basic support for
unix-like environment, in a real time multitasked-enabled fashion.

Currently, the only supported target is the LPX1768 CPU, a Cortex-M3 ARM,
which is available on the mbed board (http://mbed.org/), but I hope in
getting more platforms supported in the future. The next target in line
would be the PIC32, on its development board form.

The main goal is to get the cleanest architecture as possible, providing
the same, clean API to all the supported CPUs.


Architecture / Layout
---------------------

RTOS:

The FreeRTOS directory contains the FreeRTOS source code. The main
modification done there is a few minor bug fixes, and adding the support
to the _impure_ptr switching support for our libc's reent system. The current
memory management is done using the libc's allocator.

Architectures:

The arch directory contains architecture-specific code, including assembly
code, drivers and link scripts.

System:

The os directory contains more generic code, in order to boot the CPU, set up
your binary's environment, provide the necessary glue to libc, and set up
a proper shutdown sequence.

libc:

The current libc implementation is excessively minimal. Even more minimal than
uClibc that is. The goal is to provide bare-metal features while still having
a minimum of portability.

libm:

Right now, libm is Sun's fdlibm, compiled in IEEE mode (ie, no error handling
is being done, unlike a normal libm). Be careful though, as most uC processors
dont contain any FPU. Using any kind of math will greatly increase the code
size, as well as CPU usage while processing.

acorn:

This will be the library containing utilities for embedded development, such as
generic i2c access, malloc wrapping for RTOS multithreading, etc.


ARM / Cortex-M3 / mbed
----------------------

The arch directory contains NXP's port of CMSIS for their LPX1768 CPU. Using
their drivers is highly recommended.

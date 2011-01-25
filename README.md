uC-sdk - a lightweight development kit for embedded micro-controllers
=====================================================================

This SDK revolves around FreeRTOS and newlib in order to provide basic
support for unix-like environment, in a real time multitasked-enabled
fashion.

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
to the _impure_ptr switching support for newlib's reent system. The current
memory management is done using newlib's allocator.

Architectures:

The arch directory contains architecture-specific code, including assembly
code, drivers and link scripts.

System:

The os directory contains more generic code, in order to boot the CPU, set up
your binary's environment, provide the necessary glue to newlib, and set up
a proper shutdown sequence.


ARM / Cortex-M3 / mbed
----------------------

The arch directory contains NXP's port of CMSIS for their LPX1768 CPU. Using
their drivers is highly recommended.

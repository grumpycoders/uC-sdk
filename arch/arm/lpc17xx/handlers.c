#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <osdebug.h>
#include "LPC17xx.h"

void BoardExceptionHandler(int code);

enum FaultType {
    NMI = 2, HardFault, MemManage, BusFault, UsageFault,
};

struct fault_data_cpu_t {
    uint32_t r0, r1, r2, r3, r12, lr, pc, xPSR;
};

struct fault_data_extra_t {
    uint32_t r4, r5, r6, r7, r8, r9, r10, r11;
};

static void print_fault_data(struct fault_data_cpu_t * fault_data_cpu, struct fault_data_extra_t * fault_data_extra) {
    // These ones are saved naturally by the CPU
    DBGOUT("fd_cpu = %p - fd_extra = %p\r\n", fault_data_cpu, fault_data_extra);
    DBGOUT("pc:   %p\r\n", fault_data_cpu->pc);
    DBGOUT("sp  = %p - lr  = %p - r12 = %p -xPSR = %p\r\n", fault_data_cpu + 1, fault_data_cpu->lr, fault_data_cpu->r12, fault_data_cpu->xPSR);
    DBGOUT("r0  = %p - r1  = %p - r2  = %p - r3  = %p\r\n", fault_data_cpu->r0, fault_data_cpu->r1, fault_data_cpu->r2, fault_data_cpu->r3);
    // These ones are saved by the ASM handler; hence the reason they are before in memory.
    DBGOUT("r4  = %p - r5  = %p - r6  = %p - r7  = %p\r\n", fault_data_extra->r4, fault_data_extra->r5, fault_data_extra->r6, fault_data_extra->r7);
    DBGOUT("r8  = %p - r9  = %p - r10 = %p - r11 = %p\r\n", fault_data_extra->r8, fault_data_extra->r9, fault_data_extra->r10, fault_data_extra->r11);
}

void general_C_handler(enum FaultType fault, struct fault_data_extra_t * fault_data_extra, uintptr_t lr) {
    uintptr_t eflags = lr & 15;
    struct fault_data_cpu_t * fault_data_cpu = (struct fault_data_cpu_t *) ((eflags & 4) ? (void *) __get_PSP() : (void *) (fault_data_extra + 1));
    DBGOUT("***FAULT***\r\neflags = 0x0%x\r\nPSP = %p\r\nType: ", eflags, __get_PSP());
    switch (fault) {
    case NMI:
        DBGOUT("NMI\r\n");
        break;
    case HardFault:
        DBGOUT("HardFault\r\nHFSR: %p\r\n", SCB->HFSR);
        break;
    case MemManage:
        DBGOUT("MemManage\r\nCSFR: %p\r\nMMFAR: %p\r\n", SCB->CFSR, SCB->MMFAR);
        break;
    case BusFault:
        DBGOUT("BusFault\r\nCFSR: %p\r\nBFAR: %p\r\n", SCB->CFSR, SCB->BFAR);
        break;
    case UsageFault:
        DBGOUT("UsageFault\r\nCFSR: %p\r\nBFAR: %p\r\n", SCB->CFSR, SCB->BFAR);
        break;
    default:
        DBGOUT("Unknown\r\n");
    }
    
    print_fault_data(fault_data_cpu, fault_data_extra);
    
    DBGOUT("HALTING\r\n");
    BoardExceptionHandler(-1);
    while(1);
}

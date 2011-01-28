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

struct fault_data_t {
    uint32_t r4, r5, r6, r7, r8, r9, r10, r11, r0, r1, r2, r3, r12, lr, pc, xPSR;
};

static void print_fault_data(struct fault_data_t * fault_data1, struct fault_data_t * fault_data2) {
    // These ones are saved naturally by the CPU
    DBGOUT("pc:   %p\r\n", fault_data1->pc);
    DBGOUT("sp  = %p - lr  = %p - r12 = %p -xPSR = %p\r\n", fault_data1 + 1, fault_data1->lr, fault_data1->r12, fault_data1->xPSR);
    DBGOUT("r0  = %p - r1  = %p - r2  = %p - r3  = %p\r\n", fault_data1->r0, fault_data1->r1, fault_data1->r2, fault_data1->r3);
    // These ones are saved by the ASM handler; hence the reason they are before in memory.
    DBGOUT("r4  = %p - r5  = %p - r6  = %p - r7  = %p\r\n", fault_data2->r4, fault_data2->r5, fault_data2->r6, fault_data2->r7);
    DBGOUT("r8  = %p - r9  = %p - r10 = %p - r11 = %p\r\n", fault_data2->r8, fault_data2->r9, fault_data2->r10, fault_data2->r11);
}

void general_C_handler(enum FaultType fault, struct fault_data_t * fault_data2) {
    uintptr_t eflags = ((uintptr_t) __builtin_return_address(0)) & 15;
    struct fault_data_t * fault_data1 = (eflags & 4 ? fault_data2 : (struct fault_data_t *) __get_MSP());
    DBGOUT("***FAULT***\r\nType: ");
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
    
    print_fault_data(fault_data1, fault_data2);
    
    DBGOUT("HALTING\r\n");
    BoardExceptionHandler(-1);
    while(1);
}

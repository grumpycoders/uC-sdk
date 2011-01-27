#include <osdebug.h>
#include "LPC17xx.h"

enum FaultType {
    NMI = 2, HardFault, MemManage, BusFault, UsageFault,
};

struct fault_data_t {
    uint32_t r0, r1, r2, r3, r12, lr, pc, xPSR, r4, r5, r6, r7, r8, r9, r10, r11;
};

static void print_fault_data(struct fault_data_t * fault_data) {
    // These ones are saved naturally by the CPU
    DBGOUT("pc:   %p\r\n", fault_data->pc);
    DBGOUT("sp  = %p - lr  = %p - r12 = %p -xPSR = %p\r\n", fault_data, fault_data->lr, fault_data->r12, fault_data->xPSR);
    DBGOUT("r0  = %p - r1  = %p - r2  = %p - r3  = %p\r\n", fault_data->r0, fault_data->r1, fault_data->r2, fault_data->r3);
    // These ones are saved by the ASM handler; not sure if they are proper
    DBGOUT("r4  = %p - r5  = %p - r6  = %p - r7  = %p\r\n", fault_data->r4, fault_data->r5, fault_data->r6, fault_data->r7);
    //  Huh ? This read crashes the CPU... (cf previous comment)
//  DBGOUT("r8  = %p - r9  = %p - r10 = %p - r11 = %p\r\n", fault_data->r8, fault_data->r9, fault_data->r10, fault_data->r11);
}

void general_C_handler(enum FaultType fault, struct fault_data_t * fault_data) {
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
    }
    
    print_fault_data(fault_data);
    
    DBGOUT("HALTING\r\n");
    while(1);
}

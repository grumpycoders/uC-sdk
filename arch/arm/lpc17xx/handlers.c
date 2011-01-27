#include <osdebug.h>
#include "LPC17xx.h"

enum FaultType {
    NMI, HardFault, MemManage, BusFault, UsageFault,
};

static inline void traceme() {
    unsigned int i;
    void * frame, * pc;
    
    i = 0;
    frame = __builtin_frame_address(i);
    pc = frame ? __builtin_return_address(i) : 0;
    DBGOUT("lv%u; PC = %p, frame = %p\r\n", i, pc, frame);
}

static int general_Handler(enum FaultType fault) {
    DBGOUT("***FAULT***\r\n");
    
    traceme();
    
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
    
    while(1);
    
    return 0;
}

int NMI_Handler() { return general_Handler(NMI); }
int HardFault_Handler() { return general_Handler(HardFault); }
int MemManage_Handler() { return general_Handler(MemManage); }
int BusFault_Handler() { return general_Handler(BusFault); }
int UsageFault_Handler() { return general_Handler(UsageFault); }

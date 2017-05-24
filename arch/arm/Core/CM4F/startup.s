/*****************************************************************************/
/* startup_LPC17xx.s: Startup file for LPC17xx device series                 */
/*****************************************************************************/
/* Version: CodeSourcery Sourcery G++ Lite (with CS3)                        */
/*****************************************************************************/


/*
//*** <<< Use Configuration Wizard in Context Menu >>> ***
*/

    .cpu cortex-m4
    .syntax unified


/*
// <h> Stack Configuration
//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

    .equ    Stack_Size, 0x00000100
    .section ".stack", "w"
    .align  3
    .globl  __cs3_stack_mem
    .globl  __stack_start
    .globl  __cs3_stack_size
__cs3_stack_mem:
__stack_start:
    .if     Stack_Size
    .space  Stack_Size
    .endif
    .size   __cs3_stack_mem,  . - __cs3_stack_mem
    .set    __cs3_stack_size, . - __cs3_stack_mem


/*
// <h> Heap Configuration
//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

    .equ    Heap_Size,  0x00001000

    .section ".heap", "w"
    .align  3
    .globl  __cs3_heap_start
    .globl  __cs3_heap_end
__cs3_heap_start:
    .if     Heap_Size
    .space  Heap_Size
    .endif
__cs3_heap_end:


/* Vector Table */
    .section ".cs3.interrupt_vector_mutable"
    .globl  __cs3_interrupt_vector_cortex_m_mutable
    .type   __cs3_interrupt_vector_cortex_m_mutable, %object

__cs3_interrupt_vector_cortex_m_mutable:
    .long   __cs3_stack                   /* 0x0000 0000 Top of Stack                                          */
    .long   __cs3_reset_cortex_m          /* 0x0000 0004 Reset Handler                                         */
    .long   NMI_Handler                   /* 0x0000 0008 NMI Handler                                           */
    .long   HardFault_Handler             /* 0x0000 000C Hard Fault Handler                                    */
    .long   MemManage_Handler             /* 0x0000 0010 MPU Fault Handler                                     */
    .long   BusFault_Handler              /* 0x0000 0014 Bus Fault Handler                                     */
    .long   UsageFault_Handler            /* 0x0000 0018 Usage Fault Handler                                   */
    .long   0                             /* 0x0000 001C Reserved                                              */
    .long   0                             /* 0x0000 0020 Reserved                                              */
    .long   0                             /* 0x0000 0024 Reserved                                              */
    .long   0                             /* 0x0000 0028 Reserved                                              */
    .long   SVC_Handler                   /* 0x0000 002C SVCall Handler                                        */
    .long   DebugMon_Handler              /* 0x0000 0030 Debug Monitor Handler                                 */
    .long   0                             /* 0x0000 0034 Reserved                                              */
    .long   PendSV_Handler                /* 0x0000 0038 PendSV Handler                                        */
    .long   SysTick_Handler               /* 0x0000 003C SysTick Handler                                       */

    /* External Interrupts */
    .long   WDT_IRQHandler                /* 0x0000 0040 Watchdog Timer                                        */
    .long   PVD_IRQHandler                /* 0x0000 0044 PVD through EXTI line detection                       */
    .long   TAMP_STAMP_IRQHandler         /* 0x0000 0048 Tamper and TimeStamp interrupts through the EXTI line */
    .long   RTC_WKUP_IRQHandler           /* 0x0000 004C RTC Wakeup interrupt through the EXTI line            */
    .long   FLASH_IRQHandler              /* 0x0000 0050 FLASH                                                          */       
    .long   RCC_IRQHandler                /* 0x0000 0054 RCC                                                   */
    .long   EXTI0_IRQHandler              /* 0x0000 0058 EXTI Line0                                            */
    .long   EXTI1_IRQHandler              /* 0x0000 005C EXTI Line1                                            */
    .long   EXTI2_IRQHandler              /* 0x0000 0060 EXTI Line2                                            */
    .long   EXTI3_IRQHandler              /* 0x0000 0064 EXTI Line3                                            */
    .long   EXTI4_IRQHandler              /* 0x0000 0068 EXTI Line4                                            */
    .long   DMA1_Stream0_IRQHandler       /* 0x0000 006C DMA1 Stream 0                                         */
    .long   DMA1_Stream1_IRQHandler       /* 0x0000 0070 DMA1 Stream 1                                         */ 
    .long   DMA1_Stream2_IRQHandler       /* 0x0000 0074 DMA1 Stream 2                                         */ 
    .long   DMA1_Stream3_IRQHandler       /* 0x0000 0078 DMA1 Stream 3                                         */ 
    .long   DMA1_Stream4_IRQHandler       /* 0x0000 007C DMA1 Stream 4                                         */ 
    .long   DMA1_Stream5_IRQHandler       /* 0x0000 0080 DMA1 Stream 5                                         */ 
    .long   DMA1_Stream6_IRQHandler       /* 0x0000 0084 DMA1 Stream 6                                         */ 
    .long   ADC_IRQHandler                /* 0x0000 0088 ADC1, ADC2 and ADC3s                                  */ 
    .long   CAN1_TX_IRQHandler            /* 0x0000 008C CAN1 TX                                               */       
    .long   CAN1_RX0_IRQHandler           /* 0x0000 0090 CAN1 RX0                                              */
    .long   CAN1_RX1_IRQHandler           /* 0x0000 0094 CAN1 RX1                                              */
    .long   CAN1_SCE_IRQHandler           /* 0x0000 0098 CAN1 SCE                                              */
    .long   EXTI9_5_IRQHandler            /* 0x0000 009C External Line[9:5]s                                   */
    .long   TIM1_BRK_TIM9_IRQHandler      /* 0x0000 00A0 TIM1 Break and TIM9                                   */
    .long   TIM1_UP_TIM10_IRQHandler      /* 0x0000 00A4 TIM1 Update and TIM10                                 */
    .long   TIM1_TRG_COM_TIM11_IRQHandler /* 0x0000 00A8 TIM1 Trigger and Commutation and TIM11                */
    .long   TIM1_CC_IRQHandler            /* 0x0000 00AC TIM1 Capture Compare                                  */
    .long   TIM2_IRQHandler               /* 0x0000 00B0 TIM2                                                  */ 
    .long   TIM3_IRQHandler               /* 0x0000 00B4 TIM3                                                  */ 
    .long   TIM4_IRQHandler               /* 0x0000 00B8 TIM4                                                  */ 
    .long   I2C1_EV_IRQHandler            /* 0x0000 00BC I2C1 Event                                            */
    .long   I2C1_ER_IRQHandler            /* 0x0000 00C0 I2C1 Error                                            */
    .long   I2C2_EV_IRQHandler            /* 0x0000 00C4 I2C2 Event                                            */
    .long   I2C2_ER_IRQHandler            /* 0x0000 00C8 I2C2 Error                                            */  
    .long   SPI1_IRQHandler               /* 0x0000 00CC SPI1                                                  */ 
    .long   SPI2_IRQHandler               /* 0x0000 00D0 SPI2                                                  */ 
    .long   USART1_IRQHandler             /* 0x0000 00D4 USART1                                                */ 
    .long   USART2_IRQHandler             /* 0x0000 00D8 USART2                                                */ 
    .long   USART3_IRQHandler             /* 0x0000 00DC USART3                                                */ 
    .long   EXTI15_10_IRQHandler          /* 0x0000 00E0 External Line[15:10]s                                 */
    .long   RTC_Alarm_IRQHandler          /* 0x0000 00E4 RTC Alarm (A and B) through EXTI Line                 */
    .long   OTG_FS_WKUP_IRQHandler        /* 0x0000 00E8 USB OTG FS Wakeup through EXTI line                   */     
    .long   TIM8_BRK_TIM12_IRQHandler     /* 0x0000 00EC TIM8 Break and TIM12                                  */
    .long   TIM8_UP_TIM13_IRQHandler      /* 0x0000 00F0 TIM8 Update and TIM13                                 */
    .long   TIM8_TRG_COM_TIM14_IRQHandler /* 0x0000 00F4 TIM8 Trigger and Commutation and TIM14                */
    .long   TIM8_CC_IRQHandler            /* 0x0000 00F8 TIM8 Capture Compare                                  */
    .long   DMA1_Stream7_IRQHandler       /* 0x0000 00FC DMA1 Stream7                                          */
    .long   FSMC_IRQHandler               /* 0x0000 0100 FSMC                                                  */ 
    .long   SDIO_IRQHandler               /* 0x0000 0104 SDIO                                                  */ 
    .long   TIM5_IRQHandler               /* 0x0000 0108 TIM5                                                  */ 
    .long   SPI3_IRQHandler               /* 0x0000 010C SPI3                                                  */ 
    .long   UART4_IRQHandler              /* 0x0000 0110 UART4                                                 */ 
    .long   UART5_IRQHandler              /* 0x0000 0114 UART5                                                 */ 
    .long   TIM6_DAC_IRQHandler           /* 0x0000 0118 TIM6 and DAC1&2 underrun errors                       */ 
    .long   TIM7_IRQHandler               /* 0x0000 011C TIM7                                                  */
    .long   DMA2_Stream0_IRQHandler       /* 0x0000 0120 DMA2 Stream 0                                         */ 
    .long   DMA2_Stream1_IRQHandler       /* 0x0000 0124 DMA2 Stream 1                                         */ 
    .long   DMA2_Stream2_IRQHandler       /* 0x0000 0128 DMA2 Stream 2                                         */ 
    .long   DMA2_Stream3_IRQHandler       /* 0x0000 012C DMA2 Stream 3                                         */ 
    .long   DMA2_Stream4_IRQHandler       /* 0x0000 0130 DMA2 Stream 4                                         */ 
    .long   ETH_IRQHandler                /* 0x0000 0134 Ethernet                                              */ 
    .long   ETH_WKUP_IRQHandler           /* 0x0000 0138 Ethernet Wakeup through EXTI line                     */   
    .long   CAN2_TX_IRQHandler            /* 0x0000 013C CAN2 TX                                               */
    .long   CAN2_RX0_IRQHandler           /* 0x0000 0140 CAN2 RX0                                              */
    .long   CAN2_RX1_IRQHandler           /* 0x0000 0144 CAN2 RX1                                              */
    .long   CAN2_SCE_IRQHandler           /* 0x0000 0148 CAN2 SCE                                              */
    .long   OTG_FS_IRQHandler             /* 0x0000 014C USB OTG FS                                            */
    .long   DMA2_Stream5_IRQHandler       /* 0x0000 0150 DMA2 Stream 5                                         */ 
    .long   DMA2_Stream6_IRQHandler       /* 0x0000 0154 DMA2 Stream 6                                         */ 
    .long   DMA2_Stream7_IRQHandler       /* 0x0000 0158 DMA2 Stream 7                                         */ 
    .long   USART6_IRQHandler             /* 0x0000 015C USART6                                                */  
    .long   I2C3_EV_IRQHandler            /* 0x0000 0160 I2C3 event                                            */
    .long   I2C3_ER_IRQHandler            /* 0x0000 0164 I2C3 error                                            */
    .long   OTG_HS_EP1_OUT_IRQHandler     /* 0x0000 0168 USB OTG HS End Point 1 Out                            */ 
    .long   OTG_HS_EP1_IN_IRQHandler      /* 0x0000 016C USB OTG HS End Point 1 In                             */ 
    .long   OTG_HS_WKUP_IRQHandler        /* 0x0000 0170 USB OTG HS Wakeup through EXTI                        */       
    .long   OTG_HS_IRQHandler             /* 0x0000 0174 USB OTG HS                                            */ 
    .long   DCMI_IRQHandler               /* 0x0000 0178 DCMI                                                  */ 
    .long   CRYP_IRQHandler               /* 0x0000 017C CRYP crypto                                           */ 
    .long   HASH_RNG_IRQHandler           /* 0x0000 0180 Hash and Rng                                          */
    .long   FPU_IRQHandler                /* 0x0000 0184 FPU                                                   */

    .size   __cs3_interrupt_vector_cortex_m_mutable, . - __cs3_interrupt_vector_cortex_m_mutable


    .section ".cs3.interrupt_vector"
    .globl  __cs3_interrupt_vector_cortex_m
    .type   __cs3_interrupt_vector_cortex_m, %object

__cs3_interrupt_vector_cortex_m:
    .long   __cs3_stack                   /* Top of Stack                 */
    .long   __cs3_reset_cortex_m          /* Reset Handler                */
    .long   NMI_Handler                   /* NMI Handler                  */
    .long   HardFault_Handler             /* Hard Fault Handler           */
    .long   MemManage_Handler             /* MPU Fault Handler            */
    .long   BusFault_Handler              /* Bus Fault Handler            */
    .long   UsageFault_Handler            /* Usage Fault Handler          */
    .long   0                             /* Reserved - ROM CRC check ?   */


    .size   __cs3_interrupt_vector_cortex_m, . - __cs3_interrupt_vector_cortex_m


    .thumb

/* Fault handlers wrappers */

    .section .handlers,"x",%progbits
    .thumb_func
    .type   NMI_Handler, %function
NMI_Handler:
    MOV     R0, 2
    B       general_handler
    .type   HardFault_Handler, %function
HardFault_Handler:
    MOV     R0, 3
    B       general_handler
    .type   MemManage_Handler, %function
MemManage_Handler:
    MOV     R0, 4
    B       general_handler
    .type   BusFault_Handler, %function
BusFault_Handler:
    MOV     R0, 5
    B       general_handler
    .type   UsageFault_Handler, %function
UsageFault_Handler:
    MOV     R0, 6
    .type   general_handler, %function
general_handler:
    PUSH    {R4-R11}
    MOV     R1, SP
    MOV     R2, LR
    MOV     R4, LR
    BL      general_C_handler
    AND     R0, #7
    ORR     LR, R4, R0
    POP     {R4-R11}
    BX      LR

/* Reset Handler */

    .section .cs3.reset,"x",%progbits
    .thumb_func
    .globl  __cs3_reset_cortex_m
    .type   __cs3_reset_cortex_m, %function
__cs3_reset_cortex_m:
    .fnstart

    LDR     R0, =__rom_data_begin
    LDR     R1, =__rom_data_end
    LDR     R2, =__ram_data_begin

    MOV     R3, R0
    SUBS    R4, R1, R0
    MOV     R1, R3
    MOV     R0, R2
    MOV     R2, R4
    BL      startup_memcpy

    LDR     R0, =__bss_ram_begin
    MOV     R1, #0
    LDR     R2, =__bss_ram_len
    BL      startup_memset

/* Enable FPU */
    LDR.W   R0, =0xE000ED88
    LDR     R1, [R0]
    ORR     R1, R1, #(0xF << 20)
    STR     R1, [R0]
    DSB
    ISB

    B       _ucsdk_start
    .pool
    .cantunwind
    .fnend
    .size   __cs3_reset_cortex_m,.-__cs3_reset_cortex_m

    .section .handlers,"x",%progbits


    .weak   DebugMon_Handler
    .type   DebugMon_Handler, %function
DebugMon_Handler:
    B       .
    .size   DebugMon_Handler, . - DebugMon_Handler


/* IRQ Handlers */

    .globl  Default_Handler
    .type   Default_Handler, %function
Default_Handler:
    B       .
    .size   Default_Handler, . - Default_Handler

    .macro  IRQ handler
    .weak   \handler
    .set    \handler, Default_Handler
    .endm


    IRQ   WDT_IRQHandler
    IRQ   PVD_IRQHandler
    IRQ   TAMP_STAMP_IRQHandler
    IRQ   RTC_WKUP_IRQHandler
    IRQ   FLASH_IRQHandler
    IRQ   RCC_IRQHandler
    IRQ   EXTI0_IRQHandler
    IRQ   EXTI1_IRQHandler
    IRQ   EXTI2_IRQHandler
    IRQ   EXTI3_IRQHandler
    IRQ   EXTI4_IRQHandler
    IRQ   DMA1_Stream0_IRQHandler
    IRQ   DMA1_Stream1_IRQHandler
    IRQ   DMA1_Stream2_IRQHandler
    IRQ   DMA1_Stream3_IRQHandler
    IRQ   DMA1_Stream4_IRQHandler
    IRQ   DMA1_Stream5_IRQHandler
    IRQ   DMA1_Stream6_IRQHandler
    IRQ   ADC_IRQHandler
    IRQ   CAN1_TX_IRQHandler
    IRQ   CAN1_RX0_IRQHandler
    IRQ   CAN1_RX1_IRQHandler
    IRQ   CAN1_SCE_IRQHandler
    IRQ   EXTI9_5_IRQHandler
    IRQ   TIM1_BRK_TIM9_IRQHandler
    IRQ   TIM1_UP_TIM10_IRQHandler
    IRQ   TIM1_TRG_COM_TIM11_IRQHandler
    IRQ   TIM1_CC_IRQHandler
    IRQ   TIM2_IRQHandler
    IRQ   TIM3_IRQHandler
    IRQ   TIM4_IRQHandler
    IRQ   I2C1_EV_IRQHandler
    IRQ   I2C1_ER_IRQHandler
    IRQ   I2C2_EV_IRQHandler
    IRQ   I2C2_ER_IRQHandler
    IRQ   SPI1_IRQHandler
    IRQ   SPI2_IRQHandler
    IRQ   USART1_IRQHandler
    IRQ   USART2_IRQHandler
    IRQ   USART3_IRQHandler
    IRQ   EXTI15_10_IRQHandler
    IRQ   RTC_Alarm_IRQHandler
    IRQ   OTG_FS_WKUP_IRQHandler
    IRQ   TIM8_BRK_TIM12_IRQHandler
    IRQ   TIM8_UP_TIM13_IRQHandler
    IRQ   TIM8_TRG_COM_TIM14_IRQHandler
    IRQ   TIM8_CC_IRQHandler
    IRQ   DMA1_Stream7_IRQHandler
    IRQ   FSMC_IRQHandler
    IRQ   SDIO_IRQHandler
    IRQ   TIM5_IRQHandler
    IRQ   SPI3_IRQHandler
    IRQ   UART4_IRQHandler
    IRQ   UART5_IRQHandler
    IRQ   TIM6_DAC_IRQHandler
    IRQ   TIM7_IRQHandler
    IRQ   DMA2_Stream0_IRQHandler
    IRQ   DMA2_Stream1_IRQHandler
    IRQ   DMA2_Stream2_IRQHandler
    IRQ   DMA2_Stream3_IRQHandler
    IRQ   DMA2_Stream4_IRQHandler
    IRQ   ETH_IRQHandler
    IRQ   ETH_WKUP_IRQHandler
    IRQ   CAN2_TX_IRQHandler
    IRQ   CAN2_RX0_IRQHandler
    IRQ   CAN2_RX1_IRQHandler
    IRQ   CAN2_SCE_IRQHandler
    IRQ   OTG_FS_IRQHandler
    IRQ   DMA2_Stream5_IRQHandler
    IRQ   DMA2_Stream6_IRQHandler
    IRQ   DMA2_Stream7_IRQHandler
    IRQ   USART6_IRQHandler
    IRQ   I2C3_EV_IRQHandler
    IRQ   I2C3_ER_IRQHandler
    IRQ   OTG_HS_EP1_OUT_IRQHandler
    IRQ   OTG_HS_EP1_IN_IRQHandler
    IRQ   OTG_HS_WKUP_IRQHandler
    IRQ   OTG_HS_IRQHandler
    IRQ   DCMI_IRQHandler
    IRQ   CRYP_IRQHandler
    IRQ   HASH_RNG_IRQHandler
    IRQ   FPU_IRQHandler


    .end

#include "uart.h"

#include <FreeRTOS.h>
#include <task.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

#include <stdio.h>

struct uartInitDef_t {
    USART_TypeDef * id;
    // tx / rx
    GPIO_TypeDef * tdef[2];
    uint16_t gpio[2];
    uint16_t pinsource[2];
    // uart / gpio
    volatile uint32_t * bridge[2];
    uint32_t peripheral[2];
    uint8_t af;
};

static struct uartInitDef_t uartInitDefs[8] = {
    { USART1, { GPIOA, GPIOA}, { GPIO_Pin_9, GPIO_Pin_10 },   //UART1
      {GPIO_PinSource9, GPIO_PinSource10}, {&RCC->APB2ENR, &RCC->AHB1ENR}, 
      {RCC_APB2Periph_USART1, RCC_AHB1Periph_GPIOA }, GPIO_AF_USART1 },
    { USART2, { GPIOA, GPIOA }, { GPIO_Pin_2, GPIO_Pin_3 },   // UART2
      {GPIO_PinSource2, GPIO_PinSource3}, {&RCC->APB1ENR, &RCC->AHB1ENR}, 
      {RCC_APB1Periph_USART2, RCC_AHB1Periph_GPIOA }, GPIO_AF_USART2 },
    { USART3, { GPIOB, GPIOB }, { GPIO_Pin_10, GPIO_Pin_11 }, // UART3
      {GPIO_PinSource10, GPIO_PinSource11}, {&RCC->APB1ENR, &RCC->AHB1ENR}, 
      {RCC_APB1Periph_USART3, RCC_AHB1Periph_GPIOB }, GPIO_AF_USART3 },
    { UART4, { GPIOC, GPIOC }, { GPIO_Pin_10, GPIO_Pin_11 },  // UART4
      {GPIO_PinSource10, GPIO_PinSource11}, {&RCC->APB1ENR, &RCC->AHB1ENR}, 
      {RCC_APB1Periph_UART4, RCC_AHB1Periph_GPIOC }, GPIO_AF_UART4 },
    { UART5, { GPIOC, GPIOD }, { GPIO_Pin_12, GPIO_Pin_2 },   // UART5
      {GPIO_PinSource12, GPIO_PinSource2}, {&RCC->APB1ENR, &RCC->AHB1ENR}, 
      {RCC_APB1Periph_UART5, RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD }, GPIO_AF_UART5 },
    { USART6, { GPIOC, GPIOC}, { GPIO_Pin_6, GPIO_Pin_7 },    // UART6
      {GPIO_PinSource6, GPIO_PinSource7}, {&RCC->APB2ENR, &RCC->AHB1ENR}, 
      {RCC_APB2Periph_USART1, RCC_AHB1Periph_GPIOC }, GPIO_AF_USART6 },
    { UART7, { GPIOF, GPIOF }, { GPIO_Pin_7, GPIO_Pin_6 },    // UART7
      {GPIO_PinSource7, GPIO_PinSource6}, {&RCC->APB1ENR, &RCC->AHB1ENR}, 
      {RCC_APB1Periph_USART2, RCC_AHB1Periph_GPIOF }, GPIO_AF_UART7 },
    { UART8, { GPIOE, GPIOE }, { GPIO_Pin_1, GPIO_Pin_0 },    // UART8
      {GPIO_PinSource1, GPIO_PinSource0}, {&RCC->APB1ENR, &RCC->AHB1ENR}, 
      {RCC_APB1Periph_USART3, RCC_AHB1Periph_GPIOE }, GPIO_AF_UART8 },
};

void uart_init(uint8_t id, uint32_t baudrate)
{
    if (!((id >= 1) && (id <= 8)))
        return;
        
    struct uartInitDef_t * uartInitDef = uartInitDefs + id - 1;


    int i;
    for (i = 0; i < 2; i++)
        *(uartInitDef->bridge[i]) |= uartInitDef->peripheral[i];

    for (i = 0; i < 2; i++)
    {
	    GPIO_InitTypeDef gpiodef;
	    gpiodef.GPIO_Pin = uartInitDef->gpio[i];
	    gpiodef.GPIO_Mode = GPIO_Mode_AF;
	    gpiodef.GPIO_Speed = GPIO_Speed_50MHz;
	    gpiodef.GPIO_OType = GPIO_OType_PP;
	    gpiodef.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(uartInitDef->tdef[i], &gpiodef);
        GPIO_PinAFConfig(uartInitDef->tdef[i], uartInitDef->pinsource[i], uartInitDef->af);
    }

	USART_InitTypeDef usartdef;
    usartdef.USART_BaudRate = baudrate;
    usartdef.USART_WordLength = USART_WordLength_8b;
    usartdef.USART_StopBits = USART_StopBits_1;
    usartdef.USART_Parity = USART_Parity_No;
    usartdef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartdef.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(uartInitDef->id, &usartdef);

    USART_Cmd(uartInitDef->id, ENABLE);
}

void uart_deinit(uint8_t id)
{
    if (!((id >= 1) && (id <= 8)))
        return;

    struct uartInitDef_t * uartInitDef = uartInitDefs + id - 1;

    USART_DeInit(uartInitDef->id);
}

void uart_send_char(uint8_t id, uint8_t c)
{
    if (!((id >= 1) && (id <= 8)))
        return;

    struct uartInitDef_t * uartInitDef = uartInitDefs + id - 1;

    while (USART_GetFlagStatus(uartInitDef->id, USART_FLAG_TXE) == RESET);
    USART_SendData(uartInitDef->id, c);
}

uint8_t uart_receive_char(uint8_t id)
{
    if (!((id >= 1) && (id <= 8)))
        return 0;

    struct uartInitDef_t * uartInitDef = uartInitDefs + id - 1;

    while (USART_GetFlagStatus(uartInitDef->id, USART_FLAG_RXNE) == RESET);
    return (uint8_t) USART_ReceiveData(uartInitDef->id);
}

void uart_read(uint8_t id, uint8_t *buffer, uint8_t nb)
{
    while (nb--)
        *buffer++ = uart_receive_char(id);
}

void uart_write(uint8_t id, uint8_t *buffer, uint8_t nb)
{
    while (nb--)
        uart_send_char(id, (uint8_t) (*buffer++));
}


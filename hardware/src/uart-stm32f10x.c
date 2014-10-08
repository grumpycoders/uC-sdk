#include "spi.h"
#include "task.h"
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>

#include <stdio.h>

struct uartInitDef_t {
    // tx / rx
    USART_TypeDef * id;
    GPIO_TypeDef * tdef[2];
    GPIO_InitTypeDef gpiodef[2];
    // uart / gpio
    volatile uint32_t * bridge[2];
    uint32_t peripheral[2];
};

static struct uartInitDef_t uartInitDefs[5] = {
    { USART1, { GPIOA, GPIOA}, {   // UART1
        { GPIO_Pin_9 , GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // TX
        { GPIO_Pin_10, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING },       // RX
    }, {&RCC->APB2ENR, &RCC->APB2ENR}, {RCC_APB2Periph_USART1, RCC_APB2Periph_GPIOA } },
    { USART2, { GPIOA, GPIOA }, {   // UART2
        { GPIO_Pin_2 , GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // TX
        { GPIO_Pin_3, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING },       // RX
    }, {&RCC->APB1ENR, &RCC->APB2ENR}, {RCC_APB1Periph_USART2, RCC_APB2Periph_GPIOA } },
    { USART3, { GPIOB, GPIOB }, {   // UART3
        { GPIO_Pin_10 , GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // TX
        { GPIO_Pin_11, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING },       // RX
    }, {&RCC->APB1ENR, &RCC->APB2ENR}, {RCC_APB1Periph_USART3, RCC_APB2Periph_GPIOB } },
    { UART4, { GPIOC, GPIOC }, {   // UART4
        { GPIO_Pin_10 , GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // TX
        { GPIO_Pin_11, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING },       // RX
    }, {&RCC->APB1ENR, &RCC->APB2ENR}, {RCC_APB1Periph_UART4, RCC_APB2Periph_GPIOC } },
    { UART5, { GPIOC, GPIOD }, {   // UART5
        { GPIO_Pin_12 , GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // TX
        { GPIO_Pin_2, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING },       // RX
    }, {&RCC->APB1ENR, &RCC->APB2ENR}, {RCC_APB1Periph_UART5, RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD } },
};

void uart_init(uint8_t id, uint32_t baudrate)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct uartInitDef_t * uartInitDef = uartInitDefs + id - 1;

    //clock AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    int i;
    for (i = 0; i < 2; i++)
        *(uartInitDef->bridge[i]) |= uartInitDef->peripheral[i];

    for (i = 0; i < 2; i++)
        GPIO_Init(uartInitDef->tdef[i], &uartInitDef->gpiodef[i]);

    USART_InitTypeDef usartdef;

    usartdef.USART_BaudRate = baudrate;
    usartdef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartdef.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usartdef.USART_WordLength = USART_WordLength_8b;
    usartdef.USART_StopBits = USART_StopBits_1;
    usartdef.USART_Parity = USART_Parity_No;

    USART_Init(uartInitDef->id, &usartdef);

    USART_ClockInitTypeDef clockdef;
    USART_ClockStructInit(&clockdef);
    USART_ClockInit(uartInitDef->id, &clockdef);

    USART_Cmd(uartInitDef->id, ENABLE);
}

void uart_deinit(uint8_t id)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct uartInitDef_t * uartInitDef = uartInitDefs + id - 1;

    USART_DeInit(uartInitDef->id);
}

void uart_send_char(uint8_t id, uint8_t c)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct uartInitDef_t * uartInitDef = uartInitDefs + id - 1;

    while (USART_GetFlagStatus(uartInitDef->id, USART_FLAG_TXE) == RESET);
    USART_SendData(uartInitDef->id, c);
}

uint8_t uart_receive_char(uint8_t id)
{
    if (!((id >= 1) && (id <= 3)))
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


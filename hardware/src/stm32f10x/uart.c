#include "uart.h"

#include <stdio.h>

#include <hardware.h>

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>

#include <gpio.h>

static USART_TypeDef * const uarts[] = { (void *) 0, USART1, USART2, USART3, UART4, UART5 };


void uart_config(uart_port_t uart_port, uint32_t baudrate)
{
    uart_t uart = uart_port.uart;
    pin_t rx = uart_port.rx;
    pin_t tx = uart_port.tx;
//    pin_t rts = uart_port.rts;
//    pin_t cts = uart_port.cts;

    USART_TypeDef * id = uarts[uart];

    if (uart > uart_port_5)
        return;

    //clock USART
    switch (uart) {
    case uart_port_1:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        break;
    case uart_port_2:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        break;
    case uart_port_3:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        break;
    case uart_port_4:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
        break;
    case uart_port_5:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
        break;
    default:
        return;
    }

    //clock AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    //CLOCK GPIO pins
    volatile uint32_t port_flags = 0;
    port_flags |= 1 << (rx.port + 2);
    port_flags |= 1 << (tx.port + 2);
/*    if (uart == uart_port_1 || uart == uart_port_2 || uart == uart_port_3 || uart == uart_port_6){
        port_flags |= 1 << rts.port;
        port_flags |= 1 << cts.port;
    }*/
    RCC_APB2PeriphClockCmd(port_flags, ENABLE);

    GPIO_InitTypeDef gpiodef;
    gpiodef.GPIO_Speed = GPIO_Speed_50MHz;

    gpiodef.GPIO_Pin = 1 << rx.pin;
    gpiodef.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(stm32f10x_gpio_ports[rx.port], &gpiodef);

    gpiodef.GPIO_Pin = 1 << tx.pin;
    gpiodef.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(stm32f10x_gpio_ports[tx.port], &gpiodef);

    USART_InitTypeDef uartdef;

    uartdef.USART_BaudRate = baudrate;
    uartdef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uartdef.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    uartdef.USART_WordLength = USART_WordLength_8b;
    uartdef.USART_StopBits = USART_StopBits_1;
    uartdef.USART_Parity = USART_Parity_No;

    USART_Init(id, &uartdef);

    USART_ClockInitTypeDef clockdef;
    USART_ClockStructInit(&clockdef);
    USART_ClockInit(id, &clockdef);

    USART_Cmd(id, ENABLE);
}

/*
void uart_deinit(uart_t uart)
{
    USART_DeInit(uarts[uart]);
}*/

void uart_send_char(uart_t uart, uint8_t c)
{
    USART_TypeDef * id = uarts[uart];

    while (USART_GetFlagStatus(id, USART_FLAG_TXE) == RESET);
    USART_SendData(id, c);
}

uint8_t uart_receive_char(uart_t uart)
{
    USART_TypeDef * id = uarts[uart];

    while (USART_GetFlagStatus(id, USART_FLAG_RXNE) == RESET);
    return (uint8_t) USART_ReceiveData(id);
}

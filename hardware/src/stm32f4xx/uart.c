#include "uart.h"

#include <hardware.h>

#include <stm32f4xx.h>

#include <gpio.h>

static USART_TypeDef * const uarts[] = { (void *) 0, USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8 };

void uart_config(uart_port_t uart_port, uint32_t baudrate)
{
    if (uart_port.uart > 6)
        return;
    uart_t uart = uart_port.uart;
    pin_t rx = uart_port.rx;
    pin_t tx = uart_port.tx;
//    pin_t rts = uart_port.rts;
//    pin_t cts = uart_port.cts;

    USART_TypeDef * id = uarts[uart];

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
    case uart_port_6:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
        break;
    case uart_port_7:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
        break;
    case uart_port_8:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);
        break;
    default:
        return;
    }

    if (uart_port.uart <= 3)
    {
        gpio_config_alternate(rx, pin_dir_write, pull_up, 7);
        gpio_config_alternate(tx, pin_dir_write, pull_up, 7);
/*        if (flowfontrol)
        {
            gpio_config_alternate(rts, pin_dir_write, pull_up, 7);
            gpio_config_alternate(cts, pin_dir_write, pull_up, 7);
        }*/
    }
    else
    {
        gpio_config_alternate(rx, pin_dir_write, pull_up, 8);
        gpio_config_alternate(tx, pin_dir_write, pull_up, 8);
/*        if (flowcontrol && uart_port == 6)
        {
            gpio_config_alternate(rts, pin_dir_write, pull_up, 8);
            gpio_config_alternate(cts, pin_dir_write, pull_up, 8);
        }*/
    }

    USART_InitTypeDef uartdef;
    uartdef.USART_BaudRate = baudrate;
    uartdef.USART_WordLength = USART_WordLength_8b;
    uartdef.USART_StopBits = USART_StopBits_1;
    uartdef.USART_Parity = USART_Parity_No;
/*    if (flowcontrol)
        uartdef.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
    else*/
    uartdef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uartdef.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(id, &uartdef);

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


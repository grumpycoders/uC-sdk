#include "uart.h"
#include "hardware.h"

#include <FreeRTOS.h>
#include <task.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

#include <stdio.h>

static USART_TypeDef * const uarts[] = { (void *) 0, USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8 };

void uart_config(uart_port_t uart_port, uint32_t baudrate)
{
    uart_t uart = uart_port.uart;
    pin_t rx = uart_port.rx;
    pin_t tx = uart_port.tx;
//    pin_t rts = uart_port.rts;
//    pin_t cts = uart_port.cts;

    USART_TypeDef * id = uarts[uart];
    uint8_t af;

    switch (uart) {
    case uart_port_1:
        af = GPIO_AF_USART1;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        break;
    case uart_port_2:
        af = GPIO_AF_USART2;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        break;
    case uart_port_3:
        af = GPIO_AF_USART3;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        break;
    case uart_port_4:
        af = GPIO_AF_UART4;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
        break;
    case uart_port_5:
        af = GPIO_AF_UART5;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
        break;
    case uart_port_6:
        af = GPIO_AF_USART6;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
        break;
    case uart_port_7:
        af = GPIO_AF_UART7;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
        break;
    case uart_port_8:
        af = GPIO_AF_UART8;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);
        break;
    default:
        return;
    }

    uint32_t port_flags = 0;
    port_flags |= 1 << rx.port;
    port_flags |= 1 << tx.port;
/*    if (uart == uart_port_1 || uart == uart_port_2 || uart == uart_port_3 || uart == uart_port_6){
        port_flags |= 1 << rts.port;
        port_flags |= 1 << cts.port;
    }*/
    RCC_AHB1PeriphClockCmd(port_flags, ENABLE);

    GPIO_InitTypeDef gpiodef;
    gpiodef.GPIO_Mode = GPIO_Mode_AF;
    gpiodef.GPIO_OType = GPIO_OType_PP;
    gpiodef.GPIO_PuPd = GPIO_PuPd_UP;
    gpiodef.GPIO_Speed = GPIO_Speed_50MHz;

    gpiodef.GPIO_Pin = 1 << rx.pin;
    GPIO_PinAFConfig(stm32f4xx_gpio_ports[rx.port], rx.pin, af);
    GPIO_Init(stm32f4xx_gpio_ports[rx.port], &gpiodef);

    gpiodef.GPIO_Pin = 1 << tx.pin;
    GPIO_PinAFConfig(stm32f4xx_gpio_ports[tx.port], tx.pin, af);
    GPIO_Init(stm32f4xx_gpio_ports[tx.port], &gpiodef);

/*
    if (uart == uart_port_1 || uart == uart_port_2 || uart == uart_port_3 || uart == uart_port_6){
        if (rts){ //NOT SURE THIS IS OK
            gpiodef.GPIO_Pin = 1 << rts.pin;
            GPIO_PinAFConfig(stm32f4xx_gpio_ports[rts.port], rts.pin, af);
            GPIO_Init(stm32f4xx_gpio_ports[rts.port], &gpiodef);
        }

        if (cts){  //NOT SURE THIS IS OK
            gpiodef.GPIO_Pin = 1 << cts.pin;
            GPIO_PinAFConfig(stm32f4xx_gpio_ports[cts.port], cts.pin, af);
            GPIO_Init(stm32f4xx_gpio_ports[cts.port], &gpiodef);
        }
    }*/

    USART_InitTypeDef uartdef;
    uartdef.USART_BaudRate = baudrate;
    uartdef.USART_WordLength = USART_WordLength_8b;
    uartdef.USART_StopBits = USART_StopBits_1;
    uartdef.USART_Parity = USART_Parity_No;
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


#include "BoardConsole.h"
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <FreeRTOS.h>
#include <task.h>

#include <uart.h>

static USART_TypeDef * const uarts[] = { (void *) 0, USART1, USART2, USART3, UART4, UART5 };

void BoardConsoleInit() {
    pin_t rx = make_pin(GPIO_PORT_A, 9);
    pin_t tx = make_pin(GPIO_PORT_A, 10);

    uart_port_t uart = { .uart = uart_port_1, .rx = rx, .tx = tx };

    uart_config(uart, 115200);
}

void BoardConsolePuts(const char * str) {
    taskENTER_CRITICAL();
    while(*str)
        BoardConsolePutc(*(str++));
    BoardConsolePutc('\n');
    taskEXIT_CRITICAL();
}

void BoardConsolePutc(int c) {
    if (c == '\r') return;
    if (c == '\n') c = '\r';

    uart_send_char(uart_port_1, c);

    if (c == '\r') {
        c = '\n';
        uart_send_char(uart_port_1, c);
    }
}

void BoardConsolePrintf(const char * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    BoardConsoleVPrintf(fmt, ap);
    va_end(ap);
}

static void xprintfCallback(const char * str, int strsize, void * opaque0) {
    taskENTER_CRITICAL();
    while (strsize--)
        BoardConsolePutc(*str++);
    taskEXIT_CRITICAL();
}

void BoardConsoleVPrintf(const char * fmt, va_list ap) {
    vxprintf(xprintfCallback, NULL, fmt, ap);
}

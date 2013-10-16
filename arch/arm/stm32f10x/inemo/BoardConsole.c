#include "BoardConsole.h"
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>

void BoardConsoleInit() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef gpiodef;
    gpiodef.GPIO_Pin = GPIO_Pin_9;
    gpiodef.GPIO_Mode = GPIO_Mode_AF_PP;
    gpiodef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpiodef);
    gpiodef.GPIO_Pin = GPIO_Pin_10;
    gpiodef.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpiodef);

    USART_InitTypeDef usartdef;

    usartdef.USART_BaudRate = 115200;
    usartdef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartdef.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usartdef.USART_WordLength = USART_WordLength_8b;
    usartdef.USART_StopBits = USART_StopBits_1;
    usartdef.USART_Parity = USART_Parity_No;

    USART_Init(USART1, &usartdef);

    USART_ClockInitTypeDef clockdef;
    USART_ClockStructInit(&clockdef);
    USART_ClockInit(USART1, &clockdef);

    USART_Cmd(USART1, ENABLE);}

void BoardConsolePuts(const char * str) {
    while(*str)
        BoardConsolePutc(*(str++));
}

void BoardConsolePutc(int c) {
    if (c == '\r') return;
    if (c == '\n') c = '\r';
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
    if (c == '\r') {
        c = '\n';
        USART_SendData(USART1, c);
    }
}


void BoardConsolePrintf(const char * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    BoardConsoleVPrintf(fmt, ap);
    va_end(ap);
}

static void xprintfCallback(const char * str, int strsize, void * opaque0) {
    while (strsize--)
        BoardConsolePutc(*str++);
}

void BoardConsoleVPrintf(const char * fmt, va_list ap) {
    vxprintf(xprintfCallback, NULL, fmt, ap);
}

/*
void BoardConsolePrintf(const char * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    BoardConsoleVPrintf(fmt, ap);
    va_end(ap);
}

void BoardConsoleVPrintf(const char * fmt, va_list ap) {
	char str[512];
	vsprintf(str, fmt, ap);
	BoardConsolePuts(str);
}
*/
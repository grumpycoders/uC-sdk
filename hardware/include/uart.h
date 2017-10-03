#pragma once

#include <decl.h>
#include <stdint.h>

#include <gpio.h>

// ought to be generic, cm3-specific for now
typedef enum {
    uart_port_0,
    uart_port_1,
    uart_port_2,
    uart_port_3,
    uart_port_4,
    uart_port_5,
    uart_port_6,
    uart_port_7,
    uart_port_8,
} uart_t;

typedef struct {
    pin_t rx;
    pin_t tx;
//    pin_t cts;
//    pin_t rts;
    uart_t uart;
} uart_port_t;

ucsdk_static_assert(sizeof(uart_port_t) <= 6, "uart_port_t isn't 48 bits-wide");
//ucsdk_static_assert(sizeof(uart_port_t) <= 10, "uart_port_t isn't 80 bits-wide");

BEGIN_DECL

void uart_config(uart_port_t uart_port, uint32_t baudrate);
//void uart_deinit(uart_t uart);

void uart_send_char(uart_t uart, uint8_t c);
uint8_t uart_receive_char(uart_t uart);

static __inline__ void uart_read(uart_t uart, uint8_t *buffer, uint8_t nb){ while (nb--) *buffer++ = uart_receive_char(uart); }

static __inline__ void uart_write(uart_t uart, uint8_t *buffer, uint8_t nb) { while (nb--) uart_send_char(uart, (uint8_t) (*buffer++)); }


END_DECL


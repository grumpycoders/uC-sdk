#pragma once

#include <decl.h>
#include <stdint.h>

BEGIN_DECL

void uart_init(uint8_t id, uint32_t baudrate);
void uart_deinit(uint8_t id);

void uart_send_char(uint8_t id, uint8_t c);
uint8_t uart_receive_char(uint8_t id);

void uart_read(uint8_t id, uint8_t *buffer, uint8_t nb);
void uart_write(uint8_t id, uint8_t *buffer, uint8_t nb);

END_DECL


#pragma once

#include <decl.h>
#include <stdint.h>

BEGIN_DECL

void i2c_init(uint8_t id, uint32_t speed);
void i2c_dma_init(uint8_t id);

//call before initiating a read communication
void i2c_start_read(uint8_t id, uint8_t destination);
//call before initiating a write communication
void i2c_start_write(uint8_t id, uint8_t destination);
//call after communication ended
void i2c_stop(uint8_t id);

void i2c_read_polling(uint8_t id, uint8_t *buffer, uint8_t nb);
void i2c_write_polling(uint8_t id, uint8_t *buffer, uint8_t nb);

void i2c_read_dma(uint8_t id, uint8_t *buffer, uint8_t nb);
void i2c_write_dma(uint8_t id, uint8_t *buffer, uint8_t nb);

END_DECL


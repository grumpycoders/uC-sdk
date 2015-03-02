#pragma once

#include <decl.h>
#include <stdint.h>

BEGIN_DECL

//setup spi
void spi_init(uint8_t id);

//call before initiating a communication
void spi_start(uint8_t id);
//call after communication ended
void spi_stop(uint8_t id);

//read data from SPI
void spi_read(uint8_t id, uint8_t *buffer, uint8_t nb);
//write data to SPI
void spi_write(uint8_t id, uint8_t *buffer, uint8_t nb);

//read a register on a SPI device connected to a SPI bus
void spi_get_register(uint8_t id, uint8_t address, uint8_t *buffer, uint8_t nb);
//write a register on a SPI device connected to a SPI bus
void spi_set_register(uint8_t id, uint8_t address, uint8_t *buffer, uint8_t nb);

END_DECL


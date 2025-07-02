#ifndef SPI_SIMPLE_H
#define SPI_SIMPLE_H

#include <stdint.h>

void spi_init();
uint8_t spi_transfer(uint8_t data);

#endif

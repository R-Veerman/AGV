#include "spi_simple.h"
#include <avr/io.h>

void spi_init() {
    DDRB |= (1 << PB2) | (1 << PB1) | (1 << PB0); // MOSI (D51), SCK (D52), SS (D53)
    DDRB &= ~(1 << PB3); // MISO (D50) als input
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // SPI Enable, Master mode, clk/16
}

uint8_t spi_transfer(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

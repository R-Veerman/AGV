#include "rfid_mfrc522.h"
#include "spi_simple.h"
#include <util/delay.h>
#include <avr/io.h>

#define SS_PIN  PB0  // D53 op Arduino Mega
#define RST_PIN PL0  // D49 op Arduino Mega

#define MFRC522_COMMAND_REG      0x01
#define MFRC522_FIFO_DATA_REG    0x09
#define MFRC522_FIFO_LEVEL_REG   0x0A
#define MFRC522_BIT_FRAMING_REG  0x0D
#define MFRC522_TX_CONTROL_REG   0x14
#define MFRC522_COMMAND_TRANSCEIVE 0x0C
#define MFRC522_COM_IRQ_REG      0x04
#define MFRC522_COLL_REG         0x0E
#define MFRC522_MODE_REG         0x11
#define MFRC522_TMODE_REG        0x2A
#define MFRC522_TPRESCALER_REG   0x2B
#define MFRC522_TRELOAD_REG_L    0x2D
#define MFRC522_TRELOAD_REG_H    0x2C
#define MFRC522_TXASK_REG        0x15

#define PICC_CMD_REQA            0x26


static void write_register(uint8_t reg, uint8_t val) {
    PORTB &= ~(1 << SS_PIN);
    spi_transfer((reg << 1) & 0x7E);
    spi_transfer(val);
    PORTB |= (1 << SS_PIN);
}

static uint8_t read_register(uint8_t reg) {
    uint8_t val;
    PORTB &= ~(1 << SS_PIN);
    spi_transfer(((reg << 1) & 0x7E) | 0x80);
    val = spi_transfer(0);
    PORTB |= (1 << SS_PIN);
    return val;
}

void rfid_init(void) {
    DDRB |= (1 << SS_PIN);    // SS (D53)
    DDRH |= (1 << RST_PIN);   // RST (D9)
    PORTB |= (1 << SS_PIN);
    PORTH |= (1 << RST_PIN);

    spi_init();

    write_register(MFRC522_COMMAND_REG, 0x0F); // Soft reset
    _delay_ms(50);

    write_register(MFRC522_TMODE_REG, 0x80);
    write_register(MFRC522_TPRESCALER_REG, 0xA9);
    write_register(MFRC522_TRELOAD_REG_L, 0xE8);
    write_register(MFRC522_TRELOAD_REG_H, 0x03);
    write_register(MFRC522_TXASK_REG, 0x40);
    write_register(MFRC522_MODE_REG, 0x3D);

    uint8_t val = read_register(MFRC522_TX_CONTROL_REG);
    if (!(val & 0x03)) {
        write_register(MFRC522_TX_CONTROL_REG, val | 0x03);
    }
}

bool rfid_check_tag_present(void) {
    write_register(MFRC522_BIT_FRAMING_REG, 0x07);
    write_register(MFRC522_COMMAND_REG, 0x00);
    write_register(MFRC522_FIFO_DATA_REG, PICC_CMD_REQA);
    write_register(MFRC522_COMMAND_REG, MFRC522_COMMAND_TRANSCEIVE);
    write_register(MFRC522_BIT_FRAMING_REG, 0x87);

    for (uint8_t i = 0; i < 200; i++) {
        uint8_t irq = read_register(MFRC522_COM_IRQ_REG);
        if (irq & 0x30) break;
        _delay_ms(1);
    }

    uint8_t error = read_register(0x06); // ErrorReg
    if (error & 0x1B) return false;

    uint8_t len = read_register(MFRC522_FIFO_LEVEL_REG);
    return (len != 0);
}

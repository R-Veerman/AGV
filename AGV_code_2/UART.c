#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "UART.h"
#include "state_machine.h"

#define RICHTING_LINKS  0
#define RICHTING_RECHTS 1



// Ringbuffer voor UART3
volatile char uart3_buffer[UART_BUFFER_SIZE];
volatile uint8_t uart3_head = 0;
volatile uint8_t uart3_tail = 0;


// Overige variables
volatile uint8_t agv_buffer[4];
volatile uint8_t agv_index = 0;

volatile uint8_t cmd = 0;
volatile uint8_t param = 0;



// ---------------- UART3 ----------------

void uart3_init(void) {
    UBRR3H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR3L = (uint8_t)UBRR_VALUE;
    UCSR3B = (1 << TXEN3) | (1 << RXEN3) | (1 << RXCIE3);  // RX interrupt aan
    UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);
    sei(); // Globale interrupts aan
}

void uart3_transmit(char data) {
    while (!(UCSR3A & (1 << UDRE3)));
    UDR3 = data;
}

void uart3_print(const char *str) {
    while (*str) {
        uart3_transmit(*str++);
    }
}

void uart3_transmit_hex(uint8_t value) {
    char hex[3];
    snprintf(hex, sizeof(hex), "%02X", value);
    uart3_print(hex);
}

uint8_t uart3_available(void) {
    return (UART_BUFFER_SIZE + uart3_head - uart3_tail) % UART_BUFFER_SIZE;
}

char uart3_read(void) {
    if (uart3_head == uart3_tail) return 0; // leeg
    char data = uart3_buffer[uart3_tail];
    uart3_tail = (uart3_tail + 1) % UART_BUFFER_SIZE;
    return data;
}

ISR(USART3_RX_vect) {
    agv_buffer[agv_index++] = UDR3;

    if (agv_index == 4) {
        cmd = agv_buffer[0];
        param = agv_buffer[1];
        agv_index = 0;
        if (cmd == 0x01) {
            huidige_state = STATE_RIJDEN;
        }
        else if (cmd == 0x01 && param == 0x7F) {
            huidige_state = STATE_STOPPEN;
        }
        else if (cmd == 0x02 && param == 0x01) {
            huidige_richting = RICHTING_LINKS;
        }
        else if (cmd == 0x02 && param == 0x02) {
            huidige_richting = RICHTING_RECHTS;
        }
        else if (cmd == 0x03 && param == 0x01) {
            huidige_state = STATE_SCHUIF_LINKS;
        }
        else if (cmd == 0x03 && param == 0x02) {
            huidige_state = STATE_SCHUIF_RECHTS;

        }

    }
}

// ---------------- AGV Commando's via UART3 ----------------

void agv_send_simple_command(uint8_t commando) {
    uart3_transmit(commando);
    uart3_transmit(0xFF);
    uart3_transmit(0xFF);
    uart3_transmit(0xFF);
}

void agv_send_flag(void) {
    agv_send_simple_command(0x01);
}




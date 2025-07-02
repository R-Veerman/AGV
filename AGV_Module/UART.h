#ifndef UART_H
#define UART_H

#include <stdint.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

#define UART_BUFFER_SIZE 64

extern int AGV_flag;


// UART3 functies
void uart3_init(void);
void uart3_transmit(char data);
void uart3_print(const char *str);
void uart3_transmit_hex(uint8_t value);
uint8_t uart3_available(void);
char uart3_read(void);

// AGV commando's via UART3
void agv_send_simple_command(uint8_t commando, uint8_t parameter);
void agv_forward(void);
void agv_backward(void);
void agv_stop(void);
void agv_turn_left(void);
void agv_turn_right(void);
void agv_shift_left(void);
void agv_shift_right(void);

#endif

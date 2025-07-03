#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

// Knoppen definitie

#define START_BUTTON  (1 << PC4)  // Pin 33
#define STOP_BUTTON   (1 << PC5)  // Pin 32
#define EMERGENCY_STOP_BUTTON (1 << PC2)  // Pin 35


#define STOP_PIN	PA1
#define DDR_STOP	DDRA
#define PORT_STOP   PORTA

#define START_PIN	PA2
#define DDR_START	DDRA
#define PORT_START	PORTA


// Functies
void init_interface();
void handle_input();
uint8_t button_pressed(uint8_t button_mask);
void handle_input(void);

void start_pin_activate(void);
void start_pin_deactivate(void);
void stop_pin_activate(void);
void stop_pin_deactivate(void);
#endif

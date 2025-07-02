#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

// Knoppen definitie

#define START_BUTTON  (1 << PC4)  // Pin 33
#define STOP_BUTTON   (1 << PC3)  // Pin 34
#define EMERGENCY_STOP_BUTTON (1 << PD2)  // Pin 35


// Functies
void init_interface();
void handle_input();
uint8_t button_pressed(uint8_t button_mask);

#endif

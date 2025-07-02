#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

// Knoppen definitie


#define EMERGENCY_STOP_BUTTON (1 << PD2)  // Pin 35


// Functies
void init_interface();
uint8_t button_pressed(uint8_t button_mask);

#endif

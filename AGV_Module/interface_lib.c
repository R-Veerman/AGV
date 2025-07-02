#include <avr/io.h>
#include <util/delay.h>
#include "interface_lib.h"


// Initialiseer de knoppen
void init_interface() {
    DDRD &= ~EMERGENCY_STOP_BUTTON;
    PORTD |= EMERGENCY_STOP_BUTTON;
}



// Verbeterde debounce voor knoppen
uint8_t button_pressed(uint8_t button_mask) {
    static uint8_t button_last_state = 0;
    uint8_t current_state = PINC & button_mask;
    uint8_t pressed = 0;

    if (!current_state && (button_last_state & button_mask)) {
        _delay_ms(10); // Debounce
        pressed = 1;
    }

    button_last_state = (button_last_state & ~button_mask) | (current_state & button_mask);
    return pressed;
}





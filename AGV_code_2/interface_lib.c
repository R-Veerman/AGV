#include <avr/io.h>
#include <util/delay.h>
#include "interface_lib.h"
#include "stepper_motor_lib.h"
#include "state_machine.h"






// Initialiseer de knoppen
void init_interface() {
    DDRC &= ~(START_BUTTON | STOP_BUTTON);
    PORTC |= (START_BUTTON | STOP_BUTTON ); // Pull-ups aan
    DDRC &= ~EMERGENCY_STOP_BUTTON;
    PORTC |= EMERGENCY_STOP_BUTTON;

    DDR_STOP  |= (1 << STOP_PIN);
	DDR_START |= (1 << START_PIN);

    PORT_STOP  &= ~(1 << STOP_PIN);
	PORT_START &= ~(1 << START_PIN);
}


void start_pin_activate(){
    PORT_START |= (1 << START_PIN);
}
void start_pin_deactivate(){
    PORT_START |= (1 << START_PIN);
}
void stop_pin_activate(){
    PORT_STOP |= (1 << STOP_PIN);
}
void stop_pin_deactivate(){
    PORT_STOP |= (1 << STOP_PIN);
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

void handle_input() {
    if (huidige_state == STATE_STARTUP) {
        if (button_pressed(START_BUTTON)) {
          //  start_pin_activate();
            huidige_state = STATE_RIJDEN;
        }else{
           // start_pin_deactivate();
        }
    }
    if (huidige_state == STATE_RIJDEN) {
        if (button_pressed(STOP_BUTTON)) {
            //stop_pin_activate
            huidige_state = STATE_STARTUP;
        }else{
           // stop_pin_deactivate();
        }
    }

}



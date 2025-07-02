#include <avr/io.h>
#include <avr/interrupt.h>
#include "HC-SR04.h"

// Gets updated with approx 30Hz
volatile int centimeters_1 = 0;
volatile int centimeters_2 = 0;


ISR(TIMER4_CAPT_vect){
    static volatile int start_time_1 = 0;
    // If interrupt is set to rising edge
    if (TCCR4B & (1 << ICES4)) {
        // Store echo start value
        start_time_1 = ICR4;
        // Switch to falling edge
        TCCR4B &= ~(1 << ICES4);
    }
    // If interrupt is set to falling edge
    else {
        // distance in centimeters = (total_time/2)*0.01715 - to simplify this: distance = total_time/117
        centimeters_1 = (ICR4 - start_time_1)/117;
    }
}

ISR(TIMER5_CAPT_vect){
    static volatile int start_time_2 = 0;
    // If interrupt is set to rising edge
    if (TCCR5B & (1 << ICES5)) {
        // Store echo start value
        start_time_2 = ICR5;
        // Switch to falling edge
        TCCR5B &= ~(1 << ICES5);
    }
    // If interrupt is set to falling edge
    else {
        // distance in centimeters = (total_time/2)*0.01715 - to simplify this: distance = total_time/117
        centimeters_2 = (ICR5 - start_time_2)/117;
    }
}
// Switch back to rising edge
ISR(TIMER4_OVF_vect){
    TCCR4B |= (1 << ICES4);

}

ISR(TIMER5_OVF_vect){
    TCCR5B |= (1 << ICES5);
}

void ultrasonic_init(void){
    // Config Trig pin (Pin 44 - PL5)
    DDRL |= PIN_TRIG;
    PORTL &= ~PIN_TRIG;

    // Config Echo pin
    DDRL &= ~PIN_ECHO_1;
    DDRL &= ~PIN_ECHO_2;
    // Timer 5 Configuration for Trigger Pulse
    TCCR5A = (1 << COM5C1) | (1 << WGM51) | (1 << WGM50);
    TCCR5B = (1 << WGM53) | (1 << WGM52) | (1 << CS51) | (1 << ICES5);
    TIMSK5 = (1 << ICIE5) | (1 << TOIE5);

    // Set top value to max
    OCR5A = 0xFFFF;
    // Set trigger to 10us signal
    OCR5C = 20;

    // Timer 4 Configuration for Echo Capture
    TCCR4A = 0;
    TCCR4B = (1 << ICES4) | (1 << CS41);
    TIMSK4 = (1 << ICIE4) | (1 << TOV4);


}


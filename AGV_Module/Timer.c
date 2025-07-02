#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long millis_count = 0;

void timer0_init() {
    // Timer0: CTC mode, prescaler 64
    TCCR0A = (1 << WGM01);             // CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00); // prescaler 64
    OCR0A = 249; // 1 ms bij 16 MHz: (16e6 / (64 * 1000)) - 1

    TIMSK0 |= (1 << OCIE0A); // Enable compare match interrupt
}

ISR(TIMER0_COMPA_vect) {
    millis_count++;
}

unsigned long millis() {
    unsigned long ms;
    cli();
    ms = millis_count;
    sei();
    return ms;
}


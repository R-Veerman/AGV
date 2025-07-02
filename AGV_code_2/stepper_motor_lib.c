#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "stepper_motor_lib.h"
#include "HC-SR04.h"

// Globale snelheidsvariabelen
volatile int stepper_speed_1;
volatile int stepper_speed_2;
volatile int stepper_speed_gen;

void stepper_activation(int status) {
    if (status == 0) {
        PORTB |= EN1 | EN2;
    } else if (status >= 1) {
        PORTB &= ~(EN1 | EN2);
    }
}

void set_stepper_speed(int procent) {
    stepper_speed_gen = 10000 - ((procent * 9500) / 100);
}

void rij_door_latjes() {
    int verschil = centimeters_1 - centimeters_2;
    int speed_adjustment = abs(verschil) * 100;

    if (verschil < 0) {
        stepper_speed_1 = stepper_speed_gen + speed_adjustment;
        stepper_speed_2 = stepper_speed_gen;
    } else if (verschil > 0) {
        stepper_speed_1 = stepper_speed_gen;
        stepper_speed_2 = stepper_speed_gen + speed_adjustment;
    } else {
        stepper_speed_1 = stepper_speed_gen;
        stepper_speed_2 = stepper_speed_gen;
    }

    OCR1A = stepper_speed_1;  // linker motor
    OCR3A = stepper_speed_2;  // rechter motor
}

void stuur_om_latje(uint8_t richting) {
    int normaal_snel = 2000;
    int bijstuur_snel = 5000;  // iets sneller om weg te sturen van wand
    int afstand_drempel = 6;  // in cm

    if (richting == RICHTING_RECHTS) {
        // Rechtsom draaien  rechter sensor actief
        if (centimeters_2 < afstand_drempel) {
            // Wand rechts dichtbij wijk af naar links
            OCR1A = normaal_snel;       // linker motor normaal
            OCR3A = bijstuur_snel;      // rechter sneller
        } else {
            // Wand niet dichtbij standaard draai
            OCR1A = bijstuur_snel;
            OCR3A = normaal_snel;
        }
    } else {
        // Linksom draaien  linker sensor actief
        if (centimeters_1 < afstand_drempel) {
            // Wand links dichtbij wijk af naar rechts
            OCR1A = bijstuur_snel;      // linker sneller
            OCR3A = normaal_snel;
        } else {
            // Wand niet dichtbij standaard draai
            OCR1A = normaal_snel;
            OCR3A = bijstuur_snel;
        }
    }
}


void stepper_init() {
    DDRB |= STEP1 | DIR1 | DIR2 | EN1 | EN2;
    DDRE |= STEP2;
    PORTB |= DIR2;
    PORTB &= ~DIR1;
    PORTB |= EN1 | EN2;

    TCCR1A = (1 << COM1B1) | (1 << WGM11) | (1 << WGM10);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
    TIMSK1 = 0;

    TCCR3A = (1 << COM3B1) | (1 << WGM31) | (1 << WGM30);
    TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31);
    TIMSK3 = 0;

    stepper_speed_gen = 10000;
    OCR1A = stepper_speed_gen;
    OCR3A = stepper_speed_gen;

    OCR1B = 4;
    OCR3B = 4;

    sei(); // interrupts aan
}

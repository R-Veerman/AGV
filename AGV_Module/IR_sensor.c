#include "IR_sensor.h"
#include "state_machine_Module.h"
#include <util/delay.h>
int IR_sense_1=0;
int IR_sense_2=0;

uint8_t laatst_sensor_1 = 0;
uint8_t laatst_sensor_2 = 0;



ISR(INT0_vect){
    IR_sense_1 = 1;
   huidige_state = STATE_CHECK_BLOK;
}
ISR(INT1_vect){
    IR_sense_2 = 1;
    huidige_state = STATE_CHECK_BLOK;
}

void IR_init(){
    DDRD &= ~(IR_sense_pin_1 | IR_sense_pin_2);
    PORTD &= ~(IR_sense_pin_1 | IR_sense_pin_2);



    EICRA = (1<<ISC01) | (1<<ISC00) | (1<<ISC11) | (1<<ISC10);
    EIMSK = (1<<INT0) | (1<<INT1);

    sei();
}
void IR_enable_interrupts() {
    EIMSK |= (1<<INT0) | (1<<INT1);
}

void IR_disable_interrupts() {
    EIMSK |= (0<<INT0) | (0<<INT1);
}


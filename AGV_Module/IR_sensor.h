#ifndef IR_SENSOR_H_INCLUDED
#define IR_SENSOR_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define IR_sense_pin_1 (1<<PD1) //pin 21
#define IR_sense_pin_2 (1<<PD0) //pin 20

extern uint8_t laatst_sensor_1 ;
extern uint8_t laatst_sensor_2 ;


extern int IR_sense_1;
extern int IR_sense_2;

void IR_init();

void IR_enable_interrupts(void);
void IR_disable_interrupts(void);


#endif // IR_SENSOR_H_INCLUDED

#ifndef _STEPPER_MOTOR_LIB_H_
#define _STEPPER_MOTOR_LIB_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Definitie van pinnen
#define DIR1 (1 << PB0)     //pin 53
#define STEP1 (1 << PB6)    //pin 12
#define EN1 (1 << PB1)      //pin 52

#define DIR2 (1<<PB2)       //pin 51
#define STEP2 (1 << PE4)    //pin 2
#define EN2 (1 << PB3)      //pin 50


#define RICHTING_LINKS  0
#define RICHTING_RECHTS 1

// Functies
void stepper_init(void);
void stepper_activation(int status);
void set_stepper_speed(int procent);
void rij_door_latjes(void);
void stuur_om_latje(uint8_t richting);



#endif // STEPPER_MOTOR_LIB_H


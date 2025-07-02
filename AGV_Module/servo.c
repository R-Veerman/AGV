/*
 * servo.c - XvR 2020
 *
 * Use 16-bit timer 1. Using interrupts in order to be
 * able to use the pins on the multi-function shield
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"

#define TIME_VALUE	(40000)
#define RESET_VALUE	(65636ul-TIME_VALUE)
#define STOP_VALUE	(TIME_VALUE*0.075)
#define RANGE		(TIME_VALUE*0.025)

ISR(TIMER5_OVF_vect)
{
	TCNT5 = RESET_VALUE;

	PORT_1 |= (1<<PIN_1);
	PORT_2 |= (1<<PIN_2);
}

ISR(TIMER5_COMPA_vect)
{
	PORT_1 &= ~(1<<PIN_1);
}

ISR(TIMER5_COMPB_vect)
{
	PORT_2 &= ~(1<<PIN_2);
}

void init_servo(void)
{
	DDR_1 |= (1<<PIN_1);
	DDR_2 |= (1<<PIN_2);

	TCCR5A = 0;
	TCCR5B = (0<<CS52) | (1<<CS51) | (0<<CS50); // Prescaler = 8
	TIMSK5 = (1<<OCIE5B) | (1<<OCIE5A) | (1<<TOIE5);

	TCNT5 = RESET_VALUE;

	servo1_set_percentage(0);
	servo2_set_percentage(0);

	sei();
}

void servo1_set_percentage(signed char percentage)
{
	if (percentage >= -100 && percentage <= 100)
	{
		OCR5A = RESET_VALUE + STOP_VALUE + (RANGE/100 * percentage);
	}
}

void servo2_set_percentage(signed char percentage)
{
	if (percentage >= -100 && percentage <= 100)
	{
		OCR5B = RESET_VALUE + STOP_VALUE + (RANGE/100 * percentage);
	}
}

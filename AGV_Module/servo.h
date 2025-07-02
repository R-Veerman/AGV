// servo.h - aangepaste pinnen voor Arduino Mega

#ifndef _SERVO_H_
#define _SERVO_H_

#define PORT_1	PORTL
#define PIN_1	PL3 // pin 46 (OC5A)
#define DDR_1	DDRL

#define PORT_2	PORTL
#define PIN_2	PL4 // pin 45 (OC5B)
#define DDR_2	DDRL

void init_servo(void);
void servo1_set_percentage(signed char percentage);
void servo2_set_percentage(signed char percentage);

#endif /* _SERVO_H_ */

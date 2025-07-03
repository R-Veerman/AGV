#ifndef HC_SR04_H_INCLUDED
#define HC_SR04_H_INCLUDED

#define PIN_TRIG        (1 << PL5)  //pin 44
#define PIN_ECHO_1        (1<<PL1)    //pin 48
#define PIN_ECHO_2       (1<<PL0)    //pin 49

#define RICHTING_LINKS  0
#define RICHTING_RECHTS 1

extern volatile int centimeters_1;
extern volatile int centimeters_2;
extern volatile int verschil;
void ultrasonic_init(void);


#endif // HC_SR04_H_INCLUDED

#include "interface_lib.h"
#include "HC-SR04.h"
#include "stepper_motor_lib.h"
#include "state_machine.h"
#include <avr/io.h>

#define HALT_PIN	PA0
#define DDR_HALT	DDRA
#define PORT_HALT	PORTA



volatile State huidige_state = STATE_STARTUP;
uint8_t huidige_richting = RICHTING_RECHTS; // wordt overschreven bij start


int main(void) {
	DDR_HALT  &= ~(1 << HALT_PIN);

    init_interface();
    stepper_init();
    ultrasonic_init();
    set_stepper_speed(90);

    while (1) {
        handle_input();
        if (!(PINC & EMERGENCY_STOP_BUTTON)) {
                huidige_state = STATE_STARTUP;
            }

        switch (huidige_state) {

            case STATE_STARTUP:
                stepper_activation(0);

            break;


           case STATE_RIJDEN:

                rij_door_latjes();

                if(PINA & HALT_PIN){
                    stepper_activation(0);
                }else{
                    stepper_activation(1);
                }

                // Eind van de baan gedetecteerd
                if (centimeters_1 > 20 && centimeters_2 > 20) {
                    huidige_state = STATE_DRAAIEN;
                }
                break;

            case STATE_STOPPEN:
                stepper_activation(0);
                break;

            case STATE_DRAAIEN:
                // Voer continu stuurbeweging uit
                stuur_om_latje(huidige_richting);

                // Check of we weer tussen twee latjes zitten
                if (centimeters_1 < 20 && centimeters_2 < 20) {
                    huidige_state = STATE_RIJDEN;
                }
                break;

            case STATE_NOODSTOP:
                    stepper_activation(0); // motoren uit

                    // Pas terug naar start als knop is los (hoog) én op start gedrukt
                    if ((PIND & EMERGENCY_STOP_BUTTON) && button_pressed(START_BUTTON)) {
                        huidige_state = STATE_STARTUP;
                    }
                    break;

        }
    }
}

#include "interface_lib.h"
#include "HC-SR04.h"
#include "stepper_motor_lib.h"
#include "state_machine.h"
#include "UART.h"
#include <avr/io.h>


volatile State huidige_state = STATE_STARTUP;
uint8_t huidige_richting = RICHTING_RECHTS; // wordt overschreven bij start



int main(void) {
    init_interface();
    ultrasonic_init();
    stepper_init();
    uart3_init();


    while (1) {
        switch (huidige_state) {

            case STATE_STARTUP:
                stepper_activation(0);
            break;


           case STATE_RIJDEN:
                if (button_pressed(START_BUTTON)) {
                    stepper_activation(1);
                }
                rij_door_latjes();

                // Eind van de baan gedetecteerd
                if (centimeters_1 > 20 && centimeters_2 > 20) {
                    agv_send_flag();
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
                        agv_send_flag();
                }
                break;
            case STATE_SCHUIF_LINKS:

                break;
            case STATE_SCHUIF_RECHTS:

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

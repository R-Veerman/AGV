
#include "display.h"
#include "state_machine_Module.h"
#include "IR_sensor.h"
#include "interface_lib.h"
#include "Timer.h"
#include "rfid_mfrc522.h"
#include <avr/io.h>
#include <util/delay.h>


#define HALT_PIN	PA0
#define DDR_HALT	DDRA
#define PORT_HALT	PORTA

#define EMERGENCY_STOP_BUTTON (1 << PD2)  // Pin 35

volatile State huidige_state = STATE_RIJDEN;
volatile int blokjes_getelt = 0;
volatile int blokjes_getelt_TAG = 0;

#define WACHT_TIJD_MS 100  // wachttijd in milliseconden

// Voor het aansturen van de servo-acties in stappen
unsigned long vorige_tijd = 0;
uint8_t blok_check_substate = 0;

void HALT_activate(void){
    PORT_HALT |= (1<< HALT_PIN);
}
void HALT_deactivate(void){
    PORT_HALT &= ~(1<< HALT_PIN);
}
int main(void){

    DDR_HALT  |= (1 << HALT_PIN);
    PORT_HALT &= ~(1<< HALT_PIN);
    init_display();
    IR_init();
    init_interface();
    timer0_init();
    IR_enable_interrupts();


    spi_device rfid_left = {&PORTB, PB0, &DDRB};
    spi_device rfid_right = {&PORTL, PL0, &DDRL};
    rfid_init(rfid_left);
    rfid_init(rfid_right);
  //  *(rfid_left.port) |= (1 << rfid_left.pin);   // Deselecteer na init
  //  *(rfid_right.port) |= (1 << rfid_right.pin); // Deselecteer na init



    while (1) {
       if (PIND & EMERGENCY_STOP_BUTTON) {
          huidige_state = STATE_NOODSTOP;
       }
                display1_getal(blokjes_getelt);
                display2_getal(blokjes_getelt_TAG);
        switch (huidige_state) {

            case STATE_RIJDEN:
                HALT_deactivate();
                display1_getal(blokjes_getelt);
                display2_getal(blokjes_getelt_TAG);

                break;

            case STATE_CHECK_BLOK:

               HALT_activate();
                switch(blok_check_substate) {
                    case 0:
                        vorige_tijd = millis();
                        blok_check_substate = 1;
                        break;

                    case 1:
                        if ((millis() - vorige_tijd) >= WACHT_TIJD_MS) {
                            if (rfid_check_tag_present(rfid_right) || rfid_check_tag_present(rfid_left)) {
                                blokjes_getelt_TAG += 1;
                            } else {
                                blokjes_getelt += 1;
                            }
                            display1_getal(blokjes_getelt);
                            display2_getal(blokjes_getelt_TAG);
                            vorige_tijd = millis();
                            blok_check_substate = 2;
                        }
                        break;

                    case 2:
                        if ((millis() - vorige_tijd) >= WACHT_TIJD_MS) {
                            vorige_tijd = millis();
                            huidige_state = STATE_RIJDEN;
                            blok_check_substate = 0;
                        }
                        break;

                }

                break;


            case STATE_NOODSTOP:
                if (PIND & EMERGENCY_STOP_BUTTON) {
                    huidige_state = STATE_STARTUP;
                }

                break;

        }
    }
}


#include "display.h"
#include "state_machine_Module.h"
#include "UART.h"
#include "IR_sensor.h"
#include "interface_lib.h"
#include "Timer.h"
#include "rfid_mfrc522.h"
#include <avr/io.h>
#include <util/delay.h>

#define EMERGENCY_STOP_BUTTON (1 << PD2)  // Pin 35

volatile State huidige_state = STATE_RIJDEN;
volatile int blokjes_getelt = 0;
volatile int blokjes_getelt_TAG = 0;

#define SERVO_WACHT_TIJD_MS 1000  // wachttijd in milliseconden

// Voor het aansturen van de servo-acties in stappen
unsigned long vorige_tijd = 0;
uint8_t blok_check_substate = 0;

int main(void){
    init_display();
    IR_init();
    uart3_init();
    init_interface();
    timer0_init();
    IR_enable_interrupts();


    spi_device rfid_left = {&PORTB, PB0, &DDRB};
    spi_device rfid_right = {&PORTL, PL0, &DDRL};
    rfid_init(rfid_left);
    rfid_init(rfid_right);


    while (1) {
       if (!(PIND & EMERGENCY_STOP_BUTTON)) {
           huidige_state = STATE_NOODSTOP;
       }
                display1_getal(blokjes_getelt);
                display2_getal(blokjes_getelt_TAG);
        switch (huidige_state) {

            case STATE_RIJDEN:
                display1_getal(blokjes_getelt);
                display2_getal(blokjes_getelt_TAG);
                agv_forward();

                break;

            case STATE_CHECK_BLOK:
                switch(blok_check_substate) {
                    case 0:
                        agv_stop();
                        vorige_tijd = millis();
                        blok_check_substate = 1;
                        break;

                    case 1:
                        if ((millis() - vorige_tijd) >= SERVO_WACHT_TIJD_MS) {
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
                        if ((millis() - vorige_tijd) >= SERVO_WACHT_TIJD_MS) {
                            vorige_tijd = millis();                // Start wachttijd vóór IR aan
                            blok_check_substate = 3;
                        }
                        break;

                    case 3:
                        if ((millis() - vorige_tijd) >= 500) {     // 500ms extra wacht
                            blok_check_substate = 0;
                            huidige_state = STATE_RIJDEN;
                        }
                        break;
                }
                break;



            case STATE_DRAAIEN:
                agv_turn_right();
                if(AGV_flag){
                    huidige_state = STATE_RIJDEN;
                    AGV_flag = 0;
                }
                break;

            case STATE_NOODSTOP:
                agv_stop();
                if (PIND & EMERGENCY_STOP_BUTTON) {
                    huidige_state = STATE_STARTUP;
                }

                break;

        }
    }
}

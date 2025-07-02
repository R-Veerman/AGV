#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "display.h"
#include "state_machine_Module.h"

// Aansluitingen:
// 8: SFTCLK	-> PL7  pin 42 R
// 4: LCHCLK	-> PL6  pin 43 M
// 7: SDI	    -> PL5  pin 44 L
#define SDI_BIT		PL7
#define DDR_SDI		DDRL
#define PORT_SDI	PORTL

#define SFTCLK_BIT	PL5
#define DDR_SFTCLK	DDRL
#define PORT_SFTCLK	PORTL

#define LCHCLK_BIT	PL6
#define DDR_LCHCLK	DDRL
#define PORT_LCHCLK	PORTL

static const uint8_t segmentcodes[] = {
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8
    0b11110110, // 9
    0b11101110, // A
    0b00111110, // b
    0b10011100, // C
    0b01111010, // d
    0b10011110, // E
    0b10001110  // F
};

uint8_t char_to_segment(char c) {
    switch (c) {
        case '0' ... '9': return segmentcodes[c - '0'];
        case 'A': return 0b11101110;
        case 'B': return 0b00111110;
        case 'C': return 0b10011100;
        case 'D': return 0b01111010;
        case 'E': return 0b10011110;
        case 'F': return 0b10001110;
        case 'L': return 0b00011100;
        case 'N': return 0b11101100;
        case 'O': return 0b11111100;
        case 'P': return 0b11001110;
        case 'R': return 0b00001010;
        case 'S': return 0b10110110;
        case 'T': return 0b00011110;
        case 'U': return 0b01111100;
        case ' ': return 0x00; // alles uit
        default:  return 0x00; // onbekende tekens = uit
    }
}





void init_display (void)
{
	// Initialiseer de pinnen voor datain, shiftclk en latchclk als output
	DDR_SDI    |= (1 << SDI_BIT);
	DDR_SFTCLK |= (1 << SFTCLK_BIT);
	DDR_LCHCLK |= (1 << LCHCLK_BIT);

	// Maak shiftclk en latchclk laag
	PORT_SFTCLK &= ~(1 << SFTCLK_BIT);
	PORT_LCHCLK &= ~(1 << LCHCLK_BIT);
}

void send_data(char data)
{
	for (unsigned i = 0; i < 8; i++)
	// Herhaal voor alle bits in een char
	{
		// Bepaal de waarde van de bit die je naar het schuifregister
		// wil sturen
		int bit = data & 1;
		data >>= 1;

		// Maak de juiste pin hoog of laag op basis van de bepaalde waarde
		// van het bit
		if (bit)
		{
			PORT_SDI |= (1 << SDI_BIT);
		}
		else
		{
			PORT_SDI &= ~(1 << SDI_BIT);
		}

		// Toggle shiftclk (hoeveel tijd moest het signaal minimaal hoog zijn?)
		// Puls moet minimaal 13 ns hoog zijn. Een clk cycle op de Arduino duurt
		// 62 ns, dus signaal kan hoog en de volgende cycle weer omlaag
		PORT_SFTCLK |= (1 << SFTCLK_BIT);
		PORT_SFTCLK &= ~(1 << SFTCLK_BIT);
	}
}

void display(uint8_t segmentdata, uint8_t digitbit) {
    send_data(digitbit);         // digits eerst
    send_data(segmentdata);     // segmenten actief laag

    PORT_LCHCLK |= (1 << LCHCLK_BIT);
    PORT_LCHCLK &= ~(1 << LCHCLK_BIT);
}


void display_getal(uint32_t getal) {
    for (uint8_t i = 0; i < 8; i++) {
        uint8_t cijfer = getal % 10;
        getal /= 10;

        // Bereken bitpositie van digit: 0 t/m 7
        uint8_t digitbit = ~(1 << i);  // actief laag

        display(segmentcodes[cijfer], digitbit);

        _delay_ms(1);  // 1 ms per digit = 1 kHz refresh
    }
}
void display1_string(const char* txt) {
    for (uint8_t i = 0; i < 4; i++) {
        char c = txt[3 - i];
        uint8_t seg = char_to_segment(c);
        uint8_t digit = ~(1 << (4 + i));  // bovenste display: digit 4-7
        display(seg, digit);
        _delay_ms(1);
    }
}

void display2_string(const char* txt) {
    for (uint8_t i = 0; i < 4; i++) {
        char c = txt[3 - i];
        uint8_t seg = char_to_segment(c);
        uint8_t digit = ~(1 << i);  // onderste display: digit 0-3
        display(seg, digit);
        _delay_ms(1);
    }
}

void display1_getal(uint16_t waarde) {
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t cijfer = waarde % 10;
        waarde /= 10;
        uint8_t digit = ~(1 << (4 + i));  // bovenste display: digit 4-7
        display(segmentcodes[cijfer], digit);
        _delay_ms(1);
    }
}

void display2_getal(uint16_t waarde) {
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t cijfer = waarde % 10;
        waarde /= 10;
        uint8_t digit = ~(1 << i);  // onderste display: digit 0-3
        display(segmentcodes[cijfer], digit);
        _delay_ms(1);
    }
}







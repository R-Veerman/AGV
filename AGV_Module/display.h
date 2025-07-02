#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>

static const uint8_t segmentcodes[];

uint8_t char_to_segment(char c);

// Initialiseer alle benodigde pinnen
void init_display(void);

// Stuur 8 bits data naar shiftregister (intern gebruikt)
void send_data(char data);

// Stuur segmentdata en digitselectie in één keer
void display(uint8_t segmentdata, uint8_t digitbit);

// Toon een geheel getal van maximaal 8 cijfers verdeeld over 2 displays
void display_getal(uint32_t getal);



// ✅ Nieuwe functies – gescheiden per display & datatype
void display1_string(const char* txt);     // toont tekst op bovenste helft (digit 4–7)
void display2_string(const char* txt);     // toont tekst op onderste helft (digit 0–3)
void display1_getal(uint16_t waarde);      // toont getal op bovenste helft
void display2_getal(uint16_t waarde);      // toont getal op onderste helft

#endif

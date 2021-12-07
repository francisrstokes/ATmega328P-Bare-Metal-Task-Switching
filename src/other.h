#ifndef OTHER_H
#define OTHER_H

#include <avr/io.h>
#include <stdint.h>

void DDRB_SET(uint8_t bit);
void DDRB_CLEAR(uint8_t bit);
void PORTB_SET(uint8_t bit);
void PORTB_CLEAR(uint8_t bit);

#endif

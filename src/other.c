#include "other.h"

void DDRB_SET(uint8_t bit) {
  DDRB |= _BV(bit);
}
void DDRB_CLEAR(uint8_t bit) {
  DDRB &= ~_BV(bit);
}

void PORTB_SET(uint8_t bit) {
  PORTB |= _BV(bit);
}
void PORTB_CLEAR(uint8_t bit) {
  PORTB &= ~_BV(bit);
}

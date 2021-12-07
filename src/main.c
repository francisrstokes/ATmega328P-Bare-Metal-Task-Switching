#include "other.h"
// #include <avr/io.h>
#include <util/delay.h>

#define MS_DELAY 1000

int main (void) {
  DDRB_SET(DDB0);
  // DDRB |= DDB0;

  while(1) {
    PORTB_SET(PORTB0);
    // PORTB |= PORTB0;
    _delay_ms(MS_DELAY);
    PORTB_CLEAR(PORTB0);
    // PORTB &= ~PORTB0;
    _delay_ms(MS_DELAY);
  }
}

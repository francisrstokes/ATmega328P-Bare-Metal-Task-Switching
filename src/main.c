#include "util.h"
#include "kernel.h"

void task_blink_red() {
  DDRB |= _BV(DDB1);
  PORTB |= _BV(PORTB1);

  uint32_t counter = 0;

  while(1) {
    counter++;
    if (counter >= 0xf0000L) {
      PORTB ^= _BV(PORTB1);
      counter = 0;
    }
  }
}

void task_blink_green() {
  DDRB |= _BV(DDB0);
  PORTB |= _BV(PORTB0);

  uint32_t counter = 0;

  while(1) {
    counter++;
    if (counter >= 0x7ffffL) {
      PORTB ^= _BV(PORTB0);
      counter = 0;
    }
  }
}

struct MT_TaskDefinition redTask = {
  &task_blink_red,
  128
};

struct MT_TaskDefinition greenTask = {
  &task_blink_green,
  128
};

int main (void) {
  // Turn off internal LED
  DDRB |= _BV(DDB5);
  PORTB &= ~_BV(PORTB5);

  // Intitialise the multitasking kernel
  MT_kernel_init();

  // Register the two tasks
  MT_register_task(&redTask);
  MT_register_task(&greenTask);

  // Start the kernel and hand execution control to the first task
  MT_kernel_start();
}

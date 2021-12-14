#ifndef MT_UART
#define MT_UART

#include <stdint-gcc.h>
#include <avr/io.h>

#define BAUD_300	3332
#define BAUD_9600 103
#define BAUD_38400 25

void UART_init();
void UART_Transmit(unsigned char data);
void UART_TransmitString(unsigned char* data, uint8_t length);

#define UART_TransmitUint16AsText(value) \
  UART_Transmit((uint8_t)((value >> 12) & 0b1111) + 0x30); \
  UART_Transmit((uint8_t)((value >> 8) & 0b1111) + 0x30); \
  UART_Transmit((uint8_t)((value >> 4) & 0b1111) + 0x30); \
  UART_Transmit((uint8_t)((value) & 0b1111) + 0x30); \
  UART_Transmit('\n');

#endif
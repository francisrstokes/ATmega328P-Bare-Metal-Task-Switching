#include "uart.h"

void UART_init() {
  UCSR0A = 0x00;
  UCSR0B = _BV(TXEN0) | _BV(UCSZ01) | _BV(UCSZ00);
  UCSR0C = _BV(USBS0);

  UBRR0L = (BAUD_38400) & 0xff;
  UBRR0H = (BAUD_38400 >> 8) & 0xff;
}

void UART_Transmit(unsigned char data) {
  /* Wait for empty transmit buffer */
  while (!(UCSR0A & _BV(UDRE0))) {}

  /* Put data into buffer, sends the data */
  UDR0 = data;
}

void UART_TransmitString(unsigned char* data, uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    UART_Transmit(data[i]);
  }
}

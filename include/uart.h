#ifndef _UART_H
#define _UART_H

#include <stdint.h>

void uart_setup(uint32_t system_core_clock, uint32_t baud_rate);
void uart_write(uint16_t ch);
int uart_read_available();
uint8_t uart_read();
void uart_empty_buffer();

#endif

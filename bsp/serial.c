#include "bsp.h"

void serial_write(uint16_t ch) { usart2_write(ch); }

uint8_t serial_read() { return usart2_read_buffer(); }

int serial_available() { return usart2_buffer_available(); }

// Clear the buffer of any data
void serial_clear() { return usart2_clear_buffer(); }

void serial_init(uint32_t baud_rate) { usart2_setup(baud_rate); }

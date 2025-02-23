#ifndef KERBAL_BSP_H
#define KERBAL_BSP_H

#include <stdint.h>

void stm32_init();

/*
 * LD4 Functions
 * LD4 is a green user LED connected to GPIO pin PB13
 */
void led_init();
void led_on();
void led_off();
void led_toggle();

/*
 * B1 Push Button Functions
 * B1 is a blue user button connected to GPIO pin PC13
 */
void button_init();
uint8_t button_pressed();

/*
 * SysTick functions
 */
void systick_init();
// Intended to be run inside Systick_Handler
void systick_update();
void delay(int ms);
// 1 tick = 1 ms
uint32_t get_tick();

/*
 * Serial functions
 * This is the serial connection to the PC that will communicate
 * with Kerbal Simpit
 */
void serial_setup(uint32_t baud_rate);
void serial_write(uint16_t ch);
uint8_t serial_read();
int serial_available();
// Clear the buffer of any data
void serial_clear();

/*
 * USART2 functions
 */
void usart2_setup(uint32_t baud_rate);
void usart2_write(uint16_t ch);
void usart2_write_buffer(uint8_t ch);
uint8_t usart2_read();
uint8_t usart2_read_buffer();
int usart2_read_available();
int usart2_buffer_available();
void usart2_clear_buffer();
void usart2_irq_callback(uint8_t value);

#endif // KERBAL_BSP_H

#include "simpit.h"
#include "simpit_message_types.h"
#include "stm32l433xx.h"
#include "timers.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t SystemCoreClock = 16000000;
uint32_t BaudRate = 115200;
int state = 0;

#define LED_PIN (0x1 << 13)

void setup_clocks() {
  RCC->CR |= (RCC_CR_HSION);
  while (!(RCC->CR & RCC_CR_HSIRDY)) {
  };
  RCC->CFGR &= ~(RCC_CFGR_SW);
  RCC->CFGR |= (RCC_CFGR_SW_HSI);
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {
  };
}

void setup_led() {
  // User LED: PB13
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);

  // Configure pins to be output
  GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE13_Pos);
  GPIOB->MODER |= (0x1 << GPIO_MODER_MODE13_Pos);
}

void led_on() { GPIOB->ODR |= LED_PIN; }

void led_off() { GPIOB->ODR &= ~LED_PIN; }

void message_handler(uint8_t type, uint8_t *msg, uint8_t length) {
  if (type == ECHO_RESP_MESSAGE) {
    if (strcmp((char *)msg, "low")) {
      led_off();
    } else {
      led_on();
    }
  }
}

void init() {
  setup_clocks();
  uart_setup(SystemCoreClock, BaudRate);
  setup_led();

  led_on();
  while (!simpit_init()) {
    delay_ms(100);
  }
  led_off();

  simpit_print("Connected", PRINT_TO_SCREEN);
}

void loop() {
  if (state) {
    simpit_send(ECHO_REQ_MESSAGE, "low", 4);
  } else {
    simpit_send(ECHO_REQ_MESSAGE, "high", 5);
  }

  state = !state;
  simpit_update();
}

int main() {
  init();

  while (1) {
    loop();
    delay_ms(1000);
  }

  return 0;
}

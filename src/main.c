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

void setup_button() {
  // Enable GPIOC clock
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

  // Configure PC13 as input
  GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk;
  GPIOC->MODER |= (0x00 << GPIO_MODER_MODE13_Pos);

  // Disable PC13 Pull-up/Pull-down
  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;
}

int button_pressed() { return (GPIOC->IDR & GPIO_IDR_ID13) == GPIO_IDR_ID13; }

void led_on() { GPIOB->ODR |= LED_PIN; }

void led_off() { GPIOB->ODR &= ~LED_PIN; }

void message_handler(uint8_t type, uint8_t *msg, uint8_t length) {}

void init() {
  setup_clocks();
  uart_setup(SystemCoreClock, BaudRate);
  setup_led();
  setup_button();

  led_on();
  while (!simpit_init()) {
    delay_ms(100);
  }
  led_off();

  simpit_print("Connected", PRINT_TO_SCREEN);
}

void loop() {
  if (button_pressed())
    activate_action(STAGE_ACTION);
  simpit_update();
}

int main() {
  init();

  while (1) {
    loop();
  }

  return 0;
}

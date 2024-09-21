#include "simpit.h"
#include "stm32l433xx.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>

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

int main() {
  setup_clocks();
  uart_setup(SystemCoreClock, BaudRate);
  setup_led();

  while (!simpit_init()) {
    for (int i = 0; i < 10000; i++) {
    }
  }

  while (1) {
  }
  return 0;
}

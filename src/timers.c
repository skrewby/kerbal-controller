#include "stm32l433xx.h"
#include <stdint.h>

void delay_ms(int ms) {
  SysTick->LOAD = 16000;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | (1 << 0);

  for (int i = 0; i < ms; i++) {
    while ((SysTick->CTRL & (1U << 16)) == 0) {
    }
  }

  SysTick->CTRL = 0;
}

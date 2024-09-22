#include "bsp.h"
#include "cmsis_gcc.h"
#include <stdint.h>

void init() {
  stm32_init();
  led_init();
  button_init();
  systick_init();
  serial_setup(115200);
}

int main() {
  __disable_irq();
  init();
  __enable_irq();

  while (1) {
    if (serial_available()) {
      uint8_t ch = serial_read();
      if (ch == '1') {
        led_on();
        serial_write('Y');
      } else {
        led_off();
        serial_write('N');
      }
    }
  }
}

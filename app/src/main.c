#include "bsp.h"
#include "cmsis_gcc.h"
#include "simpit.h"
#include "simpit_message_types.h"
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

  led_on();
  while (!simpit_init()) {
    delay(100);
  }
  led_off();
  simpit_print("Screwby Controller Connected", PRINT_TO_SCREEN);

  while (1) {
    if (button_pressed()) {
      simpit_activate_action(STAGE_ACTION);
    }
  }
}

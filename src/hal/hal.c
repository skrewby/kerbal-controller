#include "hal.h"
#include "cmsis_gcc.h"
#include "system_stm32l4xx.h"
#include <stdint.h>

void stm32_init() {
    __disable_irq();
    SystemInit();
    led_init();
    button_init();
    systick_init();
    __enable_irq();
}

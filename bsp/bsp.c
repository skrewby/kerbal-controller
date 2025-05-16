#include "bsp.h"
#include "stm32l433xx.h"
#include "system_stm32l4xx.h"
#include "cmsis_gcc.h"
#include <stdint.h>

void stm32_init() { 
    __disable_irq();
    SystemInit(); 
    led_init();
    button_init();
    systick_init();
    __enable_irq();
}

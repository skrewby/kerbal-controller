#include "bsp.h"
#include "stm32l433xx.h"
#include "system_stm32l4xx.h"
#include <stdint.h>

#define __Vendor_SysTickConfig 0

volatile uint32_t systick_tick = 0;

void systick_init() {
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);
}

uint32_t get_tick() { return systick_tick; }

void systick_update() { systick_tick++; }

void delay(int ms) {
    uint32_t starttick = systick_tick;
    while ((systick_tick - starttick) - ms) {
    }
}

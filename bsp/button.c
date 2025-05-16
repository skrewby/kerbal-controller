#include "bsp.h"
#include "stm32l433xx.h"
#include <stdint.h>

void button_init() {
    // Enable GPIOC clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    // Configure PC13 as input
    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk;
    GPIOC->MODER |= (0x00 << GPIO_MODER_MODE13_Pos);

    // Disable PC13 Pull-up/Pull-down
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;
}

uint8_t button_pressed() { return (GPIOC->IDR & GPIO_IDR_ID13) == GPIO_IDR_ID13; }

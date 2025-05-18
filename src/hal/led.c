#include "hal.h"
#include "stm32l433xx.h"
#include <stdint.h>

void led_init() {
    // Enable GPIOB clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    // Configure PB13 as output
    GPIOB->MODER &= ~GPIO_MODER_MODE13_Msk;
    GPIOB->MODER |= (0x01 << GPIO_MODER_MODE13_Pos);

    // Configure PB13 as Push-Pull output
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT_13;

    // Configure PB13 as High-Speed Output
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED13_Msk;
    GPIOB->OSPEEDR |= (0x10 << GPIO_OSPEEDR_OSPEED13_Pos);

    // Disable PB13 Pull-up/Pull-down
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;

    // Set Initial State OFF
    GPIOB->BSRR |= GPIO_BSRR_BR_13;
}

void led_on() { GPIOB->BSRR |= GPIO_BSRR_BS_13; }

void led_off() { GPIOB->BSRR |= GPIO_BSRR_BR_13; }

void led_toggle() { GPIOB->ODR ^= GPIO_ODR_OD13; }

#include "bsp.h"
#include "ringbuffer.h"
#include "stm32l433xx.h"
#include <stdint.h>

static RingBuffer buffer;

void usart2_setup(uint32_t baud_rate) {
    // Enable peripheral clocks: GPIOA, USART2
    RCC->APB1ENR1 |= (RCC_APB1ENR1_USART2EN);
    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);

    // Configure pin A2 for USART2 TX (AF7)
    GPIOA->MODER &= ~(0x3 << GPIO_MODER_MODE2_Pos);
    GPIOA->MODER |= (0x2 << GPIO_MODER_MODE2_Pos);
    GPIOA->OTYPER &= ~(0x1 << GPIO_OTYPER_OT2_Pos);
    GPIOA->OSPEEDR &= ~(0x3 << GPIO_OTYPER_OT2_Pos);
    GPIOA->OSPEEDR |= (0x2 << GPIO_OSPEEDR_OSPEED2_Pos);
    GPIOA->AFR[0] &= ~(0xF << GPIO_AFRL_AFSEL2_Pos);
    GPIOA->AFR[0] |= (0x7 << GPIO_AFRL_AFSEL2_Pos);

    // Configure pin A3 for USART2 RX (AF7)
    GPIOA->MODER &= ~(0x3 << GPIO_MODER_MODE3_Pos);
    GPIOA->MODER |= (0x2 << GPIO_MODER_MODE3_Pos);
    GPIOA->OTYPER &= ~(0x1 << GPIO_OTYPER_OT3_Pos);
    GPIOA->OSPEEDR &= ~(0x3 << GPIO_OTYPER_OT3_Pos);
    GPIOA->OSPEEDR |= (0x2 << GPIO_OSPEEDR_OSPEED3_Pos);
    GPIOA->AFR[0] &= ~(0xF << GPIO_AFRL_AFSEL3_Pos);
    GPIOA->AFR[0] |= (0x7 << GPIO_AFRL_AFSEL3_Pos);

    uint16_t uartdiv = SystemCoreClock / baud_rate;
    USART2->BRR = uartdiv;
    USART2->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE | USART_CR1_RXNEIE);

    ringbuffer_init(&buffer);
    NVIC_EnableIRQ(USART2_IRQn);
}

void usart2_write(uint16_t ch) {
    while (!(USART2->ISR & USART_ISR_TXE)) {
    }

    USART2->TDR = ch;
}

void usart2_write_buffer(uint8_t ch) { ringbuffer_write(&buffer, &ch, 1); }

uint8_t usart2_read_buffer() {
    uint8_t value;
    ringbuffer_read(&buffer, &value, 1);
    return value;
}

uint8_t usart2_read() { return USART2->RDR & 0xFF; }

int usart2_buffer_available() { return ringbuffer_has_data(&buffer); }

int usart2_read_available() { return USART2->ISR & USART_ISR_RXNE; }

void usart2_clear_buffer() { ringbuffer_init(&buffer); }

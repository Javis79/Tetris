#include "gpio.h"

void gpio_clear(uint32_t pin) {
        PTB->PCOR |= (1 << pin);
}

void gpio_set(uint32_t pin) {
        PTB->PSOR |= (1 << pin);
}

void gpio_init_pin(uint32_t pin) {
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    PORTB->PCR[pin] = PORT_PCR_MUX(1);
    PTB->PDDR |= (1 << pin);
}

#include "gpioa.h"

void gpioa_clear(uint32_t pin) {
        PTA->PCOR |= (1 << pin);
}

void gpioa_set(uint32_t pin) {
        PTA->PSOR |= (1 << pin);
}

void gpioa_init_pin(uint32_t pin) {
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    PORTA->PCR[pin] = PORT_PCR_MUX(1);
    PTA->PDDR |= (1 << pin);
}

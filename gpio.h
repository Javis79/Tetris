#ifndef GPIO_H
#define GPIO_H

#include "MKL05Z4.h"
#include <stdint.h>

void gpio_clear(uint32_t pin);
void gpio_set(uint32_t pin);
void gpio_init_pin(uint32_t pin);

#endif // GPIO_H

#ifndef GPIOA_H
#define GPIOA_H

#include "MKL05Z4.h"
#include <stdint.h>

void gpioa_clear(uint32_t pin);
void gpioa_set(uint32_t pin);
void gpioa_init_pin(uint32_t pin);

#endif // GPIOA_H

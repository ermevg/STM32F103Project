#ifndef NVIC_CONFIG_H

#define NVIC_CONFIG_H

#include "main.h"

void nvic_irq_enable(uint8_t number);

void nvic_irq_disable(uint8_t number);

#endif

#ifndef TIMER_H
#define TIMER_H

#include "main.h"
#include "nvic_config.h"
#include "gpio.h"

void tim2_1ms_init(void);

uint16_t get_tim2_cnt(void);

#endif
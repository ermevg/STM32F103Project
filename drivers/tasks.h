#ifndef TASKS_H
#define TASKS_H

#include "main.h"
#include "timer.h"
#include "gpio.h"

uint16_t check_time (uint16_t*);

void pin_delay(uint16_t* start_time, uint8_t* f);


#endif
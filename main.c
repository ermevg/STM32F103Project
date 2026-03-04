#include "main.h"
#include "system_init.h"
#include "gpio.h"
#include "timer.h"
#include "nvic_config.h"
#include "uart.h"
#include "spi.h"


int main (void){

    SystemClock_Config();

    init_spi();

    while(1)
    {
        spi_transmit("Hello World");
        for(volatile int i = 0; i < 500000; i++);
    }

    return 0;
}

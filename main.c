#include "main.h"
#include "system_init.h"
#include "gpio.h"
//#include "timer.h"
//#include "nvic_config.h"
#include "uart.h"
#include "spi.h"
#include "exti.h"
#include "adc.h"


int main (void){

    SystemClock_Config();

    // init_spi();

    // init_adc();

    init_exti0();
    
    init_uart1();

    while(1)
    {
    //    for(volatile int i = 0; i < 900000; i++);
    //    uart1_send_char("PING!\r\n");
    }

    return 0;
}

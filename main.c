#include "main.h"
#include "system_init.h"
//#include "gpio.h"
#include "timer.h"
//#include "nvic_config.h"
#include "uart.h"
// #include "spi.h"
// #include "exti.h"
// #include "adc.h"
#include "lcd.h"

int main (void){

    SystemClock_Config();

    tim2_1ms_init();
    
    init_uart1();

    lcd_init();

    lcd_clear_full(LCD_WHITE);
    

    while(1)
    {
        
        
    }

    return 0;
}

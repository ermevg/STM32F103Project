#include "main.h"
#include "system_init.h"
#include "gpio.h"
//#include "timer.h"
//#include "nvic_config.h"
//#include "uart.h"
//#include "spi.h"
//#include "exti.h"
//#include "adc.h"
#include "kernel_internal.h"
#include "systick.h"

void task_test0(void *arg)
{
    uint32_t now = jiffies;
    GPIOB_SET_PIN2();
    while (1)
    {
        if((jiffies - now) > 500)
        {
            GPIOB_RESET_PIN2();
            schedule();
            now = jiffies;
            GPIOB_SET_PIN2();
        }
    }
    
}

void task_test1(void *arg)
{
    uint32_t now = jiffies;
    GPIOB_SET_PIN0();
    while (1)
    {
        if((jiffies - now) > 125)
        {
            GPIOB_RESET_PIN0();
            schedule();
            now = jiffies;
            GPIOB_SET_PIN0();
        }
    }
    
}

int main (void){

    SystemClock_Config();
    GPIOA_Init();
    GPIOB_Init();
    kernel.name[0] = '\0';
    kernel.id = 0;
    kernel.state = TASK_RUNNING;
    task_create("test0", task_test0, NULL);
    task_create("test1", task_test1, NULL);
    systick_init();
    
    while(1)
    {
       
    }

    return 0;
}

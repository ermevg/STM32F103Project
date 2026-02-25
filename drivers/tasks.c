#include "tasks.h"

uint16_t check_time (uint16_t* start)
{
    return (uint16_t)(GET_TIM2_CNT() - *start);
}

void pin_delay(void)
{
    static uint8_t parameter = 0;

    uint16_t start_delay;


    switch (parameter)
    {
    case 0:
       { 
            start_delay = GET_TIM2_CNT();
    
            GPIOB_SET_PIN2();
            
            while(check_time(&start_delay) <= 100){}

            GPIOB_RESET_PIN2();

            parameter++;

            break;
       }
    case 1:
        {
            start_delay = GET_TIM2_CNT();
 
            GPIOB_SET_PIN2();
        
            while(check_time(&start_delay) <= 800){}

            GPIOB_RESET_PIN2();

            parameter++;

            break;
        }
    
    case 2:
        {
            start_delay = GET_TIM2_CNT();
        
            GPIOB_SET_PIN2();
                
            while(check_time(&start_delay) <= 2000){}

            GPIOB_RESET_PIN2();

            parameter = 0;

            break;
        }
    
    default:
        break;
    }
}
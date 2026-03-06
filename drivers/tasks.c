#include "tasks.h"

uint16_t check_time (uint16_t* start)
{
    return (uint16_t)(GET_TIM2_CNT() - *start);
}



void pin_delay(uint16_t* start_time, uint8_t *f)
{
    static uint16_t parameter = 100;
    //uint16_t start_delay = GET_TIM2_CNT();
    // while(check_time(&start_delay) <= parameter){};
    if(check_time(start_time) >= parameter)
    {
        GPIOB_RESET_PIN2();
    
        switch (parameter)
        {
            case 100 : parameter = 800; break;
            case 800 : parameter = 2000; break;
            default : parameter = 100; break;
        }

        *start_time = GET_TIM2_CNT();
        *f = 0;
    }
    

}
#include "adc.h"

#define RCC_BASE (0x40021000)
#define RCC_APB2ENR (*(volatile uint32_t*) (RCC_BASE + 0x18))
#define RCC_CFGR (*(volatile uint32_t*) (RCC_BASE + 0x04))

#define ADC1_BASE (0x40012400)
#define ADC1_SR (*(volatile uint32_t*) (ADC1_BASE + 0x00))
#define ADC1_SMPR1 (*(volatile uint32_t*) (ADC1_BASE + 0x1C))
#define ADC1_CR2 (*(volatile uint32_t*) (ADC1_BASE + 0x08))
#define ADC1_SQR3 (*(volatile uint32_t*) (ADC1_BASE + 0x34))
#define ADC1_DR (*(volatile uint32_t*) (ADC1_BASE + 0x4C))

#define ADC1_CHANEL16 (16)
#define ADC1_CHANEL17 (17)
#define ADC1_APB2_CLOCK_EN (1 << 9)
#define ADC_PRE_2 (~(3 << 14))
#define ADC1_SMP (7 << 21)
#define ADC1_TSVREFE (1 << 23)
#define ADC1_EN (1 << 0)
#define ADC1_RSTCAL (1 << 3)
#define ADC1_CAL (1 << 2)
#define ADC1_DATA_RDY (1 << 1)

void init_adc(void)
{
    RCC_CFGR &= ADC_PRE_2;
    RCC_APB2ENR |= ADC1_APB2_CLOCK_EN;
    ADC1_SMPR1 &= ~ADC1_SMP;
    ADC1_SMPR1 |= ADC1_SMP;
    ADC1_SQR3 = ADC1_CHANEL17;
    ADC1_CR2 |= ADC1_TSVREFE;
    ADC1_CR2 |= ADC1_EN;
    ADC1_CR2 |= ADC1_RSTCAL;
    while (ADC1_CR2 & ADC1_RSTCAL);
    ADC1_CR2 |= ADC1_CAL;
    while (ADC1_CR2 & ADC1_CAL);
}

uint16_t read_adc(void)
{
    ADC1_CR2 |= ADC1_EN;
    while(!(ADC1_SR & ADC1_DATA_RDY));
    return (uint16_t)ADC1_DR;
}

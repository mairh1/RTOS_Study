#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"


void LED1_Config(void);
void LED_ON(uint16_t GPIO_Pin);
void LED_OFF(uint16_t GPIO_Pin);
#endif 

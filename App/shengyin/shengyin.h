#ifndef _shengyin_H
#define _shengyin_H

#include "delay.h"
#include "stm32f10x_it.h"	 
#include "system.h"
#define shengyin_IO GPIO_Pin_0  //K1 PE2

void shengyin_exti_init(void);
void shengyin_IO_init(void);
#endif
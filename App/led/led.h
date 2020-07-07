#ifndef _led_H
#define _led_H
#include "system.h"

void LED_Init(void);

#define led0 PCout(0)
#define led1 PCout(1)
#define led2 PCout(2)
#define led3 PCout(3)
#define led4 PCout(4)	
#endif

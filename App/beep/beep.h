#ifndef _beep_H
#define _beep_H
#include "system.h"
#include "SysTick.h"
#define beep GPIO_Pin_5	   //PB5
#define beep_io GPIOB	//PB¶Ë¿Ú
#define beepH GPIO_SetBits(beep_io,beep)
#define beepL GPIO_ResetBits(beep_io,beep)
void BEEP_Init(void);
void beep_sound(void);

#endif

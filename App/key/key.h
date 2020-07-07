#ifndef _key_H
#define _key_H
#include "system.h"

#define k_up GPIO_Pin_0	   //PA0
#define k_left GPIO_Pin_2	//PE2
#define k_down GPIO_Pin_3	//PE3
#define k_right GPIO_Pin_4	//PE4

#define K_UP GPIO_ReadInputDataBit(GPIOA,k_up)
#define K_LEFT GPIO_ReadInputDataBit(GPIOE,k_left)
#define K_DOWN GPIO_ReadInputDataBit(GPIOE,k_down)
#define K_RIGHT GPIO_ReadInputDataBit(GPIOE,k_right)

#define KEY_UP 1
#define KEY_LEFT 2
#define KEY_DOWN 3
#define KEY_RIGHT 4

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
#endif

#include "shengyin.h"

void shengyin_exti_init()  //外部中断初始化
{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	EXTI_InitTypeDef EXTI_InitStructure;

//	NVIC_InitTypeDef NVIC_InitStructure;

//	/* 开启GPIO时钟 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

//	GPIO_InitStructure.GPIO_Pin=shengyin_IO;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	 
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource0);//选择GPIO管脚用作外部中断线路
//	//此处一定要记住给端口管脚加上中断外部线路
//	/* 设置外部中断的模式 */ 
//	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure); 
//	
//	/* 设置NVIC参数 */	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		 
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 	//打开EXTI0的全局中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //响应优先级为0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		  //使能
//	NVIC_Init(&NVIC_InitStructure); 		
}

void shengyin_IO_init(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	    //使能PC端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);			     
}
#include "shengyin.h"

void shengyin_exti_init()  //�ⲿ�жϳ�ʼ��
{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	EXTI_InitTypeDef EXTI_InitStructure;

//	NVIC_InitTypeDef NVIC_InitStructure;

//	/* ����GPIOʱ�� */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

//	GPIO_InitStructure.GPIO_Pin=shengyin_IO;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	 
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource0);//ѡ��GPIO�ܽ������ⲿ�ж���·
//	//�˴�һ��Ҫ��ס���˿ڹܽż����ж��ⲿ��·
//	/* �����ⲿ�жϵ�ģʽ */ 
//	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure); 
//	
//	/* ����NVIC���� */	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		 
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 	//��EXTI0��ȫ���ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //��Ӧ���ȼ�Ϊ0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		  //ʹ��
//	NVIC_Init(&NVIC_InitStructure); 		
}

void shengyin_IO_init(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	    //ʹ��PC�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);			     
}
#ifndef __SPI_H
#define __SPI_H
#include "system.h"

							  
void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�


void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(uint8_t speed); //����SPI�ٶ�   
uint8_t SPI1_ReadWriteByte(uint8_t dat);//SPI���߶�дһ���ֽ�

		 
#endif


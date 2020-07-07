#include "lcd_driver.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "SysTick.h"	   


u8 lcddev_dir=0;
//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 
  
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
 
	 
//д�Ĵ�������
//regval:�Ĵ���ֵ
void LCD_WR_REG(u16 regval)
{   
#ifdef HX8347D	
	TFTLCD->LCD_REG=regval;//д��Ҫд�ļĴ������
#endif

#ifdef ST7793
	TFTLCD->LCD_REG=regval>>8;//д��Ҫд�ļĴ������
	TFTLCD->LCD_REG=regval&0xff;//д��Ҫд�ļĴ������
#endif
	 
}
//дLCD����
//data:Ҫд���ֵ
void LCD_WR_DATA(u16 data)
{	 
#ifdef HX8347D	
	TFTLCD->LCD_RAM=data;
#endif

#ifdef ST7793
	TFTLCD->LCD_RAM=data>>8;
	TFTLCD->LCD_RAM=data&0xff;
#endif		 
}
//��LCD����
//����ֵ:������ֵ
u16 LCD_RD_DATA(void)
{
#ifdef HX8347D
	u16 ram;			//��ֹ���Ż�
	ram=TFTLCD->LCD_RAM;	
	return ram;	 
#endif

#ifdef ST7793
 	u16 ram;		
	ram=TFTLCD->LCD_RAM;
	ram=ram<<8;
	ram|=TFTLCD->LCD_RAM;	
	return ram;	
#endif
}					   
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
#ifdef HX8347D
	TFTLCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 
	TFTLCD->LCD_RAM = LCD_RegValue;//д������	 
#endif

#ifdef ST7793
 	TFTLCD->LCD_REG = LCD_Reg>>8;		//д��Ҫд�ļĴ������
	TFTLCD->LCD_REG = LCD_Reg&0xff;	
	 
	TFTLCD->LCD_RAM = LCD_RegValue>>8;//д������
	TFTLCD->LCD_RAM = LCD_RegValue&0xff;			
#endif   		 
}	   
//���Ĵ���
//LCD_Reg:�Ĵ�����ַ
//����ֵ:����������
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
	delay_us(5);		  
	return LCD_RD_DATA();		//���ض�����ֵ
}   
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
#ifdef HX8347D 
 	TFTLCD->LCD_REG=lcddev.wramcmd;	
#endif

#ifdef ST7793
	TFTLCD->LCD_REG=lcddev.wramcmd>>8;
	TFTLCD->LCD_REG=lcddev.wramcmd&0xff;			
#endif  
}	 
//LCDдGRAM
//RGB_Code:��ɫֵ
void LCD_WriteRAM(u16 RGB_Code)
{							    
#ifdef HX8347D 	
	TFTLCD->LCD_RAM = RGB_Code;//дʮ��λGRAM
#endif

#ifdef ST7793
	TFTLCD->LCD_RAM = RGB_Code>>8;//дʮ��λGRAM
	TFTLCD->LCD_RAM = RGB_Code&0xff;
#endif 
}

//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD_ReadPoint(u16 x,u16 y)
{
#ifdef HX8347D 
 	u16 r=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���		     
	LCD_Set_Window(x, y, x, y);
	LCD_WR_REG(0X2E);     		 				    
 	r=LCD_RD_DATA();								//dummy Read	   
 	r=LCD_RD_DATA();  		  						//ʵ��������ɫ
 	return r;
#endif

#ifdef ST7793
 	u16 r=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���		     
	LCD_Set_Window(x, y, x, y);
	LCD_WR_REG(0x0202);     		 				    
 	r=LCD_RD_DATA();  		  						//ʵ��������ɫ
 	return r;
#endif						
}			 
   
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 	    
#ifdef HX8347D 
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(Xpos&0XFF); 			 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(Ypos&0XFF);
#endif

#ifdef ST7793
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(Xpos);			 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(Ypos); 
	
#endif		
} 	


//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_ClearColor(u16 Xpos, u16 Ypos,u16 Color)
{	 	    

	u32 index=0;      
	u32 totalpoint=lcddev.width-Xpos+1;
	totalpoint*=(lcddev.height-Ypos+1); 			//�õ��ܵ���
	LCD_SetCursor(Xpos,Ypos);	//���ù��λ�� 
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	
	for(index=0;index<totalpoint;index++)
	{
		#ifdef HX8347D
		TFTLCD->LCD_RAM=Color;
		#endif

		#ifdef ST7793
		TFTLCD->LCD_RAM=Color>>8;
		TFTLCD->LCD_RAM=Color&0xff;
		#endif	
	}
} 

//����LCD���Զ�ɨ�跽��	   	   
void LCD_Scan_Dir(u8 dir)
{
	if(dir==0)  //Ĭ����������
	{
#ifdef HX8347D			
		LCD_WR_REG(0x36);   //���ò�����ʾ����ļĴ���
		LCD_WR_DATA(0x4c);
		lcddev.height=480;
		lcddev.width=320;
#endif
	
#ifdef ST7793			
		LCD_WR_REG(0x0001);
		LCD_WR_DATA(0x0100);
		LCD_WR_REG(0x0003);
		LCD_WR_DATA(0x1030);
		lcddev.height=400;
		lcddev.width=240;
#endif

	}
	else
	{
#ifdef HX8347D		
		lcddev.height=320;
		lcddev.width=480;
		LCD_WR_REG(0x36);   
		LCD_WR_DATA(0x2c);
#endif
	
#ifdef ST7793
  		lcddev.height=240;
		lcddev.width=400; 
		LCD_WR_REG(0x0001);
		LCD_WR_DATA(0x0000);
		LCD_WR_REG(0x0003);
		LCD_WR_DATA(0x1038);
#endif		
	}	
}     
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_Set_Window(x, y, x, y);  //���õ��λ��
#ifdef HX8347D
	TFTLCD->LCD_RAM=POINT_COLOR;
#endif
	
#ifdef ST7793
  	TFTLCD->LCD_RAM=POINT_COLOR>>8;
	TFTLCD->LCD_RAM=POINT_COLOR&0xff;
#endif 
}
//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	LCD_Set_Window(x, y, x, y);
#ifdef HX8347D
	TFTLCD->LCD_RAM=color; 
#endif
	
#ifdef ST7793
	TFTLCD->LCD_RAM=color>>8; 
	TFTLCD->LCD_RAM=color&0xff; 
#endif	
}	 

//����LCD��ʾ����
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	lcddev.dir=dir;
	lcddev_dir=dir;
	LCD_Scan_Dir(dir);	//Ĭ��ɨ�跽��
}	 
//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height. 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{    
#ifdef HX8347D
	LCD_WR_REG(0x2A);

    LCD_WR_DATA(sx>>8);
    LCD_WR_DATA(sx&0XFF);
    LCD_WR_DATA(width>>8);
    LCD_WR_DATA(width&0XFF);

    LCD_WR_REG(0x2b);
    LCD_WR_DATA(sy>>8);
    LCD_WR_DATA(sy&0XFF);
    LCD_WR_DATA(height>>8);
    LCD_WR_DATA(height&0XFF);
    LCD_WR_REG(0x2c); 
#endif
	
#ifdef ST7793
	if(lcddev_dir==0)
	{
		LCD_WR_REG(0x0210);   
	    LCD_WR_DATA(sx);
		LCD_WR_REG(0x0211);  
	    LCD_WR_DATA(width);
		LCD_WR_REG(0x0212);   
	    LCD_WR_DATA(sy);
		LCD_WR_REG(0x0213);   
	    LCD_WR_DATA(height);
	
		LCD_WR_REG(0x0200);   
	    LCD_WR_DATA(sx);
		LCD_WR_REG(0x0201);   
	    LCD_WR_DATA(sy);	
	}   	
	else
	{
		LCD_WR_REG(0x0212);   
	    LCD_WR_DATA(sx);
		LCD_WR_REG(0x0213);  
	    LCD_WR_DATA(width);
		LCD_WR_REG(0x0210);   
	    LCD_WR_DATA(sy);
		LCD_WR_REG(0x0211);   
	    LCD_WR_DATA(height);
	
		LCD_WR_REG(0x0201);   
	    LCD_WR_DATA(sx);
		LCD_WR_REG(0x0200);   
	    LCD_WR_DATA(sy);	
	}

	LCD_WR_REG(0x0202);		
#endif
}
//��ʼ��lcd
void TFTLCD_Init(void)
{ 					
 	u16 i;
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  FSMC_ReadTimingInitStructure; 
	FSMC_NORSRAMTimingInitTypeDef  FSMC_WriteTimingInitStructure;
	
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//ʹ��FSMCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG,ENABLE);//ʹ��PORTB,D,E,G�Լ�AFIO���ù���ʱ��

 
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PB0 ������� ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
 	//PORTD�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
  	 
	//PORTE�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);    	    	 											 

   	//	//PORTG12����������� A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure); 

	FSMC_ReadTimingInitStructure.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns
  	FSMC_ReadTimingInitStructure.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
  	FSMC_ReadTimingInitStructure.FSMC_DataSetupTime = 0x0f;		 // ���ݱ���ʱ��Ϊ16��HCLK,��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�죬�����1289���IC��
  	FSMC_ReadTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
  	FSMC_ReadTimingInitStructure.FSMC_CLKDivision = 0x00;
  	FSMC_ReadTimingInitStructure.FSMC_DataLatency = 0x00;
  	FSMC_ReadTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    

	FSMC_WriteTimingInitStructure.FSMC_AddressSetupTime = 0x00;	 //��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK  
  	FSMC_WriteTimingInitStructure.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨A		
  	FSMC_WriteTimingInitStructure.FSMC_DataSetupTime = 0x03;		 ////���ݱ���ʱ��Ϊ4��HCLK	
  	FSMC_WriteTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
  	FSMC_WriteTimingInitStructure.FSMC_CLKDivision = 0x00;
  	FSMC_WriteTimingInitStructure.FSMC_DataLatency = 0x00;
  	FSMC_WriteTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 

 
  	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
  	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
  	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
  	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
  	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
  	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
  	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
  	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
  	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
  	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadTimingInitStructure; //��дʱ��
  	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_WriteTimingInitStructure;  //дʱ��

  	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // ʹ��BANK1 
		
 
	delay_ms(50); 					// delay 50 ms 
#ifdef HX8347D	
  	LCD_WR_REG(0Xd0);				   
	lcddev.id=LCD_RD_DATA();	//dummy read 	
	lcddev.id=LCD_RD_DATA();    
  	
 	printf(" LCD ID:%x\r\n",lcddev.id); //��ӡLCD ID   
	
	for(i=500; i>0; i--);
	LCD_WR_REG(0x11);
	for(i=500; i>0; i--);
	LCD_WR_REG(0xB9);
    LCD_WR_DATA(0xFF);
	LCD_WR_DATA(0x83);
	LCD_WR_DATA(0x57);
    for(i=500; i>0; i--);

	LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0xC3);
	LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x70);
	for(i=500; i>0; i--);
    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x22);
	LCD_WR_DATA(0x40);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x91);
	for(i=500; i>0; i--);
    LCD_WR_REG(0x36);   //���ò�����ʾ����ļĴ���
    LCD_WR_DATA(0x4c);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0x50);
	LCD_WR_DATA(0x50);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0xC8);
	LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x04);
     for(i=500; i>0; i--);
    for(i=500; i>0; i--);
    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0x0B); 
    LCD_WR_DATA(0x11); 
    LCD_WR_DATA(0x1E); 
    LCD_WR_DATA(0x30); 
    LCD_WR_DATA(0x3A); 
    LCD_WR_DATA(0x43); 
    LCD_WR_DATA(0x4E); 
    LCD_WR_DATA(0x56); 
    LCD_WR_DATA(0x45); 
    LCD_WR_DATA(0x3F); 
    LCD_WR_DATA(0x39); 
    LCD_WR_DATA(0x32); 
    LCD_WR_DATA(0x2F); 
    LCD_WR_DATA(0x2A); 
    LCD_WR_DATA(0x29); 
    LCD_WR_DATA(0x21); 
    LCD_WR_DATA(0x0B); 
    LCD_WR_DATA(0x11); 
    LCD_WR_DATA(0x1E); 
    LCD_WR_DATA(0x30); 
    LCD_WR_DATA(0x3A); 
    LCD_WR_DATA(0x43); 
    LCD_WR_DATA(0x4E); 
    LCD_WR_DATA(0x56); 
    LCD_WR_DATA(0x45); 
    LCD_WR_DATA(0x3F); 
    LCD_WR_DATA(0x39); 
    LCD_WR_DATA(0x32); 
    LCD_WR_DATA(0x2F); 
    LCD_WR_DATA(0x2A); 
    LCD_WR_DATA(0x29); 
    LCD_WR_DATA(0x21); 
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x01); 

    LCD_WR_REG(0x21);
 	LCD_WR_REG(0x3a);
 	LCD_WR_DATA(0x05);

    LCD_WR_REG(0x29);
	
	lcddev.wramcmd=0X2C;
	lcddev.setxcmd=0X2A;
	lcddev.setycmd=0X2B;
#endif


#ifdef ST7793
 	LCD_WR_REG(0X0000);				   	
	lcddev.id=LCD_RD_DATA();    
  	
 	printf(" LCD ID:%x\r\n",lcddev.id); //��ӡLCD ID	

	for(i=500; i>0; i--);
	LCD_WR_REG(0x0001);
	LCD_WR_DATA(0x0100);
	LCD_WR_REG(0x0003);
	LCD_WR_DATA(0x1030); //16bit 565 
	
	LCD_WR_REG(0x0008);
	LCD_WR_DATA(0x0808);
	LCD_WR_REG(0x0090);
	LCD_WR_DATA(0x8000);
	LCD_WR_REG(0x0400);
	LCD_WR_DATA(0x6200);
	LCD_WR_REG(0x0401);
	LCD_WR_DATA(0x0001);

	LCD_WR_REG(0x00ff);
	LCD_WR_DATA(0x0001);
	LCD_WR_REG(0x0102);
	LCD_WR_DATA(0x01b0);
	LCD_WR_REG(0x0710);
	LCD_WR_DATA(0x0016);
	LCD_WR_REG(0x0712);
	LCD_WR_DATA(0x000f);
	LCD_WR_REG(0x0752);
	LCD_WR_DATA(0x002f);
	LCD_WR_REG(0x0724);
	LCD_WR_DATA(0x001a);
	LCD_WR_REG(0x0754);	
	LCD_WR_DATA(0x0018);
	for(i=500; i>0; i--);
    
    /* Gamma */
	LCD_WR_REG(0x0380);
	LCD_WR_DATA(0x0000);
	LCD_WR_REG(0x0381);
	LCD_WR_DATA(0x5F10);//5f10
	LCD_WR_REG(0x0382);
	LCD_WR_DATA(0x0B02);//0b02
	LCD_WR_REG(0x0383);
	LCD_WR_DATA(0x0614);
	LCD_WR_REG(0x0384);
	LCD_WR_DATA(0x0111);
	LCD_WR_REG(0x0385);
	LCD_WR_DATA(0x0000);
	LCD_WR_REG(0x0386);
	LCD_WR_DATA(0xA90B);//a90b
	LCD_WR_REG(0x0387);
	LCD_WR_DATA(0x0606);//0606
	LCD_WR_REG(0x0388);
	LCD_WR_DATA(0x0612);
	LCD_WR_REG(0x0389);
	LCD_WR_DATA(0x0111);	  


    /* Gamma Setting */
    /* Setting */
	LCD_WR_REG(0x0702);
	LCD_WR_DATA(0x003b);//003b
	LCD_WR_REG(0x00ff);
	LCD_WR_DATA(0x0000);


    /* Vcom Setting */
	LCD_WR_REG(0x0007);
	LCD_WR_DATA(0x0100);
	for(i=500; i>0; i--); 
	LCD_WR_REG(0x0200);
	LCD_WR_DATA(0x0000);
	LCD_WR_REG(0x0201);                  
	LCD_WR_DATA(0x0000);

	lcddev.wramcmd=0x0202;
	lcddev.setxcmd=0x0200;
	lcddev.setycmd=0x0201;

#endif
	
	LCD_Display_Dir(0);		//Ĭ������
	LCD_Clear(WHITE);
}  
//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//�õ��ܵ���
	LCD_SetCursor(0x00,0x00);	//���ù��λ�� 
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
#ifdef HX8347D
		TFTLCD->LCD_RAM=color;
#endif

#ifdef ST7793
		TFTLCD->LCD_RAM=color>>8;
		TFTLCD->LCD_RAM=color&0xff;
#endif	
	}
}  
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(u16 xState,u16 yState,u16 xEnd,u16 yEnd,u16 color)
{          
	uint16_t temp;

    if((xState > xEnd) || (yState > yEnd))
    {
        return;
    }   
	LCD_Set_Window(xState, yState, xEnd, yEnd); 
    xState = xEnd - xState + 1;
	yState = yEnd - yState + 1;

	while(xState--)
	{
	 	temp = yState;
		while (temp--)
	 	{	
#ifdef HX8347D			
			TFTLCD->LCD_RAM=color;
#endif

#ifdef ST7793
			TFTLCD->LCD_RAM=color>>8;
			TFTLCD->LCD_RAM=color&0xff;
#endif
		}
	}	
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)
		{
#ifdef HX8347D			
			TFTLCD->LCD_RAM=color[i*width+j];//д������
#endif

#ifdef ST7793
		   	TFTLCD->LCD_RAM=color[i*width+j]>>8;//д������
			TFTLCD->LCD_RAM=color[i*width+j]&0xff;
#endif
		}
		 
	}		  
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}  

// ��һ��ʮ�ֵı��
// x����ǵ�X����
// y����ǵ�Y����
// color����ǵ���ɫ
void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t i;

    /* ���� */
    LCD_Set_Window(x-1, y-1, x+1, y+1);
    for(i=0; i<9; i++)
    {
        LCD_WriteRAM(color);    
    }

    /* ���� */
    LCD_Set_Window(x-4, y, x+4, y);
    for(i=0; i<9; i++)
    {
        LCD_WriteRAM(color);    
    }

    /* ���� */
    LCD_Set_Window(x, y-4, x, y+4);
    for(i=0; i<9; i++)
    {
        LCD_WriteRAM(color);    
    }
}
  
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}






































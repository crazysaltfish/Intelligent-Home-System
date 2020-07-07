#include "system.h"
#include "SysTick.h"
#include "stm32f10x_it.h"	 
#include "usart.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "ds18b20.h"
#include "esp8266_drive.h"
#include "sta_tcpclent_test.h"
#include "lcd_driver.h"
#include "gui.h"
#include "dht11.h"
#include "shengyin.h"

int main()
{  	
	u8 i;
	Systick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART1_Init(115200);
	LED_Init();
	KEY_Init();
	//TFT_Init();
  //lcd_display();
	shengyin_IO_init();
  DHT11_Init();	
	DS18B20_Init();
	BEEP_Init();

	ESP8266_Init(115200);
	ESP8266_STA_TCPClient_Test();

}





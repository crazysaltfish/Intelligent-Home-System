#include "sta_tcpclent_test.h"
#include "SysTick.h"
#include "usart.h"
#include "esp8266_drive.h"
#include "ds18b20.h"
#include "beep.h"
#include "led.h"
#include "lcd_driver.h"
#include "gui.h"
#include "dht11.h"
#include <string.h>  

volatile u8 TcpClosedFlag = 0;

void data_pros()	//数据处理函数
{
	u8 temp;  	    
	u8 humi;
	DHT11_Read_Data(&temp,&humi);
}

void ESP8266_STA_TCPClient_Test(void)
{
	u8 res;
	u8 ucId;
	u8 temp;  	    
	u8 humi;
	char cStr [ 100 ] = { 0 };
	short read_temp=0;
	float rtemp;
	
	char str[100]={0};
	char * pCh;
	
	int shengyin_count=0;
	bool voice_flag=1,guangmin_flag=1,hongwai_flag=1;
	bool adminer_flag=0;
	
	printf ( "\r\n正在配置ESP8266请耐心等待...\r\n" );

	ESP8266_CH_PD_Pin_SetH;

	ESP8266_AT_Test();
//  ESP8266_Net_Mode_Choose(STA);
//	while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));
//	ESP8266_Enable_MultipleId ( DISABLE );
//	while(!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0));
//	
//	while(!ESP8266_UnvarnishSend());
//	printf ( "\r\n配置ESP8266 OK！\r\n" );
    ESP8266_Net_Mode_Choose ( AP );  //设置AP模式
	  while ( ! ESP8266_CIPAP ( macUser_ESP8266_TcpServer_IP ) );//服务器开启的IP地址
	  while ( ! ESP8266_BuildAP ( macUser_ESP8266_BulitApSsid, macUser_ESP8266_BulitApPwd,
		      macUser_ESP8266_BulitApEcn ) );	//WF-ESP8266模块创建WiFi热点  WiFi名称字符串  WiFi密码字符串  WiFi加密方式代号字符串
	  ESP8266_Enable_MultipleId ( ENABLE );
	  while ( !	ESP8266_StartOrShutServer ( ENABLE, macUser_ESP8266_TcpServer_Port, macUser_ESP8266_TcpServer_OverTime ) );
		ESP8266_Inquire_ApIp ( cStr, 20 );
	printf ( "\r\n本模块WIFI为%s，密码开放\r\nAP IP 为：%s，开启的端口为：%s\r\n手机网络助手连接该 IP 和端口，最多可连接5个客户端\r\n",
           macUser_ESP8266_BulitApSsid, cStr, macUser_ESP8266_TcpServer_Port );
		
	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
	ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 0;			
		
	
	while ( 1 )
	{		
		if ( ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag  )
		{
			USART_ITConfig ( USART2, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
			ESP8266_Fram_Record_Struct .Data_RX_BUF [ ESP8266_Fram_Record_Struct .InfBit .FramLength ]  = '\0';
			USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清除中断标志位
			
			if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "1x" ) ) != 0 ) 
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
				led0=!led0;
			}
			
			else if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "2x" ) ) != 0 ) 
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
				adminer_flag=!adminer_flag;
				led1=!led1;
			}
			
			else if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "4x" ) ) != 0 ) 
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
				beep_sound();
			}
		
			else if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "3x" ) ) != 0 ) 
			{
				//beep_sound();
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
				DHT11_Read_Data(&temp,&humi);
				//将数据 以空格分开 写进一个字符串里，在android端解析。
		    sprintf(str,"%d %d",temp,humi);
				if(ESP8266_SendString ( DISABLE, str, strlen ( str ), ( ENUM_ID_NO_TypeDef ) ucId ))
					beep_sound();
			}
			
			else if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "5x" ) ) != 0 ) 
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
				if(voice_flag==0) voice_flag=1;
				else voice_flag=0;
			}
			
			else if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "6x" ) ) != 0 ) 
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
				if(guangmin_flag==0) guangmin_flag=1;
				else guangmin_flag=0;
			}
			
			else if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "7x" ) ) != 0 ) 
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
				if(hongwai_flag==0) hongwai_flag=1;
				else hongwai_flag=0;
			}
			
			ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
	    ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 0;	
			
			USART_ITConfig ( USART2, USART_IT_RXNE, ENABLE ); //使能串口接收中断
			
			
		}
		
		if(voice_flag&&guangmin_flag){
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)==Bit_SET)	   //监测到光照低于阈值
			{
				if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)==Bit_RESET)	   //监测到声音
				{
					if(led1==1){
						led1=0;
						shengyin_count=0;
						sprintf(str,"voice notify");
						ESP8266_SendString ( DISABLE, str, strlen ( str ), ( ENUM_ID_NO_TypeDef ) ucId );
					}
				
				} 
			}
		}
		if(voice_flag&&!guangmin_flag){
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)==Bit_RESET)	   //监测到声音
					{
						if(led1==1){
							led1=0;
							shengyin_count=0;
							sprintf(str,"voice notify");
							ESP8266_SendString ( DISABLE, str, strlen ( str ), ( ENUM_ID_NO_TypeDef ) ucId );
						}
					
					}
					
		}
		if(!voice_flag&&guangmin_flag){
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)==Bit_SET)	   //监测到光照低于阈值
				{
					
						if(led1==1){
							led1=0;
							shengyin_count=0;
							sprintf(str,"guangzhao notify");
							ESP8266_SendString ( DISABLE, str, strlen ( str ), ( ENUM_ID_NO_TypeDef ) ucId );
						}
					
				}
		}
		if(adminer_flag==0)
		if(led1==0){			
						shengyin_count++;
						if(shengyin_count>=2000000) {  //点亮延时
							shengyin_count=0;
							led1=1;
						}
		} 
		
		if(hongwai_flag){
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)==Bit_SET)	   //监测到人体红外线
			{
				led2=0;
			}else{
				led2=1;
			}
		}
		
	  
		/***********************DS18B20*******************************/
		//read_temp=DS18B20_Get_Temp();
		//rtemp=(float)read_temp*1.0/10;
		//sprintf(str,"temp=%.1fC\r\n",rtemp);
		//sprintf (str,"www.prechin.cn\r\n" );
		//ESP8266_SendString ( DISABLE, str, strlen (str ), Multiple_ID_0 );
		//DHT11_Read_Data(&temp,&humi);
		//sprintf(str,"\r\n温度为 %d°C,湿度为：%d%% \r\n",temp,humi);
		//ESP8266_SendString ( DISABLE, str, strlen (str ), Multiple_ID_0 );	
		//printf("\r\n%d  , %s\r\n",ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag,ESP8266_Fram_Record_Struct .Data_RX_BUF);
		//delay_ms(1000);
		//led0=!led0;
		
		//printf("\r\n温度为 %d°C,湿度为： %d%% \r\n",temp,humi);
		if(TcpClosedFlag) //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend(); //退出透传模式
			do
			{
				res = ESP8266_Get_LinkStatus();     //获取连接状态
			} 	
			while(!res);
			
			if(res == 4)                     //确认失去连接后重连
			{
				printf ( "\r\n请稍等，正在重连热点和服务器...\r\n" );
				
				while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
				
				while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );
			} 
			while(!ESP8266_UnvarnishSend());					
		}
	}
		
}




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

void data_pros()	//���ݴ�����
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
	
	printf ( "\r\n��������ESP8266�����ĵȴ�...\r\n" );

	ESP8266_CH_PD_Pin_SetH;

	ESP8266_AT_Test();
//  ESP8266_Net_Mode_Choose(STA);
//	while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));
//	ESP8266_Enable_MultipleId ( DISABLE );
//	while(!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0));
//	
//	while(!ESP8266_UnvarnishSend());
//	printf ( "\r\n����ESP8266 OK��\r\n" );
    ESP8266_Net_Mode_Choose ( AP );  //����APģʽ
	  while ( ! ESP8266_CIPAP ( macUser_ESP8266_TcpServer_IP ) );//������������IP��ַ
	  while ( ! ESP8266_BuildAP ( macUser_ESP8266_BulitApSsid, macUser_ESP8266_BulitApPwd,
		      macUser_ESP8266_BulitApEcn ) );	//WF-ESP8266ģ�鴴��WiFi�ȵ�  WiFi�����ַ���  WiFi�����ַ���  WiFi���ܷ�ʽ�����ַ���
	  ESP8266_Enable_MultipleId ( ENABLE );
	  while ( !	ESP8266_StartOrShutServer ( ENABLE, macUser_ESP8266_TcpServer_Port, macUser_ESP8266_TcpServer_OverTime ) );
		ESP8266_Inquire_ApIp ( cStr, 20 );
	printf ( "\r\n��ģ��WIFIΪ%s�����뿪��\r\nAP IP Ϊ��%s�������Ķ˿�Ϊ��%s\r\n�ֻ������������Ӹ� IP �Ͷ˿ڣ���������5���ͻ���\r\n",
           macUser_ESP8266_BulitApSsid, cStr, macUser_ESP8266_TcpServer_Port );
		
	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
	ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 0;			
		
	
	while ( 1 )
	{		
		if ( ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag  )
		{
			USART_ITConfig ( USART2, USART_IT_RXNE, DISABLE ); //���ô��ڽ����ж�
			ESP8266_Fram_Record_Struct .Data_RX_BUF [ ESP8266_Fram_Record_Struct .InfBit .FramLength ]  = '\0';
			USART_ClearITPendingBit(USART3,USART_IT_RXNE); //����жϱ�־λ
			
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
				//������ �Կո�ֿ� д��һ���ַ������android�˽�����
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
			
			USART_ITConfig ( USART2, USART_IT_RXNE, ENABLE ); //ʹ�ܴ��ڽ����ж�
			
			
		}
		
		if(voice_flag&&guangmin_flag){
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)==Bit_SET)	   //��⵽���յ�����ֵ
			{
				if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)==Bit_RESET)	   //��⵽����
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
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)==Bit_RESET)	   //��⵽����
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
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)==Bit_SET)	   //��⵽���յ�����ֵ
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
						if(shengyin_count>=2000000) {  //������ʱ
							shengyin_count=0;
							led1=1;
						}
		} 
		
		if(hongwai_flag){
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)==Bit_SET)	   //��⵽���������
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
		//sprintf(str,"\r\n�¶�Ϊ %d��C,ʪ��Ϊ��%d%% \r\n",temp,humi);
		//ESP8266_SendString ( DISABLE, str, strlen (str ), Multiple_ID_0 );	
		//printf("\r\n%d  , %s\r\n",ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag,ESP8266_Fram_Record_Struct .Data_RX_BUF);
		//delay_ms(1000);
		//led0=!led0;
		
		//printf("\r\n�¶�Ϊ %d��C,ʪ��Ϊ�� %d%% \r\n",temp,humi);
		if(TcpClosedFlag) //����Ƿ�ʧȥ����
		{
			ESP8266_ExitUnvarnishSend(); //�˳�͸��ģʽ
			do
			{
				res = ESP8266_Get_LinkStatus();     //��ȡ����״̬
			} 	
			while(!res);
			
			if(res == 4)                     //ȷ��ʧȥ���Ӻ�����
			{
				printf ( "\r\n���Եȣ����������ȵ�ͷ�����...\r\n" );
				
				while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
				
				while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );
				
				printf ( "\r\n�����ȵ�ͷ������ɹ�\r\n" );
			} 
			while(!ESP8266_UnvarnishSend());					
		}
	}
		
}




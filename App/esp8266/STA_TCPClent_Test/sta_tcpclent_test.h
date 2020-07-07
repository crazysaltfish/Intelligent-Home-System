#ifndef _sta_tcpclent_test_H
#define _sta_tcpclent_test_H


#include "system.h"


#define User_ESP8266_SSID	  "123456"	      //Ҫ���ӵ��ȵ������
#define User_ESP8266_PWD	  "201419106"	  //Ҫ���ӵ��ȵ������

#define User_ESP8266_TCPServer_IP	  "192.168.1.103"	  //Ҫ���ӵķ�������IP
#define User_ESP8266_TCPServer_PORT	  "8080"	  //Ҫ���ӵķ������Ķ˿�




#define   macUser_ESP8266_BulitApSsid         "BinghuoLink"      //Ҫ�������ȵ������
#define   macUser_ESP8266_BulitApEcn           OPEN               //Ҫ�������ȵ�ļ��ܷ�ʽ
#define   macUser_ESP8266_BulitApPwd           "wildfire"         //Ҫ�������ȵ����Կ

#define   macUser_ESP8266_TcpServer_IP         "192.168.123.169"      //������������IP��ַ
#define   macUser_ESP8266_TcpServer_Port       "8080"             //�����������Ķ˿�   

#define   macUser_ESP8266_TcpServer_OverTime   "1800"             //��������ʱʱ�䣨��λ���룩

#define   User_ESP8266_IP                    "192.168.123.69"       //�ֻ���IP��ַ
#define   User_ESP8266_PORT                    "5000"       //�ֻ���IP��ַ



extern volatile uint8_t TcpClosedFlag;  //����һ��ȫ�ֱ���


void ESP8266_STA_TCPClient_Test(void);



#endif

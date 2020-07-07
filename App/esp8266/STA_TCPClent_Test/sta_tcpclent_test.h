#ifndef _sta_tcpclent_test_H
#define _sta_tcpclent_test_H


#include "system.h"


#define User_ESP8266_SSID	  "123456"	      //要连接的热点的名称
#define User_ESP8266_PWD	  "201419106"	  //要连接的热点的密码

#define User_ESP8266_TCPServer_IP	  "192.168.1.103"	  //要连接的服务器的IP
#define User_ESP8266_TCPServer_PORT	  "8080"	  //要连接的服务器的端口




#define   macUser_ESP8266_BulitApSsid         "BinghuoLink"      //要建立的热点的名称
#define   macUser_ESP8266_BulitApEcn           OPEN               //要建立的热点的加密方式
#define   macUser_ESP8266_BulitApPwd           "wildfire"         //要建立的热点的密钥

#define   macUser_ESP8266_TcpServer_IP         "192.168.123.169"      //服务器开启的IP地址
#define   macUser_ESP8266_TcpServer_Port       "8080"             //服务器开启的端口   

#define   macUser_ESP8266_TcpServer_OverTime   "1800"             //服务器超时时间（单位：秒）

#define   User_ESP8266_IP                    "192.168.123.69"       //手机的IP地址
#define   User_ESP8266_PORT                    "5000"       //手机的IP地址



extern volatile uint8_t TcpClosedFlag;  //定义一个全局变量


void ESP8266_STA_TCPClient_Test(void);



#endif

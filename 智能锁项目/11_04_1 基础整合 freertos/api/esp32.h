#ifndef __ESP32_H_
#define __ESP32_H_	

#include "stm32f4xx.h"                  // Device header
//#include <malloc.h>
#include "usart.h"
#include "MQTTPacket.h"
#include "string.h"
#include "Delay.h"
#include "my_spi_flash.h"
#include "cJSON.h"

typedef struct {
	uint8_t RX_data[1024];//串口接收数据
	uint16_t	RX_count;			//串口计数器
	uint8_t RX_flag;			//接收标志位
	uint8_t tx_buf[1024];	//发送缓冲区
	uint16_t tx_count;		//发送计数器
}esp;


extern esp esp8266;	//存放8266接收信息



extern uint8_t wifi_name[32] 			;
extern uint8_t wifi_passwd[32]     ;
												
extern uint32_t write_wifi_addr    ;
extern uint32_t write_passwd_addr  ;
													
extern uint32_t write_len_addr     ;
extern uint8_t read_len[3]         ;



#define IP_aliyum	"\"iot-060a7g0x.mqtt.iothub.aliyuncs.com\""
#define PORT_aliyun	1883
#define IP_seniverse	" api.m.taobao.com "
#define PORT_seniverse	80


#define my_client_id 	"k0ay8rVM7G3.Smart_Lock1|securemode=2,signmethod=hmacsha256,timestamp=1698507660888|"
#define my_username   "Smart_Lock1&k0ay8rVM7G3"
#define my_password		"343200ead45fc6e5bc5a09dd6d8ff771c542e4854507b4d1aa39738b30014041"

#define property_publish_cstring "/sys/k0ay8rVM7G3/Smart_Lock1/thing/event/property/post"
//#define property_payload "{\"id\":1696924234768,\"params\":{\"temperature\":77,\"Humidity\":99},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}"
//#define property_payload "{\"id\":1697335968205,\"params\":{\"temperature\":%f,\"Humidity\":%f,\"HCHO\":%lf,\"co2\":%d,\"TVOC\":%f},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}"
#define property_payload "{\"id\":1698507514541,\"params\":{\"DoorPwd\":\"666666\",\"AdmPwd\":\"888888\",\"NewCard\":\"0\",\"DelCard\":\"0\",\"VoicePwr\":8,\"CardSum\":0,\"FingerprintNum\":0,\"DeviceSn\":\"12345678\"},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}"

#define property_subscribe_cstring	"/sys/k0ay8rVM7G3/Smart_Lock1/thing/service/property/set"

#define time_publish_cstring "/ext/ntp/k0ay8rVM7G3/Smart_Lock1/request"
#define	time_payload	"{\"deviceSendTime\":\"1571724098000\"}"
#define time_subscribe_cstring	"/ext/ntp/k0ay8rVM7G3/Smart_Lock1/response"


uint8_t mqtt_connect_server(void);
uint8_t mqtt2server(char* cstr, char *payl, char * reply);
void subscription_service(void);
void subscription_analisys(void);
void send_data2server(void);
void whther_analisys(void);
void xinzhi_analisys(void);
void AP_analisys(void);


void esp8266_init(void);
void usart12usart3(void);
uint8_t soft2esp(char *AT_cmd, char *AT_retval, uint16_t AT_time);
uint8_t wifi_init(void);
uint32_t get_server_time(void);
uint8_t connect_seniverse(void);
uint8_t ap_init(void);

void wifi_write(char *wifiname, char *wifipasswd);
void esp32_init(void);
void aliyun_analisys(void);


void send2server(char * data);


#endif

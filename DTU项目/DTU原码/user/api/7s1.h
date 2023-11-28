#ifndef __7S1_H_
#define __7S1_H_

#include "stm32f10x.h"                  // Device header
#include "MQTTPacket.h"
#include <string.h>
#include "usart.h"
#include "delay.h"
#include "Modbus.h"
#include "myjson.h"

typedef struct {
	uint8_t RX_data[2048];//���ڽ�������
	uint16_t	RX_count;			//���ڼ�����
	uint8_t RX_flag;			//���ձ�־λ
	uint8_t tx_buf[1024];	//���ͻ�����
	uint16_t tx_count;		//���ͼ�����
}esp;

extern esp esp8266;	//���8266������Ϣ
extern esp json_data;
//Socket����
#define SOCKETA_EN     1
#define SOCKETB_EN     1
#define SOCKETC_EN     0
#define SOCKETD_EN     0

//��֪����
#if SOCKETA_EN

#define SOCKETA_IP     "api.seniverse.com"
#define SOCKETA_PORT   80
#define SOCKETA_ID     0x61

#endif

//������
#if SOCKETB_EN

#define SOCKETB_IP     "iot-06z00ak627vyl6x.mqtt.iothub.aliyuncs.com"
#define SOCKETB_PORT   1883
#define SOCKETB_ID     0x62

#define my_client_id 	"k0ay8hbzmtt.DTUtest|securemode=2,signmethod=hmacsha256,timestamp=1699445191470|"
#define my_username   "DTUtest&k0ay8hbzmtt"
#define my_password		"98b380b6d8bd153d4e5c03bdaadb1202e602388626e11654cc9caa83d56b5653"

#define property_publish_cstring "/sys/k0ay8hbzmtt/DTUtest/thing/event/property/post"
#define property_payload "{\"id\":1699495260196,\"params\":{\"temperature\":%d,\"Humidity\":%d,\"HCHO\":%d,\"co2\":%d},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}"
//#define property_payload "{\"id\":1697335968205,\"params\":{\"temperature\":%f,\"Humidity\":%f,\"HCHO\":%lf,\"co2\":%d,\"TVOC\":%f},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}"

#define property_subscribe_cstring	"/sys/k0ay8hbzmtt/DTUtest/thing/service/property/set"

#define time_publish_cstring "/ext/ntp/k0ay8hbzmtt/DTUtest/request"
#define	time_payload	"{\"deviceSendTime\":\"1571724098000\"}"
#define time_subscribe_cstring	"/ext/ntp/k0ay8hbzmtt/DTUtest/response"

#endif



void mqtt_connect_server(void);
void subscription_service(void);
uint8_t SendCmdAndCheckRet(char *AT_cmd, char *AT_retval, uint16_t AT_time);
void cat_data_send(uint8_t socket_id, uint8_t *send_buf, uint16_t len);
uint8_t cat_data_analysis(uint8_t socket_id, uint8_t *data);
void Cat_Connect_Net(void);
void updata_aliyun(void);
void get_weather(void);
void Cat_Connect_Modify(void);


#endif

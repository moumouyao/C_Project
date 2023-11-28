#ifndef __MY_MQTT_H_
#define __MY_MQTT_H_

#include "stm32f10x.h"                  // Device header
#include "MQTTPacket.h"
#include "esp8266.h"
#include "my_spi_flash.h"

//typedef struct{
//	uint8_t tmp[4];
//	uint8_t hum[4];
//	uint8_t VOC[4];
//	uint8_t CHO[4];
//	uint8_t CO2[4];
//}SER_VAL;

#define my_client_id 	"k0aw2dYkbol.air_test1|securemode=2,signmethod=hmacsha256,timestamp=1696907245386|"
#define my_username   "air_test1&k0aw2dYkbol"
#define my_password		"865926756fbae01d6983fda8a2c61530fef8f1598824bebfaf05ec66ad854ac6"

#define property_publish_cstring "/sys/k0aw2dYkbol/air_test1/thing/event/property/post"
//#define property_payload "{\"id\":1696924234768,\"params\":{\"temperature\":77,\"Humidity\":99},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}"
#define property_payload "{\"id\":1697335968205,\"params\":{\"temperature\":%f,\"Humidity\":%f,\"HCHO\":%lf,\"co2\":%d,\"TVOC\":%f},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}"

#define property_subscribe_cstring	"/sys/k0aw2dYkbol/air_test1/thing/service/property/set"

#define time_publish_cstring "/ext/ntp/k0aw2dYkbol/air_test1/request"
#define	time_payload	"{\"deviceSendTime\":\"1571724098000\"}"
#define time_subscribe_cstring	"/ext/ntp/k0aw2dYkbol/air_test1/response"


uint8_t mqtt_connect_server(void);
uint8_t mqtt2server(char* cstr, char *payl, char * reply);
void subscription_service(void);
void subscription_analisys(void);
void send_data2server(void);
void whther_analisys(void);
void xinzhi_analisys(void);
void AP_analisys(void);


#endif

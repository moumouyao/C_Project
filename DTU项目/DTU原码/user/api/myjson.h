#ifndef __MYJSON_H_
#define __MYJSON_H_

#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include <cJSON.h>
#include "7s1.h"
#include "myjson.h"


typedef struct {
	uint8_t IP[512];
	int port;
	int socket;
	uint8_t PostUrl[512];
	uint8_t GetUrl[512];
	
}http;


typedef struct {
	uint8_t IP[512];
	int port;
	int socket;
	uint8_t clientId[512];
	uint8_t username[512];
	uint8_t passwd[512];
	uint8_t publishTopic[512];
	int subTopicCount;
	uint8_t subTopicItem[5][512];
	int unsubTopicCount;
	uint8_t unsubTopicItem[5][512];
	int RegIdentCount;
	uint8_t RegIdentItem[5][512];

}mqtt;	



typedef struct {
	int modbusIdValue;
	int regNumValue;
	int regStartAddrValue;
	
	uint8_t mode[512];
	http http_data;
	mqtt mqtt_data;

}json_recv;


extern json_recv js_re_data;




uint8_t json_analysis(void);

#endif

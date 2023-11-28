#include "myjson.h"

// char* json = "{\"modbusID\": 1,\"regNum\": 3,\"regstartAddr\": 0,\"mqtt\": {\"IP\": \"a12gvpODyql.iot-as-mqtt.cn-shanghai.aliyuncs.com\",\"port\": 1883,\"clientId\": \"a12gvpODyql.device01|securemode=2,signmethod=hmacsha256,timestamp=1692690546143|\",\"username\": \"device01&a12gvpODyql\",\"passwd\": \"715d3b3f5666a55ab5bf9740b9d337db9392d12b4bb552909b8a49aaf1f832db\",\"publishTopic\": \"/sys/a12gvpODyql/device01/thing/event/property/post\",\"subTopic\": [\"/sys/a12gvpODyql/device01/thing/service/property/set\",\"/sys/a12gvpODyql/device01/thing/event/property/post_reply\"],\"unsubTopic\": [\"/sys/a12gvpODyql/device01/thing/event/property/post_reply\"],\"RegIdent\": [\"tem\",\"humi\",\"light\"]},\"http\": {\"IP\": \"api.seniverse.com\",\"port\": 80,\"PostUrl\": \"POST https://api.senivere.com/v3\",\"GetUrl\": \"GET https://api.seniverse.com/v3/weather/now.json?key=S7JvpeeJHZ7SJSYsP&location=zhengzhou&language=en&unit=c\"},\"mode\": \"http\"}";
esp json_data;

json_recv js_re_data;

uint8_t json_analysis(void){
  // 解析JSON字符串
  cJSON* root = cJSON_Parse((char *)json_data.RX_data);
  if (!root) {
    printf("JSON解析失败\r\n");
		printf("JSON格式错误:%s\r\n", cJSON_GetErrorPtr()); //输出json格式错误信息
    return -1;
  }

  // 解析modbusID
  cJSON* modbusID = cJSON_GetObjectItem(root, "modbusID");
  if (cJSON_IsNumber(modbusID)) {
		js_re_data.modbusIdValue = modbusID->valueint;
//    int modbusIdValue = modbusID->valueint;
    printf("modbusId: %d\r\n", js_re_data.modbusIdValue);
  }

  // 解析regNum
  cJSON* regNum = cJSON_GetObjectItem(root, "regNum");
  if (cJSON_IsNumber(regNum)) {
		js_re_data.regNumValue = regNum->valueint;
//    int regNumValue = regNum->valueint;
    printf("regNum: %d\r\n", js_re_data.regNumValue);
  }

  // 解析regstartAddr
  cJSON* regStartAddr = cJSON_GetObjectItem(root, "regstartAddr");
  if (cJSON_IsNumber(regStartAddr)) {
		js_re_data.regStartAddrValue = regStartAddr->valueint;
//    int regStartAddrValue = regStartAddr->valueint;
    printf("regStartAddr: %d\r\n", js_re_data.regStartAddrValue);
  }

  // 解析mqtt对象
  cJSON* mqtt = cJSON_GetObjectItem(root, "mqtt");
  if (cJSON_IsObject(mqtt)) {
    
    // 解析mqtt.IP
    cJSON* mqttIP = cJSON_GetObjectItem(mqtt, "IP");
    if (cJSON_IsString(mqttIP)) {
			
			strcpy((char *)js_re_data.mqtt_data.IP, mqttIP->valuestring);
      printf("mqtt.IP: %s\r\n", mqttIP->valuestring);
    }
		
		// 解析mqtt.socket
    cJSON* mqttsocket = cJSON_GetObjectItem(mqtt, "socket");
    if (cJSON_IsNumber(mqttsocket)) {
			js_re_data.mqtt_data.socket = mqttsocket->valueint;
      printf("mqtt.socket: %d\r\n", mqttsocket->valueint);
    }
    
    // 解析mqtt.port
    cJSON* mqttPort = cJSON_GetObjectItem(mqtt, "port");
    if (cJSON_IsNumber(mqttPort)) {
			js_re_data.mqtt_data.port = mqttPort->valueint;
      printf("mqtt.port: %d\r\n", mqttPort->valueint);
    }

    // 解析mqtt.clientId
    cJSON* clientId = cJSON_GetObjectItem(mqtt, "clientId");
    if (cJSON_IsString(clientId)) {
			strcpy((char *)js_re_data.mqtt_data.clientId, clientId->valuestring);
      printf("mqtt.clientId: %s\r\n", clientId->valuestring);
    }

    // 解析mqtt.username
    cJSON* username = cJSON_GetObjectItem(mqtt, "username");
    if (cJSON_IsString(username)) {
			strcpy((char *)js_re_data.mqtt_data.username, username->valuestring);
      printf("mqtt.username: %s\r\n", username->valuestring);
    }

    // 解析mqtt.passwd
    cJSON* passwd = cJSON_GetObjectItem(mqtt, "passwd");
    if (cJSON_IsString(passwd)) {
			strcpy((char *)js_re_data.mqtt_data.passwd, passwd->valuestring);
      printf("mqtt.passwd: %s\r\n", passwd->valuestring);
    }    

    // 解析mqtt.publishTopic
    cJSON* publishTopic = cJSON_GetObjectItem(mqtt, "publishTopic");
    if (cJSON_IsString(publishTopic)) {
			strcpy((char *)js_re_data.mqtt_data.publishTopic, publishTopic->valuestring);
      printf("mqtt.publishTopic: %s\r\n", publishTopic->valuestring);
    }

    // 解析mqtt.subTopic
    cJSON* subTopic = cJSON_GetObjectItem(mqtt, "subTopic");
    if (cJSON_IsArray(subTopic)) {
			js_re_data.mqtt_data.subTopicCount = cJSON_GetArraySize(subTopic);
//      int subTopicCount = cJSON_GetArraySize(subTopic);
      printf("mqtt.subTopic count: %d\r\n", js_re_data.mqtt_data.subTopicCount);
      
      for(int i=0; i<js_re_data.mqtt_data.subTopicCount; i++) {
        cJSON* subTopicItem = cJSON_GetArrayItem(subTopic, i);
        if (cJSON_IsString(subTopicItem)) {
					strcpy((char *)js_re_data.mqtt_data.subTopicItem[i], subTopicItem->valuestring);
          printf("mqtt.subTopic %d: %s\r\n", i, subTopicItem->valuestring); 
        }
      }
    }

    // 解析mqtt.unsubTopic
    cJSON* unsubTopic = cJSON_GetObjectItem(mqtt, "unsubTopic");
    if (cJSON_IsArray(unsubTopic)) {
      js_re_data.mqtt_data.unsubTopicCount = cJSON_GetArraySize(unsubTopic);
//      int unsubTopicCount = cJSON_GetArraySize(unsubTopic);
      printf("mqtt.unsubTopic count: %d\r\n", js_re_data.mqtt_data.unsubTopicCount);
      
      for(int i=0; i<js_re_data.mqtt_data.unsubTopicCount; i++) {
        cJSON* unsubTopicItem = cJSON_GetArrayItem(unsubTopic, i);
        if (cJSON_IsString(unsubTopicItem)) {
					strcpy((char *)js_re_data.mqtt_data.unsubTopicItem[i], unsubTopicItem->valuestring);
          printf("mqtt.unsubTopic %d: %s\r\n", i, unsubTopicItem->valuestring);
        }
      }
    }

    // 解析mqtt.RegIdent
    cJSON* RegIdent = cJSON_GetObjectItem(mqtt, "RegIdent");
    if (cJSON_IsArray(RegIdent)) {
      js_re_data.mqtt_data.RegIdentCount = cJSON_GetArraySize(RegIdent);
//      int RegIdentCount = cJSON_GetArraySize(RegIdent);
      printf("mqtt.RegIdent count: %d\r\n", js_re_data.mqtt_data.RegIdentCount);
      
      for(int i=0; i<js_re_data.mqtt_data.RegIdentCount; i++) {
      
        cJSON* RegIdentItem = cJSON_GetArrayItem(RegIdent, i);
        if (cJSON_IsString(RegIdentItem)) {
					strcpy((char *)js_re_data.mqtt_data.RegIdentItem[i], RegIdentItem->valuestring);
          printf("mqtt.RegIdent %d: %s\r\n", i, RegIdentItem->valuestring);
        }
      }
    }

  }

  // 解析http对象
  cJSON* http = cJSON_GetObjectItem(root, "http");
  if (cJSON_IsObject(http)) {

    // 解析http.IP 
    cJSON* httpIP = cJSON_GetObjectItem(http, "IP");
    if (cJSON_IsString(httpIP)) {
			
			strcpy((char *)js_re_data.http_data.IP , httpIP->valuestring);
      printf("http.IP: %s\r\n", httpIP->valuestring);
    }
		// 解析mqtt.socket
    cJSON* httpsocket = cJSON_GetObjectItem(http, "socket");
    if (cJSON_IsNumber(httpsocket)) {
			js_re_data.http_data.socket = httpsocket->valueint;
      printf("http.socket: %d\r\n", httpsocket->valueint);
    }

    // 解析http.port
    cJSON* httpPort = cJSON_GetObjectItem(http, "port");
    if (cJSON_IsNumber(httpPort)) {
			js_re_data.http_data.port = httpPort->valueint;
      printf("http.port: %d\r\n", httpPort->valueint);
    }

    // 解析http.PostUrl
    cJSON* PostUrl = cJSON_GetObjectItem(http, "PostUrl");
    if (cJSON_IsString(PostUrl)) {
			strcpy((char *)js_re_data.http_data.PostUrl , PostUrl->valuestring);
      printf("http.PostUrl: %s\r\n", PostUrl->valuestring);
    }

    // 解析http.GetUrl
    cJSON* GetUrl = cJSON_GetObjectItem(http, "GetUrl");
    if (cJSON_IsString(GetUrl)) {
			strcpy((char *)js_re_data.http_data.GetUrl , GetUrl->valuestring);
      printf("http.GetUrl: %s\r\n", GetUrl->valuestring);
    }

  }

  // 解析mode
	
  cJSON* mode = cJSON_GetObjectItem(root, "mode");
  if (cJSON_IsString(mode)) {
		strcpy((char *)js_re_data.mode , mode->valuestring);
    printf("mode: %s\r\n", mode->valuestring); 
  }

  // 释放资源
  cJSON_Delete(root);

  return 0;
}
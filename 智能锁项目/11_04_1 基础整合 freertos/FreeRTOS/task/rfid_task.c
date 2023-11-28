#include "myrtos.h"

extern	uint8_t card_num;
extern uint8_t card_id[100][5];

void RfidTask(void *pdata)
{
	printf("finger task coming!!\r\n");
	while(1){
//		send_voi_cmd(0X13);
		while(Identificate_Card(CardType,CardId)){
			vTaskDelay(600);
		}
		for(uint8_t i = 0; i < card_num; i++){
			if(!strcmp((const char *)CardId, (const char *)card_id[i])){
				printf("Ë¢¿¨³É¹¦ %d\r\n", i);
				send_voi_cmd(Welcome_home);
				open_door(3);
				goto yes;
			}
		}
		
		send_voi_cmd(opening_failure);
		yes:
		vTaskDelay(100);
    }
	
}
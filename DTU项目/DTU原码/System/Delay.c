#include "Delay.h"

uint32_t led_tick[2] = {0, 500},getdata[2] = {0, 5000}, dataanalysis[2] = {0, 1000}, modbus_tick[2] = {0, 1000};
uint32_t  d_ms[2] = {0, 0}, d_us[2] = {0, 0}, d_s[2] = {0, 0};

/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
	d_us[1] = xus;
	d_us[0] = 0;
	while(d_us[0] < d_us[1]);
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
	d_ms[0] = 0;
	d_ms[1] = xms;
	while(d_ms[0] < d_ms[1]);
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	d_s[0] = 0;
	d_s[1] = xs  * 1000 ;
	while(d_s[0] < d_s[1]);
} 

/**
  * @brief  配置系统定时器
	*						每1us产生一次中断
  * @param  None
  * @retval None
  */
void sys_tick_init(void){
	SysTick_Config(72000000 / 1000);	//系统定时器1us产生一次中断
}


/**
  * @brief  获取数据设置时间片
  * @param  uint8_t time	唤醒时间
  * @retval None
  */
uint8_t getdata_flag = 0;
void getdata_time_scale(uint16_t time){
	if(getdata[0] > time){
		printf("\r\ngetdata_time_scale\r\n");
		if(getdata_flag == 0){
			printf("\r\naliyun updata\r\n");
			updata_aliyun();
////			get_weather();
			getdata_flag = 1;
			getdata[0] = 0;
		}
		else if(getdata_flag == 1){
			printf("\r\nweather get data\r\n");
//			updata_aliyun();
			get_weather();
			getdata_flag = 0;
			getdata[0] = 0;
		}
		
	}
}



/**
  * @brief  数据分析时间片
  * @param  uint8_t time	唤醒时间
  * @retval None
  */

void dataanalysis_time_scale(uint16_t time){
	
	uint8_t socket_id = 0;
	uint8_t socket_data[512] = {0};
	if(dataanalysis[0] > time){
		printf("\r\ndataanalysis_time_scale\r\n");
		if(cat_data_analysis(socket_id, socket_data) == 1){
			printf("socket_id = %x\r\n", socket_id);
			printf("socket_data = %s\r\n", socket_data);
		}
		
		dataanalysis[0] = 0;
	}
}

/**
  * @brief  数据分析时间片
  * @param  uint8_t time	唤醒时间
  * @retval None
  */

void led_time_scale(uint16_t time){
	
	if(led_tick[0] > time){
//		printf("\r\nled_time_scale\r\n");
		led_turn(LED1_PORT, LED1_PIN);
		led_tick[0] = 0;
	}
}

/**
  * @brief  数据分析时间片
  * @param  uint8_t time	唤醒时间
  * @retval None
  */

void modbus_time_scale(uint16_t time){
	
	if(modbus_tick[0] > time){
		printf("\r\nmodbus_time_scale\r\n");
		Host_Read03_slave(0x01,0x0000,0x0004);
		
		if(modbus.Host_send_flag)
 					{
						printf("进入接收函数\r\n");
 						modbus.Host_Sendtime=0;//发送完毕后计数清零（距离上次的时间）
 						modbus.Host_time_flag=0;//发送数据标志位清零
 						modbus.Host_send_flag=0;//清空发送结束数据标志位
 						HOST_ModbusRX();//接收数据进行处理
 					}
		
		modbus_tick[0] = 0;
	}
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	modbus_tick[0]++;
	led_tick[0]++;
	getdata[0]++;
	dataanalysis[0]++;
	d_ms[0]++;
	d_us[0]++;
	d_s[0]++;
}

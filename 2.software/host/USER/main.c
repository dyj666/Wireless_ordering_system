#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"  
#include "24cxx.h" 
#include "lora.h"
#include "myiic.h"
#include "touch.h" 
#include "timer.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "gui_app.h" 
#include <string.h>



_Bool send_flag = 0;

u8 num_send[5];
u16 num;
u8 sendbuffer[5];




int main(void)
{
	delay_init();	    	 		//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 			//串口初始化为9600
	LED_Init();		  				//初始化与LED连接的硬件接口
	LCD_Init();			   			//初始化LCD 	
	Lora_Init();
	TIM2_Int_Init(4999,71);
	TIM3_Int_Init(999,71);	//定时器初始化(1ms中断),用于给lvgl提供心跳节拍
	// U1Send("hello");
	tp_dev.init();					//触摸屏初始化

	lv_init();							//lvgl系统初始化
	lv_port_disp_init();		//lvgl显示接口初始化,放在lv_init()的后面
	lv_port_indev_init();		//lvgl输入接口初始化,放在lv_init()的后面
	
	gui_app_start();			//运行例程
	
	while(1)
	{
		tp_dev.scan(0);//触摸扫描
		lv_task_handler();//lvgl的事务处理
	}
}

uint8_t count_100ms;
uint8_t count_1s;
uint8_t send_time;
void TIM2_IRQHandler(void)   //TIM2中断
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 

		count_100ms++;

		if(count_100ms == 20)
		{
			count_100ms = 0;count_1s++;
			LED0 = !LED0;
		}
		if(count_1s == 20)
		{
			count_1s = 0;
			if(send_flag)
			{
				send_flag = 0;
				if(strlen(num_send) == 3)
					num = ((num_send[0] - '0')*100) + ((num_send[1] - '0')*10) + (num_send[2] - '0');
				else if(strlen(num_send) == 2)
					num = ((num_send[0] - '0')*10) + (num_send[1] - '0');
				else if(strlen(num_send) == 1)
					num = (num_send[0] - '0');	
				sprintf(sendbuffer,"a%3db",num);
				SendString(sendbuffer);
			}
		}
	}
}

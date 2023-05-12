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
	delay_init();	    	 		//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 			//���ڳ�ʼ��Ϊ9600
	LED_Init();		  				//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   			//��ʼ��LCD 	
	Lora_Init();
	TIM2_Int_Init(4999,71);
	TIM3_Int_Init(999,71);	//��ʱ����ʼ��(1ms�ж�),���ڸ�lvgl�ṩ��������
	// U1Send("hello");
	tp_dev.init();					//��������ʼ��

	lv_init();							//lvglϵͳ��ʼ��
	lv_port_disp_init();		//lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ���
	lv_port_indev_init();		//lvgl����ӿڳ�ʼ��,����lv_init()�ĺ���
	
	gui_app_start();			//��������
	
	while(1)
	{
		tp_dev.scan(0);//����ɨ��
		lv_task_handler();//lvgl��������
	}
}

uint8_t count_100ms;
uint8_t count_1s;
uint8_t send_time;
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 

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

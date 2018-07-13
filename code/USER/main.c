#include "led.h"
#include "delay.h"
#include "key.h"
#include "motor.h"
#include "sys.h"
#include "usart.h"
 
 
/************************************************
 ALIENTEK战舰STM32开发板实验4
 串口实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


 int main(void)
 {		
 	u16 t;  
	u16 len;	
	u16 times=0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	Motor_Init(0, 0, 0);
 	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			if((USART_RX_BUF[0]==0x55)&&(len==5))
			{
				u8 dir1=0;
				u16 temp=0;
				u16 steps=0;
				printf("\r\n\r\n");
				printf("data receive ok!!!!");
				printf("\r\n\r\n");
				
				if((USART_RX_BUF[2]==0x1)||(USART_RX_BUF[2]==0x0))
				{
					dir1 = USART_RX_BUF[2];
					printf("dir = %d\r\n",dir1);
				}
				else
				{
					printf("dir data error!!!\r\n");
				}
				temp=USART_RX_BUF[3];
				steps=USART_RX_BUF[4]|(temp<<8);
				printf("steps=%d\r\n",steps);
				do_motor(dir1,steps);
				
//				TIM2_TIM3_PWM(1000,10);
			}
			else
			{
				printf("data receive error!!!!");
				printf("\r\n\r\n");
			}
//			for(t=0;t<len;t++)
//			{
//			
//				USART_SendData(USART1, USART_RX_BUF[t]);
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//			}
			
//			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
			}
			if(times%200==0);  
			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
			delay_ms(10);   
		}
	}	 
 }



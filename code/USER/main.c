#include "led.h"
#include "delay.h"
#include "key.h"
#include "motor.h"
#include "sys.h"
#include "usart.h"
 
 
/************************************************
 ALIENTEKս��STM32������ʵ��4
 ����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


 int main(void)
 {		
 	u16 t;  
	u16 len;	
	u16 times=0;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	Motor_Init(0, 0, 0);
 	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
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
			
//			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
			}
			if(times%200==0);  
			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);   
		}
	}	 
 }



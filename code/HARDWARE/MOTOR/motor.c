#include "sys.h"
#include "motor.h"
#include "stm32f10x_tim.h"
#include "delay.h"
void do_motor(u8 dir,u16 steps)
{
	DIR1 = dir;
	delay_ms(4);
	TIM2_TIM3_PWM(1000,steps);
	
}

void Motor_Init(u16 TIM2per, u16 TIM3per, u16 TIM3Compare1)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4;//dir
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
 	GPIO_Init(GPIOA, &GPIO_InitStruct);//≥ı ºªØGPIOA4
  
  //TIM2 single plus
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//0 div
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//up count
  TIM_TimeBaseInitStruct.TIM_Prescaler = 7200;//100us
  TIM_TimeBaseInitStruct.TIM_Period = TIM2per;//500=50ms
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
  
  TIM_SelectOnePulseMode(TIM2,TIM_OPMode_Single);//one pluse mode
  TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);//
  TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC1Ref);
  
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;//??????,??TIMx_CNT<TIMx_CCR1???1?????,???????
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//OC1????
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//??????
  TIM_OCInitStruct.TIM_Pulse = 1;//????1?????
  TIM_OC1Init(TIM2,&TIM_OCInitStruct);
  
  TIM_Cmd(TIM2,DISABLE);//?????TIM2
  
  
  //TIM3?????????????PWM????
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//??????
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//??????
  TIM_TimeBaseInitStruct.TIM_Prescaler = 720;//????,10us????
  TIM_TimeBaseInitStruct.TIM_Period = TIM3per;//???
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
  
  TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Gated);//TIM3?????
  TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);//??TIM3?????
  TIM_SelectInputTrigger(TIM3,TIM_TS_ITR1);//????,?TIM2??
  
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;//??????,??TIMx_CNT<TIMx_CCR1???1?????,???????
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//OC1????
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//??????
  TIM_OCInitStruct.TIM_Pulse = TIM3Compare1;//????1?????
  TIM_OC1Init(TIM3,&TIM_OCInitStruct);
  
  TIM_Cmd(TIM3,ENABLE);//??TIM3
}

//??PWM???
//Cycle:???,??(us)
//Pulse_Num:?????(??3200)
void TIM2_TIM3_PWM(u16 Cycle, u16 Pulse_Num)
{
  u16 TIM3per = 0;
  u32 Time = 0;
  //??TIM3????????????????????50%
  //??TIM2???????????????
    
  Time = Cycle * Pulse_Num;
  Time /= 100;              //????7200,100us????
  TIM3per = Cycle/10;       //????720,10us????
  
  TIM_SetAutoreload(TIM2, Time+1);//??TIM2????
  TIM_SetAutoreload(TIM3, TIM3per-1);//??TIM3????
  TIM_SetCompare1(TIM3,TIM3per/2);//??????50%
  TIM_Cmd(TIM2,ENABLE);//??TIM2
}

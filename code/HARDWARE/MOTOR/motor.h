#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"
 
#define DIR1 PAout(4)


//my test ok!!!
void do_motor(u8 dir,u16 steps);
void Motor_Init(u16 TIM2per, u16 TIM3per, u16 TIM3Compare1);
void TIM2_TIM3_PWM(u16, u16);				    
#endif

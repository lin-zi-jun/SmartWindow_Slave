#ifndef __HX711_H__
#define __HX711_H__
#include "stm32f10x.h"
//�������߱�������
void Delay__hx711_us(void);
void HX711_Init(void);
u32 Read_HX711(void);
u32 Get_Maopi(void);
u32 Get_Shiwu(void);
void weight_state(void);
#endif




#include "delay.h"

void delay_us(u16 us)
{
	u16 i=0;  
	while(us--)
	{
		i=10;  
		while(i--) ;    
	}
}

void delay_ms(u16 ms)
{
	u16 i=0;  
	while(ms--)
	{
		i=12000;  
		while(i--) ;    
	}	
}

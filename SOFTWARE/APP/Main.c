#include "headers.h"
///////////////////////////////////////////////////////////////////////////////
//extern u8 add1,add2,add3,add4,add5,add6,add0; 

u8 ret=0;
int main()
{
	BspInit();
	FSMC_SRAM_Init();
	allocExtSram();
	USART1_Config(115200);	
	USART3_Config(115200);	
	CtrlIO_Init();
	RTC_Init();
	InitRC522();
	ili93xx_Initializtion(); 
//	InitFlashFS();
	ReadFlashDevInfo();
	ReadFlashData();
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_7tq,5,CAN_Mode_Normal);
	SysTickInit();
//  Choose_module();
	KernelMain();
      
	return 0;
}

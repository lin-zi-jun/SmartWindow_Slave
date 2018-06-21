#include "headers.h"
///////////////////////////////////////////////////////////////////////////////
u8 Usart1TxSem, Usart2TxSem, Usart3TxSem;
///////////////////////////////////////////////////////////////////////////////
// storage in ext SRAM
// USART_FIFO_LEN 1024
USART_FIFO_TYPE *pUSART1_FIFO, *pUSART2_FIFO, *pUSART3_FIFO;

// USART_PRINT_BUF_LEN 512
u8 *pUSART1_Print_Buf, *pUSART2_Print_Buf, *pUSART3_Print_Buf;

#if (USE_EXT_SRAM == 0)
USART_FIFO_TYPE USART1_FIFO;
//USART_FIFO_TYPE USART2_FIFO;
USART_FIFO_TYPE USART3_FIFO;
u8 USART1_Print_Buf[USART_PRINT_BUF_LEN];
//u8 USART2_Print_Buf[USART_PRINT_BUF_LEN];
u8 USART3_Print_Buf[USART_PRINT_BUF_LEN];
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
extern u32 gEventFlag;
extern OS_SEM mainSem;
extern u32 gUsart1TOCnt, gUsart2TOCnt, gUsart3TOCnt;
u32 FILE_SIZE=0;
u8  FILE_SEND_FLAG=0;
u8  SEND_OVER_FLAG=0;
u8  CREATE_OVER_FLAG=0;
u8  READ_FILE_FLAG=0;
u8  FILE_OK_FLAG=0;


///////////////////////////////////////////////////////////////////////////////
// 主串口的DMA发送初始化
void USART1_DMA_TxConfig(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
    // DMA1 Channel4 (triggered by USART1 Tx event) Config
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART1->DR));
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
    
    // Enable DMA1 Channel4 Transfer Complete interrupt
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
    
    // Enable DMA1 channel4 IRQ Channel
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
///////////////////////////////////////////////////////////////////////////////
void USART2_DMA_TxConfig(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* DMA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
    /* DMA1 Channel7 (triggered by USART2 Tx event) Config */
    DMA_DeInit(DMA1_Channel7);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);
    
    /* Enable DMA1 Channel7 Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
    
    /* Enable DMA1 channel7 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
///////////////////////////////////////////////////////////////////////////////
void USART3_DMA_TxConfig(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* DMA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
    /* DMA1 Channel7 (triggered by USART2 Tx event) Config */
    DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART3->DR));
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
    
    /* Enable DMA1 Channel2 Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
    
    /* Enable DMA1 channel7 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///////////////////////////////////////////////////////////////////////////////
void USART1_Config(int Baud_Rate)
{
	USART_InitTypeDef USART_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,ENABLE);
	Usart1TxSem = 0;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 		//USART1 TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 		//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 		//USART1 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 		//复用开漏输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	USART_InitStructure.USART_BaudRate = Baud_Rate;					
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			
	USART_InitStructure.USART_Parity = USART_Parity_No;			
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
	//USART1_DMA_RxConfig();
	USART1_DMA_TxConfig();
	USART_FIFO_CLEAR(pUSART1_FIFO);
}
///////////////////////////////////////////////////////////////////////////////
void USART2_Config(int Baud_Rate)
{
	USART_InitTypeDef USART_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	Usart2TxSem = 0;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         		 		//USART2 TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 		//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         	 		//USART2 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 		//复用开漏输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	 
	USART_InitStructure.USART_BaudRate = Baud_Rate;					
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			
	USART_InitStructure.USART_Parity = USART_Parity_No;			
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);	
	USART2_DMA_TxConfig();
	USART_FIFO_CLEAR(pUSART2_FIFO);
}
///////////////////////////////////////////////////////////////////////////////
void USART3_Config(int Baud_Rate)
{
	USART_InitTypeDef USART_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	Usart3TxSem = 0;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         		 	//USART3 TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 		//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         	 		//USART3 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 	//复用开漏输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	 
	USART_InitStructure.USART_BaudRate = Baud_Rate;					
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			
	USART_InitStructure.USART_Parity = USART_Parity_No;			
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);	
	USART3_DMA_TxConfig();
	USART_FIFO_CLEAR(pUSART3_FIFO);
}
///////////////////////////////////////////////////////////////////////////////
u8 USART1_SengByte(u8 ch)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
		; 
    USART_SendData(USART1,(uint8_t)ch);   
 	return ch;
}
void USART1_SendString(u8 *str)
{
	u8 *p = str;
	while (*p)
		USART1_SengByte(*p++);
}

void USART1_DMA_SendNString(u8 *str,u32 n)
{
    DMA1_Channel4->CNDTR = n;
    DMA1_Channel4->CMAR = (u32)str;
    
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA1_Channel4, ENABLE);
}

void USART1_Printf(char *fmt,...)
{
	u32 len=0;
	//CPU_SR_ALLOC();
    va_list ap;
    
	va_start(ap,fmt);
    while(Usart1TxSem!=0) {
		if (len++>200000) {
			gUsart1TOCnt++;
			if(DMA_GetITStatus(DMA1_IT_TC4))
				DMA_ClearITPendingBit(DMA1_IT_GL4);
			USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
			DMA_Cmd(DMA1_Channel4, DISABLE);
			break;
		}
	}
	
	//OS_CRITICAL_ENTER();
    Usart1TxSem = 1;
    len = vsprintf((char *)pUSART1_Print_Buf, fmt, ap);
	if (len >0)
		USART1_DMA_SendNString(pUSART1_Print_Buf, len);
	else 
		Usart1TxSem = 0;
    va_end(ap);
	//OS_CRITICAL_EXIT();
}

///////////////////////////////////////////////////////////////////////////////
u8 USART2_SengByte(u8 ch)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET)
		; 
    USART_SendData(USART2,(uint8_t)ch);   
 	return ch;
}
void USART2_SendString(u8 *str)
{
	u8 *p = str;
	while (*p)
		USART2_SengByte(*p++);
}

void USART2_DMA_SendNString(u8 *str,u32 n)
{
    DMA1_Channel7->CNDTR = n;
    DMA1_Channel7->CMAR = (u32)str;
    
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA1_Channel7, ENABLE);
}

void USART2_Printf(char *fmt,...)
{
	u32 len;
    va_list ap;
	CPU_SR_ALLOC();
    
    va_start(ap,fmt);
    while(Usart2TxSem!=0);
	OS_CRITICAL_ENTER();
    Usart2TxSem = 1;
    len = vsprintf((char *)pUSART2_Print_Buf,fmt,ap);
	if (len >0)
		USART2_DMA_SendNString(pUSART2_Print_Buf,len);
	else
		Usart2TxSem = 0;
    va_end(ap);
	OS_CRITICAL_EXIT();
}
///////////////////////////////////////////////////////////////////////////////
u8 USART3_SengByte(u8 ch)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET)
		; 
    USART_SendData(USART3,(uint8_t)ch);   
 	return ch;
}
void USART3_SendString(u8 *str)
{
	u8 *p = str;
	while (*p)
		USART3_SengByte(*p++);
}

void USART3_DMA_SendNString(u8 *str,u8 n)
{
    DMA1_Channel2->CNDTR = n;
    DMA1_Channel2->CMAR = (u32)str;
    
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA1_Channel2, ENABLE);
}

void USART3_Printf(char *fmt,...)
{
	u32 len=0;
    va_list ap;
	//CPU_SR_ALLOC();
    
    va_start(ap,fmt);
    while(Usart3TxSem!=0) {
		if (len++>200000) {
			gUsart3TOCnt++;
			if(DMA_GetITStatus(DMA1_IT_TC2))
				DMA_ClearITPendingBit(DMA1_IT_GL2);
			USART_DMACmd(USART3, USART_DMAReq_Tx, DISABLE);
			DMA_Cmd(DMA1_Channel2, DISABLE);
			break;
		}
	}
	//OS_CRITICAL_ENTER();
    Usart3TxSem = 1;
    len = vsprintf((char *)pUSART3_Print_Buf,fmt,ap);
	if (len>0)
		USART3_DMA_SendNString(pUSART3_Print_Buf,len);
	else 
		Usart3TxSem = 0;
    va_end(ap);
	//OS_CRITICAL_EXIT();
}
///////////////////////////////////////////////////////////////////////////////

// 向FIFO缓冲区写入数据
void USART_FIFO_WRITE(USART_FIFO_TYPE *p,u8 data)
{
	CPU_SR_ALLOC();
    if(p->len < USART_FIFO_LEN)   
    {
        CPU_CRITICAL_ENTER();
        p->buf[p->in++] = data;
        p->len++;
        if(p->in >= USART_FIFO_LEN) p->in = 0; 
        CPU_CRITICAL_EXIT();
    }
}

// 读取FIFO缓冲区里面的数据
u8 USART_FIFO_READ(USART_FIFO_TYPE *p)
{
	CPU_SR_ALLOC();
    u8 ch = 0;
	
    if(p->len>0)
    {
        CPU_CRITICAL_ENTER();
        ch = p->buf[p->out++]; 
        p->len--;
        if(p->out >= USART_FIFO_LEN) p->out = 0;
        CPU_CRITICAL_EXIT();
    }
    
	return ch;
}

// 清除缓冲区中的所有数据
void USART_FIFO_CLEAR(USART_FIFO_TYPE *p)
{
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    p->in  = 0;
    p->out = 0;
    p->len = 0;
    CPU_CRITICAL_EXIT();
}
///////////////////////////////////////////////////////////////////////////////
// 主串口的中断处理程序
// 在硬件中断 USART1_IRQHandler() 里调用该函数
// 注意，为了防止串口中断接收数据遗漏，串口硬件中断优先级应设置得比较高
int m=0;
void USART1_IRQHandler_Run(void)
{
    u8 ch;
//    u8 i;
	
    CPU_SR_ALLOC();

        if(((USART1->SR)&(USART_FLAG_RXNE)) != 0)
            {
//			USART1_Printf("h1\r\n");
               ch = USART1->DR;
               USART_FIFO_WRITE(pUSART1_FIFO,ch);
		   if (ch == '\n') {
		      OS_CRITICAL_ENTER();
		      gEventFlag |= FLAG_USART1_EVENT;
			OS_CRITICAL_EXIT();
			//OSSemPost(&mainSem, 0, NULL);
		    }
              }
	 
}

// 在硬件中断 USART2_IRQHandler() 里调用该函数
// 注意，为了防止串口中断接收数据遗漏，串口硬件中断优先级应设置得比较高
void USART2_IRQHandler_Run(void)
{
    u8 ch;
	CPU_SR_ALLOC();
    if(((USART2->SR)&(USART_FLAG_RXNE)) != 0)
    {
        ch = USART2->DR;
        USART_FIFO_WRITE(pUSART2_FIFO,ch);
		if (ch == '\n') {
			OS_CRITICAL_ENTER();
			gEventFlag |= FLAG_USART2_EVENT;
			OS_CRITICAL_EXIT();
			//OSSemPost(&mainSem, 0, NULL);
		}
    }
}

// 在硬件中断 USART3_IRQHandler() 里调用该函数
// 注意，为了防止串口中断接收数据遗漏，串口硬件中断优先级应设置得比较高
void USART3_IRQHandler_Run(void)
{
    u8 ch;
	//CPU_SR_ALLOC();
    if(((USART3->SR)&(USART_FLAG_RXNE)) != 0)
    {
        ch = USART3->DR;
        USART_FIFO_WRITE(pUSART3_FIFO,ch);
		//USART1_Printf("%c", ch);
		if (ch == '\n') {
			//OS_CRITICAL_ENTER();
			gEventFlag |= FLAG_USART3_EVENT;
			//OS_CRITICAL_EXIT();
			//OSSemPost(&mainSem, 0, NULL);
		}
    }
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifndef __USART_H__
#define __USART_H__
#include "headers.h"
///////////////////////////////////////////////////////////////////////////////
#define USART_PRINT_BUF_LEN	256
#define USART_FIFO_LEN		512
///////////////////////////////////////////////////////////////////////////////
typedef struct _USART_FIFO_TYPE
{
    u32 in;
    u32 out;
    u32 len;
	u8  buf[USART_FIFO_LEN];
    
} USART_FIFO_TYPE;
///////////////////////////////////////////////////////////////////////////////
void USART_FIFO_WRITE(USART_FIFO_TYPE *p,u8 data);
u8 USART_FIFO_READ(USART_FIFO_TYPE *p);
void USART_FIFO_CLEAR(USART_FIFO_TYPE *p);

///////////////////////////////////////////////////////////////////////////////
void USART1_Config(int Baud_Rate);
void USART2_Config(int Baud_Rate);
void USART3_Config(int Baud_Rate);


u8   USART1_SengByte(u8 ch);
void USART1_SendString(u8 *str);
void USART1_DMA_SendNString(u8 *str, u32 len);
void USART1_Printf(char *fmt,...);
void USART1_IRQHandler_Run(void);

u8   USART2_SengByte(u8 ch);
void USART2_SendString(u8 *str);
void USART2_DMA_SendNString(u8 *str, u32 len);
void USART2_Printf(char *fmt,...);
void USART2_IRQHandler_Run(void);

u8   USART3_SengByte(u8 ch);
void USART3_SendString(u8 *str);
void USART3_DMA_SendNString(u8 *str, u8 len);
void USART3_Printf(char *fmt,...);
void USART3_IRQHandler_Run(void);

///////////////////////////////////////////////////////////////////////////////
#endif


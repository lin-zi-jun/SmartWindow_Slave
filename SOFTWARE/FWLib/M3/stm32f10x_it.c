/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "headers.h"
///////////////////////////////////////////////////////////////////////////////
extern OS_SEM mainSem;
extern u32 gEventFlag;
extern u8 Usart1TxSem, Usart2TxSem, Usart3TxSem;
extern CanRxMsg RxMessage;
extern u8 ret;
extern u8 RFID_state;
extern u8 add1,add2,add3,add4,add5,add6,add0;
extern u8 ACQ_WEIGHT;
 u8 CTime_buf[4];
 u8 UserCard_buf[4];
extern u32 UserCard;
extern u32 CTime;
 u8 ReTime_buf[4];
extern u32 RemainTimebuf;
u8 SN_buf[4];
extern u32 SN;
extern u8 SN_init;
//extern u8 return_state;
extern RFCard_ModuleInfo ModuleInfo;
extern SupperCardInfo suppercardinfo;
extern u8 ModuleID;
//u32 SupperCardBuf=0;
extern u8 supper_state;
extern u8 return_state;
extern u8 time_cnt;
extern u8 error_return;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : This function handles DMA1 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
	CPU_SR_ALLOC();
	if(DMA_GetITStatus(DMA1_IT_TC2))
    {
        DMA_ClearITPendingBit(DMA1_IT_GL2);
        USART_DMACmd(USART3, USART_DMAReq_Tx, DISABLE);
        DMA_Cmd(DMA1_Channel2, DISABLE);
		OS_CRITICAL_ENTER();
        Usart3TxSem = 0; 
		OS_CRITICAL_EXIT();
    }
	
}

/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : This function handles DMA1 Channel 3 interrupt request.
* For USART3 DMA Rx
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
	/*
	if(DMA_GetITStatus(DMA1_IT_TC3)) //通道3传输完成中断TC 还有传输 过半中断HT 错误中断TE 全局中断GL
	{
		DMA_Cmd(DMA1_Channel3, DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_GL3);    //清除全部中断标志		
	}
	*/
}

/*******************************************************************************
* Function Name  : DMA1_Channel4_IRQHandler
* Description    : This function handles DMA1 Channel 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
	CPU_SR_ALLOC();
    if(DMA_GetITStatus(DMA1_IT_TC4))
    {
        DMA_ClearITPendingBit(DMA1_IT_GL4);
        USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
        DMA_Cmd(DMA1_Channel4, DISABLE);
		OS_CRITICAL_ENTER();
        Usart1TxSem = 0; 
		OS_CRITICAL_EXIT();
    }
}

/*******************************************************************************
* Function Name  : DMA1_Channel5_IRQHandler
* Description    : This function handles DMA1 Channel 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel5_IRQHandler(void)
{
	/*
	if(DMA_GetITStatus(DMA1_IT_TC5)) //通道3传输完成中断TC 还有传输 过半中断HT 错误中断TE 全局中断GL
	{
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_GL5);    //清除全部中断标志		
	}
	*/
}

/*******************************************************************************
* Function Name  : DMA1_Channel6_IRQHandler
* Description    : This function handles DMA1 Channel 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel7_IRQHandler
* Description    : This function handles DMA1 Channel 7 interrupt request.
* USART DMA Tx
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
	CPU_SR_ALLOC();
    if(DMA_GetITStatus(DMA1_IT_TC7))
    {
        DMA_ClearITPendingBit(DMA1_IT_GL7);
        USART_DMACmd(USART2, USART_DMAReq_Tx, DISABLE);
        DMA_Cmd(DMA1_Channel7, DISABLE);
		OS_CRITICAL_ENTER();
        Usart2TxSem = 0; 
		OS_CRITICAL_EXIT();
    }
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
	USART1_IRQHandler_Run();
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
    USART2_IRQHandler_Run();
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{ 
	USART3_IRQHandler_Run();
}


void USB_LP_CAN1_RX0_IRQHandler(void)		 //CAN中断函数
{
  //CanRxMsg RxMessage;
	u8 j=0;
  FLASH_Status status;
  u8 buf[8];
  ret = 0; 
  RxMessage.StdId=0x00;		  //初始化接收数据池
  RxMessage.ExtId=0x00;		  //
  RxMessage.IDE=0;			  //
  RxMessage.DLC=0;			  //
  RxMessage.FMI=0;			  //
  RxMessage.Data[0]=0x00;	  //
  RxMessage.Data[1]=0x00;	  //
  RxMessage.Data[2]=0x00;
  RxMessage.Data[3]=0x00;
  RxMessage.Data[4]=0x00;

  //while(CAN_IT_FF0==0);
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);	  //从CAN1口缓冲器FIFO 0接收报文到 RxMessage
	
	for(j=0;j<8;j++)
	buf[j]=RxMessage.Data[j];
	if((buf[0]=='$')&&(buf[1]=='M')&&(buf[2]=='R'))
		{
		FlashEraseLastPage();
		status = FLASH_WaitForLastOperation(ProgramTimeout);
		if(status == FLASH_COMPLETE)
			{
			FLASH->CR |= CR_PG_Set;
			*(__IO uint16_t*)BOOTSW =1;
			status = FLASH_WaitForLastOperation(ProgramTimeout);
			FLASH->CR &= CR_PG_Reset;
			}
		RunMainProgram(IAPAddress);
		}
   if(RxMessage.ExtId==ModuleID)//校验报文数量是否正确，3号机
  {
        switch(RxMessage.Data[0]){
            case 0x08:
                if(RxMessage.Data[1]==0x01){
                    RFID_state=1;//预约成功
                    time_cnt=0;
                }else if(RxMessage.Data[1]==0x02){
                    RFID_state=2;//预约不成功

                }else if(RxMessage.Data[1]==0x03)//还仪器
                {
                   return_state=1;
                   time_cnt=0;
                }else if(RxMessage.Data[1]==0x04)//归还错拿仪器
				{
					return_state=1;
                    time_cnt=0;
					error_return=1;
				}                
                break;
             case 0x07://接收卡号
                 memcpy(&UserCard,&RxMessage.Data[1],4);
                 break;
             case 0x06://接收时间

                 memcpy(&CTime,&RxMessage.Data[1],4);
                 break;
             case 0x05://接收剩余时间
 //                char2int(&RxMessage.Data[1],(u8*)&RemainTimebuf);
                 memcpy(&RemainTimebuf,&RxMessage.Data[1],4);
                 break;
             case 0x04://接收SN号
                 memcpy(&SN,&RxMessage.Data[1],4);
                 SN_init=1;
                 break;
             default:
                 break;
        }
    }else if(RxMessage.ExtId==0x10){
         switch (RxMessage.Data[0]){
                case 0xEF:
                    if(RxMessage.Data[1]==0xEF)
                     {
                         ACQ_WEIGHT=1;//获得毛重
                     }else if(RxMessage.Data[1]==0xEE){
                         ACQ_WEIGHT=2;//获得实重
                     }else if(RxMessage.Data[1]==0x01){
                         RFID_state=1;
                         time_cnt=0;
                     }
                     break;
                case 0x01:
//                    char2int(&RxMessage.Data[1],(u8*)&CTime);
                memcpy(&CTime,&RxMessage.Data[1],4);
                    RTC_SET_TIME(CTime);
                    break;
                case 0x02:
//                    char2int(&RxMessage.Data[1],(u8*)&SupperCardBuf);
                    supper_state=1;

                    break;
                default:
                    break;
        }
    } 
}    




/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

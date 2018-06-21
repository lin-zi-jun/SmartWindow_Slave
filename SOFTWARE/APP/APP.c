#include "headers.h"
///////////////////////////////////////////////////////////////////////////////
static void MainTask(void*);
void UITask(void);
void USART_Read_Pkg(USART_FIFO_TYPE *usart, u8 *buf);
///////////////////////////////////////////////////////////////////////////////
extern u32 gSramOffset;
// Storage in ext SRAM
extern u8 *pUSART_RxBuf;
#if (USE_EXT_SRAM == 0)
extern u8 USART_RxBuf[USART_FIFO_LEN];
#endif
extern USART_FIFO_TYPE *pUSART1_FIFO, *pUSART2_FIFO, *pUSART3_FIFO;
///////////////////////////////////////////////////////////////////////////////
static OS_TCB taskStartTCB;
static CPU_STK taskStartStk[STARTUP_TASK_STK_SIZE]; 		//启动任务的程序空间

static OS_TCB MainTaskTCB;
static CPU_STK MainTaskStk[MAINTASK_STK_SIZE];

static  OS_TCB      UITCB;
static  CPU_STK     UIStk[UI_STK_SIZE];
///////////////////////////////////////////////////////////////////////////////
OS_SEM mainSem;
u32 gEventFlag = 0;
void clearFlag(u32 flag);
/*******************************************************************************
* Function Name :void TaskStart(void)
* Description   :任务启动
* Input         :
* Output        :
* Other         :
* Date          :2012.04.18  11:48:23
*******************************************************************************/

static void TaskStart(void)
{
    OS_ERR 		err;
	
    OSTaskCreate(   (OS_TCB     *)&MainTaskTCB,
                    (CPU_CHAR   *)"MainTask",
                    (OS_TASK_PTR)MainTask,
                    (void       *)0,
                    (OS_PRIO    )MAINTASK_PRIO,
                    (CPU_STK    *)&MainTaskStk[0],
                    (CPU_STK_SIZE)MAINTASK_STK_SIZE / 10,
                    (CPU_STK_SIZE)MAINTASK_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);
    
     OSTaskCreate(  (OS_TCB     *)&UITCB,
                    (CPU_CHAR   *)"UITask",
                    (OS_TASK_PTR)UITask,
                    (void       *)0,
                    (OS_PRIO    )UI_PRIO,
                    (CPU_STK    *)&UIStk[0],
                    (CPU_STK_SIZE)UI_STK_SIZE / 10,
                    (CPU_STK_SIZE)UI_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);      
    
    OSSemCreate(    (OS_SEM     *)&mainSem, 
                    (CPU_CHAR   *)"mainSem", 
                    (OS_SEM_CTR )0, 
                    (OS_ERR     *)err);				
    
    OSTaskDel(      (OS_TCB     *)&taskStartTCB, 
                    (OS_ERR     *)&err);
					
}

static void MainTask(void *p_arg)
{
    OS_ERR err;
	u32 cnt = 0;
	
	InitState();
	OSSemPend(&mainSem, 0, 0, NULL, &err);
#if (WATCHDOG_EN == 1)
	IWDG_Init();
#endif

    while (1)
    {
		if ((gEventFlag & FLAG_ONE_SEC) != 0) {//gEventFlag是RTC的一秒的事件标志
			clearFlag(FLAG_ONE_SEC);
			cnt ++;
			//USART1_Printf("Second Timer:%.8d\r\n", cnt);
			OnSecondTimer();
//                  Can_Send_Msg(0x10,0xEF,0x00);
                  
                  
		}
 		if ((gEventFlag & FLAG_USART1_EVENT) != 0 ) {
 			clearFlag(FLAG_USART1_EVENT);
 			USART_Read_Pkg(pUSART1_FIFO, pUSART_RxBuf);
 			PCCmdHandler(pUSART_RxBuf);
 			USART1_Printf("%s", pUSART_RxBuf);
 		}
// 		if ((gEventFlag & FLAG_USART2_EVENT) != 0 ) {
// 			clearFlag(FLAG_USART2_EVENT);
// 			USART_Read_Pkg(pUSART2_FIFO, pUSART_RxBuf);
// 		}
// 		if ((gEventFlag & FLAG_USART3_EVENT) != 0 ) {
// 			clearFlag(FLAG_USART3_EVENT);
// 			USART_Read_Pkg(pUSART3_FIFO, pUSART_RxBuf);
// 			//USART1_Printf("%s", USART_RxBuf);
// 			SvrCmdHandler(pUSART_RxBuf);
// 		}
		
		RFCardHandler();       
		BuzzerHandler();
        weight_state();
        UIShowHandler();
//		NetResetHandler();
#if (WATCHDOG_EN == 1)
		IWDG_ReloadCounter();
#endif
		OSTimeDly(  (OS_TICK    )1, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
    }
}


void KernelMain(void)
{
    OS_ERR err;

    CPU_Init();
    OSInit(         (OS_ERR	    *)&err);

    OSTaskCreate(   (OS_TCB     *)&taskStartTCB,
                    (CPU_CHAR   *)"Task Start",
                    (OS_TASK_PTR)TaskStart,
                    (void       *)0,
                    (OS_PRIO    ) STARTUP_TASK_PRIO,
                    (CPU_STK    *)&taskStartStk[0],
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE / 10,
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);    
    
    OSStart(        (OS_ERR	    *)&err);
}

///////////////////////////////////////////////////////////////////////////////
void USART_Read_Pkg(USART_FIFO_TYPE *usart, u8 *buf) {
    u8 ch;
	u16 len;
  
	len = 0;
    while(usart->len>0) {
        ch = USART_FIFO_READ(usart);
        buf[len++] = ch;
		
        if(len>(USART_FIFO_LEN-1)) {
            len = 0;
        } else {
            if(ch=='\n') {
                buf[len] = 0;
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
void clearFlag(u32 flag) {
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	gEventFlag &= ~flag;
	OS_CRITICAL_EXIT();
}




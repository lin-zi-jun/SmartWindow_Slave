#include "headers.h"
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// import from rtl.h
extern int finit (const char *drive);
extern int fformat (const char *drive);
///////////////////////////////////////////////////////////////////////////////
//DevInfo dev;
// 继电器开关状态
//u8  SWITCH_ON = 0;
// 继电器开关导通计时
u32 gUseTime = 0;
// 服务器返回的能够使用时间
u32 gRemainTime = 0;
// 退出的标志
u32 gRandomId = 0;
u16 LOGOUT_N = 0;

u32 gRFCardIn, gRFCardUse;
u8 Card_IN=0;		// 刷卡状态，=1：有卡刷进来
u16 gUserId;		// 用户ID

u8 gBuzzerCnt;		// 蜂鸣器响声时间计数器
u8 Num=0;
u8 UIShowCnt=0;

u32 gUsart1TOCnt, gUsart2TOCnt, gUsart3TOCnt;
u32 gSramOffset;
u8 time_cnt;
u8 RFID_state=0;
CanRxMsg RxMessage;
u32 gRFCardIn, gRFCardUse;
u8 RFIDbuf[4];
u32 RFCardbuf;
u8 hx711_state=0;
u32 UserCard=0;
u32 CTime=0;
u32 ETime=0;
u8 gBuf[10];
u8 return_state=0;
u32 RemainTimebuf=0;
u8 SN_init=0;
u32 SN=0;
u8 ModuleID=0;
u8 WeightState=0;
u32 SN_return=0;
u8 Warning_state=0;//警告标志
u8 supper_state=0;//超级卡标志
u32 SupperCardBuf=0;
extern u8 ACQ_WEIGHT;//获取重量标志
u8 supper_time=0;
u16 supper_timecount=0;
extern u8 error_timecount;//时间计数
extern u8 return_timecount;//时间计数
extern u8 borrow_timecount;//时间计数
u8 timestate_error=0;//误拿时间计数状态
u8 timestate_return=0;//归还时间计数状态
u8 timestate_borrow=0;//借出时间计数状态
u8 ReBo_timecnt=0;//借与还计时
u8 ReBo_state=0;//借与还状态
u8 error_return=0;//错拿归还标志
u8 hx711state=0;
///////////////////////////////////////////////////////////////////////////////

extern u8 Usart1TxSem, Usart2TxSem, Usart3TxSem;
RFCard_ModuleInfo ModuleInfo;
SupperCardInfo suppercardinfo;
extern WeightInfo weightinfo;
///////////////////////////////////////////////////////////////////////////////
//USART_FIFO_LEN 1024
extern USART_FIFO_TYPE *pUSART1_FIFO, *pUSART2_FIFO, *pUSART3_FIFO;
#if (USE_EXT_SRAM == 0)
extern USART_FIFO_TYPE USART1_FIFO, USART2_FIFO, USART3_FIFO;
#endif
//USART_PRINT_BUF_LEN 512
extern u8 *pUSART1_Print_Buf, *pUSART2_Print_Buf, *pUSART3_Print_Buf;
#if (USE_EXT_SRAM == 0)
extern u8 USART1_Print_Buf[USART_PRINT_BUF_LEN], USART2_Print_Buf[USART_PRINT_BUF_LEN], USART3_Print_Buf[USART_PRINT_BUF_LEN];
//u8 gBuf[GBUF_LEN];
u8 USART_RxBuf[USART_FIFO_LEN];
#else 
u8 *gBuf;
#endif
u8 *pUSART_RxBuf;

///////////////////////////////////////////////////////////////////////////////
void allocExtSram(void) {
	gSramOffset = 0;
#if (USE_EXT_SRAM == 0)
	pUSART_RxBuf = USART_RxBuf;

	pUSART1_FIFO = &USART1_FIFO;
	//pUSART2_FIFO = &USART2_FIFO;
	pUSART3_FIFO = &USART3_FIFO;
	
	pUSART1_Print_Buf = USART1_Print_Buf;
	//pUSART2_Print_Buf = USART2_Print_Buf;
	pUSART3_Print_Buf = USART3_Print_Buf;
#else	
	pUSART_RxBuf = (u8*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += USART_FIFO_LEN;
	// set the USART recv FIFO
	
	pUSART1_FIFO = (USART_FIFO_TYPE*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += sizeof(USART_FIFO_TYPE);
	pUSART2_FIFO = (USART_FIFO_TYPE*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += sizeof(USART_FIFO_TYPE);
	pUSART3_FIFO = (USART_FIFO_TYPE*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += sizeof(USART_FIFO_TYPE);

	// Set USART Printf BUF
	pUSART1_Print_Buf = (u8*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += USART_PRINT_BUF_LEN;
	pUSART2_Print_Buf = (u8*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += USART_PRINT_BUF_LEN;
	pUSART3_Print_Buf = (u8*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += USART_PRINT_BUF_LEN;
	
	gBuf = (u8*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += GBUF_LEN;
#endif
	
	memset((u8*)Bank1_SRAM3_ADDR, 0, SRAM_LEN);
}
///////////////////////////////////////////////////////////////////////////////
u8 PCCmdHandler(u8 *pBuf) {
    u32 n=0,id=0,p1=0,p2=0,p3=0,p4=0,p9=0;
    u8 SupperCardID[4];
    if(pBuf[0]=='$') {
		n = sscanf((char *)pBuf,"$PC,%d,%d,%u,%d,%d",&id,&p1,&p2,&p3,&p4);
        if(n>=1) {
    		switch(id) {
			case 0x08:
				Dump();
				break;
		   }
     }
   }
}
void OnSecondTimer(void) {
	static u8 sec_cnt = 0;
	static u8 timecount=0;
    u8 time_buf[4];
    u32 time=0;
	showTime();
	UseTimeHandler();

	if(RFID_state==1||return_state==1){
          time_cnt++;
       }
    if(supper_time==1){
        supper_timecount++;
    }
    if(timestate_error==1)
    {
        error_timecount++;
    }
    if(timestate_return==1)
    {
        return_timecount++;
    }
    if(timestate_borrow==1)
    {
        borrow_timecount++;
    }
    if(ReBo_state==1)
    {
        ReBo_timecnt++;
    }
	if (++sec_cnt==10) {
		sec_cnt = 0;
        
        time=RTC_GET_TIME_UTC();
//        int2char(RTC_GET_TIME_UTC(),time_buf);
        memcpy(time_buf,&time,4);
		// 往主控发心跳包
		Can_Send_DevInfo(0x20,0x07,time_buf);
        
        switch(WeightState){
//            case 1:
//                Can_Send_Msg(ModuleID,0xEE,0x01);
//                WeightState=0;
//                break;
//            case 2:
//                Can_Send_Msg(ModuleID,0xEE,0x02);
//                WeightState=0;
//                break;
            case 3:
                Can_Send_Msg(ModuleID,0xEE,0x03);
                WeightState=0;
                break;
            case 4:
                Can_Send_Msg(ModuleID,0xEE,0x06);
                WeightState=0;
                break;
            default:
                break;
        }
     }
	if (++timecount==6){
		timecount=0;
		if(hx711state==1){ 
			Can_Send_Msg(ModuleID,0xEE,0x01);
			hx711state=0;
		}else if(hx711state==2) {
			Can_Send_Msg(ModuleID,0xEE,0x08);
			hx711state=0;
		}else if(hx711state==3){
			Can_Send_Msg(ModuleID,0xEE,0x02);
			hx711state=0;
		}
		
	} 
 }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 预约时间计数处理
void UseTimeHandler(void) {//使用时间操作
    u32 t;
    u8 d,h,m,s;
    
	if(gRemainTime>0){
//		SWITCH_ON = 1;
		gRemainTime--;
		gUseTime++;
		if (gRemainTime<10 && gRemainTime%2==0) {
			BUZZER_ON();
			gBuzzerCnt = BUZZER_ONE_SEC;
		}
		
		t = gUseTime;
        d=t/86400;
		h = (t-d*86400)/3600;
		m = (t-h*3600-d*86400)/60;
		s = t%60;
		sprintf((char*)gBuf, "%02d:%02d:%02d:%02d ",d,h,m,s);
		UI_SetUse(gBuf);
		
		t = gRemainTime;
        d=t/86400;
		h = (t-d*86400)/3600;
		m = (t-h*3600-d*86400)/60;
		s = t%60;
		sprintf((char*)gBuf, "%02d:%02d:%02d:%02d ",d,h,m,s);
		UI_SetRemain(gBuf);
	} else {

		gUserId = 0;

		BUZZER_OFF();
	}
	
}
///////////////////////////////////////////////////////////////////////////////
void InitState(void) {
//	readDevInfo();
    ReadFlashData();
	ReadFlashDevInfo();
    LED_off();

	gUseTime = 0;
	gRemainTime = 0;
	gRandomId = 0;
    time_cnt=0;

	gRFCardIn = 0;
	gRFCardUse = 0;
	Card_IN=0;		// 刷卡状态，=1：有卡刷进来
	gUserId = 0;		// 用户ID

	gBuzzerCnt = 0;		// 蜂鸣器响声时间计数器

	
	gUsart1TOCnt = 0;
	gUsart2TOCnt = 0;
	gUsart3TOCnt = 0;
}
///////////////////////////////////////////////////////////////////////////////
void InitFlashFS(void) {
	if(finit("N:") == 0)
	{
		USART1_Printf("NAND initialize OK.\r\n");
	} else {
		if(fformat("N:0") != 0)
		{													//初次使用NAND，寇需要格式化
			USART1_Printf("NAND format failed.\r\n");
			finit("N:0");
		} else {
			USART1_Printf("NAND format OK.\r\n");
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// RF card读卡处理

void RFCardHandler(void)//射频卡操作
{		
	static u8 outCounter;
    u8 RFOrder=0;
//    u16 m;
//	u32 ctime; 
    if(RFID_state==0&&return_state==0)
    {
       
      if(ReadRFID(RFIDbuf, RFOrder)==MI_OK)
       {         
         if (Card_IN == 0) {                 
//              char2int(RFIDbuf,(u8*)&RFCardbuf);

              memcpy(&RFCardbuf,RFIDbuf,4);
//              readSupperCardInfo();
			  sprintf((char*)gBuf,"%.8X",RFCardbuf);
              UI_SetID(gBuf);
			  UIShowCnt=UISHOW_THREE_SEC;
              if(ModuleInfo.UserCard_ID==RFCardbuf){
                 Can_Send_Msg(ModuleID,0xEE,0xEA);
                 Can_Send_DevInfo(ModuleID,0x09,RFIDbuf);
//                 return_state=1;   
              }else if((suppercardinfo.card==RFCardbuf)||(RFCardbuf==0xD568748F)||
			            (RFCardbuf==0xB553B98F)||(RFCardbuf==0x45DB7C8F)||(RFCardbuf==0x95E5748F)||(RFCardbuf==0xD51B348A)){
                  SupperCardHander();
              }else{
                  Can_Send_Msg(ModuleID,0xEE,0xEF);
                  Can_Send_DevInfo(ModuleID,0x09,RFIDbuf);
                }
             BUZZER_ON();
             gBuzzerCnt = BUZZER_ONE_SEC;
                 
         }
        Card_IN = 1;
		outCounter = 0;
	   } else {	//if(ReadRFID(RFIDbuf, RFOrder)==MI_OK)) {
            outCounter ++ ;
            if (outCounter>5) {	// 防抖动
                Card_IN = 0;
		   }
 	    }                                     
   }else if(RFID_state==1)
      {          
       LED_on();            
       sprintf((char*)gBuf,"%.8X",UserCard);
       UI_SetID(gBuf);
       ModuleInfo.UserCard_ID=UserCard; 
       return_state=0;
       if(hx711_state==2)
         { 
//		   hx711_state=0;
           ReBo_state=1;
           gRemainTime = 0;
           gUseTime=0;           
           gRemainTime=RemainTimebuf;
           UI_SetInfo("请刷卡确认");
           UIShowCnt=UISHOW_TWO_SEC;
           if (ReadRFID((u8*)&gRFCardIn, RFOrder)==MI_OK)
            {
                 if (Card_IN == 0) { 
                   BUZZER_ON();
                   gBuzzerCnt = BUZZER_ONE_SEC;
                   RFID_state=0;
                   time_cnt=0;
                   ReBo_state=0;
                   WriteFlashRFCardInfo();                      
                   if(UserCard==gRFCardIn)
                       {                                                                      
                          
                          LED_off();
                          Can_Send_Msg(ModuleID,0xEE,0xEE);                             
                          UI_SetInfo("借出成功");
                          UIShowCnt=UISHOW_TWO_SEC;                                                         
                        }else{
                           UI_SetInfo("借出成功");
                           UIShowCnt=UISHOW_TWO_SEC;                      
                           LED_off();
                           Can_Send_Msg(ModuleID,0xEE,0xEE);                                                      
                         }
                }
                Card_IN = 1;
                outCounter = 0;              
            }else{
                outCounter ++ ;
                if (outCounter>5) {	// 防抖动
                     Card_IN = 0;
                   }
                if(ReBo_timecnt>=20){                                    
                    RFID_state=0;
                    ReBo_state=0;
                    time_cnt=0;
                    ReBo_timecnt=0;
                    UI_SetInfo("借出成功");
                    UIShowCnt=UISHOW_TWO_SEC;
                    LED_off();
                    Can_Send_Msg(ModuleID,0xEE,0xEE);
                    WriteFlashRFCardInfo(); 
                }
            }
       }else {         
           UI_SetInfo("请拿仪器");
           UIShowCnt=UISHOW_TWO_SEC;
           if(time_cnt>=20){
               USART1_Printf("time_cnt1:%d\r\n",time_cnt); 
               UI_SetID("");             
               RFID_state=0;
               UI_SetInfo("借出失败");
               UIShowCnt=UISHOW_TWO_SEC;
               time_cnt=0;
               ModuleInfo.UserCard_ID=0;
               WriteFlashRFCardInfo();
               LED_off();
               Can_Send_Msg(ModuleID,0xEE,0xEC);
               
          }                  
       }                      
    }else if(RFID_state==2){
        
            UI_SetInfo("没有预约");
            UIShowCnt=UISHOW_TWO_SEC;
            RFID_state=0;        
       }        
    if(return_state==1)
        {
          
          LED_on();
          RFID_state=0;
          if(hx711_state==1)
            {
//			   hx711_state=0;
               ReBo_state=1; 
               gRemainTime=0;
               gUseTime = 0;  
                 
               UI_SetID("");
               UI_SetRemain("");
               UI_SetUse("");                
               UI_SetInfo("请刷卡确认");
               UIShowCnt=UISHOW_TWO_SEC;
               if (ReadRFID((u8*)&gRFCardIn, RFOrder)==MI_OK)
                  {
                      if (Card_IN == 0) {
                          
                          BUZZER_ON();
                          gBuzzerCnt = BUZZER_ONE_SEC;
                          time_cnt=0;
                          ReBo_state=0;
                          return_state=0;
                          ModuleInfo.UserCard_ID=0;
                          WriteFlashRFCardInfo();
                          USART1_Printf("dhsj\r\n");
                          if(UserCard==gRFCardIn)
                             {                                            
//                                 RFID_state==0; 
								if(error_return==1){
									error_return=0;
									Can_Send_Msg(ModuleID,0xEE,0xDC);
								}
								Can_Send_Msg(ModuleID,0xEE,0xDF);
								
                                UI_SetInfo("仪器归还成功");
                                UIShowCnt=UISHOW_TWO_SEC;
                                LED_off();
                                 USART1_Printf("96\r\n");
                              }else{
                                UI_SetInfo("仪器归还成功");
                                UIShowCnt=UISHOW_TWO_SEC;                            
//                                 RFID_state=0;								  
								if(error_return==1){
									error_return=0;
									Can_Send_Msg(ModuleID,0xEE,0xDC);
								}
								Can_Send_Msg(ModuleID,0xEE,0xDF);
								
                                LED_off();

                              }
                        }
                        Card_IN = 1;
                        outCounter = 0;                         
                  }else{
                     outCounter ++ ;
                     if (outCounter>5) {	// 防抖动
                          Card_IN = 0;
                        } 
                     if(ReBo_timecnt>=20){
                       return_state=0;
                       ReBo_state=0;
                       time_cnt=0;
                       UI_SetInfo("仪器归还成功");
                       UIShowCnt=UISHOW_TWO_SEC;                  
                       ReBo_timecnt=0;                        
					   if(error_return==1){
						   error_return=0;
						   Can_Send_Msg(ModuleID,0xEE,0xDC);
					   }
					   Can_Send_Msg(ModuleID,0xEE,0xDF);
					   
                       LED_off();
                       ModuleInfo.UserCard_ID=0;
                       WriteFlashRFCardInfo();
                       }
                   }
           }else {
               UI_SetInfo("请放入仪器");
               UIShowCnt=UISHOW_TWO_SEC;
 //               USART1_Printf("kjdj%d\r\n",time_cnt);
               if(time_cnt>=20){
                   USART1_Printf("time_cnt2:%d\r\n",time_cnt);
                   return_state=0;
                   time_cnt=0;
                   UI_SetInfo("归还失败");
                   UIShowCnt=UISHOW_TWO_SEC;
				   Can_Send_Msg(ModuleID,0xEE,0xDD);				   
                   LED_off();
                }                       
             }  
      }
    SupperCard_off();
    Can_Hander();
} 


////////////////////////////////////////////////////////////////////////////////
void BuzzerHandler(void) {//蜂鸣器操作
	if (gBuzzerCnt>0) {
		gBuzzerCnt--;
		BUZZER_ON();
	} else {
		BUZZER_OFF();
	}
}
///////////////////////////////////////////////////////////////////////////////
void UIShowHandler(void) {//屏幕显示操作
	if (UIShowCnt>0) {
		UIShowCnt--;
	} else {
		UI_SetInfo("");
	}
}
///////////////////////////////////////////////////////////////////////////////
void char2int(u8* buff,u8 *pOutBuf)
{
     pOutBuf[0] = buff[0];
	 pOutBuf[1] = buff[1];
	 pOutBuf[2] = buff[2];
	 pOutBuf[3] = buff[3];
}
void int2char(u32 cd,u8*buf)
{
      u8 j=0;
      
      for(j=0;j<4;j++)
      {
            buf[j]=*((u8*)&cd+j);
      }
}
void SupperCardHander(void)//超级卡操作
{      
      Num++;
      switch (Num){
            case 1:
               Can_Send_Msg(ModuleID,0xEE,0x04);
               Warning_state=1;
               supper_time=1;
               LED_on();
               UI_SetInfo("请清空仪器");
               UIShowCnt=UISHOW_TWO_SEC;
               break;
            case 2:
               ACQ_WEIGHT=1;
               UI_SetInfo("请更换仪器");
               UIShowCnt=UISHOW_TWO_SEC;
               break;
            case 3:
               ACQ_WEIGHT=2;
			   if(ModuleInfo.UserCard_ID!=0&&ModuleInfo.UserCard_ID!=-1)
			   {
				   Can_Send_Msg(ModuleID,0xEE,0x07);
				   gRemainTime=0;
				   gUseTime = 0;                   
				   UI_SetID("");
				   UI_SetRemain("");
				   UI_SetUse("");
				   ModuleInfo.UserCard_ID=0;
				   WriteFlashRFCardInfo();

			   }               
               Num=0;
               Warning_state=0;
               supper_time=0;
               supper_timecount=0;
               UI_SetInfo("更换成功");
               UIShowCnt=UISHOW_TWO_SEC;
               LED_off();
			   Can_Send_Msg(ModuleID,0xEE,0x05);
               break;
            case 4:
                Num=0;
                supper_time=0;
                supper_timecount=0;
                break;
            default:
                break;                        
        }
    if(Warning_state==1)
      {
          hx711_state=0;
          WeightState=0;
          gBuzzerCnt=0;
      }  
}
void SupperCard_off(void)//没有完成更换仪器步骤后
{
    if(supper_timecount>=SUPPER_ONE_SEC)
      {
        supper_time=0;
        supper_timecount=0;
        Can_Send_Msg(ModuleID,0xEE,0x05);
        Num=0;
        Warning_state=0;
        LED_off();
        UI_SetInfo("更换失败"); 
        UIShowCnt=UISHOW_TWO_SEC;
      }
}
void Can_Hander(void)//can总线部分操作
{
    if(SN_init==1)
      {
           ModuleInfo.Module_SN=SN;
           suppercardinfo.id=SN;
           WriteFlashRFCardInfo();
           WriteFlashDevInfo();
           SN_init=0;          
      }
    if(supper_state==1)
      {
           memcpy(&SupperCardBuf,&RxMessage.Data[1],4);
           suppercardinfo.card=SupperCardBuf;
           WriteFlashDevInfo();
           USART1_Printf("supper:%X\r\n",suppercardinfo.card);
           supper_state=0;
      } 
}

void Dump(void) {
//	FILE *file;
////	UserInfo *user;
//  WeightInfo *weightinfo_Dump;
//  RFCard_ModuleInfo *ModuleInfo_Dump;
//  SupperCardInfo *suppercardinfo_Dump;
//	u16 len;
	
	USART1_Printf("weightinfo.id:%d\r\n", weightinfo.id);
	USART1_Printf("weightinfo.Gross_Weight:%d\r\n", weightinfo.Gross_Weight);
	USART1_Printf("weightinfo.shiwu_Weight:%d\r\n", weightinfo.shiwu_Weight);
	USART1_Printf("ModuleInfo.id:%d\r\n",ModuleInfo.Module_SN);
	USART1_Printf("ModuleInfo.UserCard_ID:%X\r\n",ModuleInfo.UserCard_ID);
	USART1_Printf("suppercardinfo.id:%d\r\n", suppercardinfo.id);
	USART1_Printf("suppercardinfo.card:%X\r\n", suppercardinfo.card);
}

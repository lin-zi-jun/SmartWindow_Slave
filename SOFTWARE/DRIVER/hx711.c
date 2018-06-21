// #include "HX711.h"
// #include "delay.h"
// #include "stm32f10x.h"
#include "headers.h"
//**************************************************


u32 Weight_Maopi,Weight_Shiwu;
u8 ACQ_WEIGHT=0;
u8 cnt=0;
u8 cnttt=0;
extern u8 hx711_state;
WeightInfo weightinfo;
extern u8 gBuzzerCnt;
extern RFCard_ModuleInfo ModuleInfo;
extern u8 WeightState;
// u32 Avg_value[3];
// u8 i=0;
 u32 Are=0;
u32 warncnt=0;
u32 borrowcnt=0;
//u8 borrowcnt=0;
extern u8 timestate_error;
extern u8 timestate_return;
extern u8 timestate_borrow;
u8 error_timecount=0;
u8 return_timecount=0;
u8 borrow_timecount=0;
extern u8 ModuleID;
extern u8 hx711state;
//***************************************************
//��ʼ��HX711�˿� 
//***************************************************
void HX711_Init(void)	
{ 
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB�˿�ʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 //SCK 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	 //DOUT 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 //�����趨������ʼ��PC1
}

//****************************************************
//��ȡHX711
//****************************************************
u32 Read_HX711(void)  //��HX711оƬ��������ݡ� 
{ 
	u32 val = 0; 
	unsigned char i = 0; 
    u32 bitcnt=0;
	GPIO_SetBits(GPIOC,GPIO_Pin_1);	   //DOUT=1 
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	   //SCK=0 
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))   //�ȴ�DOUT=0  
    {
        bitcnt++;
        if(bitcnt>=720000)
        {
            return 2; 
        }
        
    }
		delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(GPIOC,GPIO_Pin_0);	   //SCK=1 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);	   //SCK=0 
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))   //DOUT=1 
		val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(GPIOC,GPIO_Pin_0); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(GPIOC,GPIO_Pin_0); 
	delay_us(1);  
	return val; 	
}

u32 Get_weight(void)
{
	u32 HX711_Buffer = 0;
	u32 Weight_Maopi = 0;

	HX711_Buffer = Read_HX711();
	Weight_Maopi = HX711_Buffer/100;	
    Weight_Maopi = (unsigned int)((float)Weight_Maopi/4.2+0.05); 
	return Weight_Maopi;
} 

u32 Get_Shiwu(void)
{
	u32 HX711_Buffer = 0;


	HX711_Buffer = Read_HX711();
	HX711_Buffer = HX711_Buffer/100;
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//��ȡʵ���AD������ֵ��
	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/4.4+0.05); 	//����ʵ���ʵ������
																		//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������4.30���������
																		//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
																		//������Գ���������ƫСʱ����С����ֵ��
																		//����ֵһ����4.0-5.0֮�䡣�򴫸�����ͬ������50g���룬��1g���Ҫ����һλС��
																		//+0.05��Ϊ����������ٷ�λ
																		//��취��һ��ͨ��
	}
	return Weight_Shiwu;
}

 void weight_state(void)
{      
    u32 acq_weight=0;
    
    u32 value=0;//��ǰ������ȥë��
    u32 value_Real=0;//��õ�ʵ��������ȥ
    U32 abc=0;
	U32 ef=0;
    
    if(ACQ_WEIGHT==1)
      {   
         weightinfo.id=ModuleInfo.Module_SN;
         weightinfo.Gross_Weight=Get_weight();
//         writeFile(WEIGHT_FILE_NAME, (u8*)&weightinfo, sizeof(WeightInfo));
				 WriteFlashDevInfo();
         ACQ_WEIGHT=0;
         USART1_Printf("%d\r\n",weightinfo.Gross_Weight);          
      }else if(ACQ_WEIGHT==2){
//          if(weightinfo.Gross_Weight==0)
//            {
//              ReadFlashDevInfo();
//            }
            acq_weight=Get_weight();
						USART1_Printf("%d\r\n",acq_weight); 
            weightinfo.shiwu_Weight=acq_weight-weightinfo.Gross_Weight;
//              writeFile(WEIGHT_FILE_NAME, (u8*)&weightinfo, sizeof(WeightInfo));
						WriteFlashDevInfo();
            ACQ_WEIGHT=0;
            USART1_Printf("%d\r\n",weightinfo.shiwu_Weight);            
        }
//       if(weightinfo.Gross_Weight==0||weightinfo.shiwu_Weight==0)
//          {
//             ReadFlashDevInfo();
//             USART1_Printf("899%d\r\n",weightinfo.Gross_Weight);
//             USART1_Printf("896%d\r\n",weightinfo.shiwu_Weight);
//          }
        acq_weight=Get_weight();
        value=labs(acq_weight-weightinfo.Gross_Weight);
        value_Real=labs(value-weightinfo.shiwu_Weight);
		abc=weightinfo.shiwu_Weight*0.2;
		ef=weightinfo.shiwu_Weight*0.5;
        
        if(value>=ef&&value_Real<abc){
            timestate_return=1;
            Are++;
            if(Are>=13)
            {
                Are=0;
                hx711_state=1;
				hx711state=1;

                UI_SetState("�ѷ�������");
                cnt=0;
                cnttt=0; 
            }
           if(return_timecount>=2)
            {
               Are=0;
               return_timecount=0;
               timestate_return=0;
            }
         }else if(value>=ef&&value_Real>=abc){
//							 USART1_Printf("Err:%d\r\n",acq_weight);
//							 USART1_Printf("Err:%d\r\n",weightinfo.Gross_Weight);
//					     USART1_Printf("Err:%d\r\n",weightinfo.shiwu_Weight);
//               UI_SetState("��ԭ��������");
               timestate_error=1;
               warncnt++;
               if(warncnt>=18)
                {
                    warncnt=0;
					UI_SetState("��ԭ��������");
					
                  if(cnttt==0)
                     {
						hx711_state=0;
                        warncnt=0;                        
                        cnttt=6;
                        WeightState=4;
                        BUZZER_ON();
                        gBuzzerCnt = BUZZER_WARN_SEC;
                        USART1_Printf("Error1\r\n");
                     }                            
                }
               if(error_timecount>=2)
                {
 //                  USART1_Printf("%d\r\n",warncnt);
                   warncnt=0;
                   error_timecount=0;
                   timestate_error=0;
                }
         }else if(value<ef){			 
			  timestate_borrow=1;
			  borrowcnt++;
			  if(borrowcnt>=16){
				  borrowcnt=0;
				  if(ModuleInfo.UserCard_ID==0||ModuleInfo.UserCard_ID==-1)
					 {
						hx711state=2;
						
						UI_SetState("����������");
						cnttt=0;
						if(cnt==0)
						  {
	//                         warncnt++;
	//                         if(warncnt>=40)
	//                          {
	//                            warncnt=0;
								hx711_state=0;
								cnt=6;
								WeightState=3;
								USART1_Printf("wrong2\r\n"); 
								BUZZER_ON();
								gBuzzerCnt = BUZZER_WARN_SEC;                           
	//                          }   
						 }                                           
					 }else{
//						Can_Send_Msg(ModuleID,0xEE,0x02);
						hx711state=3;
						hx711_state=2;
						UI_SetState("�����ѽ��");
						cnt=0;
						cnttt=0;
				}
			  }
              if(borrow_timecount>=2)
                {
 //                  USART1_Printf("%d\r\n",borrowcnt);
                   borrowcnt=0;
                   borrow_timecount=0;
                   timestate_borrow=0;
                }             
	 }
  }     
           






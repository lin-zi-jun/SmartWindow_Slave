#include "MFRC522.h"
#include "string.h"
#include "USART.h"
#include "stm32f10x.h" 
#include "delay.h"
///////////////////////////////////////////////////////////////////////////////
unsigned char RevBuffer[16];
void ClearBitMask(unsigned char reg,unsigned char mask);
void Write_Reg(unsigned char Address,unsigned char value);
void SetBitMask(unsigned char reg,unsigned char mask);
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit);
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData);
unsigned char Read_Reg(unsigned char Address);
void PcdAntennaOn(void);
////////////////////////////////////////////////////////////////////////////////
/**********************************************************
** 函数名:SPIReadByte
** 功能描述:GPIO模拟SPI，读一个字节
** 输入参数: 无
** 输出参数: 无
** 返    回：dat--读取的字节
***********************************************************/
unsigned char SPIReadByte(void)
{
  char i;
  u8 dat=0;                  
  SET_SPI_CK();//CLK=1
  for(i=0;i<8;i++)
  {
    CLR_SPI_CK();//CLK=0,CLK下降沿将寄存器中数据送到MISO数据线上
    dat<<=1; 
	if(STU_SPI_MISO()) //读取MISO线上数据
    {
       dat|=0x01;
    }
    SET_SPI_CK();//CLK=1
  }       
  return (dat); 
  
}
 
/**********************************************************
** 函数名:SPIWriteByte
** 功能描述:GPIO模拟SPI，写一个字节
** 输入参数: dat--写入的字节数据
** 输出参数: 无
** 返    回：无
***********************************************************/
void SPIWriteByte(u8 dat)
{
  char i;  
  CLR_SPI_CK(); //CLK=0
  for(i=8;i;i--)
  {
    if(dat&0x80)   
        SET_SPI_MOSI(); //MOSI=1
    else
        CLR_SPI_MOSI();//MOSI=0
	CLR_SPI_CK(); //CLK=0
	delay_us(10);
    SET_SPI_CK();	  //CLK=1,CLK上升沿把数据写入RC522 
    dat<<= 1;
  }          
	
} 
/**********************************************************
** 函数名:Read_Reg
** 功能描述:读RC522寄存器
** 输入参数: Address:寄存器地址
** 输出参数: 无
** 返    回：dat--读出的值
***********************************************************/
unsigned char Read_Reg(unsigned char Address)
{
    unsigned char dat=0;
	CLR_SPI_CS();//CS=0,使能片选
	SPIWriteByte(((Address<<1)&0x7E)|0x80);
	dat=SPIReadByte();
	SET_SPI_CS();
   return dat;
}

/**********************************************************
** 函数名:Write_Reg
** 功能描述:写RC522寄存器
** 输入参数: Address:寄存器地址
			 value:写入的值
** 输出参数: 无
** 返    回：无
***********************************************************/
void Write_Reg(unsigned char Address, unsigned char value)
{  
	CLR_SPI_CS();	 //CS=0,使能片选
	SPIWriteByte((Address<<1)&0x7E);
	SPIWriteByte(value);
	SET_SPI_CS();	//CS=1
} 
/**********************************************************
** 函数名:SetBitMask
** 功能描述:将RC522寄存器中mask为1的相应位置位
** 输入参数: reg--寄存器地址
			 mask--置位值
** 输出参数: 无
** 返    回：无
** 调用方法：SetBitMask(TxControlReg, 0x03)，将TxControlReg第0位及第1位置1
***********************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = Read_Reg(reg); //读取寄存器值
    Write_Reg(reg,tmp | mask);  // set bit mask
}

/**********************************************************
** 函数名:ClearBitMask
** 功能描述:将RC522寄存器中mask为1的相应位清0
** 输入参数: reg--寄存器地址
			 mask--置位值
** 输出参数: 无
** 返    回：无
** 调用方法：ClearBitMask(TxControlReg, 0x03)，将TxControlReg第0位及第1位清0
***********************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = Read_Reg(reg);//读取寄存器值
    Write_Reg(reg, tmp & ~mask);  // clear bit mask
} 

/**********************************************************
** 函数名:PcdAntennaOn
** 功能描述:开启天线，每次启动或关闭天线发射之间应至少有1ms的间隔
** 输入参数: 无
** 输出参数: 无
** 返    回：无
***********************************************************/
void PcdAntennaOn(void)
{
    unsigned char i;
    i = Read_Reg(TxControlReg);
    if (!(i & 0x03))//如果Tx2REFn、Tx1REFn位为0
    {
        SetBitMask(TxControlReg, 0x03);//Tx2REFn、Tx1REFn位置1
    }
}
/**********************************************************
** 函数名:PcdAntennaOff
** 功能描述:关闭天线
** 输入参数: 无
** 输出参数: 无
** 返    回：无
***********************************************************/
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);//Tx2REFn、Tx1REFn位清0，停止发送
}
/**********************************************************
** 函数名:CalulateCRC
** 功能描述:用RC522计算CRC16函数
** 输入参数: *pIndata--CRC输入数据地址的指针
			 len--输入数据长度
			 *pOutData--输出数据地址的指针	
** 输出参数: 无
** 返    回：无
***********************************************************/
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);//当CRC命令有效时所有数据将被处理
    Write_Reg(CommandReg,PCD_IDLE);//取消当前命令
    SetBitMask(FIFOLevelReg,0x80);// 重设FIFO缓冲区指针，清空缓冲器
    for (i=0; i<len; i++)
    {   Write_Reg(FIFODataReg, *(pIndata+i));   }
    Write_Reg(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = Read_Reg(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = Read_Reg(CRCResultRegL);//得到较验后的数据
    pOutData[1] = Read_Reg(CRCResultRegM);
}  
/**********************************************************
** 函数名:PcdComMF522
** 功能描述:通过RC522和ISO14443卡通讯
** 输入参数: Command:RC522命令字
             *pInData:通过RC522发送到卡片的数据地址的指针
             InLenByte:发送数据的字节长度     
             *pOutData:接收到的卡片返回数据地址的指针
             *pOutLenBit:返回数据的位长度地址的指针
** 输出参数: 无
** 返    回：失败返回MI_ERR，成功返回MI_OK
***********************************************************/
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int *pOutLenBit)
{
    char status = (char)MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch(Command)
    {
        case PCD_AUTHENT:  //验证密钥
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE://发送并接收数据
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
    }
   
    Write_Reg(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    Write_Reg(CommandReg,PCD_IDLE);//空闲状态
    SetBitMask(FIFOLevelReg,0x80);//清除FIFO读写指针,清空缓冲区
    
    for (i=0; i<InLenByte; i++)	//写数据到FIFO
    {   Write_Reg(FIFODataReg, pInData[i]);    }

    Write_Reg(CommandReg, Command);
    
    if (Command == PCD_TRANSCEIVE) //发送数据
    {    SetBitMask(BitFramingReg,0x80);  }	//开始发送数据
    
    //根据时钟频率调整，操作M1卡最大等待时间25ms
	i = 2000;
    do 
    {
        n = Read_Reg(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));//等待发送完成
    ClearBitMask(BitFramingReg,0x80);//发送完成，清除发送位

    if (i!=0)
    {    
        if(!(Read_Reg(ErrorReg)&0x1B)) //如果无错误
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = (char)MI_NOTAGERR;   }  //返回卡无反应信息		  
            if (Command == PCD_TRANSCEIVE) //接收数据
            {
               	n = Read_Reg(FIFOLevelReg);	//读取数据
              	lastBits = Read_Reg(ControlReg) & 0x07;	//得到最后接收字节的有效位
                if (lastBits) //如果不为0，得到有效的位数
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else //如果RxLastBits(2-0位)都为0，则最后接收数据整个字节是有效的
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)	 //读取接收的数据
                {   pOutData[i] = Read_Reg(FIFODataReg);    }
            }
        }
        else
        {   status = (char)MI_ERR;   }
        
    }
   
    SetBitMask(ControlReg,0x80);//停止定时器
    Write_Reg(CommandReg,PCD_IDLE); //进入空闲状态
    return status;
}                  
/**********************************************************
** 函数名:PcdRequest
** 功能描述:寻卡
** 输入参数: req_code:寻卡方式
                0x52--寻感应区内所有符合14443A标准的卡
                0x26--寻未进入休眠状态的卡
             *pTagType：卡片类型代码地址指针
                0x4400--Mifare_UltraLight
                0x0400--Mifare_One(S50)
                0x0200--Mifare_One(S70)
                0x0800--Mifare_Pro(X)
                0x4403--Mifare_DESFire
** 输出参数: 无
** 返    回：成功返回MI_OK
***********************************************************/
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
	char status;  
	unsigned int unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 
	//将Status2Reg寄存器MFCrypto1On位清0，置1时使能MIFARE加密单元
	//所有卡上的数据通信是被加密的
	ClearBitMask(Status2Reg,0x08);//关闭加密单元
	Write_Reg(BitFramingReg,0x07);//最后一个字节应传输7位，否则接收的数据无效
	SetBitMask(TxControlReg,0x03);//置位Tx2REFn及Tx1REFn,开启天线
 
	ucComMF522Buf[0] = req_code;//寻卡方式
	//发送寻卡方式，与卡通讯，1字节
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
	//USART1_Putc(ucComMF522Buf[0]);
	//USART1_Putc(ucComMF522Buf[1]);
	//USART1_Printf("status:%d, unLen:%d\r\n", status, unLen);
	if ((status == MI_OK) && (unLen == 0x10))//通讯OK，得到2字节数据--卡类型
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   status = (char)MI_ERR;   }
   
	return status;
}

/**********************************************************
** 函数名:PcdAnticoll
** 功能描述:防冲撞
** 输入参数: *pSnr:卡片序列号地址指针，卡片序列号4字节
** 输出参数: 无
** 返    回：成功返回MI_OK
***********************************************************/
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);//不加密
	//该寄存器指明发送帧中最后一个字节和接收帧第一个字节中不完整的比特的位数
    Write_Reg(BitFramingReg,0x00);//最后一个字节传输比特0位
    ClearBitMask(CollReg,0x80);//发生冲撞后所有接收的位清0
 
    ucComMF522Buf[0] = PICC_ANTICOLL1; //发送防冲撞指令
    ucComMF522Buf[1] = 0x20;
	//发送2字节数据与卡通讯
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++) //得到被选中的卡的序列号
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = (char)MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}
/**********************************************************
** 函数名:PcdSelect
** 功能描述:选定卡片
** 输入参数: *pSnr:卡片序列号地址指针，卡片序列号4字节
** 输出参数: 无
** 返    回：成功返回MI_OK
***********************************************************/
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
	//将卡序列号作CRC较验
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);
	//发送卡序列号与卡片通信
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }	//操作OK
    else
    {   status = (char)MI_ERR;    }

    return status;
}

/**********************************************************
** 函数名:PcdHalt
** 功能描述:命令卡片进入休眠状态
** 输入参数: 无
** 输出参数: 无
** 返    回：成功返回MI_OK
***********************************************************/
char PcdHalt(void)
{
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;//休眠命令
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return MI_OK;
}

/**********************************************************
** 函数名:PcdReset
** 功能描述:复位RC522
** 输入参数: 无
** 输出参数: 无
** 返    回：成功返回MI_OK
***********************************************************/
char PcdReset(void)
{
	SET_RC522RST();//RST=1
    delay_us(10);
	CLR_RC522RST();//RST=0
    delay_us(10);
	SET_RC522RST();//RST=1
    delay_us(10);
    Write_Reg(CommandReg,PCD_RESETPHASE);//写复位命令，软复位
    delay_us(10);
    
    Write_Reg(ModeReg,0x3D);//和Mifare卡通讯，CRC初始值0x6363
    Write_Reg(TReloadRegL,30);//加载定时时间          
    Write_Reg(TReloadRegH,0);
    Write_Reg(TModeReg,0x8D);//定时器模式设置，数据发送完成后自动启动定时器
    Write_Reg(TPrescalerReg,0x3E);//定时器分频系数为0X3E
	
	Write_Reg(TxAutoReg,0x40);//100%ASK传送
   
    return MI_OK;
}
/**********************************************************
** 函数名:PcdConfigISOType
** 功能描述:设置RC522的工作方式
** 输入参数: 无
** 输出参数: 无
** 返    回：成功返回MI_OK
***********************************************************/
char PcdConfigISOType(unsigned char type)
{
   if (type == 'A') //ISO14443_A卡
   { 
       ClearBitMask(Status2Reg,0x08);
       Write_Reg(ModeReg,0x3D);//3F
       Write_Reg(RxSelReg,0x86);//84
       Write_Reg(RFCfgReg,0x7F);   //4F
   	   Write_Reg(TReloadRegL,30);
	   Write_Reg(TReloadRegH,0);
       Write_Reg(TModeReg,0x8D);
	   //Write_Reg(TxAutoReg, 0x40);     //100%ASK
	   Write_Reg(TPrescalerReg,0x3E);
	   delay_us(1000);
       PcdAntennaOn();
   }

   return MI_OK;
}
/**********************************************************
** 函数名:InitRC522
** 功能描述:初始化RC522
** 输入参数: 无
** 输出参数: 无
** 返    回：无
***********************************************************/
void InitRC522(void)
{
  RC522_GPIO_Config();
  delay_ms(1);
  PcdReset(); //RC522复位
  PcdAntennaOff();//关闭天线  
  PcdAntennaOn();//开启天线
  PcdConfigISOType('A');//ISO14443_A卡
}

////////////////////////////////////////////////////////////////////////////////
int ReadRFID(unsigned char *pOutBuf, unsigned char bitOrder)
{
  	char status;
	
	PcdReset();
	//status=PcdRequest(PICC_REQALL,&RevBuffer[0]);//寻天线区内所有卡，返回卡片类型 2字节
	status=PcdRequest(PICC_REQIDL,&RevBuffer[0]);//寻天线区内所有卡，返回卡片类型 2字节
	if(status!=MI_OK)
	{
		return MI_NOTAGERR;
	}
	status=PcdAnticoll(&RevBuffer[2]);//防冲撞，返回卡的序列号 4字节
	if(status!=MI_OK)
	{
		return MI_ERR;
	}
	
	if (bitOrder == 0) {
		// default
		pOutBuf[0] = RevBuffer[5];
		pOutBuf[1] = RevBuffer[4];
		pOutBuf[2] = RevBuffer[3];
		pOutBuf[3] = RevBuffer[2];
	} else {
		memcpy(pOutBuf,&RevBuffer[2],4); 
	}
	return MI_OK;
}
////////////////////////////////////////////////////////////////////////////////
//RC522:RST--PB0  CS--PA4  CLK--PA5  MISO--PA6  MOSI--PA7
void RC522_GPIO_Config() 
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	// PA4, PA5, PA7推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PB0推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PA6输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

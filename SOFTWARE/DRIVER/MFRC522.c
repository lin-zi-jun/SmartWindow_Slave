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
** ������:SPIReadByte
** ��������:GPIOģ��SPI����һ���ֽ�
** �������: ��
** �������: ��
** ��    �أ�dat--��ȡ���ֽ�
***********************************************************/
unsigned char SPIReadByte(void)
{
  char i;
  u8 dat=0;                  
  SET_SPI_CK();//CLK=1
  for(i=0;i<8;i++)
  {
    CLR_SPI_CK();//CLK=0,CLK�½��ؽ��Ĵ����������͵�MISO��������
    dat<<=1; 
	if(STU_SPI_MISO()) //��ȡMISO��������
    {
       dat|=0x01;
    }
    SET_SPI_CK();//CLK=1
  }       
  return (dat); 
  
}
 
/**********************************************************
** ������:SPIWriteByte
** ��������:GPIOģ��SPI��дһ���ֽ�
** �������: dat--д����ֽ�����
** �������: ��
** ��    �أ���
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
    SET_SPI_CK();	  //CLK=1,CLK�����ذ�����д��RC522 
    dat<<= 1;
  }          
	
} 
/**********************************************************
** ������:Read_Reg
** ��������:��RC522�Ĵ���
** �������: Address:�Ĵ�����ַ
** �������: ��
** ��    �أ�dat--������ֵ
***********************************************************/
unsigned char Read_Reg(unsigned char Address)
{
    unsigned char dat=0;
	CLR_SPI_CS();//CS=0,ʹ��Ƭѡ
	SPIWriteByte(((Address<<1)&0x7E)|0x80);
	dat=SPIReadByte();
	SET_SPI_CS();
   return dat;
}

/**********************************************************
** ������:Write_Reg
** ��������:дRC522�Ĵ���
** �������: Address:�Ĵ�����ַ
			 value:д���ֵ
** �������: ��
** ��    �أ���
***********************************************************/
void Write_Reg(unsigned char Address, unsigned char value)
{  
	CLR_SPI_CS();	 //CS=0,ʹ��Ƭѡ
	SPIWriteByte((Address<<1)&0x7E);
	SPIWriteByte(value);
	SET_SPI_CS();	//CS=1
} 
/**********************************************************
** ������:SetBitMask
** ��������:��RC522�Ĵ�����maskΪ1����Ӧλ��λ
** �������: reg--�Ĵ�����ַ
			 mask--��λֵ
** �������: ��
** ��    �أ���
** ���÷�����SetBitMask(TxControlReg, 0x03)����TxControlReg��0λ����1λ��1
***********************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = Read_Reg(reg); //��ȡ�Ĵ���ֵ
    Write_Reg(reg,tmp | mask);  // set bit mask
}

/**********************************************************
** ������:ClearBitMask
** ��������:��RC522�Ĵ�����maskΪ1����Ӧλ��0
** �������: reg--�Ĵ�����ַ
			 mask--��λֵ
** �������: ��
** ��    �أ���
** ���÷�����ClearBitMask(TxControlReg, 0x03)����TxControlReg��0λ����1λ��0
***********************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = Read_Reg(reg);//��ȡ�Ĵ���ֵ
    Write_Reg(reg, tmp & ~mask);  // clear bit mask
} 

/**********************************************************
** ������:PcdAntennaOn
** ��������:�������ߣ�ÿ��������ر����߷���֮��Ӧ������1ms�ļ��
** �������: ��
** �������: ��
** ��    �أ���
***********************************************************/
void PcdAntennaOn(void)
{
    unsigned char i;
    i = Read_Reg(TxControlReg);
    if (!(i & 0x03))//���Tx2REFn��Tx1REFnλΪ0
    {
        SetBitMask(TxControlReg, 0x03);//Tx2REFn��Tx1REFnλ��1
    }
}
/**********************************************************
** ������:PcdAntennaOff
** ��������:�ر�����
** �������: ��
** �������: ��
** ��    �أ���
***********************************************************/
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);//Tx2REFn��Tx1REFnλ��0��ֹͣ����
}
/**********************************************************
** ������:CalulateCRC
** ��������:��RC522����CRC16����
** �������: *pIndata--CRC�������ݵ�ַ��ָ��
			 len--�������ݳ���
			 *pOutData--������ݵ�ַ��ָ��	
** �������: ��
** ��    �أ���
***********************************************************/
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);//��CRC������Чʱ�������ݽ�������
    Write_Reg(CommandReg,PCD_IDLE);//ȡ����ǰ����
    SetBitMask(FIFOLevelReg,0x80);// ����FIFO������ָ�룬��ջ�����
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
    pOutData[0] = Read_Reg(CRCResultRegL);//�õ�����������
    pOutData[1] = Read_Reg(CRCResultRegM);
}  
/**********************************************************
** ������:PcdComMF522
** ��������:ͨ��RC522��ISO14443��ͨѶ
** �������: Command:RC522������
             *pInData:ͨ��RC522���͵���Ƭ�����ݵ�ַ��ָ��
             InLenByte:�������ݵ��ֽڳ���     
             *pOutData:���յ��Ŀ�Ƭ�������ݵ�ַ��ָ��
             *pOutLenBit:�������ݵ�λ���ȵ�ַ��ָ��
** �������: ��
** ��    �أ�ʧ�ܷ���MI_ERR���ɹ�����MI_OK
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
        case PCD_AUTHENT:  //��֤��Կ
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE://���Ͳ���������
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
    }
   
    Write_Reg(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    Write_Reg(CommandReg,PCD_IDLE);//����״̬
    SetBitMask(FIFOLevelReg,0x80);//���FIFO��дָ��,��ջ�����
    
    for (i=0; i<InLenByte; i++)	//д���ݵ�FIFO
    {   Write_Reg(FIFODataReg, pInData[i]);    }

    Write_Reg(CommandReg, Command);
    
    if (Command == PCD_TRANSCEIVE) //��������
    {    SetBitMask(BitFramingReg,0x80);  }	//��ʼ��������
    
    //����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
	i = 2000;
    do 
    {
        n = Read_Reg(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));//�ȴ��������
    ClearBitMask(BitFramingReg,0x80);//������ɣ��������λ

    if (i!=0)
    {    
        if(!(Read_Reg(ErrorReg)&0x1B)) //����޴���
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = (char)MI_NOTAGERR;   }  //���ؿ��޷�Ӧ��Ϣ		  
            if (Command == PCD_TRANSCEIVE) //��������
            {
               	n = Read_Reg(FIFOLevelReg);	//��ȡ����
              	lastBits = Read_Reg(ControlReg) & 0x07;	//�õ��������ֽڵ���Чλ
                if (lastBits) //�����Ϊ0���õ���Ч��λ��
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else //���RxLastBits(2-0λ)��Ϊ0�������������������ֽ�����Ч��
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)	 //��ȡ���յ�����
                {   pOutData[i] = Read_Reg(FIFODataReg);    }
            }
        }
        else
        {   status = (char)MI_ERR;   }
        
    }
   
    SetBitMask(ControlReg,0x80);//ֹͣ��ʱ��
    Write_Reg(CommandReg,PCD_IDLE); //�������״̬
    return status;
}                  
/**********************************************************
** ������:PcdRequest
** ��������:Ѱ��
** �������: req_code:Ѱ����ʽ
                0x52--Ѱ��Ӧ�������з���14443A��׼�Ŀ�
                0x26--Ѱδ��������״̬�Ŀ�
             *pTagType����Ƭ���ʹ����ַָ��
                0x4400--Mifare_UltraLight
                0x0400--Mifare_One(S50)
                0x0200--Mifare_One(S70)
                0x0800--Mifare_Pro(X)
                0x4403--Mifare_DESFire
** �������: ��
** ��    �أ��ɹ�����MI_OK
***********************************************************/
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
	char status;  
	unsigned int unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 
	//��Status2Reg�Ĵ���MFCrypto1Onλ��0����1ʱʹ��MIFARE���ܵ�Ԫ
	//���п��ϵ�����ͨ���Ǳ����ܵ�
	ClearBitMask(Status2Reg,0x08);//�رռ��ܵ�Ԫ
	Write_Reg(BitFramingReg,0x07);//���һ���ֽ�Ӧ����7λ��������յ�������Ч
	SetBitMask(TxControlReg,0x03);//��λTx2REFn��Tx1REFn,��������
 
	ucComMF522Buf[0] = req_code;//Ѱ����ʽ
	//����Ѱ����ʽ���뿨ͨѶ��1�ֽ�
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
	//USART1_Putc(ucComMF522Buf[0]);
	//USART1_Putc(ucComMF522Buf[1]);
	//USART1_Printf("status:%d, unLen:%d\r\n", status, unLen);
	if ((status == MI_OK) && (unLen == 0x10))//ͨѶOK���õ�2�ֽ�����--������
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   status = (char)MI_ERR;   }
   
	return status;
}

/**********************************************************
** ������:PcdAnticoll
** ��������:����ײ
** �������: *pSnr:��Ƭ���кŵ�ַָ�룬��Ƭ���к�4�ֽ�
** �������: ��
** ��    �أ��ɹ�����MI_OK
***********************************************************/
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);//������
	//�üĴ���ָ������֡�����һ���ֽںͽ���֡��һ���ֽ��в������ı��ص�λ��
    Write_Reg(BitFramingReg,0x00);//���һ���ֽڴ������0λ
    ClearBitMask(CollReg,0x80);//������ײ�����н��յ�λ��0
 
    ucComMF522Buf[0] = PICC_ANTICOLL1; //���ͷ���ײָ��
    ucComMF522Buf[1] = 0x20;
	//����2�ֽ������뿨ͨѶ
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++) //�õ���ѡ�еĿ������к�
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
** ������:PcdSelect
** ��������:ѡ����Ƭ
** �������: *pSnr:��Ƭ���кŵ�ַָ�룬��Ƭ���к�4�ֽ�
** �������: ��
** ��    �أ��ɹ�����MI_OK
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
	//�������к���CRC����
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);
	//���Ϳ����к��뿨Ƭͨ��
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }	//����OK
    else
    {   status = (char)MI_ERR;    }

    return status;
}

/**********************************************************
** ������:PcdHalt
** ��������:���Ƭ��������״̬
** �������: ��
** �������: ��
** ��    �أ��ɹ�����MI_OK
***********************************************************/
char PcdHalt(void)
{
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;//��������
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return MI_OK;
}

/**********************************************************
** ������:PcdReset
** ��������:��λRC522
** �������: ��
** �������: ��
** ��    �أ��ɹ�����MI_OK
***********************************************************/
char PcdReset(void)
{
	SET_RC522RST();//RST=1
    delay_us(10);
	CLR_RC522RST();//RST=0
    delay_us(10);
	SET_RC522RST();//RST=1
    delay_us(10);
    Write_Reg(CommandReg,PCD_RESETPHASE);//д��λ�����λ
    delay_us(10);
    
    Write_Reg(ModeReg,0x3D);//��Mifare��ͨѶ��CRC��ʼֵ0x6363
    Write_Reg(TReloadRegL,30);//���ض�ʱʱ��          
    Write_Reg(TReloadRegH,0);
    Write_Reg(TModeReg,0x8D);//��ʱ��ģʽ���ã����ݷ�����ɺ��Զ�������ʱ��
    Write_Reg(TPrescalerReg,0x3E);//��ʱ����Ƶϵ��Ϊ0X3E
	
	Write_Reg(TxAutoReg,0x40);//100%ASK����
   
    return MI_OK;
}
/**********************************************************
** ������:PcdConfigISOType
** ��������:����RC522�Ĺ�����ʽ
** �������: ��
** �������: ��
** ��    �أ��ɹ�����MI_OK
***********************************************************/
char PcdConfigISOType(unsigned char type)
{
   if (type == 'A') //ISO14443_A��
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
** ������:InitRC522
** ��������:��ʼ��RC522
** �������: ��
** �������: ��
** ��    �أ���
***********************************************************/
void InitRC522(void)
{
  RC522_GPIO_Config();
  delay_ms(1);
  PcdReset(); //RC522��λ
  PcdAntennaOff();//�ر�����  
  PcdAntennaOn();//��������
  PcdConfigISOType('A');//ISO14443_A��
}

////////////////////////////////////////////////////////////////////////////////
int ReadRFID(unsigned char *pOutBuf, unsigned char bitOrder)
{
  	char status;
	
	PcdReset();
	//status=PcdRequest(PICC_REQALL,&RevBuffer[0]);//Ѱ�����������п������ؿ�Ƭ���� 2�ֽ�
	status=PcdRequest(PICC_REQIDL,&RevBuffer[0]);//Ѱ�����������п������ؿ�Ƭ���� 2�ֽ�
	if(status!=MI_OK)
	{
		return MI_NOTAGERR;
	}
	status=PcdAnticoll(&RevBuffer[2]);//����ײ�����ؿ������к� 4�ֽ�
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
	// PA4, PA5, PA7�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PB0�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PA6����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

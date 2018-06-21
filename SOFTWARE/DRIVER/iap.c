#include "headers.h"
///////////////////////////////////////////////////////////////////////////////
const unsigned short ccitt_table[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
///////////////////////////////////////////////////////////////////////////////
uint16_t crc_ccitt(unsigned char *q, int len)
{
	unsigned short crc = 0;

	while (len-- > 0)
		crc = ccitt_table[(crc >> 8 ^ *q++) & 0xff] ^ (crc << 8);
	return ((~crc)&0xFFFF);
}
///////////////////////////////////////////////////////////////////////////////
/**
  * @brief  Calculate the number of pages
  * @param  Size: The image size
  * @retval The number of pages
  */
uint32_t FLASH_PagesMask(uint32_t Size)
{
  uint32_t pagenumber = 0x0;
  uint32_t size = Size;

  if ((size % PAGE_SIZE) != 0)
  {
    pagenumber = (size / PAGE_SIZE) + 1;
  }
  else
  {
    pagenumber = size / PAGE_SIZE;
  }
  return pagenumber;
}
///////////////////////////////////////////////////////////////////////////////
void FlashErase(uint32_t size)
{
	u16 NbrOfPage, EraseCounter;
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	NbrOfPage = FLASH_PagesMask(size);

	/* Erase the FLASH pages */
	for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
	{
		FLASHStatus = FLASH_ErasePage(FLASH_BASE + (PAGE_SIZE * EraseCounter));
	}
}
///////////////////////////////////////////////////////////////////////////////
FLASH_Status WriteBootLoader(uint32_t offset, uint8_t *buf, uint16_t size)
{
	u8 *p = buf;
	u16 i;
    u32 address = FLASH_BASE+offset;
    
    FLASH_Status status = FLASH_COMPLETE;
    
    /* Unlock the Flash Program Erase controller */
    FLASH_Unlock();
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

    /* Erase the FLASH pages */
    
    for(i=0;i<size;i+=4)
    {    
        status = FLASH_ProgramWord(address,*(u32*)p);
        if(status != FLASH_COMPLETE) return status;
        address+=4;
        p+=4;
    }
    
    return FLASH_COMPLETE;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// for device config, write in the last page(255)
// STM32f103ZET6,512KB(2K*256) Flash on chip
//extern DevInfo dev;
extern RFCard_ModuleInfo ModuleInfo;
extern SupperCardInfo suppercardinfo;
extern WeightInfo weightinfo;
FLASH_Status WriteFlashDevInfo(void) 
{
	u32 address = FLASH_BASE+CONF_PAGE_NO*PAGE_SIZE;
    FLASH_Status status = FLASH_COMPLETE;
	u32 *p;
    
    /* Unlock the Flash Program Erase controller */
    FLASH_Unlock();
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

	USART1_Printf("WriteFlashDevInfo!\r\n");
    /* Erase the FLASH pages */
    status = FLASH_ErasePage(address);
	if(status != FLASH_COMPLETE) {
		USART1_Printf("FLASH_ErasePage error!\r\n");
		return status;
	}
	
	status = FLASH_ProgramWord(address,suppercardinfo.card);
    if(status != FLASH_COMPLETE) {
		USART1_Printf("FLASH_ProgramWord1 addr:0x%x!\r\n", address);
		return status;
	}

	address += sizeof(u32);
//	p=&(weightinfo.Gross_Weight);
	status = FLASH_ProgramWord(address,weightinfo.Gross_Weight);
    if(status != FLASH_COMPLETE) {
		USART1_Printf("FLASH_ProgramWord1 addr:0x%x!\r\n", address);
		return status;
	}	
	
	address += sizeof(u32);
//	p=&(weightinfo.shiwu_Weight);
	status = FLASH_ProgramWord(address,weightinfo.shiwu_Weight);
    if(status != FLASH_COMPLETE) {
		USART1_Printf("FLASH_ProgramWord1 addr:0x%x!\r\n", address);
		return status;
	}
    return FLASH_COMPLETE;
	
}
///////////////////////////////////////////////////////////////////////////////
void ReadFlashDevInfo(void) 
{
	u32 address;
	address = FLASH_BASE+CONF_PAGE_NO*PAGE_SIZE;
	USART1_Printf("ReadFlashDevInfo!\r\n");
	USART1_Printf("address:0x%x!\r\n", address);
	memcpy(&(suppercardinfo.card), (u32*)address, sizeof(u32));
	memcpy(&(weightinfo.Gross_Weight), (u32*)(address+sizeof(u32)), sizeof(u32));
	memcpy(&(weightinfo.shiwu_Weight), (u32*)(address+2*sizeof(u32)), sizeof(u32));
		
}
///////////////////////////////////////////////////////////////////////////////
FLASH_Status WriteFlashRFCardInfo(void) 
{
	u32 address = FLASH_BASE+RFCARDINFO_PAGE_NO*PAGE_SIZE;
    FLASH_Status status = FLASH_COMPLETE;
	u8 *p, i;
    
    /* Unlock the Flash Program Erase controller */
    FLASH_Unlock();
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

	USART1_Printf("WriteFlashRFCardInfo!\r\n");
    /* Erase the FLASH pages */
    status = FLASH_ErasePage(address);
	if(status != FLASH_COMPLETE) {
		USART1_Printf("FLASH_ErasePage error!\r\n");
		return status;
	}
	
	p = (u8*)&ModuleInfo;
    for(i=0;i<sizeof(RFCard_ModuleInfo);i+=4)
    {    
        status = FLASH_ProgramWord(address,*(u32*)p);
        if(status != FLASH_COMPLETE) {
			USART1_Printf("FLASH_ProgramWord3 addr:0x%x!\r\n", address);
			return status;
		}
        address+=4;
        p+=4;
    }
}
///////////////////////////////////////////////////////////////////////////////
void ReadFlashData(void)
{
	u32 address;
	u16 i;
	address = FLASH_BASE+RFCARDINFO_PAGE_NO*PAGE_SIZE;
	memcpy(&ModuleInfo, (u32*)(address), sizeof(RFCard_ModuleInfo));
	

}
///////////////////////////////////////////////////////////////////////////////
FLASH_Status WriteFlash(u8 page, uint8_t *buf, uint16_t size)
{
	u8 *p = buf, page_cnt;
	u16 i;
    u32 address;
    
    FLASH_Status status = FLASH_COMPLETE;
    
    /* Unlock the Flash Program Erase controller */
    FLASH_Unlock();
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

	if (size ==0) {
		address = FLASH_BASE+(page)*PAGE_SIZE;
		status = FLASH_ErasePage(address);
		return status;
	}
    /* Erase the FLASH pages */
    if ((size & (PAGE_SIZE-1))!=0) {
		page_cnt = (size / PAGE_SIZE) + 1;
	} else {
		page_cnt = size / PAGE_SIZE;
	}
	for (i=0; i<page_cnt; i++) {
		address = FLASH_BASE+(page+i)*PAGE_SIZE;
		USART1_Printf("ErasePage address:0x%X!\r\n", address);
		status = FLASH_ErasePage(address);
		if(status != FLASH_COMPLETE) {
			USART1_Printf("FLASH_ErasePage error!\r\n");
			return status;
		}
	}
	
	address = FLASH_BASE+page*PAGE_SIZE;
    for(i=0;i<size;i+=4)
    {    
        status = FLASH_ProgramWord(address,*(u32*)p);
        if(status != FLASH_COMPLETE) return status;
        address+=4;
        p+=4;
    }
    
    return FLASH_COMPLETE;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
FLASH_Status FlashEraseLastPage(void)
{
	FLASH_Status Status = FLASH_COMPLETE;
	Status = FLASH_ErasePage(FLASH_BASE + (PAGE_SIZE * 255));
	return  Status;
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void RunMainProgram(uint32_t ProgramAddr) {
	pMainProgram MainProgram;
	uint32_t JumpAddress;
	if (((*(__IO uint32_t*)ProgramAddr) & 0x2FFE0000 ) == 0x20000000) {
		JumpAddress = *(__IO uint32_t*) (ProgramAddr);
		/* Jump to user application */
		MainProgram = (pMainProgram) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) (ProgramAddr));
		MainProgram();
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



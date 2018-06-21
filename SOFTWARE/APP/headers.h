/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_HEADERS_H
#define __COMMON_HEADERS_H
////////////////////////////////////////////////////////////////////////////////
#include "stm32f10x.h"
#include "bsp.h"
#include "App.h"
#include "ili93xx.h"
#include "rtc.h"
#include "mfrc522.h"
#include "usart.h"
#include "gui.h"
#include "ctrio_driver.h"
#include "common_def.h"
#include "sram.h"
#include "sys.h"
#include "delay.h"
#include "os.h"
#include "common_types.h"
#include "common_func.h"
//#include "FileHelper.h"
#include "ui.h"
#include "can.h"
#include "hx711.h"
#include "math.h"
#include "iap.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdarg.h>

#define WATCHDOG_EN		1
#define USE_EXT_SRAM	0


#define BUZZER_ONE_SEC		10
#define BUZZER_WARN_SEC		50
#define NET_RST_CNT			50
#define GBUF_LEN			128
#define SUPPER_ONE_SEC		40
#define FAILHANDER_ONE_SEC	20
#define UISHOW_ONE_SEC      10
#define UISHOW_TWO_SEC      50
#define UISHOW_THREE_SEC    300

#define BOOTSW              (0x0807F800)  /* uboot switch */
#define CR_PG_Set           ((uint32_t)0x00000001)
#define CR_PG_Reset         ((uint32_t)0x00001FFE) 
#define IAPAddress    		  (0x8000000)
#define ProgramTimeout      ((uint32_t)0x00002000)

//////////////////////////////////////////////////////////////////////////////
#define ACK                     (0x06)  /* acknowledge */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */
#endif	/* __COMMON_HEADERS_H */

#include "ui.h"
#include "LCD_ConfDefaults.h"
#include "FHZK12GUI.h"
#include "WM_Intern_ConfDep.h" 
#include <stdlib.h>
#include "BUTTON.h"
#include "FRAMEWIN.h"
#include "TEXT.h"
///////////////////////////////////////////////////////////////////////////////
#define WIN_HEIGHT	25
#define ITEM_HEIGTH	33
#define BTN_WIDTH	75
#define BTN_LEFT	5
#define TEXT_LEFT	85
///////////////////////////////////////////////////////////////////////////////
TEXT_Handle hTextState;
TEXT_Handle hTextInfo;
TEXT_Handle hTextRemain;
TEXT_Handle hTextUse;
TEXT_Handle hTextTime;
TEXT_Handle hTextID;
extern GUI_CONST_STORAGE GUI_BITMAP bmlogo;
extern OS_SEM mainSem;
//extern u8 FILE_DATA[250000];

u8 *pdata;
///////////////////////////////////////////////////////////////////////////////
void UI_Init(void) {
	BUTTON_Handle hButton_1;
	BUTTON_Handle hButton_2;
	BUTTON_Handle hButton_3;
	BUTTON_Handle hButton_4;
	BUTTON_Handle hButton_5;
	FRAMEWIN_Handle hFrame;
	u8 yOffset;
	
	GUI_Init();
	GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ12);
	//GUI_SetFont(&GUI_Font8x16);
	GUI_Clear();
		
	GUI_DrawBitmap(&bmlogo,0,0);
	FRAMEWIN_SetDefaultCaptionSize(24);
	FRAMEWIN_SetDefaultFont(&GUI_FontHZ12);
	TEXT_SetDefaultFont(&GUI_FontHZ12);
	//FRAMEWIN_SetDefaultFont(&GUI_Font8x16);
	//TEXT_SetDefaultFont(&GUI_Font8x16);
	hFrame = FRAMEWIN_Create("橱窗实验室管理系统", 0, WM_CF_SHOW, 0, 53, 320, 187);
	FRAMEWIN_SetTextAlign(hFrame, GUI_TA_HCENTER|GUI_TA_VCENTER);
	FRAMEWIN_SetActive(hFrame,1);
	
	yOffset = 22;
	hButton_1 = BUTTON_CreateAsChild(BTN_LEFT, yOffset+ITEM_HEIGTH*1, 
		BTN_WIDTH, WIN_HEIGHT, hFrame, 1, WM_CF_SHOW);
	BUTTON_SetFont(hButton_1, &GUI_FontHZ12);
	BUTTON_SetText(hButton_1, "信息提示");
	hButton_2 = BUTTON_CreateAsChild(BTN_LEFT, yOffset+ITEM_HEIGTH*2, 
		BTN_WIDTH, WIN_HEIGHT, hFrame, 1, WM_CF_SHOW);
	BUTTON_SetFont(hButton_2, &GUI_FontHZ12);
	BUTTON_SetText(hButton_2, "仪器状态");
	hButton_3 = BUTTON_CreateAsChild(BTN_LEFT, yOffset+ITEM_HEIGTH*3, 
		BTN_WIDTH, WIN_HEIGHT, hFrame, 1, WM_CF_SHOW);
	BUTTON_SetFont(hButton_3, &GUI_FontHZ12);
	BUTTON_SetText(hButton_3, "剩余时间");
	hButton_4 = BUTTON_CreateAsChild(BTN_LEFT, yOffset+ITEM_HEIGTH*4, 
		BTN_WIDTH, WIN_HEIGHT, hFrame, 1, WM_CF_SHOW);
	BUTTON_SetFont(hButton_4, &GUI_FontHZ12);
	BUTTON_SetText(hButton_4, "已用时间");
	
	hButton_5 = BUTTON_CreateAsChild(165, yOffset+ITEM_HEIGTH*1, 
		BTN_WIDTH, WIN_HEIGHT, hFrame, 1, WM_CF_SHOW);
	BUTTON_SetFont(hButton_5, &GUI_FontHZ12);
	BUTTON_SetText(hButton_5, "用户卡号");
	
	
	hTextTime = TEXT_Create( 5, 82, 305, WIN_HEIGHT, 6, 
		WM_CF_SHOW, NULL,
		TEXT_CF_HCENTER|TEXT_CF_VCENTER);
		
	yOffset = 74;
	hTextInfo = TEXT_Create(TEXT_LEFT, yOffset+ITEM_HEIGTH*1, 80, WIN_HEIGHT, 0, 
		WM_CF_SHOW, NULL,
		TEXT_CF_LEFT|TEXT_CF_VCENTER);
	
	hTextState = TEXT_Create(TEXT_LEFT, yOffset+ITEM_HEIGTH*2, 120, WIN_HEIGHT, 1, 
		WM_CF_SHOW,"仪器已入柜",
		TEXT_CF_LEFT|TEXT_CF_VCENTER);
	
	hTextRemain = TEXT_Create(TEXT_LEFT, yOffset+ITEM_HEIGTH*3, 200, WIN_HEIGHT, 2, 
		WM_CF_SHOW, NULL,
		TEXT_CF_LEFT|TEXT_CF_VCENTER);
	
	hTextUse = TEXT_Create(TEXT_LEFT, yOffset+ITEM_HEIGTH*4, 200, WIN_HEIGHT, 3, 
		WM_CF_SHOW, NULL,
		TEXT_CF_LEFT|TEXT_CF_VCENTER);
	hTextID = TEXT_Create(245, yOffset+ITEM_HEIGTH*1, 85, WIN_HEIGHT, 4, 
		WM_CF_SHOW, NULL,
		TEXT_CF_LEFT|TEXT_CF_VCENTER);
//		OSSemPost(&mainSem, 0, NULL);
//      show_image_power(200,120);
      OSSemPost(&mainSem, 0, NULL);
//      show_image_power(20,15);	
	GUI_WaitKey();
      	
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetInfo(u8 *str) {
	TEXT_SetText(hTextInfo, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetStateColor(u8 *str) {
	TEXT_SetText(hTextState, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetState(u8 *str) {
	TEXT_SetText(hTextState, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetRemain(u8 *str) {
	TEXT_SetText(hTextRemain, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetUse(u8 *str) {
	TEXT_SetText(hTextUse, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetTime(u8 *str) {
	TEXT_SetText(hTextTime, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetID(u8 *str) {
	TEXT_SetText(hTextID, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

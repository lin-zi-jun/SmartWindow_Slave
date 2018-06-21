#ifndef	__COMMON_FUN_H
#define __COMMON_FUN_H
////////////////////////////////////////////////////////////////////////////////
#include "headers.h"
////////////////////////////////////////////////////////////////////////////////
void allocExtSram(void);
u8 PCCmdHandler(u8 *pBuf);
void SvrCmdHandler(u8 *pBuf);
void OnSecondTimer(void);
void UseTimeHandler(void);
void InitState(void);
void InitFlashFS(void);
u16 saveUserList(u8 *buf, u8 firstSection, char *fileName);
u16 findUserCard(u32 card, char *fileName);
u16 isAdminCard(u32 card);
u16 isUserCard(u32 card);
u16 findBookInfo(u16 userId, u32 now, BookInfo *bookInfo);
u16 saveBookList(u8 *buf, u8 firstSection, char *fileName);
void RFCardHandler(void);
void OfflineHandler(void);
void BuzzerHandler(void);
void NetResetHandler(void);
void Dump(void);
void char2int(u8* buff,u8 *pOutBuf);
void int2char(u32 cd,u8*buf);
void SupperCardHander(void);
void SupperCard_off(void);
void Can_Hander(void);
void UIShowHandler(void);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif /* __COMMON_FUN_H */

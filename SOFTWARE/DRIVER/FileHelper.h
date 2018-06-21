#ifndef	__FILE_HELPER_H
#define __FILE_HELPER_H
////////////////////////////////////////////////////////////////////////////////
#include "headers.h"
////////////////////////////////////////////////////////////////////////////////
#define DEV_FILE_NAME		"dev_file"
#define USER_FILE_NAME		"user_file"
#define BOOK_FILE_NAME		"book_file"
#define ADMIN_FILE_NAME		"admin_file"
#define OFFLINE_FILE_NAME	"offline_file"
#define WEIGHT_FILE_NAME      "weight_file"
#define DEV_RFCARD_FILE_NAME    "dev_RFCard_file"
#define SUPPERCARD_FILE_NAME    "suppercard_file"
////////////////////////////////////////////////////////////////////////////////
s16 readFile(char* fileName, u8 *buf, u16 len);
s16 writeFile(char* fileName, u8 *buf, u16 len);
s16 appendFile(char* fileName, u8 *buf, u16 len);
s16 truncateFile(char* fileName);
////////////////////////////////////////////////////////////////////////////////
// s16 saveDevInfo(void);
// s16 readDevInfo(void);
////////////////////////////////////////////////////////////////////////////////
u16 updateOfflineFile(OfflineInfo *info);
s16 readSupperCardInfo(void);
s16 saveSupperCardInfo(void);
s16 readDev_RFCardInfo(void);
s16 saveDev_RFCardInfo(void);
s16 readimageInfo(void);
s16 saveimageInfo(void);
s16 readWeightInfo(void);
s16 saveWeightInfo(void);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif /* __FILE_HELPER_H */

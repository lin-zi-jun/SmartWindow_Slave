#include "headers.h"

u8 IMAGE_INFO_SEND_FLAG=0;
///////////////////////////////////////////////////////////////////////////////
s16 readFile(char* fileName, u8 *buf, u16 l) {
	s16 len = -1;
	FILE *file;
	file = fopen(fileName, "rb");
	if (file != NULL) {
		len = fread(buf, 1, l, file);
		fclose(file);
		USART1_Printf("read file %s ok,len=%d.\r\n", fileName,len);
		return len;
	}
	USART1_Printf("read file %s failed,len=%d.\r\n", fileName,len);
	return len;
}
///////////////////////////////////////////////////////////////////////////////
s16 writeFile(char* fileName, u8 *buf, u16 l) {
	s16 len = -1;
	FILE *file;
      
	file = fopen(fileName, "wb");
	if (file != NULL) {
		len = fwrite(buf, 1, l, file);
		fclose(file);
		USART1_Printf("write file %s ok,len=%d.\r\n", fileName,len);
            
		return len;
	}
	USART1_Printf("write file %s failed,len=%d.\r\n", fileName,len);
	return len;
}
///////////////////////////////////////////////////////////////////////////////
s16 appendFile(char* fileName, u8 *buf, u16 l) {
	s16 len = -1;
	FILE *file;
	file = fopen(fileName, "ab");
	if (file != NULL) {
		len = fread(buf, 1, l, file);
		fclose(file);
		USART1_Printf("append file %s ok.\r\n", fileName);
		return len;
	}
	USART1_Printf("append file %s failed.\r\n", fileName);
	return len;
}
///////////////////////////////////////////////////////////////////////////////
s16 truncateFile(char* fileName) {
	s16 len = -1;
	FILE *file;
	file = fopen(fileName, "w");
	if (file != NULL) {
		fclose(file);
		len = 0;
		USART1_Printf("truncate file %s ok.\r\n", fileName);
		return len;
	}
	USART1_Printf("truncate file %s failed.\r\n", fileName);
	return len;
}
///////////////////////////////////////////////////////////////////////////////
//extern DevInfo dev;
extern image  image1;
extern RFCard_ModuleInfo ModuleInfo;
extern SupperCardInfo suppercardinfo;
extern WeightInfo weightinfo;
// s16 saveDevInfo(void) {
// 	return writeFile(DEV_FILE_NAME, (u8*)&dev, sizeof(DevInfo));
// }
// s16 readDevInfo(void) {
// 	return readFile(DEV_FILE_NAME, (u8*)&dev, sizeof(DevInfo));
//}
s16 readDev_RFCardInfo(void) {
	return readFile(DEV_RFCARD_FILE_NAME, (u8*)&ModuleInfo, sizeof(RFCard_ModuleInfo));
}
s16 saveDev_RFCardInfo(void) {
	return writeFile(DEV_RFCARD_FILE_NAME, (u8*)&ModuleInfo, sizeof(RFCard_ModuleInfo));
}
s16 readSupperCardInfo(void) {
	return readFile(SUPPERCARD_FILE_NAME, (u8*)&suppercardinfo, sizeof(SupperCardInfo));
}
s16 saveSupperCardInfo(void) {
	return writeFile(SUPPERCARD_FILE_NAME, (u8*)&suppercardinfo, sizeof(SupperCardInfo));
}
s16 readWeightInfo(void) {
	return readFile(WEIGHT_FILE_NAME, (u8*)&weightinfo, sizeof(WeightInfo));
}
s16 saveWeightInfo(void) {
	return writeFile(WEIGHT_FILE_NAME, (u8*)&weightinfo, sizeof(WeightInfo));
}

s16 saveimageInfo(void) {
   if(IMAGE_INFO_SEND_FLAG==1)
     {
       IMAGE_INFO_SEND_FLAG=0;
       fdelete("imageinfo");
	 return writeFile("imageinfo", (u8*)&image1, sizeof(image));
     }
//		 return 0;
}
s16 readimageInfo(void) {
	return readFile("imageinfo", (u8*)&image1, sizeof(image));
}
///////////////////////////////////////////////////////////////////////////////
// u16 updateOfflineFile(OfflineInfo *info) {//更新离线文件
// 	FILE *file;
// 	OfflineInfo data;
// 	u32 len=0;
// 	u16 cnt = 0;
// 	u8 not_found = 1;
// 	
// 	file = fopen(OFFLINE_FILE_NAME, "rb+");
// 	if (file == NULL) {
// 		//文件不存在
// 		file = fopen(OFFLINE_FILE_NAME, "wb+");
// 		if (file!=NULL) {
// 			fwrite(info, 1, sizeof(OfflineInfo), file);
// 			fclose(file);
// 			cnt = 1;
// 		}
// 	} else {
// 		fseek(file, 0, SEEK_SET);
// 		while (!feof(file)) {
// 			if (fread(&data, 1, sizeof(OfflineInfo), file)==sizeof(OfflineInfo)) {
// 				cnt ++;
// 				if ((data.id==info->id) && (data.btime==info->btime)) {
// 					not_found = 0;
// 					fseek(file, len, SEEK_SET);
// 					fwrite(info, 1, sizeof(OfflineInfo), file);
// 					fseek(file, 0, SEEK_END);
// 					cnt = ftell(file)/sizeof(OfflineInfo);
// 					break;
// 				} else {
// 					len += sizeof(OfflineInfo);
// 				}
// 			}
// 		}
// 		if (not_found == 1) {
// 			fwrite(info, 1, sizeof(OfflineInfo), file);
// 			cnt++;
// 		}
// 		fclose(file);
// 	}
// 	return cnt;
// }
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifndef	__COMMON_TYPES_H
#define __COMMON_TYPES_H
////////////////////////////////////////////////////////////////////////////////
#include "headers.h"
////////////////////////////////////////////////////////////////////////////////
// 设备数据结构
typedef struct _DevInfo {      // sizeof() = 12
	// 终端的ID编号
	u16 SN;
	u8  RFOrder;
} DevInfo;
////////////////////////////////////////////////////////////////////////////////
// 用户数据结构
typedef __packed struct _UserInfo {      // sizeof() = 8
	u32	id;
	u32 card;
} UserInfo;
////////////////////////////////////////////////////////////////////////////////
//仪器借出后，所对应的卡号
typedef __packed struct _RFCard_Module_Info{
    u32 Module_SN;
    U32 UserCard_ID;
}RFCard_ModuleInfo;
////////////////////////////////////////////////////////////////////////////////
// 预约数据结构
typedef __packed struct _BookInfo {      // sizeof() = 12
	u32 id;
	u32 btime,etime;
} BookInfo;
////////////////////////////////////////////////////////////////////////////////
// 离线数据结构
typedef __packed struct _OfflineInfo {   // sizeof() = 16
    u32 id;
    u32 btime;
    u32 len;    // second counter
	u32 rid;	// random id
} OfflineInfo;
////////////////////////////////////////////////////////////////////////////////
//图片信息
///////////////////////////////////////////////////////////////////////////////
//重量信息
typedef  struct _WeightInfo{
      u32 id;
      u32 Gross_Weight;//毛重
      u32 shiwu_Weight;//实物重量
}WeightInfo;
//////////////////////////////////////////////////////////////////////////////
//超级卡
typedef  struct _SupperCardInfo {      // sizeof() = 8
	u32	id;
	u32 card;
} SupperCardInfo;
#endif	/* __COMMON_TYPES_H */

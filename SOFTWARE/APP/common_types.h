#ifndef	__COMMON_TYPES_H
#define __COMMON_TYPES_H
////////////////////////////////////////////////////////////////////////////////
//#include "headers.h"
#include "stm32f10x.h"
////////////////////////////////////////////////////////////////////////////////
// 设备数据结构
typedef struct _DevInfo {      // sizeof() = 12
	// 终端的ID编号
	u32 SN;
	u16 AdminLen;
	u16 UserLen;
	u16 BookLen;
	u16 OfflineLen;
    u8  ModuleInfoLen;
	u8  WorkType;
  u8  ErrorInfoLen;
//	u8  RFOrder;
} DevInfo;
////////////////////////////////////////////////////////////////////////////////
//仪器借出后，所对应的卡号
typedef __packed struct _ModuleInfo{
    u32 Module_SN;    
    u32 UserCard_ID;
    u32 id;
	u32 RandonID;
    u8  Module_state;
}ModuleInfo;
////////////////////////////////////////////////////////////////////////////////
//模块对应SN号
typedef struct _ModuleSN{
    u32 Module_1_SN;    
    u32 Module_2_SN;
    u32 Module_3_SN;    
    u32 Module_4_SN;
    u32 Module_5_SN;    
    u32 Module_6_SN;
}ModuleSN;
////////////////////////////////////////////////////////////////////////////////
// 用户数据结构
typedef __packed struct _UserInfo {      // sizeof() = 8
	u32	id;
	u32 card;
} UserInfo;
////////////////////////////////////////////////////////////////////////////////
// 预约数据结构
typedef __packed struct _BookInfo {      // sizeof() = 12
	u32 id;
	u32 moduleNO;
	u32 btime,etime;
} BookInfo;
////////////////////////////////////////////////////////////////////////////////
// 离线数据结构
typedef __packed struct _OfflineInfo {   // sizeof() = 16
    u32 Module_SN;
    u32 id;
    u32 Borrowtime;
    u32 Returntime;
	u32 rid;	// random id
} OfflineInfo;
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
typedef __packed struct _byte {
        u8 a:1;
        u8 b:1;
        u8 c:1;
        u8 d:1;
        u8 e:1;
        u8 f:1;
        u8 g:1;
        u8 h:1;
}byte;
/////////////////////////////////////////////////////////////////////////////////
// 离线数据结构
typedef __packed struct _ErrorInfo {   // sizeof() = 13
    u32 Module_SN;
    u32 UserdCardId;
    u32 Borrowtime;
    u8  Errortpye;

} ErrorInfo;
/////////////////////////////////////////////////////////////////////////////////

// 错误数据结构
typedef __packed struct _ErrorState {   // sizeof() = 16
    u32 Module_SN;
    u32 UserdCardId;
	u32 UserdId;
    u32 Borrowtime;
} ErrorState;
// // 离线数据结构
// typedef __packed struct _DifferReturnInfo {   // sizeof() = 16
//     u32 Module_SN;
//     u32 UserdCardId;
//     u32 Returntime;
// 	u32 rid;	// random id
// } DifferReturnInfo;
/////////////////////////////////////////////////////////////////////////////////
#endif	/* __COMMON_TYPES_H */

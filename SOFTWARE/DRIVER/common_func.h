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
//u16 saveUserList(u8 *buf, u8 firstSection, char *fileName);
u16 saveUserList(u8 *buf, u8 firstSection, u8 isAdmin);
u16 saveBookList(u8 *buf, u8 firstSection);
u16 findUserCard(u32 card, UserInfo *pList, u16 Len);
u16 isAdminCard(u32 card);
u16 isUserCard(u32 card);
u16 findBookInfo(u16 userId, u32 now,u32 cardbuf);

void RFCardHandler(void);
void OfflineHandler(void);
void BuzzerHandler(void);
void NetResetHandler(void);
void Dump(void);
void char2int(u8* buff,u8 *pOutBuf);
void int2char(u32 cd,u8*buf);
void SNHandler(u8*SNbuf);
void Door_close_Hander(u32 ID);
void Door_open_Hander(u32 ID);
void BorrowHander(ModuleInfo*info,u32 card_id,u16 user_id,u8 state,u32 SN_receivebuf);
void ReturnHander(ModuleInfo*info,u32 Return_Card_buf);
void moduleinfo_init(ModuleInfo *moduleinfo,u32 Return_SN);
void StateByte_init(void);
void ModuleStateHandler(u32 numb);
//void moduleinfo_init(ModuleInfo *moduleinfo);
u32 chooseSN(u32 StdId);
u32 findModule_Info(u32 SN,ModuleInfo*info);
void ErrorHandler(u32 numb);
void SupperCardHander(u32 nub);
void Error_OffineHandler(void);
u8 RFCardSupportHander(u32 cardbuf,u32 ExtId);
void RFCardMainHander(u32 cardbuf,u32 ExtId);
u8 findfreemodule_Info(u32 SN,u8 number);
void RFCard_Main_Handler(void);
void checkdoorstate(void);
void BuzzerHandler(void);
void moduleinfo_assignment(ModuleInfo*info,u32 module_SN,u32 card_id,u16 user_id,u8 state);
u16 updateErrorInfoFile(ErrorInfo*info);
u8 updateModuleInfo(ModuleInfo *info);
u16 updateOfflineInfo(OfflineInfo *info);
u8 onlinehander(u8* buf);
void findModule_RandonID(void);
void HeartbeatSend(void);
u8 updateErrorState(ErrorState *info);
void module_zero(u32 num);
void init_clear(void);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif /* __COMMON_FUN_H */

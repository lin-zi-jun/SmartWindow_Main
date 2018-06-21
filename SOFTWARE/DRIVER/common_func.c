#include "headers.h"
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// import from rtl.h
extern int finit (const char *drive);
extern int fformat (const char *drive);
///////////////////////////////////////////////////////////////////////////////
DevInfo dev;
BookInfo book;
OfflineInfo offlineInfo;
ModuleSN moduleSN;
byte StateByte;
ErrorInfo errorinfo;
ErrorState errorstate;

// 继电器开关导通计时
u32 gUseTime = 0;
// 服务器返回的能够使用时间
u32 gRemainTime = 0;
// 退出的标志
u32 gRandomId = 0;


u32 gRFCardIn, gRFCardUse;
u8 Card_IN=0;		// 刷卡状态，=1：有卡刷进来
u16 gUserId;		// 用户ID

u8 gBuzzerCnt;		// 蜂鸣器响声时间计数器
u32 gOfflineCnt;	// 离线计时器
u32 gOnlineCnt;		// 在线计数器
u32 gOnlineCntErr;    // 在线计数器
u16 gOfflinePos;	//
u16 gOfflinePosErr;	//
u8 gOfflineDly;
u8 gOfflineDlyErr;
u8 gNetRstCnt = 0;	// 有线网口复位状态延时计数器
u16 gNetDownCnt	= 0;// 有线网口
u32 gUsart1TOCnt, gUsart2TOCnt, gUsart3TOCnt;
u32 gSramOffset;
u8  SNbuf[4];
u8 check_time=0;
u32 timebuf=0;
u32 Card_buf=0;
u8 recevecard=0;//接收卡号标识
u8 borrow_state=0;//借出仪器状态
u32 Choose_SN=0;
u8 repay_state=0;//归还仪器状态
u8 gRFCardUse_buf[4];
u8 gRemainTime_buf[4];
u32 Return_Card_buf=0;
u8 Connect=0;
u32 SN_receive=0;
u32 SN_return=0;
u8 ModuleState=0;//模块状态
u8 module_clear=0;
u32 ErrorCard_Buf=0;
u8 SupperCardState=0;//超级卡状态
u8 Differ_State=0;//错还标识
u32 moduleNO_buf=0;
u8 Suppercard_return=0;//超级卡归还仪器标志
u8 Error_state=0;//错拿标志
//u32 moduleNO_return_buf=0;
u32 ExtId_buf=0;
u8 closestate=0;//门关闭状态
u8 closecnt=0;//门关闭时间计数
u16 closecount=0;
u8 closestate_many=0;
u8 openstate=0;//门打开的状态
u8 opencnt=0;//门打开时间计数
u32 RandonIDbuf[6];
u16 ModuleUserIdbuf[6];
u8 ModuleIdbuf[6];
u8 errorbuf[6];
u32 ErrorRandomID=0;
u8 ErrorStatelen=0;
u16 UserIdbuf=0;
u8 ErrorFlag=0;//错误状态标识

///////////////////////////////////////////////////////////////////////////////

extern u8 Usart1TxSem, Usart2TxSem, Usart3TxSem;
extern u32 FILE_SIZE; //发送文件大小
extern u8  FILE_SEND_FLAG;//文件发送标志
ModuleInfo moduleinfo;
CanRxMsg RxMessage;
///////////////////////////////////////////////////////////////////////////////
//USART_FIFO_LEN 1024
extern USART_FIFO_TYPE *pUSART1_FIFO, *pUSART2_FIFO, *pUSART3_FIFO;
#if (USE_EXT_SRAM == 0)
extern USART_FIFO_TYPE USART1_FIFO, USART2_FIFO, USART3_FIFO;
#endif
//USART_PRINT_BUF_LEN 512
extern u8 *pUSART1_Print_Buf, *pUSART2_Print_Buf, *pUSART3_Print_Buf;
#if (USE_EXT_SRAM == 0)
extern u8 USART1_Print_Buf[USART_PRINT_BUF_LEN], USART2_Print_Buf[USART_PRINT_BUF_LEN], USART3_Print_Buf[USART_PRINT_BUF_LEN];
u8 gBuf[GBUF_LEN];
u8 USART_RxBuf[USART_FIFO_LEN];
#else 
u8 *gBuf;
#endif
u8 *pUSART_RxBuf;
UserInfo *pUserList, *pAdminList;
BookInfo *pBookList;
OfflineInfo *pOfflineList;
ErrorInfo *pErrorList;
ModuleInfo *pModuleList;
ErrorState *pErrorStateList;
//U32 *pRandonIDList;
//U16 *pModuleUserIDList;
///////////////////////////////////////////////////////////////////////////////
void allocExtSram(void) {//外部SRM操作
	gSramOffset = 0;
#if (USE_EXT_SRAM == 0)
	pUSART_RxBuf = USART_RxBuf;

	pUSART1_FIFO = &USART1_FIFO;
	//pUSART2_FIFO = &USART2_FIFO;
	pUSART3_FIFO = &USART3_FIFO;
	
	pUSART1_Print_Buf = USART1_Print_Buf;
	//pUSART2_Print_Buf = USART2_Print_Buf;
	pUSART3_Print_Buf = USART3_Print_Buf;
#else	
	pUSART_RxBuf = (u8*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += USART_FIFO_LEN;
	// set the USART recv FIFO
	
	pUSART1_FIFO = (USART_FIFO_TYPE*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += sizeof(USART_FIFO_TYPE);
	pUSART2_FIFO = (USART_FIFO_TYPE*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += sizeof(USART_FIFO_TYPE);
	pUSART3_FIFO = (USART_FIFO_TYPE*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += sizeof(USART_FIFO_TYPE);

	// Set USART Printf BUF
	pUSART1_Print_Buf = (u8*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += USART_PRINT_BUF_LEN;
	pUSART2_Print_Buf = (u8*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += USART_PRINT_BUF_LEN;
	pUSART3_Print_Buf = (u8*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += USART_PRINT_BUF_LEN;
	
	gBuf = (u8*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += GBUF_LEN;
#endif
	
	pAdminList = (UserInfo*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += ADMIN_MAX_LEN*sizeof(UserInfo);
	
	pUserList = (UserInfo*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += USER_MAX_LEN*sizeof(UserInfo);
	
	pBookList = (BookInfo*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += BOOK_MAX_LEN*sizeof(BookInfo);
	
	pOfflineList = (OfflineInfo*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += OFFLINE_MAX_LEN*sizeof(OfflineInfo);
	
	pErrorList = (ErrorInfo*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += ERROR_MAX_LEN*sizeof(ErrorInfo);
	
	pModuleList = (ModuleInfo*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += MODULE_MAX_LEN*sizeof(ModuleInfo);
	
	pErrorStateList = (ErrorState*)(Bank1_SRAM3_ADDR+gSramOffset);
	gSramOffset += ERRORSTATE_MAX_LEN*sizeof(ErrorState);
	
//	pRandonIDList = (U32*)(Bank1_SRAM3_ADDR+gSramOffset);
//	gSramOffset += RANDONID_MAX_LEN*sizeof(U32);
//	
//	pModuleUserIDList = (U16*)(Bank1_SRAM3_ADDR+gSramOffset);
//	gSramOffset += MODULEUSERID_MAX_LEN*sizeof(U16);
	
	memset((u8*)Bank1_SRAM3_ADDR, 0, SRAM_LEN);
}
///////////////////////////////////////////////////////////////////////////////
// 处理计算机串口发送过来的命令
u8 PCCmdHandler(u8 *pBuf) {//PC命令处理
    u32 n=0,id=0,p1=0,p2=0,p3=0,p4=0,p9=0;
    u8 SupperCardID[4];
	u32 SNhanderbuf=0;
	FLASH_Status status;
	u8 i=0;
    if(pBuf[0]=='$') {
		n = sscanf((char *)pBuf,"$PC,%d,%d,%u,%d,%d",&id,&p1,&p2,&p3,&p4);
        if(n>=1) {
    		switch(id) {
            case PC_CMD_HEART_BEAT:	
				//USART1_Printf("$PC,%d,%d,%d\r\n",id,p1,p2);
				// 报告PC心跳
			  	if (TCP_Connected()) {
					USART3_Printf("$MCU,2,%d,%d\r\n",p1,p2);
				}
				break;       
     
			case PC_CMD_ADMIN_CARD:
				// 设置超级权限管理卡
				if (n>1) {
					dev.AdminLen = saveUserList(pBuf, p1,1);
//					saveDevInfo();
				}
				break;


			case PC_CMD_SET_TCP:
				// 设置TCP端口地址
				// $PC,7,8800,192.168.0.111
				USART1_Printf("Set TCP...\r\n");
				break;
			case PC_CMD_CHANGE_ID:	
				// 设置模块ID
//				dev.SN = p1;
                USART1_Printf("%d\r\n",p1);
//                memcpy(SNbuf,&p1,4);
				dev.ModuleInfoLen=0;
				WriteFlash(MODULE_PAGE_NO, (u8*)pModuleList, 0);
			    for(i=0;i<=6;i++){
					SNhanderbuf=p1-i;
					USART1_Printf("%d\r\n",SNhanderbuf);
					memcpy(SNbuf,&SNhanderbuf,4);
					SNHandler(SNbuf);
				}
                USART1_Printf("%d,%d,%d,%d,%d,%d,%d\r\n",moduleSN.Module_1_SN,moduleSN.Module_2_SN,moduleSN.Module_3_SN,moduleSN.Module_4_SN,moduleSN.Module_5_SN,moduleSN.Module_6_SN,dev.SN);
				break; 
			case PC_CMD_DUMP:
				// 通过串口1输出内部状态信息
				Dump();
				break;
	
			case PC_CMD_CL_OFFDATA:
				dev.OfflineLen = 0;
				gOfflinePos = 0;
				WriteFlash(OFFLINE_PAGE_NO, (u8*)pOfflineList, 0);
				break;
			case PC_CMD_QUERY:
				USART1_Printf("$MCU,2,%d\r\n", dev.SN);
				break;

			case PC_CMD_GET_IP:
				//USART1_Printf("IP:%s\r\n", gIP);
				//USART1_Printf("MAC:%s\r\n", gMAC);
				USART1_Printf("no IP & MAC.\r\n");
				break;
//			case PC_CMD_RF_ORDER:
//				dev.RFOrder = p1;
//				saveDevInfo();
//				break;
			case PC_CMD_CL_BOOK:
				dev.BookLen = 0;
				WriteFlash(BOOK_PAGE_NO, (u8*)pBookList, 0);
				break;
			case PC_CMD_CL_MODULEINFO:
				dev.ModuleInfoLen=0;
				WriteFlash(MODULE_PAGE_NO, (u8*)pModuleList, 0);
				break;
			case PC_CMD_CL_ALL:
				dev.BookLen = 0;
				dev.AdminLen = 0;
				dev.UserLen = 0;
				dev.OfflineLen = 0;
				WriteFlash(ADMIN_PAGE_NO, (u8*)pAdminList, 0);
				WriteFlash(USER_PAGE_NO, (u8*)pUserList, 0);
				WriteFlash(OFFLINE_PAGE_NO, (u8*)pOfflineList, 0);
				WriteFlash(BOOK_PAGE_NO, (u8*)pBookList, 0);
				USART1_Printf("clear record ok\r\n");
				break;
			 case PC_CMD_ACQ_rough_weight:
						Can_Send_Msg(0x10,0xEF,0xEF);
						USART1_Printf("hsjhdj");
						break;
			 case PC_CMD_ACQ_WEIGHT:
						Can_Send_Msg(0x10,0xEF,0xEE);                 
						break;
			 case PC_CMD_SET_SUPPERCARD:
						sscanf((char *)pBuf,"$PC,%d,%X,",&id,&p9);
						USART1_Printf("t89:%X\r\n",p9);
	//                       int2char(p9,SupperCardID);
						memcpy(SupperCardID,&p9,4);
						Can_Send_DevInfo(0x10,0x02,SupperCardID);
						break;   
			 case PC_CMD_IAP_SW:
				FlashEraseLastPage();
				status = FLASH_WaitForLastOperation(ProgramTimeout);
				if(status == FLASH_COMPLETE)
				{
					FLASH->CR |= CR_PG_Set;
					*(__IO uint16_t*)BOOTSW =1;
					status = FLASH_WaitForLastOperation(ProgramTimeout);
					FLASH->CR &= CR_PG_Reset;
				}
				RunMainProgram(IAPAddress);		
				break;
			case PC_CMD_CAN_SW:
				FlashEraseLastPage();
				status = FLASH_WaitForLastOperation(ProgramTimeout);
				if(status == FLASH_COMPLETE)
				{
					FLASH->CR |= CR_PG_Set;
					*(__IO uint16_t*)CANSW =1;
					status = FLASH_WaitForLastOperation(ProgramTimeout);
					FLASH->CR &= CR_PG_Reset;
				}
				RunMainProgram(IAPAddress);		
				break;
				default:
					break;
            } 
        }
		return 1;
    } 
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void SvrCmdHandler(u8 *pBuf) {//服务器命令处理
    u32 n=0,id=0,p1=0,p2=0,p3=0,p4=0,p9=0;
    char *p;
//	u8 Status;
    u8 Buf=0;
    u8 SupperCardID[4];

	u8 onlineNO=0;

//    u32 SN_Online=0;
    if(pBuf[0]=='$') {
        n = sscanf((char *)pBuf,"$PC,%d,%u,%d,%d",&id,&p1,&p2,&p3);
    } else if ((pBuf[0]=='P') && (pBuf[1]=='C')) {
		n = sscanf((char *)pBuf,"PC,%d,%u,%d,%d",&id,&p1,&p2,&p3);
	} else if((p=strstr((char*)pBuf, "$PC"))!=0) {
		n = sscanf((char *)p,"$PC,%d,%u,%d,%d",&id,&p1,&p2,&p3);
	}
	
	if(n>0){
		switch(id) {
		case SVR_CMD_AUTH:	
//		case SVR_CMD_AUTH_PC:	
			// USART2_Printf("$PC,%d,%d,%d\r\n",id,p1,p2);
			// 服务器返回用户认证信息
//			Status = p1;
//			sscanf((char *)pBuf,"$PC,%d,%d,%d,%d:%X",&id,&p1,&p2,&p3,&p4);
			if(p1==0) {
				// 验证通过
				// 设置随机ID为当前时间的秒数
//				gRandomId=RTC_GET_TIME();//得到计数器中的值(秒钟数)	
//				gRemainTime = p3 * 60;
				gUseTime = 0;
				gRFCardUse =Card_buf;
				gUserId = p2;
				onlineNO=onlinehander(pBuf);
				USART1_Printf("SVR:%d\r\n", onlineNO);
				if(onlineNO>0&&onlineNO<7){					
						Door_close_Hander(moduleNO_buf);					
						Door_open_Hander((U32)onlineNO);
						openstate=1;
						moduleNO_buf=(u32)onlineNO;																								              
			    }else{
                    Can_Send_Msg(RxMessage.ExtId,0x08,0x02);
                }
			}
//			if(id==1)
//			{
//			  // 如果是PC用户认证信息
//			  USART1_Printf("$MCU,0,%d,%d,%d\r\n",dev.SN,p1,p2);
//			}
			break;        
		case SVR_CMD_TIME:	
			// 服务器同步时间
			USART1_Printf("SVR_CMD_TIME:%X\r\n", p1);
			RTC_SET_TIME(p1);
            
			USART1_Printf("SET RTC TIME OK!\r\n");
			break;   
		case SVR_CMD_CARD_ID:
			// 用户卡编号（1:F6D3A5E4|2:F7D3A5E4）
			dev.UserLen = saveUserList(pBuf, p1, 0);
//			saveDevInfo();
			break;
		case SVR_CMD_BOOK_INFO:
			// 预约纪录（1:F6D3A5E4-F6D3A5E4|）
			dev.BookLen = saveBookList(pBuf, p1);
//			saveDevInfo();
			break;
		case SVR_CMD_SET_TYPE:
			// 设置工作模式（1：需要预约；0：不需要预约）
            dev.WorkType = p1;
			WriteFlashDevInfo();
			break;
		case SVR_CMD_SYNC_ACK:
			if (dev.OfflineLen>0) {
				dev.OfflineLen = gOfflinePos;
			    WriteFlash(OFFLINE_PAGE_NO, (u8*)pOfflineList, 
				             sizeof(OfflineInfo)*dev.OfflineLen);
				gOfflineDly = 0;
			} else {
				dev.OfflineLen = 0;
				gOfflinePos = 0;
				gOfflineDly = 0;
//				truncateFile(OFFLINE_FILE_NAME);
//				saveDevInfo();
				WriteFlash(OFFLINE_PAGE_NO, (u8*)pOfflineList, 0);
			}
			break;
		case SVR_CMD_ADMIN_CARD:
			// 设置超级权限管理卡
			if (n>1) {
				dev.AdminLen = saveUserList(pBuf, p1,1);
//				saveDevInfo();
			}
			break;
//		case SVR_CMD_RFID_ORDER:
//			dev.RFOrder = p1;
//			saveDevInfo();
//			break;
        case SVR_CMD_ERROR_ACK:
//			USART1_Printf("5623");
            if (dev.ErrorInfoLen>0) {
				USART1_Printf("258\r\n");
				dev.ErrorInfoLen=gOfflinePosErr;
				WriteFlash(ERROR_PAGE_NO, (u8*)pErrorList, 
							sizeof(ErrorInfo)*dev.ErrorInfoLen);
				gOfflineDlyErr = 0;
			} else {
				USART1_Printf("693\r\n");
				dev.ErrorInfoLen = 0;
				gOfflinePosErr = 0;
				gOfflineDlyErr = 0;
//				truncateFile(ERROR_FILE_NAME);
//				saveDevInfo();
				WriteFlash(ERROR_PAGE_NO, (u8*)pErrorList, 0);
			}
			break;
        case SVR_CMD_SUPPERCARD:
            sscanf((char *)pBuf,"$PC,%d,%X,",&id,&p9);
            USART1_Printf("t89:%\r\n",p9);
//             int2char(p9,SupperCardID);
            memcpy(SupperCardID,&p9,4);
            Can_Send_DevInfo(0x10,0x02,SupperCardID);
            break;
		default:
			break;
		} 
	}  
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void OnSecondTimer(void) {
	static u8 sec_cnt = 0;
    u32  checktime=0;
//    u8 ceshi=8;
    u8 time_buf[4];
    u32 Ctimebuf=0;
    u8 statebuf=0;
	showTime();
    
//	UseTimeHandler();
	if(closestate==1)
    {
        closecnt++;
    }
    if(closestate_many==1)
    {
        closecount++;
    }
    if(openstate==1)
    {
        opencnt++;
    }
	if (++sec_cnt==10) {
		sec_cnt = 0;
//每个模块需要一个心跳包
		// 往服务器发心跳包
		if (TCP_Connected()) {
//            memcpy(&statebuf,&StateByte,1);
			HeartbeatSend();
		}
	}
	
	if (TCP_Connected()) {
		gNetDownCnt = 0;
	} else {
		gNetDownCnt ++;
		if (gNetDownCnt>=60) {
			gNetRstCnt = NET_RST_CNT;
			gNetDownCnt = 0;
			
		}
	}
//    Error_borrow(RxMessage.ExtId);
	OfflineHandler();
    Error_OffineHandler();
	if(check_time==1)
    {
        
        checktime=labs(timebuf-RTC_GET_TIME_UTC());
        if(checktime>5)
          {   
              Ctimebuf=RTC_GET_TIME_UTC();
//              int2char(RTC_GET_TIME_UTC(),time_buf);
              memcpy(time_buf,&Ctimebuf,4);
              Can_Send_DevInfo(0x10,0x01,time_buf);
          }
       check_time=0;
    }
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
void ModuleStateByte_init(void)
{
    StateByte.a=0;//第一个模块的状态
    StateByte.b=0;//第二个模块的状态
    StateByte.c=0;
    StateByte.d=0;
    StateByte.e=0;
    StateByte.f=0;//第六个模块的状态
    StateByte.g=0;
    StateByte.h=0;//f是第八位
}
///////////////////////////////////////////////////////////////////////////////
void InitState(void) {//初始化
//	readDevInfo();
//    readModuleSN();
	findModule_RandonID();
//	init_clear();
//    ModuleStateByte_init();
	gUseTime = 0;
	gRemainTime = 0;
	gRandomId = 0;


	gRFCardIn = 0;
	gRFCardUse = 0;
	Card_IN=0;		// 刷卡状态，=1：有卡刷进来
	gUserId = 0;		// 用户ID

	gBuzzerCnt = 0;		// 蜂鸣器响声时间计数器
	gOfflineCnt = 0;	// 离线计时器
	gOnlineCnt = 0;
    gOnlineCntErr=0;
	gOfflinePos = 0;	//
    gOfflinePosErr = 0;
	gOfflineDly = 0;
    gOfflineDlyErr = 0;
	gNetRstCnt = 0;	// 有线网口复位状态延时计数器
	gNetDownCnt	= 0;
	
	gUsart1TOCnt = 0;
	gUsart2TOCnt = 0;
	gUsart3TOCnt = 0;
	
    

}
///////////////////////////////////////////////////////////////////////////////
u16 saveUserList(u8 *buf, u8 firstSection, u8 isAdmin) {//保存用户信息
	u8 state=0, pos=0;
	UserInfo user, *pList;
	u8 *p;
	u16 cnt = 0, Max;
	
	p = buf;
	memset(gBuf, 0, 32);
	
	if (isAdmin == 1) {
		cnt = dev.AdminLen;
		Max = ADMIN_MAX_LEN;
		pList = pAdminList;
	} else { 
		cnt = dev.UserLen;
		Max = USER_MAX_LEN;
		pList = pUserList;
	}

	if ((firstSection&DATA_FIRST_SECTION)==DATA_FIRST_SECTION) {
		cnt = 0;
	}

	// $PC,3,1,1:F5615534|2:45155534
	while (*p) {
		if (state<3) {
			if (*p == ',')
				state ++;
		} else {
			if ((*p != '|') && (*p != '\r') && (*p != '\n') && (pos<16)) {
				gBuf[pos++] = *p;
			} else {
				if (pos>0) {
					pos = 0;
					USART1_Printf("%s\r\n", gBuf);
					if ((sscanf((char*)gBuf, "%d:%X", &(user.id), &(user.card))==2)) {
						if (cnt<Max) {
							pList[cnt].id = user.id;
							pList[cnt].card = user.card;
							(cnt)++;
							//fwrite(&user, 1, sizeof(UserInfo), file);
						} else {
							USART1_Printf("user lis is full.\r\n");
						}
					} else {
						USART1_Printf("err:%s\r\n", gBuf);
					}
					memset(gBuf, 0, 32);
				}
			}
		}
		p++;
	}
	
	
	//saveDevInfo();
	if ((firstSection&DATA_LAST_SECTION)==DATA_LAST_SECTION) {
		USART3_Printf("$MCU,5,%d,%d\r\n", dev.SN, cnt);
		USART1_Printf("write user list to flash.\r\n");
		if (isAdmin == 1) {
			WriteFlash(ADMIN_PAGE_NO, (u8*)pAdminList, cnt*sizeof(UserInfo));
		} else {
			WriteFlash(USER_PAGE_NO, (u8*)pUserList, cnt*sizeof(UserInfo));
		}
	}
	USART1_Printf("user data save ok!\r\n");
	return cnt;
}
////////////////////////////////////////////////////////////////////////////////
// 是管理员卡则返回UserID
u16 isAdminCard(u32 card) {//区别是否是管理员
	return findUserCard(card, pAdminList, dev.AdminLen);//
}
///////////////////////////////////////////////////////////////////////////////
u16 isUserCard(u32 card) {//判断是否是普通用户卡
	return findUserCard(card, pUserList, dev.UserLen);
}
///////////////////////////////////////////////////////////////////////////////
u16 findUserCard(u32 card, UserInfo *pList, u16 Len)//查找普通用户信息
{

	u16 i;
	
	for (i=0; i<Len; i++) {
		if (pList[i].card == card) {
			return pList[i].id;
		}
	}

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
u16 findBookInfo(u16 userId, u32 now,u32 cardbuf)//查找预约信息
{
	u16 i;
	u8 bookcount=0;
    u32 ctime;
	BookInfo book;
	for (i=0; i<dev.BookLen; i++) {
		if ((pBookList[i].id == userId) && 
			(pBookList[i].btime<=now) && 
			(pBookList[i].etime>now)) {
				memcpy(&book, &(pBookList[i]), sizeof(BookInfo));
//				USART1_Printf("moduleNO=%d",chooseSN(book.moduleNO));
				findModule_Info(chooseSN(book.moduleNO),&moduleinfo);
				USART1_Printf("%d\r\n",moduleinfo.Module_SN);                    
				if(moduleinfo.UserCard_ID==0&&ModuleIdbuf[book.moduleNO-1]==1)
					{
								bookcount++; 
								USART1_Printf("yuyue3\r\n");
								// 在预约的时间范围内，则打开
								SN_receive=chooseSN(book.moduleNO);
	//                            moduleNO_buf=SN_receive;                                
								gRemainTime = book.etime - ctime;
								gUseTime = 0;
								gRFCardUse = cardbuf;
								if(bookcount==1){
										Door_close_Hander(moduleNO_buf);
										Door_open_Hander(book.moduleNO);
										openstate=1;
										moduleNO_buf=book.moduleNO;
										USART1_Printf("open:%d\r\n",moduleNO_buf);
	//                                SN_receive_buf=SN_receive;
								}
								memcpy(&gRFCardUse_buf,&gRFCardUse,4);
								memcpy(&gRemainTime_buf,&gRemainTime,4);
								Can_Send_Msg(book.moduleNO,0x08,0x01);
								Can_Send_DevInfo(book.moduleNO,0x07,gRFCardUse_buf);
								Can_Send_DevInfo(book.moduleNO,0x05,gRemainTime_buf);
				 }

			}
	 }
	return bookcount;
}
///////////////////////////////////////////////////////////////////////////////
u16 saveBookList(u8 *buf, u8 firstSection)//保存用户列表 
{
	u8 state=0, pos=0;
	BookInfo book;
	u8 *p;
	u16 cnt=0;
	
	p = buf;
	memset(gBuf, 0, 64);
	
	if ((firstSection&DATA_FIRST_SECTION)==DATA_FIRST_SECTION) {
		dev.BookLen = 0;
		cnt = 0;
	} else {
		cnt = dev.BookLen;
	}

	// $PC,4,1,1:F6D3A5E4-F6D3A5E4|
	while (*p) {
		if (state<3) {
			if (*p == ',')
				state ++;
		} else {
			if ((*p != '|') && (*p != '\r') && (*p != '\n') && (pos<26)) {
				gBuf[pos++] = *p;
			} else {
				if (pos>0) {
					pos = 0;
					USART1_Printf("%s\r\n", gBuf);
					if ((sscanf((char*)gBuf, "%d:%X-%X:%d", &(book.id), &(book.btime), &(book.etime),&(book.moduleNO))==4)) {
						if (cnt<BOOK_MAX_LEN) {
							pBookList[cnt].id = book.id;
							pBookList[cnt].btime = book.btime;
							pBookList[cnt].etime = book.etime;
							pBookList[cnt].moduleNO = book.moduleNO;
							(cnt)++;
							dev.BookLen = cnt;
						} else {
							USART1_Printf("book list is full.\r\n");
						}
						
					} else {
						USART1_Printf("err:%s\r\n", gBuf);
					}
					memset(gBuf, 0, 64);
				}
			}
		}
		p++;
	}
	
	//saveDevInfo();
	if ((firstSection&DATA_LAST_SECTION)==DATA_LAST_SECTION) {
		USART3_Printf("$MCU,6,%d,%d\r\n", dev.SN, cnt);
		WriteFlash(BOOK_PAGE_NO, (u8*)pBookList, sizeof(BookInfo)*cnt);
	}
	USART1_Printf("book data save ok!\r\n");
	return cnt;
}
// ////////////////////////////////////////////////////////////////////////////////
u32 findModule_Info(u32 SN,ModuleInfo*info)//查找模块信息
{
   
	u16 i;
	
	for (i=0; i<dev.ModuleInfoLen; i++) {
		if (pModuleList[i].Module_SN == SN) {
			memcpy(info, &pModuleList[i],sizeof(ModuleInfo));
			return pModuleList[i].id;
		}
	}

	return 0;

}
////////////////////////////////////////////////////////////////////////////////
u32 findErrorState(u32 SN,ErrorState*info)//查找错误信息的状态
{
   
	u16 i;
	
	for (i=0; i<ErrorStatelen; i++) {
		if (pErrorStateList[i].Module_SN == SN) {
			memcpy(info, &pErrorStateList[i],sizeof(ErrorState));
			return pErrorStateList[i].UserdId;
		}
	}

	return 0;

}
////////////////////////////////////////////////////////////////////////////////
u8 findReturn_Info(u32 card)//查找需要归还的信息
{	
    u16 i;
	u8 return_cnt=0;

    u8 SN_return_buf=0;
	
	for (i=0; i<dev.ModuleInfoLen; i++) {
		if (pModuleList[i].UserCard_ID == card) {
			    SN_return_buf=pModuleList[i].Module_SN%10;
USART1_Printf("sn=%d\r\n",SN_return_buf);			
				Can_Send_Msg(SN_return_buf,0x08,0x03);
			    return_cnt++;
		}
	}

	return return_cnt;
}
////////////////////////////////////////////////////////////////////////////////
u8 findAdminReturn_Info(u32 SN)//查找管理员的归还信息
{
	u16 i;
	u8 SN_return_buf=0;
	for (i=0; i<dev.ModuleInfoLen; i++) {
		if (pModuleList[i].Module_SN == SN) {
				if(pModuleList[i].UserCard_ID!=0&&pModuleList[i].UserCard_ID!=-1){
						SN_return_buf=pModuleList[i].Module_SN%10;  
						Can_Send_Msg(SN_return_buf,0x08,0x03);
						return IS_OK;
		}
	 }
   }
	return IS_FAIL;

}
////////////////////////////////////////////////////////////////////////////////
u8 findfreemodule_Info(u32 SN,u8 number)//查找没有被借出模块的信息
{
	u16 i;
	u8 mudbuf=0;
	for (i=0; i<dev.ModuleInfoLen; i++) {
//		USART1_Printf("Module_SN=%d\r\n",pModuleList[i].Module_SN);
			if (pModuleList[i].Module_SN == SN) {
					mudbuf=pModuleList[i].Module_SN%10;
					USART1_Printf("%d,%d\r\n",mudbuf,ModuleIdbuf[mudbuf-1]);
					if(((pModuleList[i].UserCard_ID==0)||(pModuleList[i].UserCard_ID==-1))&&ModuleIdbuf[mudbuf-1]==1)
						{
//                            USART1_Printf("952\r\n");
//							USART1_Printf("gRFCardUse=%x\r\n",gRFCardUse);
							memcpy(gRFCardUse_buf,&gRFCardUse,4);
							memcpy(gRemainTime_buf,&gRemainTime,4);                        
							Can_Send_Msg((u32)number,0x08,0x01);
							Can_Send_DevInfo((u32)number,0x07,gRFCardUse_buf);
							Can_Send_DevInfo((u32)number,0x05,gRemainTime_buf);
							
							return mudbuf;
					}
			}
	}           
		    	
  return IS_FAIL;
}

////////////////////////////////////////////////////////////////////////////////
void RFCard_Main_Handler(void)//射频卡操作函数
{
	u8 	cnt=0;
    if(recevecard==1)
    {
        ErrorCard_Buf=Card_buf;
        if(borrow_state==4)
          {                   
            RFCardMainHander(Card_buf,RxMessage.ExtId);
            borrow_state=0;
            recevecard=0;              
          }
        if(repay_state==4)
          {              
            cnt=findReturn_Info(Card_buf);
			USART1_Printf("cnt=%d\r\n",cnt);  
			if((cnt!=0)&&(cnt<30)){
				Door_open_Hander(RxMessage.ExtId);
				openstate=1;
				moduleNO_buf=RxMessage.ExtId;				
			}			
            repay_state=0;
            recevecard=0;
          }
         
   }    
    BorrowHander(&moduleinfo,gRFCardUse,gUserId,1,RxMessage.ExtId);  
    ReturnHander(&moduleinfo,RxMessage.ExtId);
    SupperCardHander(RxMessage.ExtId);
    checkdoorstate();
}

/////////////////////////////////////////////////////////////////////////////////
void BorrowHander(ModuleInfo*info,u32 card_id,u16 user_id,u8 state,u32 moduleNObuf)//借出操作
{
     u32 module_SN=0;
//     module_SN=chooseSN(RxMessage.ExtId);
     if(borrow_state==1) 
      {   
		  module_zero(moduleNObuf);
		  gRandomId=RTC_GET_TIME_UTC();//得到计数器中的值(秒钟数)
		  RandonIDbuf[RxMessage.ExtId-1]=gRandomId;
		  ModuleUserIdbuf[RxMessage.ExtId-1]=user_id;
          module_SN=chooseSN(RxMessage.ExtId);
          moduleinfo_assignment(info,module_SN,card_id,user_id,state);
//          USART1_Printf("wi%X\r\n",info->Module_SN);          
          if (!TCP_Connected()) {			    
			// 网络连不通
                offlineInfo.Module_SN=module_SN;
                offlineInfo.Borrowtime = RTC_GET_TIME_UTC();
                offlineInfo.id = gUserId;
                offlineInfo.Returntime = 0;
                offlineInfo.rid = gRandomId;
                dev.OfflineLen = updateOfflineInfo(&offlineInfo);
			    WriteFlash(OFFLINE_PAGE_NO, (u8*)pOfflineList, 
				             sizeof(OfflineInfo)*dev.OfflineLen);

			}else{
				USART3_Printf("$MCU,9,%d,%d,%X,%X\r\n",dev.SN,RxMessage.ExtId,card_id,gRandomId);
				}
	      WriteFlash(RANDONID_PAGE_NO,(u8*)RandonIDbuf, 
				             sizeof(U32)*RANDONID_MAX_LEN);
		  WriteFlash(MODULEUSERID_PAGE_NO,(u8*)ModuleUserIdbuf, 
				             sizeof(U16)*MODULEUSERID_MAX_LEN);
          Door_close_Hander(moduleNObuf);
          USART1_Printf("close1:%d\r\n",moduleNObuf);
          borrow_state=0;
          closestate=1;
          closestate_many=1;
      }else if(borrow_state==3){               
          Door_close_Hander(moduleNObuf);//立即关门           
          borrow_state=0;
          USART1_Printf("close2:%d\r\n",moduleNObuf);
          closestate=1;
          closestate_many=1;
      }         
}

//////////////////////////////////////////////////////////////////////////////////
void ReturnHander(ModuleInfo*info,u32 return_ExId)//归还操作
{
    u32 SN_return_buf=0;
     
     if(repay_state==1)
       {
          USART1_Printf("852\r\n");
		  if(ErrorFlag==1){
			  ErrorFlag=0;
			  RandonIDbuf[return_ExId-1]=0;
			  ModuleUserIdbuf[return_ExId-1]=0;
			  SN_return_buf=chooseSN(return_ExId);			  
              findErrorState(SN_return_buf,&errorstate);
			  
//			  errorstate.Borrowtime=0;
//			  errorstate.UserdCardId=0;
//			  errorstate.UserdId=0;
//			  ErrorStatelen=updateErrorState(&errorstate);
//			  WriteFlash(ERRORSTATE_PAGE_NO, (u8*)pErrorStateList, 
//						sizeof(ErrorState)*ErrorStatelen);
			  if (!TCP_Connected()) {              
					offlineInfo.Module_SN=SN_return_buf;
		//			offlineInfo.Borrowtime = RTC_GET_TIME_UTC();
					offlineInfo.id = errorstate.UserdId;
					offlineInfo.Returntime = RTC_GET_TIME_UTC();
					offlineInfo.rid = errorstate.Borrowtime;//还需要再斟酌
					dev.OfflineLen = updateOfflineInfo(&offlineInfo);
					WriteFlash(OFFLINE_PAGE_NO, (u8*)pOfflineList, 
								 sizeof(OfflineInfo)*dev.OfflineLen);			  
//                saveDevInfo();
			  }else{
					USART3_Printf("$MCU,10,%d,%d,%X,%X\r\n",dev.SN,return_ExId,errorstate.UserdCardId,errorstate.Borrowtime);
			   }
			 errorstate.Borrowtime=0;
			 errorstate.UserdCardId=0;
			 errorstate.UserdId=0;
			 ErrorStatelen=updateErrorState(&errorstate);
			 WriteFlash(ERRORSTATE_PAGE_NO, (u8*)pErrorStateList, 
						sizeof(ErrorState)*ErrorStatelen);
			 USART1_Printf("945%d\r\n",moduleNO_buf);
			 Door_close_Hander(moduleNO_buf);//moduleNO_return_buf可能为零
			 repay_state=0;
			 closestate=1;
			 closestate_many=1;
		  }else{
			  SN_return_buf=chooseSN(return_ExId);
			  findModule_Info(SN_return_buf,info);
			  if (!TCP_Connected()) {              
					offlineInfo.Module_SN=SN_return_buf;
		//			offlineInfo.Borrowtime = RTC_GET_TIME_UTC();
					offlineInfo.id = info->id;
					offlineInfo.Returntime = RTC_GET_TIME_UTC();
					offlineInfo.rid = info->RandonID;
					dev.OfflineLen = updateOfflineInfo(&offlineInfo);
					WriteFlash(OFFLINE_PAGE_NO, (u8*)pOfflineList, 
								 sizeof(OfflineInfo)*dev.OfflineLen);			  
	//                saveDevInfo();
			 }else{
					USART3_Printf("$MCU,10,%d,%d,%X,%X\r\n",dev.SN,return_ExId,info->UserCard_ID,info->RandonID);
			  }            
			 moduleinfo_init(info,SN_return_buf);
			 gRandomId=0;
//			 RandonIDbuf[RxMessage.ExtId-1]=gRandomId;
			 RandonIDbuf[return_ExId-1]=0;
			 ModuleUserIdbuf[return_ExId-1]=0;
	//         USART1_Printf("10\r\n");
			 WriteFlash(RANDONID_PAGE_NO,(u8*)RandonIDbuf, 
				             sizeof(U32)*RANDONID_MAX_LEN);
			 WriteFlash(MODULEUSERID_PAGE_NO,(u8*)ModuleUserIdbuf, 
				             sizeof(U16)*MODULEUSERID_MAX_LEN);
			 USART1_Printf("963%d\r\n",moduleNO_buf);
			 Door_close_Hander(moduleNO_buf);//moduleNO_return_buf可能为零
			 repay_state=0;
			 closestate=1;
			 closestate_many=1;
		}
      }else if(repay_state==3){
         USART1_Printf("close3\r\n");
         Door_close_Hander(moduleNO_buf);
         repay_state=0;
         closestate=1;
         closestate_many=1;
	  }
//      }else if(repay_state==5){
//		 ErrorFlag=0;
//		 Door_close_Hander(moduleNO_buf);
//		 repay_state=0;
//         closestate=1;
//         closestate_many=1; 
//	  }                 
}
////////////////////////////////////////////////////////////////////////////////
void BuzzerHandler(void) {//蜂鸣器操作
	if (gBuzzerCnt>0) {
		gBuzzerCnt--;
		BUZZER_ON();
	} else {
		BUZZER_OFF();
	}
}
///////////////////////////////////////////////////////////////////////////////
void NetResetHandler(void) {//网络操作
	if (gNetRstCnt>0) {
		gNetRstCnt--;
		EthNetReset();
	} else {
		EthNetRun();
	}
}
///////////////////////////////////////////////////////////////////////////////
#define TCP_STABLE_CNT	10
void OfflineHandler(void) {//离线操作
//	static OfflineInfo offlineInfo;
	u8 offline_moduleSN=0;

	
	if (TCP_Connected()) {
		gOnlineCnt++;
		// 网络连通		
		if (gOfflineDly == 0 && gOnlineCnt>=TCP_STABLE_CNT) {
			if (dev.OfflineLen>0) {
				// 有离线数据
				USART1_Printf("SYNC Offline Data\r\n");
				gOfflinePos = dev.OfflineLen;
				memset(pUSART_RxBuf, 0, USART_FIFO_LEN);
				memset(gBuf, 0, GBUF_LEN);
//                int2char(offlineInfo.Module_SN,offline_moduleSN);
                offline_moduleSN=pOfflineList[gOfflinePos-1].Module_SN%10;
				sprintf((char*)pUSART_RxBuf, "$MCU,14,%d,%d:%X-%X-%X:%d|", 
						dev.SN, pOfflineList[gOfflinePos-1].id, pOfflineList[gOfflinePos-1].Borrowtime, 
						pOfflineList[gOfflinePos-1].Returntime, pOfflineList[gOfflinePos-1].rid,offline_moduleSN);
								
				for (gOfflinePos=gOfflinePos-1; gOfflinePos>0; gOfflinePos--) {
					offline_moduleSN=pOfflineList[gOfflinePos-1].Module_SN%10;
					sprintf((char*)gBuf, "%d:%X-%d-%X:%d|", 
						pOfflineList[gOfflinePos-1].id, pOfflineList[gOfflinePos-1].Borrowtime, 
						pOfflineList[gOfflinePos-1].Returntime, pOfflineList[gOfflinePos-1].rid,offline_moduleSN);
					if ((strlen((char*)gBuf) + strlen((char*)pUSART_RxBuf))<250) {
						strcat((char*)pUSART_RxBuf, (char*)gBuf);
						memset(gBuf, 0, GBUF_LEN);
					} else {
						strcat((char*)pUSART_RxBuf,"\r\n");
						break;
					}
				}				
				strcat((char*)pUSART_RxBuf,"\r\n");
				USART3_Printf("%s", pUSART_RxBuf);
				gOfflineDly = 10;
			}	//if (dev.OfflineLen>0) {
		}
		if (gOfflineDly>0){
			gOfflineDly--;
		 } 		
	}
}
////////////////////////////////////////////////////////////////////////////////
void Dump(void) {
//	FILE *file;
//	UserInfo *user;
//	BookInfo *book;
//	OfflineInfo *offline;
//    ModuleInfo *moduleinfo;
//    ErrorInfo *errorinfo;
//	u16 len;
	u16 i=0;
	
	USART1_Printf("SN:%d\r\n", dev.SN);
	USART1_Printf("Device Type:%d\r\n", dev.WorkType);
	USART1_Printf("User Len:%d\r\n", dev.UserLen);
	USART1_Printf("Admin Len:%d\r\n", dev.AdminLen);
	USART1_Printf("Book Len:%d\r\n", dev.BookLen);
	USART1_Printf("Offline Len:%d\r\n", dev.OfflineLen);
//	USART1_Printf("RFOrder:%d\r\n", dev.RFOrder);
	USART1_Printf("gRemainTime:%d\r\n", gRemainTime);
	USART1_Printf("gUserId:%d\r\n", gUserId);
	USART1_Printf("gRFCardIn:%X\r\n", gRFCardIn);
	USART1_Printf("gRFCardUse:%X\r\n", gRFCardUse);
	USART1_Printf("gOfflineCnt:%d\r\n", gOfflineCnt);
	USART1_Printf("gOfflinePos:%d\r\n", gOfflinePos);
	USART1_Printf("TCP:%d\r\n", TCP_Connected());
// 	USART1_Printf("gUsart1TOCnt=%d\r\n", gUsart1TOCnt);
// 	USART1_Printf("gUsart2TOCnt=%d\r\n", gUsart2TOCnt);
// 	USART1_Printf("gUsart3TOCnt=%d\r\n", gUsart3TOCnt);

	USART1_Printf("pAdminList=0x%X\r\n", pAdminList);
	USART1_Printf("pUserList=0x%X\r\n", pUserList);
	USART1_Printf("pBookList=0x%X\r\n", pBookList);
	USART1_Printf("pOfflineList=0x%X\r\n", pOfflineList);
	
	USART1_Printf("sizeof(UserInfo)=%d\r\n", sizeof(UserInfo));
	USART1_Printf("sizeof(BookInfo)=%d\r\n", sizeof(BookInfo));
	USART1_Printf("sizeof(OfflineInfo)=%d\r\n", sizeof(OfflineInfo));
	
	USART1_Printf("Admin card:\r\n");
	for (i=0; i<dev.AdminLen; i++) {
		USART1_Printf("%d:%X\r\n", pAdminList[i].id, pAdminList[i].card);
	}
	
	USART1_Printf("User List:\r\n");
	for (i=0; i<dev.UserLen; i++) {
		USART1_Printf("%d:%X\r\n", pUserList[i].id, pUserList[i].card);
	}
	
	USART1_Printf("Book List:\r\n");
	for (i=0; i<dev.BookLen; i++) {
		USART1_Printf("%d:%X-%X:%d\r\n", pBookList[i].id, pBookList[i].btime, pBookList[i].etime,pBookList[i].moduleNO);
	}
	
	USART1_Printf("Offline List:\r\n");
	for (i=0; i<dev.OfflineLen; i++) {
		USART1_Printf("%d:%X:%X\r\n", pOfflineList[i].id, pOfflineList[i].Borrowtime, pOfflineList[i].Returntime);
	}
	
	USART1_Printf("ModuleInfo List:\r\n");
	for (i=0; i<dev.ModuleInfoLen; i++) {
		USART1_Printf("%d:%X:%d:%X\r\n", pModuleList[i].Module_SN, pModuleList[i].UserCard_ID, pModuleList[i].id,pModuleList[i].RandonID);
	}
	
	USART1_Printf("ErrorInfo List:\r\n");
	for (i=0; i<dev.ErrorInfoLen; i++) {
		USART1_Printf("%d:%X:%X:%d\r\n", pErrorList[i].Module_SN, pErrorList[i].UserdCardId, pErrorList[i].Borrowtime,pErrorList[i].Errortpye);
	}
}
///////////////////////////////////////////////////////////////////////////////
void char2int(u8* buff,u8 *pOutBuf)
{

        pOutBuf[0] = buff[0];
		pOutBuf[1] = buff[1];
		pOutBuf[2] = buff[2];
		pOutBuf[3] = buff[3];
}
///////////////////////////////////////////////////////////////////////////////
void int2char(u32 cd,u8*buf)
{
      u8 j=0;
      
      for(j=0;j<4;j++)
      {
            buf[j]=*((u8*)&cd+j);
      }
}
///////////////////////////////////////////////////////////////////////////////
void SNHandler(u8*SNbuf)//SN号赋值与操作
{
    ModuleInfo data;
    u32 buf=0;
    u8 SN_cnt=0;
    memcpy(&buf,SNbuf,4);
    SN_cnt=buf%10;
    if(SN_cnt==0x01)
    {
      data.Module_SN=buf;
      data.UserCard_ID=0;
      data.id=0;
	  data.RandonID=0;
      moduleSN.Module_1_SN=buf;
	  WriteFlashDevInfo();			
      dev.ModuleInfoLen=updateModuleInfo(&data);     			
      Can_Send_DevInfo(0x01,0x04,SNbuf);
	  WriteFlash(MODULE_PAGE_NO, (u8*)pModuleList, 
				sizeof(ModuleInfo)*dev.ModuleInfoLen);
    }
    if(SN_cnt==0x02)
    {
      data.Module_SN=buf;
      data.UserCard_ID=0;
      data.id=0;
	  data.RandonID=0;
      moduleSN.Module_2_SN=buf;
//	  WriteFlashDevInfo();
      dev.ModuleInfoLen=updateModuleInfo(&data);       
      Can_Send_DevInfo(0x02,0x04,SNbuf);
//	  WriteFlash(MODULE_PAGE_NO, (u8*)pModuleList, 
//				sizeof(ModuleInfo)*dev.ModuleInfoLen);
    }
    if(SN_cnt==0x03)        
    {
      data.Module_SN=buf;
      data.UserCard_ID=0;
      data.id=0;
	  data.RandonID=0;
      moduleSN.Module_3_SN=buf;
//	  WriteFlashDevInfo();
      dev.ModuleInfoLen=updateModuleInfo(&data);     
      Can_Send_DevInfo(0x03,0x04,SNbuf);
//	  WriteFlash(MODULE_PAGE_NO, (u8*)pModuleList, 
//				sizeof(ModuleInfo)*dev.ModuleInfoLen);
    }        
    if(SN_cnt==0x04)
    {
      data.Module_SN=buf;
      data.UserCard_ID=0;
      data.id=0;
	  data.RandonID=0;
      moduleSN.Module_4_SN=buf;
//	  WriteFlashDevInfo();
      dev.ModuleInfoLen=updateModuleInfo(&data);      
      Can_Send_DevInfo(0x04,0x04,SNbuf);
//	  WriteFlash(MODULE_PAGE_NO, (u8*)pModuleList, 
//				sizeof(ModuleInfo)*dev.ModuleInfoLen);
    }        
    if(SN_cnt==0x05)
    {
      data.Module_SN=buf;
      data.UserCard_ID=0;
      data.id=0;
	  data.RandonID=0;
      moduleSN.Module_5_SN=buf;
//	  WriteFlashDevInfo();
      dev.ModuleInfoLen=updateModuleInfo(&data);    
      Can_Send_DevInfo(0x05,0x04,SNbuf);
//	  WriteFlash(MODULE_PAGE_NO, (u8*)pModuleList, 
//				sizeof(ModuleInfo)*dev.ModuleInfoLen);
    }
    if(SN_cnt==0x06)
    {
      data.Module_SN=buf;
      data.UserCard_ID=0;
      data.id=0;
	  data.RandonID=0;
      moduleSN.Module_6_SN=buf;
//      WriteFlashDevInfo();		
      dev.ModuleInfoLen=updateModuleInfo(&data);
      Can_Send_DevInfo(0x06,0x04,SNbuf);
//	  WriteFlash(MODULE_PAGE_NO, (u8*)pModuleList, 
//				sizeof(ModuleInfo)*dev.ModuleInfoLen);
    }
    if(SN_cnt==0x07)
    {
      dev.SN=buf;
//      WriteFlashDevInfo();
    }
}
////////////////////////////////////////////////////////////////////////////////
void Door_close_Hander(u32 SN)//关门操作
{  
//      u8 buf=0;
//      buf=SN%10;
//    USART1_Printf("%d\r\n",buf);
    switch (SN){
        case 0x01:
            DOOR_in_left();
            break;
        case 0x02:
            DOOR_in_left();
            break;
        case 0x03:
            DOOR_out_right();
            break;
        case 0x04:
            DOOR_in_left();
            break;
        case 0x05:
            DOOR_in_left();
            break;
        case 0x06:
            DOOR_out_right();
            break;
        default:
				break; 
    }
}
////////////////////////////////////////////////////////////////////////////////
void Door_open_Hander(u32 SN)//开门操作
{ 
//      u8 buf=0;
//      buf=SN%10;
    switch (SN){
        case 0x01:
            DOOR_in_right();
            break;
        case 0x02:
            DOOR_in_right();
            break;        
        case 0x03:
            DOOR_out_left();
            break;
        case 0x04:
            DOOR_in_right();
            break;
        case 0x05:
            DOOR_in_right();
            break;
        case 0x06:
            DOOR_out_left();
            break;
        default:
			break; 
    }
    
}
////////////////////////////////////////////////////////////////////////////////
void moduleinfo_init(ModuleInfo *moduleinfo,u32 Return_SN)//模块信息初始化
{
    moduleinfo->Module_SN=Return_SN;
    moduleinfo->UserCard_ID=0;
    moduleinfo->id=0;
    moduleinfo->Module_state=0;
	moduleinfo->RandonID=0;
	dev.ModuleInfoLen=updateModuleInfo(moduleinfo);
	WriteFlash(MODULE_PAGE_NO, (u8*)pModuleList, 
				sizeof(ModuleInfo)*dev.ModuleInfoLen);
}
/////////////////////////////////////////////////////////////////////////////////
u32 chooseSN(u32 StdId)//SN号选择
{
  u32 SNbuf;
  switch (StdId)
    {
    case 0x01:
        SNbuf=moduleSN.Module_1_SN;
        return SNbuf; 
    case 0x02:
        SNbuf=moduleSN.Module_2_SN;
        return SNbuf;
    case 0x03:
        SNbuf=moduleSN.Module_3_SN;
        return SNbuf;
    case 0x04:
        SNbuf=moduleSN.Module_4_SN;
        return SNbuf;
    case 0x05:
        SNbuf=moduleSN.Module_5_SN;
        return SNbuf;
    case 0x06:
        SNbuf=moduleSN.Module_6_SN;
        return SNbuf;
    default:
			  return 0;
  }      
}
/////////////////////////////////////////////////////////////////////////////////
void moduleinfo_assignment(ModuleInfo*info,u32 module_SN,u32 card_id,u16 user_id,u8 state)//模块信息赋值
{//赋值
     info->Module_SN=module_SN;
     info->UserCard_ID=card_id;
     info->id=user_id;
     info->Module_state=state;
	 info->RandonID=gRandomId;
	 dev.ModuleInfoLen=updateModuleInfo(info);
	 WriteFlash(MODULE_PAGE_NO, (u8*)pModuleList, 
				sizeof(ModuleInfo)*dev.ModuleInfoLen);               
}
////////////////////////////////////////////////////////////////////////////////////
void ModuleStateHandler(u32 numb)//模块状态操作
{
    if(Error_state==1)
    {
//		USART1_Printf("ujb\r\n");
		Error_state=0;
        switch (numb){
            case 0x01:
                errorbuf[0]=1;
				ModuleIdbuf[0]=0;
                break;
            case 0x02:
                errorbuf[1]=1;
				ModuleIdbuf[1]=0;
                break;        
            case 0x03:
                errorbuf[2]=1;
				ModuleIdbuf[2]=0;
                break;
            case 0x04:
                errorbuf[3]=1;
				ModuleIdbuf[3]=0;
                break;
            case 0x05:
                errorbuf[4]=1;
				ModuleIdbuf[4]=0;
                break;
            case 0x06:
                errorbuf[5]=1;
				ModuleIdbuf[5]=0;
                break;
            default:
                break;
        }
	}else if(ModuleState==1){
//	  USART1_Printf("hjh\r\n");
	   ModuleState=0;
       switch (numb){
            case 0x01:
                ModuleIdbuf[0]=1;
				errorbuf[0]=0;
                break;
            case 0x02:
                ModuleIdbuf[1]=1;
				errorbuf[1]=0;
                break;        
            case 0x03:
                ModuleIdbuf[2]=1;
				errorbuf[2]=0;
                break;
            case 0x04:
                ModuleIdbuf[3]=1;
				errorbuf[3]=0;
                break;
            case 0x05:
                ModuleIdbuf[4]=1;
				errorbuf[4]=0;
                break;
            case 0x06:
                ModuleIdbuf[5]=1;
				errorbuf[5]=0;
                break;
            default:
                break;   
		  }
//     return StateByte;
		}else if(ModuleState==2){
			ModuleState=0;
			module_zero(RxMessage.ExtId);
		}
			
}
///////////////////////////////////////////////////////////////////////////////////
void module_zero(u32 num)
    {
//		USART1_Printf("ujb\r\n");
		
        switch (num){
            case 0x01:
				ModuleIdbuf[0]=0;
                break;
            case 0x02:
				ModuleIdbuf[1]=0;
                break;        
            case 0x03:
				ModuleIdbuf[2]=0;
                break;
            case 0x04:
				ModuleIdbuf[3]=0;
                break;
            case 0x05:
				ModuleIdbuf[4]=0;
                break;
            case 0x06:
				ModuleIdbuf[5]=0;
                break;
            default:
                break;
        }
	}
///////////////////////////////////////////////////////////////////////////////////
void ErrorHandler(u32 numb)//对错拿与错还的操作
{    
    if(ModuleState==3)
    {
        ModuleState=0;
		module_zero(numb);
		ErrorRandomID=RTC_GET_TIME_UTC();
		RandonIDbuf[numb-1]=ErrorRandomID;
		ModuleUserIdbuf[numb-1]=UserIdbuf;
		errorstate.Module_SN=chooseSN(numb);
		errorstate.Borrowtime=ErrorRandomID;
		errorstate.UserdCardId=ErrorCard_Buf;
		errorstate.UserdId=UserIdbuf;
		ErrorStatelen=updateErrorState(&errorstate);
		WriteFlash(ERRORSTATE_PAGE_NO, (u8*)pErrorStateList, 
					sizeof(ErrorState)*ErrorStatelen);
		if(TCP_Connected())
		  {
			 USART3_Printf("$MCU,7,%d,%X,%X,%d\r\n",chooseSN(numb),ErrorCard_Buf,ErrorRandomID,BORROW_ERROR);       
		  }else{
			   errorinfo.Module_SN=chooseSN(numb);
			   errorinfo.UserdCardId=ErrorCard_Buf;
			   errorinfo.Borrowtime=ErrorRandomID;
			   errorinfo.Errortpye=BORROW_ERROR;
			   dev.ErrorInfoLen=updateErrorInfoFile(&errorinfo);
			   WriteFlash(ERROR_PAGE_NO, (u8*)pErrorList, 
							sizeof(ErrorInfo)*dev.ErrorInfoLen);
//                   saveDevInfo();
		  }
    }
    if(Differ_State==1)
    {
        Differ_State=0;
		module_zero(numb);
		ErrorRandomID=RTC_GET_TIME_UTC();
		RandonIDbuf[numb-1]=ErrorRandomID;
		ModuleUserIdbuf[numb-1]=UserIdbuf;
		errorstate.Module_SN=chooseSN(numb);
		errorstate.Borrowtime=ErrorRandomID;
		errorstate.UserdCardId=ErrorCard_Buf;
		errorstate.UserdId=UserIdbuf;
		ErrorStatelen=updateErrorState(&errorstate);
		WriteFlash(ERRORSTATE_PAGE_NO, (u8*)pErrorStateList, 
					sizeof(ErrorState)*ErrorStatelen);
        if(TCP_Connected())
          {
             USART3_Printf("$MCU,7,%d,%X,%X,%d\r\n",chooseSN(numb),ErrorCard_Buf,ErrorRandomID,RETURN_ERROR);       
          }else{
               errorinfo.Module_SN=chooseSN(numb);
               errorinfo.UserdCardId=ErrorCard_Buf;
               errorinfo.Borrowtime=RTC_GET_TIME_UTC();
               errorinfo.Errortpye=RETURN_ERROR;
               dev.ErrorInfoLen=updateErrorInfoFile(&errorinfo);
			   WriteFlash(ERROR_PAGE_NO, (u8*)pErrorList, 
							sizeof(ErrorInfo)*dev.ErrorInfoLen);
//               saveDevInfo();
           }
    }
}
//////////////////////////////////////////////////////////////////////////////////
void Error_OffineHandler(void) {//错误信息离线操作
//	static ErrorInfo errorinfo;
//	u8 offline_moduleSN[4];
//    ErrorInfo errorinfo;
//	FILE *file;
	
	if (TCP_Connected()) {
		gOnlineCntErr++;
		// 网络连通		
		if (gOfflineDlyErr == 0 && gOnlineCntErr>=TCP_STABLE_CNT) {
			if (dev.ErrorInfoLen>0) {
				// 有离线数据
//				USART1_Printf("error info data\r\n");
				gOfflinePosErr=dev.ErrorInfoLen;
				memset(pUSART_RxBuf, 0, USART_FIFO_LEN);
				memset(gBuf, 0, GBUF_LEN);
//                int2char(offlineInfo.Module_SN,offline_moduleSN);
				sprintf((char*)pUSART_RxBuf, "$MCU,8,%d:%X-%X:%d|", 
						 pErrorList[gOfflinePosErr-1].Module_SN, pErrorList[gOfflinePosErr-1].UserdCardId, pErrorList[gOfflinePosErr-1].Borrowtime, 
						pErrorList[gOfflinePosErr-1].Errortpye);

				
				for (gOfflinePosErr=gOfflinePosErr-1; gOfflinePosErr>0; gOfflinePosErr--) {
//					offline_moduleSN=pOfflineList[gOfflinePos-1].Module_SN%10;
					sprintf((char*)gBuf, "%d:%X-%X:%d|", 
                         pErrorList[gOfflinePosErr-1].Module_SN, pErrorList[gOfflinePosErr-1].UserdCardId, pErrorList[gOfflinePosErr-1].Borrowtime, 
						 pErrorList[gOfflinePosErr-1].Errortpye);					
							if ((strlen((char*)gBuf) + strlen((char*)pUSART_RxBuf))<250) {
								strcat((char*)pUSART_RxBuf, (char*)gBuf);
								memset(gBuf, 0, GBUF_LEN);
							} else {
								strcat((char*)pUSART_RxBuf,"\r\n");
								break;
							}
						}
				strcat((char*)pUSART_RxBuf,"\r\n");						
				USART3_Printf("%s", pUSART_RxBuf);
				gOfflineDlyErr = 10;
			}	//if (dev.OfflineLen>0) {
		}
		if (gOfflineDlyErr>0){
			gOfflineDlyErr--;
		 } 		
	}
}
//////////////////////////////////////////////////////////////////////////////////
void SupperCardHander(u32 nub)//超级卡操作
{
//    u32 SN_suppercard=0;
//    SN_suppercard=chooseSN(nub);
    if(SupperCardState==1)
    {
        Door_open_Hander(nub);//开门
        openstate=1;
        SupperCardState=0;
        USART1_Printf("open ok:%d\r\n",nub);
    }
    if(SupperCardState==2)
    {
        Door_close_Hander(nub);//关门 
        SupperCardState=0;		
		if(Suppercard_return==1){
			repay_state=1;
			Suppercard_return=0;
			ReturnHander(&moduleinfo,nub);
		}
		RandonIDbuf[nub-1]=0;
        USART1_Printf("close ok:%d\r\n",nub);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////

void RFCardMainHander(u32 cardbuf,u32 ExtId)//对各个模块中读取到的射频卡进行处理
{
//    BookInfo book;
	u32 ctime;
    u8 bookcnt=0;
    u8 i=0;
    u8 count=0;
    u8 number=0;
	u8 IDbuf=0;
	u32 ID_buf=0;
	u8 modbuf=0;
	u8 mod_buf=0;
//    u8 gRFCardUse_buf[4];
//    u8 gRemainTime_buf[4];
 //   u8 SN_return_buf[4];
	
	
	
      gUserId = isUserCard(cardbuf);
	  gRFCardIn	= cardbuf;
      if (gUserId>0) {
            USART1_Printf("yuyue0\r\n");
			UserIdbuf=gUserId;
            if (dev.WorkType == DEVICE_TYPE_NEED_BOOK) {
                USART1_Printf("yuyue1\r\n");
                // 需要预约
                if (dev.BookLen > 0) {
					gRFCardUse = cardbuf;
                    USART1_Printf("yuyue2\r\n");
                    ctime = RTC_GET_TIME_UTC();
                    bookcnt=findBookInfo(gUserId, ctime, cardbuf);
                    if(bookcnt>0){
                        USART1_Printf("%d\r\n",bookcnt);
                    }else{
                        Can_Send_Msg(ExtId,0x08,0x02);
                    }
                  }else{
                     Can_Send_Msg(ExtId,0x08,0x02);
                  } 
				USART1_Printf("efjdh\r\n");				  
            } else {
                                          
                // 不需要预约
                  gRemainTime = 24 * 60 * 60;		// 开通24小时
                  USART1_Printf("$MCU,0,%x,0,%d\r\n", dev.SN, (24 * 60));
                  gUseTime = 0;
                  gRFCardUse = cardbuf;
                  cardbuf=0;
                  SN_receive=chooseSN(ExtId);
//                  moduleNO_buf=ExtId;
//                  USART1_Printf("%x\r\n",ExtId);
                  

                  for(i=0;i<6;i++)
                    {
                        number=i+1;
						IDbuf=findfreemodule_Info(chooseSN((u32)number),number);
                        if(IDbuf>0&&IDbuf<10)
                            {
								if(ID_buf==0)
								{
									ID_buf=ExtId;
								}
								moduleNO_buf=ID_buf;
                                count++;
                            }
                    }
                    if(count>0){
                        Door_open_Hander(moduleNO_buf);
                        openstate=1;
						count=0;
                    }
                        
            } 
        }else{
            Can_Send_Msg(ExtId,0x08,0x02);
         }  
		if (((gUserId=isAdminCard(cardbuf))>0) || 
                (cardbuf==0x556BC58F) || 
                (cardbuf==0x9AC89BC6) ||
                (cardbuf==0x7EAD589D) ||
                (cardbuf==0xF5C3768F) ||
                (cardbuf==0x95BD858F) ||
				(cardbuf==0xE5D27C8F)){
					if(errorbuf[ExtId-1]==1){
						Error_state=0;
						moduleNO_buf=ExtId;
						UserIdbuf=0;
						ErrorFlag=1;
						gRFCardUse = cardbuf;
						Door_open_Hander(ExtId);
						Can_Send_Msg(ExtId,0x08,0x04);
						USART1_Printf("dgsh\r\n");
					}else{						
						// 设置随机ID为当前时间的秒数
		//                gRandomId=RTC_GET_TIME();//得到计数器中的值(秒钟数)
						gRemainTime = 7 * 24 * 60 * 60;		// 管理卡开通24小时
						USART1_Printf("$MCU,0,%d,0,%d\r\n", dev.SN, (24 * 60));
						gUseTime = 0;
						gRFCardUse = cardbuf;
						cardbuf=0;
						SN_receive=chooseSN(ExtId);
//						moduleNO_buf=ExtId;
			//                        USART1_Printf("%d\r\n",SN_receive);
			//                        Door_open_Hander(ExtId);
						USART1_Printf("open:%d\r\n",moduleNO_buf);
						for(i=0;i<6;i++)
						{
							number=i+1;
							modbuf=findfreemodule_Info(chooseSN((u32)number),number);							
							if(modbuf>0&&modbuf<10)
								{
									if(mod_buf==0){
										mod_buf=ExtId;
									}
										
									moduleNO_buf=mod_buf;
									count++;
								}
						}
						USART1_Printf("count=%d\r\n",count);
						if(findAdminReturn_Info(SN_receive)==IS_OK)
						  {
							    moduleNO_buf=ExtId;
								count++;
						  }
						if(count>0){
							Door_open_Hander(moduleNO_buf);
							openstate=1;
							count=0;
						}
					}						
		} 
		if (TCP_Connected()) {
			// 向服务器进行用户认证
				 SN_receive=chooseSN(ExtId);
//				 moduleNO_buf=ExtId;
				 USART3_Printf("$MCU,13,%d,%X\r\n", SN_receive , gRFCardIn); 
		}
			
			// 离线认证不通过，则判断是否管理卡						
		   // 在线认证登录
			
}
void checkdoorstate(void)//检查橱窗实验柜是否闭合，若没有没有闭合，则进行相关处理
{
    u8 checkcnt_right=0;
    u8 checkcnt_left=0;
    u8 checkcnt_open=0;
    if(closecnt>=16&&closecnt<19)
    {
//        USART1_Printf("yui");
        if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3))            
        {    
           GPIO_ResetBits(GPIOC,GPIO_Pin_6);
           checkcnt_right++;
        }
        if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))
        {   
           USART1_Printf("z9"); 
           GPIO_ResetBits(GPIOC,GPIO_Pin_6);
           checkcnt_left++;
        }
    }else if(closecnt>=19)
    {
        closecnt=0;
        closestate=0;
        USART1_Printf("y96");
        GPIO_SetBits(GPIOC,GPIO_Pin_6);         
    }
    if(opencnt>=14&&opencnt<18)
    {
        if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))            
        {    
           GPIO_ResetBits(GPIOC,GPIO_Pin_6);
           checkcnt_open++;
        }
    }else if(opencnt>=18)
    {
        opencnt=0;
        openstate=0;
        GPIO_SetBits(GPIOC,GPIO_Pin_6);
    }
    if(closecount>=60)
    {
        closecount=0;
        closestate_many=0;
        if(checkcnt_left>=4||checkcnt_right>=4||checkcnt_open>=4)
        {
             BUZZER_ON();
             gBuzzerCnt = BUZZER_ONE_SEC;
             USART3_Printf("$MCU,11,%d,%X,%X\r\n",dev.SN,RTC_GET_TIME_UTC(),ErrorCard_Buf);
        }
    }
    
}
///////////////////////////////////////////////////////////////////////////////////////////////
u16 updateOfflineInfo(OfflineInfo *info) {//更新离线数据
	u16 i;
	
	for (i=0; i<dev.OfflineLen; i++) {
		if ((pOfflineList[i].id==info->id) && 
			(pOfflineList[i].Module_SN==info->Module_SN)) {
			pOfflineList[i].Borrowtime = info->Borrowtime;
			pOfflineList[i].Returntime = info->Returntime;	
			pOfflineList[i].rid = info->rid;
			return dev.OfflineLen;
		}
	}
	if (i == dev.OfflineLen) {
		if (i<OFFLINE_MAX_LEN) {
			memcpy(&pOfflineList[i], info, sizeof(OfflineInfo));
			dev.OfflineLen += 1;
		} else {
			USART1_Printf("offline list is full.\r\n");
		}
	}
	return dev.OfflineLen;
}
///////////////////////////////////////////////////////////////////////////////////////////////
u8 updateModuleInfo(ModuleInfo *info){//更新各个模块信息
		u16 i;
	
	for (i=0; i<dev.ModuleInfoLen; i++) {
		if (pModuleList[i].Module_SN==info->Module_SN) {			
//			pModuleList[i].UserCard_ID = info->UserCard_ID;
//			pModuleList[i].id = info->id;	
//			pModuleList[i].Module_state = info->Module_state;
			memcpy(&pModuleList[i], info, sizeof(ModuleInfo));
			return dev.ModuleInfoLen;
		}
	}
	if (i == dev.ModuleInfoLen) {
		if (i<MODULE_MAX_LEN) {
			memcpy(&pModuleList[i], info, sizeof(ModuleInfo));
			dev.ModuleInfoLen += 1;
		} else {
			USART1_Printf("ModuleInfo list is full.\r\n");
		}
	}
	return dev.ModuleInfoLen;
}
///////////////////////////////////////////////////////////////////////////////////////////////
u16 updateErrorInfoFile(ErrorInfo*info){//更新错拿等离线错误信息
		u16 i;
	
	for (i=0; i<dev.ErrorInfoLen; i++) {
		if ((pErrorList[i].Module_SN==info->Module_SN) && 
			(pErrorList[i].Errortpye==info->Errortpye)&&(pErrorList[i].UserdCardId == info->UserdCardId)) {
			pErrorList[i].Borrowtime = info->Borrowtime;
//			pErrorList[i].UserdCardId = info->UserdCardId;	
			return dev.ErrorInfoLen;
		}
	}
	if (i == dev.ErrorInfoLen) {
		if (i<ERROR_MAX_LEN) {
			memcpy(&pErrorList[i], info, sizeof(ErrorInfo));
			dev.ErrorInfoLen += 1;
		} else {
			USART1_Printf("offline list is full.\r\n");
		}
	}
	return dev.ErrorInfoLen;
}
//////////////////////////////////////////////////////////////////////////////////////////////
u8 onlinehander(u8* buf){//在线认证功能操作
 	u8 state=0, pos=0;
	u8 *p;
	u32 moduleno=0;
	u32 remiantime=0;
	u8 IDbuf=0;
	u8 Free_ID=0;
	p = buf;
	memset(gBuf, 0, 32);
	// $PC,0,1,1001,1:200|2:500
	while (*p) {
		if (state<4) {
			if (*p == ',')
				state ++;
		} else {
			if ((*p != '|') && (*p != '\r') && (*p != '\n') && (pos<16)) {
				gBuf[pos++] = *p;
			} else {
				if (pos>0) {
					pos = 0;
					USART1_Printf("%s\r\n", gBuf);
					if ((sscanf((char*)gBuf, "%d:%d",&moduleno,&remiantime)==2)) {
							gRemainTime =remiantime*60;
							Free_ID=findfreemodule_Info(chooseSN(moduleno),(u8)moduleno);
							if(IDbuf==0){
							    IDbuf=Free_ID;								
							}							
						} else {
							USART1_Printf("err:%s\r\n", gBuf);
						}
					}
					memset(gBuf, 0, 32);
				}
			}
		
		p++;
	}
	return IDbuf;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void findModule_RandonID(void)//查找各个模块借出后相应随机ID
{
   
	u16 i;
	u8 j;
	u8 m;
	u8 n;
	u8 nub;
	u8 nubb;
	for(j=1;j<=6;j++)
	{
		for (i=0; i<dev.ModuleInfoLen; i++) {
			if (pModuleList[i].Module_SN == chooseSN((u32)j)) {
				nub=pModuleList[i].Module_SN%10;
				RandonIDbuf[nub-1]=pModuleList[i].RandonID;
				ModuleUserIdbuf[nub-1]=pModuleList[i].id;
				break;
			}
		}
    }
	
	for(m=1;m<=6;m++)
	{
		for (n=0; n<ErrorStatelen; n++) {
			if (pErrorStateList[n].Module_SN == chooseSN((u32)m)) {
				nubb=pErrorStateList[n].Module_SN%10;
				RandonIDbuf[nubb-1]=pErrorStateList[n].Borrowtime;
				ModuleUserIdbuf[nubb-1]=pErrorStateList[n].UserdId;
				break;
			}
		}
    }

}
///////////////////////////////////////////////////////////////////////////////////////////////
void HeartbeatSend(void){//发送心跳包
	u8 i;
	u8 cnt=0;
	sprintf((char*)pUSART_RxBuf, "$MCU,12,%d,%X,%d",dev.SN,RTC_GET_TIME_UTC(),dev.WorkType); 								
		for (i=0;i<6;i++) {
//			USART1_Printf("RandonIDbuf[%d]=%X,ModuleIdbuf[%d]=%d\r\n",i,RandonIDbuf[i],i,ModuleIdbuf[i]);
			if(RandonIDbuf[i]!=0&&ModuleIdbuf[i]!=1){
				if(cnt==0){
					sprintf((char*)gBuf, ",%d:%d-%X|",i+1,ModuleUserIdbuf[i],RandonIDbuf[i]);
				}else{
					sprintf((char*)gBuf, "%d:%d-%X|",i+1,ModuleUserIdbuf[i],RandonIDbuf[i]);
				}
				cnt++;
				if ((strlen((char*)gBuf) + strlen((char*)pUSART_RxBuf))<250) {
					strcat((char*)pUSART_RxBuf, (char*)gBuf);
					memset(gBuf, 0, GBUF_LEN);
				} else {
					strcat((char*)pUSART_RxBuf,"\r\n");
					break;
				}
		   }
		}
	strcat((char*)pUSART_RxBuf,"\r\n");		
	USART3_Printf("%s", pUSART_RxBuf);
}
///////////////////////////////////////////////////////////////////////////////////////////////
u8 updateErrorState(ErrorState *info){//更新错拿等状态
		u8 i;
	
	for (i=0; i<ErrorStatelen; i++) {
		if (pErrorStateList[i].Module_SN==info->Module_SN) {			
			pErrorStateList[i].UserdCardId = info->UserdCardId;
			pErrorStateList[i].UserdId = info->UserdId;	
			pErrorStateList[i].Borrowtime = info->Borrowtime;
			return ErrorStatelen;
		}
	}
	if (i == ErrorStatelen) {
		if (i<ERRORSTATE_MAX_LEN) {
			memcpy(&pErrorStateList[i], info, sizeof(ErrorState));
			ErrorStatelen += 1;
		} else {
			USART1_Printf("ErrorState list is full.\r\n");
		}
	}
	return ErrorStatelen;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void init_clear(void)
{
	u8 k=0;
	for(k=0;k<6;k++)
	{
		ModuleIdbuf[k]=0;
		RandonIDbuf[k]=0;
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////

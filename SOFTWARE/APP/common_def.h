/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_DEF_H
#define __COMMON_DEF_H
////////////////////////////////////////////////////////////////////////////////
// the device work type
#define	DEVICE_TYPE_NEED_BOOK	1			//工作模式，需要预约
#define DEVICE_TYPE_NO_BOOK		0			//工作模式，不需要预约
#define DEVICE_TYPE_DEFAULT		DEVICE_TYPE_NO_BOOK

// the tcp server command
#define SVR_CMD_AUTH		(12)		// 用户刷卡验证结果
#define SVR_CMD_AUTH_PC		(13)		// PC用户验证结果
#define SVR_CMD_TIME		(2)		// 同步服务器时间
#define SVR_CMD_CARD_ID		(3)		// 同步用户卡信息
#define SVR_CMD_BOOK_INFO	(11)		// 同步预约记录
#define SVR_CMD_SET_TYPE	(5)		// 设置工作模式
#define SVR_CMD_SYNC_ACK	(6)		// 
#define SVR_CMD_ADMIN_CARD	(7)		// 发送管理卡
#define SVR_CMD_RFID_ORDER	(8)		// RFID卡卡号位序
#define SVR_CMD_ERROR_ACK	(9)     // 离线误拿数据上传的回应
#define SVR_CMD_SUPPERCARD   (10)   //设置超级卡
// the pc command through USART1
#define PC_CMD_HEART_BEAT	(0)		// 心跳包
#define PC_CMD_AUTH			(1)		// PC用户验证
#define PC_CMD_CHANGE_ID	(99)	// 更改模块ID
#define PC_CMD_LOGOUT		(2)		// logout by pc
#define PC_CMD_ADMIN_CARD	(4)		// 管理卡命令
#define PC_CMD_SCAN			(5)		// 扫描WIFI热点
#define PC_CMD_SET_WIFI		(6)		// 配置WIFI
#define PC_CMD_SET_TCP		(7)		// 设置TCP服务器
#define PC_CMD_DUMP			(8)		// 显示状态信息
#define PC_CMD_SETTIME		(9)		// 
#define PC_CMD_QUERY		(10)	//
#define PC_CMD_CL_OFFDATA	(11)	//
#define PC_CMD_GET_IP		(12)	// 显示IP和MAC
#define PC_CMD_RF_ORDER		(13)	// 设置RFID位序
#define PC_CMD_CL_BOOK		(14)	// 清空预约信息
#define PC_CMD_CL_ALL		(15)	// 清空NAND FLASH信息
#define PC_CMD_SEND_FILE  	(16) 	//发送文件
#define PC_CMD_ACQ_rough_weight	(17) //获取毛重
#define PC_CMD_ACQ_WEIGHT   (18) //获取当前重量
#define PC_CMD_CL_MODULEINFO	(19)     //清空模块信息
#define PC_CMD_SET_SUPPERCARD   (20)//设置超级卡
#define PC_CMD_CL_ERRORDATA	(21)	//
#define PC_CMD_RELAY_TEST	(50)	// 继电器测试
//#define PC_CMD_MODULE_INIT	(21)
#define PC_CMD_IAP_SW		(100)	//升级主控
#define PC_CMD_CAN_SW		(90)	//升级从机

#define BOOTSW              (0x0807F800)  /* uboot switch */
#define CANSW               (0x0807F804)  /* uboot switch */
#define CR_PG_Set           ((uint32_t)0x00000001)
#define CR_PG_Reset         ((uint32_t)0x00001FFE) 
#define IAPAddress    		(0x8000000)
#define ProgramTimeout       ((uint32_t)0x00002000)

// the server data
#define DATA_FIRST_SECTION	(1)		// 
#define DATA_LAST_SECTION	(2)		//

// the flag
#define FLAG_1_SEC			(1)		//
#define FLAG_10_SEC			(2)		//

//the can bus cmd
#define CAN_TRANS_CMD           (0x08)
#define CAN_TRANS_BROADCAST     (0x10)
#define CAN_TRANS_RECV_CARDID   (0x07)
#define CAN_TRANS_RECV_TIME   (0x06)
#define CAN_TRANS_RECV_REMAINTIME   (0x05)
#define CAN_TRANS_RECV_SN   (0x04)
#define CAN_TRANS_CMD_ORDER_OK    (0x01)
#define CAN_TRANS_CMD_ORDER_FAIL  (0x02)
#define CAN_TRANS_CMD_RE_FLAG   (0x03)
#define CAN_TRANS_WEIGHT_CMD   (0xEF)
#define CAN_TRANS_WEIGHT_TUER   (0xEE)
#define CAN_TRANS_RECV_SUPPERCard (0x02) 


// flagh
#define FLAG_ONE_SEC		(1)
#define FLAG_USART1_EVENT	(2)
#define FLAG_USART2_EVENT	(4)
#define FLAG_USART3_EVENT	(8)
// error tpye
#define BORROW_ERROR        1
#define RETURN_ERROR        2
//
#define IS_OK               2
#define IS_FAIL             0
#endif	/* __COMMON_DEF_H */

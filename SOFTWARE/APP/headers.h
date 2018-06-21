/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_HEADERS_H
#define __COMMON_HEADERS_H
////////////////////////////////////////////////////////////////////////////////
#include "stm32f10x.h"
#include "bsp.h"
#include "App.h"
#include "ili9325.h"
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
#include "iap.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdarg.h>

#define WATCHDOG_EN		1
#define USE_EXT_SRAM	0

#define BUZZER_ONE_SEC		20
#define NET_RST_CNT			50
#define GBUF_LEN			128
///////////////////////////////////////////////////////////////////////////////
#define USER_MAX_LEN		1024	//8*1024=8k
#define ADMIN_MAX_LEN		256		//8*256=2k
#define OFFLINE_MAX_LEN		409		//20*409=8180
#define BOOK_MAX_LEN		512		//16*512=8k
#define ERROR_MAX_LEN       310     //13*310=4030
#define MODULE_MAX_LEN      60      //17*60=1020
#define ERRORSTATE_MAX_LEN  20		//16*20=320
#define RANDONID_MAX_LEN  6		//8*4=32
#define MODULEUSERID_MAX_LEN  6		//8*2=16
//////////////////////////////////////////////////////////////////////////////
#define ACK                     (0x06)  /* acknowledge */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */
///////////////////////////////////////////////////////////////////////////////

#endif	/* __COMMON_HEADERS_H */

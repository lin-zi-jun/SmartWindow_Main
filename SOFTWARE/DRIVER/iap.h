#ifndef __IAP_H
#define __IAP_H															    
#include "stm32f10x.h"
///////////////////////////////////////////////////////////////////////////////
#define BOOTLOADER_LEN	0x10000	//64KB
#define PAGE_SIZE		0x800	//2KB
#define CONF_PAGE_NO	(254)
#define USER_PAGE_NO	(CONF_PAGE_NO-4)
#define ADMIN_PAGE_NO	(USER_PAGE_NO-1)
#define OFFLINE_PAGE_NO (ADMIN_PAGE_NO-4)
#define BOOK_PAGE_NO	(OFFLINE_PAGE_NO-4)
#define ERROR_PAGE_NO   (BOOK_PAGE_NO-2)
#define MODULE_PAGE_NO  (ERROR_PAGE_NO-1)
#define ERRORSTATE_PAGE_NO  (MODULE_PAGE_NO-1)
#define RANDONID_PAGE_NO  (ERRORSTATE_PAGE_NO-1)
#define MODULEUSERID_PAGE_NO  (RANDONID_PAGE_NO-1)
///////////////////////////////////////////////////////////////////////////////
typedef  void (*pMainProgram)(void);
///////////////////////////////////////////////////////////////////////////////
uint16_t crc_ccitt(unsigned char *q, int len);
uint32_t FLASH_PagesMask(uint32_t Size);
void FlashErase(uint32_t size);
FLASH_Status WriteBootLoader(uint32_t offset, uint8_t *buf, uint16_t size);
///////////////////////////////////////////////////////////////////////////////
FLASH_Status WriteFlashDevInfo(void);
void ReadFlashDevInfo(void);
///////////////////////////////////////////////////////////////////////////////
void ReadFlashData(void);
FLASH_Status WriteFlash(u8 page, uint8_t *buf, uint16_t size);
///////////////////////////////////////////////////////////////////////////////
void DLHandler(void);
///////////////////////////////////////////////////////////////////////////////
FLASH_Status FlashEraseLastPage(void);
void RunMainProgram(uint32_t ProgramAddr);
///////////////////////////////////////////////////////////////////////////////
#endif


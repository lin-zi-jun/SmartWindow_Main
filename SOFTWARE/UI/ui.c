#include "ui.h"
#include "LCD_ConfDefaults.h"
#include "FHZK12GUI.h"
#include "WM_Intern_ConfDep.h" 
#include <stdlib.h>
#include "BUTTON.h"
#include "FRAMEWIN.h"
#include "TEXT.h"
///////////////////////////////////////////////////////////////////////////////
#define WIN_HEIGHT	25
#define ITEM_HEIGTH	33
#define BTN_WIDTH	75
#define BTN_LEFT	5
#define TEXT_LEFT	85
///////////////////////////////////////////////////////////////////////////////
TEXT_Handle hTextState;
TEXT_Handle hTextInfo;
TEXT_Handle hTextRemain;
TEXT_Handle hTextUse;
TEXT_Handle hTextTime;
TEXT_Handle hTextID;
extern GUI_CONST_STORAGE GUI_BITMAP bmlogo;
extern OS_SEM mainSem;
extern image image1;
//extern u8 FILE_DATA[250000];
extern u8 FILE_buf[200000];
u8 *pdata;
///////////////////////////////////////////////////////////////////////////////
void UI_Init(void) {
	BUTTON_Handle hButton_1;
	BUTTON_Handle hButton_2;
	BUTTON_Handle hButton_3;
	BUTTON_Handle hButton_4;
	BUTTON_Handle hButton_5;
	FRAMEWIN_Handle hFrame;
	u8 yOffset;
	
	GUI_Init();
	GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontHZ12);
	//GUI_SetFont(&GUI_Font8x16);
	GUI_Clear();
		
	GUI_DrawBitmap(&bmlogo,0,0);
	FRAMEWIN_SetDefaultCaptionSize(24);
	FRAMEWIN_SetDefaultFont(&GUI_FontHZ12);
	TEXT_SetDefaultFont(&GUI_FontHZ12);
	//FRAMEWIN_SetDefaultFont(&GUI_Font8x16);
	//TEXT_SetDefaultFont(&GUI_Font8x16);
	hFrame = FRAMEWIN_Create("实验室设备管理系统", 0, WM_CF_SHOW, 0, 53, 320, 187);
	FRAMEWIN_SetTextAlign(hFrame, GUI_TA_HCENTER|GUI_TA_VCENTER);
	FRAMEWIN_SetActive(hFrame,1);
	
	yOffset = 22;
	hButton_1 = BUTTON_CreateAsChild(BTN_LEFT, yOffset+ITEM_HEIGTH*1, 
		BTN_WIDTH, WIN_HEIGHT, hFrame, 1, WM_CF_SHOW);
	BUTTON_SetFont(hButton_1, &GUI_FontHZ12);
	BUTTON_SetText(hButton_1, "联网状态");
	hButton_2 = BUTTON_CreateAsChild(BTN_LEFT, yOffset+ITEM_HEIGTH*2, 
		BTN_WIDTH, WIN_HEIGHT, hFrame, 1, WM_CF_SHOW);
	BUTTON_SetFont(hButton_2, &GUI_FontHZ12);
	BUTTON_SetText(hButton_2, "信息提示");
	hButton_3 = BUTTON_CreateAsChild(BTN_LEFT, yOffset+ITEM_HEIGTH*3, 
		BTN_WIDTH, WIN_HEIGHT, hFrame, 1, WM_CF_SHOW);
	BUTTON_SetFont(hButton_3, &GUI_FontHZ12);
	BUTTON_SetText(hButton_3, "剩余时间");
	hButton_4 = BUTTON_CreateAsChild(BTN_LEFT, yOffset+ITEM_HEIGTH*4, 
		BTN_WIDTH, WIN_HEIGHT, hFrame, 1, WM_CF_SHOW);
	BUTTON_SetFont(hButton_4, &GUI_FontHZ12);
	BUTTON_SetText(hButton_4, "已用时间");
	
	hButton_5 = BUTTON_CreateAsChild(155, yOffset+ITEM_HEIGTH*1, 
		BTN_WIDTH, WIN_HEIGHT, hFrame, 1, WM_CF_SHOW);
	BUTTON_SetFont(hButton_5, &GUI_FontHZ12);
	BUTTON_SetText(hButton_5, "用户卡号");
	
	
	hTextTime = TEXT_Create( 5, 82, 305, WIN_HEIGHT, 6, 
		WM_CF_SHOW, NULL,
		TEXT_CF_HCENTER|TEXT_CF_VCENTER);
		
	yOffset = 74;
	hTextState = TEXT_Create(TEXT_LEFT, yOffset+ITEM_HEIGTH*1, 50, WIN_HEIGHT, 0, 
		WM_CF_SHOW, "未连接",
		TEXT_CF_LEFT|TEXT_CF_VCENTER);
	
	hTextInfo = TEXT_Create(TEXT_LEFT, yOffset+ITEM_HEIGTH*2, 120, WIN_HEIGHT, 1, 
		WM_CF_SHOW, NULL,
		TEXT_CF_LEFT|TEXT_CF_VCENTER);
	
	hTextRemain = TEXT_Create(TEXT_LEFT, yOffset+ITEM_HEIGTH*3, 200, WIN_HEIGHT, 2, 
		WM_CF_SHOW, NULL,
		TEXT_CF_LEFT|TEXT_CF_VCENTER);
	
	hTextUse = TEXT_Create(TEXT_LEFT, yOffset+ITEM_HEIGTH*4, 200, WIN_HEIGHT, 3, 
		WM_CF_SHOW, NULL,
		TEXT_CF_LEFT|TEXT_CF_VCENTER);
	hTextID = TEXT_Create(235, yOffset+ITEM_HEIGTH*1, 85, WIN_HEIGHT, 4, 
		WM_CF_SHOW, NULL,
		TEXT_CF_LEFT|TEXT_CF_VCENTER);
//		OSSemPost(&mainSem, 0, NULL);
//      show_image_power(200,120);
      OSSemPost(&mainSem, 0, NULL);
     show_image_power(20,15);	
	GUI_WaitKey();
      	
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetState(u8 *str) {
	TEXT_SetText(hTextState, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetStateColor(u8 *str) {
	TEXT_SetText(hTextState, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetInfo(u8 *str) {
	TEXT_SetText(hTextInfo, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetRemain(u8 *str) {
	TEXT_SetText(hTextRemain, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetUse(u8 *str) {
	TEXT_SetText(hTextUse, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetTime(u8 *str) {
	TEXT_SetText(hTextTime, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
void UI_SetID(u8 *str) {
	TEXT_SetText(hTextID, (char*)str);
	WM_Exec();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
u16 LD_WORD(u8 *ptr)
{
 return (u16)(((u16)*((u8*)(ptr)+1)<<8)|(u16)*(u8*)(ptr)); 
}
// void LCD_show_bin(u16 x,u16 y)
// {
// //	USART1_Printf("shaknmn\r\n");
// 	u8 x1=x;
// 	u8 y1=y;
// 	u16 Data=0;
// 	int i,j,k=0;
// 	USART1_Printf("shaknmn%d\r\n",image1.width);
//       USART1_Printf("shaknmn%d\r\n",image1.height);
// //      USART1_Printf("shaknmn\r\n");
// 	if(x+image1.width<320)
// 	{
//      for( i=0;i<image1.height;i++)
// 		{
// 	   for( j=0;j<image1.width;j++)
// 	    {
// 	         pdata=&FILE_DATA[k];	
//                Data=LD_WORD(pdata);
// 		   LCD_Fast_DrawPoint(x1,y1,Data);
// 		   k=k+2;
// 		   x1++;
//       }
// 	    x1=x;
// 	    y1++;
//     }
//  }
//   else
//   {
//    for( i=0;i<image1.height;i++)
// 		{
// 	   for( j=0;j<320-x;j++)
// 	    {
// 				pdata=&FILE_DATA[k];
//        Data=LD_WORD(pdata);
// 		   LCD_Fast_DrawPoint(x1,y1,Data);
// 	     if(j==319-x)
// 			 {
// 		     k=k+x+image1.width-320;
// 			 }
// 			 else				
//        k=k+2;			 
// 		   x1++;
//       }
// 	    x1=x;
// 	    y1++;    

//     }
//  }
// }

void LCD_show_bin(u16 x,u16 y)
{
	u16 x1=x;
	u16 y1=y;
	u16 Data=0;
	int i,j,k=0;
//       image1.width=180;
//       image1.height=180;
      USART1_Printf("shaknmn%d\r\n",image1.size);
	USART1_Printf("shaknmn%d\r\n",image1.width);
      USART1_Printf("shaknmn%d\r\n",image1.height);
//      USART1_Printf("shaknmn\r\n");
	if(x+image1.width<320)
	{
        for( i=0;i<image1.height;i++)
	    {
               for( j=0;j<image1.width;j++)
                {
                     pdata=&FILE_buf[k];	
                     Data=LD_WORD(pdata);
                     LCD_Fast_DrawPoint(x1,y1,Data);
                     k=k+2;
                     x1++;
                 }
	    x1=x;
	    y1++;
       }
 }
  else
  {
   for( i=0;i<image1.height;i++)
    {
	   for( j=0;j<320-x;j++)
	    {
		   pdata=&FILE_buf[k];
               Data=LD_WORD(pdata);
		   LCD_Fast_DrawPoint(x1,y1,Data);
	         if(j==319-x)
		     {
		       k=k+x+image1.width-320;
		     }
		   else				
                 k=k+2;			 
	           x1++;
           }
	    x1=x;
	    y1++;    

    }
 }
}
void show_image(u16 x,u16 y)
{
	extern u8 READ_FILE_FLAG;
      create_image_file();
      read_image_file();
  	if(READ_FILE_FLAG==1)
  	  {						
 	    LCD_show_bin(x,y);
          READ_FILE_FLAG=0;                  
	   }
}
void show_image_power(u16 x,u16 y)
   {
      readimageInfo();	
      USART1_Printf("dwwdanmn%d\r\n",readFile("test.bin", FILE_buf, image1.size));                        
      LCD_show_bin(x,y);     
   }
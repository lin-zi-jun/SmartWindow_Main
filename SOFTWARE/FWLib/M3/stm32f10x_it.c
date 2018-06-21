/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "headers.h"
///////////////////////////////////////////////////////////////////////////////
extern OS_SEM mainSem;
extern u32 gEventFlag;
extern u8 Usart1TxSem, Usart2TxSem, Usart3TxSem;
extern CanRxMsg RxMessage;
extern u8 ret;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : This function handles DMA1 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
	CPU_SR_ALLOC();
	if(DMA_GetITStatus(DMA1_IT_TC2))
    {
        DMA_ClearITPendingBit(DMA1_IT_GL2);
        USART_DMACmd(USART3, USART_DMAReq_Tx, DISABLE);
        DMA_Cmd(DMA1_Channel2, DISABLE);
		OS_CRITICAL_ENTER();
        Usart3TxSem = 0; 
		OS_CRITICAL_EXIT();
    }
	
}

/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : This function handles DMA1 Channel 3 interrupt request.
* For USART3 DMA Rx
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
	/*
	if(DMA_GetITStatus(DMA1_IT_TC3)) //通道3传输完成中断TC 还有传输 过半中断HT 错误中断TE 全局中断GL
	{
		DMA_Cmd(DMA1_Channel3, DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_GL3);    //清除全部中断标志		
	}
	*/
}

/*******************************************************************************
* Function Name  : DMA1_Channel4_IRQHandler
* Description    : This function handles DMA1 Channel 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
	CPU_SR_ALLOC();
    if(DMA_GetITStatus(DMA1_IT_TC4))
    {
        DMA_ClearITPendingBit(DMA1_IT_GL4);
        USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
        DMA_Cmd(DMA1_Channel4, DISABLE);
		OS_CRITICAL_ENTER();
        Usart1TxSem = 0; 
		OS_CRITICAL_EXIT();
    }
}

/*******************************************************************************
* Function Name  : DMA1_Channel5_IRQHandler
* Description    : This function handles DMA1 Channel 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel5_IRQHandler(void)
{
	/*
	if(DMA_GetITStatus(DMA1_IT_TC5)) //通道3传输完成中断TC 还有传输 过半中断HT 错误中断TE 全局中断GL
	{
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_GL5);    //清除全部中断标志		
	}
	*/
}

/*******************************************************************************
* Function Name  : DMA1_Channel6_IRQHandler
* Description    : This function handles DMA1 Channel 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel7_IRQHandler
* Description    : This function handles DMA1 Channel 7 interrupt request.
* USART DMA Tx
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
	CPU_SR_ALLOC();
    if(DMA_GetITStatus(DMA1_IT_TC7))
    {
        DMA_ClearITPendingBit(DMA1_IT_GL7);
        USART_DMACmd(USART2, USART_DMAReq_Tx, DISABLE);
        DMA_Cmd(DMA1_Channel7, DISABLE);
		OS_CRITICAL_ENTER();
        Usart2TxSem = 0; 
		OS_CRITICAL_EXIT();
    }
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
	USART1_IRQHandler_Run();
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
    USART2_IRQHandler_Run();
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{ 
	USART3_IRQHandler_Run();
}

extern u8 add1,add2,add3,add4,add5,add6,add0;
extern u32 Card_buf;
u8 i,j=0;
FLASH_Status status;
u8 buf[8];
extern u8 check_time;
extern u8 borrow_state;
extern u8 recevecard;
extern u8 repay_state;
extern u32 timebuf;
extern u32 Return_Card_buf;
extern u8 ModuleState;
extern u8 SupperCardState;
extern u8 Differ_State;
extern u8 Suppercard_return;
extern u8 Error_state;
extern u8 ErrorFlag;
void USB_LP_CAN1_RX0_IRQHandler(void)		 //CAN中断函数
{
  //CanRxMsg RxMessage;
  ret = 0;      
  RxMessage.StdId=0x00;		  //初始化接收数据池
  RxMessage.ExtId=0x00;		  //
  RxMessage.IDE=0;			  //
  RxMessage.DLC=0;			  //
  RxMessage.FMI=0;			  //
  RxMessage.Data[0]=0x00;	  //
  RxMessage.Data[1]=0x00;	  //
  RxMessage.Data[2]=0x00;
  RxMessage.Data[3]=0x00;
  RxMessage.Data[4]=0x00;

  //while(CAN_IT_FF0==0);
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);	  //从CAN1口缓冲器FIFO 0接收报文到 RxMessage
	
  for(j=0;j<8;j++)
	buf[j]=RxMessage.Data[j];
	if((buf[0]=='$')&&(buf[1]=='M')&&(buf[2]=='R'))
		{
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
		}
 
  if(RxMessage.ExtId==0x20)
   {
      if(RxMessage.Data[0]==0x07)
      {
//         char2int(&RxMessage.Data[1],(u8*)&timebuf);
          memcpy(&timebuf,&RxMessage.Data[1],4);
         check_time=1; 
      }
   }else {
       if(RxMessage.Data[0]==0xEE)
         {
             switch (RxMessage.Data[1]){
                case 0xEE:
                    borrow_state=1;//借出成功，立即关门
                    break;
                case 0xEC:
                    borrow_state=3;//借出失败，延时20秒关门
                    break;
                case 0xEF:
                    borrow_state=4;//借出标志
                    break;
                case 0xEA:
                    repay_state=4;//归还标志
                    break;
                case 0xDF:
                    repay_state=1;//归还成功，立即关门
                    break;
                case 0xDD:
                    repay_state=3;//归还失败，延时20秒关门
                    break;
				case 0xDC:
                    ErrorFlag=1;//归还误拿仪器标志
                    break;
                case 0x01:
                    ModuleState=1;//该模块已经归还				
//				USART1_Printf("ghg");
                    break;
                case 0x02:
                    ModuleState=2; //该模块已经借走
                    break;
                case 0x03:
                    ModuleState=3; //该模块被误拿
                    break;
                case 0x04:
                    SupperCardState=1;//超级卡使用中开门
                    break;
                case 0x05:
                    SupperCardState=2;//超级卡使用中关门
                    break;
                case 0x06:
                    Differ_State=1;//与原仪器不符的操作
                    break;
				case 0x07:
                    Suppercard_return=1;//超级卡处理仪器丢失
                    break;
				case 0x08:
                    Error_state=1;//仪器错拿标志
                    break;
                default:
                    break;
             }
           }else if(RxMessage.Data[0]==0x09){
        //          USART1_Printf("hush\r\n");
//                  char2int(&(RxMessage.Data[1]),(u8*)&Card_buf);  //这里就已经取得gRFCardIn这个数组的空间地址了，(u8 *)这是强制类型转换，将gRFCardIn数组强制转化成u8类型的指针。
                  memcpy(&Card_buf,&(RxMessage.Data[1]),4);
                  recevecard=1;//
                  USART1_Printf("%X\r\n",Card_buf);
            }
    }
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

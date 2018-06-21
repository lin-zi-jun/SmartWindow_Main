#include "ctrio_driver.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//∑‰√˘∆˜IO ≈‰÷√
u8 add1=0x00,add2=0x00,add3=0x00,add4=0x00,add5=0x00,add6=0x00,add0=0x00;
void BuzzerIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOG, &GPIO_InitStructure);				
	GPIO_ResetBits(GPIOG,GPIO_Pin_15);						
}
//ºÃµÁ∆˜IO ≈‰÷√
void RelayIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	
	BUZZER_OFF();
}
///////////////////////////////////////////////////////////////////////////////
void SWIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 |
								  GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOF, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_Init(GPIOG, &GPIO_InitStructure); 
}
///////////////////////////////////////////////////////////////////////////////
void EthIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// ETH_Link=PB12 (Activ Low)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
 	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	// ETH_RST=PB13(Activ Low), ETH_CFG=PB14(Low to config via USART)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14);
}
///////////////////////////////////////////////////////////////////////////////
void BUZZER_ON(void)
{
	GPIO_SetBits(GPIOG,GPIO_Pin_15);
}
///////////////////////////////////////////////////////////////////////////////
void BUZZER_OFF(void)
{
	GPIO_ResetBits(GPIOG,GPIO_Pin_15);
}
///////////////////////////////////////////////////////////////////////////////
void BUZZER_TOGGLE(void) {
	GPIO_WriteBit(GPIOG, GPIO_Pin_15, 
		(BitAction)(1-GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_15)));
}
///////////////////////////////////////////////////////////////////////////////
void RELAY_ON(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}
///////////////////////////////////////////////////////////////////////////////
void RELAY_OFF(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}
///////////////////////////////////////////////////////////////////////////////
u8 TCP_Connected(void) 
{
	return (u8)(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==Bit_RESET);
}
///////////////////////////////////////////////////////////////////////////////
void EthNetReset(void) {
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}
///////////////////////////////////////////////////////////////////////////////
void EthNetRun(void) {
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}
///////////////////////////////////////////////////////////////////////////////
void CtrlIO_Init(void) {
	BuzzerIO_Init();
	RelayIO_Init();
	SWIO_Init();
	EthIO_Init();
    DOOR_Init();
    doorcheckstate_init();
}
///////////////////////////////////////////////////////////////////////////////

void DOOR_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOD, &GPIO_InitStructure);
      
	GPIO_ResetBits(GPIOC,GPIO_Pin_12);//ƒ¨»œ¿Ô√Ê≤£¡ßœÚ◊Û      
	GPIO_SetBits(GPIOD,GPIO_Pin_2);//ƒ¨»œÕ‚√Ê≤£¡ßœÚ”“						
}
void DOOR_out_right(void)
{
     GPIO_SetBits(GPIOD,GPIO_Pin_2); 
}
void DOOR_out_left(void)
{
     GPIO_ResetBits(GPIOD,GPIO_Pin_2); 
}
void DOOR_in_right(void)
{
     GPIO_SetBits(GPIOC,GPIO_Pin_12); 
}
void DOOR_in_left(void)
{
     GPIO_ResetBits(GPIOC,GPIO_Pin_12); 
}

void doorcheckstate_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);				
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
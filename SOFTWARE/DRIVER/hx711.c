#include "HX711.h"
#include "delay.h"
#include "stm32f10x.h"
//**************************************************


u32 Weight_Maopi,Weight_Shiwu;

//***************************************************
//��ʼ��HX711�˿� 
//***************************************************
void HX711_Init(void)	
{ 
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB�˿�ʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 //SCK 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	 //DOUT 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 //�����趨������ʼ��PC1
}

//****************************************************
//��ȡHX711
//****************************************************
u32 Read_HX711(void)  //��HX711оƬ��������ݡ� 
{ 
	u32 val = 0; 
	unsigned char i = 0; 

	GPIO_SetBits(GPIOC,GPIO_Pin_1);	   //DOUT=1 
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	   //SCK=0 
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1));   //�ȴ�DOUT=0  
		delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(GPIOC,GPIO_Pin_0);	   //SCK=1 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);	   //SCK=0 
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))   //DOUT=1 
		val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(GPIOC,GPIO_Pin_0); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(GPIOC,GPIO_Pin_0); 
	delay_us(1);  
	return val; 	
}

u32 Get_weight(void)
{
	u32 HX711_Buffer = 0;
	u32 Weight_Maopi = 0;

	HX711_Buffer = Read_HX711();
	Weight_Maopi = HX711_Buffer/100;	

	return Weight_Maopi;
} 

u32 Get_Shiwu(void)
{
	u32 HX711_Buffer = 0;


	HX711_Buffer = Read_HX711();
	HX711_Buffer = HX711_Buffer/100;
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//��ȡʵ���AD������ֵ��
	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/4.4+0.05); 	//����ʵ���ʵ������
																		//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������4.30���������
																		//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
																		//������Գ���������ƫСʱ����С����ֵ��
																		//����ֵһ����4.0-5.0֮�䡣�򴫸�����ͬ������50g���룬��1g���Ҫ����һλС��
																		//+0.05��Ϊ����������ٷ�λ
																		//��취��һ��ͨ��
	}
	return Weight_Shiwu;
}
u8 hx711_state=0;
 void weight_state(void)
{
      u32 weight=0,weight_buf=0,maopi=0;
      u8 i=0;
      weight_buf=weight;
      weight=Get_weight();
      if(weight_buf!=0)
      {
            if(weight>=weight_buf)
            {
               if((weight-weight_buf)>100)
               {                                  
                   hx711_state=1;       //            "���������"
                                                              
               }
            }
            else
            {
                if((weight_buf-weight)>100)
                {
                    hx711_state=2;      //          "�����ѽ��"    
                }
            }
      }
      switch(hx711_state)
      
         case 0:
              // "���������"
                  break;
         case 1:
               //"�����ѷ���"
                  break;
         case 2��
              // "�����ѽ��"
                  break;

}





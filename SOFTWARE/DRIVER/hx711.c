#include "HX711.h"
#include "delay.h"
#include "stm32f10x.h"
//**************************************************


u32 Weight_Maopi,Weight_Shiwu;

//***************************************************
//初始化HX711端口 
//***************************************************
void HX711_Init(void)	
{ 
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB端口时钟 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 //SCK 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	 //DOUT 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 //根据设定参数初始化PC1
}

//****************************************************
//读取HX711
//****************************************************
u32 Read_HX711(void)  //读HX711芯片输出的数据。 
{ 
	u32 val = 0; 
	unsigned char i = 0; 

	GPIO_SetBits(GPIOC,GPIO_Pin_1);	   //DOUT=1 
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	   //SCK=0 
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1));   //等待DOUT=0  
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
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/4.4+0.05); 	//计算实物的实际重量
																		//因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的4.30这个除数。
																		//当发现测试出来的重量偏大时，增加该数值。
																		//如果测试出来的重量偏小时，减小改数值。
																		//该数值一般在4.0-5.0之间。因传感器不同而定。50g砝码，差1g差不多要调整一位小数
																		//+0.05是为了四舍五入百分位
																		//想办法搞一个通过
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
                   hx711_state=1;       //            "仪器已入柜"
                                                              
               }
            }
            else
            {
                if((weight_buf-weight)>100)
                {
                    hx711_state=2;      //          "仪器已借出"    
                }
            }
      }
      switch(hx711_state)
      
         case 0:
              // "请放入仪器"
                  break;
         case 1:
               //"仪器已放入"
                  break;
         case 2：
              // "仪器已借出"
                  break;

}





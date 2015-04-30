#include "stm32f10x_gpio.h"


#define  ADDO  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) 
#define  ADSK_ON  GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define  ADSK_OFF  GPIO_ResetBits(GPIOA, GPIO_Pin_6)

void AD_GPIOConfig(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

long ReadCount(void){
	unsigned long Count;   
	unsigned int i;   
	ADSK_OFF;   
	Count=0;   
	while(ADDO);   
	for (i=0;i<24;i++){
		ADSK_ON;     
	  Count=Count<<1;     
	  ADSK_OFF;     
	  if(ADDO) Count++;   
	}   
	ADSK_ON;   
	//Count=Count^0x800000;    
	ADSK_OFF;   
	return(Count); 
}

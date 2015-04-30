#include "stm32f10x.h"
#include "USART.h"
#include "stdio.h" 
#define empty 4000;
#define total 4000;
#define full 8000;

GPIO_InitTypeDef GPIO_InitStructure;


void Init_NVIC(void)
{ 	
  	NVIC_InitTypeDef NVIC_InitStructure;			//定义一个NVIC向量表结构体变量

	#ifdef  VECT_TAB_RAM  							//向量表基地址选择

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//将0x20000000地址作为向量表基地址(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //将0x08000000地址作为向量表基地址(FLASH)  
	#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断组 为2 

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//配置串口1为中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//设置占先优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//设置副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//使能串口1中断
	NVIC_Init(&NVIC_InitStructure);							  	//根据参数初始化中断寄存器
}

long AdVal; 
long weight;
int flag =0;
void Delay(u32 nCount);
void Delay_Ms(uint16_t time)  //延时函数
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<10260;j++);
}


int main(){
	SystemInit();
	Init_NVIC();
	AD_GPIOConfig();
	Init_Usart();
  Usart_Configuration(115200);
	USART1_IRQHandler(); //串口中断设置，可选
	Tim_GPIO_Config();
	Tim_Config();
	printf("start from here/n/r");
	while(1){
		AdVal=ReadCount();
		weight=(AdVal-160001)/100;
		if(flag==1){//flag代表是否已发送短信，当发送短信后置为1，水重新装上时清零。
			weight+=total;
		}
		if(weight>=4000&&weight<=8000&&flag==0){//此时处于水量充足状态
			TIM_SetCompare2(TIM4, 40000);
			//green on
		}
		else if(weight>=4000&&weight<=8000&&flag==1){//此时已发送短信，等待装水
			
			//blue on
		}
		else if(weight<4000){//此时水量低于设定值，红灯亮并发送短信
			TIM_SetCompare2(TIM4, 00000);
			//SendMessage(BeareNum,GetWater);
			flag=1;
			//red on
			Delay_Ms(2000);
		}
		else if(weight>8000){//此时水刚被装上
			flag=0;
		}
		//printf("\n%d\r",AdVal);
		//printf("\n%d\r",weight);
		Delay_Ms(400);
	}
}

void Delay(u32 nCount)
{
   for(; nCount != 0; nCount--);
}
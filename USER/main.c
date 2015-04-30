#include "stm32f10x.h"
#include "USART.h"
#include "stdio.h" 
#define empty 4000;
#define total 4000;
#define full 8000;

GPIO_InitTypeDef GPIO_InitStructure;


void Init_NVIC(void)
{ 	
  	NVIC_InitTypeDef NVIC_InitStructure;			//����һ��NVIC������ṹ�����

	#ifdef  VECT_TAB_RAM  							//���������ַѡ��

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//��0x20000000��ַ��Ϊ���������ַ(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //��0x08000000��ַ��Ϊ���������ַ(FLASH)  
	#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж��� Ϊ2 

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//���ô���1Ϊ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//����ռ�����ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//���ø����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//ʹ�ܴ���1�ж�
	NVIC_Init(&NVIC_InitStructure);							  	//���ݲ�����ʼ���жϼĴ���
}

long AdVal; 
long weight;
int flag =0;
void Delay(u32 nCount);
void Delay_Ms(uint16_t time)  //��ʱ����
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
	USART1_IRQHandler(); //�����ж����ã���ѡ
	Tim_GPIO_Config();
	Tim_Config();
	printf("start from here/n/r");
	while(1){
		AdVal=ReadCount();
		weight=(AdVal-160001)/100;
		if(flag==1){//flag�����Ƿ��ѷ��Ͷ��ţ������Ͷ��ź���Ϊ1��ˮ����װ��ʱ���㡣
			weight+=total;
		}
		if(weight>=4000&&weight<=8000&&flag==0){//��ʱ����ˮ������״̬
			TIM_SetCompare2(TIM4, 40000);
			//green on
		}
		else if(weight>=4000&&weight<=8000&&flag==1){//��ʱ�ѷ��Ͷ��ţ��ȴ�װˮ
			
			//blue on
		}
		else if(weight<4000){//��ʱˮ�������趨ֵ������������Ͷ���
			TIM_SetCompare2(TIM4, 00000);
			//SendMessage(BeareNum,GetWater);
			flag=1;
			//red on
			Delay_Ms(2000);
		}
		else if(weight>8000){//��ʱˮ�ձ�װ��
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
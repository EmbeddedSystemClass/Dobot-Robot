#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "ILI93xx.h"
#include "key.h"  
#include "timer.h"
#include "sram.h"   
#include "malloc.h" 
#include "touch.h" 
#include "GUIDemo.h"
#include "GUI.h"

#include "interpolation.h"
#include "decode.h"

extern void Task_1(void);
extern void Task_2(void);

int main(void)
{        
	extern GCode_Msg* pGCode_Msg;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  			//��ʼ����ʱ����
	uart_init(115200);			//��ʼ�����ڲ�����Ϊ115200
	LED_Init();							//��ʼ��LED
 	TFTLCD_Init();					//LCD��ʼ�� 
	TP_Init();							//��ʼ��������
 	KEY_Init();							//������ʼ�� 
 	FSMC_SRAM_Init();				//��ʼ���ⲿSRAM  
	TIM3_Int_Init(999,83); 	//1KHZ ��ʱ��3����Ϊ1ms

	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	TIM4_Int_Init(999,839); //������ɨ���ٶ�,100HZ.
	//TIM4_Int_Init(100-1,8400-1);//10ms

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);//����CRCʱ��
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();									//emWin��ʼ��
	
	pGCode_Msg=(GCode_Msg*)malloc(sizeof(GCode_Msg));//��̬�ڴ�����
			 
	Task_1();	
	Task_2();
	
	  while(1)
	{
		GUI_Exec();//�������к�����������´���
	}		
}



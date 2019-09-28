#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

#include "led.h"
#include "decode.h"
#include "interpolation.h"
#include "usart.h"
#include "delay.h"


extern GCode_Msg* pGCode_Msg;
extern double x,y,z,L1,L2,L3; 
extern int i;


char* pcode;	//ȡһ�е�ָ��

extern void Task_1(void);





/************************************************************************************************/
/*
 *
 *	�������ܣ�����쳣 
 *
 */
void OnButtonClicked_Clear(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON1
{
	Reset_Dobot();					//	��λ��������
}	


/*
 *
 *	�������ܣ���ֹ��е���˶�
 *
 */
void OnButtonClicked_Temination(WM_MESSAGE * pMsg)		// GUI_ID_BUTTON5
{
	LED1 = 0;
}	


/*
 *
 *	�������ܣ�������е��
 *
 */
void OnButtonClicked_Stat(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON4
{
	
}	

/*
 *
 *	�������ܣ���е�۸�λ
 *
 */
void OnButtonClicked_Reset(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON3
{
	LED0 = 0;
	Reset_Dobot();					//��λ��������
}	


/*
 *
 *	�������ܣ�������һҳ��
 *
 */
void OnButtonClicked_Back(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON2
{
	Task_1();
}	


/************************************************************************************************/
/*
 *
 *	�������ܣ���ֱ��
 *
 */
void OnButtonReleased_Line(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON6
{
	 int word = 1;
	 
	 while(word <= 6)
	 {
		 switch (word)
		 {
			 case 1:pcode = "G01X120Y0Z93F800";break;
			 case 2:pcode = "G01X120Y20Z93F800";break;
			 case 3:pcode = "G01X140Y20Z93F800";break;
			 case 4:pcode = "G01X140Y0Z93F800";break;
			 case 5:pcode = "G01X120Y0Z93F800";break;
			 case 6:pcode = "G01X120Y0Z120F800";break;			 
		 }
		printf("��ȡ����:%s\r\n",pcode);
	 
	 //����
		printf("����\r\n");
		decode(pcode);//���뺯��
		printf("pGCode_Msg->G_instruct = %u\r\n",pGCode_Msg->G_instruct);
		
		GetArcO1(pGCode_Msg);//������Բ�ĺ���
		printf("pGCode_Msg->O_X = %f\r\n",pGCode_Msg->O_X);
		printf("pGCode_Msg->O_Y = %f\r\n",pGCode_Msg->O_Y);

		 x = pGCode_Msg->coor_value[0];
		 y = pGCode_Msg->coor_value[1];
		 z = pGCode_Msg->coor_value[2];
		 printf("x = %f\r\n",x);
		 
	 /************G01	ֱ�߲岹************/
		if(pGCode_Msg->G_instruct == 0x01)	
		{
				Inverse_Solution();				//���⺯������
				Line_Interpolation();			//ֱ�߲岹��������
			 
				for(i=0; i < 3; i++) 
				{
					pGCode_Msg->coor_currentValue[i] = pGCode_Msg->coor_value[i];  //���浱ǰ����
					printf("pGCode_Msg->coor_currentValue[%d] = %d\r\n",i, pGCode_Msg->coor_currentValue[i]);									
				}

		}
	 
		/*********G02	˳Բ���岹	***********/
		if(pGCode_Msg->G_instruct == 0x02)	
		{
			//�ȵ���ֱ�߲岹�������
			Inverse_Solution();				//���⺯������
			Line_Interpolation();			//ֱ�߲岹��������
			
			//Ȼ����˳Բ���岹
			Arc_Interpolation(pGCode_Msg);//˳Բ���岹
		
		}
		
		/*********G03	��Բ���岹	***********/	
		if(pGCode_Msg->G_instruct == 0x03)	
		{		
			//�ȵ���ֱ�߲岹�������
			Inverse_Solution();				//���⺯������
			Line_Interpolation();			//ֱ�߲岹��������
			
			//Ȼ������Բ���岹
			Counter_Arc_Interpolation(pGCode_Msg);//��Բ���岹
		
		}
		word++;			
	}	
}	


/*
 *
 *	�������ܣ���Բ��
 *
 */
void OnButtonReleased_Arc(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON7
{
	
}	


/*
 *
 *	�������ܣ���Բ
 *
 */
void OnButtonReleased_Circle(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON8
{
	 int word = 1;
	 	 
	 /*		��G����		*/
	 while(word <= 2)
	 {
		 switch (word)
		 {
			 case 1:pcode = "G02X200Y10Z95R20F800";break;
			 case 2:pcode = "G01X200Y10Z120F800";break;			
		 }
		printf("��ȡ����:%s\r\n",pcode);
	 
	 //����
		printf("����\r\n");
		decode(pcode);//���뺯��
		printf("pGCode_Msg->G_instruct = %u\r\n",pGCode_Msg->G_instruct);
		
		GetArcO1(pGCode_Msg);//������Բ�ĺ���
		printf("pGCode_Msg->O_X = %f\r\n",pGCode_Msg->O_X);
		printf("pGCode_Msg->O_Y = %f\r\n",pGCode_Msg->O_Y);

		 x = pGCode_Msg->coor_value[0];
		 y = pGCode_Msg->coor_value[1];
		 z = pGCode_Msg->coor_value[2];
		 printf("x = %f\r\n",x);
		 
	 /************G01	ֱ�߲岹************/
		if(pGCode_Msg->G_instruct == 0x01)	
		{
				Inverse_Solution();				//���⺯������
				Line_Interpolation();			//ֱ�߲岹��������
			 
				for(i=0; i < 3; i++) 
				{
					pGCode_Msg->coor_currentValue[i] = pGCode_Msg->coor_value[i];  //���浱ǰ����
					printf("pGCode_Msg->coor_currentValue[%d] = %d\r\n",i, pGCode_Msg->coor_currentValue[i]);									
				}

		}
	 
		/*********G02	˳Բ���岹	***********/
		if(pGCode_Msg->G_instruct == 0x02)	
		{
			//�ȵ���ֱ�߲岹�������
			Inverse_Solution();				//���⺯������
			Line_Interpolation();			//ֱ�߲岹��������
			
			//Ȼ����˳Բ���岹
			Arc_Interpolation(pGCode_Msg);//˳Բ���岹
		
		}
		
		/*********G03	��Բ���岹	***********/	
		if(pGCode_Msg->G_instruct == 0x03)	
		{		
			//�ȵ���ֱ�߲岹�������
			Inverse_Solution();				//���⺯������
			Line_Interpolation();			//ֱ�߲岹��������
			
			//Ȼ������Բ���岹
			Counter_Arc_Interpolation(pGCode_Msg);//��Բ���岹
		
		}
		word++;			
	}
}	





/**************************************************************************************************/
/*
 *
 *	�������ܣ��������, 20��
 *
 */
void OnButtonMovedOut_BigUp(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON9
{
	int i3=0;
	for(i3=0; i3<88; i3++)			
	{
		DIR_Y = 0; PUL_Y = 1; delay_us(800); PUL_Y = 0; delay_us(800);	//�����������20��
	}
}	


/*
 *
 *	�������ܣ��������, 20��
 *
 */
void OnButtonMovedOut_BigDown(WM_MESSAGE * pMsg)					// GUI_ID_MAXIMIZE
{
	int i3=0;
	for(i3=0; i3<88; i3++)			
	{
		DIR_Y = 1; PUL_Y = 1; delay_us(800); PUL_Y = 0; delay_us(800);	//�����������20��
	}
}	


/*
 *
 *	�������ܣ�С������, 20��
 *
 */
void OnButtonMovedOut_SmallUp(WM_MESSAGE * pMsg)					// GUI_ID_YES
{
	int i3=0;
	for(i3=0; i3<88; i3++)			
	{
		DIR_Z = 1; PUL_Z = 1; delay_us(800); PUL_Z = 0; delay_us(800);	//С����������20��
	}
}	


/*
 *
 *	�������ܣ�С������, 20��
 *
 */
void OnButtonMovedOut_SmallDown(WM_MESSAGE * pMsg)					// GUI_ID_NO
{
	int i3=0;
	for(i3=0; i3<88; i3++)			
	{
		DIR_Z = 0; PUL_Z = 1; delay_us(800); PUL_Z = 0; delay_us(800);	//С����������20��
	}
}	


/*
 *
 *	�������ܣ���������ת, 20��
 *
 */
void OnButtonMovedOut_BaseLeft(WM_MESSAGE * pMsg)					// GUI_ID_CLOSE
{
	int i3=0;
	for(i3=0; i3<88; i3++)			
	{
		DIR_X = 0; PUL_X = 1; delay_us(800); PUL_X = 0; delay_us(800);	//����˳ʱ�룬��20��
	}
}	


/*
 *
 *	�������ܣ���������ת, 20��
 *
 */
void OnButtonMovedOut_BaseRight(WM_MESSAGE * pMsg)					// GUI_ID_HELP
{
	int i3=0;
	for(i3=0; i3<88; i3++)			
	{
		DIR_X = 1; PUL_X = 1; delay_us(800); PUL_X = 0; delay_us(800);	//������ʱ�룬��20��
	}
}	






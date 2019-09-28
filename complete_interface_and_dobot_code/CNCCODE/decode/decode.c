#include "decode.h"
#include "sys.h"
#include "usart.h"	
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "stdbool.h"

//����ѡ�� 1�岹ʹ�ô���������� 0��ʹ��
#define USE_INTER_DEBUG (1)

#if  USE_INTER_DEBUG

#endif

#define NUM_AXIS 4
char axis_codes1[NUM_AXIS]= {'X', 'Y', 'Z', 'F'};//����G�����еĹؼ���

char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc 
GCode_Msg* pGCode_Msg;
int i = 0;


//������code_value
//���ܣ���ȡ�ַ�����ָ���ַ�ָ��strchr_pointer��������ֲ��������ڻ�ȡG�������ֵ
//���룺�ؼ��ֵ�ָ��strchr_pointer
//���أ�G�����йؼ��ֺ������ֵ
 float code_value1(char* strchr_pointer) 
{ 
	#if  USE_INTER_DEBUG
	printf("0------%s\r\n",(const char*)strchr_pointer);
	printf("1------%f\r\n",(float)strtod((const char*)(strchr_pointer+1),NULL));
	printf("2------%f\r\n",strtod("1.34569",NULL));
	#endif
	
	return ((float)strtod((strchr_pointer+1), NULL));
}


//������code_seen
//���ܣ������ַ���strcmdbuffer�еĹؼ���code
//���룺�ؼ���code
//���أ����ҵ��ؼ���1 ����0
 bool code_seen1(char code,char *strcmdbuffer)
	{
		strchr_pointer = strchr(strcmdbuffer, (int)code);
		
	#if  USE_INTER_DEBUG
		printf("code_seen�ҵ�%c %d\r\n",code,(strchr_pointer != NULL));
		printf("code_seen�ҵ�%s\r\n",(char*)(strchr_pointer) );
	#endif
	
		return ((strchr_pointer != NULL));	//Return True if a character was found
	}

	
//������get_coordinatesValue
//���ܣ������ַ���strcmdbuffer�е�����ֵ XYZ R
//���룺�ַ���strcmdbuffer �����ṹ��ָ��pNC_Para
//�����G������Ϣ�ṹ��ָ��pGCode_Msg
 void get_coordinatesValue1(char* strcmdbuffer,GCode_Msg* pGCode_Msg)
 {
	 int i;

	#if  USE_INTER_DEBUG
		printf("get_coordinatesValues����%s\r\n",(char*)(strcmdbuffer) );
	#endif
	
	
	 
	 //���ͱ�������ֵ XYZ
 	 for(i=0; i < NUM_AXIS; i++) 
	 {
			if(code_seen1(axis_codes1[i],strcmdbuffer)) 
			{
					#if  USE_INTER_DEBUG
						printf("111--:%c %s\r\n",axis_codes1[i],(char*)strchr_pointer);
					#endif
				pGCode_Msg->coor_value[i]= (float)code_value1(strchr_pointer);//+ (axis_relative_modes[i] || relative_mode)*current_position[i];
			
				
		
			}
			else //pGCode_Msg->coor_value[i] = pNC_Para->coor_currentValue[i];      //Are these else lines really needed?
				pGCode_Msg->coor_value[i] = pGCode_Msg->coor_value[i]; 
			
			//printf("pGCode_Msg->coor_value[%d] = %f\r\n",i,(float)code_value1(strchr_pointer));
			
			//printf("pGCode_Msg->coor_value[%d] = %f\r\n",i,pGCode_Msg->coor_value[i]);
	 }
	 
	
	 
	//���ͱ���뾶 R
	 if(code_seen1('R',strcmdbuffer))
		{
			pGCode_Msg->O_R=code_value1(strchr_pointer);
		}
		
	 if(code_seen1('F',strcmdbuffer))//�����ٶ�F
	 {
			 pGCode_Msg->F = code_value1(strchr_pointer);
	 }
		
 }
	

//������decode
//���ܣ���һ���ַ�ָ��������룬����õ�����Ϣ������pGCode_Msg�ṹ����
//���룺�ַ���strcmdbuffer 
//�����G������Ϣ�ṹ��ָ��pGCode_Msg
void decode(char* strcmdbuffer )
{
	
	#if  USE_INTER_DEBUG
		printf("DECODE---%s\r\n",strcmdbuffer);
	#endif
	if(code_seen1('G',strcmdbuffer))//G??
   {
		 	
	#if  USE_INTER_DEBUG
	   printf("22222G:%d\r\n",(int)code_value1(strchr_pointer));
	#endif

      switch((int)code_value1(strchr_pointer))
      {
				
				case 0: // G0
				case 1: // G1
				{
#include <stm32f4xx.h>
						pGCode_Msg->G_instruct=0x01;
						get_coordinatesValue1(strcmdbuffer,pGCode_Msg);//��ȡ XYZR ����ֵ
						break;
				} 
				 case 2://G2 ˳Բ���岹
				{
					 pGCode_Msg->G_instruct=0x02;
					get_coordinatesValue1(strcmdbuffer,pGCode_Msg);//��ȡ XYZR ����ֵ
					 break;
				}

				 case 3://G3 ��Բ���岹
				{
						 pGCode_Msg->G_instruct=0x03;
						get_coordinatesValue1(strcmdbuffer,pGCode_Msg);//��ȡ XYZR ����ֵ
					 break;
				}		 
      }
   }
	 else if(code_seen1('M',strcmdbuffer))//����Mָ��
	 {
			pGCode_Msg->M_instruct=(int)code_value1(strchr_pointer);;
	 }
	 else if(code_seen1('F',strcmdbuffer))//�����ٶ�F
	 {
			 pGCode_Msg->F = code_value1(strchr_pointer);
	 }
	
	 //��ӡ����֮���x,y,z���ꡢ�뾶R���ٶ�F	 
	 #if  USE_INTER_DEBUG
	   for(i=0;i<4;i++)
		{
			printf("pGCode_Msg->coor_value[%d]:%f\r\n",i,pGCode_Msg->coor_value[i]);
		}
		
		printf("pGCode_Msg->O_R:%f\r\n",pGCode_Msg->O_R);
		
		printf("pGCode_Msg->F:%d\r\n",pGCode_Msg->F);
	#endif

}


// //������quad_test
// //���ܣ��岹��ʽG_instruct��x,y��������
// //���룺�����x,y �岹��ʽG_instruct
// //���أ�����ֵ
int quad_test2(float x,float y,char G_instruct)
{
 	switch(G_instruct)
 		{
		case 0x02:
			if((x>0&&y>=0))//||(x==0&&y==0))      //quad 1 and (0,0)point	and x+axis
			return (1);

			if(x<=0&&y>0)                      //quad 2	and y+axis
				return (2);
		
				 if(x<0&&y<=0)                      //quad 3 and x-axis	
					return (3);
							
					 if(x>=0&&y<0)                      //quad 4	and y-axis
						return (4);
				break;
					 
		case 0x03:
			if((x>=0&&y>0))//||(x==0&&y==0))      //quad 1 and (0,0)point	and x+axis
			return (1);

			if(x<0&&y>=0)                      //quad 2	and y+axis
				return (2);
		
				 if(x<=0&&y<0)                      //quad 3 and x-axis	
					return (3);
							
					 if(x>0&&y<=0)                      //quad 4	and y-axis
						return (4);
				break;
					 
 		case 0x01:	//ֱ�߲岹����ʵֱ�߲岹����Ҫ֪������ֵ��ֱ�Ӳ岹�Ϳ�����
			if((x>0&&y>=0))//||(x==0&&y==0))      //quad 1 and (0,0)point	and x+axis
				return (1);

			if(x<=0&&y>0)                      //quad 2	and y+axis
				return (2);
		
			if(x<0&&y<=0)                      //quad 3 and x-axis	
				return (3);
							
			if(x>=0&&y<0)                      //quad 4	and y-axis
	return (4);
		}

		return 0;
}




//������GetArcO
//���ܣ���ȡԲ���岹��Բ�����꣬װ�ص��岹�����ṹ��pNC_Para
//���룺�岹�����ṹ��ָ��pNC_Para
//������岹�����ṹ��ָ��pNC_Para�е�Բ��ֵO_X O_Y
//���ò�������GetSteps1 
void GetArcO1(GCode_Msg* pGCode_Msg)
{
	if(pGCode_Msg->G_instruct == 0x03)	//G03	��Բ���岹	
	{
		pGCode_Msg->O_X = pGCode_Msg->coor_value[0] - pGCode_Msg->O_R;
		pGCode_Msg->O_Y = pGCode_Msg->coor_value[1];		
	}
	
	else if(pGCode_Msg->G_instruct == 0x02)
	{
		pGCode_Msg->O_X = pGCode_Msg->coor_value[0] + pGCode_Msg->O_R;
		pGCode_Msg->O_Y = pGCode_Msg->coor_value[1];	
	}
	
}


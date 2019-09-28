#include "interpolation.h"
#include "sys.h"
#include <math.h>
#include "key.h"
#include "led.h"
#include "delay.h"
#include <stdio.h>
#include <stdlib.h>
#include "decode.h"

//L1,L2,L3�ֱ���3�����˵ĳ��ȣ�step_1,step_2,step_3��������ߵ��������������и�����x_term,y_term,z_term��������ߵ�������ȡ�˾���ֵ�����ǷǸ���
//step_sum�ܲ���;x1,y1,z1������꣨0,0,0);i_con��ǰ���ߵĲ�����f1,f2,f3ֱ�߷���;x,y,z��е������ֵ��
//angle_Z1,angle_Z2,angle_Z3,angle_Z4�Ƕ�ֵ��w1,w2,w3,w4,w5,w6����ʱ�õ��м������
//start_angle_1,start_angle_2,start_angle_3,start_angle_4��ʼ�ĽǶȣ�motor_angle_1,motor_angle_2,motor_angle_3,motor_angle_4�ǶȲ�ֵ
		
	int i1,i2,i3,word,step_1,step_2,step_3,step_sum,step_sum1,x_term,y_term,z_term,i_con;
  double f1,f2,f3,angle_Z1,angle_Z2,angle_Z3,angle_Z4,i_con1,x1,y1,z1,motor_angle_1,motor_angle_2,motor_angle_4;		
	double w1,w2,w3,w4,w5,w6;
  double x,y,z;
	double start_angle_1,start_angle_2,start_angle_4;
	double x20,y20,x30,y30,x40,y40,x50,y50;
	double L1 = 113; double L2 = 135; double L3 = 170;	//3�����˵ĳ���
 
void Reset_Dobot(void)					//��λ��������
 {
		 while(KEY0 != 0)	{	DIR_X = 0; PUL_X = 0;delay_us(800); PUL_X = 1;delay_us(800);}		//������λ
		 while(KEY1 != 0)	{	DIR_Y = 0; PUL_Y = 0;delay_us(800); PUL_Y = 1;delay_us(800);}		//��۸�λ
		 while(KEY2 != 0)	{	DIR_Z = 1; PUL_Z = 0;delay_us(800); PUL_Z = 1;delay_us(800);}		//С�۸�λ
		 
		 for(i1=1; i1<3556;i1++)	{DIR_X = 1; PUL_X = 0;delay_us(800); PUL_X = 1;delay_us(800);}		//������ԭ��
		 for(i2=1; i2<288; i2++)	{DIR_Y = 1; PUL_Y = 0;delay_us(800); PUL_Y = 1;delay_us(800);}		//��ۻ�ԭ��
		 for(i3=1; i3<1049;i3++)	{DIR_Z = 0; PUL_Z = 0;delay_us(800); PUL_Z = 1;delay_us(800);}		//С�ۻ�ԭ��
 }
 
 void Inverse_Solution(void)			//���⺯������
 {
		
		/******�����㷨********/			 
				w1 = pow(x,2) + pow(y,2);
				w2 = w1 + pow((z-L1),2);
				w3 = sqrt(w2);
				w4 = atan((z-L1)/sqrt(w1)) * 180 /3.141592654;
				w5 = acos((L2*L2+w2-L3*L3)/(2*w3*L2)) * 180 / 3.141592654;
				w6 = acos((L2*L2+L3*L3-w2)/(2*L2*L3)) * 180 / 3.141592654;

				angle_Z1 = atan(y/x) * 180 /3.141592654;
				angle_Z2 = 90 - (w4 + w5);
				angle_Z3 = 180 - w6;
				angle_Z4 = 90 - angle_Z2 - angle_Z3;
			
		/******************************/ 
			 
			  if(word == 1)					//���Ƕ�
				{
					start_angle_1 = 0;	//��ʼ�Ƕ�Ϊ0,0,0
					start_angle_2 = 0;
					start_angle_4 = 0;
				}
			
			 
				motor_angle_1 = angle_Z1 - start_angle_1;	//�ǶȲ�ֵ
				motor_angle_2 = angle_Z2 - start_angle_2;
				motor_angle_4 = angle_Z4 - start_angle_4;
				
				start_angle_1 = angle_Z1;									//ת����ĽǶ��ٵ�����һ���˶��ĳ�ʼֵ
				start_angle_2 = angle_Z2;
				start_angle_4 = angle_Z4;
				
				/**  �жϵ������   ************/
				if(motor_angle_1 >= 0)		//˵������ʱ����ת
				{
						DIR_X = 0;
				}
				else
				{
						DIR_X = 1;
				}
				
				if(motor_angle_2 >= 0)		//˵���������
				{
						DIR_Y = 1;
				}
				else
				{
						DIR_Y = 0;
				}
				
				if(motor_angle_4 >= 0)		//˵��С������
				{
						DIR_Z = 1;
				}
				else
				{
						DIR_Z = 0;
				}
				
				step_1 = motor_angle_1 / 0.0225;		//�Ƕ�ת��Ϊ������
				step_2 = motor_angle_2 / 0.0225;
			  step_3 = motor_angle_4 / 0.0225;
				
				x_term = abs(step_1);								//�������Ӧת������������Ϊ����
				y_term = abs(step_2);
				z_term = abs(step_3);
}
 
void Line_Interpolation(void)		//ֱ�߲岹��������
{
			//���ȽϷ�ֱ�߲岹
			step_sum = x_term + y_term + z_term;			
			x1 = 0; y1 = 0; z1 = 0;								//��ʼ���꣨0��0��0��
			i_con = 0;
			
			
			//�ռ�ֱ�߲岹����������
			while(i_con < step_sum)
			{		
					if(x_term == 0)			//��yzƽ����
					{
						f3 = y_term * z1 - z_term * y1;		//yzƽ��
						if(f3 >= 0)			//��+y�������һ��
						{
								if(y_term == 0)
								{
									PUL_Z = 0;delay_us(800); PUL_Z = 1;delay_us(800);	
									z1++; 
									i_con++;
								}
								else
								{
									PUL_Y = 0;delay_us(800); PUL_Y = 1;delay_us(800);
									y1++;
									i_con++;
								}					
						}
						else
						{
								PUL_Z = 0;delay_us(800); PUL_Z = 1;delay_us(800);
								z1++; 
								i_con++;
						}
							
					}
					
					else	
					{
						f1 = x_term * y1 - y_term * x1;		//xyƽ��
						f2 = x_term * z1 - z_term * x1;		//xzƽ��
						
						if((f1 >= 0) && (f2 >= 0))
						{
								if(x1 == x_term)
								{
									i_con = step_sum;
								}
								else 
								{
									PUL_X = 0;delay_us(800); PUL_X = 1;delay_us(800);
									x1++; 
									i_con++;
								}
						}
					
						else if (f2 < 0) 
						{
								if(z1 == z_term)
								{
									i_con = step_sum;
								}
								else 
								{
									PUL_Z = 0;delay_us(800); PUL_Z = 1;delay_us(800);
									z1++; 
									i_con++;
								}
						}
					
						else if(f1 < 0)
						{
								if(y1 == y_term)
								{
									i_con = step_sum;
								}
								else 
								{
									PUL_Y = 0;delay_us(800); PUL_Y = 1;delay_us(800);
									y1++; 
									i_con++;
								}
						}
					
					}
			}		
}


	

void Counter_Arc_Interpolation(GCode_Msg* pGCode_Msg)	//��Բ���岹
{
	
	
	
	x20 = pGCode_Msg->coor_value[0] - pGCode_Msg->O_R;				//�ڶ�����
	y20 = pGCode_Msg->coor_value[1] + pGCode_Msg->O_R;

	x30 = pGCode_Msg->coor_value[0] - 2 * pGCode_Msg->O_R;		//��������
	y30 = pGCode_Msg->coor_value[1];
	
	x40 = pGCode_Msg->coor_value[0] - pGCode_Msg->O_R;				//���ĸ���
	y40 = pGCode_Msg->coor_value[1] - pGCode_Msg->O_R;
	
	x50 = pGCode_Msg->coor_value[0];
	y50 = pGCode_Msg->coor_value[1];
	
	GetArcO1(pGCode_Msg);		//������Բ�ĺ���
	printf("pGCode_Msg->O_X = %f\r\n",pGCode_Msg->O_X);
	printf("pGCode_Msg->O_Y = %f\r\n",pGCode_Msg->O_Y);
	
	printf("pGCode_Msg->O_R:%f\r\n",pGCode_Msg->O_R);//��ӡ�뾶
	


	i_con1 = 0;																						//Բ���岹�ĵ�ǰ����Ϊi_con1	
	step_sum1 = 2 * pGCode_Msg->O_R;

	while(i_con1 < step_sum1)		//	��һ��1/4Բ												//�յ��б𣬵���ǰ����С���ܲ���ʱ������ѭ��
	{										
			f1 = (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X) + (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y) - pGCode_Msg->O_R * pGCode_Msg->O_R;				//xyƽ��	Բ�ģ�80,80���뾶20��Բ������					
			if(f1 >= 0)							//����Բ�⣬Ӧ����-x����һ����һ��Ϊ0.3			
			{
					if(x == x20)
					{
						i_con1 = step_sum1;
					}
					else						 		//��-x����һ��,һ��Ϊ0.3��x��y��i_con1�ı仯Ҫһ�£��������� ��õ�һ������Ϊ0.3
					{		
						x = x - 0.3;			//��99.7,80,50��
						y = abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X))) + pGCode_Msg->O_Y;
						
						Inverse_Solution();				//���⺯������
						Line_Interpolation();			//ֱ�߲岹��������		
						
						i_con1 = i_con1 + 0.3;									//��ǰ������0.3��
					}
			}									
			else 						//����Բ�ڣ�Ӧ����+y����һ��		
			{
					if(y == y20)
					{
						i_con1 = step_sum1;
					}
					else 
					{	
						y = y + 0.3;		//��+y����һ��
						x = abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y))) + pGCode_Msg->O_X;
						Inverse_Solution();				//���⺯������
						Line_Interpolation();			//ֱ�߲岹��������		
						i_con1 = i_con1 + 0.3;									//��ǰ������1��0.3
					}										
			}
		}
	
		x = x20; y = y20;		//�ӹ���������							//��ʼ�㣨100��80,50��
		i_con1 = 0;																						//Բ���岹�ĵ�ǰ����Ϊi_con1	
//		step_sum1 = 2 * pGCode_Msg->O_R;						//Բ�ģ�80��80����Բ���岹���ܲ���Ϊstep_sum1

		//�ռ�Բ���岹����������
		while(i_con1 < step_sum1)	//	�ڶ���1/4Բ												//�յ��б𣬵���ǰ����С���ܲ���ʱ������ѭ��
		{										
				f1 = (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X) + (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y) - pGCode_Msg->O_R * pGCode_Msg->O_R;					//xyƽ��	Բ�ģ�80,80���뾶20��Բ������					
				if(f1 >= 0)							//����Բ�⣬Ӧ����-y����һ����һ��Ϊ0.3			
				{
						if(y == y30)
						{
							i_con1 = step_sum1;
						}
						else						 		//��-x����һ��,һ��Ϊ0.3��x��y��i_con1�ı仯Ҫһ�£��������� ��õ�һ������Ϊ0.3
						{		
							y = y - 0.5;			//��99.7,80,50��	
							x = (-1) * abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y))) + pGCode_Msg->O_X; 
							Inverse_Solution();				//���⺯������
							Line_Interpolation();			//ֱ�߲岹��������		
							i_con1 = i_con1 + 0.5;									//��ǰ������0.3��
						}
				}
				
				else 						//����Բ�ڣ�Ӧ����-x����һ��		
				{
						if(x == x30)
						{
							i_con1 = step_sum1;
						}
						else 
						{	
							x = x - 0.5;		//��+y����һ��	
							y = abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X))) + pGCode_Msg->O_Y; 
							Inverse_Solution();				//���⺯������
							Line_Interpolation();			//ֱ�߲岹��������		
							i_con1 = i_con1 + 0.5;									//��ǰ������1��0.3
						}										
				}
		}				
		
		
		x = x30; y = y30;		//�ӹ���������							//��ʼ�㣨100��80,50��
		i_con1 = 0;																						//Բ���岹�ĵ�ǰ����Ϊi_con1	
//		step_sum1 = 2 * r;						//Բ�ģ�80��80����Բ���岹���ܲ���Ϊstep_sum1

		//�ռ�Բ���岹����������
		while(i_con1 < step_sum1)	//	������1/4Բ													//�յ��б𣬵���ǰ����С���ܲ���ʱ������ѭ��
		{										
				f1 = (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X) + (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y) - pGCode_Msg->O_R * pGCode_Msg->O_R;					//xyƽ��	Բ�ģ�80,80���뾶20��Բ������					

				if(f1 >= 0)							//����Բ�⣬Ӧ����+x����һ����һ��Ϊ0.3			
				{
						if(x == x40)
						{
							i_con1 = step_sum1;
						}
						else						 		//��-x����һ��,һ��Ϊ0.3��x��y��i_con1�ı仯Ҫһ�£��������� ��õ�һ������Ϊ0.3
						{		
							x = x + 0.5;			//��99.7,80,50��
							y = (-1) * abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X))) + pGCode_Msg->O_Y; 
							Inverse_Solution();				//���⺯������
							Line_Interpolation();			//ֱ�߲岹��������		
							i_con1 = i_con1 + 0.5;									//��ǰ������0.3��
						}
				}
				
				else 						//����Բ�ڣ�Ӧ����-y����һ��		
				{
						if(y == y40)
						{
							i_con1 = step_sum1;
						}
						else 
						{	
							y = y - 0.5;		//��+y����һ��	
							x = (-1) * abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y))) + pGCode_Msg->O_X; 
							Inverse_Solution();				//���⺯������
							Line_Interpolation();			//ֱ�߲岹��������		
							i_con1 = i_con1 + 0.5;									//��ǰ������1��0.3
						}										
				}
		}				
		
		
		x = x40; y = y40; 		//�ӹ���������							//��ʼ�㣨100��80,50��
		i_con1 = 0;																						//Բ���岹�ĵ�ǰ����Ϊi_con1	
//		step_sum1 = 2 * r;						//Բ�ģ�80��80����Բ���岹���ܲ���Ϊstep_sum1
		//�ռ�Բ���岹����������
		while(i_con1 < step_sum1)	//	���ĸ�1/4Բ															//�յ��б𣬵���ǰ����С���ܲ���ʱ������ѭ��
		{										
				f1 = (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X) + (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y) - pGCode_Msg->O_R * pGCode_Msg->O_R;					//xyƽ��	Բ�ģ�80,80���뾶20��Բ������					

				if(f1 >= 0)							//����Բ�⣬Ӧ����+y����һ����һ��Ϊ0.3			
				{
						if(y == y50)
						{
							i_con1 = step_sum1;
						}
						else						 		//��-x����һ��,һ��Ϊ0.3��x��y��i_con1�ı仯Ҫһ�£��������� ��õ�һ������Ϊ0.3
						{		
							y = y + 0.5;			//��99.7,80,50��
							x = abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y))) + pGCode_Msg->O_X; 
							Inverse_Solution();				//���⺯������
							Line_Interpolation();			//ֱ�߲岹��������		
							i_con1 = i_con1 + 0.5;									//��ǰ������0.3��
						}
				}
				
				else 						//����Բ�ڣ�Ӧ����+x����һ��		
				{
						if(x == x50)
						{
							i_con1 = step_sum1;
						}
						else 
						{	
							x = x + 0.5;		//��+y����һ��	
							y = (-1) * abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (x -  pGCode_Msg->O_X) * (x -  pGCode_Msg->O_X))) + pGCode_Msg->O_Y; 
							Inverse_Solution();				//���⺯������
							Line_Interpolation();			//ֱ�߲岹��������		
							i_con1 = i_con1 + 0.5;									//��ǰ������1��0.3
						}										
				}
		}				
		
		
}


void Arc_Interpolation(GCode_Msg* pGCode_Msg)			//˳Բ���岹
{
	x20 = pGCode_Msg->coor_value[0] + pGCode_Msg->O_R;				//�ڶ�����
	y20 = pGCode_Msg->coor_value[1] + pGCode_Msg->O_R;

	x30 = pGCode_Msg->coor_value[0] + 2 * pGCode_Msg->O_R;		//��������
	y30 = pGCode_Msg->coor_value[1];
	
	x40 = pGCode_Msg->coor_value[0] + pGCode_Msg->O_R;				//���ĸ���
	y40 = pGCode_Msg->coor_value[1] - pGCode_Msg->O_R;
	
	x50 = pGCode_Msg->coor_value[0];
	y50 = pGCode_Msg->coor_value[1];
	
	GetArcO1(pGCode_Msg);		//������Բ�ĺ���
	printf("pGCode_Msg->O_X = %f\r\n",pGCode_Msg->O_X);
	printf("pGCode_Msg->O_Y = %f\r\n",pGCode_Msg->O_Y);
	
	printf("pGCode_Msg->O_R:%f\r\n",pGCode_Msg->O_R);//��ӡ�뾶
	
	
	
	i_con1 = 0;																						//Բ���岹�ĵ�ǰ����Ϊi_con1	
	step_sum1 = 2 * pGCode_Msg->O_R;						//Բ�ģ�80��80����Բ���岹���ܲ���Ϊstep_sum1
	//�ռ�Բ���岹����������
	while(i_con1 < step_sum1)			//	��һ��1/4Բ												//�յ��б𣬵���ǰ����С���ܲ���ʱ������ѭ��
	{										
			f1 = (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X) + (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y) - pGCode_Msg->O_R * pGCode_Msg->O_R;				//xyƽ��	Բ�ģ�80,80���뾶20��Բ������					
			if(f1 >= 0)							//����Բ�⣬Ӧ����+x����һ����һ��Ϊ0.3			
			{
					if(x == x20)
					{
						i_con1 = step_sum1;
					}
					else						 		//��-x����һ��,һ��Ϊ0.3��x��y��i_con1�ı仯Ҫһ�£��������� ��õ�һ������Ϊ0.3
					{		
						x = x + 0.3;			//��99.7,80,50��											
						y = abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X))) + pGCode_Msg->O_Y;
						
						Inverse_Solution();				//���⺯������
						Line_Interpolation();			//ֱ�߲岹��������		
						
						i_con1 = i_con1 + 0.3;									//��ǰ������0.3��
						
					}
			}									
			else 						//����Բ�ڣ�Ӧ����+y����һ��		
			{
					if(y == y20)
					{
						i_con1 = step_sum1;
					}
					else 
					{	
						y = y + 0.3;		//��+y����һ��
						x = (-1) * abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y))) + pGCode_Msg->O_X;
						Inverse_Solution();				//���⺯������
						Line_Interpolation();			//ֱ�߲岹��������		
						i_con1 = i_con1 + 0.3;									//��ǰ������1��0.3
					}										
			}
}
	

	i_con1 = 0;																						//Բ���岹�ĵ�ǰ����Ϊi_con1	
	//�ռ�Բ���岹����������
	while(i_con1 < step_sum1)			//	�ڶ���1/4Բ											//�յ��б𣬵���ǰ����С���ܲ���ʱ������ѭ��
	{										
			f1 = (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X) + (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y) - pGCode_Msg->O_R * pGCode_Msg->O_R;					//xyƽ��	Բ�ģ�80,80���뾶20��Բ������					
			if(f1 >= 0)							//����Բ�⣬Ӧ����-y����һ����һ��Ϊ0.3			
			{
					if(y == y30)
					{
						i_con1 = step_sum1;
					}
					else						 		//��-y����һ��,һ��Ϊ0.3��x��y��i_con1�ı仯Ҫһ�£��������� ��õ�һ������Ϊ0.3
					{		
						y = y - 0.3;			//��99.7,80,50��	
						x = abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y))) + pGCode_Msg->O_X; 
						Inverse_Solution();				//���⺯������
						Line_Interpolation();			//ֱ�߲岹��������		
						i_con1 = i_con1 + 0.3;									//��ǰ������0.3��
					}
			}
			
			else 						//����Բ�ڣ�Ӧ����+x����һ��		
			{
					if(x == x30)
					{
						i_con1 = step_sum1;
					}
					else 
					{	
						x = x + 0.3;		//��+y����һ��	
						y = abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X))) + pGCode_Msg->O_Y; 
						Inverse_Solution();				//���⺯������
						Line_Interpolation();			//ֱ�߲岹��������		
						i_con1 = i_con1 + 0.3;									//��ǰ������1��0.3
					}										
			}
	}

		
	i_con1 = 0;																						//Բ���岹�ĵ�ǰ����Ϊi_con1			
	//�ռ�Բ���岹����������
	while(i_con1 < step_sum1)			//	������1/4Բ											//�յ��б𣬵���ǰ����С���ܲ���ʱ������ѭ��
	{										
			f1 = (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X) + (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y) - pGCode_Msg->O_R * pGCode_Msg->O_R;					//xyƽ��	Բ�ģ�80,80���뾶20��Բ������					

			if(f1 >= 0)							//����Բ�⣬Ӧ����-x����һ����һ��Ϊ0.3			
			{
					if(x == x40)
					{
						i_con1 = step_sum1;
					}
					else						 		//��-x����һ��,һ��Ϊ0.3��x��y��i_con1�ı仯Ҫһ�£��������� ��õ�һ������Ϊ0.3
					{		
						x = x - 0.3;			//��99.7,80,50��
						y = (-1) * abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X))) + pGCode_Msg->O_Y; 
						Inverse_Solution();				//���⺯������
						Line_Interpolation();			//ֱ�߲岹��������		
						i_con1 = i_con1 + 0.3;									//��ǰ������0.3��
					}
			}
			
			else 						//����Բ�ڣ�Ӧ����-y����һ��		
			{
					if(y == y40)
					{
						i_con1 = step_sum1;
					}
					else 
					{	
						y = y - 0.3;		//��+y����һ��	
						x = abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y))) + pGCode_Msg->O_X; 
						Inverse_Solution();				//���⺯������
						Line_Interpolation();			//ֱ�߲岹��������		
						i_con1 = i_con1 + 0.3;									//��ǰ������1��0.3
					}										
			}
	}	
	
	
	i_con1 = 0;																						//Բ���岹�ĵ�ǰ����Ϊi_con1	
	//�ռ�Բ���岹����������
	while(i_con1 < step_sum1)		//	������1/4Բ													//�յ��б𣬵���ǰ����С���ܲ���ʱ������ѭ��
	{										
			f1 = (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X) + (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y) - pGCode_Msg->O_R * pGCode_Msg->O_R;					//xyƽ��	Բ�ģ�80,80���뾶20��Բ������					

			if(f1 >= 0)							//����Բ�⣬Ӧ����+y����һ����һ��Ϊ0.3			
			{
					if(y == y50)
					{
						i_con1 = step_sum1;
					}
					else						 		//��-x����һ��,һ��Ϊ0.3��x��y��i_con1�ı仯Ҫһ�£��������� ��õ�һ������Ϊ0.3
					{		
						y = y + 0.5;			//��99.7,80,50��
						x = (-1) * abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (y - pGCode_Msg->O_Y) * (y - pGCode_Msg->O_Y))) + pGCode_Msg->O_X; 
						Inverse_Solution();				//���⺯������
						Line_Interpolation();			//ֱ�߲岹��������		
						i_con1 = i_con1 + 0.5;									//��ǰ������0.3��
					}
			}
			
			else 						//����Բ�ڣ�Ӧ����-x����һ��		
			{
					if(x == x50)
					{
						i_con1 = step_sum1;
					}
					else 
					{	
						x = x - 0.5;		//��+y����һ��	
						y = (-1) * abs(sqrt(pGCode_Msg->O_R * pGCode_Msg->O_R - (x - pGCode_Msg->O_X) * (x - pGCode_Msg->O_X))) + pGCode_Msg->O_Y; 
						Inverse_Solution();				//���⺯������
						Line_Interpolation();			//ֱ�߲岹��������		
						i_con1 = i_con1 + 0.5;									//��ǰ������1��0.3
					}										
			}
	}			


}


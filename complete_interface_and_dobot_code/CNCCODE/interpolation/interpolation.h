#ifndef __INTERPOLATION_H
#define __INTERPOLATION_H
#include "sys.h"
#include "decode.h"

void Reset_Dobot(void);					//��λ��������
void Inverse_Solution(void);		//���⺯������
void Line_Interpolation(void);	//ֱ�߲岹��������

void Counter_Arc_Interpolation(GCode_Msg* pGCode_Msg);	//��Բ���岹
void Arc_Interpolation(GCode_Msg* pGCode_Msg);					//˳Բ���岹

#endif

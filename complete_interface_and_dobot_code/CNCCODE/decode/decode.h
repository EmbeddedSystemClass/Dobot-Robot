#ifndef __DECODE_H
#define __DECODE_H

//���ͺ��G�������
//�ռ��С:4*3+4*1+2*1+1*1+1*1=20byte;
//�ַ�������ͺ󱣴�
typedef struct {
	
	float coor_value[3]; //G���� X Y Z
	float O_R;//Բ���岹�뾶
	float O_X,O_Y;//Բ�ĺ�������
	int F;//�岹�ٶ�
	char G_instruct;//Gָ�� G00 G01 G02 G03����
	char M_instruct;//M����ָ��
	
	
	long int  coor_currentValue[3]; //��ǰ����
}GCode_Msg;


//����岹�����еĲ���
//�ռ��С��4*3+4*3+4*3+4*1+2*1+1*1+1*1+2*1+4*2=54Byte
typedef struct{
	 //long int  coor_currentValue[3]; //��ǰ����
	 long int  coor_originValue[3];	//�������
	 long int coor_destinationValue[3];//�յ�����
	float O_R;//Բ���岹�뾶
	int F;//�岹�ٶ�
	char G_instruct;//Gָ�� G00 G01 G02 G03����
	signed char f;//��ǰλ��������λ�õ�ƫ��
	unsigned int steps;//ʣ�ಽ��
	//float O_X,O_Y;//Բ�ĺ�������
	char humode;//Բ���岹ģʽ0 �ӻ� 1�Ż�
	char IS_INTERPOLATINO;//��ʱ���Ƿ����ڽ��в岹���̣�0 ���� 1 ��
} NC_Para;



void decode(char* strcmdbuffer );
void GetArcO1(GCode_Msg* pGCode_Msg);

#endif

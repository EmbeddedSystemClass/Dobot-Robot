#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

extern void Task_1(void);

/************************************************************************************************/
/*
 *
 *	�������ܣ�����쳣 
 *
 */
void OnButtonClicked_Clear(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON1
{
}	


/*
 *
 *	�������ܣ���ֹ��е���˶�
 *
 */
void OnButtonClicked_Temination(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON5
{
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
}	



/**************************************************************************************************/
/*
 *
 *	�������ܣ���������˶�
 *
 */
void OnButtonMovedOut_BigUp(WM_MESSAGE * pMsg)					// GUI_ID_BUTTON9
{
}	


/*
 *
 *	�������ܣ���������˶�
 *
 */
void OnButtonMovedOut_BigDown(WM_MESSAGE * pMsg)					// GUI_ID_MAXIMIZE
{
}	


/*
 *
 *	�������ܣ�С�������˶�
 *
 */
void OnButtonMovedOut_SmallUp(WM_MESSAGE * pMsg)					// GUI_ID_YES
{
}	


/*
 *
 *	�������ܣ�С�������˶�
 *
 */
void OnButtonMovedOut_SmallDown(WM_MESSAGE * pMsg)					// GUI_ID_NO
{
}	


/*
 *
 *	�������ܣ���������ת
 *
 */
void OnButtonMovedOut_BaseLeft(WM_MESSAGE * pMsg)					// GUI_ID_CLOSE
{
}	


/*
 *
 *	�������ܣ���������ת
 *
 */
void OnButtonMovedOut_BaseRight(WM_MESSAGE * pMsg)					// GUI_ID_HELP
{
}	






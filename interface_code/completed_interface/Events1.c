#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

extern void Task_2(void);

/*
 *
 *	�������ܣ�������һ���棬����ϵͳ 
 *
 */
void OnButtonClicked_EnterSys(WM_MESSAGE * pMsg)
{
	Task_2();
}


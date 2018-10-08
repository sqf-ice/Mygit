/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: timerģ��
* file name		: timer1.c
* Author 			: huangbs
* version			: 
* DATE			: 20120905
* directory 		: 
* description		: ����NDK_Wait�ӿڹ���
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_Wait"

/****************************************************************
* function name 	 	: void timer1(void)
* functional description 	: ��ʱ����
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  huangbs     20120905	created
*****************************************************************/
void timer1(void)
{
	time_t oldtime=0, diff=0;

	cls_printf("����%s...�����ĵȴ�30s", TESTAPI); 

	//case1 :������ʱ
	oldtime=time(NULL);
	NDK_Wait(30*ONESECOND);
	if((diff=abs(time(NULL)-oldtime-30))>2)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, diff);
		RETURN;
	}

	send_result("%s����ͨ��", TESTAPI);
	return;
}


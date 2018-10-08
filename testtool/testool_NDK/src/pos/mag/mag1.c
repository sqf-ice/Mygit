/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: magģ��
* file name		: mag1.c
* Author 			: chenfm
* version			: 
* DATE			: 20121220
* directory 		: 
* description		: ����NDK_MagOpen�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_MagOpen"

/****************************************************************
* function name 	 	: void mag1(void)
* functional description 	: �򿪴ſ��豸
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  chenfm     20121220	created
*****************************************************************/
void mag1(void)
{
	int ret= -1, pnErrorCode=0;
	uchar pcSwiped= -1;
	char pszTk1[MAXTRACKLEN]={0}, pszTk2[MAXTRACKLEN]={0}, pszTk3[MAXTRACKLEN]={0};

	cls_printf("����%s...", TESTAPI); 

	//����ǰ�ã�
	NDK_MagClose();
#if !(defined ME30MH ||defined ME32||defined N920) //ME30MH�ſ��ǳ������ ���ܶ��쳣������ж�
	//case1:δ�򿪴ſ���ˢ����������Ӧ�ö�������
	cls_show_msg("��ˢ����[ENTER]����.....");
	if((ret=NDK_MagSwiped(&pcSwiped))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pcSwiped);
		GOTOERR;
	}
	if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2, "����������ͨ��!");
#endif
	//case2 :��һ�δ򿪴ſ��豸��ˢ���������ݣ�Ӧ�ɹ��������ݡ�
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !(defined ME15CHX||defined ME30MH||defined ME32||defined ME15B||defined N920)
	cls_show_msg("��ˢ����[ENTER]����..");
	if((ret=NDK_MagSwiped(&pcSwiped))!=NDK_OK||pcSwiped!=1)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pcSwiped);
		GOTOERR;
	}
#else
	cls_printf("��ˢ��..");
	while(1)
	{
		NDK_MagSwiped(&pcSwiped);
		if(pcSwiped==1)
			break;
	}
#endif
	if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
#if !(defined ME30MH ||defined ME32||defined N920) //ME30MH�ſ��ǳ������ ���ܶ��쳣������ж�
	//case3:�ظ��򿪣�Ӧ��ʧ�ܡ�
	if((ret=NDK_MagOpen())!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#endif
	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_MagClose();
}


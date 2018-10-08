/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: magģ��
* file name		: mag3.c
* Author 			: chenfm
* version			: 
* DATE			: 20121220
* directory 		: 
* description		: ����NDK_MagSwiped�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_MagSwiped"

/****************************************************************
* function name 	 	: void mag3(void)
* functional description 	: �ж��Ƿ�ˢ����
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  chenfm     20121220	created
*****************************************************************/
void mag3(void)
{
	int ret=-1;
	uchar pcSwiped=-1;

	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_MagClose();
#if !(defined ME30MH ||defined ME32||defined N920)
	//case1:δ�򿪴ſ��豸�󣬵��øýӿ�Ӧ����ʧ�ܡ�
	if((ret=NDK_MagSwiped(&pcSwiped))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case2:�򿪴ſ��豸���ж�ˢ���ṹӦ����0��δˢ������
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MagSwiped(&pcSwiped))!=NDK_OK||pcSwiped!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pcSwiped);
		GOTOERR;
	}  

	//case3:����ˢ����������ˢ��
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

	//case4:������Ĳ�����Ӧʧ�ܷ��ء�
	if((ret=NDK_MagSwiped(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_MagClose();
	return;
}


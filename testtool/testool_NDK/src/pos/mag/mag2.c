/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: magģ��
* file name		: mag2.c
* Author 			: chenfm
* version			: 
* DATE			: 20121220
* directory 		: 
* description		: ����NDK_MagClose�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_MagClose"

/****************************************************************
* function name 	 	: void mag2(void)
* functional description 	: �رմſ��豸
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  chenfm     20121220	created
*****************************************************************/
void mag2(void)
{
	int ret=-1;

	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_MagClose();
#if !(defined ME30MH ||defined ME32||defined N920)
	//case1 :��û�򿪴ſ��豸ǰ�����øýӿڹرմſ��豸��Ӧʧ�ܷ��ء�
	if((ret=NDK_MagClose())!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case2:�򿪴ſ��豸���ٹرմſ��豸Ӧ�óɹ����ء�
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MagClose())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if !(defined ME30MH ||defined ME32||defined N920)
	//case3:�ر��ѹرյĴſ��豸��Ӧ��ʧ�ܷ��ء�
	if((ret=NDK_MagClose())!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#endif

	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_MagClose();
	return;
}


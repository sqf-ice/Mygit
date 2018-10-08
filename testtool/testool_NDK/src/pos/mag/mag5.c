/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: magģ��
* file name		: mag5.c
* Author 			: chenfm
* version			: 
* DATE			: 20121220
* directory 		: 
* description		: ����NDK_MagReset�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_MagReset"

/****************************************************************
* function name 	 	: void mag5(void)
* functional description 	: ��λ��ͷ
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  chenfm     20121220	created
*****************************************************************/
void mag5(void)
{
	int ret=-1, pnErrorCode=0;
	uchar pcSwiped=-1;
	char pszTk1[MAXTRACKLEN]={0}, pszTk2[MAXTRACKLEN]={0}, pszTk3[MAXTRACKLEN]={0};

	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_MagClose();
#if !(defined ME30MH ||defined ME32||defined N920)
	//case1:��û�򿪴ſ��豸ǰ����λӦʧ�ܷ��ء�
	if((ret=NDK_MagReset())!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case2:�򿪴ſ��豸����λ��Ӧ�ɹ����ء�
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MagReset())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:ˢ���������ݣ���ˢ������λ���ٶ����ݣ�ˢ��״̬Ӧδˢ����Ӧ����������
#if !(defined ME15CHX||defined ME30MH||defined ME32||defined ME15B||defined N920)
	cls_show_msg("��ˢ123�ŵ��������󣬰�[ENTER]����..");
	if((ret=NDK_MagSwiped(&pcSwiped))!=NDK_OK||pcSwiped!=1)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pcSwiped);
		GOTOERR;
	}
#else
	cls_printf("��ˢ123�ŵ�������");
	while(1)
	{
		NDK_MagSwiped(&pcSwiped);
		if(pcSwiped==1)
			break;
	}
#endif
	if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode))!=NDK_OK||!strlen(pszTk1)||!strlen(pszTk2)||!strlen(pszTk3))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(2, "����������ͨ��!");
	cls_show_msg("��ˢ123�ŵ��������󣬰�[ENTER]����..");  
	if((ret=NDK_MagReset())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MagSwiped(&pcSwiped))!=NDK_OK||pcSwiped!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pcSwiped);
		GOTOERR;
	} 	
	if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode))!=NDK_ERR_NOSWIPED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	
#if !(defined ME30MH||defined ME32||defined N920)
	//case4:�رմſ��豸����λӦʧ�ܷ��ء�
	if((ret=NDK_MagClose())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MagReset())!=NDK_ERR)
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


/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: magģ��
* file name		: mag4.c
* Author 			: chenfm
* version			: 
* DATE			: 20121220
* directory 		: 
* description		: ����NDK_MagReadNormal�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

#define TESTAPI	"NDK_MagReadNormal"

extern int test_mag_errcode(int *perrcard1, int *perrcard2, int *perrcard3, int *perrcard4);

/****************************************************************
* function name 	 	: void mag4(void)
* functional description 	: ��ȡ�ſ���������1��2��3�ŵ�������
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		: author		date		remarks
*			  	  	  chenfm     20121220	created
*****************************************************************/
void mag4(void)
{
	int ret=-1, ret1=-3, ret2=-3, ret3=-3, pnErrorCode=0;
	int errcard1=0, errcard2=0, errcard3=0, errcard4=0;
	char pszTk1[MAXTRACKLEN]={0}, pszTk2[MAXTRACKLEN]={0}, pszTk3[MAXTRACKLEN]={0};
	char pszTk11[MAXTRACKLEN]={0}, pszTk21[MAXTRACKLEN]={0}, pszTk31[MAXTRACKLEN]={0};
	uchar pcSwiped=-1;

	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_MagClose();

	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1:δˢ����ֱ�Ӷ���Ӧʧ�ܷ��ء�
	if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode))!=NDK_ERR_NOSWIPED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_MagReset();
	
	//case7:�����ڽ����쳣����(����11#��)��,�ڽ�������������Ӧ��������ȡ
#if !(defined ME15CHX||defined ME30MH||defined ME32||defined ME15B||defined N920)
	cls_show_msg("��ˢ����һ���쳣����(����11#),��[ENTER]����..");
#else
	cls_printf("��ˢ����һ���쳣����(����11#)");
	while(1)
	{
		NDK_MagSwiped(&pcSwiped);
		if(pcSwiped==1)
			break;
	}
#endif
	if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pnErrorCode);
		GOTOERR;
	}
	NDK_MagReset();
	
	//case2:ˢһ��123�ſ���Ӧ�ɹ����ء�
#if !(defined ME15CHX||defined ME30MH||defined ME32||defined ME15B||defined N920)
	cls_show_msg("��ˢ123�ŵ�����ˢ�꿨��[ȷ��]����..");
#else
	cls_printf("��ˢ123�ŵ���");
	while(1)
	{
		NDK_MagSwiped(&pcSwiped);
		if(pcSwiped==1)
			break;
	}
#endif
	if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode))!=NDK_OK||(pnErrorCode!=0&&pnErrorCode!=3073))//ME11��ME15Ӳ���ϲ�֧��1�ŵ�����˷ſ�����������3073 20141015linwl
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pnErrorCode);
		GOTOERR;
	} 
	//�����˹�checkһ�κ���Ҫ!�ɼ��7e��7f�����
#if !TK1_DISABLE	//Ӳ����֧��1�ŵ��ż��1�ŵ�����
	if((ret=check_stripe_result_tk1(pszTk1))!=0x00)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif	
	if((ret=check_stripe_result_tk2(pszTk2))!=0x00)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=check_stripe_result_tk3(pszTk3))!=0x00)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_MagReset();

	//case3: ˢ��һ��123�ŵ���������Ӧ��һ�¡�
#if !(defined ME15CHX||defined ME30MH||defined ME32||defined ME15B||defined N920)
	cls_show_msg("��ˢ��һ��123�ŵ�����ˢ�꿨��[ȷ��]����..");   
#else
	cls_printf("��ˢ��һ��123�ŵ���");
	while(1)
	{
		NDK_MagSwiped(&pcSwiped);
		if(pcSwiped==1)
			break;
	}
#endif
	if((ret=NDK_MagReadNormal(pszTk11, pszTk21, pszTk31, &pnErrorCode))!=NDK_OK||(pnErrorCode!=0&&pnErrorCode!=3073))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pnErrorCode);
		GOTOERR;
	}
	//�����˹�checkһ�κ���Ҫ!�ɼ��7e��7f�����
#if !TK1_DISABLE	//Ӳ����֧��1�ŵ��ż��1�ŵ�����
	if((ret=check_stripe_result_tk1(pszTk11))!=0x00)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	if((ret=check_stripe_result_tk2(pszTk21))!=0x00)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=check_stripe_result_tk3(pszTk31))!=0x00)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}    
	if(((ret1=strcmp(pszTk1, pszTk11))==0)&&((ret2=strcmp(pszTk2, pszTk21))==0)&&((ret3=strcmp(pszTk3, pszTk31))==0))
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret1, ret2, ret3);
		GOTOERR;
	}
	NDK_MagReset();
	if((ret=NDK_MagReadNormal(pszTk11, pszTk21, pszTk31, &pnErrorCode))!=NDK_ERR_NOSWIPED)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pnErrorCode);
		GOTOERR;
	}
	NDK_MagReset();

	//case4 :������ΪNULL��ˢһ��123�ŵ�����Ӧ�ɹ���
#if !(defined ME15CHX||defined ME30MH||defined ME32||defined ME15B||defined N920)
	cls_show_msg("��ˢ123�ŵ�����ˢ�꿨��[ȷ��]����..");
	if((ret=NDK_MagSwiped(&pcSwiped))!=NDK_OK||pcSwiped!=1)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pcSwiped);
		GOTOERR;
	}
#else
	cls_printf("��ˢ123�ŵ���");
	while(1)
	{
		NDK_MagSwiped(&pcSwiped);
		if(pcSwiped==1)
			break;
	}
#endif
	if((ret=NDK_MagReadNormal(NULL, NULL, NULL, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_MagReset();
	
#if !(defined ME30MH||defined ME32||defined N920)
	//case5:�رմſ��豸��ˢ�������ſ���Ӧʧ�ܷ��ء�
	if((ret=NDK_MagClose())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	} 
	NDK_MagReset();
#endif

	//case6:
	//����15�������ſ���ר�����for �ſ���errcode
	//errcard1����HEX��ʽ��ʾ��BIT14---BIT0�ֱ��ʾ15��---1�ſ�����ˢ2�ŵ�(bit15����),��BIT30---BIT16�ֱ��ʾ15��---1�ſ�����ˢ3�ŵ�(bit31����),��������ӦBIT�ͻ���1
	//errcard2,errcard3,errcard4ͬ��
	//eg.��ˢ11#��,2�ŵ�������,����0x0800
	if(FAIL==test_mag_errcode(&errcard1, &errcard2, &errcard3, &errcard4))
	{
		if(errcard1!=0||errcard2!=0||errcard3!=0||errcard4!=0)//�ǰ�ESC�Թ����Ե����
		{
			send_result("%s���Դ��ڴ���(����:0x%08x,����:0x%08x,����:0x%08x,�췴:0x%08x)", TESTAPI, errcard1, errcard2, errcard3, errcard4);
			goto ERR;
		}
	}    

	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_MagClose();
	return;
}


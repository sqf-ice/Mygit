/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys40.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ����NDK_SysSetWeakupDelay�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysSetWeakupDelay"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		         sull             20180709              created
*****************************************************************/
void sys40(void)
{
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ�����,��ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	
	//����Ϊ�ֲ�����
	int nRet = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);  

	//case5:���븺��������һ���ܴ������
	cls_show_msg("���������������,���ߺ�ȴ�1�����ֶ�����,�ֶ����Ѻ�������");
	if((nRet=NDK_SysSetWeakupDelay(-1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	if((nRet=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	NDK_LedStatus(LED_RED_X_ON/*LED_RFID_RED_ON*/);
	if(cls_show_msg("�������ߺ��ʱ���ڲ����Զ�����?��[ENTER],��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}	

	//case1:��������10s���ܹ��Զ�����
	cls_show_msg("���������������,10s��Ӧ���Զ�������������������");
	if((nRet=NDK_SysSetWeakupDelay(10)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	if((nRet=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	NDK_LedStatus(LED_BLUE_X_ON/*LED_RFID_BLUE_ON*/);
	if(cls_show_msg("��������10s���Զ�����,����������?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case2:δ�������߻���ʱ�䣬Ӧֻ��ͨ���ֶ�����
	cls_show_msg("���������������,�ȴ�1���Ӻ��������������,���ѵ�ʱ����Ӧ������");
	if((nRet=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	NDK_LedStatus(LED_RED_X_ON/*LED_RFID_RED_ON*/);
	if(cls_show_msg("�������ߺ󲻻��Զ�����,ֻ���ֶ�����?��[ENTER],��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
		
	//case3:�������߻���ʱ��Ϊ0s��Ӧ��Ч
	cls_show_msg("���������������,�������ߺ�ȴ�10s���������������,����ʱ��������");
	if((nRet=NDK_SysSetWeakupDelay(0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	if((nRet=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	NDK_LedStatus(LED_BLUE_X_ON/*LED_RFID_BLUE_ON*/);
	if(cls_show_msg("�������ߺ��谴�����ܻ���,�һ���ʱ��������?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
		
	//case4:�������߻���ʱ��Ϊ5���ӣ�5���Ӻ�Ӧ���Զ�����
	cls_show_msg("���������������,���ĵȴ�5���ӣ�5���Ӻ�Ӧ���Զ�������������������");
	if((nRet=NDK_SysSetWeakupDelay(5*60)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	if((nRet=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	NDK_LedStatus(LED_BLUE_X_ON/*LED_RFID_BLUE_ON*/);
	if(cls_show_msg("��������5���Ӻ��Զ�����,����������?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}	


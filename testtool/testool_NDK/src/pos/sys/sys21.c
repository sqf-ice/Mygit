/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys21.c
* Author 			: jiangym
* version			: 
* DATE				: 20130926
* directory 			: 
* description			: ����NDK_SysSetSuspendDuration�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysSetSuspendDuration"

#if CPU5892_ENABLE||CPU5830X_ENABLE||OVERSEAS_ENABLE	//589Xƽ̨��SP60���ͣ���ʱ���ѵľ��Ƚϵͣ������128�����ҡ� ������Ҫ���ô���128��ʱ�������Ч,С��128��ʱ��Ч�����
#define WAKETIME	(180)
#else
#define WAKETIME	(60)
#endif

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
*			  		     
*****************************************************************/
void sys21(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
	int ret = 0;

	//����Ϊ������
	//����ǰ��
	if(ENTER!=cls_show_msg("����%s,���ڹ����зֱ�����[����/��ǳ]ģʽ��ENTER��������", TESTAPI))
	{
		send_result("line %d:%s�����ж�", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg("��ֱ��ֻ���Դ��ֻ�е�������������(5892������ʱ�����128�����ڵ����),���������");

	//case1:�쳣����:��unSecС��60���ʱ�򣬽����ش���
	if((ret=NDK_SysSetSuspendDuration(1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetSuspendDuration(59))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetSuspendDuration(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:���������߻���ʱ��ΪWAKETIME��,������,������������60���ỽ��(����������RTC���߻��ѹ���)
	cls_show_msg1(3,"3s�������,���ߺ󲻰���,%ds���Զ�����", WAKETIME);
	if((ret=NDK_SysSetSuspendDuration(WAKETIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(3,"�ѻ���,3s���ٴν�������,%ds���Զ�����", WAKETIME);
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	if(cls_show_msg("�������ξ�%ds���Զ�����,�һ���ʱ������,��ȷ[Enter],����[����]", WAKETIME)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_SysSetSuspendDuration(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:���������߻���ʱ��ΪWAKETIME��,���������߻���ʱ��Ϊ0,������,������������60��󲻻ỽ��(���������0ʵ��ʵʱȡ�����߶�ʱ���ѹ���)
	cls_show_msg1(3,"3s�������,�ȴ�%ds�󲻻��Զ�����,��Ҫ��������,����ʱ��Ļ��ˢ��", WAKETIME);
	if((ret=NDK_SysSetSuspendDuration(WAKETIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetSuspendDuration(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	if(cls_show_msg("��Ļ��ˢ��,����%ds���ް���������,��ȷ[Enter],����[����]", WAKETIME)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case4:���������߻���ʱ��ΪWAKETIME+10��,������,��������,�ٽ�����,�������ỽ��
	cls_show_msg1(3,"3s�������,����%ds���밴������,����ʱ��Ļ��ˢ��", WAKETIME+10);
	if((ret=NDK_SysSetSuspendDuration(WAKETIME+10))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	cls_show_msg("�ѻ���,��Ļ��ˢ��,�������[ȷ�ϼ�����]��������,%ds���Զ�����", WAKETIME+10);
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	if(cls_show_msg("���ߺ�%ds�ް����ỽ��,����ʱ������,��ȷ[Enter],����[����]", WAKETIME+10)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_SysSetSuspendDuration(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:���������߻���ʱ��ΪWAKETIME��,�ȴ��Զ���������,������������60���ỽ��(����������RTC���߻��ѹ���)
	if((ret=NDK_SysSetSuspendDuration(WAKETIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("�ȴ��Զ���������,���ߺ󲻰���,%ds���Զ�����,���Ѻ󰴼�����", WAKETIME);
	cls_show_msg("�ȴ��ٴν�������,���ߺ󲻰���,%ds���Զ�����,���Ѻ󰴼�����", WAKETIME);
	if(cls_show_msg("�������ξ�%ds���Զ�����,��ȷ[Enter],����[����]", WAKETIME)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_SysSetSuspendDuration(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6:���������߻���ʱ��ΪWAKETIME��,���������߻���ʱ��Ϊ0,�ȴ��Զ���������,������������60��󲻻ỽ��(���������0ʵ��ʵʱȡ�����߶�ʱ���ѹ���)	
	if((ret=NDK_SysSetSuspendDuration(WAKETIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetSuspendDuration(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("�ȴ��Զ���������,�����������Զ�����,�������Ѻ󰴼�����");
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	if(cls_show_msg("����%ds���ް���������,��ȷ[Enter],����[����]", WAKETIME)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case7:���������߻���ʱ��ΪWAKETIME+10��,�ȴ��Զ���������,��������,�ٵȴ��Զ���������,�������ỽ��
	if((ret=NDK_SysSetSuspendDuration(WAKETIME+10))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("�ȴ��Զ���������,����%ds���밴������,�������Ѻ󰴼�����", WAKETIME+10);
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	cls_show_msg("�ѻ���,�ȴ��ٴν�������,%ds���Զ�����,���Ѻ󰴼�����", WAKETIME+10);
	if(cls_show_msg("���ߺ��ް����ỽ��,��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_SysSetSuspendDuration(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_SysSetSuspendDuration(0);
	return;
}


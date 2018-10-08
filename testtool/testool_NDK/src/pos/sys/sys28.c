/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys28.c
* Author 			: zhengry
* version			: 
* DATE				: 20150727
* directory 		: 
* description		: ��������NDK_SysWakeUp���߻��ѹ���
* related document	: NDK.chm
*
************************************************************************/ 
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysWakeUp"	 
#define WAITTIME 30
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  zhengry       20150727       created
*****************************************************************/
void sys28()//�޲����޷���
{
	/*private & local definition*/
	int ret=0;
	
	if(auto_flag==1)
	{
		//send_result("line %d:%s�޴��Զ���������", __LINE__, TESTAPI);
		return;
	}
	/*process body*/
	cls_printf("����%s...", TESTAPI);  

	//����ǰ�� 

	//case1:3s��������ߣ����û��Ѻ�����Ӧ����������
	cls_show_msg1(3, "3s���������,��ȴ�5s�󣬽��Զ�����");
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	//NDK_SysDelay(5);//����ndk.so�е��øú����ᷢ��ָ���k21��Ҳ�ỽ������,���Բ����øú���
	usleep(5000000);
	if((ret=NDK_SysWakeUp()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if(cls_show_msg("�ѻ���,û���κΰ���Ҳ�����߻���,��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}

	if(cls_show_msg("�Ƿ��ѿ���:K21��ʱ�䲻����,��׿�˻�������ߺ�����K21�������ߵĹ���,�ǰ�[ENTER],��������")!=ENTER)
	{
		send_result("%s�û�ȡ������", TESTAPI);
		return;
	}
	
	//case2:��POS�Զ����ߣ��ȴ����ߺ󣬵��û��Ѻ�����Ӧ����������
	cls_show_msg("�ȴ�K21�Զ���������,���ߺ����������,5s���Զ�����");//�˴�N900AZ��K21�˻���30��(��׿������)���ڰ�׿��ܲ��ֵ���NDK_SysGoSuspend�����������ߣ��ù��ܿ����������Զ����ߣ���ʵ�ʻ��Ǻ�������
	//NDK_SysDelay(5);
	usleep(5000000);
	if((ret=NDK_SysWakeUp()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if(cls_show_msg("�ѻ���,��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return;
	}

	//case3:���߻��Ѻ�,IC��/��Ƶ/��ӡ/��������������������

	send_result("%s����ͨ��", TESTAPI);
	return;	
}


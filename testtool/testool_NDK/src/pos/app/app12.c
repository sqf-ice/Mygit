/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: appģ��
* file name		: App12.c
* Author 		:
* version		: 
* DATE			:
* directory 		: 
* description	: ����NDK_AppStartChildProcess�ܷ��������к�̨������� ����ȡ����ֵ
				�Լ���NDK_AppGetDatafromParentProcess,NDK_AppChildProcessExit֮���ܷ�����ͨѶ
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_AppStartChildProcess,NDK_AppGetDatafromParentProcess,NDK_AppChildProcessExit"
#define   SUBAPP1          "subapp1"
#define   SUBAPP2         "subapp2"
#define   ERRPATH         "err"
#define   APPNAME       "SUBAPP"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : ģ���12������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*						  jiangym			20170616     created
*****************************************************************/
void app12(void)
{
	/*private & local definition*/
	int  ret = -1, len = 0, data = 0;
	AppExitStatusT  *appstatus;
	char text[] = "�����ӽ���2";
		
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}

	if(cls_show_msg1(30, "����%s...��ȷ�������ļ�%s,%s����,%s.NLD������,ESC�˳�", TESTAPI,SUBAPP1,SUBAPP2,APPNAME)==ESC)
		return;//��Ļ��ʾ��ǰ��������ģ��������
		
	//case1:�����쳣����
	if(( ret =NDK_AppStartChildProcess(NULL,NULL,0,&appstatus))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(( ret =NDK_AppStartChildProcess(SUBAPP1,NULL,0,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(( ret =NDK_AppGetDatafromParentProcess(NULL,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(( ret =NDK_AppGetDatafromParentProcess(&data,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(( ret =NDK_AppChildProcessExit(0,NULL,len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case2:��Ӧ�ò�����ʱӦ�÷���ʧ�� ������ȷ�ϴ˲��Ե���Բ���

	//case3:��������:��������Ӧ���޲��� ��Ӧ�÷��صĲ����е���������Ϊ0
	cls_show_msg("��ȷ����ǰĿ¼�µ���Ӧ��%s��%s���޸�Ϊ��ִ���ļ�",SUBAPP1,SUBAPP2);
	if(( ret =NDK_AppStartChildProcess(SUBAPP1,NULL,0,&appstatus))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�ж���Ӧ�û�ȡ�������̵Ĳ����Ƿ���ȷ
	if(cls_show_msg("�ѻص�������,�ӽ����Ƿ�����ʾ:�յ������̴���������Ϊ0,��ȷ��[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//�������ж���Ӧ�÷��صĲ���:�ɹ��˳����ؿղ����Լ�����Ϊ0
	if(appstatus->code != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, appstatus->code);
		RETURN;
	}
	if(appstatus->cbData != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, appstatus->cbData);
		RETURN;
	}

	//case4:��������Ӧ���д�����, �����̴���������Ӧ��
	data = 10;
	if(( ret =NDK_AppStartChildProcess(SUBAPP2,&data,sizeof(int),&appstatus))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�ж���Ӧ�û�ȡ�������̵Ĳ����Ƿ���ȷ
	if(cls_show_msg("�ѻص�������,�ӽ����Ƿ�����ʾ:�յ������̴���������Ϊ%d,��ȷ��[Enter],����[����]",data)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//�������ж���Ӧ�÷��صĲ���:�ɹ��˳���������Ч�Ĳ���
	if(appstatus->code != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, appstatus->code);
		RETURN;
	}
	if(strcmp(appstatus->data,text))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, appstatus->data);
		RETURN;
	}
	if(appstatus->cbData != 12)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, appstatus->cbData);
		RETURN;
	}

	//case5:������������Ӧ��·��������Ӧ�������ļ���,�������д���������Ӧ��,��Ӧ�÷��ؿյĲ���
	data = 20;
	if(( ret =NDK_AppStartChildProcess(APPNAME,&data,sizeof(int),&appstatus))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�жϻ�ȡ������Ӧ�õĲ����Ƿ���ȷ
	if(cls_show_msg("�ѻص�������,�ӽ����Ƿ�����ʾ:�յ������̴���������Ϊ%d,��ȷ��[Enter],����[����]",data)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//�������ж���Ӧ�÷��صĲ���:�ɹ��˳���������Ч�Ĳ���
	if(appstatus->code != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, appstatus->code);
		RETURN;
	}
	if(appstatus->cbData != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, appstatus->cbData);
		RETURN;
	}

	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);	
	return;
}

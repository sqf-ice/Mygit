/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys9.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysSetSuspend�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysSetSuspend"
#define	MAXWAITTIME 60

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
* history 		 		: author			date			remarks
*			  		        linwei	           20130121	 	created
*****************************************************************/
void sys9(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
	int ret = -1;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//case1: �쳣���ԣ��������
	if((ret=NDK_SysSetSuspend(2)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:����ͬϵͳ���ò���
	if(cls_show_msg("֧��ʹ�õ�ز�ȷ��û���Դ,[ȷ��]����,[����]����") != ENTER)
	{
		send_result("line %d:%s��������", __LINE__, TESTAPI);
		GOTOERR;
	}
	ret=cls_show_msg("ѡ�����������״̬:\n1.�Ӳ�����\n2.����ʱ��20��");//�������������ߺͲ����߿�����֤NDK_SysSetSuspend�������������е�����ģʽӰ��
	switch(ret)
	{
	case '1':
		//case2.1: �������Զ���������
		if((ret=NDK_SysSetSuspend(0)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if (cls_show_msg1(30, "�벻Ҫ�����ȴ�30��,��Ӧ��������.����������,��ΪBUG") != 0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		cls_show_msg1(2, "����������ͨ��!");
		
		//case2.2: �����Զ���������
		if((ret=NDK_SysSetSuspend(1)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if (cls_show_msg1(30, "�벻Ҫ�����ȴ�30��,��Ӧ��������.����������,��ΪBUG") != 0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		
		break;
	case '2':	
		//case2.3: �������Զ��������� 20140603 modified by jiangym
		if((ret=NDK_SysSetSuspend(0)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if (cls_show_msg1(25, "1.�벻Ҫ�����ȴ�25��,��Ӧ��������.����������,��ΪBUG") != 0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		//case2.4:�������β������Զ���������
		if((ret=NDK_SysSetSuspend(0)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if (cls_show_msg1(25, "2.�벻Ҫ�����ȴ�25��,��Ӧ��������.����������,��ΪBUG") != 0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		//case2.5:�����Զ��������ߣ�Ӧ�ÿ��Խ�������˵�����һ��������Ч������Ҫ�ɶ�ʹ�øú��� 
		if((ret=NDK_SysSetSuspend(1)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg1(25,"1.�벻�����ȴ�20��,Ӧ��������.����������,���ϰ�������");
		NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
		if (cls_show_msg("�ڵ�20��ʱ�Ƿ��������,��[ȷ��],����[����]") != ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		//case2.6:�ٵ���һ�������Զ���������
		if((ret=NDK_SysSetSuspend(1)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_show_msg1(25,"2.�벻�����ȴ�20��,Ӧ��������.����������,���ϰ�������");
		NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
		if (cls_show_msg("�ڵ�20��ʱ�Ƿ��������,��[ȷ��],����[����]") != ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		//case2.7:��֤��ʱ�����㹦�� 20140610  add  jiangym
		cls_printf("���ĵȴ�18s,�ȴ����̲�Ӧ��������(����������,��ΪBUG)");
		sleep(18);
		if((ret=NDK_SysSetSuspend(0)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_SysSetSuspend(1)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		cls_printf("���ĵȴ�5s,�ȴ����̲�Ӧ��������(����������,��ΪBUG)");
		sleep(5);
		if(ENTER!=cls_show_msg("5��ĵȴ������в�������,��[ȷ��],����[����]"))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		//case2.8:�������Զ���������Ӧ����Ч
		if((ret=NDK_SysSetSuspend(0)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if (cls_show_msg1(30, "3.�벻Ҫ�����ȴ�30��,��Ӧ��������.����������,��ΪBUG") != 0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		break;
	default:
		send_result("line %d:%s��������", __LINE__, TESTAPI);
		RETURN;
	}
	//���Խ���
	send_result("%s����ͨ��(��������������״̬,�زⱾ����)", TESTAPI);
	goto ERR;
ERR:
	NDK_SysSetSuspend(1);//����������߿��أ������˳������Ժ��޷��Զ����ߣ���������жϴ��� 20140509 linwl
	return;
}


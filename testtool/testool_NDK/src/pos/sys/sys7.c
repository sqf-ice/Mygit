/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys7.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysSetBeepVol,NDK_SysGetBeepVol�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysSetBeepVol,NDK_SysGetBeepVol"

#define MAXVOL 5
#define	MAXWAITTIME 30

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
*			  		        linwei	           20130121	 	created
*****************************************************************/
void sys7(void)
{
	if(auto_flag==1)
		send_result("%s�Զ����Բ�����Ϊ���ղ��Խ���������ֶ�������֤", TESTAPI);

	//����Ϊ�ֲ�����
	int i,ret = -1,nKeyin;
	unsigned int unVolumn=0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//case1: �쳣���ԣ��������
	if((ret=NDK_SysSetBeepVol(MAXVOL*10)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysGetBeepVol(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:��������Ϊ4,����������Ϊ1,��ȡ�������Ƿ�Ϊ1 20140603 added by jiangym
	if((ret=NDK_SysSetBeepVol(4)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysSetBeepVol(1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysGetBeepVol(&unVolumn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(unVolumn != 1)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2: ��������Ϊ0��5
	for (i=0; i<=MAXVOL; i++)
	{
		if((ret=NDK_SysSetBeepVol(i)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret=NDK_SysGetBeepVol(&unVolumn)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if(unVolumn != i)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysBeep();
		NDK_SysMsDelay(200);
	}

	nKeyin = cls_show_msg1(MAXWAITTIME, "����������С����,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	return;
}


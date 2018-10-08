/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util9.c
* Author 			: jiangym
* version			:
* DATE				: 20130108
* directory 	:
* description	: ���� NDK_CalcLRC    �ӿڹ���
* related document	: NDK.chm
*
************************************************************************
* log			:
* Revision
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_CalcLRC"

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
* history 		 		:
*****************************************************************/
void util9(void)
{
	//����Ϊ�ֲ�����
	uchar sbuf[4] = {0x31,0x15,0x14,0x21};
	int len = 4, ret = 0;
	uchar lrc = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣����:��������ΪNULL�����Ӧ����ʧ��
	if((ret=NDK_CalcLRC(NULL,len,&lrc))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_CalcLRC(sbuf,-1,&lrc))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_CalcLRC(sbuf,len,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:��������:psBuf[4]="0x31 0x15 0x14 0x21"������Ϊ4��������Ϊ0x11,��λ������㲻��λ
	if((ret=NDK_CalcLRC(sbuf,4,&lrc))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(lrc!=0x11)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}


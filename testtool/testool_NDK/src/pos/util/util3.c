/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util3.c
* Author 			: jiangym
* version			:
* DATE				: 20130108
* directory 	:
* description	: ����NDK_FmtAmtStr  �ӿڹ���
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
#define 	TESTAPI		"NDK_FmtAmtStr"

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
void util3(void)
{
	//����Ϊ�ֲ�����
	uchar target[16] = {0};
	int len = 30, ret = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������
					 
	//case1:�쳣����:��������ΪNULL�����Ӧ����ʧ��
	if((ret=NDK_FmtAmtStr(NULL,target,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_FmtAmtStr((uchar *)"75.36",NULL,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_FmtAmtStr((uchar *)"75.36",target,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:pszSource�ַ�����ʽ����ȷ����75.1.2������ʧ��
	len = sizeof("75.1.2")+1;
	if((ret=NDK_FmtAmtStr((uchar *)"75.1.2",target,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}  

	//case3:���볤��С�������ַ����ĳ��ȣ�����ʧ��
	len = 0;
	if((ret=NDK_FmtAmtStr((uchar *)"75.12",target,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:pszSourceΪ.987,���䷵��ֵΪ?
	len = sizeof(".987")+1;
	if((ret=NDK_FmtAmtStr((uchar *)".987",target,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case4:��������:pszSourceΪ755.153�����䷵��ֵΪ75515,����NDK_OK
	len = sizeof("755.153")+1;
	if((ret=NDK_FmtAmtStr((uchar *)"755.153",target,&len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("755153",(char *)target))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}


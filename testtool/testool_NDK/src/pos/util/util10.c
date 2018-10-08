/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util10.c
* Author 			: jiangym
* version			:
* DATE				: 20130109
* directory 	:
* description	: ���� NDK_LeftTrim  ��NDK_RightTrim ��NDK_AllTrim   �ӿڹ���
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
#define 	TESTAPI		"NDK_LeftTrim,NDK_RightTrim,NDK_AllTrim"

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
void util10(void)
{
	//����Ϊ�ֲ�����
	uchar zbuf[10] = {0};
	int ret = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣���:pszBufΪNULL,��Ӧ����ʧ��
	if((ret=NDK_LeftTrim(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_RightTrim(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AllTrim(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�������:�пո��pszBufΪ"  ab  cd  "������NDK_LeftTrim������Ϊ"ab  cd  "
	strcpy((char *)zbuf,"  ab  cd  ");
	if((ret=NDK_LeftTrim(zbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("ab  cd  ",(char *)zbuf))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case3:�пո��pszBufΪ"  ab  cd  ",NDK_RightTrim������Ϊ"  ab  cd"
	strcpy((char *)zbuf,"  ab  cd  ");
	if((ret=NDK_RightTrim(zbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("  ab  cd",(char *)zbuf))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case4:�пո��pszBufΪ"  ab  cd  ",NDK_AllTrim������Ϊ"ab  cd"
	strcpy((char *)zbuf,"  ab  cd  ");
	if((ret=NDK_AllTrim(zbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("ab  cd",(char *)zbuf))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}


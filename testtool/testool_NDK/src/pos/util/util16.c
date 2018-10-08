/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util16.c
* Author 			: jiangym
* version			:
* DATE				: 20130109
* directory 	:
* description	: ����NDK_IsValidDate   �ӿڹ���
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
#define 	TESTAPI		"NDK_IsValidDate"

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
*			  	  	     jiangym         20130110  	   created
*****************************************************************/
void util16(void)
{
	//����Ϊ�ֲ�����
	int ret = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣����:pszDateΪNULL,Ӧ����ʧ��
	if((ret=NDK_IsValidDate(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:pszDate�ַ�����ʽΪYYYYMMDD�������Ĳ��������ʽ�ģ�����ʧ��
	if((ret=NDK_IsValidDate((uchar *)"201304"))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:pszDateΪ19990733����19008927����19cd2305������ʧ��
	if((ret=NDK_IsValidDate((uchar *)"19990733"))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_IsValidDate((uchar *)"19cd2305"))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case4:pszDate ����һ����Χ�ڵ�����12120304������ʧ��
	if((ret=NDK_IsValidDate((uchar *)"12120304"))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case5:����ͷ�����2��29�յĲ���
	//case5.1:��������:pszDateΪ20060229�����سɹ�NDK_ERR_PARA
	if((ret=NDK_IsValidDate((uchar *)"20060229"))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case5.2:��������:pszDateΪ20080229�����سɹ�NDK_OK 
	if((ret=NDK_IsValidDate((uchar *)"20080229"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case6:��������:pszDateΪ20150104�����سɹ�NDK_OK 
	if((ret=NDK_IsValidDate((uchar *)"20150104"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}


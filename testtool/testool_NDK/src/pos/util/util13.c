/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util13.c
* Author 			: jiangym
* version			:
* DATE				: 20130109
* directory 	:
* description	: ����NDK_IsDigitChar ��NDK_IsDigitStr   �ӿڹ���
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
#define 	TESTAPI		"NDK_IsDigitChar,NDK_IsDigitStr "

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
void util13(void)
{
	//����Ϊ�ֲ�����
	int ret = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣����:chΪNULL,Ӧ����ʧ��
	if((ret=NDK_IsDigitChar(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:chΪa,��ôӦ�÷���ʧ��
	if((ret=NDK_IsDigitChar('a'))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:��������:chΪ3,���سɹ�
	if((ret=NDK_IsDigitChar('3'))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:�쳣����:pszStringΪNULL,Ӧ����ʧ��
	if((ret=NDK_IsDigitStr(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case5:pszStringΪabc23de,��Ӧ����ʧ��
	if((ret=NDK_IsDigitStr((uchar *)"abc23de"))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case6:��������:pszStringΪ"123456",�򷵻سɹ�NDK_OK 
	if((ret=NDK_IsDigitStr((uchar *)"123456"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	send_result("%s����ͨ��", TESTAPI);
	return;
}


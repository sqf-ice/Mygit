/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util1.c
* Author 			: jiangym
* version			:
* DATE				: 20130107
* directory 	:
* description	: ����NDK_AddDigitStr�ӿڹ���
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
#define 	TESTAPI		"NDK_AddDigitStr"

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
void util1(void)
{
	//����Ϊ�ֲ�����

	uchar digstr1[14] = {0}, digstr2[14] = {0}, resultstr[14] = {0};
	int len = 13, ret = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣����:��������ΪNULL�����Ӧ����ʧ��
	if((ret=NDK_AddDigitStr(NULL, digstr2, resultstr, &len))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AddDigitStr(digstr1, NULL, resultstr, &len))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AddDigitStr(digstr1, digstr2, NULL, &len))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AddDigitStr(digstr1, digstr2, resultstr, NULL))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:pszDigStr1�������ȳ���12����pszDigStr2�������ȳ���12��Ӧ����ʧ��
	if((ret=NDK_AddDigitStr((uchar *)"1234567890123", (uchar *)"1234", resultstr, &len))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AddDigitStr((uchar *)"1234", (uchar *)"1234567890123", resultstr, &len))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:pszDigStr1������pszDigStr2�������Ϸ���123ABC,����ʧ��
	if((ret=NDK_AddDigitStr((uchar *)"123ABC", (uchar *)"1234", resultstr, &len))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AddDigitStr((uchar *)"1234", (uchar *)"123abc", resultstr, &len))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:������֮��pszResult����12���������ʧ��
	memset(resultstr,0,sizeof(resultstr));
	len = 13;
	if((ret=NDK_AddDigitStr((uchar *)"653224567821", (uchar *)"653224567821", resultstr, &len))!=NDK_ERR) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case5:��ʼ�� lenΪ5С�ڼ�������ĳ���6,Ӧ�÷��ز�������
	memset(resultstr,0,sizeof(resultstr));
	len = 5;
	if((ret=NDK_AddDigitStr((uchar *)"1234",(uchar *) "123456", resultstr, &len))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case6:��������
	memset(resultstr,0,sizeof(resultstr));
	len = 6;
	if((ret=NDK_AddDigitStr((uchar *)"1234", (uchar *)"123456", resultstr, &len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("124690",(char *)resultstr)||len!=6)
	{
		send_result("line %d:%s����ʧ��%s,%d", __LINE__, TESTAPI, resultstr, len);
		RETURN;
	}
	
	//case7:��������12λ����12λ���õ�12λ��
	memset(resultstr,0,sizeof(resultstr));
	len = 12;
	if((ret=NDK_AddDigitStr((uchar *)"111122221111", (uchar *)"222233332222", resultstr, &len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("333355553333",(char *)resultstr)||len!=12)
	{
		send_result("line %d:%s����ʧ��%s,%d", __LINE__, TESTAPI, resultstr, len);
		RETURN;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

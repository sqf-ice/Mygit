/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util2.c
* Author 			: jiangym
* version			:
* DATE				: 20130107
* directory 	:
* description	: ����NDK_IncNum�ӿڹ���
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
#define 	TESTAPI		"NDK_IncNum"

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
void util2(void)
{
	//����Ϊ�ֲ�����
	uchar strnum[10] = {0};
	int ret = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ������� 	

	//case1:�쳣����:pszStrNum����ΪNULL,Ӧ����ʧ��
	if((ret=NDK_IncNum(NULL))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:pszStrNum�������ȴ���6����1234567��Ӧ����ʧ��
	strcpy((char *)strnum,"1234567");
	if((ret=NDK_IncNum(strnum))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:pszStrNum�������Ϊ��������ĸ�������123hf,Ӧ�÷���ʧ��
	strcpy((char *)strnum,"123hf");
	if((ret=NDK_IncNum(strnum))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)(%s)", __LINE__, TESTAPI, ret, strnum);
		RETURN;
	}
	
	//case4:pszStrNum��������С��6��Ӧ���سɹ�
	strcpy((char *)strnum,"12345");
	if((ret=NDK_IncNum(strnum))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("012346",(char *)strnum))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//case4:��������:pszStrNumΪ999999������ֵ��000001������NDK_OK
	strcpy((char *)strnum,"999999");
	if((ret=NDK_IncNum(strnum))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("000001",(char *)strnum))
	{
		send_result("line %d:%s����ʧ��%s", __LINE__, TESTAPI, strnum);
		RETURN;
	}
	
	//case5:��������:pszStrNumΪ123456������ֵ��123457������NDK_OK
	strcpy((char *)strnum,"123456");
	if((ret=NDK_IncNum(strnum))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("123457",(char *)strnum))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

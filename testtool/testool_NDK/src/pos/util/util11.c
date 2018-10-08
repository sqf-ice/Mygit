/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util11.c
* Author 			: jiangym
* version			:
* DATE				: 20130109
* directory 	:
* description	: ����NDK_AddSymbolToStr �ӿڹ���
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
#define 	TESTAPI		"NDK_AddSymbolToStr"

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
void util11(void)
{
	//����Ϊ�ֲ�����
	uchar zstr[16] = {0};
	int len = 10;
	uchar ch = 'A';
	int op = 0, ret = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣����:����ΪNULL�����Ӧ����ʧ��
	if((ret=NDK_AddSymbolToStr(NULL,len,ch,op))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:nlenΪ-1�����Ӧ����ʧ��
	if((ret=NDK_AddSymbolToStr(zstr,-1,ch,op))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:pszString �ַ�������>nlen�����Ӧ����ʧ��
	strcpy((char *)zstr,"12ab34de7");
	if((ret=NDK_AddSymbolToStr(zstr,strlen((char *)zstr)-1,ch,op))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

#if 0
	//case4:pszString����ĳ���<nlen�����ʵ�ʻᲹ���ַ������������Ⱥ�ɹ�����
	//���case�Ƿ���Ҫ����??// ����������ܻ�������������쳣����Ϊ������������θ����� 20150125 linwl
	strcpy((char *)zstr,"12ab34de7");
	if((ret=NDK_AddSymbolToStr(zstr,sizeof(zstr)+1,ch,op))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, zstr);
		RETURN;
	}
#endif

	//case5:nOption����Ϊ3 ��-1��Ӧ����ʧ��
	if((ret=NDK_AddSymbolToStr(zstr,len,ch,3))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AddSymbolToStr(zstr,len,ch,-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case5:��������:pszString[9]Ϊ"ab12cde",chΪA,�ֱ����nOptionΪ0,1,2�������
	strcpy((char *)zstr,"ab12cde");
	if((ret=NDK_AddSymbolToStr(zstr,9,ch,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("AAab12cde",(char *)zstr))
	{
		send_result("line %d:%s����ʧ��%s", __LINE__, TESTAPI, zstr);
		RETURN;
	}
	strcpy((char *)zstr,"ab12cde");
	if((ret=NDK_AddSymbolToStr(zstr,9,ch,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("ab12cdeAA",(char *)zstr))
	{
		send_result("line %d:%s����ʧ��%s", __LINE__, TESTAPI, zstr);
		RETURN;
	}
	strcpy((char *)zstr,"ab12cde");
	if((ret=NDK_AddSymbolToStr(zstr,11,ch,2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("AAab12cdeAA",(char *)zstr))
	{
		send_result("line %d:%s����ʧ��%s", __LINE__, TESTAPI, zstr);
		RETURN;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}


/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util12.c
* Author 			: jiangym
* version			:
* DATE				: 20130109
* directory 	:
* description	: ����NDK_SubStr  �ӿڹ���
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
#define 	TESTAPI		"NDK_SubStr"

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
void util12(void)
{
	//����Ϊ�ֲ�����
	uchar soustr[20] = {0}, objstr[7] = {0};
	int stp = 2, num = 5, ret = 0;
	int len = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣����:��������ΪNULL�����Ӧ����ʧ��
	if((ret=NDK_SubStr(NULL,stp,num,objstr,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SubStr(soustr,stp,num,NULL,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SubStr(soustr,stp,num,objstr,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:nStartPos��ʼλ��С��1,��ʼλ�ô��ڴ����ȵ����������ʧ��
	if((ret=NDK_SubStr(soustr,0,num,objstr,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SubStr(soustr,sizeof(soustr)+1,num,objstr,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:nNum��ȡ���ַ���С�ڵ���0������ʧ��
	if((ret=NDK_SubStr(soustr,1,0,objstr,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:pszSouStrΪ"abc234defgh",nStartPosΪ3��nNumΪ3�����ȡ����c23,len = 2С��NUM Ӧ�÷��ز�������
	memset(objstr,0,sizeof(objstr));
	len = 2; //���ں���ʵ������lenС��num��ô�ͷ���ʧ��
	if((ret=NDK_SubStr((uchar *)"abc234defgh",3,3,objstr,&len))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case5:��������:pszSouStrΪ"abc234defgh",nStartPosΪ3��nNumΪ3�����ȡ����c23
	memset(objstr,0,sizeof(objstr));
	len = 4;
	if((ret=NDK_SubStr((uchar *)"abc234defgh",3,3,objstr,&len))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//cls_show_msg("obj=%s",objstr);
	if(strcmp("c23",(char *)objstr)||len!=3)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	send_result("%s����ͨ��", TESTAPI);
	return;
}


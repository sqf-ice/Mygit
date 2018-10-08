/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util8.c
* Author 			: jiangym
* version			:
* DATE				: 20130108
* directory 	:
* description	: ���� NDK_IntToBcd   ��NDK_BcdToInt     �ӿڹ���
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
#define 	TESTAPI		"NDK_IntToBcd ,NDK_BcdToInt "

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
void util8(void)
{
	//����Ϊ�ֲ�����
	uchar bbuf[2] = {0x00,0x00}, cbuf[2] = {0x00,0x00};
	int num = 0, i = 0;
	int len = 2, ret = 0;

	//����Ϊ������ 
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������
	
	//case1:�쳣����:��������ΪNULL,Ӧ����ʧ��
	if((ret=NDK_IntToBcd(NULL,&len,num))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_IntToBcd(bbuf,NULL,num))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_BcdToInt(NULL,&num))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_BcdToInt(bbuf,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:nNumΪ-1��10000�Ĳ��ԣ��������ʧ��
	if((ret=NDK_IntToBcd(bbuf,&len,-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_IntToBcd(bbuf,&len,10000))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//psBcd����Ĳ���0��9��BCD�� ����0xaabb��Ӧ�÷���ʧ��
	bbuf[0] = 0xaa;
	bbuf[1] = 0xbb;
	if((ret=NDK_BcdToInt(bbuf,&num))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:����pnBcdLen����С��2������ʧ�� 
	len = 1;
	if((ret=NDK_IntToBcd(bbuf,&len,56))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:��������:��������nNum 1234�����䷵���ж�psBcd��0x1234�pnBcdLenΪ2
	memset(bbuf,0,sizeof(bbuf));
	len = 2;
	if((ret=NDK_IntToBcd(bbuf,&len,1234))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cbuf[0] = 0x12;
	cbuf[1] = 0x34;
	for(i=0;i<2;i++)
	{
		if(bbuf[i]!=cbuf[i])
		{
			send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, i);
			RETURN;
		}
		
	}
	if(len!=2)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	num = 0;
	if((ret=NDK_BcdToInt(bbuf,&num))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(num!=1234)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	send_result("%s����ͨ��", TESTAPI);
	return;
}


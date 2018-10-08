/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util6.c
* Author 			: jiangym
* version			:
* DATE				: 20130108
* directory 	:
* description	: ���� NDK_IntToC2  ��NDK_C2ToInt    �ӿڹ���
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
#define 	TESTAPI		"NDK_IntToC2,NDK_C2ToInt"

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
void util6(void)
{
	//����Ϊ�ֲ�����
	uint unum = 0;
	uchar sbuf[5] = {0};
	uchar c2buf[2] = {0xff,0xff};
	int ret = 0, i = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������
	
	//case1:�쳣����:��������ΪNULL�����Ӧ����ʧ��
	if((ret=NDK_IntToC2(NULL,unum))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_C2ToInt(NULL,sbuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_C2ToInt(&unum,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��������:����һ����5876����ת�����ַ�����0x16F4
	if((ret=NDK_IntToC2(sbuf,5876))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	c2buf[0] = 0x16;
	c2buf[1] = 0xF4;
	for(i=0;i<2;i++)
	{
		if(sbuf[i]!=c2buf[i])
		{
			send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, i);
			RETURN;
		}
	}
	unum = 0;
	if((ret=NDK_C2ToInt(&unum,c2buf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(unum!=5876)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case3:unNum�����䷶Χ������65536,��-1,�䷵��ֵ��Ϊ0 
	memset(sbuf,0,sizeof(sbuf));
	if((ret=NDK_IntToC2(sbuf,65536))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	for(i=0;i<2;i++)
	{
		if(sbuf[i]!=0x00)
		{
			send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, i);
			RETURN;
		}
	}
	memset(sbuf,0,sizeof(sbuf));
	if((ret=NDK_IntToC2(sbuf,-1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	for(i=0;i<2;i++)
	{
		if(sbuf[i]!=0x00)
		{
			send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, i);
			RETURN;
		}
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}


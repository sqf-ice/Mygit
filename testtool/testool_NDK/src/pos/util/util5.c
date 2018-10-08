/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util5.c
* Author 			: jiangym
* version			:
* DATE				: 20130108
* directory 	:
* description	: ���� NDK_IntToC4  ��NDK_C4ToInt    �ӿڹ���
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
#define 	TESTAPI		"NDK_IntToC4,NDK_C4ToInt"

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
void util5(void)
{
	//����Ϊ�ֲ�����
	uint unum = 0;
	uchar sbuf[10] = {0};
	uchar c4buf[4] = {0xff,0xff,0xff,0xff};
	int ret = 0, i = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������
	
	//case1:�쳣����:��������ΪNULL,Ӧ����ʧ��
	if((ret=NDK_IntToC4(NULL,unum))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_C4ToInt(NULL,sbuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_C4ToInt(&unum,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:��������:����һ������356789������ת�����ֽ��ַ�Ϊ0x571B5
	memset(sbuf,0,sizeof(sbuf));
	if((ret=NDK_IntToC4(sbuf,356789))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	c4buf[0] = 0x00;
	c4buf[1] = 0x05;
	c4buf[2] = 0x71;
	c4buf[3] = 0xB5;
	for(i=0;i<4;i++)
	{
		if(sbuf[i]!=c4buf[i])
		{
			send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, i);
			RETURN;
		}	
	}
	//��������:�����ַ�����0x571B5������ת��������Ϊ356789
	unum = 0;
	if((ret=NDK_C4ToInt(&unum,c4buf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(unum!=356789)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case3:������Ϊ-1����ת�����ֽ��ַ�Ϊ0xFFFFFFFF,������Ϊ4294967296����ת��Ϊ���ֽ��ַ�0x00000000
	memset(sbuf,0,sizeof(sbuf));
	if((ret=NDK_IntToC4(sbuf,-1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	for(i=0;i<4;i++)
	{
		if(sbuf[i]!=0xff)
		{
			send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI, i);
			RETURN;
		}		
	}
	memset(sbuf,0,sizeof(sbuf));
	if((ret=NDK_IntToC4(sbuf,4294967296))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	for(i=0;i<4;i++)
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


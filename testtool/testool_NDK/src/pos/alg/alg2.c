/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �㷨ģ��
* file name		: alg2.c
* Author 			: wangzy
* version			:
* DATE				: 20130319
* directory 	:
* description	: ����NDK_AlgSHA1�ӿڹ���
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
#define 	TESTAPI		"NDK_AlgSHA1"

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
*			  	  	     wangzy         20130319  	   created
*****************************************************************/
void alg2(void)
{
	//�ֲ�������ʼ�����������������ݡ����ݳ��ȡ��������
	uchar datain[1024]={0},dataout[20]={0};
	uchar outcrc[21]={0xf9,0x7e,0xb9,0x00,0x5c,0xd3,0x5d,0x29,0x32,0xe7,0x1a,0x7c,0xc07,0x54,0x7b,0xc3,0x2f,0x68,0x93,0x23};
	int ret=0;	

	cls_printf("����%s...", TESTAPI);
	
	//case1: �Ƿ���������
	//case1.1����Ƿ������ַ��NULL
	if((ret = NDK_AlgSHA1(NULL, sizeof(datain), dataout)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case1.2:����Ƿ����ݳ��ȣ�-1
	if((ret = NDK_AlgSHA1(datain, -1, dataout)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case1.3:����Ƿ������ַ��NULL
	if((ret = NDK_AlgSHA1(datain, sizeof(datain), NULL)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	memset(datain,'1',sizeof(datain));
	//case2:�������ԣ�����λ���ݼ��ܺ����ݳ���Ϊ20�ֽ�BUG:���򷵻�1
	if((ret = NDK_AlgSHA1(datain, sizeof(datain), dataout)) !=NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(memcmp(outcrc,dataout,sizeof(outcrc)-1))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 	 
	return;
}


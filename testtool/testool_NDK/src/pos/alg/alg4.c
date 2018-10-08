/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �㷨ģ��
* file name		: alg4.c
* Author 			: wangzy
* version			:
* DATE				: 20130319
* directory 	:
* description	: ����NDK_AlgSHA256�ӿڹ���
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
#define 	TESTAPI		"NDK_AlgSHA256"

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
void alg4(void)
{
	//�ֲ�������ʼ�����������������ݡ����ݳ��ȡ��������
	uchar datain[1024]={0},dataout[32]={0};
	uchar outcrc[32]={0xb4,0xa3,0x8f,0x5f,0x5a,0x21,0xb6,0xba,0x9f,0x20,0x08,0x87,0x8b,0xea,0xa3,0xa7,0x08,0x86,0x6c,0x66,0xcb,0xad,0xa7,0x1b,0xa2,0x8e,0xc3,0x59,0xb5,0xa5,0x3c,0x3c};
	int ret=0;
	
	cls_printf("����%s...", TESTAPI);
	
	//case1: �Ƿ���������
	//case1.1����Ƿ������ַ��NULL
	if((ret = NDK_AlgSHA256(NULL, sizeof(datain), dataout)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case1.2:����Ƿ����ݳ��ȣ�-1
	if((ret = NDK_AlgSHA256(datain, -1, dataout)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case1.3:����Ƿ������ַ��NULL
	if((ret = NDK_AlgSHA256(datain, sizeof(datain), NULL)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�������ԣ�����λ���ݼ��ܺ����ݳ���Ϊ32�ֽ� ?BUG:���򷵻�1 
	memset(datain,'1',sizeof(datain));
	if((ret = NDK_AlgSHA256(datain, sizeof(datain), dataout)) !=NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(memcmp(outcrc,dataout,sizeof(outcrc)))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 	 
	return;
}



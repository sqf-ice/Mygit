/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �㷨ģ��
* file name		: alg3.c
* Author 			: wangzy
* version			:
* DATE				: 20130319
* directory 	:
* description	: ����NDK_AlgSHA512�ӿڹ���
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
#define 	TESTAPI		"NDK_AlgSHA512"

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
void alg3(void)
{
	//�ֲ�������ʼ�����������������ݡ����ݳ��ȡ��������
	uchar datain[1024]={0},dataout[64]={0};
	uchar outcrc[64]={0x1f,0x95,0x45,0x3d,0xda,0xca,0x9e,0xd5,0xbb,0x07,0x5a,0x69,0x76,0xbf,0x69,0xee,0xe0,0x2a,0x91,0x86,0xac,0x9b,0xf6,0x34,0x59,0x65,0xaf,0x47,0xfd,0x61,0x34,0xa4,0x27,0xc7,0xf5,0xe7,0xb5,0xbb,0xfe,0xae,0xae,0x3c,0x84,0x3b,0x07,0xb5,0x46,0x46,0xf6,0x6f,0xd6,0x73,0x40,0x04,0xeb,0xc4,0xff,0x9e,0x06,0xba,0xb3,0xe1,0x88,0x5b};
	int ret=0;

	cls_printf("����%s...", TESTAPI);
	
	//case1: �Ƿ���������
	//case1.1����Ƿ������ַ��NULL
	if((ret = NDK_AlgSHA512(NULL, sizeof(datain), dataout)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case1.2:����Ƿ����ݳ��ȣ�-1
	if((ret = NDK_AlgSHA512(datain, -1, dataout)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case1.3:����Ƿ������ַ��NULL
	if((ret = NDK_AlgSHA512(datain, sizeof(datain), NULL)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�������ԣ�����λ���ݼ��ܺ����ݳ���Ϊ64�ֽ� ?BUG:���򷵻�1 
	memset(datain,'1',sizeof(datain));
	if((ret = NDK_AlgSHA512(datain, sizeof(datain), dataout)) !=NDK_OK)
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


/************************************************************************
*
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			: �㷨ģ��
* file name		    : alg8.c
* Author 			: zhengry	
* version			:
* DATE				: 20150907
* directory 		:
* description		: ����NDK_AlgSM2KeyPairGen����SM2�ǶԳƹ�˽Կ�ԵĹ��ܣ������㷨��
* related document	: NDK.chm
*
************************************************************************
* log			:
* Revision
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#define     MES_LEN    1024-96

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_AlgSM2KeyPairGen"

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
*			  	  	      zhengry           20150907  	    created
*****************************************************************/
void alg8(void)
{
	uchar pubkey[64]={0}, prikey[32]={0}, pubkey1[65]={0}, prikey1[33]={0};
	int ret=0;
		
	//process body
	cls_printf("����%s...", TESTAPI);

	//����ǰ��
	memset(pubkey, 0, sizeof(pubkey));
	memset(prikey, 0, sizeof(prikey));
	memset(pubkey1, 0, sizeof(pubkey1));
	memset(prikey1, 0, sizeof(prikey1));

	//�ýӿڲ�����NULL����������Կֵ
	//case1:��������
	if((ret=NDK_AlgSM2KeyPairGen(NULL, prikey))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM2KeyPairGen(pubkey, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AlgSM2KeyPairGen(NULL, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case2:���ɵĹ�ԿӦ����64�ֽڣ����ɵ�˽ԿӦ����32�ֽ�
	if((ret=NDK_AlgSM2KeyPairGen(pubkey1, prikey1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(pubkey1[64]!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pubkey1[64]);
		RETURN;
	}
	if(prikey1[32]!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, prikey1[32]);
		RETURN;
	}
	
	//case3:��������
	if((ret=NDK_AlgSM2KeyPairGen(pubkey, prikey))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:�ڶ������ɵ���Կ��Ӧ�����һ�����ɵ���Կ�Բ�һ��
	if(!memcmp(prikey, prikey1, sizeof(prikey)))
	{
		send_result("line %d:%s������Կ��ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	if(!memcmp(pubkey, pubkey1, sizeof(pubkey)))
	{
		send_result("line %d:%s������Կ��ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
		
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 	
	return;

}



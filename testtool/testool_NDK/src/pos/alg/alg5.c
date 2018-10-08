/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �㷨ģ��
* file name		: alg5.c
* Author 			: wangzy
* version			:
* DATE				: 20130319
* directory 	:
* description	: ����NDK_AlgRSAKeyPairGen�ӿڹ���
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
#define 	TESTAPI		"NDK_AlgRSAKeyPairGen"
//#define     RSA_KEYLEN_NUM  3
//#define     RSA_EXPKIND_NUM  2

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
void alg5(void)
{
	//�ֲ�������ʼ��
	ST_RSA_PUBLIC_KEY   pubkey={0};
	ST_RSA_PUBLIC_KEY   pubkey2={0};
	ST_RSA_PRIVATE_KEY prikey={0};
	ST_RSA_PRIVATE_KEY prikey2={0};
	//uchar pubkey[1024]={0};
	//uchar prikey[1024]={0};
	int ret=0;
	//uchar *moduleshex=(unsigned char*)("CDBFBED0A58B5CFC23B43C910B2A76024846FE1ECE162798294719458D90235787C64D3081318C936C24AF8DAEA9BB16AA698A17FCBA97C8EBD36A13EB1B4B69");
	//uchar *priexp=(unsigned char*)("892A7F35C3B23DFD6D22D30B5CC6F956DAD9FEBF340EC5101B84BB83B3B56CE3D2A05FFC5DDA6992812803A1900236D461F9938DAFD18702734E6D8BDACA7303");

	cls_printf("����%s...", TESTAPI);
	
	//case1: NDK_AlgRSAKeyPairGen����Ƿ���������
	//case1.1����Ƿ���Կλ����-1,10
	if((ret = NDK_AlgRSAKeyPairGen(-1, RSA_EXP_10001,&pubkey,&prikey)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_AlgRSAKeyPairGen(10, RSA_EXP_10001,&pubkey,&prikey)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
		
	//case1.2:����Ƿ�ָ������
#if !K21_ENABLE
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_512, RSA_EXP_3-1, &pubkey,&prikey)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#else//�Ͷ�ƽ̨֧�ֹ���оƬ�Ĳ�Ʒ����ʹ�ù���оƬ����RSA����,����֧��RSA_EXP_3��RSA_KEY_LEN_512���� 20160314 wangzy
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_1024, RSA_EXP_10001-1, &pubkey,&prikey)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_1024, RSA_EXP_10001+1, &pubkey,&prikey)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#if 1
	//case1.3:����Ƿ������Կ��NULL
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_1024, RSA_EXP_10001,NULL,&prikey)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case1.4:����Ƿ����˽Կ��NULL
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_1024, RSA_EXP_10001,&pubkey,NULL)) !=NDK_ERR_PARA)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif 
	//case2: NDK_AlgRSAKeyPairGen�������ԣ�����NDK_AlgRSAKeyPairVerify���м��� BUG:����ֵ����1
	//case2.1:512λ��Կ������
	//case2.1.1:512λ��Կ��RSA_EXP_3ָ�������µ�����*/
#if !K21_ENABLE
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_512,RSA_EXP_3,&pubkey,&prikey)) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_AlgRSAKeyPairVerify(&pubkey,&prikey)) !=NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2.1.2:512λ��Կ��RSA_EXP_10001ָ�������µ�����
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_512,RSA_EXP_10001, &pubkey2,&prikey2)) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_AlgRSAKeyPairVerify(&pubkey2,&prikey2)) !=NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	memset(&pubkey,0,sizeof(ST_RSA_PUBLIC_KEY));
	memset(&prikey,0,sizeof(ST_RSA_PRIVATE_KEY));
	memset(&pubkey2,0,sizeof(ST_RSA_PUBLIC_KEY));
	memset(&prikey2,0,sizeof(ST_RSA_PRIVATE_KEY));
	
	//case2.2:1024λ��Կ������ָ�������µ�������Կ
	//case2.2.1 RSA_EXP_3ָ��������������Կ
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_1024,RSA_EXP_3, &pubkey,&prikey)) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_AlgRSAKeyPairVerify(&pubkey,&prikey)) !=NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif	

	//case2.2.2 RSA_EXP_10001 ָ��������������Կ
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_1024,RSA_EXP_10001, &pubkey2,&prikey2)) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_AlgRSAKeyPairVerify(&pubkey2,&prikey2)) !=NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	memset(&pubkey,0,sizeof(ST_RSA_PUBLIC_KEY));
	memset(&prikey,0,sizeof(ST_RSA_PRIVATE_KEY));
	memset(&pubkey2,0,sizeof(ST_RSA_PUBLIC_KEY));
	memset(&prikey2,0,sizeof(ST_RSA_PRIVATE_KEY));
	
	//case2.3:2048λ��Կ������ָ�������µ�������Կ	
	//case2.3.1 RSA_EXP_3ָ��������������Կ
#if !K21_ENABLE
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_2048,RSA_EXP_3, &pubkey,&prikey)) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_AlgRSAKeyPairVerify(&pubkey,&prikey)) !=NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif	

	//case2.3.2 RSA_EXP_10001 ָ��������������Կ
	if((ret = NDK_AlgRSAKeyPairGen(RSA_KEY_LEN_2048,RSA_EXP_10001, &pubkey2,&prikey2)) != NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_AlgRSAKeyPairVerify(&pubkey2,&prikey2)) !=NDK_OK)
	{	
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 	
	return;

}


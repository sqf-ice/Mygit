/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name		: sec35.c
* Author 		: sull
* version		: 
* DATE			: 20180321
* directory 		: 
* description		: ����NDK_SecLoadKey AES��Կװ�ؽӿڹ���
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SecLoadKey"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef struct key_kcv{
	uchar *pkey;
	uchar *pkcv;
} KEY_KCV;
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author		date	remarks
*			  	  	      sull   20180321	created
*****************************************************************/
void sec35(void)
{
	int ret=0;
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_EXTEND_KEYBLOCK stExtendKey;
#if	0//TR31_ENABLE
	tr31_key_t tr31_st;
	unsigned int len = 0;
	ST_EXTEND_KEYBLOCK* p;
	uchar block[512]={0},mkey[17]={0};
#endif
	KEY_KCV SecLoad[ ]={
		{(uchar *)"\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11",	           						NULL},				// 0  case1.1
		{(uchar *)"\x12\x12\x12\x12\x12\x12\x12\x12\x12\x12\x12\x12\x12\x12\x12\x12",		   						NULL},				// 1  case1.2  ���İ�װTLK(�㷨ΪDES),����Ϊ16���ֽڵ�0x12
		{(uchar *)"\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13\x13",		   						NULL},				// 2  case1.3  (�쳣)Դ��Կ(��Կ����TLK,��Կ�㷨AES,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨DES) 
		{(uchar *)"\x14\x14\x14\x14\x14\x14\x14\x14\x14\x14\x14\x14\x14\x14\x14\x14",		   						NULL},	                        // 3  case1.4: (�쳣)Դ��Կ(��Կ����TLK,��Կ�㷨AES,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨SM4)  
		{(uchar *)"\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15",		   						NULL},                          // 4  case1.5: (�쳣)Դ��Կ(��Կ����TLK,��Կ�㷨DES,������Ϊ1)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨DES)
		{(uchar *)"\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16",		   						NULL},				// 5  case1.6  ���İ�װTLK(�㷨ΪDES),����Ϊ16���ֽڵ�0x16  (TLK��TMK����һ������Կ)����������case1.13 Դ��Կ(��Կ����TLK,��Կ�㷨DES,������Ϊ1)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨DES,������Ϊ2)case1.14 Դ��Կ(��Կ����TLK,��Կ�㷨SM4,������Ϊ1)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨SM4,������Ϊ2)
		{(uchar *)"\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17\x17",		   						NULL},				// 6  case1.7: (�쳣)Դ��Կ(��Կ����TLK,��Կ�㷨SM4,������Ϊ1)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨SM4)
		{(uchar *)"\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21\x21",		   						(uchar *)"\x9F\xC1\x89\x09"},				// 7  case1.8  ���İ�װTLK(�㷨ΪSM4),����Ϊ16���ֽڵ�0x21  (��ͬ��ϵ��Կֵ������ͬ��������case1.11 Դ��Կ(��Կ����TLK,��Կ�㷨AES,������Ϊ1)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TMK,������Ϊ2)  
		{(uchar *)"\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23\x23",  (uchar *)"\xFE\x2B\xFD\x6A"},				// 8  case1.9  (�쳣)Դ��Կ(��Կ����TLK,��Կ�㷨DES,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨AES)  ��װʧ���˿���������case1.12 ��SEC_KCV_ZERO��ʽ���İ�װĿ����Կ(��Կ����TMK,������Ϊ2)(���ǰ�װ)
		{(uchar *)"\x25\x25\x25\x25\x25\x25\x25\x25\x25\x25\x25\x25\x25\x25\x25\x25",		   						NULL},				// 9  case1.10 (�쳣)Դ��Կ(��Կ����TLK,��Կ�㷨SM4,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨AES) ��װʧ�ܿ������� case1.15 (�쳣)Դ��Կ(��Կ����TMK,��Կ�㷨AES,������Ϊ4)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TAK,������Ϊ3)
		{(uchar *)"\x27\x27\x27\x27\x27\x27\x27\x27\x27\x27\x27\x27\x27\x27\x27\x27",		  						 NULL},				//10  case1.16 (�쳣)Դ��Կ(��Կtype=100,��Կ�㷨AES,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TAK,������Ϊ3)
		{(uchar *)"\x29\x29\x29\x29\x29\x29\x29\x29\x29\x29\x29\x29\x29\x29\x29\x29",		   						(uchar *)"\xD6\xB9\x4A\xAC"},	                        //11  case1.17 Դ��Կ(��Կ����TMK,��Կ�㷨AES,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TAK,������Ϊ3)
		{(uchar *)"\x31\x31\x31\x31\x31\x31\x31\x31\x31\x31\x31\x31\x31\x31\x31\x31",		   						NULL},				//12  case1.18 (�쳣)Դ��Կ(��Կ����TAK,��Կ�㷨AES,������Ϊ3)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TPK,������Ϊ4)
		{(uchar *)"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33",								  NULL},				//13  case1.19 (�쳣)Դ��Կ(��Կ����TAK,��Կ�㷨AES,������Ϊ3)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,������Ϊ4)
		{(uchar *)"\x35x35x35x35x35x35x35x35x35x35x35x35x35x35x35x35x35x35x35x35",                                                          (uchar *)"\x35\xA5\x70\xE2"},         //14  case1.20 Դ��Կ(��Կ����TMK,��Կ�㷨AES,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TPK,������Ϊ4) 
		{(uchar *)"\x37\x37\x37\x37\x37\x37\x37\x37",															   NULL},				//15 case1.21:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,��Կ�㷨AES,������Ϊ5,����Ϊ8),Ӧ����ʧ��,AES��Կ����ֻ��Ϊ16
		{(uchar *)"\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39", NULL},				//16  case1.22:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊ24),Ӧ���سɹ�
		{(uchar *)"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41",								   NULL},				//17 case1.23:��������:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊ16)
		{(uchar *)"\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43",                                                           NULL},  				//18 case1.24�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կtype=100,������Ϊ6),����type��������Ӧ����ʧ��
		{(uchar *)"\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45",                                                           (uchar *)"\x19\xD5\xD5\x54"},  				//19  case1.25:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ7)Ӧ�ɹ�,�ٰ�װĿ����Կ(��Կ����TAK,������Ϊ8),Ӧ����ʧ��
		{(uchar *)"\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51\x51",								  NULL},				//20  case2.1  (�쳣)��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TAK,������Ϊ4,����Ϊsizeof(ST_EXTEND_KEYBLOCK))
		{(uchar *)"\x52\x52\x52\x52\x52\x52\x52\x52\x52\x52\x52\x52\x52\x52\x52\x52",								  (uchar *)"\x40\xD2\x45\x55"},				//21  case2.2  (�쳣)��SEC_KCV_ZERO��ʽ���İ�װĿ����Կ(��Կ����TAK,������Ϊ4,����Ϊ16)
		{(uchar *)"\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55",								  NULL},    			//22  case2.3  ��SEC_KCV_ZERO��ʽ���İ�װĿ����Կ(��Կ����TPK,������Ϊ3,����Ϊ16)
		{(uchar *)"\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71",								 NULL},   			//23  case2.4  (�쳣)��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կtype=100,������Ϊ3,����Ϊ16)
		{(uchar *)"\x73\x73\x73\x73\x73\x73\x73\x73\x73\x73\x73\x73\x73\x73\x73\x73",								 NULL},    			//24  case2.5  (�쳣)��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TDK,������Ϊ9,����Ϊ16)
		{(uchar *)"\x75",								 													NULL},   			//25  case2.6  (�쳣)��key����żУ��λ��ͬ,Ӧ��Ϊͬһ��key,��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TDK,������Ϊ9,����Ϊ16)
		{(uchar *)"\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16",		 NULL},   			//26  case1.121 ������װ32λtmk��Կ ����case1.26��װ��Կ��tmk��ɢ
		{(uchar *)"\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x16\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15",		 NULL}, 			//27  case1.26 �������԰�װ32λ��Կ 
	};
	char KeyValue[32] = {0};
	uchar ch = 0x00;

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	if(!flag_sdk3)
	{
		send_result("line %d:sdk2.0�汾��֧��%s,�������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI); 

	//����ǰ��
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//case1:MK/SK��ϵ 	***��װAES��Կ�谴����չ��ʽ���а�װ��ʽ����***
	//case1.1:�쳣����:��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TLK,��Կ�㷨aes,������Ϊ2),����TLK����IDֻ��Ϊ1,Ӧ����ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[0].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;	
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case1.2(����) �쳣����:ʹ��DES�İ�װ��ʽ���а�װ�᷵��NDK_ERR_SECP_PARAM
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[1].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case1.2:��������:��SEC_KCV_NONE����Կ�㷨AES��ʽ���İ�װTLK(��Կ����TLK,������Ϊ1)
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[1].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//������ʹ��AES�㷨��Կ��װDES�㷨��Կ
	//case1.3:�쳣����:Դ��Կ(��Կ����TLK,��Կ�㷨AES,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨DES),Ӧ�÷���ʧ��
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_DES;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[2].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if SEC_SM4_ENABLE   //��֧�ֹ��ܵĲ���֤
	//������ʹ��AES�㷨��Կ��װSM4�㷨��Կ
	//case1.4:�쳣����:Դ��Կ(��Կ����TLK,��Կ�㷨AES,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨SM4),Ӧ�÷���ʧ��
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[3].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif	
	
	//DES�㷨��Կ��ϵ��AES�㷨��Կ��ϵ�໥����������ӵ�ж����Ĵ洢�ռ䡣
	//case1.5:�쳣����:Դ��Կ(��Կ����TLK,��Կ�㷨DES,������Ϊ1)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨DES,������Ϊ2),����DESδ��װTLK��Ӧ����ʧ��
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_DES;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[4].pkey, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	
#if !defined N910P //N910P���߱��˹���
	if(flag_sdk3 && !g_seckeyowner)//sdk3.0���ڹ�����Կ��ʱ�尲ȫ������Կ��Ĭ�ϰ�װTLK��sdk2.0����װ  20180724  modify
	{
		if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	else
	{
		if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}		

#else
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1.6:��������:��SEC_KCV_NONE����Կ�㷨DES��ʽ���İ�װTLK(��Կ����TLK,������Ϊ1),��װDES��TLKӦ�óɹ�
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[5].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if SEC_SM4_ENABLE   //��֧�ֹ��ܵĲ���֤
	//SM4�㷨��Կ��ϵ��AES�㷨��Կ��ϵ�໥����������ӵ�ж����Ĵ洢�ռ䡣
	//case1.7:�쳣����:Դ��Կ(��Կ����TLK,��Կ�㷨SM4,������Ϊ1)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨SM4,������Ϊ2),����SM4δ��װTLK��Ӧ����ʧ��
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[6].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.8:��������:��SEC_KCV_NONE����Կ�㷨SM4��ʽ���İ�װTLK(��Կ����TLK,������Ϊ1),��װSM4��TLKӦ�óɹ�
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[7].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//������ʹ��DES�㷨��Կ��װAES�㷨��Կ
	//case1.9:�쳣����:Դ��Կ(��Կ����TLK,��Կ�㷨DES,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨AES),Ӧ�÷���ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[8].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if SEC_SM4_ENABLE   //��֧�ֹ��ܵĲ���֤
	//������ʹ��SM4�㷨��Կ��װAES�㷨��Կ
	//case1.10:�쳣����:Դ��Կ(��Կ����TLK,��Կ�㷨SM4,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨AES),Ӧ�÷���ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[9].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1.26:TMK�������ڷ�Χ��
	//��Χ�ѵ���Ϊ0-255,�ʴ˲��Ե���Ч,�ݲ�ʵ��

	//case1.11:��������:Դ��Կ(��Կ����TLK,��Կ�㷨AES,������Ϊ1)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TMK,������Ϊ2)
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[7].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[7].pkcv, stKcvInfoIn.nLen);
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//�����Բ�ͬ��Կֵ�ظ���װ��ID�����͡���;���������Կ�洢�ռ��ϣ�����ֵͬ���ǰ�װ��
	//case1.12:��������:��SEC_KCV_ZERO��ʽ���İ�װĿ����Կ(��Կ����TMK,������Ϊ2)(���ǰ�װ)
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 24;
	stExtendKey.pblock = (char*)SecLoad[8].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[8].pkcv, stKcvInfoIn.nLen);
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.121 ��װ32λTMK��Կ(�����ϲ�Ʒ����*���Ǵ����k21��,k21���ܰ�װ32λ��Կ,����׿�˿��԰�װ,�˴���װ��Ϊ�˺��氲װ32λ��tak)
#if defined N900AZ||defined N910AZ ||defined IM81AZ
	if(g_seckeyowner)
#endif
	{
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=9;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 32;
		stExtendKey.pblock = (char*)SecLoad[26].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}	

	//case1.13:Դ��Կ(��Կ����TLK,��Կ�㷨DES,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨DES,������Ϊ2)Ӧ�óɹ�
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_DES;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[5].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.14:�쳣����:Դ��Կ(��Կ����TMK,��Կ�㷨AES,������Ϊ4)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TAK,������Ϊ3),����ID=4û�κ���Կ��Ӧ����ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=4;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[9].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.15:�쳣����:Դ��Կ(��Կtype=100,��Կ�㷨AES,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TAK,������Ϊ3),����type��������Ӧ����ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=100;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[10].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.16:��������:Դ��Կ(��Կ����TMK,��Կ�㷨AES,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TAK,������Ϊ3)
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[11].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[11].pkcv, stKcvInfoIn.nLen);
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��һ�����ڲ�νṹ����Կ��ϵ����ҪָMKSK���У��ͼ�����Կ��Ӧ���ڰ�װͬ���Ļ�����߼�����Կ
	//case1.17:�쳣����:Դ��Կ(��Կ����TAK,��Կ�㷨AES,������Ϊ3)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TPK,������Ϊ4),Ӧ����ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=3;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[12].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.18:�쳣����:Դ��Կ(��Կ����TAK,��Կ�㷨AES,������Ϊ3)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,������Ϊ4),Ӧ����ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=3;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[13].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.19:��������:Դ��Կ(��Կ����TMK,��Կ�㷨AES,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TPK,������Ϊ4)
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[14].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[14].pkcv, stKcvInfoIn.nLen);
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//������ͬһ����Կֵ�ظ���װ��ID�����͡���;���������Կ�洢�ռ��ϣ�����ֵͬ�ظ���װ��
	//case1.20:��������:Դ��Կ(��Կ����TMK,��Կ�㷨AES,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TPK,������Ϊ4)
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[14].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	
	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[14].pkcv, stKcvInfoIn.nLen);
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//case1.21:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,��Կ�㷨AES,������Ϊ5,����Ϊ8),Ӧ����ʧ��,AES��Կ����ֻ��Ϊ16
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 8;
	stExtendKey.pblock = (char*)SecLoad[15].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.22:��������:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊ24),Ӧ���سɹ�
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 24;
	stExtendKey.pblock = (char*)SecLoad[16].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.26:������������:Դ��Կ(��Կ����TMK,������Ϊ9)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ13,����Ϊ32),Ӧ���سɹ�
#if defined N900AZ||defined N910AZ ||defined IM81AZ
	if(g_seckeyowner)
#endif
	{
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=9;
		stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
		stKeyInfo.ucDstKeyIdx=13;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 32;
		stExtendKey.pblock = (char*)SecLoad[27].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	
	//case1.23:��������:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ6,����Ϊ16)
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=6;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock =(char*)SecLoad[17].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//AES��ʱ��֧��TR 31��ʽ��װ

	//case1.24:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կtype=100,������Ϊ7),����type��������Ӧ����ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=7;
	stKeyInfo.ucDstKeyType = 100;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[18].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��������ͬһ����Կֵ�ظ���װ����ͬID�����ͻ���;����Կ�洢�ռ��ϣ��������ͬһ��Կ��ϵ�ģ��������㡰������Կ��ֵ������ͬ"):
	//case1.25:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ7)Ӧ�ɹ�,�ٰ�װĿ����Կ(��Կ����TAK,������Ϊ8),Ӧ����ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=7;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[19].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[19].pkcv, stKcvInfoIn.nLen);
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	stKeyInfo.ucDstKeyIdx=8;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_AES;
	stExtendKey.pblock = (char*)SecLoad[19].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
#if ANDIROD_ENABLE&!defined N910P  //������ȷ�ϰ�׿ƽ̨����
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)	/*��δִ��*/
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_INSTALL_REC)	/*Ӧʧ��*/
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:FIXED��ϵ:��һ����Կ��û��tmk ֱ�Ӱ�װ pin/mac/data key
	//case2.1:��������:��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TAK,������Ϊ4,����Ϊsizeof(ST_EXTEND_KEYBLOCK))
#if defined N900AZ||defined N910AZ ||defined IM81AZ
	if(g_seckeyowner)
#endif
	{
		memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
		memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyIdx=4;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 32;
		stExtendKey.pblock = (char*)SecLoad[20].pkey;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	//case2.2:��������:��SEC_KCV_ZERO��ʽ���İ�װĿ����Կ(��Կ����TAK,������Ϊ5,����Ϊ16)
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[21].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[21].pkcv, stKcvInfoIn.nLen);
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.3:��������:��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TPK,������Ϊ3,����Ϊ24)
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 24;
	stExtendKey.pblock = (char*)SecLoad[22].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.4:�쳣����:��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կtype=100,������Ϊ3,����Ϊ16),����type��������Ӧ����ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = 100;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[23].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		GOTOERR;
	}

	//case2.5:�쳣����:��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TDK,������Ϊ9,����Ϊ16),ͬһ����Կ��װ����ͬ��ID��,ID9Ӧ�óɹ�,ID10Ӧ��ʧ��
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=9;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	stExtendKey.pblock = (char*)SecLoad[24].pkey;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	stKeyInfo.ucDstKeyIdx=10;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_AES;
#if ANDIROD_ENABLE&!defined N910P//defined IM81||defined N900||
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)	/*��δִ��*/
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_INSTALL_REC)	/*Ӧʧ��*/
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.6:�쳣����:��key����żУ��λ��ͬ,Ӧ��Ϊͬһ��key,��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TDK,������Ϊ11,����Ϊ16),�ٰ�װĿ����Կ(��Կ����TAK,������Ϊ12),Ӧ���ذ�װ��Կ��¼����
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	memset(&stKeyInfo,0,sizeof(ST_SEC_KEY_INFO));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=11;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memset(KeyValue,*SecLoad[25].pkey,stExtendKey.len);
	stExtendKey.pblock = KeyValue;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	ch = (*(SecLoad[25].pkey)&0xfe)|(*(SecLoad[25].pkey)&0x01?0x00:0x01);
	memset(KeyValue, ch, stExtendKey.len);
	stExtendKey.pblock = KeyValue;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucDstKeyIdx=12;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_AES;
#if ANDIROD_ENABLE
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)/*��δʵ��ִ��*/
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_INSTALL_REC )/*Ӧʧ��*/
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//DUKPT ��NDK_SecLoadTIK�����в���
	send_result("%s����ͨ��", TESTAPI);
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}


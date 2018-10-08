/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec26.c
* Author 		: zhengry	
* version		: 
* DATE			: 20160107
* directory 		: 
* description		: ����NDK_SecLoadKey���ܲ�����Կװ�ؽӿڹ���
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
*			  	  	      zhengry   20160108	created
*****************************************************************/
void sec26(void)
{
	int ret=0;
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
#if	TR31_ENABLE
	tr31_key_t tr31_st;
	unsigned int len = 0;
	ST_EXTEND_KEYBLOCK* p;
	uchar block[512]={0},mkey[17]={0};
#endif
	KEY_KCV SecLoad[ ]={
		{(uchar *)"\x11",															   NULL},						// 0  case1.1  
		{(uchar *)"\x13",															   NULL},						// 1  case1.5  ���İ�װTLK(�㷨ΪDES),����Ϊ16���ֽڵ�0x13 
		{(uchar *)"\x15",															   NULL},						// 2  case1.3  
		{(uchar *)"\x1B",															   NULL},	                    // 3  case1.4  
		{(uchar *)"\x19",															   NULL},                       // 4  case1.2  ���İ�װTLK,����Ϊ16���ֽڵ�0x19
		{(uchar *)"\xE1\x46\x47\xE8\xA1\x35\x06\x1A\xE1\x46\x47\xE8\xA1\x35\x06\x1A", (uchar *)"\x3A\xDE\xBB\xE0"},	// 5  case1.9  SEC_KCV_ZERO��װTMK(�㷨ΪDES),��TLK��ɢ,����Ϊ16���ֽڵ�0x17
		{(uchar *)"\x1D",															   NULL},						// 6  case1.6
		{(uchar *)"\x51\xE9\x6A\x1A\x86\x9F\x3B\x7C\x9B\xD3\x3F\x6C\x34\x4C\x76\x32", (uchar *)"\x59\x4F\x26\x2E"},	// 7  case1.7  SEC_KCV_ZERO��װTMK(��TLK��ɢ),����Ϊ16���ֽڵ�0X1F  
		{(uchar *)"\x21",															  (uchar *)"\x6C\xD7\x6C\x59"},	// 8  case1.8  ��SEC_KCV_ZERO��ʽ���İ�װTMK,����Ϊ16���ֽڵ�0X21 
		{(uchar *)"\x23",															   NULL},						// 9  case1.10 (�쳣)SEC_KCV_NONE��װTAK,id=3(��TLK,id=4��ɢ),����Ϊ16���ֽڵ�0X23    
		{(uchar *)"\x25",															   NULL},						//10  case1.11 (�쳣)SEC_KCV_NONE��װTAK,id=3(��TYPE=100��ɢ),����Ϊ16���ֽڵ�0X25  
		{(uchar *)"\xC0\xD3\xEB\x69\xB1\x06\x47\xEF\x76\xD2\x49\x62\x75\xA4\x8B\xFB",(uchar *)"\x93\x38\x75\xD7"},	//11  case1.12 SEC_KCV_ZERO��װTAK(��TMK��ɢ),����Ϊ16���ֽڵ�0X27 
		{(uchar *)"\x29",															   NULL},						//12  case1.13 (�쳣)SEC_KCV_NONE��װTPK(��TAK��ɢ),����Ϊ16���ֽڵ�0X29 
		{(uchar *)"\x2A",															   NULL},						//13  case1.14 (�쳣)SEC_KCV_NONE��װTMK(��TAK��ɢ),����Ϊ16���ֽڵ�0X2B 
		{(uchar *)"\x44\x4E\x8D\x2A\x2A\xAA\x91\xB4\xAD\x47\x94\x78\xE6\x7D\x71\x75",(uchar *)"\x2E\x36\x8F\x3C"},  //14  case1.15��case1.16  SEC_KCV_ZERO��װTPK(��TMK��ɢ),����Ϊ16���ֽڵ�0X2D 
		{(uchar *)"\x2F",															   NULL},						//15  case1.17 (�쳣)SEC_KCV_NONE��װTDK(��TMK��ɢ,����Ϊ8),����0X2F 
		{(uchar *)"\x31",															   NULL},						//16  case1.18 (�쳣)SEC_KCV_NONE��װTPK(��TMK��ɢ,����Ϊ24),����0X31
		{(uchar *)"\x33",															   NULL},						//17  case1.19  SEC_KCV_NONE��װTDK(��TMK��ɢ),����Ϊ16���ֽڵ�0X33 
		{(uchar *)"\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39",(uchar *)"\xA1\xE5\xF0\xD6"},  //18  case1.20  (�쳣)��TR31�����SEC_KCV_ZERO��ʽ��װTDK(��TMK��ɢ),����Ϊ16���ֽڵ�0X39 
		{(uchar *)"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41",(uchar *)"\x42\x65\x95\x98"},  //19  case1.21  (�쳣)��TR31�����SEC_KCV_ZERO��ʽ��װTDK(��TMK��ɢ),����Ϊ16���ֽڵ�0X41
		{(uchar *)"\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43",(uchar *)"\x04\x97\x88\x04"},  //20  case1.22  (�쳣)��TR31�����SEC_KCV_ZERO��ʽ��װTDK(��TMK��ɢ),����Ϊ16���ֽڵ�0X43 
		{(uchar *)"\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45\x45",(uchar *)"\xE4\x80\x14\x7D"},  //21  case1.23  ��TR31�����SEC_KCV_ZERO��ʽ��װTDK(��TMK��ɢ),����Ϊ16���ֽڵ�0X45 
		{(uchar *)"\x47",															   NULL},						//22  case1.24  (�쳣)SEC_KCV_NONE��װtype=100(��TMK��ɢ),����0X47
		{(uchar *)"\xB7\xCE\x64\x11\xD8\xF0\xFA\xED\x20\xFD\xF1\x58\x9C\x5C\xD2\x1E",(uchar *)"\xED\xA3\x96\x39"},	//23  case1.25  (�쳣)SEC_KCV_ZERO��װTDK(��TMK��ɢ),����0X49
		{(uchar *)"\x51",															   NULL},						//24  case2.1	(�쳣)���İ�װTAK,����Ϊ16���ֽڵ�0X51,����Ϊsizeof(ST_EXTEND_KEYBLOCK)) 
		{(uchar *)"\x53",															 (uchar *)"\xE5\x24\x34\x5A"},	//25  case2.2   SEC_KCV_ZERO���İ�װTAK,����Ϊ16���ֽڵ�0X53 
		{(uchar *)"\x55",															 (uchar *)"\x25\xF5\x7A\xEB"},	//26  case2.3   SEC_KCV_ZERO���İ�װTDK,����Ϊ16���ֽڵ�0X55 
		{(uchar *)"\x57",															 (uchar *)"\xFE\xBE\x78\x43"},	//27  case2.4   SEC_KCV_ZERO���İ�װTDK,����Ϊ16���ֽڵ�0X57
		{(uchar *)"\x61",															   NULL},						//28  case2.5   (�쳣)SEC_KCV_NONE���İ�װtype=100,����0X61
		{(uchar *)"\x63",															   NULL},						//29  case2.6   (�쳣)SEC_KCV_NONE���İ�װTAK,����0X63
		{(uchar *)"\x65",															   NULL},						//30  case2.7   (�쳣)SEC_KCV_NONE���İ�װTAK,����0X65
		{(uchar *)"\x12\x41\x41\x41\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39",		   NULL},				//31 ����case1.26
	};
	uchar ch = 0x00;

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI); 

	//����ǰ��
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//case1:MK/SK��ϵ
	//case1.1:�쳣����:��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TLK,��Կ�㷨sm4,������Ϊ2),����TLK����IDֻ��Ϊ1,Ӧ����ʧ��
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[0].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case1.2:��������:��SEC_KCV_NONE����Կ�㷨SM4��ʽ���İ�װTLK(��Կ����TLK,������Ϊ1)
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[4].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//������ʹ��SM4�㷨��Կ��װDES�㷨��Կ
	//case1.3:�쳣����:Դ��Կ(��Կ����TLK,��Կ�㷨SM4,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨DES),Ӧ�÷���ʧ��
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_DES;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[2].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//DES�㷨��Կ��ϵ��SM4�㷨��Կ��ϵ�໥����������ӵ�ж����Ĵ洢�ռ䡣
	//case1.4:�쳣����:Դ��Կ(��Կ����TLK,��Կ�㷨DES,������Ϊ1)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨DES,������Ϊ2),����DESδ��װTLK��Ӧ����ʧ�ܡ�
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_DES;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[3].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;

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

	//case1.5:��������:��SEC_KCV_NONE����Կ�㷨DES��ʽ���İ�װTLK(��Կ����TLK,������Ϊ1),��װDES��TLKӦ�óɹ�
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[1].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//������ʹ��DES�㷨��Կ��װSM4�㷨��Կ
	//case1.6:�쳣����:Դ��Կ(��Կ����TLK,��Կ�㷨DES,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨sm4),Ӧ�÷���ʧ��
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[6].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.26:TMK�������ڷ�Χ��
	//��Χ�ѵ���Ϊ0-255,�ʴ˲��Ե���Ч,�ݲ�ʵ��

	//case1.7:��������:Դ��Կ(��Կ����TLK,������Ϊ1)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TMK,������Ϊ2)
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[7].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[7].pkcv, stKcvInfoIn.nLen);
	
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//�����Բ�ͬ��Կֵ�ظ���װ��ID�����͡���;���������Կ�洢�ռ��ϣ�����ֵͬ���ǰ�װ��
	//case1.8:��������:��SEC_KCV_ZERO��ʽ���İ�װĿ����Կ(��Կ����TMK,������Ϊ2)(���ǰ�װ)
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[8].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[8].pkcv, stKcvInfoIn.nLen);
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.9:Դ��Կ(��Կ����TLK,��Կ�㷨DES,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,��Կ�㷨DES,������Ϊ2)Ӧ�óɹ�
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_DES;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[5].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[5].pkcv, stKcvInfoIn.nLen);
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.10:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ4)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TAK,������Ϊ3),����ID=4û�κ���Կ��Ӧ����ʧ�ܡ�
	stKeyInfo.ucScrKeyIdx=4;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[9].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECKM_READ_REC) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.11:�쳣����:Դ��Կ(��Կtype=100,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TAK,������Ϊ3),����type��������Ӧ����ʧ��
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=100;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[10].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.12:��������:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TAK,������Ϊ3)
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[11].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[11].pkcv, stKcvInfoIn.nLen);

	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//��һ�����ڲ�νṹ����Կ��ϵ����ҪָMKSK���У��ͼ�����Կ��Ӧ���ڰ�װͬ���Ļ�����߼�����Կ
	//case1.13:�쳣����:Դ��Կ(��Կ����TAK,������Ϊ3)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TPK,������Ϊ4),Ӧ����ʧ��
	stKeyInfo.ucScrKeyIdx=3;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[12].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case1.14:�쳣����:Դ��Կ(��Կ����TAK,������Ϊ3)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TMK,������Ϊ4),Ӧ����ʧ��
	stKeyInfo.ucScrKeyIdx=3;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[13].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.15:��������:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TPK,������Ϊ4)
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[14].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[14].pkcv, stKcvInfoIn.nLen);

	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//������ͬһ����Կֵ�ظ���װ��ID�����͡���;���������Կ�洢�ռ��ϣ�����ֵͬ�ظ���װ��
	//case1.16:��������:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TPK,������Ϊ4)
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[14].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[14].pkcv, stKcvInfoIn.nLen);

	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

 	//case1.17:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊ8),Ӧ����ʧ��,SM4��Կ����ֻ��Ϊ16
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=8;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[15].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
 	//case1.18:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊ24),Ӧ����ʧ��
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=24;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[16].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
 	//case1.19:��������:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊ16)
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[17].pkey, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//case1.20:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊsizeof(ST_EXTEND_KEYBLOCK)),ST_SEC_KEY_INFO.len����
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, *SecLoad[8].pkey, 16);//��Ӧ��TMK ����key SecLoad[8].pkey

	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = SecLoad[18].pkey;
	tr31_st.keylen = 16;
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	p=(ST_EXTEND_KEYBLOCK *)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31;
	p->len=1;
	p->pblock=(char *)block;
	
	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[18].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_PARAM)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.21:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊsizeof(ST_EXTEND_KEYBLOCK)),ST_SEC_KEY_INFO.block����
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, *SecLoad[8].pkey, 16);//��Ӧ��TMK ����key SecLoad[8].pkey

	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = SecLoad[19].pkey;
	tr31_st.keylen = 16;
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	p=(ST_EXTEND_KEYBLOCK *)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31;
	p->len=len;
	memset(p->pblock, 0x00, len);
	
	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[19].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_PARAM)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.22:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊsizeof(ST_EXTEND_KEYBLOCK)),ST_SEC_KEY_INFO.format����
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, *SecLoad[8].pkey, 16);//��Ӧ��TMK ����key SecLoad[8].pkey

	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = SecLoad[20].pkey;
	tr31_st.keylen = 16;
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	p=(ST_EXTEND_KEYBLOCK *)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31+1;
	p->len=len;
	memset(p->pblock, 0x00, len);
	
	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[20].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_PARA)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.23:��������:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊsizeof(ST_EXTEND_KEYBLOCK))
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, *SecLoad[8].pkey, 16);//��Ӧ��TMK ����key SecLoad[8].pkey

	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = SecLoad[21].pkey;
	tr31_st.keylen = 16;
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	p=(ST_EXTEND_KEYBLOCK *)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31;
	p->len=len;
	p->pblock=(char *)block;
	
	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[21].pkcv, stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if 1//ANDIROD_ENABLE  //Ŀǰ�ǰ�׿ƽ̨֧�� ��������ƽ̨֧�ֿɿ��� 20180328 add  //����ƽ̨��Ʒ��Ӧ��֧��
	//case1.26:������DES�㷨��TLK��TR31��ʽ�·���װSM4�㷨�Ĺ�����Կ
	//Դ��Կ(��Կ����TLK,������Ϊ1)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ8,����Ϊsizeof(ST_EXTEND_KEYBLOCK))
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_DES;
	stKeyInfo.ucDstKeyIdx=8;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, *SecLoad[1].pkey, 16);//��Ӧ��TLK ����key SecLoad[1].pkey
	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = SecLoad[31].pkey;
	tr31_st.keylen = 16;
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	p=(ST_EXTEND_KEYBLOCK *)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31;
	p->len=len;
	p->pblock=(char *)block;
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
#endif

	//case1.24:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_NONE��ʽ��װĿ����Կ(��Կtype=100,������Ϊ6),����type��������Ӧ����ʧ��
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=6;
	stKeyInfo.ucDstKeyType=100;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[22].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//��������ͬһ����Կֵ�ظ���װ����ͬID�����ͻ���;����Կ�洢�ռ��ϣ��������ͬһ��Կ��ϵ�ģ��������㡰������Կ��ֵ������ͬ"):
	//case1.25:�쳣����:Դ��Կ(��Կ����TMK,������Ϊ2)��SEC_KCV_ZERO��ʽ��װĿ����Կ(��Կ����TDK,������Ϊ7)Ӧ�ɹ�,�ٰ�װĿ����Կ(��Կ����TAK,������Ϊ8),Ӧ����ʧ��
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=7;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[23].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[23].pkcv, stKcvInfoIn.nLen);

	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	stKeyInfo.ucDstKeyIdx=8;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
#if ANDIROD_ENABLE//defined IM81||defined N900||
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)	/*��δִ��*/
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_INSTALL_REC)	/*Ӧʧ��*/
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:FIXED��ϵ
	//case2.1:�쳣����:��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TAK,������Ϊ4,����Ϊsizeof(ST_EXTEND_KEYBLOCK)),����len=sizeof(ST_EXTEND_KEYBLOCK) ����MK/SK��ϵ,��Ӧ����
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(stKeyInfo.sDstKeyValue, *SecLoad[24].pkey, 16);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

 	//case2.2:��������:��SEC_KCV_ZERO��ʽ���İ�װĿ����Կ(��Կ����TAK,������Ϊ4,����Ϊ16)
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[25].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[25].pkcv, stKcvInfoIn.nLen);

	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
 	//case2.3:��������:��SEC_KCV_ZERO��ʽ���İ�װĿ����Կ(��Կ����TDK,������Ϊ5,����Ϊ16)
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[26].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[26].pkcv, stKcvInfoIn.nLen);

	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
 	//case2.4:��������:��SEC_KCV_ZERO��ʽ���İ�װĿ����Կ(��Կ����TPK,������Ϊ3,����Ϊ16)
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[27].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[27].pkcv, stKcvInfoIn.nLen);

	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
 	//case2.5:�쳣����:��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կtype=100,������Ϊ3,����Ϊ16),����type��������Ӧ����ʧ��
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=100;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[28].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		GOTOERR;
	}

 	//case2.6:�쳣����:��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TDK,������Ϊ9,����Ϊ16),ͬһ����Կ��װ����ͬ��ID��,ID9Ӧ�óɹ�,ID10Ӧ��ʧ��
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=9;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[29].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	stKeyInfo.ucDstKeyIdx=10;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
#if ANDIROD_ENABLE//defined IM81||defined N900||
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)	/*��δִ��*/
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_INSTALL_REC)	/*Ӧʧ��*/
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
 	//case2.7:�쳣����:��key����żУ��λ��ͬ,Ӧ��Ϊͬһ��key,��SEC_KCV_NONE��ʽ���İ�װĿ����Կ(��Կ����TDK,������Ϊ9,����Ϊ16),�ٰ�װĿ����Կ(��Կ����TAK,������Ϊ10),Ӧ���ذ�װ��Կ��¼����
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=9;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[30].pkey, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	ch = (*(SecLoad[30].pkey)&0xfe)|(*(SecLoad[30].pkey)&0x01?0x00:0x01);
	memset(stKeyInfo.sDstKeyValue, ch/**(SecLoad[30].pkey)-1*/, stKeyInfo.nDstKeyLen);
	stKeyInfo.ucDstKeyIdx=10;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
#if ANDIROD_ENABLE//defined IM81||defined N900||
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)/*��δʵ��ִ��*/
#else
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_SECKM_INSTALL_REC )/*Ӧʧ��*/
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//��NDK_SecLoadTIK�����в���
	send_result("%s����ͨ��", TESTAPI);
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}

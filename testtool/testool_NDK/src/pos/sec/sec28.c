/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec28.c
* Author 		: zhengry	
* version		: 
* DATE			: 20160107
* directory 		: 
* description		: ����NDK_SecGetMac����MAC����ӿڹ���
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
#define TESTAPI	"NDK_SecGetMac"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author		date	remarks
*			  	  	      zhengry   20160115	created
*****************************************************************/
void sec28(void)
{
	int ret=0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar szDataIn[257]={0},szMac[17]={0};
#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ��װ
	uchar sKeyValueIn[17]={0},sKsnIn[11]={0};
#endif
#if	TR31_ENABLE    
	tr31_key_t tr31_st;
	unsigned int len = 0;
	ST_EXTEND_KEYBLOCK* p;
	uchar block[512]={0},mkey[17]={0}, skey[17]={0};
#endif
#if SEC_RSA_ENABLE
	ST_SEC_RSA_KEY RsaKey;
	uchar *Pmod=(uchar *)"66C2BCD4AC436815C193A8D29A0755C80393D7AFC7D78B12FE371F91874D449271207DFA6A054D185B03E543137C59577769940830FEEA040053F72583B85951";
#endif

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI); 

	//����ǰ��:����������Կ 
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	memset(szDataIn, 0x20, sizeof(szDataIn));

	//���İ�װTMK,id=2,����Ϊ8��0x11+8��0x13
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x11\x11\x11\x11\x11\x11\x11\x11\x13\x13\x13\x13\x13\x13\x13\x13", stKeyInfo.nDstKeyLen);
	
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
		
	//��SEC_KCV_NONE���ķ�ʽ��װTAK1,id=3������Ϊ16��0x15
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15", stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//��SEC_KCV_ZERO��ʽ(TMK��ɢ)��װTAK2,id=4
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x16\x26\xE4\x01\xBF\xF1\x1B\x1B\x64\xF7\x4D\x21\x39\xEF\x27\xFA", stKeyInfo.nDstKeyLen);/*��Ӧ��key����16���ֽڵ�0x17*/

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, "\x66\x42\xC0\xC2", stKcvInfoIn.nLen);
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
#if TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//��SEC_KCV_ZERO��ʽ(TMK��TR31��ʽ�����ɢ)��װTAK3,id=5
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memcpy(mkey, "\x11\x11\x11\x11\x11\x11\x11\x11\x13\x13\x13\x13\x13\x13\x13\x13", 16);//��Ӧ��TMK 
	memset(skey, 0x1b, 8);	//TAK2��Կ����:8���ֽڵ�0x1b+8���ֽ�0x1d,�޸���Կǰ8�ֽںͺ�8�ֽڲ�һ�� 
	memset(skey+8, 0x1d, 8);	
	
	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAC_TYPE, 4);	/*TAK*/
	tr31_st.key = skey;
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
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);//ret=-1312
		GOTOERR;
	}
#else
	//��֧��TR31��ʽ�Ĳ�Ʒ�������İ�װ��ʽ��װ��Կ
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x1b, 8);
	memset(stKeyInfo.sDstKeyValue+8, 0x1d, 8);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//��SEC_KCV_NONE���ķ�ʽ��װTPK,id=6,����Ϊ16��0x1f
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=6;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f", stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
#if SEC_RSA_ENABLE
	//���İ�װRSA, ID=7
	RsaKey.usBits=RSA_KEY_LEN_512;
	strcpy((char *)RsaKey.sExponent, "03");
	memcpy(RsaKey.sModulus, Pmod, RsaKey.usBits*2/8);
	if ((ret=NDK_SecLoadRsaKey(7, &RsaKey))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ��װ
	//���İ�װDUKPT,ID=8
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(sKeyValueIn, 0x1d, 16);
	memset(sKsnIn, 0x12, 10);
	if ((ret=NDK_SecLoadTIK(8, 0, 16, sKeyValueIn, sKsnIn, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1:��������:��SEC_MAC_SM4ģʽ,7�ֽ���������,TAK1
	if ((ret=NDK_SecGetMac(3, szDataIn, 7, szMac, SEC_MAC_SM4))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szMac, "\x6F\x41\x61\x91\x36\xAF\x4B\xC4\xDE\xBD\x30\x7E\x6B\x85\x17\x3C", 16))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7],szMac[8],szMac[9],szMac[10],szMac[11],szMac[12],szMac[13],szMac[14],szMac[15]);
		GOTOERR;
	}
	
	//case2:��������:��SEC_MAC_SM4ģʽ,16�ֽ���������,TAK2
	if ((ret=NDK_SecGetMac(4, szDataIn, 16, szMac, SEC_MAC_SM4))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szMac, "\x33\xBA\x97\x84\x7A\xCA\x62\xF6\xCC\xF4\x9E\x78\xA1\x2C\x53\xF6", 16))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7],szMac[8],szMac[9],szMac[10],szMac[11],szMac[12],szMac[13],szMac[14],szMac[15]);
		GOTOERR;
	}
	
	//case3:��������:��SEC_MAC_SM4ģʽ,256�ֽ���������,TAK3
	if ((ret=NDK_SecGetMac(5, szDataIn, 256, szMac, SEC_MAC_SM4))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szMac, "\xfa\xcd\x5f\x4e\xba\x8a\x2d\x05\x3a\x96\x10\xed\x11\xa9\x22\xed", 16))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7],szMac[8],szMac[9],szMac[10],szMac[11],szMac[12],szMac[13],szMac[14],szMac[15]);
		GOTOERR;
	}

	//case4:�쳣����:��SEC_MAC_SM4ģʽ,16�ֽ���������,TPK
	if ((ret=NDK_SecGetMac(6, szDataIn, 16, szMac, SEC_MAC_SM4))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);  
		GOTOERR;
	}
	
#if SEC_RSA_ENABLE
	//case5:�쳣����:��SEC_MAC_SM4ģʽ,16�ֽ���������,RSA
	if ((ret=NDK_SecGetMac(7, szDataIn, 16, szMac, SEC_MAC_SM4))!=NDK_ERR_SECCR_GET_KEY)	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ����
	//case6:�쳣����:��SEC_MAC_SM4ģʽ,16�ֽ���������,DUKPT
	if ((ret=NDK_SecGetMac(8, szDataIn, 16, szMac, SEC_MAC_SM4))!=NDK_ERR_SECCR_GET_KEY)	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#endif

	send_result("%s����ͨ��", TESTAPI);
	
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}


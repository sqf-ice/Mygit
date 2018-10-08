/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name		: sec36.c
* Author 		:  	
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_SecCalcDes����AES�㷨�ӽ��ܽӿڹ���
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
#define TESTAPI	"NDK_SecCalcDes"	

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
*			  	  	      jiangym      20180321	created
*****************************************************************/
void sec36()
{
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_EXTEND_KEYBLOCK stExtendKey;
	int ret=0;
	uchar udesout[17]={0}, udesin[17]={0};
	char keyval[33] = {0};
#if	0//TR31_ENABLE    
	tr31_key_t tr31_st;
	unsigned int len = 0;
	ST_EXTEND_KEYBLOCK* p;
	uchar block[512]={0},mkey[17]={0},skey[17]={0};
#endif
#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ��װ
	uchar sKeyValueIn[17]={0},sKsnIn[11]={0};
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
	if(!flag_sdk3)
	{
		send_result("line %d:sdk2.0�汾��֧��%s,�������", __LINE__, TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...", TESTAPI); 
	
	memset(udesin, 0x20, 16);

	//����ǰ��:����������Կ
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//���İ�װTMK,ID=2
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memset(keyval, 0x11, stExtendKey.len);
	stExtendKey.pblock = keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		RETURN;
	}
	
	//���İ�װTDK1,ID=3   ��Կ����Ϊ24λ
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES; 
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 24;
	memset(keyval, 0x13, stExtendKey.len);
	stExtendKey.pblock = keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		RETURN;
	}
	
	//TMK��װTDK2,ID=4,���İ�װ
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memcpy(keyval, "\x48\x5F\x0E\x65\x96\x7D\x5B\x95\xA8\x8D\x06\xA3\x7C\xD0\xAA\x14 ", stExtendKey.len);
	stExtendKey.pblock = keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		RETURN;
	}

	//AES ��ʱ��֧��TR31��ʽ��װ
#if 0//TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//TMK��TR31��ʽ�����װTDK3,ID=5
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);	
	memset(mkey, 0x11, 16);/*��TMK��ɢ*/
	memset(skey, 0x17, 8);
	memset(skey+8, 0x27, 8);

	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = skey;
	tr31_st.keylen = 16;
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	p=(ST_EXTEND_KEYBLOCK*)(stKeyInfo.sDstKeyValue);
	p->format=SEC_KEYBLOCK_FMT_TR31;
	p->len=len;
	p->pblock=(char *)block;
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)     
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	//��֧��TR31��ʽ�Ĳ�Ʒ�������İ�װ��ʽ��װ��Կ
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_AES;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memset(keyval, 0x17, stExtendKey.len/2);
	memset(keyval+stExtendKey.len/2, 0x27, stExtendKey.len/2);
	stExtendKey.pblock = keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//���İ�װTAK,ID=6
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=6;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memset(keyval, 0x19, stExtendKey.len/2);
	memset(keyval+8, 0x29, stExtendKey.len/2);
	stExtendKey.pblock = keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//���İ�װTPK,ID=7
	memset(&stKeyInfo, 0, sizeof(stKeyInfo));
	memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
	memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_AES;
	stKeyInfo.ucDstKeyIdx=7;
	stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
	stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
	stExtendKey.len = 16;
	memset(keyval, 0x1b, stExtendKey.len);
	stExtendKey.pblock =keyval;
	memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	stKcvInfoIn.nLen=0;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if SEC_RSA_ENABLE
	//���İ�װRSA, ID=8
	RsaKey.usBits=RSA_KEY_LEN_512;
	strcpy((char *)RsaKey.sExponent, "03");
	memcpy(RsaKey.sModulus, Pmod, RsaKey.usBits*2/8);
	if ((ret=NDK_SecLoadRsaKey(8, &RsaKey))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ��װ
	//���İ�װDUKPT,ID=9	
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(sKeyValueIn, 0x1f, 16);
	memset(sKsnIn, 0x12, 10);
	if ((ret=NDK_SecLoadTIK(9, 0, 16, sKeyValueIn, sKsnIn, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if defined N900AZ||defined N910AZ ||defined IM81AZ
	if(g_seckeyowner)
#endif
	{
		//����32λ����,���İ�װTAK,ID=10 20180417 sull add
		memset(&stKeyInfo, 0, sizeof(stKeyInfo));
		memset(&stKcvInfoIn, 0, sizeof(stKcvInfoIn));
		memset(&stExtendKey, 0, sizeof(ST_EXTEND_KEYBLOCK));
		stKeyInfo.ucScrKeyIdx=0;
		stKeyInfo.ucScrKeyType=0;
		stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_AES;
		stKeyInfo.ucDstKeyIdx=10;
		//stKeyInfo.nDstKeyLen=16;
		//memset(stKeyInfo.sDstKeyValue, 0x19, stKeyInfo.nDstKeyLen/2);//�޸���Կǰ8�ֽںͺ�8�ֽڲ�һ�� 
		//memset(stKeyInfo.sDstKeyValue+stKeyInfo.nDstKeyLen/2, 0x29, stKeyInfo.nDstKeyLen/2);

		//stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		stKeyInfo.nDstKeyLen = sizeof(ST_EXTEND_KEYBLOCK);
		stExtendKey.format = SEC_KEYBLOCK_FMT_AES;
		stExtendKey.len = 32;
		memset(keyval, 0x31, stExtendKey.len);
		stExtendKey.pblock = keyval;
		memcpy(stKeyInfo.sDstKeyValue, &stExtendKey, sizeof(ST_EXTEND_KEYBLOCK));
		stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
		stKcvInfoIn.nLen=0;
		if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

	//case1.1:�쳣����:��SEC_AES_ENCRYPT����/SEC_AES_DECRYPT����,���ݳ��Ȳ���16������ Ӧ��ʧ��
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 8, udesout, SEC_AES_ENCRYPT))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 15, udesout, SEC_AES_DECRYPT))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.3:�쳣����:uckeyIdx������(id=10)
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 10, udesin, 16, udesout, SEC_AES_ENCRYPT))!=NDK_ERR_SECCR_GET_KEY)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:��������:ucKeyType�����㷨���ͣ�Ӧ��Ҳ���Գɹ�
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK|SEC_KEY_AES, 3, udesin, 16, udesout, SEC_AES_ENCRYPT))!=NDK_OK)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Ԥ�ڽ��������16�ֽڵ�0x20��24�ֽڵ�0x13��Կ��AES�㷨�¼��ܵõ�(����ΪAES �ӽ������)
	if(memcmp(udesout, "\x9E\x3A\xA3\xB9\x8B\xE0\xEB\x37\x06\xE3\x26\x7A\xD1\xA6\x01\xD4", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case3:��������:��SEC_AES_ENCRYPT ���� ��ԿID=3(��ԿΪ24λ)
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 16, udesout, SEC_AES_ENCRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(udesout, "\x9E\x3A\xA3\xB9\x8B\xE0\xEB\x37\x06\xE3\x26\x7A\xD1\xA6\x01\xD4", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case4:��������:��SEC_AES_DECRYPT ���� ��ԿID=4
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 4, udesin, 16, udesout, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Ԥ�ڽ������16�ֽڵ�����0X20��ID=4����Կֵ(16��0X15)��AES�㷨�½��ܵõ�
	if(memcmp(udesout, "\x80\xFD\x39\x1A\xD9\x34\x9D\x37\xEE\xBD\x0F\x67\xBB\xC3\x40\x26 ", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case5:��������:��SEC_AES_ENCRYPT ���� ��ԿID=5
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 5, udesin, 16, udesout, SEC_AES_ENCRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Ԥ�ڽ������16�ֽڵ�0X20��8���ֽڵ�0x17+8���ֽڵ�0x27����Կ(��TR31��ʽ���)��AES�㷨�¼��ܵõ�
	if(memcmp(udesout, "\xF3\x56\x83\x05\x43\x4F\x20\x6A\x22\xB7\xA9\x55\x5B\xE5\x69\x31 ", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case6:��SEC_AES_DECRYPTģʽ,ucKeyIdx=TAK's ID,ucKeyType=TAK,Ӧ�ɹ�(���Ź���)
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TAK, 6, udesin, 16, udesout, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Ԥ�ڽ��������16�ֽڵ�0x20��8���ֽڵ�0x19+8���ֽڵ�0x29��Կ��AES�㷨�½��ܵõ�
	if(memcmp(udesout, "\x64\x83\xC3\x22\x53\x13\x5A\x5B\x88\x82\xBC\x08\xF9\x55\x59\x89 ", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
#if !K21_ENABLE
	//case7:��SEC_AES_DECRYPTģʽ,ucKeyIdx=TPK's ID,ucKeyType=TPK,Ӧʧ��(�ʺ��ж�)
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 7, udesin, 16, udesout, SEC_AES_DECRYPT))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	//case8:�Ͷ�ƽ̨��Ʒ����ͨ������ӿ�������PIN��Կ��չӦ�óɹ�  
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 7, udesin, 16, udesout, SEC_AES_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Ԥ�ڽ��������16�ֽڵ�0x20��16�ֽڵ�0x1b��Կ��AES�㷨�½��ܵõ�
	if(memcmp(udesout, "\x0D\xCB\x1C\xA7\xCB\x91\xF1\x22\xD4\x2B\xE4\x80\x03\x13\x2B\x42 ", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

#if SEC_RSA_ENABLE
	//case9:��SEC_AES_DECRYPTģʽ,ucKeyIdx=RSA's ID,ucKeyType=TDK,Ӧʧ��
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 8, udesin, 16, udesout, SEC_AES_DECRYPT))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	
	//case10:��SEC_AES_DECRYPTģʽ,ucKeyIdx=DUKPT's ID,ucKeyType=TDK,Ӧʧ��
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 9, udesin, 16, udesout, SEC_AES_DECRYPT))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#endif

#if defined N900AZ||defined N910AZ ||defined IM81AZ
	if(g_seckeyowner)
#endif
	{
		//case11:��������:SEC_AES_ENCRYPT ���� ��ԿID=10
		if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TAK, 10, udesin, 16, udesout, SEC_AES_ENCRYPT))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//Ԥ�ڽ������16�ֽڵ�����0X20��ID=10����Կֵ(32��0X31)��AES�㷨�¼��ܵõ�
		if(memcmp(udesout, "\x71\xBA\xF2\x19\xAA\x78\x6E\x6B\x6E\x09\xE1\x6C\xD9\x00\x24\xC2", 16))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	send_result("%s����ͨ��", TESTAPI);
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}


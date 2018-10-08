/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec27.c
* Author 		: zhengry	
* version		: 
* DATE			: 20160107
* directory 		: 
* description		: ����NDK_SecCalcDes��������㷨sm4�ӽ��ܽӿڹ���
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
*			  	  	      zhengry   20160114	created
*****************************************************************/
void sec27()
{
	ST_SEC_KEY_INFO stKeyInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
	int ret=0;
	uchar udesout[17]={0}, udesin[17]={0};
#if	TR31_ENABLE    
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
	cls_show_msg1(2, "����%s...", TESTAPI); 
	
	memset(udesin, 0x20, 16);

	//����ǰ��:����������Կ
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//���İ�װTMK,ID=2
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		RETURN;
	}
	
	//���İ�װTDK1,ID=3
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		RETURN;
	}
	
	//TMK��װTDK2,ID=4
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, 0x15, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		RETURN;
	}
	
#if TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//TMK��TR31��ʽ�����װTDK3,ID=5
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
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
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x17, 8);
	memset(stKeyInfo.sDstKeyValue+8, 0x27, 8);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//���İ�װTAK,ID=6
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=6;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x19, stKeyInfo.nDstKeyLen/2);//�޸���Կǰ8�ֽںͺ�8�ֽڲ�һ�� 
	memset(stKeyInfo.sDstKeyValue+stKeyInfo.nDstKeyLen/2, 0x29, stKeyInfo.nDstKeyLen/2);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//���İ�װTPK,ID=7
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=7;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x1b, stKeyInfo.nDstKeyLen);
	
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
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

	//case1.1:�쳣����:��SEC_SM4_ENCRYPT����/SEC_SM4_DECRYPT����,���ݳ��Ȳ���16������ Ӧ��ʧ��
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 8, udesout, SEC_SM4_ENCRYPT))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 15, udesout, SEC_SM4_DECRYPT))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.3:�쳣����:uckeyIdx������(id=10)
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 10, udesin, 16, udesout, SEC_SM4_ENCRYPT))!=NDK_ERR_SECCR_GET_KEY)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:��������:ucKeyType�����㷨���ͣ�Ӧ��Ҳ���Գɹ�
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK|SEC_KEY_SM4, 3, udesin, 16, udesout, SEC_SM4_ENCRYPT))!=NDK_OK)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Ԥ�ڽ��������16�ֽڵ�0x20��16�ֽڵ�0x13��Կ��sm4�㷨�¼��ܵõ�(����Ϊ�й����������ӽ��ܹ������)
	if(memcmp(udesout, "\x75\x6C\x98\xAE\x73\x17\xB8\xCF\x7D\xA2\x02\xD7\x62\x6F\x17\xF9", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case3:��������:��SEC_SM4_ENCRYPT ���� ��ԿID=3
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 16, udesout, SEC_SM4_ENCRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(udesout, "\x75\x6C\x98\xAE\x73\x17\xB8\xCF\x7D\xA2\x02\xD7\x62\x6F\x17\xF9", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case4:��������:��SEC_SM4_DECRYPT ���� ��ԿID=4
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 4, udesin, 16, udesout, SEC_SM4_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Ԥ�ڽ������16�ֽڵ�����0X20��ID=4����Կֵ(��16��0X11�ֽڶ�����16��0X15���ܵõ�)��sm4�㷨�½��ܵõ�(����Ϊ�й����������ӽ��ܹ������)
	if(memcmp(udesout, "\x24\xED\xA0\x62\xC1\xBC\x74\x88\xE3\x08\x3D\x79\x8B\xBF\xF1\x26", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case5:��������:��SEC_SM4_ENCRYPT ���� ��ԿID=5
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 5, udesin, 16, udesout, SEC_SM4_ENCRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Ԥ�ڽ������16�ֽڵ�0X20��8���ֽڵ�0x17+8���ֽڵ�0x27����Կ(��TR31��ʽ���)��sm4�㷨�¼��ܵõ�(����Ϊ�й����������ӽ��ܹ������)
	if(memcmp(udesout, "\x99\x5C\xB1\x8C\xC2\x96\xC2\xC1\x09\x98\x47\xF5\xAE\x07\x0F\x83", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case6:��SEC_SM4_DECRYPTģʽ,ucKeyIdx=TAK's ID,ucKeyType=TAK,Ӧ�ɹ�(���Ź���)
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TAK, 6, udesin, 16, udesout, SEC_SM4_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Ԥ�ڽ��������16�ֽڵ�0x20��8���ֽڵ�0x19+8���ֽڵ�0x29��Կ��sm4�㷨�½��ܵõ�(����Ϊ�й����������ӽ��ܹ������)
	if(memcmp(udesout, "\x02\x01\x16\xF1\xB6\xB3\xCD\x68\x21\xFC\xDF\x4B\x7A\x0A\x63\x20", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
#if !K21_ENABLE
	//case7:��SEC_SM4_DECRYPTģʽ,ucKeyIdx=TPK's ID,ucKeyType=TPK,Ӧʧ��(�ʺ��ж�)
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 7, udesin, 16, udesout, SEC_SM4_DECRYPT))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	//case8:(δ���뻹δ����)�Ͷ�ƽ̨��Ʒ����ͨ��sm4����ӿ�������PIN��Կ��չsm4���� ucKeyIdx=TPK's ID,ucKeyType=TPK����sm4���㣬Ӧ�óɹ�  
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 7, udesin, 16, udesout, SEC_SM4_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Ԥ�ڽ��������16�ֽڵ�0x20��16�ֽڵ�0x1b��Կ��sm4�㷨�½��ܵõ�(����Ϊ�й����������ӽ��ܹ������)
	if(memcmp(udesout, "\x75\x4A\x94\xE0\x0F\xA7\x9B\x39\xBD\xB8\xA9\xEC\x30\x67\x01\xD3", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

#if SEC_RSA_ENABLE
	//case9:��SEC_SM4_DECRYPTģʽ,ucKeyIdx=RSA's ID,ucKeyType=TDK,Ӧʧ��
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 8, udesin, 16, udesout, SEC_SM4_DECRYPT))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	
	//case10:��SEC_SM4_DECRYPTģʽ,ucKeyIdx=DUKPT's ID,ucKeyType=TDK,Ӧʧ��
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 9, udesin, 16, udesout, SEC_SM4_DECRYPT))!=NDK_ERR_SECCR_GET_KEY)
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

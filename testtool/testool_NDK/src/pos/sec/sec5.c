/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec5.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecGetMac�ӿڹ���
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
* history 		 		:  author		date		remarks
*			  	  	   linrq       20130228	created
*****************************************************************/
static int sec_919test(int  nDataLen, char *psData, char *sMacValue, int nGroupNo, int ucMode)
{
	int nLen = 0, i = 0, j = 0, nRet = 0;
	char sMacBlock[1024] = {0};

	memset(sMacBlock, 0x00, sizeof(sMacBlock));
	/*��Ϊ3DESʱ��Ԥ�����һ���������3DES����*/
	if (nDataLen%8 == 0)
	{
		nLen = nDataLen - 8;
	}
	else
	{
		nLen = (nDataLen / 8) * 8;
	}

	for(i=0; i<(nLen/8); i++)
	{
		for(j=0;j<8;j++)
		{
			sMacBlock[j] ^= *(psData+i*8+j);
		}		
		nRet = NDK_SecCalcDes(SEC_KEY_TYPE_TAK, nGroupNo, (uchar *)sMacBlock, 8, (uchar *)sMacValue, SEC_DES_KEYLEN_8);	
		if( nRet!=0 )
		{
			return -1;
		}
		memcpy(sMacBlock , sMacValue , 8);

	}

	for(j=0;j < (nDataLen-nLen) ; j++)
		sMacBlock[j] ^= psData[i*8+j];

	nRet = NDK_SecCalcDes(SEC_KEY_TYPE_TAK, nGroupNo, (uchar *)sMacBlock, 8, (uchar *)sMacValue, (ucMode==SEC_DES_KEYLEN_24)?SEC_DES_KEYLEN_24:SEC_DES_KEYLEN_16/*SEC_DES_KEYLEN_16*/);	
	if( nRet!=0 )
	{
		return -1;
	}
	return 0;
}



/*RSA: p=9D390558CA2B13CF4F7B265B6799E15E4231AD33B06566963683BBA61B178433;  q=A7524B070E59FEF8B1466A912C4471207D8CB9DDF38963FDC88D9C99B1B3886B
		e=4481D33872D79AB92BB7C5E1BC04E3DAAD0D3A752FE50761FECF6A6104DE2DB61DB8C911B6557C3591818D8EFFBEAF3B251CC8A45E0ABFA000D71499249E3323*/
void sec5(void)
{
	int ret = 0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar szDataIn[31]={0},szMac[9]={0};
#if DUKPT_ENABLE
	uchar sKeyValueIn[17]={0},sKsnIn[11]={0};
#endif
#if	TR31_ENABLE
	tr31_key_t tr31_st;
	unsigned int len = 0;
	ST_EXTEND_KEYBLOCK* p;
	uchar block[512]={0},mkey[17]={0},skey[17]={0};
#endif
#if SEC_RSA_ENABLE
	ST_SEC_RSA_KEY RsaKey;
	uchar *Pmod=(uchar *)"66C2BCD4AC436815C193A8D29A0755C80393D7AFC7D78B12FE371F91874D449271207DFA6A054D185B03E543137C59577769940830FEEA040053F72583B85951";
#endif
	uchar szout[9]={0};
#if OVERSEAS_ENABLE||defined ME30S||defined N910P
	uchar szout_hw[] = {0x26,0x9c,0x3b,0xec,0xb2,0xda,0xb1,0x82};
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
	
	//��װTMK, ID=5
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x11\x11\x11\x11\x11\x11\x11\x11\x13\x13\x13\x13\x13\x13\x13\x13", stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//��װTAK1(��Կ8�ֽ�), ID=1,���İ�װ
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=8;
	memcpy(stKeyInfo.sDstKeyValue, "\xE8\xAE\x27\xAF\xB8\x90\xD5\xC3", stKeyInfo.nDstKeyLen);/*��Ӧ��key����8���ֽڵ�0x15*/
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
#if OVERSEAS_ENABLE
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_ERR_SECP_PARAM)
#elif defined ME30S||defined N910P //ME30s���ز����Ƿ�-6
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_ERR_PARA)
#else
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//��װTAK2(��Կ16�ֽ�), ID=2,TR-31��ʽ��װ
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memcpy(mkey, "\x11\x11\x11\x11\x11\x11\x11\x11\x13\x13\x13\x13\x13\x13\x13\x13", 16);/*��TMK��ɢ*/
	//memset(skey, 0x17, 16); /*TPK2, ��Կ����16���ֽ�0x17*/
	memset(skey, 0x17, 8);	//TAK2��Կ����:8���ֽڵ�0x17+8���ֽ�0x19,�޸���Կǰ8�ֽںͺ�8�ֽڲ�һ�� linwl20130903
	memset(skey+8, 0x19, 8);	
	
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
	stKeyInfo.ucScrKeyIdx=0;/*��ʾ���İ�װ*/
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x17\x17\x17\x17\x17\x17\x17\x17\x19\x19\x19\x19\x19\x19\x19\x19", stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//��װTPK, ID=3
	stKeyInfo.ucScrKeyIdx=0;/*��ʾ���İ�װ*/
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x19, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��װTAK3(24�ֽ�), ID=4
	stKeyInfo.ucScrKeyIdx=0;/*��ʾ���İ�װ*/
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=24;
	memcpy(stKeyInfo.sDstKeyValue, "\x21\x21\x21\x21\x21\x21\x21\x21\x23\x23\x23\x23\x23\x23\x23\x23\x25\x25\x25\x25\x25\x25\x25\x25", stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if SEC_RSA_ENABLE
	//���İ�װRSA, ID=6
	RsaKey.usBits=RSA_KEY_LEN_512;
	strcpy((char *)RsaKey.sExponent, "03");
	memcpy(RsaKey.sModulus, Pmod, RsaKey.usBits*2/8);
	if ((ret=NDK_SecLoadRsaKey(6, &RsaKey))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	//֧��DUKPT��Կ��ϵ�Ų���
	//���İ�װDUKPT,ID=7
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(sKeyValueIn, 0x1d, 16);
	memset(sKsnIn, 0x12, 10);
	if ((ret=NDK_SecLoadTIK(7, 0, 16, sKeyValueIn, sKsnIn, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1:�Ƿ�����
	//case1.1:psDataIn==NULL
	if ((ret=NDK_SecGetMac(2, NULL, 8, szMac, SEC_MAC_X99))!=NDK_ERR_PARA)	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.2:psMacOut==NULL
	if ((ret=NDK_SecGetMac(2, szDataIn, 8, NULL, SEC_MAC_X99))!=NDK_ERR_PARA)	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.3:ucMod �Ƿ�
	if ((ret=NDK_SecGetMac(2, szDataIn, 8, szMac, 100))!=NDK_ERR_SECCR_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.4:Datalen<0
	if ((ret=NDK_SecGetMac(2, szDataIn, -1, szMac, SEC_MAC_X99))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.5:uckeyIdx������
	if ((ret=NDK_SecGetMac(10, szDataIn, 8, szMac, SEC_MAC_X99))!= NDK_ERR_SECCR_GET_KEY)	   
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if 0//101�ѺϷ�
	if ((ret=NDK_SecGetMac(101,szDataIn, 8, szMac, SEC_MAC_X99))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if !(OVERSEAS_ENABLE||defined ME30S||defined N910P)//ME30S�Ǻ����Ʒ
	//case2:��SEC_MAC_X99ģʽ,7�ֽ���������,TAK1
	if ((ret=NDK_SecGetMac(1,szDataIn, 7, szMac, SEC_MAC_X99))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(szMac, "\xBC\x99\x60\x6A\xBA\x33\xCC\x76", 8))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7]);
		GOTOERR;
	}
#endif

	//case3:��SEC_MAC_X99ģʽ,8�ֽ���������,TAK2
	if ((ret=NDK_SecGetMac(2, szDataIn, 8, szMac, SEC_MAC_X99))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//if(memcmp(szMac, "\x38\x12\x10\xE0\x4F\xFA\xFA\x80", 8))
	if(memcmp(szMac, "\xE8\x78\x69\x90\xBB\x51\x92\xD2", 8))//�޸���Կ2 ǰ8�ֽںͺ�8�ֽڲ�һ��,��Ӧ���޸Ľ�� linwl20130903
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7]);
		GOTOERR;
	}

#if !(OVERSEAS_ENABLE||defined ME30S||defined N910P)//ME30S�Ǻ����Ʒ
	//case4:��SEC_MAC_X919ģʽ,13�ֽ���������,TAK1
	if ((ret=NDK_SecGetMac(1, szDataIn, 13, szMac, SEC_MAC_X919))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if((ret=sec_919test(13, (char *)szDataIn, (char *)szout, 1, SEC_DES_KEYLEN_16))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if(memcmp(szMac, szout, 8))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7]);
		GOTOERR;
	}
#endif

	//case5:��SEC_MAC_X919ģʽ,16�ֽ���������,TAK3
	if ((ret=NDK_SecGetMac(4, szDataIn, 16, szMac, SEC_MAC_X919))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !(OVERSEAS_ENABLE||defined ME30S||defined N910P)//ME30S�Ǻ����Ʒ
	if((ret=sec_919test(16, (char *)szDataIn, (char *)szout, 4, SEC_DES_KEYLEN_24))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(szMac, szout, 8))
#else //����汾��֧��sec_919test��NDK_SecCalcDes���һλ��������Ϊ8�ֽڵ���֤��ֱ�Ӷ������֤ 20160706 modify 
	if(memcmp(szMac, szout_hw, 8))
#endif
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7]);
		GOTOERR;
	}

#if !(OVERSEAS_ENABLE||defined ME30S||defined N910P)//ME30S�Ǻ����Ʒ
	//case6:��SEC_MAC_ECBģʽ,17�ֽ���������,TAK1
	if ((ret=NDK_SecGetMac(1, szDataIn, 17, szMac, SEC_MAC_ECB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(szMac, "\x46\x36\x38\x34\x30\x45\x35\x32", 8))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7]);
		GOTOERR;
	}
#if 0
	//case7:��SEC_MAC_ECBģʽ,19�ֽ���������,TAK2
	if ((ret=NDK_SecGetMac(2, szDataIn, 19, szMac, SEC_MAC_ECB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//if(memcmp(szMac, "\x43\x35\x39\x39\x42\x34\x44\x46", 8))
	if(memcmp(szMac, "\x41\x43\x33\x37\x39\x45\x37\x32", 8))//�޸���Կ2 ǰ8�ֽںͺ�8�ֽڲ�һ��,��Ӧ���޸Ľ�� linwl20130903
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7]);
		GOTOERR;
	}
#endif	
	//case8:��SEC_MAC_9606ģʽ,21�ֽ���������,TAK1
	if ((ret=NDK_SecGetMac(1, szDataIn, 21, szMac, SEC_MAC_9606))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	if(memcmp(szMac, "\x50\x06\x2F\x41\x99\x00\xC4\x31", 8))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7]);
		GOTOERR;
	}
#endif

	//case8:��SEC_MAC_9606ģʽ,23�ֽ���������,TAK3
	if ((ret=NDK_SecGetMac(4, szDataIn, 23, szMac, SEC_MAC_9606))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(szMac, "\x03\x34\x74\xF4\x85\x6C\x90\x3F", 8))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5],szMac[6],szMac[7]);
		GOTOERR;
	}

	//case9:��SEC_MAC_X919ģʽ,13�ֽ���������,TPK
	if ((ret=NDK_SecGetMac(3, szDataIn, 13, szMac, SEC_MAC_X919))!=NDK_ERR_SECCR_GET_KEY)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if SEC_RSA_ENABLE
	//case10:��SEC_MAC_X919ģʽ,13�ֽ���������,RSA
	if ((ret=NDK_SecGetMac(6, szDataIn, 13, szMac, SEC_MAC_X919))!=NDK_ERR_SECCR_GET_KEY)   
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	//֧��DUKPT��Կ��ϵ�Ų���
	//case11:��SEC_MAC_X919ģʽ,13�ֽ���������,DUKPT
	if ((ret=NDK_SecGetMac(7, szDataIn, 13, szMac, SEC_MAC_X919))!=NDK_ERR_SECCR_GET_KEY)   
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


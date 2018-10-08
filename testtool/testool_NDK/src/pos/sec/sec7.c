/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec7.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecCalcDes�ӿڹ���
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
void sec7(void)
{
	int ret = 0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar udesout[31]={0}, udesin[31]={0};
#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ��װ
	uchar sKeyValueIn[17]={0},sKsnIn[11]={0};
#endif
#if	TR31_ENABLE && !(OVERSEAS_ENABLE||defined ME30S||defined N910P)
	tr31_key_t tr31_st;
	unsigned int len = 0;
	ST_EXTEND_KEYBLOCK* p;
	uchar block[512]={0},mkey[17]={0},skey[32]={0};
#endif
#if SEC_RSA_ENABLE
	ST_SEC_RSA_KEY RsaKey;
	uchar *Pmod=(uchar *)"66C2BCD4AC436815C193A8D29A0755C80393D7AFC7D78B12FE371F91874D449271207DFA6A054D185B03E543137C59577769940830FEEA040053F72583B85951";
#endif
#if ANDIROD_ENABLE
	uchar udeserrout[1025]={0}, udeerrsin[1025]={0};
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

	memset(udesin, 0x20, sizeof(udesin));

	//��װTMK, ID=5
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

#if !(OVERSEAS_ENABLE||defined ME30S||defined N910P)//ME30S�Ǻ����Ʒ
	//��װTDK1(��Կ8�ֽ�),ID=1
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=8;
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//��װTDK2(��Կ16�ֽ�),ID=2,���İ�װ
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x14\x5F\x5C\x6E\x3D\x91\x44\x57\x38\xBE\xDB\x24\xA6\xD3\x80\x18", stKeyInfo.nDstKeyLen); /*��Ӧ��key����Ϊ8���ֽڵ�0x15+8���ֽڵ�0x17���޸�ǰ8��8��Կ��һ��*/
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if TR31_ENABLE&&!(OVERSEAS_ENABLE||defined ME30S||defined N910P)	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//��װTDK3(��Կ24�ֽ�),ID=3,TR-31��ʽ��װ
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, 0x11, 16);/*��TMK��ɢ*/
//	memset(skey, 0x17, 24);
	memset(skey, 0x17, 8);
	memset(skey+8, 0x19, 8);
	memset(skey+16, 0x1B, 8);
	
	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_MAG_TYPE, 4);	/*TDK*/
	tr31_st.key = skey;
	tr31_st.keylen = 24;
	
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
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=24;
	memset(stKeyInfo.sDstKeyValue, 0x17, 8);
	memset(stKeyInfo.sDstKeyValue+8, 0x19, 8);
	memset(stKeyInfo.sDstKeyValue+16, 0x1B, 8);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//��װTAK, ID=4
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x19, stKeyInfo.nDstKeyLen/2);//�޸���Կǰ8�ֽںͺ�8�ֽڲ�һ�� linwl20130903
	memset(stKeyInfo.sDstKeyValue+stKeyInfo.nDstKeyLen/2, 0x1B, stKeyInfo.nDstKeyLen/2);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��װTPK, ID=8
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK;
	stKeyInfo.ucDstKeyIdx=8;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x1b, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if SEC_RSA_ENABLE//!K21_ENABLE
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
	
#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ��װ
	//���İ�װDUKPT,ID=7	
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(sKeyValueIn, 0x1f, 16);
	memset(sKsnIn, 0x12, 10);
	if ((ret=NDK_SecLoadTIK(7, 0, 16, sKeyValueIn, sKsnIn, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1:�Ƿ�����
	//case1.1:psDataIn==NULL
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, NULL, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_PARA) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.2:psDataOut==NULL
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, 8, NULL, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_PARA)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case1.3:ucMod �Ƿ�
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, 8, udesout, 100))!=NDK_ERR_SECCR_PARAM)	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.4:Datalen<=0
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, -1, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.5:uckeyIdx������(10,101)
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 10, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECCR_GET_KEY)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0 //101�ѺϷ�
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 101, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if 0 //�Ϸ��ĺ��Ź���,��case12
	//case1.6:ucKeyType=TAK
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TAK, 4, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1.7:nDataInLen��8������(nDataInLen=5),20130625����ȷ�Ϸ�8������Ӧ�÷��ز������󣬵ײ�������޸�linwl
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 5, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECP_PARAM/*NDK_OK*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if ANDIROD_ENABLE
	//case1.8:��������psDataIn����1024ʱ���ش���	//20160531����N900 3G�汾 V1.1.16��������
	memset(udeerrsin, 5, sizeof(udeerrsin));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udeerrsin, sizeof(udeerrsin), udeserrout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if !(OVERSEAS_ENABLE||defined ME30S||defined N910P)//ME30S�Ǻ����Ʒ
	//case2:�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODEģʽ, 8�ֽ���������,TDK1
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 8))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case3:�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ, 16�ֽ���������,TDK1
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, 16, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case4:�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|SEC_DES_ECBMODEģʽ, 24�ֽ���������,TDK1
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, 24, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9\x38\x77\x48\x93\xF5\x5F\xF4\xC9\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 24))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

	//case5:�� SEC_DES_DECRYPT|SEC_DES_KEYLEN_8|SEC_DES_CBCMODEģʽ, 8�ֽ���������,TDK2	
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 8, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_8))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !OVERSEAS_ENABLE
	if(memcmp(udesout, "\x28\xC6\xF7\xAC\x64\x87\x4D\xC1", 8))
#else  //����汾���һ��������Ч��ʹ�������id2 ��Կ����16�ֽڽ��мӽ���20160706 modify
	if(memcmp(udesout, "\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7", 8))
#endif
	{
		send_result("line %d:%s����ʧ��(%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x)", __LINE__, TESTAPI,udesout[0],udesout[1],udesout[2],udesout[3],udesout[4],udesout[5],udesout[6],udesout[7]);
		GOTOERR;
	}

	//case6:�� SEC_DES_DECRYPT|SEC_DES_KEYLEN_16|SEC_DES_CBCMODEģʽ, 16�ֽ���������,TDK2
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 16, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_16))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(udesout, "\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7", 16))//��Կ�޸ĳ�ǰ8�ֽںͺ�8�ֽڲ�һ������Ӧ���޸�Ԥ�ڽ��linwl20130903
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case7:�� SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_CBCMODEģʽ, 16�ֽ���������,TDK2	
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 16, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(udesout, "\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7", 16))//��Կ�޸ĳ�ǰ8�ֽںͺ�8�ֽڲ�һ������Ӧ���޸�Ԥ�ڽ��linwl20130903
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case8:�� SEC_DES_DECRYPT|SEC_DES_KEYLEN_24|SEC_DES_CBCMODEģʽ, 24�ֽ���������,TDK2
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 24, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(udesout, "\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7\x1E\x91\xC7\xC6\x2A\xBC\xCC\xA7", 24))//��Կ�޸ĳ�ǰ8�ֽںͺ�8�ֽڲ�һ������Ӧ���޸�Ԥ�ڽ��linwl20130903
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case9:�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8|SEC_DES_ECBMODEģʽ, 8�ֽ���������,TDK3
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !OVERSEAS_ENABLE
	if(memcmp(udesout, "\x38\x12\x10\xE0\x4F\xFA\xFA\x80", 8))
#else //����汾���һ��������Ч��ʹ�������id3 ��Կ����24�ֽڽ��мӽ���20160706 modify
	if(memcmp(udesout, "\x51\xBA\xAF\xE8\x08\xFA\x27\x37", 8))
#endif
	{
	send_result("line %d:%s����ʧ��(%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x)", __LINE__, TESTAPI,udesout[0],udesout[1],udesout[2],udesout[3],udesout[4],udesout[5],udesout[6],udesout[7]);
		GOTOERR;
	}

	//case10:�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16|SEC_DES_ECBMODEģʽ,16�ֽ���������,TDK3
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 16, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !OVERSEAS_ENABLE
	if(memcmp(udesout, "\xE8\x78\x69\x90\xBB\x51\x92\xD2\xE8\x78\x69\x90\xBB\x51\x92\xD2", 16))//��Կ�޸ĳ�ǰ8�ֽںͺ�8�ֽڲ�һ������Ӧ���޸�Ԥ�ڽ��linwl20130903
#else //����汾���һ��������Ч��ʹ�������id3 ��Կ����24�ֽڽ��мӽ���20160706 modify
	if(memcmp(udesout, "\x51\xBA\xAF\xE8\x08\xFA\x27\x37\x51\xBA\xAF\xE8\x08\xFA\x27\x37", 16))
#endif
	{
	send_result("line %d:%s����ʧ��(%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x)", __LINE__, TESTAPI,udesout[0],udesout[1],udesout[2],udesout[3],udesout[4],udesout[5],udesout[6],udesout[7]);
		GOTOERR;
	}

	//case11:�� SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24|SEC_DES_ECBMODEģʽ,24�ֽ���������,TDK3
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 24, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_24))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(udesout, "\x51\xBA\xAF\xE8\x08\xFA\x27\x37\x51\xBA\xAF\xE8\x08\xFA\x27\x37\x51\xBA\xAF\xE8\x08\xFA\x27\x37", 24))//��Կ�޸ĳ�ǰ8�ֽںͺ�8�ֽڲ�һ������Ӧ���޸�Ԥ�ڽ��linwl20130903
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case12:��SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_CBCMODEģʽ,ucKeyIdx=TAK's ID,ucKeyType=TAK,Ӧ�ɹ�(���Ź���)
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TAK, 4, udesin, 16, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(udesout, "\x3F\xFA\x7C\x9D\xEB\xD8\xD5\x1C\x3F\xFA\x7C\x9D\xEB\xD8\xD5\x1C", 16))//��Կ�޸ĳ�ǰ8�ֽںͺ�8�ֽڲ�һ������Ӧ���޸�Ԥ�ڽ��linwl20130903
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
#if !K21_ENABLE
	//case13:��SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_CBCMODEģʽ,ucKeyIdx=TPK's ID,ucKeyType=TPK,Ӧʧ��(�ʺ��ж�)
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 8, udesin, 16, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	//case13:�Ͷ�ƽ̨��Ʒ����ͨ��DES����ӿ�������PIN��Կ��չDES���� ucKeyIdx=TPK's ID,ucKeyType=TPK����DES���㣬Ӧ�óɹ� 
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 8, udesin, 16, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(udesout, "\x4D\x58\xB8\xA6\xCD\x9D\xEE\x86\x4D\x58\xB8\xA6\xCD\x9D\xEE\x86", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

#if SEC_RSA_ENABLE
	//case14:��SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_CBCMODEģʽ,ucKeyIdx=RSA's ID,ucKeyType=TDK,Ӧʧ��
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 6, udesin, 16, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ����
	//case15:��SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT|SEC_DES_CBCMODEģʽ,ucKeyIdx=DUKPT's ID,ucKeyType=TDK,Ӧʧ��
	 if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 7, udesin, 16, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT))!=NDK_ERR_SECCR_GET_KEY)
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


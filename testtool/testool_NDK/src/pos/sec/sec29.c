/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec29.c
* Author 		: zhengry	
* version		: 
* DATE			: 20160107
* directory 		: 
* description		: ����NDK_SecGetPin,NDK_SecGetPinResult��������PIN����ӿڹ���
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
#define TESTAPI	"NDK_SecGetPin,NDK_SecGetPinResult"	
#define PINTIMEOUT_MAX	(200*1000)
#define PINTIMEOUT_MIN	(5*1000)
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
*			  	  	      zhengry   20160120	created
*****************************************************************/
void sec29(void)
{
	int ret = 0, nStatus = 0, i = 0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar szPinOut[17]={0}, PinSM4Out[17]={0}, PinSM4Out1[17]={0};
	uchar *szPan=(uchar *)"6225885916163157";
	uchar szPan1[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x85,0x91,0x61,0x63,0x15}; //ʹ�����˺���䷽ʽ1
	uchar szPan2[16]={0x00,0x00,0x58,0x85,0x91,0x61,0x63,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //ʹ�����˺���䷽ʽ2
	uchar PinKeyValue[16]={0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19};//��Կ����
#if	TR31_ENABLE    
	tr31_key_t tr31_st;
	unsigned int len = 0;
	ST_EXTEND_KEYBLOCK* p;
	uchar block[512]={0},mkey[17]={0}, skey[17]={0};
#endif
#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ��װ
	uchar sKeyValueIn[17]={0},sKsnIn[11]={0};
#endif
#if SEC_RSA_ENABLE
	ST_SEC_RSA_KEY RsaKey;
	uchar *Pmod=(uchar *)"66C2BCD4AC436815C193A8D29A0755C80393D7AFC7D78B12FE371F91874D449271207DFA6A054D185B03E543137C59577769940830FEEA040053F72583B85951";
#endif

	cls_show_msg1(2, "����%s...", TESTAPI); 
	//����ǰ��:����������Կ
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//��SEC_KCV_NONE��ʽ���İ�װTMK, ID=5
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��SEC_KCV_NONE��ʽ���İ�װTAK, ID=1
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��SEC_KCV_NONE��ʽ���İ�װTPK1����TMK��ɢ��, ID=2
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x0A\x8D\xF7\xD1\x7C\xBF\x0C\xBA\x66\x84\xF4\x37\x66\xE6\x4B\x47", stKeyInfo.nDstKeyLen);/*��Ӧ��key����Ϊ16���ֽڵ�0x15*/
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//��TR31��ʽ�����װTPK2����TMK��ɢ��, ID=3
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, 0x11, 16);/*��TMK��ɢ*/
	memset(skey, 0x17, 8);	/*TPK2��Կ����:8���ֽڵ�0x17+8���ֽ�0x19*/
	memset(skey+8, 0x19, 8);	
	
	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = stKeyInfo.ucDstKeyIdx;
	memcpy(tr31_st.usage, KT_PIN_TYPE, 4);	/*TPK*/
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
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x17, 8);
	memset(stKeyInfo.sDstKeyValue+8, 0x19, 8);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//��SEC_KCV_NONE��ʽ���İ�װTPK3, ID=4
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x19, stKeyInfo.nDstKeyLen);
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

#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ��װ
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

 if(auto_flag==0||auto_flag==1)
 {
	//case1.1:�쳣����:����ʼ��������ȡ��Ļ����ֵ���򷵻س�ʼ��ʧ��
#if ANDIROD_ENABLE && !defined IM81AZ
	if((ret=NDK_SecGetPin(2, (uchar *)"0", szPan, szPinOut, SEC_PIN_SM4_2, PINTIMEOUT_MIN))!=NDK_ERR_SECVP_NOT_ACTIVE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinResult(szPinOut, &nStatus))!=NDK_ERR_SECVP_NOT_ACTIVE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1.2:�쳣����:��SEC_PIN_SM4_2(ʹ�����˺ŷ�ʽ),���˺�pszDataInΪNULL,Ӧʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(2, (uchar *)"0", NULL, szPinOut, SEC_PIN_SM4_2, PINTIMEOUT_MIN))!=NDK_ERR_PARA)		
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.3:�쳣����:��SEC_PIN_SM4_2,pszExpPinLenIn=13,Ӧʧ��(���볤�Ȳ��ܳ���12)
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(2, (uchar *)"13", szPan, szPinOut, SEC_PIN_SM4_2, PINTIMEOUT_MIN))!=NDK_ERR_PARA)		
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.4:�쳣����:������ʱ�䳬��ָ��ʱ��,Ӧʧ��
	cls_printf("�벻Ҫ����,���ĵȴ�%ds...", PINTIMEOUT_MIN/1000);
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(2, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_SM4_2, PINTIMEOUT_MIN))!=NDK_ERR_SECVP_TIMED_OUT/*NDK_ERR_SECP_TIMEOUT*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.5:�쳣����:��ԿΪTAK����,Ӧʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(1, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_SM4_2, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.6:�쳣����:��ԿΪRSA����,Ӧʧ��
#if SEC_RSA_ENABLE
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(6, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_SM4_2, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if DUKPT_ENABLE	//֧��DUKPT��Կ��Կ��ϵ�ĲŽ���DUKPT��Կ����
	//case1.7:�쳣����:��ԿΪDUKPT����,Ӧʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(7, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_SM4_2, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1.8:�쳣����:����Կ����ָ������Կ������ʱ,Ӧʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(8, (uchar *)"4", szPan, szPinOut, SEC_PIN_SM4_2, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
 }
 if(auto_flag==0||auto_flag==2)
 {
	//case2:�������ԣ����趨�ȴ�ʱ���ڣ���Ӧ�������ߣ���SEC_PIN_SM4_2,pszExpPinLenIn=0,pszDataIn!=NULL,id=2,Ӧ�ɹ�
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��ȴ�60���[ȷ��],�ȴ������в�Ӧ�ý�������");
#else
	cls_printf("��ȴ�60���[ȷ��],�ȴ������в�Ӧ�ý�������");
#endif
	if((ret=NDK_SecGetPin(2, (uchar *)"0", szPan, szPinOut, SEC_PIN_SM4_2, PINTIMEOUT_MAX))!=NDK_OK)		
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("��������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if(memcmp(szPinOut, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16)) /*�뿪��ȷ�Ϻ�,��pszExpPinLenIn="0"�򷵻�ȫ0*/
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}
	
	//case3:��������:��SEC_PIN_SM4_2,pszExpPinLenIn="0,0,0",pszDataIn=NULL,���볤�����Ƴ����ظ�"0,0,0"Ԥ�ڰ�ȷ�Ϸ���NDK_OK
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("�뾡�찴[ȷ��]...");
#else
	cls_printf("�뾡�찴[ȷ��]...");
#endif
	if((ret=NDK_SecGetPin(2, (uchar *)"0,0,0", szPan, szPinOut, SEC_PIN_SM4_2, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16)) /*�뿪��ȷ�Ϻ�,��pszExpPinLenIn="0"�򷵻�ȫ0*/
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}
	
	//case4:��������:ֻ�������趨�����볤��ʱ,�ſɼ���,��SEC_PIN_SM4_1,pszExpPinLenIn=4,pszDataIn=NULL,id=2,Ӧ�ɹ�
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������123��ȷ��,���޷�Ӧ������4��ȷ��...");
#else
	cls_printf("��������123��ȷ��,���޷�Ӧ������4��ȷ��...");
#endif	
	if((ret=NDK_SecGetPin(2, (uchar *)"4,4", NULL, szPinOut, SEC_PIN_SM4_1, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); 
		GOTOERR;
	}
	//�ü��ܽ����PIN:1234,���ղ�ʹ�����˺�,���벻�㲹F�ķ�ʽ�õ�:04 12 34 FF FF FF FF FF FF FF FF FF FF FF FF FF ʹ����Կ:ID=2 ��pinkey����Ϊ16���ֽڵ�0x15 ����SM4�㷨���ܵõ�
	if(memcmp(szPinOut, "\xA0\xF0\x47\x9C\xBF\xB0\xB8\xAE\xA4\x18\x8B\x3C\x30\xC7\x0B\x10", 16))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}
	
	//case5:��������:���������λ��Ӧ�������趨ֵ,���������˸�����޸�,��SEC_PIN_SM4_3(ʹ�����˺���䷽ʽ1�����벻��λ���������),pszExpPinLenIn=6,id=3,Ӧ�ɹ�
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("�볢������1237890��,�粻���������˸��Ϊ123456��ȷ��...");
#else
	cls_printf("�볢������1237890��,�粻���������˸��Ϊ123456��ȷ��...");
#endif		
	if((ret=NDK_SecGetPin(3, (uchar *)"6", szPan, szPinOut, SEC_PIN_SM4_3, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);  
		GOTOERR;
	}
	//���˺ž���1���õ�:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x85,0x91,0x61,0x63,0x15
	//���뾭����䷽ʽ1��������õ�:06 12 34 56 +12�ֽ������
	//���߽��а�λ�������õ�PINBLCOK��ʹ��ID=3��PIN��Կ����PinKeyValue ���ܵõ���Կ����
	//����:���������szPinOutͨ��sm4���ܺ�õ������ľ��������PINBLCOK
	if((ret=NDK_AlgSM4Compute(PinKeyValue, NULL, 16, szPinOut, PinSM4Out1, ALG_SM4_DECRYPT_ECB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);  
		GOTOERR;
	}
	//��PINBLCOK�����˺���䷽ʽ1�õ��Ľ��������������Ϳ��Եõ���Կ��䷽ʽ1��������Ľ��
	memset(PinSM4Out, 0, sizeof(PinSM4Out));
	for (i = 0; i < 16; i++) 
	{
		PinSM4Out[i] = PinSM4Out1[i] ^ szPan1[i];
	}
	if(memcmp(PinSM4Out, "\x06\x12\x34\x56", 4)) 
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x)", __LINE__, TESTAPI, PinSM4Out[0],PinSM4Out[1],PinSM4Out[2],PinSM4Out[3]);
		GOTOERR;
	}
	
	//case6:��������:��SEC_PIN_SM4_4,pszExpPinLenIn=9,id=4,Ӧ�ɹ�
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������123456789��ȷ��...");
#else
	cls_printf("��������123456789��ȷ��...");
#endif	
	if((ret=NDK_SecGetPin(4, (uchar *)"9", szPan, szPinOut, SEC_PIN_SM4_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ü��ܽ����PIN:123456789,ʹ�����˺���䷽ʽ2,���˺�00 00 58 85 91 61 63 15 00 00 00 00 00 00 00 00 �����  09 12 34 56 78 9F FF FF FF FF FF FF FF FF FF FF �������Կ:ID=4 ��pinkey����Ϊ16���ֽڵ�0x19 SM4���ܵõ���
	if(memcmp(szPinOut, "\x87\xC8\xF5\xE8\xF4\x35\xE6\xE8\x92\xA5\x9F\x50\x15\xEE\xB4\xBD", 16)) 
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}

	//case7:��������:��SEC_PIN_SM4_5,pszExpPinLenIn=12,Ӧ�ɹ�
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������123456789012��ȷ��...");
#else
	cls_printf("��������123456789012��ȷ��...");
#endif	
	if((ret=NDK_SecGetPin(3, (uchar *)"12", szPan, szPinOut, SEC_PIN_SM4_5, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//���������szPinOut��PINBLOCK��ID=3��ԿSM4���ܵõ���,�����ID=3����Կ��SM4����szPinOut�����ܽ��ΪPINBLOCK 
	if((ret=NDK_AlgSM4Compute(PinKeyValue, NULL, 16, szPinOut, PinSM4Out1, ALG_SM4_DECRYPT_ECB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);  
		GOTOERR;
	}
	//PINBLOCK���� ���˺Ű���䷽ʽ1�Ľ�� ����� pin���Ľ��(SEC_PIN_SM4_5�����벻�㲹�����) �õ���
	memset(PinSM4Out, 0, sizeof(PinSM4Out));
	for (i = 0; i < 16; i++) 
	{
		PinSM4Out[i] = PinSM4Out1[i] ^ szPan2[i];
	}
	if(memcmp(PinSM4Out, "\x0c\x12\x34\x56\x78\x90\x12", 7)) 
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinSM4Out[0],PinSM4Out[1],PinSM4Out[2],PinSM4Out[3],PinSM4Out[4],PinSM4Out[5],PinSM4Out[6]);
		GOTOERR;
	}
	
	//case8:֧����������ʱ,����������ȡ��,��SEC_PIN_SM4_5,pszExpPinLenIn=6,Ӧ֧����������ȡ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������123456����2��ȡ��...");
#else
	cls_printf("��������123456����2��ȡ��...");
#endif	
	if((ret=NDK_SecGetPin(3, (uchar *)"6", szPan, szPinOut, SEC_PIN_SM4_5, PINTIMEOUT_MAX))!=NDK_ERR_SECVP_GET_ESC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��������
	cls_show_msg1(2, "������ͨ��!��Ҫ�ٰ�[ȡ��]");
	
	//case9:��������:��SEC_PIN_SM4_2,pszExpPinLenIn="4,5,12"
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������12345��ȷ��...");
#else
	cls_printf("��������12345��ȷ��...");
#endif	
	if((ret=NDK_SecGetPin(2, (uchar *)"4,5,12", szPan, szPinOut, SEC_PIN_SM4_2, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ü��ܽ����PIN:12345,ʹ�����˺���䷽ʽ1,���˺�00 00 00 00 00 00 00 00 00 00 58 85 91 61 63 15 �����  05 12 34 5F FF FF FF FF FF FF FF FF FF FF FF FF �������Կ:ID=2 ��pinkey����Ϊ16���ֽڵ�0x15 SM4���ܵõ���
	if(memcmp(szPinOut, "\xA7\x65\x60\x7C\xE1\x96\x68\x48\xE5\xEA\xAB\x52\x8B\xFD\x63\x3A", 16)) 
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}

	//case10:�����ΪNULLʱ,Ӧ������ʾ�����밴��,��SEC_PIN_SM4_2,pszExpPinLenIn="4",NDK_SecGetPinResult()������ȡ PINBLOCK 
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������4321(�����������밴��)��ȷ��...");
#else
	cls_printf("��������4321(�����������밴��)��ȷ��...");
#endif		
	if((ret=NDK_SecGetPin(2, (uchar *)"4", szPan, NULL, SEC_PIN_SM4_2, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !K21_ENABLE//K21ƽ̨��֧��æ״̬�ж� lijq
	//����������֤����PIN����״̬���豸����æ״̬���������Զ�����,��NDK_SysGoSuspend����PINæ״̬���أ� NDK_ERR_PIN_BUSY = -3107	
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern())!=-3107)
#else
	if((ret=NDK_SysGoSuspend())!=-3107)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif	
#if 0   /*���ݿ�����Ա����,�޸�NDK_SecGetPinResultʹ��*/
	if((ret=NDK_SecGetPinResult(szPinOut, &nStatus))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else	
	do {
		ret=NDK_SecGetPinResult(szPinOut, &nStatus);
		if (ret != 0) {
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if (nStatus == SEC_VPP_KEY_ENTER) { 	/*ȷ�ϼ����£��ɷ�������*/
			break;
		} else if (nStatus == SEC_VPP_KEY_ESC) {	/*ȡ�������£����ش���*/
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
			break;
		} else if (nStatus == SEC_VPP_KEY_PIN || nStatus == SEC_VPP_KEY_BACKSPACE || nStatus == SEC_VPP_KEY_CLEAR ||  nStatus == SEC_VPP_KEY_NULL) {   
			//���ڱ������ڴ���pinblockʱ��������״̬�����������������������
			//����ѭ���ȴ�
			//����ȡPIN������Ҫ����������ʹûֵҲҪ����Ӧ�ó������ж��Ƿ��ж��˳������������SEC_VPP_NULL ����ֵ
		} else {
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
			break;
		}
		NDK_SysMsDelay(10);//usleep(10*1000);
	} while(1);
#endif
	//�ü��ܽ����PIN:4321,ʹ�����˺���䷽ʽ1,���˺�00 00 00 00 00 00 00 00 00 00 58 85 91 61 63 15 �����  04 43 21 FF FF FF FF FF FF FF FF FF FF FF FF FF �������Կ:ID=2 ��pinkey����Ϊ16���ֽڵ�0x15 SM4���ܵõ���
	if(memcmp(szPinOut, "\xE8\x3E\x0E\xC9\x2E\x65\x4A\x3F\x88\xA1\xED\xBA\x1E\xE2\x61\x3D", 16)) 
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}
	cls_show_msg("�������,[ȷ�ϼ�]����,Ӧ�ÿ������Ͻ�������,���ߺ��뻽��");
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	usleep(1000000);
	if((ret=NDK_SysWakeUp()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#else
	if((ret=NDK_SysGoSuspend())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(1);
#endif
	if(cls_show_msg("��������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case11:��������:�������pinʱ,ȡ������������ȷpinӦ�ÿ��Գɹ�
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������4321��,�ٰ�1��ȡ��,������1234��ȷ��...");
#else
	cls_printf("��������4321��,�ٰ�1��ȡ��,������1234��ȷ��...");
#endif		
	if((ret=NDK_SecGetPin(2, (uchar *)"4", szPan, szPinOut, SEC_PIN_SM4_4, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ü��ܽ����PIN:1234,ʹ�����˺���䷽ʽ2,���˺�00 00 58 85 91 61 63 15 00 00 00 00 00 00 00 00 �����  04 12 34 FF FF FF FF FF FF FF FF FF FF FF FF FF �������Կ:ID=2 ��pinkey����Ϊ16���ֽڵ�0x15 SM4���ܵõ���
	if(memcmp(szPinOut, "\xD6\xD5\xEA\xA4\x60\xAD\x30\x09\x94\x42\xD6\xFC\xB6\x2E\x63\x8B", 16))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}

	//case10:��������:�ڲ�ΪԤ������λ��ʱ����ȷ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������12345��ȷ��,���޷�Ӧ������6����ȷ��");
#else
	cls_printf("��������12345��ȷ��,���޷�Ӧ������6����ȷ��");
#endif		
	if((ret=NDK_SecGetPin(2, (uchar *)"6,4", szPan, szPinOut, SEC_PIN_SM4_2, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ü��ܽ����PIN:123456,ʹ�����˺���䷽ʽ1,���˺�00 00 00 00 00 00 00 00 00 00 58 85 91 61 63 15 �����  06 12 34 56 FF FF FF FF FF FF FF FF FF FF FF FF �������Կ:ID=2 ��pinkey����Ϊ16���ֽڵ�0x15 SM4���ܵõ���
	if(memcmp(szPinOut, "\xE1\xB2\xC7\x54\x8B\x72\x39\x79\xEE\x26\xDD\xA1\x2D\x09\xC5\x24", 16))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}

	//case12:��������:����Ԥ������λ���޶���Ӧ���ܳ������ֵ
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("�볢������123456789,Ӧ�޷�����9,�ٰ�ȷ��");
#else
	cls_printf("�볢������123456789,Ӧ�޷�����9,�ٰ�ȷ��");
#endif		
	if((ret=NDK_SecGetPin(2, (uchar *)"8,6,4", NULL, szPinOut, SEC_PIN_SM4_1, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ü��ܽ����PIN:12345678,���ղ�ʹ�����˺����벻�㲹F�ķ�ʽ�õ�   08 12 34 56 78 FF FF FF FF FF FF FF FF FF FF FF ʹ����Կ:ID=2 ��pinkey����Ϊ16���ֽڵ�0x15 ����SM4�㷨���ܵõ�
	if(memcmp(szPinOut, "\x31\x43\xC5\x94\x6C\x4F\x2C\x72\x0D\x9E\x29\xFC\xA6\x8B\xA5\xE2", 16))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7],szPinOut[8],szPinOut[9],szPinOut[10],szPinOut[11],szPinOut[12],szPinOut[13],szPinOut[14],szPinOut[15]);
		GOTOERR;
	}
}
else
	send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);

	send_result("%s����ͨ��", TESTAPI);
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;

}

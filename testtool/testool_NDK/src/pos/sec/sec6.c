/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec6.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecGetPin,NDK_SecGetPinResult�ӿڹ���
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
* history 		 		:  author		date		remarks
*			  	  	   linrq       20130228	created
*****************************************************************/
void sec6(void)
{
	int ret = 0, nStatus = 0, i = 0;
#if TR31_ENABLE
	tr31_key_t tr31_st;
	unsigned int len = 0;
	ST_EXTEND_KEYBLOCK *p;
	uchar mkey[17]={0},skey[17]={0},block[512]={0};
#endif	
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar sKeyValueIn[17]={0}, sKsnIn[11]={0},szExpPinLenIn[100+1]={0},szPinOut[8+1]={0};
	uchar *szPan=(uchar *)"6225885916163157";	
	uchar szPan1[8] = {0x00,0x00,0x58,0x85,0x91,0x61,0x63,0x15};
#if SEC_RSA_ENABLE
	ST_SEC_RSA_KEY RsaKey;
	uchar *Pmod=(uchar *)"66C2BCD4AC436815C193A8D29A0755C80393D7AFC7D78B12FE371F91874D449271207DFA6A054D185B03E543137C59577769940830FEEA040053F72583B85951";
#endif
	uchar PinKeyValue[8]={0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15};
	uchar PinKeyValue1[24]={0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19};
	uchar PinDesOut[9]={0},PinDesOut1[9]={0};
	cls_show_msg1(2, "����%s...", TESTAPI); 

	//����ǰ��:����������Կ
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//��װTMK, ID=5
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.nDstKeyLen=16;
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��װTAK(16bytes), ID=1
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if !(OVERSEAS_ENABLE||defined ME30S||defined N910P)//ME30S�Ǻ����Ʒ
	//��װTPK1(8bytes), ID=2,���İ�װ
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=8;
	memcpy(stKeyInfo.sDstKeyValue, "\x14\x5F\x5C\x6E\x3D\x91\x44\x57", stKeyInfo.nDstKeyLen);/*��Ӧ��key����Ϊ8���ֽڵ�0x15*/
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//��װTPK2(16bytes), ID=3,TR-31��ʽ��װ
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, 0x11, 16);/*��TMK��ɢ*/
	//memset(skey, 0x17, 16); /*TPK3, ��Կ����16���ֽ�0x17*/
	memset(skey, 0x17, 8);	/*TPK3��Կ����:8���ֽڵ�0x17+8���ֽ�0x19*/
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
	stKeyInfo.ucScrKeyIdx=0;/*��ʾ���İ�װ*/
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=16;
	memcpy(stKeyInfo.sDstKeyValue, "\x17\x17\x17\x17\x17\x17\x17\x17\x19\x19\x19\x19\x19\x19\x19\x19", stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//��װTPK3(24bytes), ID=4
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=24;
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
	
	//���İ�װDUKPT,ID=7	
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(sKeyValueIn, 0x1d, 16);
	memset(sKsnIn, 0x12, 10);
	if ((ret=NDK_SecLoadTIK(7, 0, 16, sKeyValueIn, sKsnIn, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
if((auto_flag==0)||(auto_flag==1))
{
	//case3:����ʼ��������ȡ��Ļ����ֵ���򷵻س�ʼ��ʧ��
#if ANDIROD_ENABLE && !defined IM81AZ
	if((ret=NDK_SecGetPin(2, (uchar *)"0", szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_SECVP_NOT_ACTIVE)
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

	//case1:�Ƿ�����
	//case1.1:pszExpPinLenIn==NULL
	strcpy((char *)szExpPinLenIn, "4,5,6,7,8,9,10,11,12");

	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(2, NULL, szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.2:pszDataIn==NULL //pszDataInĳЩ����¿���Ϊ��.�ŵ�������������.

	//case1.3:ucMod �Ƿ�
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(2, szExpPinLenIn, szPan, szPinOut, -1, PINTIMEOUT_MIN))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.4:nTimeOutMs>MAX��nTimeOutMs<MIN
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(2, szExpPinLenIn, szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX+1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(2, szExpPinLenIn, szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.5:uckeyIdx������
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(10, szExpPinLenIn, szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.6:����NDK_SecGetPinResult ����Ĳ���ΪNULL Ӧ�÷��ز���ʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"4", szPan, NULL, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinResult(NULL, &nStatus))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinResult(szPinOut, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:��SEC_PIN_ISO9564_0,pszExpPinLenIn=0,pszDataIn=NULL,Ӧʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"0", NULL, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_PARA)		
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
}
if((auto_flag==0)||(auto_flag==2))
{
	//case3:��SEC_PIN_ISO9564_0,pszExpPinLenIn=0,pszDataIn!=NULL,Ӧ�ɹ�
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !(K21_ENABLE||defined SP50||defined SP50G)//�Ͷ�,SP50û���Զ����߹���
	cls_printf("��ȴ�60���[ȷ��],�ȴ������в�Ӧ�ý�������");
#else
#if ANDIROD_ENABLE
	NDK_ScrPrintf("�뾡�찴[ȷ��]...");
#else
	cls_printf("�뾡�찴[ȷ��]...");
#endif
#endif
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"0", szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !(K21_ENABLE||defined SP50||defined SP50G)//�Ͷ�,SP50û���Զ����߹���
	if(cls_show_msg("��������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif	
	if(memcmp(szPinOut, "\x00\x00\x00\x00\x00\x00\x00\x00", 8)) /*�뿪��ȷ�Ϻ�,��pszExpPinLenIn="0"�򷵻�ȫ0*/
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		GOTOERR;
	}
	cls_show_msg1(2, "������ͨ��!");

	//case17:��֤���볤�����Ƴ����ظ�"0,0,0"Ԥ�ڰ�ȷ�Ϸ���NDK_OK��Ӧ�÷��ָ�����°�ȷ�ϻ᷵��ʧ��-1121��BUG����˵��������
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
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"0,0,0", szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(szPinOut, "\x00\x00\x00\x00\x00\x00\x00\x00", 8))	/*�뿪��ȷ�Ϻ�,��pszExpPinLenIn="0"�򷵻�ȫ0*/
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		GOTOERR;
	}

#if !(OVERSEAS_ENABLE||defined ME30S||defined N910P)//ME30S�Ǻ����Ʒ
	//case4:��SEC_PIN_ISO9564_1,pszExpPinLenIn=4,pszDataIn=NULL,Ӧ�ɹ�(SEC_PIN_ISO9564_1 ģʽ��ʹ����������ʲ����Ƚ�)
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
	if((ret=NDK_SecGetPin(2, (uchar *)"4,4", NULL, szPinOut, SEC_PIN_ISO9564_1, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//���������ͨ��des���ܺ���Եõ�����,���ĵ�ǰ��λ�����ǹ̶��Ŀ����ж� added jiangym 20150125
	if((ret=NDK_AlgTDes(szPinOut, PinDesOut, PinKeyValue, sizeof(PinKeyValue), ALG_TDS_MODE_DEC)) !=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����Ϊ141234xxxxxxxxxx (16λ)��һλ��ʾ����ģʽΪSEC_PIN_ISO9564_1,�ڶ�λ��ʾ������4λ,����������1234ʣ����������
	if(memcmp(PinDesOut, "\x14\x12\x34", 3))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

#if 1  /*�ײ����:���������λ��Ӧ�������趨ֵ,�����δ�����chensj 20130726*/
	//case12:��SEC_PIN_ISO9564_2,pszExpPinLenIn=6,pszDataIn=NULL,Ӧ�ɹ�
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
	if((ret=NDK_SecGetPin(3, (uchar *)"6", NULL, szPinOut, SEC_PIN_ISO9564_2, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//���ʹ��8�ֽ�0x17+8�ֽ�0x19��26123456FFFFFFFF������Ϊ:\xe2\x9c\x72\xc1\x6\x43\x53\x48
	if(memcmp(szPinOut,"\xe2\x9c\x72\xc1\x6\x43\x53\x48",8))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		GOTOERR;
	}
#endif
	
	//case5:��SEC_PIN_ISO9564_3,pszExpPinLenIn=12,pszDataIn!=NULL,Ӧ�ɹ�(SEC_PIN_ISO9564_3 ģʽ��ʹ����������ʲ����Ƚ�)
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
	if((ret=NDK_SecGetPin(4, (uchar *)"12", szPan, szPinOut, SEC_PIN_ISO9564_3, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//���������ͨ�����ܵó�����,�������뿨��������Եó��̶���ǰ��λ��  added jiangym 20150125
	if((ret=NDK_AlgTDes(szPinOut, PinDesOut1, PinKeyValue1, sizeof(PinKeyValue1), ALG_TDS_MODE_DEC)) !=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����ΪPinDesOut1 Ȼ���뿨���ұ�12λ(ȥ�����һλ,ǰ�油0)  ��:0000588591616315���ó�����ٱȽ�	
	memset(PinDesOut, 0, sizeof(PinDesOut));
	for (i = 0; i < 8; i++) 
	{
		PinDesOut[i] = PinDesOut1[i] ^ szPan1[i];
	}
	//����Ľ��:��һλ��ʾ����ģʽΪSEC_PIN_ISO9564_3,�ڶ�λ��ʾ������10λ(a),����������123456789012ʣ����������
	if(memcmp(PinDesOut, "\x3c\x12\x34\x56\x78\x90\x12", 7))
	{		
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	cls_show_msg1(2, "������ͨ��!");

	//case6:��SEC_PIN_ISO9564_0,pszExpPinLenIn=13,pszDataIn!=NULL,Ӧʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������1234567890123��ȷ��...");
#else
	cls_printf("��������1234567890123��ȷ��...");
#endif		
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"13", szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:��SEC_PIN_ISO9564_0,pszExpPinLenIn=12,pszDataIn!=NULL
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
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"12", szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_ERR_SECVP_GET_ESC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��������
	cls_show_msg1(2, "������ͨ��!��Ҫ�ٰ�[ȡ��]");

	//case8:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4,5,12",pszDataIn!=NULL
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
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"4,5,12", szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "\xE9\xF2\xED\xDD\x59\x85\x7F\xD0", 8))		//��Կ3�ļ�����
	//if(memcmp(szPinOut, "\x4F\xCB\xA0\x15\xC8\xDA\x83\x9D", 8))	//��Կ2�ļ�����
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		GOTOERR;
	}
	
	//case9:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,NDK_SecGetPinResult()������ȡ PINBLOCK 
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������1234(�����������밴��)��ȷ��...");//���ݸ߶��鷴��˵pinblock����0x00����������00��bug,�޸Ĳ�������Ϊ1234  20171115
#else
	cls_printf("��������1234(�����������밴��)��ȷ��...");
#endif		
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"4", szPan, NULL, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !(K21_ENABLE||defined SP50||defined SP50G)//K21ƽ̨��֧��æ״̬�ж� lijq, SP50��Ʒ��֧������20170418linying added
	//����������֤����PIN����״̬���豸����æ״̬���������Զ�����,��NDK_SysGoSuspend����PINæ״̬���أ� NDK_ERR_PIN_BUSY = -3107	
#if ANDIROD_ENABLE
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_PIN_BUSY)
#else
	if((ret=NDK_SysGoSuspend())!=NDK_ERR_PIN_BUSY)
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
		if (nStatus == SEC_VPP_KEY_ENTER) {		/*ȷ�ϼ����£��ɷ�������*/
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
	if(memcmp(szPinOut, "\x2e\x40\x00\xef\xf8\x6d\xca\x6a", 8))//\x35\x27\x9a\xf7\x14\xda\xce\x0d
	//if(memcmp(szPinOut, "\x1E\x03\x49\xA2\x7B\x5D\x55\x16", 8))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		GOTOERR;
	}
#if !(defined SP50||defined SP50G)//SP50��Ʒ��֧������20170418linying added
	cls_show_msg("�������,[ȷ�ϼ�]����,Ӧ�ÿ������Ͻ�������,���ߺ��뻽��");//�˴�������Ӧ�������밴����Ч�������ȷ�ϼ������������ߺ����ְ�ȷ�ϼ�ͨ��������ȷ�ϼ���ʶ��ɳ�������������552�г���ʵ�ʼ���Ϊ400d�����������֣��ȷ��
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
		GOTOERR;
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
#endif
}
else
	send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);
if((auto_flag==0)||(auto_flag==1))
{
	//case10:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,nTimeOutMs=MIN
	cls_printf("�벻Ҫ����,���ĵȴ�%ds...", PINTIMEOUT_MIN/1000);
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_SECVP_TIMED_OUT/*NDK_ERR_SECP_TIMEOUT*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case11:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,ucKeyIdx=TAK's ID
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(1, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if SEC_RSA_ENABLE
	//case12:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,ucKeyIdx=RSA's ID
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(6, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case13:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,ucKeyIdx=DUKPT's ID
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPin(7, (uchar *)"4", szPan, szPinOut/*NULL*/, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
}
if((auto_flag==0)||(auto_flag==2))
{
	//case14:��SEC_PIN_ISO9564_0,pszExpPinLenIn=4,pszDataIn!=NULL,Ӧ�ɹ�
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
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"4", szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "\x2e\x40\x00\xef\xf8\x6d\xca\x6a", 8))
	//if(memcmp(szPinOut, "\xE5\x5E\xB5\xED\xC5\xF6\x45\x8F", 8))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		GOTOERR;
	}

	//case15: �ڲ�ΪԤ������λ��ʱ����ȷ��
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
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"6,4", szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "\x98\xec\xab\x1b\xc8\x63\xdf\x00",8))
	//if(memcmp(szPinOut, "\x8E\x99\x8A\x1C\x02\x6F\x05\x21",8))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		GOTOERR;
	}

	//case16: ����Ԥ������λ���޶���Ӧ���ܳ������ֵ
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
	
	if((ret=NDK_SecGetPin(3/*2*/, (uchar *)"8,6,4", szPan, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "\x6b\x76\x24\x43\x71\x63\x9a\xf9",8))
	//if(memcmp(szPinOut, "\xA6\x26\xC1\x9A\xDF\x99\x7D\x69",8))
	{
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, szPinOut[0],szPinOut[1],szPinOut[2],szPinOut[3],szPinOut[4],szPinOut[5],szPinOut[6],szPinOut[7]);
		goto ERR;
	}
}	
else
	send_result("line %d:%s�������������ֶ�����", __LINE__, TESTAPI);

	send_result("%s����ͨ��", TESTAPI);
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}


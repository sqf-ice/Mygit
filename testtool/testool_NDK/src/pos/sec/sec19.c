/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec6.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecGetPinDukpt,NDK_SecGetPinResultDukpt�ӿڹ���
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
#define TESTAPI	"NDK_SecGetPinDukpt,NDK_SecGetPinResultDukpt"

#define PINTIMEOUT_MAX	(200*1000)
#define PINTIMEOUT_MIN	(5*1000)
//#define KT_BDK_TYPE             "B1TE"

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
*						jiangym			20150125	modified
*****************************************************************/
void sec19(void)
{
	int ret = 0, nStatus = 0, i = 0;
	uchar KeyValueIn[17]={0}, szPinOut[8+1]={0}, KSNOUT[11]={0}, KSN1[11]={0},KSN2[11]={0},KSN3[11]={0};
	uchar PinDesOut[9]={0}, PinDesOut1[9]={0};
	ST_SEC_KCV_INFO KcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
#if TR31_ENABLE
	tr31_key_t tr31_st;
	unsigned int len = 0;
	uchar mkey[17]={0},block[512]={0};
#endif
#if SEC_RSA_ENABLE
	ST_SEC_RSA_KEY RsaKeyIn;
	uchar *uModeulus1=(uchar *)"4FB2D08918F766B30B2144FE25D09A160FFCF256086D82773FB04D4BD7E4772F2557FC25EE130A0B984467995BE80EE1751F577BD3E5CAFF60712722F33D0479";
#endif
	uchar *szPan=(uchar *)"6225885916163157";
	uchar szPan1[8] = {0x00,0x00,0x58,0x85,0x91,0x61,0x63,0x15};
	uchar DukptKeyValue[16]={0xD5,0xD4,0x4F,0xF7,0x20,0x68,0x3D,0x0D,0x70,0xBE,0x25,0x28,0x18,0xE2,0xA1,0x8A};//DUKPY��ʼ������Կ	
	uchar DukptPinKeyValue[16]={0x6E,0xF9,0x27,0x95,0xE5,0x12,0x71,0xB2,0xCD,0xEB,0x76,0x7B,0x33,0xF1,0xA9,0x42};//DUKPY��������pin��Կ
	uchar DukptPinKeyValue2[16]={0xba,0xe6,0x98,0xc2,0x15,0x01,0x8d,0x2a,0xe3,0xcf,0x99,0x43,0x3d,0xd1,0x56,0xf1};//idΪ2��DUKPT��Կ��������pin��Կ
	uchar KSN[11]={0};//��ʼKSN

	cls_show_msg1(2, "����%s...", TESTAPI); 

	//����ǰ��:����������Կ
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
			
	//���İ�װһ��DUKPT, ID=6, KSNΪ10���ֽ�0x00 /*Լ��:����KCV���Բ�У�鷽ʽ��װ*/
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(KSN, 0x00, 10);//��ʼKSN
	if ((ret=NDK_SecLoadTIK(6, 0, 16, DukptKeyValue, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��װTLK, ID=1 ����Կ�������İ�װ��ID=2��DUKPT��Կ
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	 //���ں���汾��ֹ��װ��ͬ��DES��Կ (����DES��Կ,DUKPT ��ʼ��ԿTIK) ��������,������ʹ�ò�ͬ����Կ��װʹ��������ƽ̨��Ʒ 20180116 modify
	//���İ�װһ��DUKPT, ID=2, KSNΪ10���ֽ�0x00 \xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23 ���������� ��ID1��Կ���ܺ������(�ֳ�ǰ������8�ֽڼ���)
	//�˴�����������Ϊ:4B82DBD972BBBF20BCBA8632CF9AB57C��16λ0x11���ܺ�Ľ��:1A5DA388BAB978A8AA577F104AF0FC9B
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memcpy(KeyValueIn, "\x1A\x5D\xA3\x88\xBA\xB9\x78\xA8\xAA\x57\x7F\x10\x4A\xF0\xFC\x9B", 16);
	if ((ret=NDK_SecLoadTIK(2, 1, 16, KeyValueIn, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0  //���ﰲװ���id2��id6��һ������Կ 
	//���İ�װһ��DUKPT, ID=2, KSNΪ10���ֽ�0x00 \xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23 ��DukptKeyValue ��ID1��Կ���ܺ������(�ֳ�ǰ������8�ֽڼ���)
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memcpy(KeyValueIn, "\xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23", 16);
	if ((ret=NDK_SecLoadTIK(2, 1, 16, KeyValueIn, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//TR-31���İ�װһ��DUKPT, ID=3, KSNΪ10���ֽ�0x00,������Ľ��  block ��װ��ID=3��  
	memset(mkey, 0x11, 16);
	memset(KSN, 0x00, 10);
	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = 3;
	memcpy(tr31_st.usage, KT_BDK_TYPE, 4);	/*DUKPT*/
	 //���ں���汾��ֹ��װ��ͬ����Կ������ĳ�ʹ�ò�ͬ�����İ�װ20180116 modify by jym
	//��װ�������޸�Ϊ25AB8A24A244883CB2CA76D7617BC60B
	memcpy(DukptKeyValue, "\x25\xAB\x8A\x24\xA2\x44\x88\x3C\xB2\xCA\x76\xD7\x61\x7B\xC6\x0B", 16);
	tr31_st.key = DukptKeyValue;//��DUKPT������Կ����TR31��ʽ���м����õ�block
	tr31_st.keylen = 16;
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��DUKPT������Կ����TR31��ʽ���м����õ�block��Ȼ����а�װ
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadTIK(3, 1, len, block, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	//���İ�װһ��DUKPT, ID=3, KSNΪ10���ֽ� ������ ��ID1��Կ���ܺ������(�ֳ�ǰ������8�ֽڼ���)
	//����25AB8A24A244883CB2CA76D7617BC60B��16λ0x11des���ܽ��(�ֳ�ǰ��8�ֽ�)3CC6B71F7117702C3850C279A2CF834D
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(KSN, 0x00, 10);
	memcpy(KeyValueIn, "\x3C\xC6\xB7\x1F\x71\x17\x70\x2C\x38\x50\xC2\x79\xA2\xCF\x83\x4D", 16);
	if ((ret=NDK_SecLoadTIK(3, 1, 16, KeyValueIn, KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif	

#if SEC_RSA_ENABLE
	//���İ�װRSA, ID=4
	RsaKeyIn.usBits=RSA_KEY_LEN_512;
	strcpy((char *)RsaKeyIn.sExponent, "03");
	memcpy(RsaKeyIn.sModulus, uModeulus1, RsaKeyIn.usBits*2/8);
	if((ret=NDK_SecLoadRsaKey(4, &RsaKeyIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//���İ�װTPK, ID=5
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x21, stKeyInfo.nDstKeyLen);
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &KcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
if(auto_flag==0 || auto_flag==1)
{
	//case3:����ʼ��������ȡ��Ļ����ֵ���򷵻س�ʼ��ʧ��
#if ANDIROD_ENABLE && !defined IM81AZ 
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"4", szPan, KSN, NULL, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_ERR_SECVP_NOT_ACTIVE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinResultDukpt(szPinOut, KSNOUT, &nStatus))!=NDK_ERR_SECVP_NOT_ACTIVE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1:�Ƿ�����
	//case1.1:pszExpPinLenIn==NULL
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinDukpt(2, NULL, szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_PARA)
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
	if((ret=NDK_SecGetPinDukpt(2, (uchar *)"1,2", szPan, KSN,szPinOut, -1, PINTIMEOUT_MIN))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.4:nTimeOutMs>MAX��<MIN
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinDukpt(2, (uchar *)"4", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX+1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinDukpt(2, (uchar *)"4", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1.5:ucGroupIdx������(10,101),��NDK_SecGetPinһ������NDK_ERR_SECCR_GET_KEY
	//��Ӧ����ʾ!zhangnw20130913 cls_printf("��������1234��ȷ��...");
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinDukpt(10, (uchar *)"4", szPan,KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.6:psKsnOut=NULL Ӧ��Ҫʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinDukpt(2, (uchar *)"4", szPan, NULL, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.7:����NDK_SecGetPinResultDukpt �������ΪNULL Ӧ�÷���ʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"4", szPan, KSN, NULL, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinResultDukpt(NULL, KSNOUT, &nStatus))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinResultDukpt(szPinOut, NULL, &nStatus))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinResultDukpt(szPinOut, KSNOUT, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:��SEC_PIN_ISO9564_0,pszExpPinLenIn=0,pszDataIn=NULL, ID=6,Ӧʧ�ܣ�SEC_PIN_ISO9564_0Ҫ��ʹ�����ʺż��ܵ�ͬʱ���ʺ�ΪNULL��Ӧ�÷���NDK_ERR_PARA
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"0", NULL, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
}
if(auto_flag==0 || auto_flag==2)
{
	//case3:��SEC_PIN_ISO9564_0,pszExpPinLenIn=0,pszDataIn!=NULL, ID=6,Ӧ�ɹ�
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
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"0", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if(memcmp(szPinOut, "\x00\x00\x00\x00\x00\x00\x00\x00", 8))	/*�뿪��ȷ�Ϻ�,��pszExpPinLenIn="0"�򷵻�ȫ0*/
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	cls_show_msg1(2, "������ͨ��!");

	//case19:��֤���볤�����Ƴ����ظ�"0,0,0"Ԥ�ڰ�ȷ�Ϸ���NDK_OK��Ӧ�÷��ָ�����°�ȷ�ϻ᷵��ʧ��-1121��BUG����˵��������
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
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"0,0,0", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if(memcmp(szPinOut, "\x00\x00\x00\x00\x00\x00\x00\x00", 8))	/*�뿪��ȷ�Ϻ�,��pszExpPinLenIn="0"�򷵻�ȫ0*/
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	cls_show_msg1(2, "������ͨ��!");
	
	//case4:��SEC_PIN_ISO9564_1,pszExpPinLenIn=4,pszDataIn=NULL, ID=2,Ӧ�ɹ�
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
	if((ret=NDK_SecGetPinDukpt(2, (uchar *)"4,4"/*"4"*/, NULL, KSN, szPinOut, SEC_PIN_ISO9564_1, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//idΪ2��ʼ��Կ�ı�,��������pin��ԿҲ�ı�Ϊ:bae698c215018d2ae3cf99433dd156f1
	//���������ͨ��des���ܺ���Եõ�����,���ĵ�ǰ��λ�����ǹ̶��Ŀ����ж�
	if((ret=NDK_AlgTDes(szPinOut, PinDesOut, DukptPinKeyValue2, sizeof(DukptPinKeyValue2), ALG_TDS_MODE_DEC)) !=NDK_OK)
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

#if 1 /*���������޸�,��������벻Ӧ�����趨��ֵ�������δ�����*/
	//case12:��SEC_PIN_ISO9564_2,pszExpPinLenIn=6,pszDataIn=NULL, ID=3,Ӧ�ɹ�
	memset(szPinOut, 0, sizeof(szPinOut));
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
	if((ret=NDK_SecGetPinDukpt(3, (uchar *)"6", NULL, KSN, szPinOut, SEC_PIN_ISO9564_2, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//�޸�Ϊʹ��id3��ʼ��Կ������PIN��Կ9b53578c8110f739f3bf8ca3bbff63a7��SEC_PIN_ISO9564_2��ʽPINBLOCK 26123456FFFFFFFF����3DES���ܵó�3983B8E7E8A856AA
	/*if(memcmp(szPinOut, "\xC0\x86\x6C\x84\x43\x86\x9A\xED", 8))*/
	if(memcmp(szPinOut, "\x39\x83\xB8\xE7\xE8\xA8\x56\xAA", 8))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

	//case5:��SEC_PIN_ISO9564_3,pszExpPinLenIn=12,pszDataIn!=NULL, ID=6,Ӧ�ɹ�	
	memset(szPinOut, 0, sizeof(szPinOut));
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
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"12", szPan, KSN, szPinOut, SEC_PIN_ISO9564_3, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//���������ͨ�����ܵó�����,�������뿨��������Եó��̶���ǰ��λ��
	if((ret=NDK_AlgTDes(szPinOut, PinDesOut1, DukptPinKeyValue, sizeof(DukptPinKeyValue), ALG_TDS_MODE_DEC)) !=NDK_OK)
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
	//����Ľ��:��һλ��ʾ����ģʽΪSEC_PIN_ISO9564_3,�ڶ�λ��ʾ������12λ(c),����������123456789012ʣ����������
	if(memcmp(PinDesOut, "\x3c\x12\x34\x56\x78\x90\x12", 7))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	cls_show_msg1(2, "������ͨ��!");

	//case6:��SEC_PIN_ISO9564_0,pszExpPinLenIn=13,pszDataIn!=NULL, ID=6,Ӧʧ��
	//cls_printf("��������1234567890123��ȷ��...");
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"13", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:��SEC_PIN_ISO9564_0,pszExpPinLenIn=12,pszDataIn!=NULL, ID=6
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
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"12", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_ERR_SECVP_GET_ESC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case8:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4,5,12",pszDataIn!=NULL, ID=6
	memset(szPinOut, 0, sizeof(szPinOut));
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
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"4,5,12", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ý��Ϊʹ��16�ֽ�PIN��Կ 6ef92795e51271b2cdeb767b33f1a942���ʺź���Կ����PIN���ܼ���ó� 69C1207610BB425F
	if(memcmp(szPinOut, "\x69\xC1\x20\x76\x10\xBB\x42\x5F", 8))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case9:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL, ID=6,NDK_SecGetPinResultDukpt()������ȡ PINBLOCK 
	memset(szPinOut, 0, sizeof(szPinOut));
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
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"4", szPan, KSN, NULL, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0
	if((ret=NDK_SecGetPinResult(szPinOut, &nStatus))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	do {
		ret= NDK_SecGetPinResultDukpt(szPinOut, KSNOUT, &nStatus);
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
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nStatus);
			goto ERR;
			break;
		}
		NDK_SysMsDelay(10);//usleep(10*1000);
	} while(1);
#endif
	//�ý��Ϊʹ��16�ֽ�PIN��Կ 6ef92795e51271b2cdeb767b33f1a942���ʺź���Կ����PIN���ܼ���ó� F0828F1547B362AD
	if(memcmp(szPinOut, "\xF0\x82\x8F\x15\x47\xB3\x62\xAD", 8))
	{		
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case10:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,nTimeOutMs=0, ID=6
	//cls_printf("��������4321��ȷ��...");    //nTimeOutMs=0 ���ز���ʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"4", szPan, KSN, NULL, SEC_PIN_ISO9564_0, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case13:��SEC_PIN_ISO9564_0,pszExpPinLenIn=4,pszDataIn!=NULL, ID=6,Ӧ�ɹ�
	memset(szPinOut, 0, sizeof(szPinOut));
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
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"4", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ý��Ϊʹ��16�ֽ�PIN��Կ 6ef92795e51271b2cdeb767b33f1a942���ʺź���Կ����PIN���ܼ���ó� 756000DE335639DF
	if(memcmp(szPinOut, "\x75\x60\x00\xDE\x33\x56\x39\xDF", 8))
	{	
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case14:��֤GETPIN��,KSN����
	//getksn
	if((ret=NDK_SecGetDukptKsn(2, KSN1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//getpin
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������1234��ȷ��...");
#else
	cls_printf("��������1234��ȷ��...");
#endif
	if((ret=NDK_SecGetPinDukpt(2, (uchar *)"4", szPan, KSN2, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//getksn
	if((ret=NDK_SecGetDukptKsn(2, KSN3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(KSN1, KSN2, 10)||memcmp(KSN1, KSN3, 10))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

#if SEC_RSA_ENABLE
	//case15:ID=RSA's ID,Ӧʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������1��ȷ��...");
#else
	cls_printf("��������1��ȷ��...");
#endif
	if((ret=NDK_SecGetPinDukpt(4, (uchar *)"1", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY/*NDK_ERR_SECVP_GENERALERROR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case16:ID=TPK's ID,Ӧʧ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������1��ȷ��...");
#else
	cls_printf("��������1��ȷ��...");
#endif
	if((ret=NDK_SecGetPinDukpt(5, (uchar *)"1", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN))!=NDK_ERR_SECCR_GET_KEY/*NDK_ERR_SECVP_GENERALERROR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case17: �ڲ�ΪԤ������λ��ʱ����ȷ��
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
	if((ret=NDK_SecGetPinDukpt(2, (uchar *)"6,4", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case18: ����Ԥ������λ���޶���Ӧ���ܳ������ֵ
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
	if((ret=NDK_SecGetPinDukpt(2, (uchar *)"8,6,4", szPan, KSN, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case19:KSN�ԼӺ���Ӧ������ǰ����Կ���Զ����£�����KSNҪ�Զ�����
	if ((ret=NDK_SecIncreaseDukptKsn(6))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ԼӺ�����ǰ��Կ���:5635b184dd5d8f415617b6600c6e47cf���ɴ�������pin�ӽ�����Կ5635b184dd5d8fbe5617b6600c6e4730
	memset(szPinOut, 0, sizeof(szPinOut));
	memset(KSN1, 0x00, sizeof(KSN1));
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	NDK_ScrPrintf("��������123456��ȷ��...");
#else
	cls_printf("��������123456��ȷ��...");
#endif
	if((ret=NDK_SecGetPinDukpt(6, (uchar *)"6", szPan, KSN1, szPinOut, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��Ԥ�ڽ��ʹ��16�ֽ���Կ5635b184dd5d8fbe5617b6600c6e4730 �뿨�� �������ó� E2EB3D877C330126
	if(memcmp(szPinOut, "\xE2\xEB\x3D\x87\x7C\x33\x01\x26", 8))
	{	
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if( KSN1[7]!=0x00 || KSN1[8]!=0x00 || KSN1[9]!=0x02)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
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


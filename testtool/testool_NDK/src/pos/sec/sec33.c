/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec33.c
* Author 		: linwl
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����Ndk_getVKeybPin�ܷ������������,�Ӽ��̶��밴������PIN����,��������PINBLOCK�Ľ���ͻ�(DUKPT��Կ��ϵ)
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
#define TESTAPI	"Ndk_getVKeybPin"

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
*			  	  	 		linwl       20160720	created
*****************************************************************/
void sec33(void)
{
	int ret = 0, i = 0;
	uchar KeyValueIn[17]={0}, KSN1[11]={0}, KSN2[11]={0}, KSN3[11]={0};//, KSNOUT[11]={0}
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
	char *szPan="6225885916163157";
	char szPan1[8] = {0x00,0x00,0x58,0x85,0x91,0x61,0x63,0x15}, szPinOutStr[8+1]={0}, szPinOut[16+1]={0};
	uchar DukptKeyValue[16]={0xD5,0xD4,0x4F,0xF7,0x20,0x68,0x3D,0x0D,0x70,0xBE,0x25,0x28,0x18,0xE2,0xA1,0x8A};//DUKPY��ʼ������Կ	
	uchar DukptPinKeyValue[16]={0x6E,0xF9,0x27,0x95,0xE5,0x12,0x71,0xB2,0xCD,0xEB,0x76,0x7B,0x33,0xF1,0xA9,0x42};//DUKPY��������pin��Կ
	char KSN[11]={0};//��ʼKSN

	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}

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
	if ((ret=NDK_SecLoadTIK(6, 0, 16, DukptKeyValue, (uchar *)KSN, &KcvInfoIn))!=NDK_OK)
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
	
	//���İ�װһ��DUKPT, ID=2, KSNΪ10���ֽ�0x00 \xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23 ��DukptKeyValue ��ID1��Կ���ܺ������(�ֳ�ǰ������8�ֽڼ���)
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memcpy(KeyValueIn, "\xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23", 16);
	if ((ret=NDK_SecLoadTIK(2, 1, 16, KeyValueIn, (uchar *)KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if TR31_ENABLE	//֧��TR31��ʽ��Կ��װ���豸�Ž���TR31����
	//TR-31���İ�װһ��DUKPT, ID=3, KSNΪ10���ֽ�0x00,������Ľ��  block ��װ��ID=3��  
	memset(mkey, 0x11, 16);
	memset(KSN, 0x00, 10);
	memset(&tr31_st, 0, sizeof(tr31_st));
	tr31_st.gid = 3;
	memcpy(tr31_st.usage, KT_BDK_TYPE, 4);	/*DUKPT*/
	tr31_st.key = DukptKeyValue;//��DUKPT������Կ����TR31��ʽ���м����õ�block
	tr31_st.keylen = 16;
	if((ret=sec_tr31block_format(block, &len,  mkey, 16, &tr31_st))!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��DUKPT������Կ����TR31��ʽ���м����õ�block��Ȼ����а�װ
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadTIK(3, 1, len, block, (uchar *)KSN, &KcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	//���İ�װһ��DUKPT, ID=3, KSNΪ10���ֽ�0x00 \xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23 ��DukptKeyValue ��ID1��Կ���ܺ������(�ֳ�ǰ������8�ֽڼ���)
	KcvInfoIn.nCheckMode=SEC_KCV_NONE;
	memset(KSN, 0x00, 10);
	memcpy(KeyValueIn, "\xDA\x36\xEC\x83\x4A\xDE\x38\xD9\x5A\x51\x68\xF9\x5E\x9A\x5E\x23", 16);
	if ((ret=NDK_SecLoadTIK(3, 1, 16, KeyValueIn, (uchar *)KSN, &KcvInfoIn))!=NDK_OK)
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

	//case1:�Ƿ�����
	//case1.1:pszExpPinLenIn==NULL
	if((ret=Ndk_getVKeybPin(NULL, 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, KSN, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=Ndk_getVKeybPin("0", 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, NULL, KSN, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=Ndk_getVKeybPin("0", 2, SEC_PIN_ISO9564_1, PINTIMEOUT_MIN, NULL, KSN, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=Ndk_getVKeybPin("4", 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, KSN, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case1.2:uckeyIdx������
	if((ret=Ndk_getVKeybPin("4", 10, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, KSN, szPinOut))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case1.3:ucMod �Ƿ�
	if((ret=Ndk_getVKeybPin("4", 2, -1, PINTIMEOUT_MIN, szPan, KSN, szPinOut))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.4:nTimeOutMs>MAX��nTimeOutMs<MIN
	if((ret=Ndk_getVKeybPin("4", 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX+1, szPan, KSN, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=Ndk_getVKeybPin("4", 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN-1, szPan, KSN, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.5:��SEC_PIN_ISO9564_0,pszExpPinLenIn=13,pszDataIn!=NULL,Ӧʧ��
	//cls_printf("��������1234567890123��ȷ��...");
	if((ret=Ndk_getVKeybPin("13", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, KSN, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.7:����NDK_SecGetPinResultDukpt �������ΪNULL Ӧ�÷���ʧ��
	if((ret=Ndk_getVKeybPin("4", 6, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:��SEC_PIN_ISO9564_0,pszExpPinLenIn=0,pszDataIn!=NULL, ID=6,Ӧ�ɹ�
	cls_printf("\n�뾡�찴[ȷ��]...");
	if((ret=Ndk_getVKeybPin("0", 6, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, KSN, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if(memcmp(szPinOut, "0000000000000000", 16))	/*�뿪��ȷ�Ϻ�,��pszExpPinLenIn="0"�򷵻�ȫ0*/
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	cls_show_msg1(2, "������ͨ��!");

	//case19:��֤���볤�����Ƴ����ظ�"0,0,0"Ԥ�ڰ�ȷ�Ϸ���NDK_OK��Ӧ�÷��ָ�����°�ȷ�ϻ᷵��ʧ��-1121��BUG����˵��������
	cls_printf("\n�뾡�찴[ȷ��]...");
	if((ret=Ndk_getVKeybPin("0,0,0", 6, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, KSN, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if(memcmp(szPinOut, "0000000000000000", 16))	/*�뿪��ȷ�Ϻ�,��pszExpPinLenIn="0"�򷵻�ȫ0*/
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	cls_show_msg1(2, "������ͨ��!");
	
	//case4:��SEC_PIN_ISO9564_1,pszExpPinLenIn=4,pszDataIn=NULL, ID=2,Ӧ�ɹ�
	cls_printf("\n��������123��ȷ��,���޷�Ӧ������4��ȷ��...");
	if((ret=Ndk_getVKeybPin("4,4", 2, SEC_PIN_ISO9564_1, PINTIMEOUT_MAX, NULL, KSN, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_AscToHex((uchar *)szPinOut,strlen((char *)szPinOut),0,(uchar *)szPinOutStr);//�����Ӳ������ַ���szPinOutת����HEX����м���
	//���������ͨ��des���ܺ���Եõ�����,���ĵ�ǰ��λ�����ǹ̶��Ŀ����ж�
	if((ret=NDK_AlgTDes((uchar *)szPinOutStr, PinDesOut, DukptPinKeyValue, sizeof(DukptPinKeyValue), ALG_TDS_MODE_DEC)) !=NDK_OK)
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

	//case12:��SEC_PIN_ISO9564_2,pszExpPinLenIn=6,pszDataIn=NULL, ID=3,Ӧ�ɹ�
	memset(szPinOut, 0, sizeof(szPinOut));
	cls_printf("\n�볢������1237890��,�粻���������˸��Ϊ123456��ȷ��...");
	if((ret=Ndk_getVKeybPin("6", 3, SEC_PIN_ISO9564_2, PINTIMEOUT_MAX, NULL, KSN, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	//�ý��Ϊʹ��16�ֽ�PIN��Կ 6ef92795e51271b2cdeb767b33f1a942 ��SEC_PIN_ISO9564_2��ʽPINBLOCK 26123456FFFFFFFF����3DES���ܵó� C0866C8443869AED 
	if(memcmp(szPinOut, "C0866C8443869AED", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case5:��SEC_PIN_ISO9564_3,pszExpPinLenIn=12,pszDataIn!=NULL, ID=6,Ӧ�ɹ�	
	memset(szPinOut, 0, sizeof(szPinOut));
	cls_printf("\n��������123456789012��ȷ��...");
	if((ret=Ndk_getVKeybPin("12", 6, SEC_PIN_ISO9564_3, PINTIMEOUT_MAX, szPan, KSN, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_AscToHex((uchar *)szPinOut,strlen((char *)szPinOut),0,(uchar *)szPinOutStr);
	//���������ͨ�����ܵó�����,�������뿨��������Եó��̶���ǰ��λ��
	if((ret=NDK_AlgTDes((uchar *)szPinOutStr, PinDesOut1, DukptPinKeyValue, sizeof(DukptPinKeyValue), ALG_TDS_MODE_DEC)) !=NDK_OK)
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
	if((ret=Ndk_getVKeybPin("13", 6, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, KSN, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:��SEC_PIN_ISO9564_0,pszExpPinLenIn=12,pszDataIn!=NULL, ID=6
	cls_printf("\n��������123456����2��ȡ��...");
	if((ret=Ndk_getVKeybPin("12", 6, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, KSN, szPinOut))!=NDK_ERR_QUIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case8:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4,5,12",pszDataIn!=NULL, ID=6
	memset(szPinOut, 0, sizeof(szPinOut));
	cls_printf("\n��������12345��ȷ��...");
	if((ret=Ndk_getVKeybPin("4,5,12", 6, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, KSN, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ý��Ϊʹ��16�ֽ�PIN��Կ 6ef92795e51271b2cdeb767b33f1a942���ʺź���Կ����PIN���ܼ���ó� 69C1207610BB425F
	if(memcmp(szPinOut, "69C1207610BB425F", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case9:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL, ID=6,NDK_SecGetPinResultDukpt()������ȡ PINBLOCK 
	memset(szPinOut, 0, sizeof(szPinOut));
	cls_printf("\n��������4321(�����������밴��)��ȷ��...");
	if((ret=Ndk_getVKeybPin("4", 6, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, KSN, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ý��Ϊʹ��16�ֽ�PIN��Կ 6ef92795e51271b2cdeb767b33f1a942���ʺź���Կ����PIN���ܼ���ó� F0828F1547B362AD
	if(memcmp(szPinOut, "F0828F1547B362AD", 16))
	{		
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case10:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,nTimeOutMs=0, ID=6
	//cls_printf("��������4321��ȷ��...");    //nTimeOutMs=0 ���ز���ʧ��
	if((ret=Ndk_getVKeybPin("4", 6, SEC_PIN_ISO9564_0, 0, szPan, KSN, NULL))!=NDK_ERR_SECVP_TIMED_OUT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case13:��SEC_PIN_ISO9564_0,pszExpPinLenIn=4,pszDataIn!=NULL, ID=6,Ӧ�ɹ�
	memset(szPinOut, 0, sizeof(szPinOut));
	cls_printf("\n��������4321��,�ٰ�1��ȡ��,������1234��ȷ��...");
	if((ret=Ndk_getVKeybPin("4", 6, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, KSN, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ý��Ϊʹ��16�ֽ�PIN��Կ 6ef92795e51271b2cdeb767b33f1a942���ʺź���Կ����PIN���ܼ���ó� 756000DE335639DF
	if(memcmp(szPinOut, "756000DE335639DF", 16))
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
	cls_printf("\n��������1234��ȷ��...");
	if((ret=Ndk_getVKeybPin("4", 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, (char *)KSN2, szPinOut))!=NDK_OK)
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
	cls_printf("\n��������1��ȷ��...");
	if((ret=Ndk_getVKeybPin("1", 4, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, KSN, szPinOut))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case16:ID=TPK's ID,Ӧʧ��
	cls_printf("\n��������1��ȷ��...");
	if((ret=Ndk_getVKeybPin("1", 5, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, KSN, szPinOut))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case17: �ڲ�ΪԤ������λ��ʱ����ȷ��
	cls_printf("\n��������12345��ȷ��,���޷�Ӧ������6����ȷ��");
	if((ret=Ndk_getVKeybPin("6,4", 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, KSN, szPinOut))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case18: ����Ԥ������λ���޶���Ӧ���ܳ������ֵ
	cls_printf("\n�볢������123456789,Ӧ�޷�����9,�ٰ�ȷ��");
	if((ret=Ndk_getVKeybPin("8,6,4", 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, KSN, szPinOut))!=NDK_OK)
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
	cls_printf("\n��������123456��ȷ��...");
	if((ret=Ndk_getVKeybPin("6", 6, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, (char *)KSN1, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��Ԥ�ڽ��ʹ��16�ֽ���Կ5635b184dd5d8fbe5617b6600c6e4730 �뿨�� �������ó� E2EB3D877C330126
	if(memcmp(szPinOut, "E2EB3D877C330126", 16))
	{	
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if( KSN1[7]!=0x00 || KSN1[8]!=0x00 || KSN1[9]!=0x02)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}


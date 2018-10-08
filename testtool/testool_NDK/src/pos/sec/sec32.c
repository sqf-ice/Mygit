/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec32.c
* Author 		: linwl
* version		: 
* DATE			: 20160720
* directory 		: 
* description		: ����Ndk_getVKeybPin�ܷ������������,�Ӽ��̶��밴������PIN����,��������PINBLOCK�Ľ���ͻ�(MK/SK��Կ��ϵ)
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
*			  	  	   		linwl       20160720	created
*****************************************************************/
void sec32(void)
{
	int ret = 0, i = 0;
	tr31_key_t tr31_st;
	unsigned int len = 0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	ST_EXTEND_KEYBLOCK *p;
	uchar sKeyValueIn[17]={0}, sKsnIn[11]={0}, mkey[17]={0}, skey[17]={0}, block[512]={0};
	char szPinOutStr[8+1]={0}, szPinOut[16+1]={0}, szExpPinLenIn[100+1]={0};
	char *szPan="6225885916163157";	
	char szPan1[8] = {0x00,0x00,0x58,0x85,0x91,0x61,0x63,0x15};
#if SEC_RSA_ENABLE
	ST_SEC_RSA_KEY RsaKey;
	char *Pmod=(uchar *)"66C2BCD4AC436815C193A8D29A0755C80393D7AFC7D78B12FE371F91874D449271207DFA6A054D185B03E543137C59577769940830FEEA040053F72583B85951";
#endif
	uchar PinKeyValue[8]={0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15};
	uchar PinKeyValue1[24]={0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19};
	uchar PinDesOut[9]={0},PinDesOut1[9]={0};
	//char KSN[11]={0};//��ʼKSN
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	cls_show_msg1(2, "����%s...��ȷ����׿ϵͳ�Ѿ���װ���������APK", TESTAPI); 

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

#if !OVERSEAS_ENABLE
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

	//��װTPK2(16bytes), ID=3,TR-31��ʽ��װ
	stKeyInfo.ucScrKeyIdx=5;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=sizeof(ST_EXTEND_KEYBLOCK);
	memset(mkey, 0x11, 16);/*��TMK��ɢ*/
	//memset(skey, 0x17, 16); /*TPK2, ��Կ����16���ֽ�0x17*/
	memset(skey, 0x17, 8);	/*TAK2��Կ����:8���ֽڵ�0x17+8���ֽ�0x19*/
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

	//case1:�Ƿ�����
	//case1.1:pinlen, account, KSN, pinblock��4������ΪNULL�����Ӧ�÷��ز�������-4022
	strcpy((char *)szExpPinLenIn, "4,5,6,7,8,9,10,11,12");
	if((ret=Ndk_getVKeybPin(NULL, 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, NULL, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=Ndk_getVKeybPin("0", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, NULL, NULL, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=Ndk_getVKeybPin("0", 3, SEC_PIN_ISO9564_1, PINTIMEOUT_MIN, NULL, NULL, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=Ndk_getVKeybPin("13", 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, NULL, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=Ndk_getVKeybPin("4", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, NULL, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case1.2:uckeyIdx������
	if((ret=Ndk_getVKeybPin(szExpPinLenIn, 10, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, NULL, szPinOut))!=NDK_ERR_SECCR_GET_KEY)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.3:ucMod �Ƿ�
	if((ret=Ndk_getVKeybPin(szExpPinLenIn, 2, -1, PINTIMEOUT_MIN, szPan, NULL, szPinOut))!=NDK_ERR_SECP_PARAM)   
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.4:nTimeOutMs>MAX��nTimeOutMs<MIN
	if((ret=Ndk_getVKeybPin(szExpPinLenIn, 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX+1, szPan, NULL, szPinOut))!=NDK_ERR_PARA)	 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=Ndk_getVKeybPin(szExpPinLenIn, 2, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN-1, szPan, NULL, szPinOut))!=NDK_ERR_PARA)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1.5:��SEC_PIN_ISO9564_0,pszExpPinLenIn=13,pszDataIn!=NULL,Ӧʧ��
	//cls_printf("��������1234567890123��ȷ��...");
	if((ret=Ndk_getVKeybPin("13", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:��SEC_PIN_ISO9564_0,pszExpPinLenIn=0,pszDataIn!=NULL,Ӧ�ɹ�
	cls_printf("\n�벻Ҫ�����κβ����ȴ���׿ϵͳ�Զ���������");
	if((ret=Ndk_getVKeybPin("0", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_ERR_QUIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(10, "������ͨ��!�뾡�컽��POS");//���컽��POS������������״̬����������̻᷵��-11

	//case3:��֤���볤�����Ƴ����ظ�"0,0,0"Ԥ�ڰ�ȷ�Ϸ���NDK_OK��Ӧ�÷��ָ�����°�ȷ�ϻ᷵��ʧ��-1121��BUG����˵��������
	cls_printf("\n�뾡�찴[ȷ��]...");
	if((ret=Ndk_getVKeybPin("0,0,0", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(memcmp(szPinOut, "0000000000000000", 16))	/*�뿪��ȷ�Ϻ�,��pszExpPinLenIn="0"�򷵻�ȫ0*/
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, szPinOut);
		GOTOERR;
	}
	
	//case4:��SEC_PIN_ISO9564_1,pszExpPinLenIn=4,pszDataIn=NULL,Ӧ�ɹ�(SEC_PIN_ISO9564_1 ģʽ��ʹ����������ʲ����Ƚ�)
	cls_printf("\n��������123��ȷ��,���޷�Ӧ������4��ȷ��...");
	if((ret=Ndk_getVKeybPin("4,4", 2, SEC_PIN_ISO9564_1, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_AscToHex((uchar *)szPinOut,strlen((char *)szPinOut),0,(uchar *)szPinOutStr);//�����Ӳ������ַ���szPinOutת����HEX����м���
	//���������ͨ��des���ܺ���Եõ�����,���ĵ�ǰ��λ�����ǹ̶��Ŀ����ж� added jiangym 20150125
	if((ret=NDK_AlgTDes((uchar *)szPinOutStr, PinDesOut, PinKeyValue, sizeof(PinKeyValue), ALG_TDS_MODE_DEC)) !=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����Ϊ141234xxxxxxxxxx (16λ)��һλ��ʾ����ģʽΪSEC_PIN_ISO9564_1,�ڶ�λ��ʾ������4λ,����������1234ʣ����������
	if(memcmp(PinDesOut, "\x14\x12\x34", 3))
	{
		send_result("line %d:%s����ʧ��0x%2x,0x%2x,0x%2x", __LINE__, TESTAPI, PinDesOut[0], PinDesOut[1], PinDesOut[2]);
		GOTOERR;
	}

	//case5:��SEC_PIN_ISO9564_2,pszExpPinLenIn=6,pszDataIn=NULL,Ӧ�ɹ�
	cls_printf("\n�볢������1237890��,�粻���������˸��Ϊ123456��ȷ��...");
	if((ret=Ndk_getVKeybPin("6", 3, SEC_PIN_ISO9564_2, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//���ʹ��8�ֽ�0x17+8�ֽ�0x19��26123456FFFFFFFF������Ϊ:\xe2\x9c\x72\xc1\x6\x43\x53\x48
	if(memcmp(szPinOut,"E29C72C106435348",16))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, szPinOut);
		GOTOERR;
	}
	
	//case6:��SEC_PIN_ISO9564_3,pszExpPinLenIn=12,pszDataIn!=NULL,Ӧ�ɹ�(SEC_PIN_ISO9564_3 ģʽ��ʹ����������ʲ����Ƚ�)
	cls_printf("\n��������123456789012��ȷ��...");
	if((ret=Ndk_getVKeybPin("12", 4, SEC_PIN_ISO9564_3, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_AscToHex((uchar *)szPinOut,strlen((char *)szPinOut),0,(uchar *)szPinOutStr);
	//���������ͨ�����ܵó�����,�������뿨��������Եó��̶���ǰ��λ��  added jiangym 20150125
	if((ret=NDK_AlgTDes((uchar *)szPinOutStr, PinDesOut1, PinKeyValue1, sizeof(PinKeyValue1), ALG_TDS_MODE_DEC)) !=NDK_OK)
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
		send_result("line %d:%s����ʧ��(%x,%x,%x,%x,%x,%x,%x,%x)", __LINE__, TESTAPI, PinDesOut[0],PinDesOut[1],PinDesOut[2],PinDesOut[3],PinDesOut[4],PinDesOut[5],PinDesOut[6],PinDesOut[7]);
		GOTOERR;
	}
	cls_show_msg1(2, "������ͨ��!");

	//case7:��SEC_PIN_ISO9564_0,pszExpPinLenIn=12,pszDataIn!=NULL
	cls_printf("\n��������123456����2��ȡ��...");
	if((ret=Ndk_getVKeybPin("12", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_ERR_QUIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��������
	cls_show_msg1(2, "������ͨ��!��Ҫ�ٰ�[ȡ��]");

	//case8:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4,5,12",pszDataIn!=NULL
	cls_printf("\n��������12345��ȷ��...");
	if((ret=Ndk_getVKeybPin("4,5,12", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "E9F2EDDD59857FD0", 16))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, szPinOut);
		GOTOERR;
	}

	//case9:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,nTimeOutMs=MIN
	cls_printf("\n�벻Ҫ����,���ĵȴ�%ds...", PINTIMEOUT_MIN/1000);
	if((ret=Ndk_getVKeybPin("4", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, NULL, szPinOut))!=NDK_ERR_SECVP_TIMED_OUT)	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case10:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,ucKeyIdx=TAK's ID
	if((ret=Ndk_getVKeybPin("4", 1, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, NULL, szPinOut))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if SEC_RSA_ENABLE
	//case11:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,ucKeyIdx=RSA's ID
	if((ret=Ndk_getVKeybPin("4", 6, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, NULL, szPinOut))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case12:��SEC_PIN_ISO9564_0,pszExpPinLenIn="4",pszDataIn!=NULL,psPinBlockOut=NULL,ucKeyIdx=DUKPT's ID
	if((ret=Ndk_getVKeybPin("4", 7, SEC_PIN_ISO9564_0, PINTIMEOUT_MIN, szPan, NULL, szPinOut))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case13:��SEC_PIN_ISO9564_0,pszExpPinLenIn=4,pszDataIn!=NULL,Ӧ�ɹ�
	cls_printf("\n��������4321��,�ٰ�1��ȡ��,������1234��ȷ��...");
	if((ret=Ndk_getVKeybPin("4", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "2E4000EFF86DCA6A", 16))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, szPinOut);
		GOTOERR;
	}

	//case14: �ڲ�ΪԤ������λ��ʱ����ȷ��
	cls_printf("\n��������12345��ȷ��,���޷�Ӧ������6����ȷ��");
	if((ret=Ndk_getVKeybPin("6,4", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "98ECAB1BC863DF00",16))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, szPinOut);
		GOTOERR;
	}

	//case15: ����Ԥ������λ���޶���Ӧ���ܳ������ֵ
	cls_printf("\n�볢������123456789,Ӧ�޷�����9,�ٰ�ȷ��");
	if((ret=Ndk_getVKeybPin("8,6,4", 3, SEC_PIN_ISO9564_0, PINTIMEOUT_MAX, szPan, NULL, szPinOut))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(szPinOut, "6B76244371639AF9",16))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, szPinOut);
		goto ERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR://���Ժ���:����������Կ
	NDK_SecKeyErase();
	return;
}


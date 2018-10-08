/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec6.c
* Author 		: linrq
* version		: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_SecSetKeyOwner�����µ�Ӧ����Կ�����ù�����Կ��ӿڹ���
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
#define TESTAPI	"NDK_SecSetKeyOwner"
#define		SUBAPP1_NAME	"a@#^+,.[}��_14"
#define 	SEC_SUBAPP_NLD1	"a@#^+,.[}��_14.NLD" //���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ�������� 20170621

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
*			  	  	        linrq         20130228	      created
						jiangym      20140814		modify
*****************************************************************/
void sec14(void)
{
	int ret = -1/*, MAXCNT = 5000, i = 0*/;
	char ab_name[257]={0};
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar udesout[31]={0}, udesin[31]={0};
	//char sKey[8+1]={0x34,0x78,0x12,0x67,0x93,0x11,0x54,0x28};
	//uchar sOutData[8+1]={0};
	//uchar szBuf[]={"879423714192374092137"};

	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
#if !ANDIROD_ENABLE
	if(cls_show_msg1(30, "����%s...,��ȷ��%s����,ESC�˳�", TESTAPI, SEC_SUBAPP_NLD1)==ESC)
		return;
#else
	cls_printf("����%s...",TESTAPI);
#endif

	//����ǰ��:����������Կ
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#if ANDIROD_ENABLE
	if((ret = NDK_SecSetKeyOwner("")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#endif	
	//�ȶ�Ӧ���������Կ�����װ����Կ
	memset(udesin, 0x20, sizeof(udesin));

	//��װTDK3(��Կ8�ֽ�),ID=3
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16/*8*/;  //�����Ʒ��֧��8λ��Կ��ȫ���ĳ�16λ��װ 20180419 sull modify
	memset(stKeyInfo.sDstKeyValue, 0x35, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��װTPK(��Կ16�ֽ�), ID=6
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK;
	stKeyInfo.ucDstKeyIdx=6;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x1b, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1:�Ƿ����Դ���������ݳ��ȴ���255
	memset(ab_name, 0x32, 256);
#if ANDIROD_ENABLE
	if((ret = NDK_SecSetKeyOwner(ab_name)) != NDK_ERR_SECP_PARAM)
#else
	if((ret=NDK_SecSetKeyOwner(ab_name))!=NDK_ERR_PARA)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:�л���������Կ��,����*���͵���Կ�� ������Կ�İ�װ
	if((ret=NDK_SecSetKeyOwner("*"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:��֤share��Ȩ�޲��ᱻ�޸�,�˴����ø߰�ȫ�����,�������ԵͰ�ȫ����Ӧ�û��ǳɹ�
#if !ANDIROD_ENABLE  //ANDROID�޷����ø߰�ȫ����
	if(( ret = NDK_SecSetCfg(0x1F))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//share��װTMK, ID=5
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=5;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK;
	stKeyInfo.nDstKeyLen=16/*8*/; 
	memset(stKeyInfo.sDstKeyValue, 0x11, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//share��װ��ԿID2��TDK2(��Կ8�ֽ�),ID=2,0x13��
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16/*8*/; 
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:��װ��Կ��ͬ��TDK:ID9��ID2��ͬ,��֤share��� ��������ԿΩһ��
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=9;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16/*8*/; 
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//share��װTPK, ID=8
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK;
	stKeyInfo.ucDstKeyIdx=8;
	stKeyInfo.nDstKeyLen=16/*8*/; 
	memset(stKeyInfo.sDstKeyValue, 0x1b, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5����֤share�����к�Ӧ�ñ���������Կ�飺share��TDK2 DES����Ӧ�óɹ�,��TDK3 DES����Ӧ��ʧ��
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 16/*8*/, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*SEC_DES_KEYLEN_8*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 16/*8*/))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 16/*8*/, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*SEC_DES_KEYLEN_8*/))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6:share�����Կר�������ƹرգ�������TLK/TPK/TMK/TAK/TDK����Կִ��des�ӽ�������	
#if !ANDIROD_ENABLE  //ANDROID��֧�֣�ֻ֧��data�ӽ���
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TMK, 5, udesin, 16/*8*/, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*SEC_DES_KEYLEN_8*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(udesout, "\x77\xC4\xB0\xF4\x22\x56\xD2\x84\x77\xC4\xB0\xF4\x22\x56\xD2\x84", 16/*8*/))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif
/*
	//case7:share������д������ƹر�,���Խ��������Ľ���
	//��װTAK�������з������Ʋ���
	memset(&stKeyInfo,0,sizeof(stKeyInfo));
	memset(&stKcvInfoIn,0,sizeof(stKcvInfoIn));
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK;
	stKeyInfo.ucDstKeyIdx = 1;
	stKeyInfo.nDstKeyLen = 8;
	memcpy(stKeyInfo.sDstKeyValue,sKey,8);
	if ((ret=NDK_SecLoadKey (&stKeyInfo,&stKcvInfoIn))!=NDK_OK)
	{
		cls_show_msg("line %d:NDK_SecLoadKeyװ����Կʧ��(ret=%d)", __LINE__ , ret);
		GOTOERR;
	}
	cls_printf("���з��������Ʋ����У����Ե�!");
	for(i=0; i<MAXCNT; i++)
	{
		//cls_printf("���з��񳬴β�����,��%d��MAC������...", i+1);
		if(NDK_SecGetMac(1,szBuf,sizeof(szBuf),sOutData,SEC_MAC_ECB)!=NDK_OK)
		{
			cls_show_msg("���з��񳬴β������(�ѳɹ�ʹ��%d��,�����Ѵ�ʹ������)", i+1);
			GOTOERR;
		}
	}
	*/
	//case8:��֤��Ӧ��1����ʹ��*��Կ���е���Կ����calcdes
#if ANDIROD_ENABLE
	cls_show_msg("�л�����Ӧ��B(JDK sec17��Ӧ��B),��֤��Ӧ�ÿ���ʹ�ù�����Կ��");
#else
	cls_printf("��֤��Ӧ��1����ʹ�ù�����Կ��");
#if defined ME31SLS 
	system("cd /appfs/download/");
	system("./sec14");
#else
	if(( ret = NDK_AppLoad((uchar *)SEC_SUBAPP_NLD1,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(( ret = NDK_AppRun((uchar *)SUBAPP1_NAME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
#endif
	if (cls_show_msg("��ȷ����Ӧ��ִ�н��:�ɹ�[ȷ��],ʧ��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case9: BӦ��2�в�������Կ��*���е���Կ�Ѳ���ʹ����
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 16/*8*/, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*SEC_DES_KEYLEN_8|SEC_DES_ECBMODE*/))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case10:���ݿ�ָ����߿մ�""���л���Ӧ���������Կ��  
	if((ret=NDK_SecSetKeyOwner(NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if ANDIROD_ENABLE
	//"*"�˵���ԿӦ������
	memset(udesout, 0, sizeof(udesout));
	if((ret = NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 8, udesin, 16/*8*/, udesout, SEC_DES_ENCRYPT/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//Android�˵���Կ
	memcpy(udesin, "\x11\x22\x33\x44\x55\x66\x77\x88\x11\x22\x33\x44\x55\x66\x77\x88", 16/*8*/);
	memset(udesout, 0, sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 16/*8*/, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*SEC_DES_KEYLEN_8*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(udesout, "\xB6\xBD\x05\xE8\x4C\xAE\x15\xAD\xB6\xBD\x05\xE8\x4C\xAE\x15\xAD", 16/*8*/))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}	
	//���ݿ��ַ������л���Ӧ���������Կ��
	if((ret=NDK_SecSetKeyOwner(""))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case11:��ʱ��ID3����NDK_SecCalcDes�ͻ�ɹ�
	memset(udesin, 0x20, sizeof(udesin));
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 3, udesin, 16/*8*/, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_16/*SEC_DES_KEYLEN_8*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(udesout, "\x3B\xEE\x4A\x4A\xFE\xD0\x67\xD7\x3B\xEE\x4A\x4A\xFE\xD0\x67\xD7", 16/*8*/))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

#if !ANDIROD_ENABLE  //ANDROID��֧��
	//case12:��ʱ�������ƿ�����,���ڷ�TDK ��(TPK,ID6)�Ͳ��ܽ���des������
	//NDK_SecSetCfg(0x06);//Ĭ�ϰ�ȫ����
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 6, udesin, 16, udesout, SEC_DES_DECRYPT|SEC_DES_KEYLEN_DEFAULT))!=NDK_ERR_SECP_PARAM)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	
#endif
	
	send_result("%s����ͨ��", TESTAPI);
	//���Ժ���:
ERR:
#if ANDIROD_ENABLE   //�ָ�����ڴ�������
	if(g_seckeyowner)
		NDK_SecSetKeyOwner("");
	else
		NDK_SecSetKeyOwner("*");
#else
	NDK_AppDel(SUBAPP1_NAME);
	NDK_SecSetCfg(0x06);//Ĭ�ϰ�ȫ����
	NDK_SecSetKeyOwner(NULL);
#endif
	NDK_SecKeyErase();	
	return;
}


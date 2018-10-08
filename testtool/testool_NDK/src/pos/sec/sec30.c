/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec30.c
* Author 		: zhengry
* version		: 
* DATE			: 20160128
* directory 		: 
* description		: ����NDK_SecGetCfg,NDK_SecSetCfg��ȫ���öԹ�����Կ��ϵ�Ĺ�����
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
#define		TESTAPI		"NDK_SecGetCfg,NDK_SecSetCfg"
#define		TESTAPP		"a@#^+,.[}��_23"
#define 	FILETEST23	"a@#^+,.[}��_23.NLD" //���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ�������� 20170621

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef struct key_kcv {
	uchar *pkey;
	uchar *pkcv;
} KEY_KCV;

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: �ṩ��ȫ�ȼ����������ȡ����,��ԿΨһ��,��Կר����,��Կ����ǿ�ȵ���֤ (�Թ����������ֽ�����֤)
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author			date		remarks
*			  	  	  		zhengry     20160126		created
*****************************************************************/
void sec30(void)
{
	/*private & local definition*/
	int ret = 0;
	uint punCfgInfo;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar udesin[31]={0},udesout[31]={0},udesout1[31]={0};
	KEY_KCV SecLoad[ ]={
		{(uchar *)"\x13", 																					 NULL},   // 0  ���İ�װTLK
		{(uchar *)"\x16\x36\x01\xEE\xC3\x53\x04\x00\xF2\x8E\xF1\xE3\x77\x4D\x31\x59", (uchar *)"\x73\x0E\x17\xFB"},   // 1  ��SEC_KCV_ZERO��ʽ��װTMK(��TLK��ɢ)������Ϊ16�ֽڵ�0X15
		{(uchar *)"\x6F\xD1\xDC\x2C\x2C\x55\xFA\x9F\x8B\x73\x19\xF6\x37\xC4\x77\x00", 						 NULL},   // 2  ��SEC_KCV_NONE��ʽ,��װTAK(��TMK��ɢ)  ����Ϊ16�ֽڵ�0X17
		{(uchar *)"\x19", 																					 NULL},   // 3  ���İ�װTAK
		{(uchar *)"\x1B",															  (uchar *)"\x96\x64\xBE\x8E"},   // 4	���İ�װTMK
	};
	uchar ch = 0x00;
	
	memset(udesin, 0x20, sizeof(udesin));//��ʼ����������

	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	if(cls_show_msg1(30, "����%s...��ȷ��%s����,ESC�˳�", TESTAPI, FILETEST23)==ESC)
		return;

	//����ǰ��
	NDK_SecSetKeyOwner("");
	if(((ret=NDK_SecGetCfg(&punCfgInfo))!=NDK_OK)||(punCfgInfo!=0x06)) //Ĭ�ϰ�ȫ�ȼ�Ϊ0x06
	{
		send_result("line %d:%s����ʧ��(%d,%02x)", __LINE__, TESTAPI, ret, punCfgInfo);
		return;
	}
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	
	if((ret=NDK_SecSetCfg(0x18))!=NDK_OK) //�޸İ�ȫ�ȼ���Ĭ��ֵ�෴ 0x18
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	
	//case1����֤ÿ��Ӧ���ж����İ�ȫ�ȼ����ã���Ӧ���л��������ò�ͬ�İ�ȫ��Կ�ȼ���Ӧ�ò�Ӱ����Ӧ��
#if defined ME31SLS //��ˢ���Ʋ���ʹ��app��غ���,�ʴ���ɲ�������ʹ��ϵͳ��������������Ӧ��
	system("cd /appfs/download/");
	system("./sec23");
#else
	if(( ret = NDK_AppLoad((uchar *)FILETEST23,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(( ret = NDK_AppRun((uchar *)TESTAPP))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	if (cls_show_msg("��ȷ��Ӧ��Bִ�н��:�ɹ�[ȷ��],ʧ��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//��SEC_KCV_NONE��ʽ���İ�װTLK,ID=2
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TLK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memset(stKeyInfo.sDstKeyValue, *SecLoad[0].pkey, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//��SEC_KCV_ZERO��ʽ��װTMK(��TLK��ɢ),ID=2
	stKeyInfo.ucScrKeyIdx=1;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TLK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[1].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_ZERO;
	stKcvInfoIn.nLen=4;
	memcpy(stKcvInfoIn.sCheckBuf, SecLoad[1].pkcv, stKcvInfoIn.nLen);	
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if  defined ME31SLS  //������ˢ��Ӧ�ò����ڴ����ʱ���½�һ��Ŀ¼,ʹ��ԭ�е�Ŀ¼�ᵼ�°�ȫ�ȼ����Ļ�ȥ,����������һ��
	if((ret=NDK_SecSetCfg(0x18))!=NDK_OK) //�޸İ�ȫ�ȼ���Ĭ��ֵ�෴ 0x18
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#endif

	//case2:��ԿΩһ�����Ʊ��رգ�ͬһ����Կֵ�ظ���װ����ͬID�����ͻ���;����Կ�洢�ռ��ϣ�Ӧ�óɹ�
	//case2.1:ʹ��ͬһ��KEY,��SEC_KCV_NONE��ʽ,��װTAK1(��TMK��ɢ)
	stKeyInfo.ucScrKeyIdx=2;
	stKeyInfo.ucScrKeyType=SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;	
	memcpy(stKeyInfo.sDstKeyValue, SecLoad[2].pkey, stKeyInfo.nDstKeyLen);

	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK) /*Ӧ�ɹ�*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2.2:ʹ��ͬһ��KEY,��װTDK1(id=3)(��TMK��ɢ)
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)	/*Ӧ�ɹ�*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2.3:ʹ��ͬһ��KEY,��װTAK2(id=4)(��TMK��ɢ)
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)	/*Ӧ�ɹ�*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2.4:��key����żУ��λ��ͬ,Ӧ��Ϊͬһ��key,��װTDK(id=9),��SEC_KCV_NONE��ʽ���İ�װTAK3(id=10)
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=9;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TDK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, *SecLoad[3].pkey, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	ch = (*(SecLoad[3].pkey)&0xfe)|(*(SecLoad[3].pkey)&0x01?0x00:0x01);
	memset(stKeyInfo.sDstKeyValue, ch/**(SecLoad[22].pkey)-1*/, stKeyInfo.nDstKeyLen);
	stKeyInfo.ucDstKeyIdx=10;
	stKeyInfo.ucDstKeyType=SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	if((ret = NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn)) != NDK_OK )/*Ӧ�ɹ�*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:��Կר�������ƹر�:����ʹ������Կ��PIN��Կ��MAC��Կ�����������ݵļӽ��ܲ���
	//case3.1:����Կ��SM4�ӽ���
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TMK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x23, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TMK, 2, udesin, 16, udesout, SEC_SM4_ENCRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout, "\x9E\xE8\x30\xB9\x23\x4A\x7F\x13\xD0\xE0\x3F\x99\xC6\x1C\xF7\x79", 16))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	memset(udesout1,0,sizeof(udesout1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TMK, 2, udesout, 16, udesout1, SEC_SM4_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout1, "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR ;
	}

	//case3.2:PIN��Կ��SM4�ӽ���
	memset(udesout, 0, sizeof(udesout));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=3;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x23, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 3, udesin, 16, udesout, SEC_SM4_ENCRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout, "\x9E\xE8\x30\xB9\x23\x4A\x7F\x13\xD0\xE0\x3F\x99\xC6\x1C\xF7\x79", 16))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	memset(udesout1,0,sizeof(udesout1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TPK, 3, udesout, 16, udesout1, SEC_SM4_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout1, "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR ;
	}
	
	//case3.3:MAC��Կ��DES�ӽ���
	memset(udesout, 0, sizeof(udesout));
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=4;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TAK|SEC_KEY_SM4;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x23, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TAK, 4, udesin, 16, udesout, SEC_SM4_ENCRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout, "\x9E\xE8\x30\xB9\x23\x4A\x7F\x13\xD0\xE0\x3F\x99\xC6\x1C\xF7\x79", 16))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	memset(udesout1,0,sizeof(udesout1));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TAK, 4, udesout, 16, udesout1, SEC_SM4_DECRYPT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout1, "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20", 16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR ;
	}
		
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	
ERR://���Ժ���:����������Կ
	NDK_SecSetCfg(0x06);
	NDK_AppDel(TESTAPP);
	NDK_SecKeyErase();
	return;
}

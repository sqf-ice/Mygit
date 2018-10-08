/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ȫģ��
* file name	: sec19.c
* Author 		: huangjb
* version		: 
* DATE			: 20140523
* directory 		: 
* description		: ����Ӧ��A��Ӧ��B��װ��Կ
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
#define		TESTAPI		"��Ӧ�ð�װ��Կ"
#if OVERSEAS_ENABLE	//���ں���汾��֧�������ַ��ͺ����ַ��Ĵ��,���޸Ĵ���ļ����ļ���
#define		TESTAPP		"test22"
#define 	FILETEST22	"/appfs/download/test22.NLD"  
#else
#define		TESTAPP		"a@#^+,.[}��_22"
#define 	FILETEST22	"a@#^+,.[}��_22.NLD"  //���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ�������� 20170621
#endif

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
* history 		 		:  author			date				remarks
*			  	  	  		 huangjb    20140523		created
*****************************************************************/
void sec22(void)
{
	/*private & local definition*/
	int ret = 0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar udesin[31]={0},udesout[31]={0};
#if !OVERSEAS_ENABLE
	uint unLen=0;
	char sBuf[128]={0};
#endif
	
	memset(udesin, 0x20, sizeof(udesin));//��ʼ����������

	/*process body*/
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//appfs/download/temp/App_encTest.NLD ��ȷ��֤NLD������
	if(cls_show_msg1(30, "����%s...��ȷ��%s����,ESC�˳�", TESTAPI, FILETEST22)==ESC)
		return;
	
	//����ǰ��:��װӦ��B�Ͳ���������Կ
#if !defined ME31SLS	
	if(( ret = NDK_AppLoad((uchar *)FILETEST22,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}

	//A���Լ�װ��ԿID=1����Կ����װTDK1(��Կ8�ֽ�),0x13
	if((ret=NDK_SecSetKeyOwner(""))!=NDK_OK)//�л��ص�ǰӦ��A
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:A��B��װ��ԿB1��B2
	if((ret=NDK_SecSetKeyOwner(TESTAPP))!=NDK_OK)//Ϊ��Ӧ��B������Կ������������װ����ԿΪӦ��B����Կ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
	
	//A��B��ԿB1��TDK1(��Կ8�ֽ�),ID=1,0x15��
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x15, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//A��B��װ��Կ2��TDK1(��Կ8�ֽ�),ID=2,0x13��
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:������ȷ�����:A��֤��Կ���ܽ��Ӧ�ú�A�Լ�����Կ�Ľ��һ�£�˵��Ӧ��A��B�ж�������Կ�ռ��ID
	if((ret=NDK_SecSetKeyOwner(""))!=NDK_OK)//�л���AӦ��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 8))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	
	//case4:����ΪA�������:A��֤B2��Կʧ�ܣ�˵��Ӧ��A��B�ж�������Կ�ռ��ID,��������ȷ�����A����ʹ��B����Կ
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}

	//case12:�л���Ӧ��B��������Ӧ��AӦ�ÿ���ʹ��Ӧ��B����Կ2���мӽ��ܣ�������ȷ�����A����ʹ��B����Կ
	if((ret=NDK_SecSetKeyOwner(TESTAPP))!=NDK_OK)//�л���BӦ��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 8))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}

	//case5:������Ӧ��B����Ӧ��B��Bʹ���Լ�����ԿB1,B2����DES����Ӧ��Ԥ��һ�£���Ӧ��B����ɾ���Լ�����Կ
	if((ret=NDK_SecSetKeyOwner(TESTAPP))!=NDK_OK)//�л���BӦ��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
#if defined ME31SLS //��ˢ���Ʋ���ʹ��app��غ���,�ʴ���ɲ�������ʹ��ϵͳ��������������Ӧ��
	system("cd /appfs/download/");
	system("./sec22");
#else
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

	//case6:BӦ���в�������Կ��Ӧ�ò�Ӱ��A����Կ
	if((ret=NDK_SecSetKeyOwner(""))!=NDK_OK)//�л���AӦ��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 8))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	
	//AӦ�����¸�BӦ�ð�װ������Կ���ں�������������ȷ�����A����B��ԿӦ�óɹ�
	if((ret=NDK_SecSetKeyOwner(TESTAPP))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x15, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=2;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !OVERSEAS_ENABLE  //����汾û�н��л�ȡTUSN   
	//case11:������ȡTUSN���ܸı�Ӧ��A����Կ����(ԭ�ȵ�bug�ǻ�ȡTUSN����޸�Ӧ�õ�ǰ��Կ�����Ӷ����»��ȡ����Ӧ�õ���Կ)20170727 added by jym
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_TUSN, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s��ȡTUSNʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//��ʱ��������B��,ʹ��B�����ԿӦ�óɹ�
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 2, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 8))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
#endif	
	//case7.1:����Կ��������Bʱ Ӧ��A����B��ָ����ID=2����ԿӦ�óɹ�,ɾ����Bʹ��ID=2����Կ�ӽ���Ӧ��ʧ��
	if((ret=NDK_SecSetKeyOwner(TESTAPP))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
	if((ret=NDK_SecKeyDelete(2,SEC_KEY_TYPE_TDK))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
#if defined ME31SLS
	system("cd /appfs/download/");
	system("./sec22");
#else
	if(( ret = NDK_AppRun((uchar *)TESTAPP))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	if (cls_show_msg("��ȷ���Ƿ���50����Ӧ��B������Կ����ʧ��:��[ȷ��],��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case7.2:A����B��������ԿӦ�óɹ�,ɾ����Bʹ��ID=1����Կ�ӽ���Ӧ��ʧ��
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
#if defined ME31SLS
	system("cd /appfs/download/");
	system("./sec22");
#else
	if(( ret = NDK_AppRun((uchar *)TESTAPP))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	if (cls_show_msg("��ȷ���Ƿ���33����Ӧ��B������Կ����ʧ��:��[ȷ��],��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case10:��֤Ӧ��A����ɾ��B��Կʱ����Ӧ��ɾ���Լ�����Կ������A����Կ��Ӧ���ܹ�ʹ��
	if((ret=NDK_SecSetKeyOwner(""))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 8))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	
	//case8:A����A����֤Aʧ��
	if((ret=NDK_SecSetKeyOwner(""))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
	if((ret=NDK_SecKeyErase())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		GOTOERR;
	}
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_ERR_SECCR_GET_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	
	
	//case9������NDK_SecSetKeyOwner��A���Լ�װ��ԿӦ�ɹ�
	//��װTDK1(��Կ8�ֽ�),0x13
	if((ret=NDK_SecSetKeyOwner(""))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__,TESTAPI,ret);
		return;
	}
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TDK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x13, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(udesout,0,sizeof(udesout));
	if((ret=NDK_SecCalcDes(SEC_KEY_TYPE_TDK, 1, udesin, 8, udesout, SEC_DES_ENCRYPT|SEC_DES_KEYLEN_8/*|SEC_DES_ECBMODE*/))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR ;
	}
	if(memcmp(udesout, "\x38\x77\x48\x93\xF5\x5F\xF4\xC9", 8))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	//���Ժ���:����������Կ�Ͳ���Ӧ��
ERR:
	NDK_AppDel(TESTAPP);
	NDK_SecSetKeyOwner("");
	NDK_SecKeyErase();
	return;
}

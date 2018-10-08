/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App4.c
* Author 			: dait
* version			: 
* DATE			: 20130228
* directory 		: 
* description		: ���� NDK_AppDel�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define		TESTAPI		"NDK_AppDel"
#if K21_ENABLE&&!CPU5810X_ENABLE&&!defined ME50NX&&!defined ME68
#define TESTAppName		"�Լ�5"
#define	FILETEST4		"/appfs/�Լ�5.NLP" 
#elif CPU5810X_ENABLE||defined ME50NX ||defined ME68
#define TESTAppName		"�Լ�1"
#define	FILETEST4		"�Լ�1.NLP" 
#define TESTAppName1 	"k#~a"
#define	FILETEST5		"k#~a.NLP"
#elif OVERSEAS_ENABLE
#define TESTAppName 	"typeAunMaster"
#define FILETEST4		"typeAunMaster.NLD" 
#if OVERSEAS_OS70_ENABLE  //7.0�汾��ʼ��֧��rootȨ��Ӧ��  20180420 modify
#define TESTAppName1 	"typeAunMaster1"
#define FILETEST5		"typeAunMaster1.NLD" 
#else
#define TESTAppName1 	"typeRunMaster"
#define FILETEST5		"typeRunMaster.NLD" 
#endif
#elif defined ME50S|| defined ME50NS
#define TESTAppName 	"App_A_NonMaster"
#define TESTAppName1 	"a~!@#%^_=+,.[{]}��"
#define FILETEST4		"/appfs/apps/download/App_A_NonMaster.NLD" 
#define	FILETEST5		"/appfs/apps/download/a~!@#%^_=+,.[{]}��.NLD"
#else
#define TESTAppName 	"App_A_NonMaster"
#define TESTAppName1 	"a~!@#%^_=+,.[{]}��"
#define FILETEST4		"/appfs/download/App_A_NonMaster.NLD" 
#define	FILETEST5		"/appfs/download/a~!@#%^_=+,.[{]}��.NLD"
#endif
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���2������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*
*****************************************************************/
void app4(void)
{
	/*private & local definition*/
	int ret = 0;
	ST_APPINFO UsrPrgInfo ;
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
	int nFlag = 1, nPos = 0;
#endif	
	
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	memset(&UsrPrgInfo,0,sizeof(UsrPrgInfo));
	if(cls_show_msg1(30, "����%s...��ȷ��%s,%s����,ESC�˳�", TESTAPI,FILETEST4,FILETEST5)==ESC)
		return;
	
	//case1:ɾ������0xA1��������ʾ�ַ�Ӧ��,����װ��,�������������ж��
	if(cls_show_msg("��һ�ν��˰�����ѡ��װ��(װ�غ�����),�ٴν��벻װ��.[Enter]װ��,[����]����")==ENTER)
	{
		//20140603 added by huangjianb
		if(( ret = NDK_AppLoad((uchar *)FILETEST5,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#if !(defined ME50||defined ME50N ||defined ME50C ||defined ME50H||defined ME62||defined ME50NX||defined ME68)
		memset(&UsrPrgInfo, 0, sizeof(ST_APPINFO));
		if((ret = NDK_AppGetInfo((uchar *)TESTAppName1,0,&UsrPrgInfo,sizeof(ST_APPINFO)))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#endif
		//End 20140603
		NDK_SysReboot();
	}
	//������ɾ��Ӧ��Ӧ���ܹ��ɹ�,�����ٻ�ȡӦ����Ϣʧ��
	//20140603 added by huangjianb
	if((ret=NDK_AppDel(TESTAppName1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !(defined ME50||defined ME50N||defined ME50C||defined ME50H||defined ME62||defined ME50NX||defined ME68)
	memset(&UsrPrgInfo, 0, sizeof(ST_APPINFO));
	if((ret = NDK_AppGetInfo((uchar *)TESTAppName1,0,&UsrPrgInfo,sizeof(ST_APPINFO)))!=NDK_ERR_APP_NOT_EXIST)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#else
	while (nFlag) 
	{
		memset(&UsrPrgInfo, 0, sizeof(ST_APPINFO));
		ret = NDK_AppGetInfo(NULL,nPos++,&UsrPrgInfo,sizeof(ST_APPINFO));
		switch(ret)
		{
			case NDK_OK:
				//�Ƚ�����в鵽��Ӧ�þͳ���,û�оͼ���ֱ�����һ��
				if(!strcmp(UsrPrgInfo.szAppName,TESTAppName1))
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto ERR;
				}
				else
					continue;
				break;
			case NDK_ERR_APP_NOT_EXIST://��ǰû��Ӧ����Ϣ �ɹ��˳�
				nFlag = 0;
				break;
			case NDK_ERR:
				//cls_show_msg("��%d���̼����ļ�����Ϊ:file�ļ�����", nPos);
				break;
			default:
				send_result("line %d:%s����ʧ��(%d,npos=%d)", __LINE__, TESTAPI, ret, nPos);  
				goto ERR;
				break;	
		}
	}
#endif
	//End 20140603
	
	//case2:����Ϊ�ղ���
	if((ret=NDK_AppDel(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:��������Ĳ����ڵ�Ӧ������,ɾ��ʧ��
	if((ret=NDK_AppDel("testNoneapp"))!=NDK_ERR_APP_NOT_EXIST)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:ɾ����ȷ��Ӧ��,ɾ���ɹ�,ɾ�����ٻ�ȡ��ϢӦ����ʾӦ�ò�����
	if(( ret = NDK_AppLoad((uchar *)FILETEST4,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_AppDel(TESTAppName))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !(defined ME50||defined ME50N||defined ME50C||defined ME50H||defined ME62||defined ME50NX||defined ME68)
	memset(&UsrPrgInfo, 0, sizeof(ST_APPINFO));
	if((ret = NDK_AppGetInfo((uchar *)TESTAppName,0,&UsrPrgInfo,sizeof(ST_APPINFO)))!=NDK_ERR_APP_NOT_EXIST)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#else
	while (nFlag) 
	{
		memset(&UsrPrgInfo, 0, sizeof(ST_APPINFO));
		ret = NDK_AppGetInfo(NULL,nPos++,&UsrPrgInfo,sizeof(ST_APPINFO));
		switch(ret)
		{
			case NDK_OK:
				//�Ƚ�����в鵽��Ӧ�þͳ���,û�оͼ���ֱ�����һ��
				if(!strcmp(UsrPrgInfo.szAppName,TESTAppName))
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto ERR;
				}
				else
					continue;
				break;
			case NDK_ERR_APP_NOT_EXIST://��ǰû��Ӧ����Ϣ �ɹ��˳�
				nFlag = 0;
				break;
			case NDK_ERR:
				//cls_show_msg("��%d���̼����ļ�����Ϊ:file�ļ�����", nPos);
				break;
			default:
				send_result("line %d:%s����ʧ��(%d,npos=%d)", __LINE__, TESTAPI, ret, nPos);  
				goto ERR;
				break;	
		}
	}
#endif
	// ����ͨ��
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_AppDel(TESTAppName);
	NDK_AppDel(TESTAppName1);
	return;
}

/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App6.c
* Author 			: dait
* version			: 
* DATE			: 20130228
* directory 		: 
* description		: ���� NDK_AppDoEvent�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_AppDoEvent"
#define	APP_NAME	"app_event"
#if defined ME50S|| defined ME50NS
#define	FILETEST	"/appfs/apps/download/app_event.NLD" 
#elif OVERSEAS_ENABLE
#define	FILETEST	"app_event.NLD" //Ϊ�˲��Է������Ѻ����Ʒ��Ϊ���ڵ�ǰ·����  20180420 modify
#else
#define	FILETEST	"/appfs/download/app_event.NLD" 
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
void app6(void)
{
	/*private & local definition*/
	int ret = 0, nLen = 0, nOutLen = 0;
	int nModule = 1;
	char szEventMsg[256]="Hello world";
	char szOut[256]={0};
	char strresult[256] = "�������";
	
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	if(cls_show_msg1(30, "����%s...��ȷ��%s����,ESC�˳�", TESTAPI,FILETEST)==ESC)
		return;
	if(( ret = NDK_AppLoad((uchar *)FILETEST,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case1:Ӧ����ΪNULLӦ�÷��ز�������
	nLen = strlen(szEventMsg);
	if((ret=NDK_AppDoEvent (NULL,nModule,szEventMsg,nLen,szOut,256,&nOutLen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2:�����¼�ΪNULLӦ�÷��ز�������
	if((ret=NDK_AppDoEvent ((uchar *)APP_NAME,nModule,NULL,nLen,szOut,256,&nOutLen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case3:����¼�����NULLӦ�÷��ز�������
	if((ret=NDK_AppDoEvent ((uchar *)APP_NAME,nModule,szEventMsg,nLen,szOut,256,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case4:���ݲ����ڵ�Ӧ��Ӧ����ʧ��
	if((ret=NDK_AppDoEvent ((uchar *)"App_NULL",nModule,szEventMsg,nLen,szOut,256,&nOutLen))!=NDK_ERR_APP_NOT_EXIST)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case5:������ȷ����Ӧ���سɹ�
	if((ret=NDK_AppDoEvent ((uchar *)APP_NAME,nModule,szEventMsg,nLen,szOut,256,&nOutLen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(strcmp(strresult,szOut))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, szOut);
		goto ERR;
	}

	//���Ժ���,ɾ����س���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_AppDel((char *)APP_NAME);
	return;
}

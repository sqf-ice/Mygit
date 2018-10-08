/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App7.c
* Author 			: dait
* version			: 
* DATE			: 20130228
* directory 		: 
* description		: ���� NDK_AppSetEventCallBack�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_AppSetEventCallBack"
#define	APP_NAME	"event_nonecallback"
#define	FILETEST	"/appfs/download/event_nonecallback.NLD" 

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
int Helloworld(int n, void * pIn,int nMaxLen,void **pOut,int *nOutLen)
{
	char Buff[256]="hello world";
	//fprintf(stderr,"%s %d [pIn] = %s [n] = %d [nMaxLen] = %d\n",__func__,__LINE__,pIn,n,nMaxLen);
	cls_printf("%s %d [pIn] = %s [n] = %d [nMaxLen] = %d\n",__func__,__LINE__,pIn,n,nMaxLen);
	memcpy(*pOut,Buff,sizeof(Buff));
	*nOutLen = sizeof(Buff);
	return 0;
}
void app7(void)
{
	/*private & local definition*/
	int ret = 0;
	int nLen = 0, nOutLen = 0;
	int nModule = 0;
	char szEventMsg[256]="Hello world";
	char szOut[256]={0};
	nLen = strlen(szEventMsg);
	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	if(cls_show_msg1(30, "����%s...��ȷ��%s����,ESC�˳�", TESTAPI,FILETEST)==ESC)
		return;
	if(( ret = NDK_AppLoad((uchar *)FILETEST,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_AppSetEventCallBack(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret=NDK_AppSetEventCallBack(Helloworld))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//APP_NAMEying event_nonecallback ��main��������û�е���NDK_AppSetEventCallBack
	if((ret=NDK_AppDoEvent ((uchar *)APP_NAME,nModule,szEventMsg,nLen,szOut,256,&nOutLen))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_AppDel((char *)APP_NAME);
	return;
}

/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App5.c
* Author 			: dait
* version			: 
* DATE			: 20130228
* directory 		: 
* description		: ���� NDK_AppEXECV�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_AppEXECV"  
#if OVERSEAS_ENABLE
#define TESTApp 	"typeAunMaster"
#if defined SP630PG||defined SP830PG
#define TESTAppName "typeAunMaster/main"
#else
#define TESTAppName "typeAunMaster/sp33_20160622_user_test"
#endif
#define	FILETEST1 	"typeAunMaster.NLD"  //Ϊ�˲��Դ������ֱ�Ӹ�Ϊ���ڵ�ǰ·���� 20180423
#elif defined ME50S|| defined ME50NS
#define TESTApp 	"App_U_NonMaster"
#define TESTAppName "App_U_NonMaster/main"
#define	FILETEST1 	"/appfs/apps/download/App_U_NonMaster.NLD"
#else
#define TESTApp 	"App_U_NonMaster"
#define TESTAppName "App_U_NonMaster/main"
#define	FILETEST1 	"/appfs/download/App_U_NonMaster.NLD" 
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
void app5(void)
{
	/*private & local definition*/
	int ret = 0;
	char *exec_argv [4];
	exec_argv[0]="telnet";
	exec_argv[1]="ip";
	exec_argv[2]="port";
	exec_argv[3]=NULL;
	
	char *exec_argv2 [4];
	exec_argv[0]="telnet";
	exec_argv[1]="ip";
	exec_argv[2]="port";
	
	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	if(cls_show_msg1(30, "����%s...��ȷ��%s����,ESC�˳�", TESTAPI,FILETEST1)==ESC)
		return;
	if(( ret = NDK_AppLoad((uchar *)FILETEST1,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
		
	//case1:Ӧ����ΪNULLӦ�÷��ز�������
	if((ret=NDK_AppEXECV (NULL,exec_argv,1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if 1
	//case2:��3������Ϊ�ĵ��涨�����ֵ��Ӧ�÷���ʧ��
	if((ret=NDK_AppEXECV ((uchar *)TESTAppName,exec_argv,2))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case3:Ӧ�����в���ΪNULL��Ӧ�÷��سɹ�
	if((ret=NDK_AppEXECV ((uchar *)TESTAppName,NULL,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:��ȷִ�п�ִ�г���
	if((ret=NDK_AppEXECV ((uchar *)TESTAppName,exec_argv2,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:��ȷִ�п�ִ�г���
	if((ret=NDK_AppEXECV ((uchar *)TESTAppName,exec_argv,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
#if 1
	//case5:���������Ӧ������ Ӧ�÷���NDK_ERR_APP_NOT_EXIST  20140924 jiangym modify
	if((ret=NDK_AppEXECV ((uchar *)"testNoneApp",exec_argv,0))!=NDK_ERR_APP_NOT_EXIST/*NDK_OK*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#endif

	// ����ͨ��
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_AppDel(TESTApp);
	return;
}

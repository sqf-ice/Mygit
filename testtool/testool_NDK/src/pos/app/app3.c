/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name		: App3.c
* Author 			: dait
* version			: 
* DATE			: 20130228
* directory 		: 
* description		: ���� NDK_AppRun�ӿ��ܷ�������Ӧ��Ӧ��
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_AppRun"
#if K21_ENABLE&&!CPU5810X_ENABLE&&!defined ME50NX&&!defined ME68
#define	TESTAppName		"�Լ�"
#define	FILETEST1		"/appfs/�Լ�.NLP" 
//���ڵͶ˲�Ʒ��NLP�����ַ����ܳ���12λ,�ʷֳ�����
#define	TESTAppName2	"a~!@#%"
#define	FILETEST2		"/appfs/a~!@#%.NLP"
#define	TESTAppName3	"^_=+.,"
#define	FILETEST3		"/appfs/^_=+.,.NLP"
#define	TESTAppName4	"[{]}��"
#define	FILETEST4		"/appfs/[{]}��.NLP"
#elif CPU5810X_ENABLE ||defined ME50NX||defined ME68
#define	TESTAppName		"�Լ�"
#define	FILETEST1		"�Լ�.NLP" 
//���ڵͶ˲�Ʒ��NLP�����ַ����ܳ���12λ,�ʷֳ�����
#define	TESTAppName2	"a~!@#%"
#define	FILETEST2		"a~!@#%.NLP"
#define	TESTAppName3	"^_=+.,"
#define	FILETEST3		"^_=+.,.NLP"
#define	TESTAppName4	"[{]}��"
#define	FILETEST4		"[{]}��.NLP"
#elif OVERSEAS_ENABLE
//7.0�汾��ʼ��֧��rootȨ��Ӧ�ð����������������  20180420 modify
#define	TESTAppName		"typeAunMaster"
#define	FILETEST1		"typeAunMaster.NLD"
#if !OVERSEAS_OS70_ENABLE
#define	TESTAppName2	"typeRunMaster" 
#define	FILETEST2		"typeRunMaster.NLD"
#endif
#elif defined ME50S|| defined ME50NS
#define	TESTAppName		"App_U_NonMaster"
#define	TESTAppName2	"a~!@#%^_=+,.[{]}��"
#define	FILETEST2		"/appfs/apps/download/a~!@#%^_=+,.[{]}��.NLD"
#define	FILETEST1		"/appfs/apps/download/App_U_NonMaster.NLD"
#else
#define	TESTAppName		"App_U_NonMaster"
#define	TESTAppName2	"a~!@#%^_=+,.[{]}��"
#define	FILETEST2		"/appfs/download/a~!@#%^_=+,.[{]}��.NLD"
#define	FILETEST1		"/appfs/download/App_U_NonMaster.NLD"
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
void app3(void)
{
	/*private & local definition*/
	int ret = 0;
	float appruntime = 0;
#if K21_ENABLE
	char str[32] = {0};
	int fd = 0;
#endif
#if CPU5810X_ENABLE
	int i = 0;
	char *filename[] = {
							"selftest.NLP",
							"font24_h.NLP",
							"font_h.NLP",
							"font32_h.NLP",
							"statusic.NLP",
							"asc.NLP",
							"gb18030.NLP",
							"wifi-bin.NLP",
							"xui.NLP",
							"factory.NLP",
							"kla.NLP",
#if defined SP610   //ME62 �������debugapp����������20171010 Ŀǰֻ��610�д˹��� 20180529 modify
							"debugapp.NLP",//SP610����
#endif							
						};
	char *appname[] = {
							"selftest",
							"font24_h",
							"font_h",
							"font32_h",
							"statusicon",
							"asc",
							"gb18030",
							"wifi-bin",
							"xui",
							"factory",
							"kla",
#if defined SP610   //Ŀǰֻ��610�д˹��� 20180529 modify
							"debugapppk",
#endif							
						};
#endif
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	if(cls_show_msg1(30, "����%s...��ȷ�������ļ�����,ESC�˳�", TESTAPI)==ESC)
		return;//��Ļ��ʾ��ǰ��������ģ��������

	//case2:���ݲ���Ϊ�յ�
	if((ret=NDK_AppRun(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:���ݲ����ڵ�Ӧ������
#if (defined ME30MH || defined ME30GX ||defined ME32 || defined ME32GX||defined ME68||defined ME50NX||defined ME65)  //ME30MH����ֵ��һ��
	if((ret=NDK_AppRun((const char *)"test_Noneapp"))!=NDK_ERR)
#else
	if((ret=NDK_AppRun((const char *)"test_Noneapp"))!=NDK_ERR_APP_NOT_EXIST)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//��һ��ִ��װ�����run,֮������������װ��ֱ��run
	if(cls_show_msg("��һ�ν��˰�����ѡ��װ��(װ�غ�ѡ������),�ٴν��벻װ�ز�����.[Enter]װ��,[����]����")==ENTER)
	{
#if defined ME68 || defined ME50NX || defined ME32 || defined ME65
		if((fd = NDK_FsOpen(FILETEST1, "w")) <0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fd);
			RETURN;
		}
		if((ret = NDK_FsWrite(fd, "Scorpiop", 8)) != 8)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret = NDK_FsClose(fd)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
#endif
		if(( ret = NDK_AppLoad((const char *)FILETEST1,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#if !OVERSEAS_OS70_ENABLE
		//20140603 added by huangjianb
#if defined ME68 || defined ME50NX || defined ME32 || defined ME65
		if((fd = NDK_FsOpen(FILETEST2, "w")) <0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fd);
			RETURN;
		}
		if((ret = NDK_FsWrite(fd, "Scorpiop", 8)) != 8)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret = NDK_FsClose(fd)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
#endif
		if(( ret = NDK_AppLoad((const char *)FILETEST2,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#endif
#if K21_ENABLE
		//End 2014/6/3
		//jiangym add 20140708  Ҫ֧�ֲ�ͬ���ַ�,��ֳ�С��12λ�ַ���NLP�ֱ���֤
#if defined ME68 || defined ME50NX || defined ME32 || defined ME65
		if((fd = NDK_FsOpen(FILETEST3, "w")) <0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fd);
			RETURN;
		}
		if((ret = NDK_FsWrite(fd, "Scorpiop", 8)) != 8)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret = NDK_FsClose(fd)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
#endif
		if(( ret = NDK_AppLoad((const char *)FILETEST3,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#if defined ME68 || defined ME50NX || defined ME32 || defined ME65
		if((fd = NDK_FsOpen(FILETEST4, "w")) <0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fd);
			RETURN;
		}
		if((ret = NDK_FsWrite(fd, "Scorpiop", 8)) != 8)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret = NDK_FsClose(fd)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
#endif
		if(( ret = NDK_AppLoad((const char *)FILETEST4,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
#endif
	}
	
	//case1:Ӧ�����ƴ��ڵ�����µ���
	lib_StartStopwatch();
	if(( ret = NDK_AppRun((const char *)TESTAppName))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	appruntime = lib_ReadStopwatch();
	//20140603 added by huangjianb
	//case2:�����ַ�Ӧ������װ��Ӧ�ɹ�
#if !OVERSEAS_OS70_ENABLE
	if(( ret = NDK_AppRun((const char *)TESTAppName2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
#if K21_ENABLE
	//End 20140603
	//jiangym add 20140708
	if(( ret = NDK_AppRun((const char *)TESTAppName3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if(( ret = NDK_AppRun((const char *)TESTAppName4))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
#if 0//(defined IM81||defined N900||ANDIROD_ENABLE)//����Ļ ͨ�����ڴ�ӡ��Ϣ���ж� ͬʱ�ܹ�������������
	if(cls_show_msg("�Ƿ��ڿ���̨��������ӡ������:4 5 6 7,��:��[Enter]����,��:��[����]")!=ENTER)
	{//�����ĸ���Ӧ���Ƿֱ�������ӡ������4,5,6,7���ڴ˽����ж��Ƿ�����������Ӧ��
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif

#if !(defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B)
	if(cls_show_msg("�Ƿ�����(������������),[Enter]����,[����]����")==ENTER)
	{
#if (defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME31||defined SP10||defined ME30GX||defined ME32||defined ME32GX||defined ME32THM)//K21_ENABLE	Ŀǰֻ��30 31 SP10��Ҫ����usb����
		cls_show_msg("ȷ����������usb"); //Ӳ�������Ҫ����usb��������
#endif
		NDK_SysReboot();
	}
#endif

#if (defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B)//����������Ϳ���̨��ͻ���޷������������Ϣ,�ɲ�ȡ�ֶ���������
	if(cls_show_msg("�Ƿ�������������,���ڿ���̨��������ӡ������:4 5 6 7,��:��[Enter]����,��:��[����]")!=ENTER)
	{//�����ĸ���Ӧ���Ƿ������ֱ�������ӡ������4,5,6,7���ڴ˽����ж��Ƿ�����������Ӧ��
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if(cls_show_msg("�Ƿ��״ν���˰���,��[Enter]����,[����]����")==ENTER)
	{
		cls_show_msg("������ѡ��װ���ٲ���һ��,���������");
		return;
	}
#endif

#if CPU5810X_ENABLE
	//�������������˲���: װ��ʹ�ð����������nlp������Ӧ��ʧ��
	for(i = 0;i<SZ_ARRAY(appname);i++)
	{
		if(( ret = NDK_AppLoad((const char *)filename[i],0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,��%d��װ��ʧ��)", __LINE__, TESTAPI, ret, i+1);
			GOTOERR;
		}
		if(( ret = NDK_AppRun((const char *)appname[i]))!=NDK_ERR)
		{
			send_result("line %d:%s����ʧ��(%d,��%d������Ӧʧ��)", __LINE__, TESTAPI, ret, i+1);
			GOTOERR;
		}
	}
	cls_show_msg("ע��:���������������Ҫ�������ع̼�...\nSP610,ME62�����������ֿ⡢�Լ졢������statusicon.NLP");
#endif

#if !K21_ENABLE	
	if((ret=NDK_AppDel((const char *)TESTAppName)!=NDK_OK))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !OVERSEAS_OS70_ENABLE
	//20140603 added by huangjianb
	if((ret=NDK_AppDel((const char *)TESTAppName2)!=NDK_OK))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
#if  0//K21_ENABLE
	//End 20140603
	//jiangym add 20140708
	if((ret=NDK_AppDel((const char *)TESTAppName3)!=NDK_OK))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_AppDel((const char *)TESTAppName4)!=NDK_OK))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
#endif	
	
	// ����ͨ��
#if !K21_ENABLE
	send_result("%s����ͨ��(����ʱ��:%f)", TESTAPI, appruntime);
#else
	memset(str,0,sizeof(str));
	ftos(appruntime,str);
	send_result("%s����ͨ��(����ʱ��:%s)", TESTAPI, str);
#endif
goto ERR;
ERR:
#if !K21_ENABLE	
	NDK_AppDel(TESTAppName);
#if !OVERSEAS_OS70_ENABLE
	NDK_AppDel(TESTAppName2);
#endif
#if 0//K21_ENABLE
	NDK_AppDel(TESTAppName3);
	NDK_AppDel(TESTAppName4);
#endif
#endif
	return;
}

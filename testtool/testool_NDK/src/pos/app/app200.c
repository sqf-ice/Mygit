/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module				: appģ�� 
* file name				: 
* Author 				: 
* version				: 
* DATE					: 
* directory 				: 
* description			:appģ���������ϲ���
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI	"appģ����������"
#if K21_ENABLE&&!CPU5810X_ENABLE&&!defined ME50NX&&!defined ME68
#define  FILETEST1 "/appfs/test1.NLP"   //��ĸ����
#define  TestAppName  "test1"
#elif CPU5810X_ENABLE||defined ME50NX||defined ME68
#define  FILETEST1 "test1.NLP"   //��ĸ����
#define  TestAppName  "test1"
#elif OVERSEAS_ENABLE
#define  FILETEST1 "/appfs/download/typeAunMaster.NLD"  //������������ͨӦ��
#define  TestAppName  "typeAunMaster"
#define TESTAppName1  "typeAunMaster/sp33_20160622_user_test"
#define	FILETEST	"/appfs/download/app_event.NLD" 
#define	APP_NAME	"app_event"
#else
#define  FILETEST1 "/appfs/download/App_U_NonMaster.NLD"  //���ļ�����/appfs/download/tempĿ¼��
#define  TestAppName  "App_U_NonMaster"
#define	APP_NAME	"app_event"
#if defined ME50S||defined ME50NS
	#define TESTAppName1  "App_U_NonMaster/main"
	#define	FILETEST	"/appfs/apps/download/app_event.NLD" 
#else
	#define TESTAppName1  "App_U_NonMaster/main"
	#define	FILETEST	"/appfs/download/app_event.NLD" 
#endif
#endif
#if 0 //��Ϊӡ�Ȱ汾����
#if OVERSEAS_ENABLE && !(defined SP930G||defined SP630G||defined SP930PG||defined SP630PG)
	#define   SUBAPP1  "subapp1"
#endif
#endif

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef enum {
	AppLoad,
#if !K21_ENABLE
	AppGetReboot,
#endif
	AppRun,
#if K21_ENABLE
	AppDataSave,
	AppDataVerify,
	AppUpdate,
#endif
#if !K21_ENABLE|| CPU5810X_ENABLE
	AppGetInfo,
	AppDel,
#endif
#if !K21_ENABLE
	AppEXECV,
	AppDoEvent,
	AppGetPathById,
	AppGetPathByAppName,
#elif defined SP610
	NlpVerify,
#endif
#if 0 //ֻ��ӡ�Ȱ汾��֧���⼸������������ӡ�Ȱ汾�����п���
#if OVERSEAS_ENABLE && !(defined SP930G||defined SP630G||defined SP930PG||defined SP630PG)
	AppStartChildProcess,
	AppGetDatafromParentProcess,
	AppChildProcessExit,
#endif
#endif
	MaxNum,//����ö��ֵʱ�ڴ�ֵǰ���,Ҫ��֤��ֵΪ���һ��ֵ
}EM_APP;

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: 
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  sull             20180213         created
*****************************************************************/
#if K21_ENABLE
int app_transdata1(const char *pszName, int offseterrflag, int buferrflag)
{
	int  ret = 0, offset=0, fp=0, tmp = 0;
	uint len = 0;
	uchar buf[1024]={0};
	if((fp=NDK_FsOpen(pszName, "r"))<0)
	{
		cls_show_msg1(1, "line %d:%s�ļ���ʧ��(%d)", __LINE__, pszName, fp);
		return NDK_ERR;
	}
	while(1)
	{
		if((len=NDK_FsRead(fp, (char *)buf, 1024))<0)
		{
			cls_show_msg1(1, "line %d:%s�ļ���ȡ����ʧ��(%d)", __LINE__, pszName, len);
			NDK_FsClose(fp);
			return NDK_ERR;
		}
		if(buferrflag==1)
		{
			tmp = rand()%(len-272)+272;
			if(buf[tmp]==0)
				buf[tmp]=0xff;
			else
			buf[tmp]=0;//�����޸ĵ�һ�ζ��������е�����һ���ֽ�Ϊ0,�����������ݴ���
			buferrflag = 0;
		}
#if defined ME68||defined ME50NX  //����ƽ̨����app:offset = 256*1024+offset�����¹̼�:offset = 0+ offset
		if((ret=NDK_AppDataSave(256*1024+offset,buf,len))!=NDK_OK)
#else
		if((ret=NDK_AppDataSave(offset, buf, len))!=NDK_OK)
#endif
		{
			cls_show_msg1(1, "line %d:%s�ļ�App���ݴ���ʧ��(%d)", __LINE__, pszName, ret);
			NDK_FsClose(fp);
			return NDK_ERR;
		}
		if(offseterrflag!=0)
		{
			offset+=offseterrflag;//���׵�ַ�޸ĳ�offseterrflag,ʹ��Ե�ַ����ȷ�������ݱ������
			offseterrflag=0;
		}
		offset+=len;
		
		if(len<1024)
			break;
	}
	NDK_FsClose(fp);
	return NDK_OK;
}
#endif

static int app_randtest(int funnum)
{
#if !K21_ENABLE||CPU5810X_ENABLE
	ST_APPINFO UsrPrgInfo ;
#endif
#if defined SP610
	char nlpbuf[950] = {0}; //50*19
	int errnum = 0;
#endif
#if !K21_ENABLE
	char szOut[256] = {0}, emPathId1[64]={0}, szEventMsg[256]="Hello world";
	int nModule = 1, nOutLen = 0, nLen = 0, nRebootFlag = -1;
#if OVERSEAS_ENABLE
	char pszAppName1[64]={"typeAunMaster"}, pszAppPath[64]={"0"};
#if 0//!(defined SP930G||defined SP630G||defined SP930PG||defined SP630PG)
	AppExitStatusT	*appstatus;
	int len1 = 0, data = 0;
#endif
#else
	char pszAppName1[64]={"App_A_NonMaster"}, pszAppPath[64]={"0"};
#endif
#endif
	switch(funnum)
	{ 
		case AppLoad:
			NDK_AppLoad((char *)FILETEST1,0);//uchar�ĳ�char ��Ϊ���������� 
			break;
#if !K21_ENABLE
		case AppGetReboot:
			NDK_AppGetReboot(&nRebootFlag);
			break;
#endif
		case AppRun:
			NDK_AppRun((char *)TestAppName);
			break;
#if K21_ENABLE
		case AppDataSave:
			app_transdata1(FILETEST1,0,0);
			break;
		case AppDataVerify:
			NDK_AppDataVerify();
			break;
		case AppUpdate:
			NDK_AppUpdate(0);
			break;
#endif
#if !K21_ENABLE||CPU5810X_ENABLE
		case AppGetInfo:
			memset(&UsrPrgInfo, 0, sizeof(ST_APPINFO));
			NDK_AppGetInfo(NULL,1,&UsrPrgInfo,sizeof(ST_APPINFO));
			break;
		case AppDel:
			NDK_AppDel(TestAppName);
			break;		
#endif
#if !K21_ENABLE
		case AppEXECV:
			NDK_AppEXECV ((uchar *)TESTAppName1,NULL,1);
		case AppDoEvent:
			memset(szOut, 0, sizeof(szOut));
			NDK_AppDoEvent ((uchar *)APP_NAME,nModule,szEventMsg,nLen,szOut,256,&nOutLen);
			cls_printf("��Ӧ��B�������...");
			break;
		case AppGetPathById:
			memset(emPathId1, 0, sizeof(emPathId1));
			NDK_AppGetPathById(rand()%3,emPathId1);
			break;
		case AppGetPathByAppName:
			memset(pszAppPath, 0, sizeof(pszAppPath));
			NDK_AppGetPathByAppName(pszAppName1,pszAppPath);
			break;
#elif defined SP610
		case NlpVerify:
			memset(nlpbuf, 0, sizeof(nlpbuf));
			NDK_NlpVerify(nlpbuf,&errnum);
			break;
#endif
#if 0 //ֻ��ӡ�Ȱ汾��֧���⼸������������ӡ�Ȱ汾�����п���
#if OVERSEAS_ENABLE && !(defined SP930G||defined SP630G||defined SP930PG||defined SP630PG)
		case AppStartChildProcess:
			NDK_AppStartChildProcess(SUBAPP1,NULL,0,&appstatus);    
			break;
		case AppGetDatafromParentProcess:
			NDK_AppGetDatafromParentProcess(&data,NULL);
			break;
		case AppChildProcessExit:
			NDK_AppChildProcessExit(0,NULL,len1);
			break;
#endif
#endif
	}		
	return SUCC;
}

void app200(void)
{
	/*private & local definition*/
	int  i = 0, bak = 0, num[30] ={0}, j = 0; 
	int ret = -1,cnt =50, len = 0;//cnt ����
#if  defined SP610
	char *appstr[] = {"NDK_AppLoad", "NDK_AppRun", "NDK_AppDel", "NDK_AppDataSave", "NDK_AppDataVerify", "NDK_AppUpdate", "NDK_NlpVerify"};
#elif K21_ENABLE && !CPU5810X_ENABLE
	char *appstr[] = {"NDK_AppLoad", "NDK_AppRun", "NDK_AppDataSave", "NDK_AppDataVerify", "NDK_AppUpdate"};
#elif CPU5810X_ENABLE &&!defined SP610
	char *appstr[] = {"NDK_AppLoad", "NDK_AppRun", "NDK_AppDataSave", "NDK_AppDataVerify", "NDK_AppUpdate", "NDK_AppGetInfo", "NDK_AppDel"};
#else
	char *appstr[] = {"NDK_AppLoad", "NDK_AppGetReboot", "NDK_AppRun", "NDK_AppGetInfo", "NDK_AppDel", "NDK_AppEXECV", "NDK_AppDoEvent", "NDK_AppGetPathById", "NDK_AppGetPathByAppName"};
#endif

	/*process body*/
#if !K21_ENABLE||defined SP610
	if(cls_show_msg1(30, "����%s...��ȷ��%s,%s,%s�����ļ�����,ESC�˳�", TESTAPI,FILETEST1)==ESC)
#elif !K21_ENABLE
	if(cls_show_msg1(30, "����%s...��ȷ��%s,%s,%s�����ļ�����,ESC�˳�", TESTAPI,FILETEST1,FILETEST)==ESC)
#else
	if(cls_show_msg1(30, "����%s...��ȷ��%s�����ļ�����,ESC�˳�", TESTAPI,FILETEST1)==ESC)
#endif
		return;//��Ļ��ʾ��ǰ��������ģ��������

	len = (MaxNum>20)?MaxNum:20;
	while(cnt)
	{	
		cnt--;
		bak++;
		
		//���������
		cls_printf("��%d�����˳��:",bak);
		srand((unsigned)time(NULL));
		for(i=0;i<len;i++)
		{	
			num[i] = rand()%MaxNum;
			NDK_ScrPrintf("%d ", num[i]);
			NDK_ScrRefresh();
		}
		
		//�����������
		for(i=0;i<len;i++)
		{
			if((ret = app_randtest(num[i])) != NDK_OK)
			{
				send_result("line %d:%s��%d�β���ʧ��(%d,%d)", __LINE__, TESTAPI, bak,ret,num[i]);
				for(j=0;j<=i;j++)
					send_result("%s ",appstr[num[j]]);
				goto ERR;
			}
		}	
	}

	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_FsDel(FILETEST1);
#if !K21_ENABLE|| CPU5810X_ENABLE
	NDK_AppDel(TestAppName);
#endif
#if !K21_ENABLE
	NDK_AppDel(TESTAppName1);
#endif
	return;
}


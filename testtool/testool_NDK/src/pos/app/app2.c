/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App2.c
* Author 			: dait
* version			: 
* DATE			: 20130228
* directory 		: 
* description		: ���� NDK_AppGetInfo�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define		TESTAPI		"NDK_AppGetInfo"
#if K21_ENABLE&&!CPU5810X_ENABLE&&!defined ME50NX&&!defined ME68
#define	TESTAPP		"test1"
#define	TESTAPP2	"�Լ�1"
#define	TESTAPP3	"k#~a"
#define	TESTNONAPP	"App_GetInfo"
#define	FILETEST	"/appfs/test1.NLP"
#define	FILETEST2	"/appfs/�Լ�1.NLP"
#define	FILETEST3	"/appfs/k#~a.NLP"
#define	MAXPOS     11 
#elif CPU5810X_ENABLE ||defined ME50NX||defined ME68
#define	TESTAPP		"test1"
#define	TESTAPP2	"�Լ�1"
#define	TESTAPP3	"k#~a"
#define	TESTNONAPP	"App_GetInfo"
#define	FILETEST	"test1.NLP"
#define	FILETEST2	"�Լ�1.NLP"
#define	FILETEST3	"k#~a.NLP"
#define	MAXPOS     49 //ME50������50��0~49
#elif OVERSEAS_ENABLE
//�����Ʒ7.0�汾��ʼ��֧��rootȨ��,������rootȨ�޲��Եĸ�Ϊ����ͨ�û�Ȩ�޲���  20180420 modify
#define	TESTAPP	    "typeAunMaster"
#define	TESTNONAPP	"App_GetInfo"
#define	FILETEST 	"typeAunMaster.NLD" 
#define	MAXPOS     64   //???
#if !OVERSEAS_OS70_ENABLE
#define	TESTAPP2	"typeRunMaster"
#define	FILETEST2	"typeRunMaster.NLD" 
#endif
#elif defined ME50S|| defined ME50NS
#define	TESTAPP		"App_U_NonMaster"
#define	TESTAPP2	"App_A_NonMaster"
#define	TESTAPP3	"a~!@#%^_=+,.[{]}��"
#define	TESTNONAPP	"App_GetInfo"
#define	FILETEST	"/appfs/apps/download/App_U_NonMaster.NLD" 
#define	FILETEST2	"/appfs/apps/download/App_A_NonMaster.NLD" 
#define	FILETEST3	"/appfs/apps/download/a~!@#%^_=+,.[{]}��.NLD"
#define	MAXPOS     64 
#else
#define	TESTAPP		"App_U_NonMaster"
#define	TESTAPP2	"App_A_NonMaster"
#define	TESTAPP3	"a~!@#%^_=+,.[{]}��"
#define	TESTNONAPP	"App_GetInfo"
#define	FILETEST	"/appfs/download/App_U_NonMaster.NLD" 
#define	FILETEST2	"/appfs/download/App_A_NonMaster.NLD" 
#define	FILETEST3	"/appfs/download/a~!@#%^_=+,.[{]}��.NLD"
#define	MAXPOS     64 
#endif

//�����µķ��������У����������뾭��ǩ�����ܳɹ����أ�ǩ���ֲ��ܰ���appinfo��appver�ֶΣ��ʴ˴����⣬��app1��app11����Ӧ��ʱ��
#if 0//defined SP610//����xml�������Ӧ��
#define	TESTAPP4	"n��!%1" 
#define FILETEST4   "n��!%1.NLP" //���ܳ���12���ֽ�
#define APPINFO		"new_�´�½!@#$%^&*(0"
#define APPVER      "20170412"
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
void app2(void)
{
	/*private & local definition*/
	ST_APPINFO UsrPrgInfo ;
	int ret = 0,  nPos = 0;
	int nFlag = 1;

	memset(&UsrPrgInfo,0,sizeof(UsrPrgInfo));
	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	//appfs/download/temp/App_A_Master.NLD 
	//appfs/download/temp/App_A_NonMaster.NLD ��ȷ��֤��������NLD������
	if(cls_show_msg1(30, "����%s...��ȷ�������ļ�����,ESC�˳�", TESTAPI)==ESC)
		return;

	if(( ret = NDK_AppLoad((uchar *)FILETEST,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !OVERSEAS_OS70_ENABLE//����汾��֧��rootȨ�޲���  20180420 modify
	if(( ret = NDK_AppLoad((uchar *)FILETEST2,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
#if !OVERSEAS_ENABLE//����汾UPT���߲�֧�������ַ����,��˲�����FILETEST3
	//20140603 added by huangjianb
	if(( ret = NDK_AppLoad((uchar *)FILETEST3,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//End 20140603
#endif
#if 0//defined SP610//����xml(����appinfo��Ϣ��appver��Ϣ)����µ�Ӧ�ý��в���
	if(( ret = NDK_AppLoad((uchar *)FILETEST4,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	//case1:nPos����Ϊ-1,Ӧ�÷��ز�������
	if((ret = NDK_AppGetInfo(NULL,-1,&UsrPrgInfo,sizeof(ST_APPINFO)))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:Ӧ����Ϣ���ջ���������ΪNULL,Ӧ�÷��ز�������
	if((ret = NDK_AppGetInfo(NULL,0,NULL,sizeof(ST_APPINFO)))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:�������Ӧ����Ŀ64,Ӧ�÷��ز������󣬸�����Ʒ�����в���
	if((ret = NDK_AppGetInfo(NULL,MAXPOS+1,&UsrPrgInfo,sizeof(ST_APPINFO)))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !K21_ENABLE || defined SP610
	//case4:��ȡ�����ڵ�Ӧ����Ϣ��Ӧ���ǻ�ȡ������
	 if((ret = NDK_AppGetInfo((uchar *)TESTNONAPP,0,&UsrPrgInfo,sizeof(ST_APPINFO)))!=NDK_ERR_APP_NOT_EXIST)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif	

	//case5:��������:������POS�˵�Ӧ����Ϣ��ȡ��
	while (nFlag) 
	{
		memset(&UsrPrgInfo, 0, sizeof(ST_APPINFO));
		ret = NDK_AppGetInfo(NULL,nPos++,&UsrPrgInfo,sizeof(ST_APPINFO));
		switch(ret)
		{
			case NDK_OK://ȡ��Ӧ����Ϣ ��ɷ���
#if !K21_ENABLE
					if(auto_flag==0)
					{
						cls_show_msg("�û���������:%s\n�汾��Ϣ:%s\n����ʱ��:%s\n���ر�־:%d", UsrPrgInfo.szAppName, UsrPrgInfo.szVerBuf, UsrPrgInfo.szBuildTime, UsrPrgInfo.nIsMaster);
						cls_show_msg("���԰汾:%d\nӦ�ñ��:%d", UsrPrgInfo.sunReverse[0], UsrPrgInfo.nSeriNo);
					}
					else
					{
						send_result("�û���������:%s �汾��Ϣ:%s ����ʱ��:%s ���ر�־:%d ���԰汾��־:%d Ӧ�ñ��:%d", UsrPrgInfo.szAppName, UsrPrgInfo.szVerBuf, UsrPrgInfo.szBuildTime, UsrPrgInfo.nIsMaster, UsrPrgInfo.sunReverse[0], UsrPrgInfo.nSeriNo);
					}
#if !OVERSEAS_ENABLE
					if(!strcmp((char *)UsrPrgInfo.szAppName,TESTAPP))
					{
						if(UsrPrgInfo.nSeriNo != 1)
						{
							send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, UsrPrgInfo.nSeriNo);
							GOTOERR;
						}
					}
					if(!strcmp((char *)UsrPrgInfo.szAppName,TESTAPP3))
					{
						if(UsrPrgInfo.nSeriNo != 2)
						{
							send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, UsrPrgInfo.nSeriNo);
							GOTOERR;
						}
					}
#else
					if(!strcmp((char *)UsrPrgInfo.szAppName,TESTAPP))
					{
						if(UsrPrgInfo.nSeriNo != 0)//����汾UPT�����޷�����nSeriNoֵ,Ĭ��Ϊ0
						{
							send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, UsrPrgInfo.nSeriNo);
							GOTOERR;
						}
					}
#endif
#else			//K21��Ʒֻ��szAppName��Ч,�����������ж�
				if(auto_flag==0)
					cls_show_msg("��%d���̼����ļ�����Ϊ:app�ļ�����,Ӧ����Ϊ:%s", nPos, UsrPrgInfo.szAppName);
				else
					send_result("��%d���̼����ļ�����Ϊ:app�ļ�����,Ӧ����Ϊ:%s", nPos, UsrPrgInfo.szAppName);
#if 0//defined SP610 //SP610��Ʒ�϶��´���Ĳ����ļ�TESTAPP4�����ֺͰ汾��Ϣ���бȽ�
				if(!strcmp((char *)UsrPrgInfo.szAppName, APPINFO))
				{
					if(strcmp((char *)UsrPrgInfo.szVerBuf, APPVER))
					{
						send_result("line %d:%s����ʧ��(Name:%s ,VerBuf:%s)", __LINE__, TESTAPI, UsrPrgInfo.szAppName, UsrPrgInfo.szVerBuf);
						GOTOERR;
					}
				}					
#endif			
				break;
#endif
				continue;
				break;
			case NDK_ERR_APP_NOT_EXIST://��ǰû��Ӧ����Ϣ �ɹ��˳�
#if defined SP610 || defined ME50H || defined ME50C	//���Ȼ�ȡ���̼��б���Ϣ,����Ӧ����Ϣ��ȡ����,�ʼ��ϴ�Ϊ���ܹ�������ȡӦ����Ϣ 20180714	
				send_result("��%d���̼����ļ�����Ϊ:file�ļ�����,���������в鿴�̼��б��жϻ�ȡ��Ϣ����ȷ��", nPos);
				continue;
#else				
 				nFlag = 0;
				break;
#endif				
#if K21_ENABLE 
			case NDK_ERR://�Ͷ�ƽ̨��ȡ�����ļ�����Ϊfileʱֱ�ӷ���NDK_ERR
#if !defined SP610
				if(auto_flag==0)
					cls_show_msg("��%d���̼����ļ�����Ϊ:file�ļ�����,���������в鿴�̼��б��жϻ�ȡ��Ϣ����ȷ��", nPos);
				else
					send_result("��%d���̼����ļ�����Ϊ:file�ļ�����,���������в鿴�̼��б��жϻ�ȡ��Ϣ����ȷ��", nPos);
#else
				send_result("���ҽ�������ȷ�ϲ��ҵ����ļ����Ƿ��ʵ��һ��");
				nFlag = 0;//SP610������APP��FILE�����������󷵻�NDK_ERR���ҽ���
#endif				
				break;			
#endif
			default:
				send_result("line %d:%s����ʧ��(%d,npos=%d)", __LINE__, TESTAPI, ret, nPos);  
				goto ERR;
				break;
		}
	}
	
#if !K21_ENABLE || defined SP610//��֧��Ӧ����ȡapp��Ϣ�Ĺ��ܲ�����
	//case6:����ֱ��ͨ��Ӧ������ȡӦ����ϢӦ�óɹ�
	memset(&UsrPrgInfo, 0, sizeof(ST_APPINFO));
	 if((ret = NDK_AppGetInfo((uchar *)TESTAPP,0,&UsrPrgInfo,sizeof(ST_APPINFO)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	 
#if !OVERSEAS_ENABLE
	//20140603 added by huangjianb
	//case7:�����ַ���ɵ�Ӧ�����ƻ�ȡӦ����ϢӦ�ɹ�
	memset(&UsrPrgInfo, 0, sizeof(ST_APPINFO));
	 if((ret = NDK_AppGetInfo((uchar *)TESTAPP3,0,&UsrPrgInfo,sizeof(ST_APPINFO)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	//End 20140603
#endif
#endif
#if 0//defined SP610
	memset(&UsrPrgInfo, 0, sizeof(ST_APPINFO));
	 if((ret = NDK_AppGetInfo((uchar *)TESTAPP4,0,&UsrPrgInfo,sizeof(ST_APPINFO)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#endif
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_AppDel(TESTAPP);
#if !OVERSEAS_OS70_ENABLE
	NDK_AppDel(TESTAPP2);
#endif
#if !OVERSEAS_ENABLE
	NDK_AppDel(TESTAPP3);
#endif
#if 0//defined SP610
	NDK_AppDel(TESTAPP4);
#endif
	return;
}


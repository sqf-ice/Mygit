/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name			: 
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: fsģ�������������
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define DESTFILE "/appfs/test"
#define TESTDIR     "./test1"
#if !K21_ENABLE //K21ƽֻ̨֧�־���·��
#define  RANDFILE	"/tmp/randfile"	//����·�� 
#elif OVERSEAS_ENABLE||defined SP610//SP610�ļ�������Ҫ��·��,�ײ���Զ�����/appfs·����
#define  RANDFILE	"randfile" 
#else
#define  RANDFILE	"/appfs/randfile"	//����·�� 
#endif

#define	TESTAPI	"fsģ����������"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef enum {
	FsOpen=0,
	FsClose,
	FsSeek,
	FsWrite,
	FsDel,
	FsRead,
	FsTell,
	FsRename,
	FsTruncate,
	FsExist,
	FsFileSize,
#if K21_ENABLE
	//FsFormat,�Ͷ�ƽ̨���ô˺���������������
#if ANDIROD_ENABLE
	CopyFileToSecMod,
#else
	FsGetDiskSpace,
	FsDir,
	FsClearAppFile,
#endif
#else
	FsGetDiskSpace,
	FsDir,
	FsCreateDirectory,
	FsRemoveDirectory,
#endif
	MaxNum,
}EM_FS;

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: 
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						sull					20180208		created
*****************************************************************/
static int fs_randtest(int funnum)
{
	int ret = -1, fp = -1;
	char writebuf[200] = {0}, readbuf[200] = {0};
	ulong pulRet = 0,spaceofdisk1=0;
	char s[3000]={0};
	uint dirnum=0, punSize = 0; 
	switch(funnum)
	{	
		case FsOpen:
			fp = NDK_FsOpen(RANDFILE, "w");
			NDK_FsClose(fp); //���ӹرվ��,Ϊ��ֹ�˳����������ģ�����ڴ򿪾����̫�ർ��û�о����������20180801 modify
			break;
		case FsClose:
			NDK_FsClose(fp);
			break;
		case FsSeek:
			NDK_FsSeek(fp, 0l, SEEK_SET); 
			break;
		case FsWrite:
			NDK_FsWrite(fp, writebuf, 200); 
			break;
		case FsDel:
			NDK_FsDel(RANDFILE);
			break;
		case FsRead:
			NDK_FsRead(fp, readbuf, 200); 
			break;
		case FsTell:
			NDK_FsTell(fp, &pulRet); 
			break;
		case FsRename:
			NDK_FsRename(RANDFILE,RANDFILE); 
			break;
		case FsTruncate:
			NDK_FsTruncate(RANDFILE, 200); 
			break;
		case FsExist:
			NDK_FsExist(RANDFILE);
			break;
		case FsFileSize:
			NDK_FsFileSize(RANDFILE,&punSize);
			break;
#if K21_ENABLE
#if ANDIROD_ENABLE
		case CopyFileToSecMod:
			NDK_CopyFileToSecMod(RANDFILE, DESTFILE);
		break;
#else
		case FsGetDiskSpace:
			if((ret = NDK_FsGetDiskSpace(rand()%2, &spaceofdisk1)) != NDK_OK)
				goto ERR;
			break;
		case FsDir:
			memset(s, 0, sizeof(s));
			if(( ret = NDK_FsDir("./", s, &dirnum)) != NDK_OK)
				goto ERR;
			break;
		case FsClearAppFile:
			if((ret = NDK_FsClearAppFile()) != NDK_OK)
				goto ERR;
			break;
#endif 
#else
			case FsGetDiskSpace:
				if((ret = NDK_FsGetDiskSpace(rand()%2, &spaceofdisk1)) != NDK_OK)
					goto ERR;
				break;
			case FsDir:
				memset(s, 0, sizeof(s));
				NDK_FsDir("/appfs", s, &dirnum);
				break;
			case FsCreateDirectory:
				NDK_FsCreateDirectory(TESTDIR);
				break;
			case FsRemoveDirectory:
				NDK_FsRemoveDirectory(TESTDIR);
				break;
#endif
	}
	return SUCC;
ERR:
	NDK_FsDel(RANDFILE);
	NDK_FsDel(DESTFILE);
#if !K21_ENABLE
	NDK_FsRemoveDirectory(TESTDIR);
#endif
	return ret;	
}

void fs200(void)
{
	/*private & local definition*/
	int  i = 0, bak = 0, num[20] ={0}, j = 0;
	int ret = -1,cnt =50, len = 0;//cnt ����
#if K21_ENABLE&&!ANDIROD_ENABLE
	char *fsstr[] = {"NDK_FsOpen", "NDK_FsClose", "NDK_FsSeek", "NDK_FsWrite", "NDK_FsDel", "NDK_FsRead", "NDK_FsTell", "NDK_FsRename", "NDK_FsTruncate", "NDK_FsExist", "NDK_FsFileSize",  "NDK_FsGetDiskSpace", "NDK_FsDir", "NDK_FsClearAppFile"};
#elif ANDIROD_ENABLE
	char *fsstr[] = {"NDK_FsOpen", "NDK_FsClose", "NDK_FsSeek", "NDK_FsWrite", "NDK_FsDel", "NDK_FsRead", "NDK_FsTell", "NDK_FsRename", "NDK_FsTruncate", "NDK_FsExist", "NDK_FsFileSize", "NDK_CopyFileToSecMod"};
#else
	char *fsstr[] = {"NDK_FsOpen", "NDK_FsClose", "NDK_FsSeek", "NDK_FsWrite", "NDK_FsDel", "NDK_FsRead", "NDK_FsTell", "NDK_FsRename", "NDK_FsTruncate", "NDK_FsExist", "NDK_FsFileSize", "NDK_FsGetDiskSpace", "NDK_FsDir", "NDK_FsCreateDirectory", "NDK_FsRemoveDirectory"};
#endif

	/*process body*/
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
			if((ret = fs_randtest(num[i])) != NDK_OK)
			{
				send_result("line %d:%s��%d�β���ʧ��(%d,%d)", __LINE__, TESTAPI, bak,ret,num[i]);
				for(j=0;j<=i;j++)
					send_result("%s ",fsstr[num[j]]);
				return;
			}
		}
	}
	//���Ժ��� ɾ���ļ�
	NDK_FsDel(RANDFILE);
	NDK_FsDel(DESTFILE);
#if !K21_ENABLE
	NDK_FsRemoveDirectory(TESTDIR);
#endif
	//test over
	send_result("%s����ͨ��", TESTAPI);
	return;
}




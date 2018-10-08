/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: fsģ��
* file name			: fs19.c
* Author 			: zhengry
* version			: 
* DATE				: 20150723
* directory 		: 
* description		: ��������K21�ļ�ϵͳ����ļ���������ļ���С
* related document	: NDK.chm
*
************************************************************************/ 
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if defined SP610
#define  FILETEST19 "fs19_"  
#define  FILETEST19_1 "fs19_1"  
#define  FILE_TEST1 "ak_main.data"  
#define  FILE_TEST2 "ak_main.idx" 
#else
#define  FILETEST19 "/appfs/fs19_"  //���·��
#define  FILETEST19_1 "/appfs/fs19_1"  //���·��
#define  FILE_TEST1 "/appfs/ak_main.data"  //���·��
#define  FILE_TEST2 "/appfs/ak_main.idx"  //���·��
#endif

#define TESTAPI	"����ļ���������ļ���С"
#if defined ME31
#define MAX_FLIES_NUM 20
#define MAX_FLIES_SIZE 360		//������Ƶĵ����ļ���󳤶�,��λKB
#elif defined ME30||defined ME30S||defined ME50||defined ME30MH||defined ME30GX //||defined ME62
#define MAX_FLIES_NUM 55
#define MAX_FLIES_SIZE 512
#elif defined ME50NX||defined ME68
#define MAX_FLIES_NUM 254
#define MAX_FLIES_SIZE 65535
#elif defined ME11||defined ME15
#define MAX_FLIES_NUM 27
#define MAX_FLIES_SIZE 256		
#elif defined ME15C||defined ME20
#define MAX_FLIES_NUM 17
#define MAX_FLIES_SIZE 256
#elif defined ME30THM||defined ME32THM//ԭ���ļ�8Kһ����λ������ļ�����11�����ڸ�Ϊ4Kһ����λ������ļ���Ϊ24
#define MAX_FLIES_NUM 24
#define MAX_FLIES_SIZE 88
#elif defined ME32
#define MAX_FLIES_NUM 22
#define MAX_FLIES_SIZE 72
#elif defined ME65
#define MAX_FLIES_NUM 27
#define MAX_FLIES_SIZE 92
#elif defined ME32GX
#define MAX_FLIES_NUM 20
#define MAX_FLIES_SIZE 95
//SP610���ļ�ϵͳ �ļ���������Ϊ306��,�����ļ�����Сû������,Ԥ���ܹ�д���ļ�ϵͳ,д�����ļ�ϵͳ�ռ�Ϊ0,�ļ��ռ䱻����ļ�д�������,ɾ��ĳ�������ļ���,Ԥ��ʣ����ļ����ݲ�Ӧ�ñ��޸�
#elif defined SP610||defined ME50N||defined ME50H||defined ME50C||defined ME62
#define MAX_FLIES_NUM 306
#define MAX_FLIES_SIZE 65535	//������û������,�����ó����ֵ
#endif

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  linwl       20150723       created
*****************************************************************/
void fs19(void)
{
	/*private & local definition*/
	int ret=0, fp1=0, writelen=0, i=0, maxfilessize = 0;
	char writebuf1[1024] = {0};
	char fname[13]={0};
	ulong pulSpace = 0;
	
	/*process body*/
	if(auto_flag==1)//NDK_FsClearAppFile������ɾ��paraconf�ļ����ʲ��Զ�����
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);

	//����ǰ��:
	//ɾ�����ܴ��ڵĲ����ļ�
#if 0//defined ME15C	//��֧��NDK_FsClearAppFile�������豸��򿪱�����ʹ��NDK_FsFormat����
	if(cls_show_msg("�״ν���˰���,��[Enter]���и�ʽ��,�ٴν���ʱ��[����]��ʼ����")==ENTER)
	{
		cls_show_msg1(3, "3��󽫸�ʽ��������...��ʽ��������,�������ٴο�ѡ���뱾�������в���");
		if((ret=NDK_FsFormat())!=NDK_OK)
		{
			send_result("line %d:��ʽ��������ʧ��(ret=%d)", __LINE__, ret);
			goto ERR;
		}
	}
#else
	if((ret=NDK_FsClearAppFile())!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#endif

	NDK_FsDel(FILE_TEST1);
	NDK_FsDel(FILE_TEST2);
	for (i=0; i<sizeof(writebuf1); i++)
		writebuf1[i]=rand()%256;
	
	//case1:����55���ļ�,ǰ55��Ӧ�óɹ���56��Ӧ��ʧ��
	for(i=0; i<MAX_FLIES_NUM; i++)
	{
		sprintf(fname, "%s%d", FILETEST19, i+1);
		if ((fp1=NDK_FsOpen(fname, "w"))<0)
		{
			send_result("line %d: %s����ʧ��(%d)(��%d���ļ�)", __LINE__, TESTAPI, fp1, i+1);
			goto ERR;
		}
		//ME32����1k���ļ�Ҳ�Ƿ���4K�Ŀռ䣬���ɴ���22���ļ�(������ƹ涨)����ʵ�ʴ�19����ʼ����Ϊ�ռ䲻�������޷�д��1k�����ˣ�����ֻ�ж�ǰ18���ļ�д��ĳ��� linying modified 20171212
#if defined ME32
        if(i<18) 
	{
		if((writelen=NDK_FsWrite(fp1,writebuf1, 1024))!=1024)
		{
		    send_result("line %d: %s����ʧ��(%d)(��%d���ļ�)", __LINE__, TESTAPI, writelen, i+1);
		    NDK_FsClose(fp1);
		    goto ERR;
		}
        }
#elif defined ME32GX  //ME32GX ���ɴ���20���ļ���ʵ�ʴ�15����ʼ����Ϊ�ռ䲻�������޷�д��1k������
	if(i<14) 
	{
		if((writelen=NDK_FsWrite(fp1,writebuf1, 1024))!=1024)
		{
		    send_result("line %d: %s����ʧ��(%d)(��%d���ļ�)", __LINE__, TESTAPI, writelen, i+1);
		    NDK_FsClose(fp1);
		    goto ERR;
		}
        }
#elif defined ME65  
	if(i<23) 
	{
		if((writelen=NDK_FsWrite(fp1,writebuf1, 1024))!=1024)
		{
	   	 	send_result("line %d: %s����ʧ��(%d)(��%d���ļ�)", __LINE__, TESTAPI, writelen, i+1);
	    	NDK_FsClose(fp1);
	    	goto ERR;
		}
    	}
#else
        if((writelen=NDK_FsWrite(fp1,writebuf1, 1024))!=1024)
		{
			send_result("line %d: %s����ʧ��(%d)(��%d���ļ�)", __LINE__, TESTAPI, writelen, i+1);
			NDK_FsClose(fp1);
			goto ERR;
	    }
#endif
		NDK_FsClose(fp1);
	}
	//������56���ļ�Ӧ��ʧ��
	sprintf(fname, "%s%d", FILETEST19, i+1);
	if ((fp1=NDK_FsOpen(fname, "w"))>=0)
	{
		send_result("line %d: %s����ʧ��(%d)(��%d���ļ�)", __LINE__, TESTAPI, fp1, i+1);
		goto ERR;
	}
#if defined ME15C||defined ME20	//ME15Cɾ��֮ǰ���������ļ�,��Դ����
	for(i=0; i<MAX_FLIES_NUM; i++)
	{
		sprintf(fname, "%s%d", FILETEST19, i+1);//�����ļ���
		if ((ret=NDK_FsDel(fname))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)(��%d���ļ�)", __LINE__, TESTAPI, ret, i+1);
			goto ERR;
		}
	}
#else	
	//ɾ��һ���Ѵ��ڵ��ļ�
	NDK_FsDel(FILETEST19_1);
#endif

	//ͨ����ȡ����Ӳ��ʣ��ռ��������ƵĴ�С���бȽ�,����ʵ�ʲ���ʱ����ļ���СΪ��С��ֵ
	NDK_FsGetDiskSpace(1,&pulSpace);
	maxfilessize = pulSpace==0?MAX_FLIES_SIZE:((pulSpace/1024)<MAX_FLIES_SIZE)?(pulSpace/1024):MAX_FLIES_SIZE;	
	//case2:�ļ���󳤶�Ϊ512K��д���512K+1�ֽ�Ӧ��ʧ��
	if ((fp1=NDK_FsOpen(FILETEST19_1, "w"))<0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp1);
		goto ERR;
	}
	for(i=0;i<maxfilessize;i++)
	{
		if((writelen=NDK_FsWrite(fp1,writebuf1, 1024))!=1024)
		{
			if(i+1==maxfilessize)
				;//����ÿ���ļ���ҪһЩ�ռ����ļ���Ϣ,�����޷�д��512K,���1Kд����1024
			else
			{
				send_result("line %d: %s����ʧ��(%d)(��%dK)", __LINE__, TESTAPI, writelen, i+1);
				NDK_FsClose(fp1);
				goto ERR;
			}
		}
	}
	if((writelen=NDK_FsWrite(fp1,writebuf1, 1))!=0)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, writelen);
		NDK_FsClose(fp1);
		goto ERR;
	}
	NDK_FsClose(fp1);

	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
ERR:
#if 0//defined ME15C	//ME15Cɾ��֮ǰ���������ļ�,��Դ����
	for(i=0; i<MAX_FLIES_NUM; i++)
	{
		sprintf(fname, "%s%d", FILETEST19, i+1);
		NDK_FsDel(fname);
	}
#else
	NDK_FsClearAppFile();
#endif
	return;
}


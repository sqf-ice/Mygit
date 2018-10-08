/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: usb
* file name		: 
* Author 			: 
* version			: V1.0
* DATE			: 
* directory 		: 
* description		: ����NDK_DiskGetInfo�ܷ���ȷ��ȡ��Ϣ
 * related document	: ����Ա�ο��ֲ�
 *
 ************************************************************************

 * log			: 
 * Revision 1.0  
 * no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_DiskGetInfo"
#define	TESTFILE	"test.file"
#define	BUFFERSIZE	(1024*20)
#define MAXWAITTIME 30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		
*****************************************************************/
void sdusb4(void)
{
	if(auto_flag==2)
		return;
	/*private & local definition*/
	char rootdir[][16] = {{0}}, fname[64] = {0}, writebuf[BUFFERSIZE] = {0}, readbuf[BUFFERSIZE] = {0};
	int ret = 0, fp = 0, loop = 0,state = 0, cnt = rand()%1023+2 /*�����ļ���С��Լ����[40K, 20M]*/, wrlen = BUFFERSIZE-rand()%8;
	time_t oldtime = 0, diff1 = 0, diff2 = 0;
	uint punSize ;
	ST_DISK_INFO pstInfo;
//	ST_DISK_INFO diskinfo;
//	ST_DISK_INFO  *pstInfo = &diskinfo;//=(ST_DISK_INFO *)malloc(sizeof(ST_DISK_INFO));
	int disktype = get_disktype();
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ�ñ���
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:�������ǰ��ʧ��(ret=%d)", __LINE__, ret);
		return;	
	}
	
	//case1,����ֱ�ӵ���DiskGetInfo
	if((ret=NDK_DiskGetInfo(rootdir[0],&pstInfo))!=NDK_ERR)
	{		
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�����豸
	cls_show_msg1(3,"��ȷ���Ѳ���%s,�����������...",(disktype==UDISK)?"U��":"SD��");
	if((ret=NDK_DiskOpen(disktype,MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%sʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	oldtime=time(NULL);
	while(1)
	{
		if((ret=NDK_DiskGetState(disktype,&state))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		} 
		if(state==DISKMOUNTSUCC)
			break;
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			send_result("line %d:%s����ʧ�ܳ�ʱ", __LINE__, TESTAPI);	
			goto ERR;	
		}
		//usleep(100);
		NDK_SysMsDelay(1);
	}
	if((ret=NDK_DiskGetRootDirName(disktype,(char **)rootdir))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:�Ƿ���������
	if((ret=NDK_DiskGetInfo(NULL,&pstInfo))!=NDK_ERR_USDDISK_PARAM)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_DiskGetInfo(rootdir[0],NULL))!=NDK_ERR_USDDISK_PARAM)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:��������
	if((ret=NDK_DiskGetInfo(rootdir[0],&pstInfo))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	else
	{
		if(auto_flag==0||auto_flag==2)
		{
			if(cls_show_msg1(MAXWAITTIME, "ʣ��ռ�:[%u]k,�ܿռ�:[%u]k.\n"
										"ȷ�ϼ�:һ��\n"
										"������:����\n", pstInfo.unFreeSpace,pstInfo.unTotalSpace)!=ENTER)

			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				GOTOERR;		
			}
		}
		else
			send_result("ʣ��ռ�:[%u]k,�ܿռ�:[%u]k.���ж��Ƿ���ȷ", pstInfo.unFreeSpace,pstInfo.unTotalSpace);
	}

	//case3,���ж�д������Ȼ����֤�ռ��С
	cls_show_msg1(5, "���²���ʹ��%dB��Ϊÿ�ζ�д����!", wrlen);
	sprintf(fname, "%s/%s", rootdir[0], TESTFILE);
	NDK_FsDel(fname);
	if ((fp=NDK_FsOpen(fname, "w"))<0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	cls_printf("����%s�ļ���(Լ%dkB),���Ժ�...",(disktype==UDISK)?"U��":"SD��", cnt*wrlen/1024);
	for (loop=0; loop<wrlen; loop++)
		writebuf[loop] = rand()%256;
	oldtime = time(NULL);
	for (loop=0; loop<cnt; loop++)
	{
		if ((ret=NDK_FsWrite( fp,writebuf, wrlen))!=wrlen)
		{
			NDK_FsClose(fp);
			NDK_FsDel(fname);
			send_result("line %d:%s����ʧ��(cnt=%d, loop=%d, ret=%d, wrlen=%d)", __LINE__, TESTAPI, cnt, loop, ret, wrlen);
			GOTOERR;
		}
		show_stopwatch(ENABLE, (diff1=time(NULL)-oldtime));
	}
	show_stopwatch(DISABLE, 0);

	if(NDK_FsClose(fp)!=NDK_OK)
	{
		NDK_FsDel(fname);
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;		
	}
	if((ret=NDK_DiskClose(disktype))!=NDK_OK)
	{
		send_result("line %d:UsbDiskCloseʧ��(ret:%d)", __LINE__, ret);
		GOTOERR;
	}
	Beep(3);//д��ɵ���ʾ��
	cls_show_msg1(3,"�����������...");
	if((ret=NDK_DiskOpen(disktype,MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:UsbDiskOpenʧ��(ret:%d)", __LINE__, ret);
		GOTOERR;
	}
	oldtime=time(NULL);
	while(1)
	{
		if((ret=NDK_DiskGetState(disktype,&state))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		} 
		if(state==DISKMOUNTSUCC)
			break;
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			send_result("line %d:%s����ʧ�ܳ�ʱ", __LINE__, TESTAPI);	
			goto ERR;	
		}
		//usleep(100);
		NDK_SysMsDelay(1);
	}
	if ((fp=NDK_FsOpen(fname, "r"))<0)
	{
		NDK_FsDel(fname);
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	//�ļ���С�Ƿ����ı�
	if ((NDK_FsFileSize(fname, &punSize))!=NDK_OK)
	{	
		NDK_FsClose(fp);
		NDK_FsDel(fname);
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	//fprintf(stderr,"%s %d %d\n",__FUNCTION__,__LINE__,*punSize);
	if ((punSize)!=cnt*wrlen)
	{	
		NDK_FsClose(fp);
		NDK_FsDel(fname);
		send_result("line %d:%s����ʧ��(ʵ��:%dB, Ԥ��:%dB)", __LINE__, TESTAPI, ret, cnt*wrlen);
		GOTOERR;
	}
	cls_printf("У��U���ļ���(Լ%dkB),���Ժ�...", cnt*wrlen/1024);
	oldtime = time(NULL);
	for (loop=0; loop<cnt; loop++)
	{
		if ((ret=NDK_FsRead( fp,readbuf, wrlen))!=wrlen||memcmp(writebuf, readbuf, wrlen))
		{
			NDK_FsClose(fp);
			NDK_FsDel(fname);
			send_result("line %d:%s����ʧ��(cnt=%d, loop=%d, ret=%d, wrlen=%d)", __LINE__, TESTAPI, cnt, loop, ret, wrlen);
			debug_buf("���ͻ���", writebuf, wrlen);
			debug_buf("���ջ���", readbuf, ret);
			cls_show_msg("�뱣�����ݲ��رմ���,���������...");//��ֹ�������UsbDiskClose��,Ҳ��USB������Ϣ��¼������������
			GOTOERR;
		}
		show_stopwatch(ENABLE, (diff2=time(NULL)-oldtime));
	}
	show_stopwatch(DISABLE, 0);
	if((ret=NDK_DiskGetInfo(rootdir[0],&pstInfo))!=NDK_OK)
	{
		NDK_FsClose(fp);
		NDK_FsDel(fname);
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	else
	{
		if(auto_flag==0||auto_flag==2)
		{
			if(cls_show_msg1(MAXWAITTIME, "ʣ��ռ�:[%u]k,�ܿռ�:[%u]k.\n"
										"ȷ�ϼ�:һ��\n"
										"������:����\n", pstInfo.unFreeSpace,pstInfo.unTotalSpace)!=ENTER)

			{
				NDK_FsClose(fp);
				NDK_FsDel(fname);
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);		
				goto ERR;
			}
		}
		else
			send_result("ʣ��ռ�:[%u]k,�ܿռ�:[%u]k.���ж��Ƿ���ȷ", pstInfo.unFreeSpace,pstInfo.unTotalSpace);
	}

	//test over
	send_result("%s����ͨ��(д%dkB/s,��%dkB/s)", TESTAPI, (cnt*wrlen)/(1024*diff1), (cnt*wrlen)/(1024*diff2));
	NDK_FsClose(fp);
	NDK_FsDel(fname);
ERR:
	show_stopwatch(DISABLE, 0);
	NDK_DiskClose(disktype);
	return;
}


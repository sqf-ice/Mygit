/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �ۺϲ���
* file name		: 
* Author 			: chensj
* version			: V1.0
* DATE			: 2013/06/17
* directory 		:
* description		:SD/U���ۺϲ���
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"SD/U���ۺ�"
#define	TESTFILE	"test.file"
#define	TESTBIGFILE	"testbig.file"
#define	BUFFERSIZE	(1024*20) //��������СЩ��������ƶ�ջ�����ƻ�����
#define MAXWAITTIME 10//���������س�ʱʱ��
#define GETSTATETIMES 30//��ȡ״̬����
#define	DEFAULT_CNT_STR	"1000"	//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)
#define   ABILITYNUM	 20 //���ܲ��Դ���

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
void SD_press_mnt(int devicetype)
{
	/*private & local definition*/
	int ret = 0, cnt = 0,bak=0, succ=0, nCnt = 0, state = 0;
	char CntStr[8]=DEFAULT_CNT_STR;
	uint nLen=0;
	
	/*process body*/
	if(auto_flag==1)
	{
		bak = g_CycleTime;
	}
	else
	{
		cls_printf("Ĭ��ѹ�����Դ���:");
		if ((ret=lib_kbgetinput(CntStr,2, sizeof(CntStr)-1,&nLen, 2, 5,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
			bak = DEFAULT_CNT_VLE;
		else
			bak = atoi(CntStr);
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"��ȷ���Ѳ���%s,�����������...", devicetype==SDDISK?"SD��":"U��");

	while(1)
	{
		//��������
		NDK_DiskClose(devicetype);
		if(cls_show_msg1(2, "�ܹ�%d�ο�ʼ��%d��%sװ��/ж��ѹ��,������˳�...", bak,cnt+1, devicetype==SDDISK?"SD��":"U��")!=0)
			break;
		if(cnt==bak)
			break;
		cnt++;
		
		if((ret=NDK_DiskOpen(devicetype,MAXWAITTIME))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�μ���ʧ��", __LINE__, cnt);
			continue;
		}
		nCnt = 0;
		while(1)
		{
			if((ret=NDK_DiskGetState(devicetype,&state))!=NDK_OK)
				;
			if(state==DISKMOUNTSUCC)
				break;
			sleep(1);
			if(++nCnt>=GETSTATETIMES)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d�β���δ��ȡ��DISKMOUNTSUCC״̬", __LINE__,cnt);
				NDK_DiskClose(devicetype);
				return;//���30��״̬,���30�ζ�û��⵽DISKMOUNTSUCC,�ͱ����˳�
			}
		}
		if((ret=NDK_DiskClose(devicetype))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��ж��ʧ��", __LINE__, cnt);
			continue;	
		}
		succ++;
		
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"ѹ���������!��ִ��:%d��,�ɹ�:%d��", cnt, succ);
	return;
}

void SD_press_wr(int devicetype)
{
	/*private & local definition*/
	char rootdir[][16] ={{0}}, fname[64] = {0}, writebuf[BUFFERSIZE] = {0}, readbuf[BUFFERSIZE] = {0};
	int ret = 0, fp = 0, loop = 0, cnt = 0, wrlen = 0, i = 0, state = 0, bak = 0, nCnt = 0, nSuccNum = 0;
	time_t oldtime = 0, diff2 = 0;
	char CntStr[8]=DEFAULT_CNT_STR;
	uint nLen=0;
	
	/*process body*/
	if(auto_flag==1)
	{
		bak = g_CycleTime;
	}
	else
	{
		cls_printf("Ĭ��ѹ�����Դ���:");
		if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&nLen, 2, 5,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
			bak = DEFAULT_CNT_VLE;
		else
			bak = atoi(CntStr);
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"��ȷ���Ѳ���%s,�����������...", devicetype==SDDISK?"SD��":"U��");
//����ǰ��:�ȰѴ��ڵĲ����ļ���ֵ��fname
	NDK_DiskGetRootDirName(devicetype, (char **)rootdir);//����ެ��ȷ�Ͽ���ֱ�ӵ��û�ȡ��Ŀ¼�������open
	sprintf(fname, "%s/%s", rootdir[0], TESTFILE);//ɾ�������ļ��������ڲ����ļ��������������дУ���������ǰ�ڶ�д�������쳣�ϵ� 20140730 jiangym
BEGIN:
	while(1)
	{
		//��������
		NDK_FsClose(fp);
		NDK_FsDel(fname);//ɾ�����ڵĲ����ļ�
		NDK_DiskClose(devicetype);
		
		if(cls_show_msg1(1, "�ܹ�%d�ο�ʼ��%d��%s��дѹ��,������˳�...",bak, i+1, devicetype==SDDISK?"SD��":"U��")!=0)
			break;
			
		if(i==bak)//�ﵽ���ô������˳�
			break;
		i++;//���Դ�������
		
		//���豸
		if((ret=NDK_DiskOpen(devicetype,MAXWAITTIME))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NDK_DiskOpenʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		//��ȡ�豸״̬
		nCnt = 0;
		while(1)
		{
			if((ret=NDK_DiskGetState(devicetype,&state))!=NDK_OK)
				;
		
			if(state==DISKMOUNTSUCC)
				break;
			sleep(1);
			if(++nCnt>=GETSTATETIMES)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d��δ��ȡ��DISKMOUNTSUCC״̬", __LINE__, i);
				goto BEGIN;
			}
		}
		//��ȡ�豸��Ŀ¼
		if((ret=NDK_DiskGetRootDirName(devicetype, (char **)rootdir))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�λ�ȡ��Ŀ¼ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		sprintf(fname, "%s/%s", rootdir[0], TESTFILE);
		
		if ((fp=NDK_FsOpen(fname, "w"))<0)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ��������ļ�ʧ��(%d)", __LINE__, i, fp);
			continue;
		}
		//д�ļ�
		cnt = rand()%1023+2;//д�ļ����� ��С2��
		wrlen = BUFFERSIZE-rand()%8;//һ��д�ļ�Լ20K   �ϼ�/*�����ļ���С��Լ����[40K, 20M]*/
		for (loop=0; loop<wrlen; loop++)
			writebuf[loop] = rand()%256;//�������
		for(loop=0; loop<cnt; loop++)
		{
			cls_printf("��%d�������豸�ļ���(Լ%dkB),������(%dkB)���Ժ�...", i, cnt*wrlen/1024, loop*wrlen/1024);
			if ((ret=NDK_FsWrite(fp,writebuf,wrlen))!=wrlen)
			{	
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��д�����ļ�ʧ��(%d)", __LINE__, i, ret);
				goto BEGIN;
			}
		}
		//�ر��ļ�
		if(NDK_FsClose(fp)!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ιرղ����ļ�ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		//�ر��豸
		if((ret=NDK_DiskClose(devicetype))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NDK_DiskCloseʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		sleep(3);

		//���´��豸У���ļ�
		if((ret=NDK_DiskOpen(devicetype,MAXWAITTIME))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NDK_DiskOpenʧ��(%d)", __LINE__, i, ret);
			continue;	
		}
		//��ȡ�豸״̬
		nCnt = 0;
		while(1)
		{
			if((ret=NDK_DiskGetState(devicetype,&state))!=NDK_OK)
				;
			if(state==DISKMOUNTSUCC)
				break;
			sleep(1);
			if(++nCnt>=GETSTATETIMES)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d��δ��ȡ��DISKMOUNTSUCC״̬", __LINE__, i);
				goto BEGIN;
			}
		}
		//���ļ�
		if ((fp=NDK_FsOpen(fname, "r"))<0)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ��������ļ�ʧ��(%d)", __LINE__, i, fp);
			continue;
		}
		
		//�ļ���С�Ƿ����ı�
		if(NDK_FsFileSize(fname, (uint *)&ret)!=NDK_OK||ret!=cnt*wrlen)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��У���ļ�ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		//���ļ�
		cls_printf("��%d��У��U���ļ���(Լ%dkB),���Ժ�...", i, cnt*wrlen/1024);
		oldtime = time(NULL);
		for (loop=0; loop<cnt; loop++)
		{
			if ((ret=NDK_FsRead(fp,readbuf, wrlen))!=wrlen||memcmp(writebuf, readbuf, wrlen))
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζ��ļ�ʧ��(%d)", __LINE__, i, ret);
				goto BEGIN;
			}
			show_stopwatch(ENABLE, (diff2=time(NULL)-oldtime));
		}
		show_stopwatch(DISABLE, 0);
		//ɾ���ļ��͹ر��豸
		NDK_FsClose(fp);
		NDK_FsDel(fname);
		if((ret=NDK_DiskClose(devicetype))!=NDK_OK)//�����umount
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��NDK_DiskCloseʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		nSuccNum++;//�ɹ�����
	}

	cls_show_msg_record(FILENAME,FUNCNAME,"%s��дѹ���������,һ��%d�β����ѳɹ�%d��", devicetype==SDDISK?"SD��":"U��", i, nSuccNum);
	return;
}

static void SD_filesys_bigfile(int devicetype)
{
	/*private & local definition*/
	int fp = 0, writelen = 0, loop = 0, ret = 0, state = 0,cnt = 0,i = 0,j=0,m=0,n=0,nCnt,succ=0;
	char rootdir[][16] = {{0}}, fname[64] = {0}, writebuf[BUFFERSIZE] = {0},filename[4] = {0};
	ST_DISK_INFO pstInfo;

	memset(&pstInfo,0,sizeof(pstInfo));
	/*process body*/
	//��������
	NDK_DiskClose(devicetype);	

	cls_show_msg_record(FILENAME,FUNCNAME,"��ȷ���Ѳ���%s,�����������...", devicetype==SDDISK?"SD��":"U��");
	
	if((ret=NDK_DiskOpen(devicetype,MAXWAITTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:��%sʧ��", __LINE__,devicetype==SDDISK?"SD��":"U��");
		return;
	}

	nCnt=0;
	while(1)
	{
		if((ret=NDK_DiskGetState(devicetype,&state))!=NDK_OK)
			;	
		if(state==DISKMOUNTSUCC)
			break;
		sleep(1);
		if(++nCnt>=GETSTATETIMES)
		{
			cls_show_msg1(g_keeptime,"line %d:ȡ%s״̬ʧ��", __LINE__,devicetype==SDDISK?"SD��":"U��");
			NDK_DiskClose(devicetype);
			return;
		}
	}

	if((ret=NDK_DiskGetRootDirName(devicetype, (char **)rootdir))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:DiskGetRootdirNameʧ��(ret:%d)", __LINE__, ret);
		NDK_DiskClose(devicetype);
		return;
	}

	
BEGIN:
	while(1)
	{
		//��������
		NDK_FsClose(fp);
		
		i++;
		if(cls_show_msg1(3, "��ʼ��%d��%s��дѹ��,������˳�...", i, devicetype==SDDISK?"SD��":"U��")!=0)
			break;
		for(loop=0;loop<3;loop++)
      		sprintf(filename+loop, "%x", rand()%9);
		sprintf(fname, "%s/%s", rootdir[0], filename);

		if ((fp=NDK_FsOpen(fname, "w"))<0)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d�ļ���ʧ��",__LINE__);	
			if(++j>=GETSTATETIMES)//����ʧ�����˳����򣬷�ֹ��ѭ��
				goto ERR;	
			continue;
		}
		j=0;

		memset(writebuf, rand()/256, sizeof(writebuf));
		
		cnt = rand()%1023+2 , writelen = BUFFERSIZE-rand()%8;
		for (loop=0; loop<cnt; loop++)
		{
			if((ret=NDK_DiskGetInfo(rootdir[0],&pstInfo))!=NDK_OK)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d��ȡ%s��Ϣʧ��",__LINE__,devicetype==SDDISK?"SD��":"U��");	
				if(++m>=GETSTATETIMES)
					goto ERR;
				goto BEGIN;
			}
			m=0;
			cls_printf("%s�ܿռ�:[%u]k\nʣ��ռ�:[%u]k\n�˴�д��[%d]k\nʣ��[%d]k\n",devicetype==SDDISK?"SD��":"U��", pstInfo.unTotalSpace,pstInfo.unFreeSpace,cnt*writelen/1024,(cnt*writelen/1024-loop*writelen/1024));
		
			if ((writelen=NDK_FsWrite(fp,writebuf, sizeof(writebuf)))!=sizeof(writebuf))
			{
				cls_printf("line %d:�豸��д��,����������!(%d,%d)", __LINE__, loop, writelen);
				NDK_FsClose(fp);
				NDK_DiskClose(devicetype);
				while(1);//��ѭ��,��ǿ���û��ػ�
				//return;
			}
		}
		if(NDK_FsClose(fp)!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d�ر��ļ�ʧ��",__LINE__);
			if(++n>=GETSTATETIMES)
				goto ERR;
			NDK_FsDel(fname);
			continue;	
		}
		n=0;
		
		succ++;
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"line %d:������δ��д��,����ʧ��!\n�ܹ�д�ļ�%d�γɹ�%d��", __LINE__,i-1,succ);
ERR:
	NDK_FsClose(fp);
	NDK_FsDel(fname);
	NDK_DiskClose(devicetype);
	return;
}

void sd_abnormal(int devicetype)
{
	/*private & local definition*/
	char rootdir[][16] ={{0}}, fname[64] = {0}, writebuf[BUFFERSIZE] = {0}, readbuf[BUFFERSIZE] = {0};
	int ret = 0, fp = 0, state = 0, nCnt = 0;
	
	/*process body*/
	cls_show_msg("��ȷ���Ѳ���%s,�����������...", devicetype==SDDISK?"SD��":"U��");
	//����ǰ�ã�
	NDK_DiskClose(devicetype);
	
	//case1:��֤���豸�󣬽��������ٻ��ѣ�Ӧ���������ؽ��ж�д
	if((ret=NDK_DiskOpen(devicetype,MAXWAITTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:��%sʧ��", __LINE__,devicetype==SDDISK?"SD��":"U��");
		goto END;
	}
	nCnt=0;
	while(1)
	{
		if((ret=NDK_DiskGetState(devicetype,&state))!=NDK_OK)
			;	
		if(state==DISKMOUNTSUCC)
			break;
		sleep(1);
		if(++nCnt>=GETSTATETIMES)
		{
			cls_show_msg1(g_keeptime,"line %d:ȡ%s״̬ʧ��", __LINE__,devicetype==SDDISK?"SD��":"U��");
			goto END;
		}
	}
	cls_show_msg("%s,�����������",devicetype==SDDISK?"��ȴ�POS�������ߺ�����":"��ȴ������Զ�����ʱ��,U�̲�������");//jiangym modified 20140623 U���޸ĳ�OPEN�󲻽����ߣ���Ӧ���޸�����

	if((ret=NDK_DiskGetRootDirName(devicetype, (char **)rootdir))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:DiskGetRootdirNameʧ��(ret:%d)", __LINE__, ret);
		goto END;
	}
	sprintf(fname, "%s/%s", rootdir[0], TESTFILE);
	
	NDK_FsDel(fname);
	if((fp=NDK_FsOpen(fname, "w"))<0)
	{
		cls_show_msg1(g_keeptime, "line %d:���������ļ�ʧ��(%d)", __LINE__, fp);
		goto END;
	}
	memset(writebuf, rand()/256, sizeof(writebuf));
	if((ret=NDK_FsWrite(fp,writebuf,BUFFERSIZE))!=BUFFERSIZE)
	{	
		cls_show_msg1(g_keeptime, "line %d:д�����ļ�ʧ��(%d)", __LINE__, ret);
		goto END;
	}
	NDK_FsClose(fp);
	//�ر��豸
	if((ret=NDK_DiskClose(devicetype))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:NDK_DiskCloseʧ��(%d)", __LINE__, ret);
		goto END;
	}
	sleep(3);
	//���´��豸У���ļ�
	if((ret=NDK_DiskOpen(devicetype,MAXWAITTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:NDK_DiskOpenʧ��(%d)", __LINE__, ret);
		goto END;
	}
	//��ȡ�豸״̬
	nCnt = 0;
	while(1)
	{
		if((ret=NDK_DiskGetState(devicetype,&state))!=NDK_OK)
			;
		if(state==DISKMOUNTSUCC)
			break;
		sleep(1);
		if(++nCnt>=GETSTATETIMES)
		{
			cls_show_msg1(g_keeptime,"line %d:δ��ȡ��DISKMOUNTSUCC״̬", __LINE__);
			goto END;
		}
	}
	cls_show_msg("%s,�����������",devicetype==SDDISK?"���ٴεȴ�POS�������ߺ�����":"��ȴ������Զ�����ʱ��,U�̲�������");//jiangym modified 20140724
	//���ļ�
	if((fp=NDK_FsOpen(fname, "r"))<0)
	{
		cls_show_msg1(g_keeptime, "line %d:���ļ�ʧ��(%d)", __LINE__, fp);
		goto END;
	}
	//���ļ�
	if ((ret=NDK_FsRead(fp, readbuf, BUFFERSIZE))!=BUFFERSIZE||memcmp(writebuf, readbuf, BUFFERSIZE))
	{
		cls_show_msg1(g_keeptime, "line %d:У���ļ�ʧ��(%d)", __LINE__, ret);
		goto END;
	}
	
	cls_show_msg("%s�쳣����ͨ��",devicetype==SDDISK?"SD��":"U��");
END:
	NDK_FsClose(fp);
	NDK_FsDel(fname);
	NDK_DiskClose(devicetype);
	return;
}

/****************************************************************
* function name 	 	:
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		  
*****************************************************************/
void sd_press(int devicetype)
{
	/*private & local definition*/
	int keyin = '0';
	
	/*process body*/
	while (1)
	{	

		if(g_SequencePressFlag)
		{
			if(( ++keyin=='3') || (cls_show_msg1(2, "��������%s����ѹ������,ESC�˳�",TESTITEM))==ESC)
				return;
		}
		else
		{
			keyin = cls_show_msg("1.װ��/ж��ѹ��\n"
								"2.�豸��дѹ��\n"
								"3.д���豸\n"
								);
		}
		
		switch(keyin)
		{
		case '1':
			SD_press_mnt(devicetype);
			break;
		case '2':
			SD_press_wr(devicetype);
			break;
		case '3':
			SD_filesys_bigfile(devicetype);
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
}

void sd_ability(int devicetype)
{
	/*private & local definition*/
	char rootdir[][16] = {{0}}, fname[64] = {0}, writebuf[BUFFERSIZE] = {0}, readbuf[BUFFERSIZE] = {0};
	int ret = 0, fp = 0, loop = 0, cnt = 50, wrlen = BUFFERSIZE, i = 0,state = 0;
	time_t oldtime = 0, diff1 = 0, diff2 = 0;
	float hanguptime = 0, hangdowntime = 0, writetime = 0, readtime = 0, readtime2 = 0, cmptime = 0;
	uint unSize = 0;

	/*process body*/
	cls_show_msg_record(FILENAME,FUNCNAME,"��ȷ���Ѳ���%s,�����������...", devicetype==SDDISK?"SD��":"U��");
	
	while(1)
	{
		if(cls_show_msg1(3, "��ʼ��%d��%s��д,��ESC�˳�...", i+1, devicetype==SDDISK?"SD��":"U��")==ESC)
			break;

		if (i++>=ABILITYNUM)
			break;
		
		//��SD
		lib_StartStopwatch();
		if((ret=NDK_DiskOpen(devicetype,MAXWAITTIME))!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:DiskOpenʧ��(ret:%d)", __LINE__, ret);
			return;
		}
		oldtime=time(NULL);
		while(1)
		{
			if((ret=NDK_DiskGetState(devicetype,&state))!=SUCC)
			{
				cls_show_msg1(g_keeptime,"line %d:DiskGetStateʧ��(ret:%d)", __LINE__, ret);
				goto ERR;
			} 
			if(state==DISKMOUNTSUCC)
				break;
			if(time(NULL)-oldtime>MAXWAITTIME)
			{
				cls_show_msg1(g_keeptime,"line %d:��ȡ״̬��ʱstate=%d", __LINE__, state);	
				goto ERR;	
			}
			sleep(1);
		}		
		hanguptime += lib_ReadStopwatch();
		
		if((ret=NDK_DiskGetRootDirName(devicetype, (char **)rootdir))<0)
		{
			cls_show_msg1(g_keeptime,"line %d:DiskGetRootdirNameʧ��(ret:%d)", __LINE__, ret);
			goto ERR;	
		}	
		cls_show_msg1(5, "���²���ʹ��%dB��Ϊÿ�ζ�д����!", wrlen);
		sprintf(fname, "%s/%s", rootdir[0], TESTFILE);
		NDK_FsDel(fname);
		if ((fp=NDK_FsOpen(fname, "w"))<0)
		{
			cls_show_msg1(g_keeptime,"line %d:�����ļ�ʧ��", __LINE__);
			goto ERR;
		}
		//д�ļ�
		cls_printf("��%d���������ļ���(Լ%dkB),���Ժ�...", i, cnt*wrlen/1024);
		for (loop=0; loop<wrlen; loop++)
			writebuf[loop] = rand()%256;
		
		oldtime = time(NULL);
		lib_StartStopwatch();
		for (loop=0; loop<cnt; loop++)
		{
			if ((ret=NDK_FsWrite(fp,writebuf, wrlen))!=wrlen)
			{
				cls_show_msg1(g_keeptime,"line %d:��%d��д�ļ�ʧ��(loop=%d, ret=%d, wrlen=%d)", __LINE__, i, loop, ret, wrlen);
				goto ERR;
			}
			show_stopwatch(ENABLE, (diff1=time(NULL)-oldtime));
		}
		writetime += lib_ReadStopwatch();
		show_stopwatch(DISABLE, 0);	
		
		if(NDK_FsClose(fp)!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:�ر��ļ�ʧ��", __LINE__);
			goto ERR;
		}
		if ((fp=NDK_FsOpen(fname, "r"))<0)
		{
			cls_show_msg1(g_keeptime,"line %d:���ļ�ʧ��", __LINE__);
			goto ERR;
		}
		//�ļ���С�Ƿ����ı�
		if ((ret=NDK_FsFileSize(fname,&unSize))!=NDK_OK||unSize!=cnt*wrlen)
		{
			cls_show_msg1(g_keeptime,"line %d:��%d���ļ���С��һ��(ʵ��:%dB, Ԥ��:%dB)", __LINE__, i, ret, cnt*wrlen);
			goto ERR;
		}
		//���ļ�
		cls_printf("��%d��У���ļ���(Լ%dkB),���Ժ�...", i, cnt*wrlen/1024);
		
		oldtime = time(NULL);
		lib_StartStopwatch();
		for (loop=0; loop<cnt; loop++)
		{
			if (((ret=NDK_FsRead(fp,readbuf, wrlen))!=wrlen)||memcmp(writebuf, readbuf, wrlen))
			{
				cls_show_msg1(g_keeptime,"line %d:��%d�ζ��ļ�ʧ�ܻ�У��ʧ��(loop=%d, ret=%d, wrlen=%d)", __LINE__, i, loop, ret, wrlen);
				goto ERR;
			}
			show_stopwatch(ENABLE, (diff2=time(NULL)-oldtime));
		}
		readtime += lib_ReadStopwatch();
		show_stopwatch(DISABLE, 0);
		
		lib_StartStopwatch();
		for(loop=0; loop<cnt; loop++)
		{
			memcmp(writebuf, readbuf, wrlen);
		}
		cmptime += lib_ReadStopwatch();

		readtime2=readtime-cmptime;

		NDK_FsClose(fp);
		NDK_FsDel(fname);
		
		//�ر�SD
		lib_StartStopwatch();
		if((ret=NDK_DiskClose(devicetype))!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:DiskCloseʧ��(ret:%d)", __LINE__, ret);
			goto ERR;
		}	
		hangdowntime += lib_ReadStopwatch();
		
	}//ѭ��20�κ����open��close��ʱ��Ͷ�д�ٶ�,ȡƽ��ֵ
	if(i==ABILITYNUM+1)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"open %sʱ��:%fs/��,close %sʱ��:%fs/��",devicetype==SDDISK?"SD��":"U��", hanguptime/ABILITYNUM,devicetype==SDDISK?"SD��":"U��", hangdowntime/ABILITYNUM);
		cls_show_msg_record(FILENAME,FUNCNAME,"дʱ��:%fs/��,��ʱ��:%fs/��", writetime/ABILITYNUM, readtime2/ABILITYNUM);
		cls_show_msg_record(FILENAME,FUNCNAME,"д�����ٶ�%fkB/s,�������ٶ�:%fkB/s",((cnt*20*ABILITYNUM)/(writetime)), ((cnt*20*ABILITYNUM)/(readtime2)));
	}
	else
		cls_show_msg_record(FILENAME,FUNCNAME,"�û�ȡ������");
ERR:
	NDK_FsClose(fp);
	NDK_FsDel(fname);	
	NDK_DiskClose(devicetype);
	return;
}

void systest40(void)
{
	/*private & local definition*/
	int keyin = 0, disktype = SDDISK, ret = 0;
	
	/*process body*/
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_disktype()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,SD/U������ʧ��,���������ļ�",TESTITEM);
			return;
		}
		disktype=get_disktype();
		g_CycleTime = 100;

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ��SD/U��ѹ������
		sd_press(disktype);

		//ִ��SD/U�����ܲ���
		sd_ability(disktype);

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		keyin = cls_show_msg("_SD/U���ۺϲ���_\n"
							"1.ѹ�� 2.����\n"
							"3.�쳣����\n"
							"4.SD/U������\n"
							);
		switch(keyin)
		{
		case '1':
			sd_press(disktype);
			break;
		case '2':
			//cls_show_msg("�ݲ�֧�����ܲ���");
			sd_ability(disktype);
			break;
		case '3':
			//cls_show_msg("�ݲ�֧���쳣����");
			sd_abnormal(disktype);
			break;
		case '4':
			if((ret=conf_disktype())!=SUCC)
			{
				cls_show_msg1(2, "�豸ѡ��ʧ��,������ѡ��", ret);
				return;
			}
			disktype=get_disktype();
			break;
		case ESC:
			cls_show_msg("��ʹ��8GB��16GB�Ŀ�Ƭ���½��в���!���������...");
			return;
			break;
		default:
			continue;
			break;
		}
	}
}


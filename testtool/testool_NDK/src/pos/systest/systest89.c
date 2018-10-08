/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 
* Author 			: zhengry
* version			: V1.0
* DATE			: 2016/06/02
* directory 		: 
* description		: ����ͷ/U�̽���
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  2016/06/02  zhengry
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"����ͷ/U�̽���"
#define MAXTIME		60
#define TESTFILE   "cameratest.file"
#define	BUFFERSIZE	(1024*5) //��������СЩ��������ƶ�ջ�����ƻ�����
#define GETSTATETIMES 30//��ȡ״̬����

/*----------global variables declaration("extern")--------------*/
static int nConfInit = 0;	 /*�Ƿ����ù��˲���:0δ���� 1������*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
void camera_UDISK(void)
{
	/*private & local definition*/
	int i = 0, j=0, succ = 0, ret=0, timeout = SO_TIMEO, fp = 0, nCnt=0, state = 0, wrlen = 0;
	char writebuf[BUFFERSIZE] = {0}, readbuf[BUFFERSIZE] = {0};
	char rootdir[][16] ={{0}}, fname[64] = {0};
	/*process body*/		
	cls_show_msg_record(FILENAME,FUNCNAME,"��ȷ���Ѱ�װU��,���������");	
	/*����ǰ��*/
	if(nConfInit == 0)
	{
		cls_show_msg1(g_keeptime, "����ͷ����δ����");
		return;
	}
	//�ȰѴ��ڵĲ����ļ���ֵ��fname
	NDK_DiskGetRootDirName(UDISK, (char**)rootdir);//����ެ��ȷ�Ͽ���ֱ�ӵ��û�ȡ��Ŀ¼�������open
	sprintf(fname, "%s/%s", rootdir[0], TESTFILE);//ɾ�������ļ��������ڲ����ļ��������������дУ���������ǰ�ڶ�д�������쳣�ϵ� 

	while(1)
	{
		//��������
ERR:
		NDK_FsClose(fp);
		NDK_FsDel(fname);
		NDK_DiskClose(UDISK);
		
		if(cls_show_msg1(2,"���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", i+1, TESTITEM, succ)==ESC)
			break;
		i++;
		sleep(5);//ÿ�ιҶ�֮����������Ҫ�ȴ�5��,������оƬ��ѹ��

		//����ͷ����
		if((ret=NDK_ScanInit())!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
			continue;
		}
		NDK_ScanSet(CAMERA_SETTYPE_IMAGE, g_ShowImage);
		NDK_ScanSet(CAMERA_SETTYPE_SATURATION, g_Saturation);
		NDK_ScanSet(CAMERA_SETTYPE_SHARPNESS, g_Sharpness);
		NDK_ScanSet(CAMERA_SETTYPE_GAMMA, g_Contrast);
		if((ret=lib_doscan(g_ScanType, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
			if(g_ScanType==1)
				NDK_ScanClose();
			continue;
		}
		if(g_ScanType==1)
			NDK_ScanClose();

		if((ret=NDK_DiskOpen(UDISK,timeout))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�δ�U��ʧ��(%d)",  __LINE__, i, ret);
			continue;
		}
		//��ȡ�豸״̬
		nCnt = 0;
		while(1)
		{
			if((ret=NDK_DiskGetState(UDISK,&state))!=NDK_OK)
				;
			if(state==DISKMOUNTSUCC)
				break;
			sleep(1);
			if(++nCnt>=GETSTATETIMES)
			{
				cls_show_msg1(g_keeptime,"line %d:��%d��δ��ȡ��DISKMOUNTSUCC״̬", __LINE__, i);
				goto ERR;
			}
		}
		//���ļ�
		if((fp=NDK_FsOpen(fname, "w"))<0)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�δ��������ļ�ʧ��(%d)", __LINE__, i, fp);
			continue;
		}
		//���ļ�д����
		wrlen = 1024;//
		for (j=0; j<wrlen; j++)
			writebuf[j] = rand()%256;//�������
		cls_printf("�����豸�ļ���(Լ%dB),���Ժ�...",wrlen);
		if ((ret=NDK_FsWrite(fp,writebuf,wrlen))!=wrlen)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��д�����ļ�ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		//�ر��ļ�
		if(NDK_FsClose(fp)!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�ιرղ����ļ�ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		//�ر��豸
		if((ret=NDK_DiskClose(UDISK))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��NDK_DiskCloseʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		//���´��豸У���ļ�
		cls_printf("���´��豸У���ļ�,���Ժ�...");
		if((ret=NDK_DiskOpen(UDISK,timeout))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��NDK_DiskOpenʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		//��ȡ�豸״̬
		nCnt = 0;
		while(1)
		{
			if((ret=NDK_DiskGetState(UDISK,&state))!=NDK_OK)
				;
			if(state==DISKMOUNTSUCC)
				break;
			sleep(1);
			if(++nCnt>=GETSTATETIMES)
			{
				cls_show_msg1(g_keeptime,"line %d:��%d��δ��ȡ��DISKMOUNTSUCC״̬", __LINE__, i);
				goto ERR;
			}
		}
		//���ļ�
		if ((fp=NDK_FsOpen(fname, "r"))<0)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�δ��������ļ�ʧ��(%d)", __LINE__, i, fp);
			continue;
		}
		//���ļ�
		if ((ret=NDK_FsRead(fp,readbuf, wrlen))!=wrlen||memcmp(writebuf, readbuf, wrlen))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�ζ��ļ�ʧ��(%d)", __LINE__, i, ret);
			continue;
		}		
		//ɾ���ļ��͹ر��豸
		NDK_FsClose(fp);
		NDK_FsDel(fname);
		//U�̹ر�
		if((ret=NDK_DiskClose(UDISK))!=NDK_OK)//�����umount
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��NDK_DiskCloseʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		succ++;
	}
	
	cls_show_msg("%s�������\n�ܴ���:%d\n�ɹ�:%d��", TESTITEM, i, succ);
	return;
}

void systest89(void)
{
	int nKeyin=0;

	cls_show_msg1(2, "%s������...", TESTITEM);
	while(1)
	{
		nKeyin=cls_show_msg("1.�������\n"
							"0.����ͷ����\n");
		switch(nKeyin)
		{
			case '1':
				camera_UDISK();
				break;
		    case '0':
				conf_camera();
				nConfInit=1;
				break;
			case ESC:
				return;
				break;
			default:
				continue;
				break;
		}
	}

	return;
}

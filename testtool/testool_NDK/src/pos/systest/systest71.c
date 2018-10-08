/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	 U��/3g���
* file name			: systest69.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: U��/3g���
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20131202  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"U��/3g���"
#define TESTFILE   "3gtest.file"
#define	BUFFERSIZE	(1024*5) //��������СЩ��������ƶ�ջ�����ƻ�����
#define GETSTATETIMES 30//��ȡ״̬����

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
* history 				: author			date			remarks
					 	jiangym			20140808	created
*****************************************************************/	
static void UDISK_WLM(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, j = 0, state = 0, nCnt = 0,succ = 0, timeout = SO_TIMEO, fp = 0, wrlen = 0 ,slen = 0, rlen = 0;
	char rootdir[][16] ={{0}}, fname[64] = {0}, writebuf[BUFFERSIZE] = {0}, readbuf[BUFFERSIZE] = {0};
	uint h_tcp = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;	

	/*process body*/		
	cls_show_msg_record(FILENAME,FUNCNAME,"��ȷ���Ѱ�װU��,���������");	
	//����ǰ��:�ȰѴ��ڵĲ����ļ���ֵ��fname
	//NDK_DiskGetRootDirName(UDISK, (char**)rootdir);//����ެ��ȷ�Ͽ���ֱ�ӵ��û�ȡ��Ŀ¼�������open
	//sprintf(fname, "%s/%s", rootdir[0], TESTFILE);//ɾ�������ļ��������ڲ����ļ��������������дУ���������ǰ�ڶ�д�������쳣�ϵ� 

	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	
	while(1)
	{
		//��������
ERR:
		TransDown(h_tcp);
		NetDown();
		NDK_FsClose(fp);
		NDK_FsDel(fname);
		NDK_DiskClose(UDISK);
		
		if(cls_show_msg1(2,"���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", i+1, TESTITEM, succ)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
		
		sleep(5);//ÿ�ιҶ�֮����������Ҫ�ȴ�5��,������оƬ��ѹ��
		
		if(NetUp()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��NetUpʧ��", __LINE__, i);
			continue;
		}
		
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
		
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��TransUpʧ��", __LINE__, i);
			continue;
		}
		
		//��ȡ�豸��Ŀ¼
		if((ret=NDK_DiskGetRootDirName(UDISK, (char **)rootdir))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�λ�ȡ��Ŀ¼ʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		sprintf(fname, "%s/%s", rootdir[0], TESTFILE);
		
		//���ļ�
		if((fp=NDK_FsOpen(fname, "w"))<0)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�δ��������ļ�ʧ��(%d)", __LINE__, i, fp);
			continue;
		}
		
		//���߷�������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�η���ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, slen, sendpacket.len);
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
		
		//���߽�������
		memset(rbuf, 0, sizeof(rbuf));
		if((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d�ν���ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, i, rlen, sendpacket.len);
			continue;
		}
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��У��ʧ��", __LINE__, i);
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
			
		//���߹Ҷ�
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:��%d��TransDownʧ��", __LINE__, i);
			continue;
		}
		//ɾ���ļ��͹ر��豸
		NDK_FsClose(fp);
		NDK_FsDel(fname);
		
		//����netdown
		if (NetDown()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:NetDownʧ��", __LINE__);
			continue;
		}	
		//U�̹ر�
		if((ret=NDK_DiskClose(UDISK))!=NDK_OK)//�����umount
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��NDK_DiskCloseʧ��(%d)", __LINE__, i, ret);
			continue;
		}
		succ++;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"%s�������:�ܴ���:%d �ɹ�:%d��", TESTITEM, i, succ);
	return;
}

void systest71(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		//��������
		if(conf_conn_WLM(TRUE)!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,WLM����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		g_CycleTime = 10;
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ��U��/3g�������
		UDISK_WLM();
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{
		keyin = cls_show_msg("1.U��/3g�������\n"
							 "0.��������");
		switch(keyin)
		{
		case '1':
			UDISK_WLM();
			break;
		case '0':
			switch(conf_conn_WLM(TRUE))
			{
			case SUCC:
				cls_show_msg1(2, "�������óɹ�!");
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
				//return;
				break;
			case NDK_ERR_QUIT://�û�����
			default:
				//return;
				break;
			}
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


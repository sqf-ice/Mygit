/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 
* Author 			: zhengry
* version			: V1.0
* DATE			: 2016/05/30
* directory 		: 
* description		: ����ͷ/WLM���
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  2016/05/30  zhengry
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"����ͷ/WLM���"
#define MAXTIME		60
/*----------global variables declaration("extern")--------------*/
static int nConfInit[2] = {0};	 /*�Ƿ����ù��˲���:0δ���� 1������*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
void camera_wlm(void)
{
	/*private & local definition*/
	int ret = 0, i = 0, succ = 0, timeout = SO_TIMEO, slen = 0, rlen = 0;
	uint h_tcp = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;

	/*����ǰ��*/
	if(nConfInit[0] == 0)
	{
		cls_show_msg1(g_keeptime, "Wlm����δ����");
		return;
	}
	if(nConfInit[1] == 0)
	{
		cls_show_msg1(g_keeptime, "Scan����δ����");	
		return;
	}
	
	/*process body*/	
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);

	while(1)
	{
		//��������
		TransDown(h_tcp);
		NetDown();
		
		if(cls_show_msg1(2,"���ڽ��е�%d��%s����(�ѳɹ�%d��),��ESC�˳�", i+1, TESTITEM, succ)==ESC)
			break;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;

		sleep(10);//ÿ�ιҶ�֮����������Ҫ�ȴ�10��,������оƬ��ѹ��

		if(NetUp()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:NetUpʧ��", __LINE__);
			continue;
		}	
		if((ret=NDK_ScanInit())!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
			goto ERR;
		}
	    NDK_ScanSet(CAMERA_SETTYPE_IMAGE, g_ShowImage);
		NDK_ScanSet(CAMERA_SETTYPE_SATURATION, g_Saturation);
		NDK_ScanSet(CAMERA_SETTYPE_SHARPNESS, g_Sharpness);
		NDK_ScanSet(CAMERA_SETTYPE_GAMMA, g_Contrast);

		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket.len);
			goto ERR;
		}
		if((ret=lib_doscan(g_ScanType, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sendpacket.len);
			goto ERR;
		}
		if((ret=lib_doscan(g_ScanType, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}

		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:У��ʧ��", __LINE__);
			goto ERR;
		}
		if((ret=lib_doscan(g_ScanType, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
			
		//�Ҷ�/
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:TransDownʧ��", __LINE__);
			goto ERR;
		}
		if(g_ScanType==1)
		{
			if((ret=NDK_ScanClose())!=NDK_OK)
			{
				cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
				goto ERR;
			}
		}
		if (NetDown()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:NetDownʧ��", __LINE__);
			continue;
		}

		succ++;
	ERR:
		if(g_ScanType==1)
			NDK_ScanClose();
	}
	
	cls_show_msg("%s�������\n�ܴ���:%d\n�ɹ�:%d��", TESTITEM, i, succ);
	return;
}

void systest86(void)
{
	int nKeyin=0;

	cls_show_msg1(2, "%s������...", TESTITEM);
	while(1)
	{
		nKeyin=cls_show_msg("1.�������\n"
							"9.��������\n"
							"0.����ͷ����\n");
		switch(nKeyin)
		{
			case '1':
				camera_wlm();
				break;
			case '9':
				switch(conf_conn_WLM(TRUE))
				{
				case SUCC:
					cls_show_msg1(2, "�������óɹ�!");
					nConfInit[0]=1;
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
		    case '0':
				conf_camera();
				nConfInit[1]=1;
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


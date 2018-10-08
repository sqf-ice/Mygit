/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��Ƶ/���߽������
* file name		: systest91.c
* Author 			: linwl
* version			: V1.0
* DATE			: 2017/03/21
* directory 		: 
* description		: ��Ƶ/���߽������
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  2017/03/21 linwl
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"��Ƶ/���߽���"

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
*			  		  linwl 		   	2017/03/17		created
*****************************************************************/
//��д����ע�⣺�������Ҫ������������Ƚ���!����,ҲҪ��һ����ǿ�ȡ�
static void wireless_voice(void)
{
	/*private & local definition*/
	int i = 0, succ = 0, timeout = SO_TIMEO, slen = 0, rlen = 0, ret = -1;
	uint h_tcp = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;
	
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
		if((ret=NDK_VoiceLoadFile(4, NULL)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��Ƶ����ʧ��ʧ��%d", __LINE__, ret);
			continue;
		}
		NDK_SysDelay(20);//���û�в���������,�Ϳ��ٰ�����ʼ��һ���������Ż�ʧ�ܷ���-320,����������ʱ
		
		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
			continue;
		}
		if((ret=NDK_VoiceLoadFile(0, NULL)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��Ƶ����ʧ��ʧ��%d", __LINE__, ret);
			continue;
		}
		NDK_SysDelay(20);//���û�в���������,�Ϳ��ٰ�����ʼ��һ���������Ż�ʧ�ܷ���-320,����������ʱ
		
		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket.len);
			continue;
		}
		if((ret=NDK_VoiceLoadFile(2, NULL)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��Ƶ����ʧ��ʧ��%d", __LINE__, ret);
			continue;
		}
		NDK_SysDelay(20);//���û�в���������,�Ϳ��ٰ�����ʼ��һ���������Ż�ʧ�ܷ���-320,����������ʱ
		
		//��������
		memset(rbuf, 0, sizeof(rbuf));
		if((rlen=sock_recv(h_tcp, rbuf, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, rlen, sendpacket.len);
			continue;
		}
		//�Ƚ��շ�
		if (MemCmp(sendpacket.header, rbuf, sendpacket.len))
		{
			cls_show_msg1(g_keeptime, "line %d:У��ʧ��", __LINE__);
			continue;
		}
		if((ret=NDK_VoiceLoadFile(4, NULL)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��Ƶ����ʧ��ʧ��%d", __LINE__, ret);
			continue;
		}
		NDK_SysDelay(20);//���û�в���������,�Ϳ��ٰ�����ʼ��һ���������Ż�ʧ�ܷ���-320,����������ʱ
			
		//�Ҷ�/
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:TransDownʧ��", __LINE__);
			continue;
		}
		if((ret=NDK_VoiceLoadFile(0, NULL)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��Ƶ����ʧ��ʧ��%d", __LINE__, ret);
			continue;
		}
		NDK_SysDelay(20);//���û�в���������,�Ϳ��ٰ�����ʼ��һ���������Ż�ʧ�ܷ���-320,����������ʱ
		
		if (NetDown()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:NetDownʧ��", __LINE__);
			continue;
		}
		if((ret=NDK_VoiceLoadFile(2, NULL)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:��Ƶ����ʧ��ʧ��%d", __LINE__, ret);
			continue;
		}
		NDK_SysDelay(20);//���û�в���������,�Ϳ��ٰ�����ʼ��һ���������Ż�ʧ�ܷ���-320,����������ʱ
		succ++;
	}
	
	cls_show_msg("%s�������\n�ܴ���:%d\n�ɹ�:%d��", TESTITEM, i, succ);
	return;
}

void systest91(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	while (1)
	{
		keyin = cls_show_msg("1.�������\n"
							"0.��������");
		switch(keyin)
		{
		case '1':
			wireless_voice();
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


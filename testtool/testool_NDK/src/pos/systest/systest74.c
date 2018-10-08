/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �ſ�/���߽������
* file name		: systest74.c
* Author 			: huangjianb
* version			: V1.0
* DATE			: 20141009
* directory 		: 
* description		: NFC/���߽������
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  2013/02/21 chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"NFC/���߽���"

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
*			  		  huangjianb 		20141009	created
*****************************************************************/
//��д����ע�⣺�������Ҫ������������Ƚ���!����,ҲҪ��һ����ǿ�ȡ�
static void nfc_wireless(void)
{
	/*private & local definition*/
	int i = 0, succ = 0, ret=-1, timeout = SO_TIMEO, slen = 0, rlen = 0 ,nSeekCnt = 3 ,pnRecvlen = 0;
	uint h_tcp = 0;
	char buf[PACKMAXLEN] = {0}, rbuf[PACKMAXLEN] = {0};
	Packet sendpacket;
	uchar psRecebuf[64] = {0};
	
	/*process body*/	
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);

	cls_show_msg("����ֻ�NFC���Թ��߲�����pos...,���������");
	
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
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, i, ret);
			continue;
		}

		if(TransUp(&h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:TransUpʧ��", __LINE__);
			continue;
		}
		//����NFC����֮����Ҫ�и�2S������ʱ�䣬�����ֻ�����������Ӧ
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, i, ret);
			continue;
		}

		//��������
		if((slen=sock_send(h_tcp, sendpacket.header, sendpacket.len, timeout))!=sendpacket.len)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(ʵ��%d, Ԥ��%d)", __LINE__, slen, sendpacket.len);
			continue;
		}
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, i, ret);
			continue;
		}

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
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, i, ret);
			continue;
		}

		//�Ҷ�/
		if (TransDown(h_tcp)!=SUCC)
		{
			cls_show_msg1(g_keeptime,"line %d:TransDownʧ��", __LINE__);
			continue;
		}
		if (NetDown()!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:NetDownʧ��", __LINE__);
			continue;
		}
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����(��%d��)ʧ��(%d)", __LINE__, i, ret);
			continue;
		}

		succ++;
	}
	
	cls_show_msg("%s�������\n�ܴ���:%d\n�ɹ�:%d��", TESTITEM, i, succ);
	return;
}

void systest74(void)
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
			nfc_wireless();
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


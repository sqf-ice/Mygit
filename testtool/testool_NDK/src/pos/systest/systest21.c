/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name		: systest21.c
* Author 			: chenjs
* version			: 
* DATE			: 20130227
* directory 	: 
* description	: ����/MDM�������
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM		"����/MDM����"

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
*			  		chenjs		   20130227	 		created
*****************************************************************/

static void wlm_mdm_press(LinkType *lnktype)
{
	/*private & local definition*/
	int i = 0, succ1 = 0, succ2 = 0, dialtype = 0,cnt = 0;
	Packet sendpacket;
	char buf[PACKMAXLEN] = {0};
	//float call_time = 0.0 , hangdowntime = 0.0;
	float dial = 0.0, hang = 0.0;
	
	/*process body*/
	if(cls_show_msg1(5, "ѡ�񲦺�ѹ������:\n[ȷ��]��ͨ��\n[����]��ǿ��")==ENTER)
		dialtype = 1;//��������
	else
		dialtype = 0;//�쳣����	
	
	//��ʼ��������������MDM����
	init_snd_packet(&sendpacket, buf);
	set_snd_packet(&sendpacket);
	mdm_clrportbuf_all();

	if(sendpacket.forever==FALSE)  //������ѭ�����ε��õ�update_snd_packet���ʵ�ʴ�������
	{
		cnt = sendpacket.lifecycle;
		sendpacket.lifecycle = 2*cnt;
	}
	//����ͨѶ
	while(1)
	{
		if(cls_show_msg1(3, "��%d��%s������,ESC�˳�...", i+1, TESTITEM)==ESC)
			break;

		//��MDM���͵İ���ʽ����Ϊ����ͨѶ��ʽ�µķ���
		setlinktype(lnktype[0]);
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;
		i++;
		setlinktype(lnktype[1]);

		//����ͨѶ
		//ShowSignalQuality(-1, -1);//��������(AT����)����ǰ,�ȹ��ź���ʾ
		//cls_show_msg1(3, "���߶�����ͨѶ��...��ǰ�ź���:%d", getsqwirelessmodem());
		//ShowSignalQuality(0, 0);//�˳�����(AT����)���Ժ�,Ҫ�ָ��ź�ˢ��

		if(lnktype[0]==ASYN)//linwl20130510�����첽PPP������PPP���û��������벻һ���������ٲ���ǰ��Ҫ������ĳ���Ӧ������
		{
			PppSetUsrCfg();//��Ҫ�ú������¸��ߵײ�PPP�����ṹ
			strcpy(PPPUSR,DEFAULT_PPP_USR);
			strcpy(PPPPWD,DEFAULT_PPP_PWD);
		}
		if(wireless_dial_comm(&sendpacket, RESET_NO)==SUCC)
			succ1++;
		
		//����MDMͨѶ
		if(lnktype[0]==ASYN)//linwl20130510�����첽PPP������PPP���û��������벻һ���������ٲ���ǰ��Ҫ������ĳ���Ӧ������
			PppSetUsrCfg();
		setlinktype(lnktype[0]);//linktype = SYNC;
		if(update_snd_packet(&sendpacket)!=SUCC)
			break;		
		if(mdm_dial_comm(MDMDialStr, &sendpacket, FALSE, &dial, &hang)==SUCC)
		{
			succ2++;
		}

		if(dialtype==1)//��������ʱ��Ҫ�ȴ�
			cls_show_msg1(rand()%15+20, "ģ����Ϣ��,�����������Ϣ����ѹ��...");
	}

	cls_show_msg_record(FILENAME ,FUNCNAME ,"�������%d��.���߳ɹ�%d��,MDM�ɹ�%d��.", i, succ1, succ2);

	return;
}

void systest21(void)
{
	/*private & local definition*/
	int keyin = 0;
	LinkType type[2] = {NONE, NONE};
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_MDM()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,MDM����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		type[0] = getlinktype();
		
		if(conf_conn_WLM(TRUE)!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,WLM����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		type[1] = getlinktype();
		g_CycleTime = 10;
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н���ѹ������
		wlm_mdm_press(type);
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		keyin = cls_show_msg("1.����ѹ��\n"
							"0.MDM����\n"
							"9.��������");
		switch(keyin)
		{
		case '1':
			wlm_mdm_press(type);
			break;
		case '0':			
			switch(conf_conn_MDM())
			{
			case SUCC:
				type[0] = getlinktype();
				cls_show_msg1(2, "MDM���óɹ�!");
				break;
			case FAIL:
				cls_show_msg("line %d:����modemʧ��!!!\n������˳�����...", __LINE__);	
				break;
			case NDK_ERR_QUIT://�û�����
			default:
				continue;
				break;
			}
			break;
		case '9':
			switch(conf_conn_WLM(TRUE))
			{
			case SUCC:
				type[1] = getlinktype();
				cls_show_msg1(2, "�������óɹ�!");
				break;
			case FAIL:
				cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
				break;
			case NDK_ERR_QUIT://�û�����
			default:
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


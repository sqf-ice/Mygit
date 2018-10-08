/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	����/ȡ��ص���
* file name			: systest58.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: ����/ȡ��ص�������
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
#define	TESTITEM	"����/ȡ��ص���"
#define MAXWAITTIME		10
#define	DEFAULT_CNT_STR	"10"
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)
//#define MAX_SIZE BUFSIZE_BT//1024

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
					 	jiangym			20140228	created
*****************************************************************/	
static void bt_getpowervol(void)
{
	int ret = 0, bak = 0,cnt = 0, succ = 0, recvLen = 0, j = 0, cmdlen =0;
	uint unLen = 0;
	char sendBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0}, recvBuf[BUFSIZE_BT-MIN_BTCMD_LEN] = {0};//ԭʼ����
	char cmdBuf[BUFSIZE_BT] = {0};// cmdrecvBuf[BUFSIZE_BT] = {0}, ;//���뱨��ͷ�������
	char  CntStr[8] = DEFAULT_CNT_STR;
	unsigned int unVol = 300;

	if(g_SequencePressFlag) 
	{
		bak = cnt = GetCycleValue();  //����ѹ�����ԣ���Ҫ�ֹ�����ѭ�������Ķ�ֱ������Ĭ��ֵ
	}
	else
	{
		cls_printf("Ĭ�Ͻ�����Դ���:\n");
		if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
			bak = cnt = DEFAULT_CNT_VLE;
		else
			bak = cnt = atoi(CntStr);
	}
	cls_show_msg1(g_keeptime,"�ֳ��豸��ȷ��ֻͨ����ع���,��������ӵ�Դ,û�����Ӵ�����USB�ߵ�,���������...");
	if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK )
	{
		cls_show_msg1(g_keeptime, "line %d:%sʧ��(ret=%d,vol=%d)", __LINE__, TESTITEM, ret, unVol);
		return;
	}
	for(j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	NDK_PortClose(PORT_NUM_BT);
	while(cnt)
	{
		if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��)(��ص���:%d),��ESC�˳�", TESTITEM, cnt, succ, unVol&0x7fffffff))//	value & 0x7fffffff
			break;	
		cnt--;
		//����������
		if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�δ���������ʧ��", __LINE__, bak-cnt);
			NDK_PortClose(PORT_NUM_BT);
			continue;
		}
		if(g_btdiscoverflag)
		{
			if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
			{
				cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:���������㲥ʧ��(%d)", __LINE__, ret);
				continue;
			}
		}
		//ȡ��ص���
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK )
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(ret=%d,vol=%d)", __LINE__, bak-cnt, TESTITEM, ret, unVol);
			NDK_PortClose(PORT_NUM_BT);
			continue;
		}
		if((ret=lib_bt_pairing(1))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME, FUNCNAME, g_keeptime, "line %d:��%d���������ʧ��(%d)", __LINE__, bak-cnt, ret);
			NDK_BTDisconnect();
			NDK_PortClose(PORT_NUM_BT);
			continue;
		}
#if BTMFI_ENABLE
		cls_show_msg("�ֻ����������г�ʼ������pos�����������");
#endif
		//�建����
		if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
		{				
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d���建����ʧ��(%d)", __LINE__, bak-cnt, ret);
			goto DISCONNECT;
		}
		//ȡ��ص���
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK )
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(ret=%d,vol=%d)", __LINE__, bak-cnt, TESTITEM, ret, unVol);
			goto DISCONNECT;
		}
		
		//cls_printf("�������ݰ���...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		bt_cmd_packet(0x05, sendBuf, sizeof(sendBuf), cmdBuf);
		cmdlen = sizeof(sendBuf)+MIN_BTCMD_LEN;
		if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, bak-cnt, ret);
			goto DISCONNECT;
		}
		//ȡ��ص���
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK )
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(ret=%d,vol=%d)", __LINE__, bak-cnt, TESTITEM, ret, unVol);
			goto DISCONNECT;
		}
		//cls_printf("�������ݰ���...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		if(((ret=NDK_PortRead(PORT_NUM_BT,cmdlen, cmdBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ν��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, bak-cnt, ret, recvLen, cmdlen);
			bt_comm_once(12,0);
			goto DISCONNECT;
		}
		memset(recvBuf, 0, sizeof(recvBuf));
		bt_cmd_uppacket(cmdBuf, sizeof(sendBuf), recvBuf);
		if(memcmp(sendBuf, recvBuf, sizeof(sendBuf)))
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d������У��ʧ��", __LINE__, bak-cnt);
			goto DISCONNECT;
		}
		//ȡ��ص���
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK )
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(ret=%d,vol=%d)", __LINE__, bak-cnt, TESTITEM, ret, unVol);
			goto DISCONNECT;
		}
		//������Ӧȷ�ϱ���
		cls_printf("������Ӧ���ݱ�����...");
		memset(cmdBuf, 0, sizeof(cmdBuf));
		bt_cmd_packet(0x06, NULL, 0, cmdBuf);
		if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdBuf))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�η���ʧ��(%d)", __LINE__, bak-cnt, ret);
			goto DISCONNECT;
		}
		succ++;
		
DISCONNECT:
		//�Ͽ���������(�Ͷ���Ҫ�˺������Ͽ�����,�ж˵��ò�Ӱ��)
		cls_show_msg1(1, "POS�˹Ҷ�����������...");
		if((ret=NDK_BTDisconnect())!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ζϿ���������ʧ��", __LINE__, bak-cnt);
			NDK_PortClose(PORT_NUM_BT);
			continue;
		}
		//�ر���������
		if((ret=NDK_PortClose(PORT_NUM_BT))!=NDK_OK)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d�ιر���������ʧ��", __LINE__, bak-cnt);
			continue;
		}
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"%s���,��ִ�д���Ϊ%d,�ɹ�%d��", TESTITEM, bak-cnt, succ);
	return;
}

void systest58(void)
{
	/*private & local definition*/
	int ret = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_conn_BT()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,BT����ʧ��,���������ļ�",TESTITEM);
			return;
		}
		g_CycleTime = 10;
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);	
		//ִ�н������
		bt_getpowervol();
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.��������");
		
		switch(ret)
		{
		case '1':
			bt_getpowervol();
			break;
		case '0':
			conf_conn_BT();
#if BTMFI_ENABLE
			NDK_BTSetBleStatus(0);//��MFIЭ���£��ر�BLE��ʽ
#endif
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



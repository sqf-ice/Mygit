/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt8.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_PortRead�ܷ��ȡ��������,NDK_PortWrite�ܷ�����������������
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define     TESTAPI		"������д"
#define     MAXWAITTIME		10
#define		MAXTIMEOUT	10//��ʱ����ʱ��
#define		WUCHASEC	0.03 //���ֵ����
#define		MAX_SIZE	BUFSIZE_BT-MIN_BTCMD_LEN
#define 	SNDCNT			(71-MIN_BTCMD_LEN)
#define 	SNDCNT1			(128-MIN_BTCMD_LEN)


/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt8
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20140217		created
*****************************************************************/
void bt8(void)
{
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = -1, j = 0, cmdlen = 0, recvLen = 0;
	float tmp = 0.0, unTime = 0.0;
	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};
	char cmdrecvBuf[BUFSIZE_BT] = {0}, cmdsendBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
#if K21_ENABLE
	char str[32] = {0};
#endif

	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;

#if !K21_ENABLE
	//case1:��������δ�򿪽��ж�д����,Ӧ������Ӧ����ֵ
	if((ret = NDK_PortRead(PORT_NUM_BT, sizeof(recvBuf), recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_ERR_OPEN_DEV)
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, recvLen);
		GOTOERR;
	}
	if((ret = NDK_PortWrite(PORT_NUM_BT, sizeof(sendBuf), sendBuf)) != NDK_ERR_OPEN_DEV)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	
	//����ǰ��,�򿪴���
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ME30MH������Ҫ�����㲥
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case2:Ӧ��֤�ڸߡ��Ͳ������£������շ�������(ָ�������ڻ�������С��һ���Ϊ4K)����
	//�����ݶ�ȡ(�Ͷ�2K-1���ն�4K)	
#if BTMFI_ENABLE
	cls_show_msg("�ֻ����������Ӻ��ʼ��������pos�����������");
#else
	cls_show_msg1(3,"����%dB���ݶ�д",MAX_SIZE);
#endif
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendBuf, MAX_SIZE, cmdsendBuf);
	cmdlen=MAX_SIZE+MIN_BTCMD_LEN;//���ݰ����º��ĳ���=���ݳ���+��С���ĳ���
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
	{
		send_result("line %d:%s���Խ��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, TESTAPI, ret, recvLen, cmdlen);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	bt_cmd_uppacket(cmdrecvBuf, MAX_SIZE, recvBuf);
	if(memcmp(sendBuf, recvBuf, MAX_SIZE))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:Ӧ������������дָ�����ȵ�����:�����շ�����8�ֽ�����
	cls_show_msg1(3,"���з���8�ֽ����ݶ�д");
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendBuf, SNDCNT, cmdsendBuf);//��sendBuf��ǰSNDCNT�ֽ���Ϊ��������
	cmdlen=SNDCNT+MIN_BTCMD_LEN;
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
	{
		send_result("line %d:%s���Խ��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, TESTAPI, ret, recvLen, cmdlen);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	bt_cmd_uppacket(cmdrecvBuf, SNDCNT, recvBuf);
	if(memcmp(sendBuf, recvBuf, SNDCNT))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:����64���������ֲ�����4K������
	cls_show_msg1(3,"����64���������ֲ�����4K�����ݶ�д");
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendBuf, SNDCNT1, cmdsendBuf);//��sendBuf��ǰSNDCNT1�ֽ���Ϊ��������
	cmdlen=SNDCNT1+MIN_BTCMD_LEN;
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
	{
		send_result("line %d:%s���Խ��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, TESTAPI, ret, recvLen, cmdlen);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	bt_cmd_uppacket(cmdrecvBuf, SNDCNT1, recvBuf);
	if(memcmp(sendBuf, recvBuf, SNDCNT1))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:����ʱ����:Ӧ�÷��س�ʱ,���ҳ�ʱʱ������Χ��
	cls_show_msg1(1,"����ʱ������");
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !(defined ME15CHX||defined ME15B)
	lib_StartStopwatch();
	unTime = 0;
#else
	unTime = lib_ReadStopwatch();
#endif
#if defined ME30GX||defined ME32GX
	if( ((ret=NDK_PortRead(PORT_NUM_BT, sizeof(recvBuf), recvBuf, MAXTIMEOUT*1000, &recvLen))!=NDK_ERR_TIMEOUT) || (tmp=abs(INT_ReadStopwatch()-unTime-MAXTIMEOUT))>WUCHASEC)	/*��ȡ��������*/
#else
	if( ((ret=NDK_PortRead(PORT_NUM_BT, sizeof(recvBuf), recvBuf, MAXTIMEOUT*1000, &recvLen))!=NDK_ERR_TIMEOUT) || (tmp=fabs(lib_ReadStopwatch()-unTime-MAXTIMEOUT))>WUCHASEC)	/*��ȡ��������*/
#endif
	{
#if K21_ENABLE
		memset(str,0,sizeof(str));
		ftos(tmp,str);
		send_result("line %d:%s����ʧ��(%d,%ss)", __LINE__, TESTAPI, ret, str);
#else
		send_result("line %d:%s����ʧ��(%d,%f)", __LINE__, TESTAPI, ret, tmp);
#endif
		GOTOERR;
	}
	
	//case6:���Զ����������Ϲرմ��ڣ���Ӧ�ó����쳣(�ܷɻ�������)
	cls_show_msg1(1,"���Զ���رմ�����");
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendBuf, MAX_SIZE, cmdsendBuf);
	cmdlen=MAX_SIZE+MIN_BTCMD_LEN;
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
	{
		send_result("line %d:%s���Խ��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, TESTAPI, ret, recvLen, cmdlen);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	bt_cmd_uppacket(cmdrecvBuf, MAX_SIZE, recvBuf);
	if(memcmp(sendBuf, recvBuf, MAX_SIZE))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(1);//��ʱ1��,ȷ���ֻ��յ�06ָ��
	if((ret = NDK_PortClose(PORT_NUM_BT)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case8:���ṩ���������ڹرչ��ܣ����ڶ�(case5)��д�������ݺ󣨲������������Ƿ����������Ƿ���������Ҫ�ȴ���д��ɣ����Զ�д��������Ϊ׼�������������������ڹرչ��ܣ���ʱ�п��ܶ�дδʵ����ɣ�����Ӧ�г����ܷɻ��������쳣
	//����д���������Ϲرմ��ڣ���Ӧ�ó����쳣(�ܷɻ�������)
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if !K21_ENABLE  //K21���Բ���Ҫ�˶���֮ǰ�����ӻ��������ܽ���������д����
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	cls_show_msg1(3,"����д��رմ�����");
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendBuf, MAX_SIZE, cmdsendBuf);
	cmdlen=MAX_SIZE+MIN_BTCMD_LEN;
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(3);//��ʱ1��,ȷ���ֻ��յ�06ָ��
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_PortClose(PORT_NUM_BT)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case9:���´��������ڽ��������շ�Ӧ�óɹ�
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ME30MH������Ҫ�����㲥
	if(g_btdiscoverflag)
	{
		if((ret=NDK_BTSetDiscoverableStatus(1))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if BTMFI_ENABLE
	cls_show_msg("�ֻ����������Ӻ��ʼ��������pos�����������");
#else
	cls_show_msg1(3,"�������´��������ڽ��������շ���");
#endif
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendBuf, MAX_SIZE, cmdsendBuf);
	cmdlen=MAX_SIZE+MIN_BTCMD_LEN;
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_OK)||(recvLen != cmdlen))
	{
		send_result("line %d:%s���Խ��մ�(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, TESTAPI, ret, recvLen, cmdlen);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	bt_cmd_uppacket(cmdrecvBuf, MAX_SIZE, recvBuf);
	if(memcmp(sendBuf, recvBuf, MAX_SIZE))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(1);
	if((ret = NDK_PortClose(PORT_NUM_BT)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case7:�ر��������ں󣬶�д��Ӧ�óɹ�
	if((ret = NDK_PortRead(PORT_NUM_BT, sizeof(recvBuf), recvBuf, MAXWAITTIME*1000, &recvLen)) != NDK_ERR_OPEN_DEV)
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, recvLen);
		GOTOERR;
	}
	if((ret = NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf)) != NDK_ERR_OPEN_DEV)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}


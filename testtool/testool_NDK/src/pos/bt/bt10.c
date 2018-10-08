/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt10.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_PortReadLen�ܷ���ȷ�������������ֽ�
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"������ȡ������"
#define   MAXWAITTIME	 10
#define		MAX_SIZE	 BUFSIZE_BT-MIN_BTCMD_LEN

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt10
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20140217		created
*****************************************************************/
void bt10(void)
{
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = -1, ret1 = -1,len = 0,len1 = 0, readlen = 0, j = 0, cmdlen = 0;
	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};
	char cmdrecvBuf[BUFSIZE_BT+10] = {0}, cmdsendBuf[BUFSIZE_BT] = {0};//���뱨��ͷ�������
	//��оME30���Թ�����cmdrecvBuf���������ڰ�׿���������෵��9�ֽ�����,����Ҫ���ӻ�������С,������20170801
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;

#if !K21_ENABLE
	//case1:��δ���Ĵ��ڽ���ȡ�������ֽ�������,Ӧ������Ӧ����ֵ
	if((ret = NDK_PortReadLen(PORT_NUM_BT,&len)) != NDK_ERR_OPEN_DEV)
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}
#endif
	
	//case2:�򿪴���,����Ƿ������Ƿ�����
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_PortReadLen(-1,&len)) != NDK_ERR_PARA)||((ret1=NDK_PortReadLen(PORT_NUM_BT,NULL)) != NDK_ERR_PARA))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
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
	//case2:���������ܹ���ȡ����ȷ�Ļ��������ݳ���
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if BTMFI_ENABLE
	cls_show_msg("�ֻ����������г�ʼ������pos�����������");
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
#if K21_ENABLE
	sleep(5);//�����㹻��ʱ��Ȼ������̨�ͻ����ݵ�POS���ջ�����
#endif
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if((ret = NDK_PortReadLen(PORT_NUM_BT,&len)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}
	if(len != cmdlen)//��һ�ζ����ĳ��Ȳ�Ϊ4096��������
	{
		while(1)//����������Ӧ��ȷ�������ں�ԭ��NDK_PortReadLen�������ֻ�ܷ���4095,�����������ѭ���ĵ��øú���ͨ����ε��õĳ��Ⱥ����ж������Ƿ�
		{
			sleep(1);//�����ֻ���IOS���������ٶȱȽ��������������������0��������ʱ
			if((ret = NDK_PortReadLen(PORT_NUM_BT,&len1)) != NDK_OK)
			{				
				send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len1);
				goto ERR;
			}
			if(len1==0)
			{
				if(readlen!=cmdlen)//��ε��ú�ĳ��Ⱥ�Ӧ�õ���Ԥ��ֵ ##����������Ԥ��ֵ��ʵ��ֵ��9�ֽڵĻ����Ѷ�λΪ��׿apk������20170523 added
				{
					send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, readlen, sizeof(cmdsendBuf));
					goto ERR;		
				}
				break;
			}
			readlen+=len1;//NDK_PortReadLenȡ���ĳ��ȱ����NDK_PortReadʵ�ʶ�����һ��
			memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
			if(((ret=NDK_PortRead(PORT_NUM_BT, len1, cmdrecvBuf, MAXWAITTIME*1000, &len)) != NDK_OK) || (len!=len1))
			{
				send_result("line %d:%s����ʧ��(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, TESTAPI, ret, len, len1);
				goto ERR;
			}
		}
	}
	
	//case4:�建����ڵ��û�ȡ���Ļ��������ݳ���Ӧ��Ϊ0
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendBuf, MAX_SIZE, cmdsendBuf);
	cmdlen=MAX_SIZE+MIN_BTCMD_LEN;
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(1);
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	sleep(5);//�����㹻��ʱ��Ȼ������̨�ͻ����ݵ�POS���ջ�����
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_PortReadLen(PORT_NUM_BT,&len)) != NDK_OK||len != 0)
	{				
 		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
 		GOTOERR;
	}
	
	//case5:�������ݽ��պ󣬽��ջ���������0
	sleep(5);
	if((ret = NDK_PortClrBuf(PORT_NUM_BT)) != NDK_OK)
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x05, sendBuf, sizeof(sendBuf), cmdsendBuf);
	cmdlen = sizeof(sendBuf) + MIN_BTCMD_LEN;
	if((ret=NDK_PortWrite(PORT_NUM_BT, cmdlen, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(cmdrecvBuf, 0, sizeof(cmdrecvBuf));
	if(((ret=NDK_PortRead(PORT_NUM_BT, cmdlen, cmdrecvBuf, MAXWAITTIME*1000, &len)) != NDK_OK)||(len != cmdlen))
	{
		send_result("line %d:%s����ʧ��(ret=%d)(ʵ��%d, Ԥ��%d)", __LINE__, TESTAPI, ret, len, cmdlen);
		GOTOERR;
	}
	bt_cmd_uppacket(cmdrecvBuf, sizeof(sendBuf), recvBuf);
	if(memcmp(sendBuf, recvBuf, sizeof(sendBuf)))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret = NDK_PortReadLen(PORT_NUM_BT,&len)) != NDK_OK||len != 0)
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}
	memset(cmdsendBuf, 0, sizeof(cmdsendBuf));
	bt_cmd_packet(0x06, NULL, 0, cmdsendBuf);
	if((ret=NDK_PortWrite(PORT_NUM_BT, MIN_BTCMD_LEN, cmdsendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}


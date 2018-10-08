/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: port4.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_PortClrBuf�ӿڹ���
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_PortClrBuf"
#define		MAX_SIZE	BUFSIZE_SERIAL
#define		MAXWAITTIME 150//���յȴ�ʱ��
#define		MAXTIMEOUT	3//��ʱ����ʱ��

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
*			  						chensl		   20130105	 		created
*****************************************************************/
void port4(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, ret1 = -1, port = -1, len = 0, j = 0;
	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};
	char *pszAttr;
	int bpsid=0;
	
	//����Ϊ������
	//����ǰ��:�رմ���
	//ѡ����ԵĴ���(����1�򴮿�2),������
	cls_show_msg1(2, "%s������...", TESTAPI);
	port = select_serial();
	pszAttr = conf_serial(&bpsid);
	NDK_PortClose(port);
	
	//case1:��δ���Ĵ��ڽ����建�����,Ӧ������Ӧ����ֵ
	if((ret = NDK_PortClrBuf(port)) != NDK_ERR_OPEN_DEV)	/*����ջ���*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}
	
	//case2:�򿪴���,����Ƿ�����,Ӧ���ض�Ӧ�Ĵ���ֵ
	cls_show_msg1(MAXWAITTIME, "�̽���ѡ���ڵ�23��,���������...");
	if((ret=NDK_PortOpen(port, pszAttr)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_PortClrBuf(-1)) != NDK_ERR_PARA)
		||((ret1=NDK_PortClrBuf(100/*9*/)) != NDK_ERR_PARA))//linwl20131014 ����������һ��ɨ��ͷPORT_NUM_SCAN 9 ��������ʧ�ܣ��ʽ���������ֱֵ�ӸĴ��100
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		GOTOERR;
	}
	
	//case3:����ջ���֮����ȥ������,Ӧ������������
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	if((ret = NDK_PortWrite(port, MAX_SIZE, sendBuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysDelay(MAXWAITTIME/(bpsid+1)*10);	/*��������·����Ҫʱ��,�ӳ�һ��,�ò������ݽ�����ջ�����*/
	if((ret = NDK_PortClrBuf(port)) != NDK_OK)	/*����ջ���*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	if((ret = NDK_PortRead(port, MAX_SIZE, recvBuf, MAXTIMEOUT*1000, &len)) != NDK_ERR_TIMEOUT)	/*��ȡ��������*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}

	//case4:����ʹ�ò���,�建���У���д��׼ȷ��
	if((ret = NDK_PortClrBuf(port)) != NDK_OK)	/*����ջ���*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_PortWrite(port, MAX_SIZE, sendBuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	if((ret = NDK_PortRead(port, MAX_SIZE, recvBuf, MAXWAITTIME/(bpsid+1)*1000, &len)) != NDK_OK)	/*��ȡ��������*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}
	if((len != sizeof(sendBuf)) || memcmp(sendBuf, recvBuf, len))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len, sizeof(sendBuf));
		goto ERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	if (g_com!=INVALID_COM)
	{
		if ((ret=NDK_PortOpen(g_com, "115200,8,N,1")) != NDK_OK)
		{
			//��ʼ��ʧ����,Ҳ����send_result("line %d: ��ʼ�����ڴ�", __LINE__);
			cls_show_msg("line %d: ��ʼ�����ڴ�", __LINE__);
			return;
		}
	}
	return;
}

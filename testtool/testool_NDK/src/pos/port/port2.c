/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: port2.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_PortRead�ӿڹ���
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
#define		TESTAPI			"NDK_PortRead"
#define		MAXTIMEOUT	10//��ʱ����ʱ��
#define		MAXWAITTIME 150//���յȴ�ʱ��
#define		WUCHASEC	0.03 //���ֵ����
#define		MAX_SIZE	BUFSIZE_SERIAL


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
void port2(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, ret1 = -1, ret2 = -1, ret3 = -1, ret4 = -1, ret5 = -1, port = -1, len = 0, j = 0;
	float tmp = 0.0;
	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0}, *pszAttr;
#if K21_ENABLE
	char str[32] = {0};
#endif
	int bpsid=0;
	
	//����Ϊ������
	//����ǰ��:�رմ���
	//ѡ����ԵĴ���(����1�򴮿�2),������
	cls_show_msg1(2, "%s������...", TESTAPI);
	port = select_serial();
	pszAttr = conf_serial(&bpsid);
	NDK_PortClose(port);

	//case1:��δ���Ĵ��ڽ��ж�����,Ӧ������Ӧ����ֵ
	if((ret = NDK_PortRead(port, sizeof(recvBuf), recvBuf, MAXWAITTIME/(bpsid+1)*1000, &len)) != NDK_ERR_OPEN_DEV)	/*��ȡ��������*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}
	
	//case2:�򿪴���,����Ƿ�����NULL,�Ƿ�����,�Ƿ�����,�Ƿ���ʱʱ��
	cls_show_msg1(MAXWAITTIME, "�̽���ѡ���ڵ�23��,���������...");
	if((ret=NDK_PortOpen(port, pszAttr)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_PortClrBuf(port)) != NDK_OK)	/*����ջ���*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	if(((ret=NDK_PortRead(-1, sizeof(recvBuf), recvBuf, 0, &len)) != NDK_ERR_PARA)
		||((ret1=NDK_PortRead(port, sizeof(recvBuf), recvBuf, 0, NULL)) != NDK_ERR_PARA)
		||((ret2=NDK_PortRead(port, sizeof(recvBuf), NULL, 0, &len)) != NDK_ERR_PARA)
		||((ret3=NDK_PortRead(port, sizeof(recvBuf), recvBuf, -1, &len)) != NDK_ERR_PARA)
		||((ret4=NDK_PortRead(port, BUFSIZE_SERIAL+1/*4097*/, recvBuf, 0, &len)) != NDK_ERR_PARA)
		||((ret5=NDK_PortRead(port, 0, recvBuf, 0, &len)) != NDK_ERR_PARA))
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d,%d,%d,%d)", __LINE__, TESTAPI, ret, ret1, ret2, ret3, ret4, ret5);
		GOTOERR;
	}
	
	//case3:�����ݶ�ȡ(4K)	
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
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
		GOTOERR;
	}

	//case7:������������unLen>pnReadLen�����portread�����ķ���ֵ��ʵ���յ�������ֵ 20150819linwl,�˴��еͶ˴��ڲ���,�Ͷ˷���-10�ж˷���0
	if((ret = NDK_PortWrite(port, 1024, sendBuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	if((ret = NDK_PortRead(port, 2048, recvBuf, MAXWAITTIME/(bpsid+1)*1000, &len)) != NDK_OK)	/*��ȡ��������*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}
	if((len != 1024) || memcmp(sendBuf, recvBuf, len))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, len);
		GOTOERR;
	}
	
	//case4:����ʱ����
	if((ret = NDK_PortClrBuf(port)) != NDK_OK)	/*����ջ���*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	lib_StartStopwatch();
	if( ((ret=NDK_PortRead(port, sizeof(recvBuf), recvBuf, MAXTIMEOUT*1000, &len))!=NDK_ERR_TIMEOUT) || (tmp=fabs(lib_ReadStopwatch()-MAXTIMEOUT))>WUCHASEC)	/*��ȡ��������*/
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

	//case6:���ڽ������Ƴ���Ϊ4K(4K����,case2�Ѳ�)
	//case5:���Զ����������Ϲرմ��ڣ���Ӧ�ó����쳣(�ܷɻ�������)
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
	if((ret = NDK_PortClose(port)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
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

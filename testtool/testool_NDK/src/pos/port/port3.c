/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: port3.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_PortWrite�ӿڹ���
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
#define 	TESTAPI		"NDK_PortWrite"
#define		MAX_SIZE	5678
#define		MAXWAITTIME 150//���յȴ�ʱ��


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
void port3(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, ret1 = -1, ret2 = -1, port = -1, len1 = 0, len2 = 0, j = 0;
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
	
	//case1:��δ���Ĵ��ڽ���д����,Ӧ������Ӧ����ֵ
	if((ret = NDK_PortWrite(port, sizeof(sendBuf), sendBuf)) != NDK_ERR_OPEN_DEV)	/*��ȡ��������*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:�򿪴���,����Ƿ�����NULL,�Ƿ�����,�Ƿ�����
	cls_show_msg1(MAXWAITTIME, "�̽���ѡ���ڵ�23��,���������...");
	if((ret=NDK_PortOpen(port, pszAttr)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_PortWrite(-1, sizeof(sendBuf), sendBuf)) != NDK_ERR_PARA)
		||((ret1=NDK_PortWrite(port, 0, sendBuf)) != NDK_ERR_PARA)		
		||((ret2=NDK_PortWrite(port, sizeof(sendBuf), NULL)) != NDK_ERR_PARA))
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, ret1, ret2);
		GOTOERR;
	}
	
	//case3:��ͨ���ݰ����ͽ���Ҫ�Ƚ�һ��(����4K)
	if((ret = NDK_PortClrBuf(port)) != NDK_OK)	/*����ջ���*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	if((ret = NDK_PortWrite(port, MAX_SIZE, sendBuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	if((ret = NDK_PortRead(port, 4096, recvBuf, MAXWAITTIME/(bpsid+1)*1000, &len1)) != NDK_OK)	/*��ȡ��������*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len1);
		GOTOERR;
	}
	if((ret = NDK_PortRead(port, MAX_SIZE-4096, recvBuf+4096, MAXWAITTIME/(bpsid+1)*1000, &len2)) != NDK_OK)	/*�����ζ�ȡ��������*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len2);
		GOTOERR;
	}
	if(((len1+len2) != sizeof(sendBuf)) || memcmp(sendBuf, recvBuf, sizeof(sendBuf)))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len1+len2, sizeof(sendBuf));
		GOTOERR;
	}

	//case4:����д���������Ϲرմ��ڣ���Ӧ�ó����쳣(�ܷɻ�������)
	if((ret = NDK_PortWrite(port, MAX_SIZE, sendBuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_PortClose(port)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:�Ҷ�֮�����´򿪴���Ӧ���ܹ����������շ��������ϴ��쳣�Ҷ�Ӱ��
	if((ret=NDK_PortOpen(port, pszAttr)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for (j=0; j<sizeof(sendBuf); j++)
		sendBuf[j]=rand()%256;
	if((ret = NDK_PortWrite(port, MAX_SIZE, sendBuf)) != NDK_OK)	/*�����ڷ��Ͳ�������*/
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(recvBuf,0,sizeof(recvBuf));
	if((ret = NDK_PortRead(port, 4096, recvBuf, MAXWAITTIME/(bpsid+1)*1000, &len1)) != NDK_OK)	/*��ȡ��������*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len1);
		GOTOERR;
	}
	if((ret = NDK_PortRead(port, MAX_SIZE-4096, recvBuf+4096, MAXWAITTIME/(bpsid+1)*1000, &len2)) != NDK_OK)	/*�����ζ�ȡ��������*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len2);
		GOTOERR;
	}
	if(((len1+len2) != sizeof(sendBuf)) || memcmp(sendBuf, recvBuf, sizeof(sendBuf)))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len1+len2, sizeof(sendBuf));
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

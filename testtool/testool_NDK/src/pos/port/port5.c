/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: port5.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_PortReadLen�ӿڹ���
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
#define 	TESTAPI		"NDK_PortReadLen"
#define		MAX_SIZE	BUFSIZE_SERIAL
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
void port5(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, ret1 = -1, port = -1, len = 0,len1 = 0,readlen = 0, j = 0;
	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};
	char *pszAttr;
	int bpsid=0;
	
	//����Ϊ������
	//����ǰ��:�رմ���
	//ѡ����ԵĴ���(����1�򴮿�2),������
	cls_show_msg1(2, "%s������...", TESTAPI);
#if !K21_ENABLE  //������ȷ�ϵͶ�ƽ̨�޴˽ӿ�
	NDK_SysSetSuspend(0);//��ֹ��������
#endif
	port = select_serial();
	pszAttr = conf_serial(&bpsid);
	NDK_PortClose(port);
	
	//case1:��δ���Ĵ��ڽ���ȡ�������ֽ�������,Ӧ������Ӧ����ֵ
	if((ret = NDK_PortReadLen(port,&len)) != NDK_ERR_OPEN_DEV)	/*ȡ�������ֽ���*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}
	
	//case2:�򿪴���,����Ƿ�����NULL,�Ƿ�����
	cls_show_msg1(MAXWAITTIME, "�̽���ѡ���ڵ�23��,���������...");
	if((ret=NDK_PortOpen(port, pszAttr)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_PortReadLen(-1,&len)) != NDK_ERR_PARA)
		||((ret1=NDK_PortReadLen(port,NULL)) != NDK_ERR_PARA))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		GOTOERR;
	}
		
	//case3:����ʹ�ò���
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
	NDK_SysDelay(MAXWAITTIME/(bpsid+1)*10);	/*��������·����Ҫʱ��,�ӳ�һ��,�ò������ݽ�����ջ�����*/
	if((ret = NDK_PortReadLen(port,&len)) != NDK_OK)	/*ȡ�������ֽ���*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}
	if(len != sizeof(sendBuf))//��һ�ζ����ĳ��Ȳ�Ϊ4096��������
	{
		while(1)//����������Ӧ��ȷ�������ں�ԭ��NDK_PortReadLen�������ֻ�ܷ���4095,�����������ѭ���ĵ��øú���ͨ����ε��õĳ��Ⱥ����ж������Ƿ�
		{
			if((ret = NDK_PortReadLen(port,&len1)) != NDK_OK)	/*ȡ�������ֽ���*/
			{				
				send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len1);
				goto ERR;
			}
			if(len1==0)
			{
				if(readlen!=sizeof(sendBuf))//��ε��ú�ĳ��Ⱥ�Ӧ�õ���Ԥ��ֵ
				{
					send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, readlen, sizeof(sendBuf));
					goto ERR;
				}
				break;
			}
			readlen+=len1;
			memset(recvBuf,0,sizeof(recvBuf));
			if((ret = NDK_PortRead(port, len1, recvBuf, MAXWAITTIME/(bpsid+1)*1000, &len)) != NDK_OK)	/*��ȡ��������*/
			{				
				send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
				goto ERR;
			}	
			if(len!=len1)//NDK_PortReadLenȡ���ĳ��ȱ����NDK_PortReadʵ�ʶ�����һ��
			{				
				send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, len, len1);
				goto ERR;
			}
		}
	}
	
	//case4:�建����ڵ���
	if((ret = NDK_PortClrBuf(port)) != NDK_OK)	/*����ջ���*/
	{				
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_PortReadLen(port,&len)) != NDK_OK)	/*ȡ�������ֽ���*/
	{				
 		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
 		GOTOERR;
	}
	if(len != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, len);
		GOTOERR;
	}
	
	//case5:�������ݽ��պ󣬽��ջ���������0ô
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
	if((ret = NDK_PortReadLen(port,&len)) != NDK_OK)	/*ȡ�������ֽ���*/
	{				
		send_result("line %d:%s����ʧ��(%d),%d", __LINE__, TESTAPI, ret, len);
		GOTOERR;
	}
	if(len != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, len);
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
#if !K21_ENABLE  //������ȷ�ϵͶ�ƽ̨�޴˽ӿ�
	NDK_SysSetSuspend(1);//�����������
#endif
	return;
}

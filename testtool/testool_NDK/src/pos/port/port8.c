/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: 
* Author 			: liny
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����ME66Aָ���
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
#define		TESTAPI		"ME66Aָ�����"
#define WAITTIME 30
#define		MAXWAITTIME 10//���յȴ�ʱ��

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
*			  				liny		          20180730    	 		created
*****************************************************************/
void port8(void)
{
	//����Ϊ�ֲ�����
	int ret = 0,nkeyin=0;
	//Ӧ�÷���ÿ��ָ����涼����س�����
    	char sendbuf[200]={0}, recbuf[200]={0},expected[]={"<STX><0009><SET><00><01><00><ETX><5E>\x0d\x0a"},expected1[]={"<STX><0114><SET><00><01><00><ECHO=1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890><ETX><6D>\x0d\x0a"};
	int len = 0;

	//����Ϊ������
	//����ǰ��:�رմ���
	if((ret=NDK_PortClose(PORT_NUM_USB_HOST))!=NDK_OK)
	{
		send_result("line %d:���ڹر�ʧ��(%d)", __LINE__, ret);
		GOTOERR;
   	}
	
	//case1:��������Ϊ1
	cls_show_msg("������ɨ����,���������");
	if ((ret=NDK_PortOpen(PORT_NUM_USB_HOST, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:���ڴ�ʧ��(ret:%d)", __LINE__, ret);
		GOTOERR;
	}
	if((ret = NDK_PortClrBuf(PORT_NUM_USB_HOST)) != NDK_OK)	/*����ջ���*/
	{				
		cls_show_msg("line %d:�建����ʧ��(%d)", __LINE__, ret);
		NDK_PortClose(PORT_NUM_USB_HOST);
		return;	
    }
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, "<STX><0015><SET><00><00><VOLUME=1><ETX><56>", sizeof(sendbuf));
	//��ģʽ����ָ��
	if((ret = NDK_PortWrite(PORT_NUM_USB_HOST, strlen(sendbuf), sendbuf))!=NDK_OK)
	{
		send_result("line %d:����дʧ��(ret:%d)", __LINE__,  ret);
		GOTOERR;
	}
	if((nkeyin=cls_show_msg1(WAITTIME,"��ȷ�Ͽ����״����д�case,������С,������������Ϊ:����1.�ǰ�ȷ�ϼ�,����������"))!=ENTER)
	{
		send_result("line %d:�������������쳣", __LINE__);
		GOTOERR;
	}
	memset(recbuf, 0, sizeof(recbuf));
	//����ָ��
	if((ret = NDK_PortRead(PORT_NUM_USB_HOST,sizeof(recbuf),recbuf,MAXWAITTIME*1000,&len))!=NDK_OK)
	{
		send_result("line %d:���ڶ�ʧ��(ret:%d)", __LINE__, ret);
		GOTOERR;
	}
	/*for(i=0; i<strlen(recbuf);i++)
        {
            fprintf(stderr,"%d:[%02x] ",i,recbuf[i] );
        }
		send_result("\n");
		for(i=0; i<sizeof(expected);i++)
        {
            fprintf(stderr,"%d:[%02x] ",i,expected[i] );
        }*/
	if(memcmp(recbuf,expected,strlen(recbuf)))
	{
		send_result("line %d:��Ԥ�ڷ������ݲ�һ��(%d,%d)", __LINE__, len, sizeof(expected));
		GOTOERR;
	}
		
	//case2:	���ֽڷ���
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, "<STX><0081><SET><00><00><VOICE=00521 2 3 4 5 6 7 8 9 0ABCDEFGHIJKLMNOPQRSTUVWXYZ�й�����������β+=_><ETX><48>", sizeof(sendbuf));
	if((ret = NDK_PortWrite(PORT_NUM_USB_HOST, strlen(sendbuf), sendbuf))!=NDK_OK)
	{
		send_result("line %d:����дʧ��(ret:%d)", __LINE__,  ret);
		GOTOERR;
	}
	if((nkeyin=cls_show_msg1(WAITTIME,"��ȷ��������������Ϊ:1 2 3 4 5 6 7 8 9 0ABCDEFGHIJKLMNOPQRSTUVWXYZ�й�����������β+=_.�ǰ�ȷ�ϼ�,����������"))!=ENTER)
	{
		send_result("line %d:�������������쳣", __LINE__);
		GOTOERR;
	}
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB_HOST,sizeof(recbuf),recbuf,MAXWAITTIME*1000,&len))!=NDK_OK)
	{
		send_result("line %d:���ڶ�ʧ��(ret:%d)", __LINE__, ret);
		GOTOERR;
	}
	if(memcmp(recbuf,expected,strlen(recbuf)))
	{
		send_result("line %d:��Ԥ�ڷ������ݲ�һ��(%d,%d)", __LINE__, len, sizeof(expected)-1);
		GOTOERR;
	}
	
	//case3:���ֽڷ���
	//�ر�LED��
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, "<STX><0014><SET><00><00><LED=OFF><ETX><6C>", sizeof(sendbuf));
	if((ret = NDK_PortWrite(PORT_NUM_USB_HOST, strlen(sendbuf), sendbuf))!=NDK_OK)
	{
		send_result("line %d:����дʧ��(ret:%d)", __LINE__,  ret);
		GOTOERR;
	}
	if((nkeyin=cls_show_msg1(WAITTIME,"��ȷ��LED���Ƿ�ر�.�ǰ�ȷ�ϼ�,����������"))!=ENTER)
	{
		send_result("line %d:LED�ƹر�ʧ��", __LINE__);
		GOTOERR;
	}
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB_HOST,sizeof(recbuf),recbuf,MAXWAITTIME*1000,&len))!=NDK_OK)
	{
		send_result("line %d:���ڶ�ʧ��(ret:%d)", __LINE__, ret);
		GOTOERR;
	}
	if(memcmp(recbuf,expected,strlen(recbuf)))
	{
		send_result("line %d:��Ԥ�ڷ������ݲ�һ��(%d,%d)", __LINE__, len, sizeof(expected)-1);
		GOTOERR;
	}
	//����LED��
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, "<STX><0013><SET><00><00><LED=ON><ETX><25>", sizeof(sendbuf));
	if((ret = NDK_PortWrite(PORT_NUM_USB_HOST, strlen(sendbuf), sendbuf))!=NDK_OK)
	{
		send_result("line %d:����дʧ��(ret:%d)", __LINE__,  ret);
		GOTOERR;
	}
	if((nkeyin=cls_show_msg1(WAITTIME,"��ȷ��LED���Ƿ���.�ǰ�ȷ�ϼ�,����������"))!=ENTER)
	{
		send_result("line %d:LED�ƹر�ʧ��", __LINE__);
		GOTOERR;
	}
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB_HOST,sizeof(recbuf),recbuf,MAXWAITTIME*1000,&len))!=NDK_OK)
	{
	    send_result("line %d:���ڶ�ʧ��(ret:%d)", __LINE__, ret);
		GOTOERR;
	}
	if(memcmp(recbuf,expected,strlen(recbuf)))
	{
		send_result("line %d:��Ԥ�ڷ������ݲ�һ��(%d,%d)", __LINE__, len, sizeof(expected)-1);
		GOTOERR;
	}
	
	//case4:���Բ���
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, "<STX><0112><SET><00><00><ECHO=1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890><ETX><68>", sizeof(sendbuf));
	if((ret = NDK_PortWrite(PORT_NUM_USB_HOST, strlen(sendbuf), sendbuf))!=NDK_OK)
	{
		send_result("line %d:����дʧ��(ret:%d)", __LINE__,  ret);
		GOTOERR;
	}
	memset(recbuf, 0, sizeof(recbuf));
	if((ret = NDK_PortRead(PORT_NUM_USB_HOST,sizeof(recbuf),recbuf,MAXWAITTIME*1000,&len))!=NDK_OK)
	{
		send_result("line %d:���ڶ�ʧ��(ret:%d)", __LINE__, ret);
		GOTOERR;
	}
	if(memcmp(recbuf,expected1,strlen(recbuf)))
	{
		send_result("line %d:��Ԥ�ڷ������ݲ�һ��(%d,%d)", __LINE__, len, sizeof(expected1)-1);
		GOTOERR;
	}
	if((ret=NDK_PortClose(PORT_NUM_USB_HOST))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
	    GOTOERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	if (g_com!=INVALID_COM)
	{
		if ((ret=NDK_PortOpen(g_com, "115200,8,N,1")) != NDK_OK)
		{
			cls_show_msg("line %d: ��ʼ�����ڴ�", __LINE__);
			return;
		}
	}
	return;
}



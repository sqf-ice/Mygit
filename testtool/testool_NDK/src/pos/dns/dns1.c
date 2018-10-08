/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ethģ��
* file name		: eth9.c
* Author 			: huangjianb
* version			: 
* DATE				: 20140623
* directory 	: 
* description	: ����NDK_NetDnsResolv�ӿڹ���
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
#define 	TESTAPI		"NDK_NetDnsResolv"

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
*			  						huangjianb	20140623	 		created
*****************************************************************/
void dns1(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, ret1 = -1, ret2 = -1;
	char DnsName[16] = "www.baidu.com", DnsName126[16] = "www.126.com";
	char ErrDnsName[16] = "err1111111", ErrDnsName1[16] = "err2222222";
	char LongDnsName[60] = "hardware-upgrade.oss-cn-hangzhou.aliyuncs.com";
	char DnsIP[16] = {0}, DnsIP126[16] = {0}, DnsIP1[16] = {0},DnsIPlong[60] = {0};
	LinkType dnstype = 0;
	EM_COMM_TYPE commtype = 0;
	
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}

	dnstype = getlinktype();
	switch(dnstype)
	{
		case GPRS:
		case CDMA:
		case TD:
		case ASYN:
			commtype = COMM_TYPE_PPP;
			break;
		case NONE:
		case SYNC:
		case SERIAL:
			commtype = COMM_TYPE_UNKNOW;
			break;
		default:
		case ETH:
			commtype = COMM_TYPE_ETH;
			break;
		case WLAN:
			commtype = COMM_TYPE_WIFI;
			break;
		case BT:
			commtype = COMM_TYPE_BTH;
			break;
	}
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	if(NetUp()!=SUCC)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	
	//case1:����Ƿ�����,����������
	if(((ret=NDK_NetDnsResolv(commtype,NULL, DnsName)) != NDK_ERR_PARA)
		||((ret1=NDK_NetDnsResolv(commtype,DnsIP, NULL)) != NDK_ERR_PARA)
		||((ret2=NDK_NetDnsResolv(COMM_TYPE_UNKNOW,DnsIP, DnsName)) != NDK_ERR_NET_UNKNOWN_COMMTYPE)){
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, ret1,ret2);
		GOTOERR;
	}

	//case5:��������������޷�������Ӧ���ش���  20180517  sull add  //����������ַʱ���������й�ϵ���д����Ż�
	memset(DnsIP, 0, sizeof(DnsIP));
	if((ret=NDK_NetDnsResolv(commtype,DnsIP,ErrDnsName)) != NDK_ERR)
	{
		if(ret == NDK_OK)  
		{			
			if((ret=NDK_NetDnsResolv(commtype,DnsIP1,ErrDnsName1)) != NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			if(strcmp(DnsIP, DnsIP1))
			{
				send_result("line %d:%s����ʧ��(DnsIP = %s, DnsIP1 = %s)", __LINE__, TESTAPI, DnsIP, DnsIP1);
				GOTOERR;
			}
		}
		else
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}		
	
	//case2:��������,�˴�����ͨ������ڴ����ò�ͬ��ͨѶ��ʽ��DHCP��ʽ����֤������ȷ������������������Ƿ���ȷ
	memset(DnsIP, 0, sizeof(DnsIP));
	if ((ret=NDK_NetDnsResolv(commtype,DnsIP, DnsName)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	if(!strlen(DnsIP))
	{
		send_result("line %d:%s����ʧ��IP:%s", __LINE__, TESTAPI, DnsIP);
		goto ERR;
	}

	//case3:��������
	memset(DnsIP, 0, sizeof(DnsIP));
	if ((ret=NDK_NetDnsResolv(commtype,DnsIP, DnsName)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	if(!strlen(DnsIP))
	{
		send_result("line %d:%s����ʧ��IP:%s", __LINE__, TESTAPI, DnsIP);
		goto ERR;
	}

	//case4:�������Խ���www.126.com  20161019
	memset(DnsIP126, 0, sizeof(DnsIP126));
	if ((ret=NDK_NetDnsResolv(commtype,DnsIP126, DnsName126)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	if(!strlen(DnsIP126))
	{
		send_result("line %d:%s����ʧ��IP:%s", __LINE__, TESTAPI, DnsIP126);
		goto ERR;
	}

	//case5:�������Խ���������20180830
	memset(DnsIPlong, 0, sizeof(DnsIPlong));
	if ((ret=NDK_NetDnsResolv(commtype,DnsIPlong, LongDnsName)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	if(!strlen(DnsIPlong))
	{
		send_result("line %d:%s����ʧ��IP:%s", __LINE__, TESTAPI, DnsIPlong);
		goto ERR;
	}
	send_result("������:%s,IP:%s", LongDnsName, DnsIPlong);
	
	//���Խ���
	send_result("%s����ͨ��!�ٶ�����:%s IP:%s,126����:%s IP:%s,ETH��WIFI��ֱ����ö�̬IP�;�̬IP���ظ�����������", TESTAPI, DnsName, DnsIP, DnsName126, DnsIP126);
ERR:
	NetDown();
	return;
}


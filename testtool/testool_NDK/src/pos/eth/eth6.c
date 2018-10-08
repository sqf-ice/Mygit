/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��̫��ģ��
* file name		: eth1.c
* Author 			: chensl
* version			: 
* DATE				: 20130105
* directory 	: 
* description	: ����NDK_NetGetAddr�ӿڹ���
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
#define 	TESTAPI		"NDK_NetGetAddr"
//#define		MAX_SIZE	128
#define		MAXSETTIME	2//��̬����IP��ַ��ʱ������

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
void eth6(void)
{
	//����Ϊ�ֲ�����
	int ret=-1, ret1 = -1, ret2 = -1, ret3 = -1, i = 0;
	char szIPaddr[16] = {0}, szMaskaddr[16] = {0}, szGateway[16] = {0}, szDNS[16*3+2] = {0}, szDNS3[16*3+2] = {0};
	float settime = 0.0;
#if K21_ENABLE
	char str[32] = {0};
#endif

	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������
	
	//case1:����Ƿ�����,����ΪNULL
	if((ret=NDK_EthSetAddress(NULL, NULL, NULL, NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_NetGetAddr(COMM_TYPE_ETH, NULL, NULL, NULL, NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:����Ƿ�����,����Ϊ����ȷ�ĵ�ַ
	if(((ret=NDK_EthSetAddress("192.168.4", netsetting.LocalMask.Str, netsetting.LocalGateway.Str, NULL)) != NDK_ERR_PARA/*NDK_ERR*/)//�ײ����ӵ�ַ�Ƿ��жϷ��� NDK_ERR_PARA
		||((ret1=NDK_EthSetAddress(netsetting.LocalIP.Str, "255.255.255", netsetting.LocalGateway.Str, NULL)) != NDK_ERR_PARA/*NDK_ERR*/)
		||((ret2=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, "192.168.4.2254", NULL)) != NDK_ERR_PARA/*NDK_ERR*/)
		||((ret3=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, "")) != NDK_ERR)){
		send_result("line %d:%s����ʧ��(%d,%d,%d,%d)", __LINE__, TESTAPI, ret, ret1, ret2, ret3);
		RETURN;
	}
	
	if(((ret=NDK_NetGetAddr(COMM_TYPE_ETH-1, szIPaddr, szMaskaddr, szGateway, NULL)) != NDK_ERR_PARA)
		||((ret1=NDK_NetGetAddr(COMM_TYPE_UNKNOW, szIPaddr, szMaskaddr, szGateway, NULL)) != NDK_ERR_PARA))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		RETURN;
	}
	
	
	//case3:��ipv4��ʽ,ipv6��ʽ
	cls_printf("��������IPv6...");
	if ((ret=NDK_EthSetAddress("AAAA.AAAA.AAAA.AAAA.AAAA.AAAA", "FFFF.FFFF.FFFF.FFFF.FFFF.0000", "AAAA.AAAA.AAAA.AAAA.AAAA.FFFE", NULL)) != NDK_ERR_PARA/*NDK_ERR*/)//�ײ����ӵ�ַ�����ȳ���16ֱ�ӷ��ز���������ж�,����IPV6����16����ֱ�ӷ����˲�������
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case4:��ȷIPv4��ʽ,�Ҳ�����DNS
	cls_printf("��������IPv4...");
	if ((ret=NDK_EthSetAddress("1.0.0.1", "255.0.0.0", "1.0.0.254", NULL)) != NDK_OK)//����C���ַ,������IPv4ҲӦ�����óɹ�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, szMaskaddr, szGateway, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if (strcmp(netsetting.LocalIP.Str, szIPaddr) || strcmp(netsetting.LocalMask.Str, szMaskaddr) || strcmp(netsetting.LocalGateway.Str, szGateway))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case5:����һ��DNS������IP��ַ
	cls_printf("��������DNS������...");
	memset(szDNS,0,sizeof(szDNS));
	if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, DNS3)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, szMaskaddr, szGateway, szDNS)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if (memcmp(DNS3, szDNS, strlen(DNS3)))
	{
		send_result("line %d:%s����ʧ��DNS:%s", __LINE__, TESTAPI, szDNS);
		RETURN;
	}

	//case6:��������DNS������IP��ַ
	memset(szDNS3,0,sizeof(szDNS3));
	memset(szDNS,0,sizeof(szDNS));
	sprintf(szDNS3, "%s;%s;%s", DNS1, DNS2, DNS3);
	if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, szDNS3)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	memset(szDNS, 0, sizeof(szDNS));
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, szMaskaddr, szGateway, szDNS)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if (strcmp(szDNS3, szDNS))
	{
		send_result("line %d:%s����ʧ��DNS:%s", __LINE__, TESTAPI, szDNS);
		RETURN;
	}
	
	//case7:��������ÿһ��
	cls_printf("���Ե�������ÿ������...");
	if ((ret=NDK_EthSetAddress("1.0.0.1", "255.0.0.0", "1.0.0.254", "192.168.30.1")) != NDK_OK)	//������ÿ�����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, NULL, NULL, NULL)) != NDK_OK)	//��������IP
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_EthSetAddress(NULL, netsetting.LocalMask.Str, NULL, NULL)) != NDK_OK)	//��������Mask
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_EthSetAddress(NULL, NULL, netsetting.LocalGateway.Str, NULL)) != NDK_OK)	//������������
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_EthSetAddress(NULL, NULL, NULL, DNS3)) != NDK_OK)	//��������DNS
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	memset(szIPaddr, 0, sizeof(szIPaddr));
	memset(szMaskaddr, 0, sizeof(szMaskaddr));
	memset(szGateway, 0, sizeof(szGateway));
	memset(szDNS, 0, sizeof(szDNS));
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, NULL, NULL, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, NULL, szMaskaddr, NULL, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, NULL, NULL, szGateway, NULL)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if ((ret=NDK_NetGetAddr(COMM_TYPE_ETH, NULL, NULL, NULL, szDNS)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if (strcmp(netsetting.LocalIP.Str, szIPaddr) 
		|| strcmp(netsetting.LocalMask.Str, szMaskaddr) 
		|| strcmp(netsetting.LocalGateway.Str, szGateway)
		|| memcmp(DNS3, szDNS,strlen(DNS3)))
	{
		send_result("line %d:%s����ʧ��%s", __LINE__, TESTAPI, szDNS);
		RETURN;
	}

	//case8:���Ծ�̬���ù��ܵ�ִ�в�Ӧ����2s(����ֵ�涨)
	cls_printf("���Ծ�̬����IPʱ��������,���Ե�");
	lib_StartStopwatch();
	for(i=0;i<20;i++)
	{
		if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, netsetting.DNSHost[1]))!=NDK_OK)
		{                                                                                            
			send_result("line %d:%s����ʧ��(%d)", __LINE__, ret);
			RETURN;
		}
	}
	if((settime=lib_ReadStopwatch()/20)>MAXSETTIME)
	{
		send_result("line %d:%s����ʧ��(NDK_EthSetAddressʱ��:%f)", __LINE__, TESTAPI, settime);
		RETURN;
	}
	
	//���Խ���
#if K21_ENABLE
	memset(str,0,sizeof(str));
	ftos(settime,str);
	send_result("%s����ͨ��(NDK_EthSetAddressʱ��:%s)", TESTAPI, str);
#else
	send_result("%s����ͨ��(NDK_EthSetAddressʱ��:%f)", TESTAPI, settime);
#endif
	return;
}


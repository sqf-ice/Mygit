/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: eth ģ��
* file name			: 
* Author 			: liny
* version			: 
* DATE				: 20180130
* directory 			: 
* description			: �����ϵ���eth ģ�麯�����Ƿ���ܷ�
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"
#define		TIMEOUT		3.0

/*---------------constants/macro definition---------------------*/
#define	TESTAPI	"ETHģ����������"

/*------------global variables definition-----------------------*/
/*----------global variables declaration("extern")--------------*/
/*---------------structure definition---------------------------*/
//����ö��
typedef enum {
    EthSetAddress,
    EthGetMacAddr,
    NetPing,
    NetDHCP,
    NetGetAddr,
    NetAddRouterTable,
    NetDnsResolv,
    EthGetNetAddr,
    EthDisConnect,
#if !defined ME50C
     GetDnsIp,
     NetSetDns,
#endif
    MaxNum
}EM_ETH;
/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: 
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*                                 linying            20180211           created
*****************************************************************/
//�������ETHģ�麯��
static int ETH_RandTest(int funnum)
{
    	//����Ϊ�ֲ�����
    	int ret=-1;
	char szIPaddr[16] = {0}, szMaskaddr[16] = {0}, szGateway[16] = {0}, szDNS[16*3+2] = {0};	
	char DnsName[16] = "www.baidu.com";
	char MacAddr[18] = {0};
	char DnsIP[16] = {0};
	EM_COMM_TYPE commtype = 0;
   	srand((unsigned)time(NULL));
	switch(funnum)
    	{
	    case EthSetAddress:
		    if ((ret=NDK_EthSetAddress(netsetting.LocalIP.Str, netsetting.LocalMask.Str, netsetting.LocalGateway.Str, DNS3)) != NDK_OK)
	        {
		       	 send_result("line %d:���������ַʧ��(%d)", __LINE__, ret);
		         return ret;
		     }
			break;
		case EthGetMacAddr:
			memset(MacAddr, 0, sizeof(MacAddr));
	        NDK_EthGetMacAddr(MacAddr);
			break;
		case NetPing:
			NDK_NetPing(netsetting.SvrIP.Str, TIMEOUT);
			break;
		case NetDHCP:
		    NDK_NetDHCP();
			break;
		 case NetGetAddr:
		 	NDK_NetGetAddr(COMM_TYPE_ETH, szIPaddr, szMaskaddr, szGateway, szDNS);
			break;
		 case NetAddRouterTable:
		 	NDK_NetAddRouterTable(COMM_TYPE_ETH,"192.168.1.1");
			break;
		 case NetDnsResolv:
		 	NDK_NetDnsResolv(COMM_TYPE_ETH,DnsIP, DnsName);
			break;
		 case EthGetNetAddr:
			memset(szDNS,0,sizeof(szDNS));
	        NDK_EthGetNetAddr(szIPaddr, szMaskaddr, szGateway, szDNS) ;
			break;
		case EthDisConnect:
		 	 if ((ret=NDK_EthDisConnect()) != NDK_OK)
		        {
			        send_result("line %d:��̫���Ͽ�ʧ��(%d)", __LINE__, ret);
			        return ret;
		        }
			break;	
#if !defined ME50C
		 case GetDnsIp:		
			memset(DnsIP, 0, sizeof(DnsIP));
			NDK_GetDnsIp(DnsIP, DnsName);
			break;
		case NetSetDns:
		 	memset(szDNS,0,sizeof(szDNS));
	        sprintf(szDNS, "%s;%s;%s", DNS1, DNS2, DNS3);	
	        NDK_NetSetDns(commtype,szDNS);	
			break;
#endif
    	}
	return SUCC;
}
//��������������ú��� ETH_RandTest���������ϲ���
void eth200(void)
{
	/*private & local definition*/
	int i = 0, bak = 0,ret = -1,cnt =100, len = 0;//cnt ����   
	int num[30]={0}, j = 0;
#if !defined ME50C
	char *ethstr[] = {" NDK_EthSetAddress","NDK_EthGetMacAddr","NDK_NetPing","NDK_NetDHCP","NDK_NetGetAddr","NDK_NetAddRouterTable","NDK_NetDnsResolv","NDK_EthGetNetAddr","NDK_GetDnsIp","NDK_EthDisConnect","NDK_NetSetDns"};
#else
    char *ethstr[] = {" NDK_EthSetAddress","NDK_EthGetMacAddr","NDK_NetPing","NDK_NetDHCP","NDK_NetGetAddr","NDK_NetAddRouterTable","NDK_NetDnsResolv","NDK_EthGetNetAddr","NDK_EthDisConnect"};
#endif
	/*process body*/
	if(MaxNum>20)
		len = MaxNum;	 
	else
		len = 20;
	while(cnt)
	{	
		cnt--;
		bak++;			
		//���������
		srand((unsigned)time(NULL));
		for(i=0;i<len;i++)
		{	
			num[i] = rand()%MaxNum;
		}
		cls_printf("��%d�����˳��:",bak);
		for(i=0;i<len;i++)
		{
			NDK_ScrPrintf("%d ", num[i]);
			NDK_ScrRefresh();
		}
		//�����������
		for(i=0;i<len;i++)
		{
			if((ret =  ETH_RandTest(num[i])) != NDK_OK)
			{
				for(j=0;j<=i;j++)
					send_result("%s ",ethstr[num[j]]);
				goto ERR;
			}
		}			
	}
	send_result("%s����ͨ��", TESTAPI);
ERR:
#if !defined ME50C
	NDK_EthDisConnect();
#endif
	return;
}



/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��wifiģ��
* file name			: 
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description			: WIFIģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
*  Revision 1.0  2011.09.11 bcat
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"WIFIģ��"
#define MAXWAITTIME 30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
int scan_get_mac(uchar outmac[6])
{
	/*private & local definition*/
	int ret = 0, i = 0;
	ST_WIFI_AP_LIST list_scan;
	ST_WIFI_AP_INFO ap[32];
	char ssidout[32][33];
	uint ssidoutlen = 0;

	/*process body*/
	//cls_printf("ɨ��AP��,���Ժ�...");
	list_scan.num = 32;
	list_scan.list = ap;
	memset(ap,0,sizeof(ap));
	if ((ret=NDK_WifiGetFunc(WIFI_GET_FUNC_SCAN_RESULT,&list_scan)) != NDK_OK) 
	{
		cls_show_msg("line %d:��ȡwifiɨ����ʧ��(%d)", __LINE__,ret);
		return FAIL;
	}
	for (i=0; i<list_scan.num; i++)
	{
		if(!strcmp(ap[i].ssid,""))//���������ap  ֱ�Ӵ������buf
			strcpy(ssidout[i],ap[i].ssid);
		else
		{
			if((ret=NDK_Utf8ToGbk((uchar *)ap[i].ssid,strlen(ap[i].ssid),(uchar *)ssidout[i],&ssidoutlen))!=NDK_OK)
			{
				cls_show_msg("line %d:utf8ת��gbkʧ��(%d)", __LINE__, ret);
				return FAIL;
			}
		}
		if(cls_show_msg("AP%d:%s(%02x:%02x:%02x:%02x:%02x:%02x)�Ƿ�Ϊ����AP,��[ȷ��],��[����]", i, ssidout[i],ap[i].mac.octet[0],ap[i].mac.octet[1],ap[i].mac.octet[2],ap[i].mac.octet[3],ap[i].mac.octet[4],ap[i].mac.octet[5])==ENTER)//ѡ��SSID��
		{
			memcpy(outmac, ap[i].mac.octet, 6);
			break;
		}
	}
	if(i>=list_scan.num)
	{
		cls_show_msg("line %d:δɨ�赽������AP������ɨ��", __LINE__);
		return FAIL;
	}
	return SUCC;
}

/****************************************************************
* function name 	 		: WIFI
* functional description 	: WIFIģ�����������,�ɸ�����Ҫ��չ
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks	
*			  	 	       jiangym		 20160223      created 
*****************************************************************/
void wifi(PF* list, unsigned char id[])
{
	/*private & local definition*/
	int ret=-1, retime = -1;
	
	/*process body*/
	if(cls_show_msg1(MAXWAITTIME, "%s����...��ȷ����Ҫ���ò��Ի���(�����ĵ�)...ESC�˳���ģ�����", TESTMODULE)==ESC)//ʱ�䳤Щ,�Ա�֤�û���ʱ�����
		return;
	//�ָ��Զ�������Ĭ��ֵ :������
	retime = -1;
	if((ret = NDK_WifiSetFunc(WIFI_SET_FUNC_RELINK_TIME,&retime)) != NDK_OK)
	{
		cls_show_msg("line %d: ����ʧ��(%d)", __LINE__, ret);
		return;
	}
/*	switch(conf_conn_WLAN())//���������Ĳ��Ի���д��,��������
	{
	case SUCC:
		break;
	case FAIL:
		cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
		return;
		break;
	default:
		return;
		break;
	}
	*/
	
	//ִ�в�������
#if STABAR_ENABLE
	if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_TIME|STATUSBAR_DISP_WIFI))!=NDK_OK)
	{
		cls_show_msg("״̬����ʾʧ��!(%d)", ret);
		return;
	}
#endif
	process(list, id);
#if STABAR_ENABLE	
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
	return;
}


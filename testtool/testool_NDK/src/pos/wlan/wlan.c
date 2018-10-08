/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: WLANģ��
* file name		: 
* Author 			: chensj
* version			: 
* DATE			: 20130513
* directory 		: 
* description		: WLANģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
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
#define	TESTMODULE	"WLANģ��"
#define MAXWAITTIME 30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
extern void wlan13(void);

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: Wlan
* functional description 	: WLANģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks	
*			  	 	   
*****************************************************************/
void wlan(PF* list, unsigned char id[])
{
	/*private & local definition*/
	int ret=-1;
	PF tmplist[] = {wlan13, NULL};
	uchar tmpid[] = {13, 0}; 
	
	/*process body*/
	if(cls_show_msg1(MAXWAITTIME, "%s����...��ȷ��������AP...ESC�˳���ģ�����", TESTMODULE)==ESC)//ʱ�䳤Щ,�Ա�֤�û���ʱ�����
		return;
	
	switch(conf_conn_WLAN())
	{
	case SUCC:
		break;
	case FAIL:
		cls_show_msg("line %d:����δ��ͨ!!!\n������˳�����...", __LINE__);
		return;
		break;
//	case QUIT://�û�����
	default:
		return;
		break;
	}
	
	//ִ�в�������
#if STABAR_ENABLE
	if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_TIME|STATUSBAR_DISP_WIFI))!=NDK_OK)
	{
		cls_show_msg("״̬����ʾʧ��!(%d)", ret);
		return;
	}
#endif
	if(linksetting.WLANEncMode!=WIFI_NET_SEC_WPA_EAP)
		process(list, id);
	else
		process(tmplist, tmpid);
		
#if STABAR_ENABLE	
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
	return;
}


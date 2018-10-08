/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: dnsģ��
* file name		: dns.c
* Author 			: linwl
* version			: 
* DATE				: 20150705
* directory 	: 
* description	: dnsģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
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
#define	TESTMODULE	"dnsģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: dns
* functional description 	: ��̫��ģ�����������
* input parameter	 	:
* output parameter	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  						linwl		   20150705	 		created
*****************************************************************/
void dns(PF* list, unsigned char id[])
{
#if STABAR_ENABLE
	int ret=-1;
#endif
	float testtime = 0;
#if K21_ENABLE
	char str[32] = {0};
#endif
	//����Ϊ������
	cls_show_msg1(1, "%s����...", TESTMODULE);
	if(conf_conn_DNS()!=SUCC)
	{
		cls_show_msg("line %d:��������ʧ��!!!\n������˳�����...", __LINE__);
		return;
	}
	
#if STABAR_ENABLE
	if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_TIME/*|STATUSBAR_DISP_ETH*/))!=NDK_OK)
	{
		cls_show_msg("״̬����ʾʧ��!(%d)", ret);
		return;
	}
#endif

	lib_StartStopwatch();
	process(list, id);
	testtime = lib_ReadStopwatch();
	
#if STABAR_ENABLE	
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif

#if K21_ENABLE
	memset(str,0,sizeof(str));
	ftos(testtime,str);
	send_result("%s����ʱ��Ϊ%ss��", TESTMODULE,str);
#else
	send_result("%s����ʱ��Ϊ%f��", TESTMODULE,testtime);
#endif
	
	//ģ����Խ���
	return;
}


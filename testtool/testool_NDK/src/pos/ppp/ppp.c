/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module						: PPPͨѶģ��
* file name					: 
* Author 						: chensj
* version						: 
* DATE							: 
* directory 				: 
* description				: PPPͨѶģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: DNKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"PPPģ��"
#define	MAXTIMEOUT	60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
int IsPPPTimeout(int state, int *err)
{
	/*private & local definition*/
	time_t diff = 0, oldtime = time(NULL);
	EM_PPP_STATUS emStatus=0;
	char szIpaddr[20]={0};
	
	/*process body*/
	//��state==PHASE_NETWORKʱ,���һ�������붯̬IP�Ƿ�ɹ�(����һ��ȡ����,��ȡ����)   
	//getLocalAddr()==0��ʾδȡ�����ص�ַ(��δ����ɹ�)  
	if(state==PPP_STATUS_CONNECTED )
		return (lib_GetLocalAddr(szIpaddr, NULL, NULL, NULL)!=SUCC?TRUE:FALSE);
	else
	{
		while(1)
		{
			if(NDK_PppCheck(&emStatus, err)!=NDK_OK)
				return TRUE;
			if(emStatus!=state)
				break;
			//��ʱ��һֱ����ĳ��״̬̬,˵��FSM��������!
			if((diff=time(NULL)-oldtime)>MAXTIMEOUT)//if(ReadStopwatch()>MAXTIMEOUT)
				return TRUE;
			show_stopwatch(ENABLE, diff);
			//NDK_SysDelay(10);// 20140505linwl ����ʱ�����ƶ���show_stopwatch������
		}

		show_stopwatch(DISABLE, 0);
		return FALSE;
	}
}

/****************************************************************
* function name 	 				: ppp
* functional description 	: pppģ�����������,�ɸ�����Ҫ��չ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						: author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void ppp(PF* list, unsigned char id[])
{
	/*private & local definition*/
	if(auto_flag==2) 
	{
		g_UCID = 0;
		send_result("%s���Զ����ԣ���ʹ���Զ�������֤", TESTMODULE);
		return;
	}
	/*process body*/
#if STABAR_ENABLE
	NDK_ScrStatusbar(STATUSBAR_DISP_ALL);
#endif
	cls_show_msg1(1, "%s����...", TESTMODULE);
	if(conf_conn_PPP()!=SUCC)
		return;
	//����process����ģ�����ǰ,Ҫ�ȶ���·������linktype��ֵ
	process(list, id);
#if STABAR_ENABLE	
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif	
	//ģ����Խ���
	return;
}


/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: PPPͨѶģ��
* file name		: 
* Author 			: jiangwx
* version			: V1.0
* DATE			: 200601026
* directory 		: .\SRC\Net\
* description		: PPPͨѶģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  200601026 14:10 jiangwx
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
int IsPPPTimeout(int ph, int state)
{
	/*private & local definition*/
	time_t diff = 0, oldtime = time(NULL);//StartStopwatch();
	
	/*process body*/
	//��state==PHASE_NETWORKʱ,���һ�������붯̬IP�Ƿ�ɹ�(����һ��ȡ����,��ȡ����)   
	//getLocalAddr()==0��ʾδȡ�����ص�ַ(��δ����ɹ�)  
	if(state==PHASE_NETWORK)
		return (GetLocalAddr()==0?TRUE:FALSE);
	else
	{
		while(GetPPPphase(ph)==state)
		{
			//��ʱ��һֱ����ĳ��״̬̬,˵��FSM��������!
			if((diff=time(NULL)-oldtime)>MAXTIMEOUT)//if(ReadStopwatch()>MAXTIMEOUT)
				return TRUE;
			show_stopwatch(ENABLE, diff);
			delay(10);//����ʱ��Ϊ�˽���������״̬��оƬ��ѹ��
		}

		show_stopwatch(DISABLE, 0);
		return FALSE;	
	}
}

/****************************************************************
* function name 	 	: ppp
* functional description 	: pppģ�����������,�ɸ�����Ҫ��չ
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  			  jiangwx		   20061016 		created
*					linwl				20080923		modify
*****************************************************************/
void ppp(PF* list, unsigned char id[])
{
	/*private & local definition*/
	
	/*process body*/
	cls_show_msg1(1, "%s����...", TESTMODULE);

	//����process����ģ�����ǰ,Ҫ�ȶ���·������linktype��ֵ
	if(conf_conn_PPP()!=SUCC)
		return;
	process(list, id);
}


/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys5.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysShutDown�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysShutDown"

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
*			  		        linwei	           20130121	 	created
*****************************************************************/
void sys5(void)
{
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
#if STABAR_LED_ENABLE
	NDK_ScrStatusbar(STATUSBAR_DISP_LED|STATUSBAR_POSITION_TOP);
#endif
	NDK_LedStatus(LED_RFID_RED_ON);
	#if (defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME65||defined ME30GX||defined ME32GX||defined ME32THM||defined ME50||(CPU5810X_ENABLE&&!defined ME62)||defined ME50NX||defined ME68||defined ME50NS||defined ME31||ANDIROD_ENABLE)//||defined IM81||defined N900
		if(cls_show_msg("�������桪��\n������ػ���?\n��:�ε�USB�ߺ�[ENTER] ��[����]")==ENTER)//K21Ӳ�����USB��������²��ܹػ������ú�������߻����Ϲػ�
			NDK_SysShutDown();
		goto end;
	#elif  defined ME11||defined ME15||defined ME20||defined ME15C  //��ֻ�е�ع��������,��ع�����ʹ���ֻ�APP����
		if(cls_show_msg("������ػ���?(ֻ�е�ع���ʱ���ܹػ�)\n��:��[ENTER] ��[����]")==ENTER)
		{
			cls_show_msg1(5,"5���ڰε���ӵ�Դ��,�۲�LED��Ӧ�����");//���������������USB���ߴ����߹��磬������ػ�
			NDK_SysShutDown();
			return;
		}
		goto end;
	#else	
		send_result("���ڲ������˵���[.]����%s��֤", TESTAPI);
	#endif
end:
	NDK_LedStatus(LED_RFID_RED_OFF);
#if STABAR_LED_ENABLE
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
	return;
}


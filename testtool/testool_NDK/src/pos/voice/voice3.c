/************************************************************************
* copyright(c)2005-2016 Newland Co. Ltd. All rights reserved
* module			: ��Ƶ
* file name			: 3.c
* Author 			: 
* version			: 
* DATE				:20160511
* directory 			: 
* description			: ����NDK_PlayVoice�Ƿ�����������ŵ�0�ε���11�ε���Ƶ�ļ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_PlayVoice"
#define  MAXWAITTIME 30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: voice3
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*					    zhengry		 20160511		created
*****************************************************************/
void voice3(void)
{
	/*private & local definition*/
	int ret = 0, VoiceId = 0, nKeyin=0;
	char *voice_select[12] = {
		"�տ�ɹ�",
		"�տ�ʧ��",
		"���׳ɹ�",
		"����ʧ��",
		"��չʾ������",
		"��ӿ�",
		"�տ�ɹ���лл�ݹˣ�",
		"Ǯ����",
		"���׻���",
		"����",
		"��������",
		"�����",
	};

	/*process body*/	
	cls_show_msg1(2,"����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�����쳣����
	if((ret=NDK_PlayVoice(-1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_PlayVoice(12)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��������,������ʱ,������������Ӧ����������
	for(VoiceId=0; VoiceId<7; VoiceId++) //12--7
	{
		cls_printf("������������Ϊ:%s��...", voice_select[VoiceId]);
		if((ret=NDK_PlayVoice(VoiceId)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, VoiceId);
			RETURN;
		}
		NDK_SysDelay(30);
		nKeyin=cls_show_msg1(MAXWAITTIME, "������������Ϊ:%s,�Ҳ�����������Ӧ����������.��[ENTER],����[����].", voice_select[VoiceId]);
		if( nKeyin != ENTER && nKeyin != 0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

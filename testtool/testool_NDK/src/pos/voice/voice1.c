/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��Ƶ
* file name			: 1.c
* Author 			: 
* version			: 
* DATE				:20131028
* directory 			: 
* description			: ����NDK_VoiceLoadFile�Ƿ��������������Ƶ�ļ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_VoiceLoadFile"
#if K21_ENABLE

#else
#define  NOEXIST  "noexist.wav"
#define  ERRFILE   "errfile.wav"
#define  RIGHTFILE   "shuaka.wav"
#define  RIGHTFILE1   "duka.wav"
#endif
#define  MAXWAITTIME 30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: voice1
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*					  jiangym		20131030		created
*
*****************************************************************/
#if K21_ENABLE
void voice1(void)
{
	/*private & local definition*/
	int ret = 0, nKeyin=0;
	uint VoiceId = 0;
	char *voice_select[14] = {
		"�տ�ɹ�",		
		"�տ�ʧ��",		
		"���׳ɹ�",		
		"����ʧ��",
		"��չʾ������",	//ʵ��:��չʾ������
		"��ӿ�",
		"�տ�ɹ���лл�ݹˣ�",
		"Ǯ����",		//ʵ��:������ЧTTS��δ֧��
		"���׻���",		//ʵ��:������ЧTTS��δ֧��
		"����",
		"��������",		//ʵ��:������ЧTTS��δ֧��
		"�����",		//ʵ��:������ЧTTS��δ֧��
		"�Զ���Ĳ�������ABC",
		"CHINESE"
	};

	/*process body*/	
	cls_show_msg1(2,"����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�����쳣����
	if((ret=NDK_VoiceLoadFile(-1, NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_VoiceLoadFile(12, NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��������,������ʱ,������������Ӧ����������
#if !defined ME50NX //����ME50N��VoiceId��֧��0-11��������֧��
	for(VoiceId=0; VoiceId<12; VoiceId++)
	{
		cls_printf("������������Ϊ:%s��...", voice_select[VoiceId]);
		if((ret=NDK_VoiceLoadFile(VoiceId, NULL)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, VoiceId);
			RETURN;
		}
		NDK_SysDelay(40);//���û�в���������,�Ϳ��ٰ�����ʼ��һ���������Ż�ʧ�ܷ���-320,����������ʱ
		nKeyin=cls_show_msg1(MAXWAITTIME, "������������Ϊ:%s,�Ҳ�����������Ӧ����������.��[ENTER],����[����].", voice_select[VoiceId]);
		if( nKeyin != ENTER && nKeyin != 0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, VoiceId);
			RETURN;
		}
	}
#endif

	//case3:��������,����id��ЧʱpszFile���Դ����Զ���Ĳ�������,����ȷ����
	VoiceId = 12;
	cls_printf("������������Ϊ:%s��...", voice_select[VoiceId]);
	if((ret=NDK_VoiceLoadFile(VoiceId, voice_select[VoiceId])) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, VoiceId);
		RETURN;
	}
	//case4:��һ������δ��ɵ�ʱ������ϵ�����һ������,Ԥ�ڷ���-320
	if((ret=NDK_VoiceLoadFile(VoiceId, voice_select[VoiceId])) != NDK_ERR_WLM_SEND_AT_FAIL)//ע��:EC20ģ�鷵�سɹ��������ġ�EC20ģ���G510ģ�鲻ͬ��G510���ȷ�ATָ���ģ�����ȼ����ߣ�EC20ģ���Ǻ�ATָ����������ȼ�����
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, VoiceId);
		RETURN;
	}
	NDK_SysDelay(40);
	nKeyin=cls_show_msg1(MAXWAITTIME, "������������Ϊ:%s,�Ҳ�����������Ӧ����������.��[ENTER],����[����].", voice_select[VoiceId]);
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, VoiceId);
		RETURN;
	}

	//case5:��������,����id��ЧʱpszFile���Դ����Զ����Ӣ����ĸ����,����ȷ����
	VoiceId = 13;
	cls_printf("������������ΪӢ����ĸ��:%s��...", voice_select[VoiceId]);
	if((ret=NDK_VoiceLoadFile(VoiceId, voice_select[VoiceId])) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, VoiceId);
		RETURN;
	}
	NDK_SysDelay(40);
	nKeyin=cls_show_msg1(MAXWAITTIME, "������������ΪӢ����ĸ��:%s,�Ҳ�����������Ӧ����������.��[ENTER],����[����].", voice_select[VoiceId]);
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case6:����unVoiceId���Ȳ���,��ID�Ϸ�Ϊ1ʱ��ʹpszFile������ҲӦ�ò���ID1������
	VoiceId = 1;
	cls_printf("������������Ϊ:%s��...", voice_select[VoiceId]);
	if((ret=NDK_VoiceLoadFile(VoiceId, "������ű����������ʧ��")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, VoiceId);
		RETURN;
	}
	NDK_SysDelay(40);
	nKeyin=cls_show_msg1(MAXWAITTIME, "������������Ϊ:%s,�Ҳ�����������Ӧ����������.��[ENTER],����[����].", voice_select[VoiceId]);
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}
#else
void voice1(void)
{
	/*private & local definition*/
	int ret = 0, nKeyin=0;
	float loadtime = 0.0;
	uint VoiceId = 0;
		
	/*process body*/
	cls_show_msg1(2,"����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	cls_show_msg1(MAXWAITTIME, "ȷ����Ƶ�ļ�errfile.wav��shuaka.wav��duka.wav�Ѿ����ص����Գ���·��Ŀ¼��,���������");
	
	//case1: pszFileΪNULL
	if((ret=NDK_VoiceLoadFile(VoiceId,NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�����ڵ���Ƶ�ļ�
	if((ret=NDK_VoiceLoadFile(VoiceId,NOEXIST)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:��Ƶ�ļ���ʽ(ָ�ļ�����)����
	if((ret=NDK_VoiceLoadFile(VoiceId,ERRFILE)) != NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case4:��������
	VoiceId = 1;
	cls_printf("����������:��ˢ��");
	lib_StartStopwatch();
	if((ret=NDK_VoiceLoadFile(VoiceId,RIGHTFILE)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	loadtime=lib_ReadStopwatch();
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_START,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(2);//��ʱΪ����������Ч��
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_STOP,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "��������Ϊ:��ˢ��,��ȷ[Enter],����[����]");
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case5:��ͬ��Ƶ�ļ�����ͬһλ��
	VoiceId = 1;
	cls_printf("������������:��ˢ��");
	if((ret=NDK_VoiceLoadFile(VoiceId,RIGHTFILE)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_START,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(2);//��ʱΪ����������Ч��
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_STOP,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "��������Ϊ:��ˢ��,��ȷ[Enter],����[����]");
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case6:��ͬ��Ƶ�ļ����ص�ͬһλ��
	VoiceId = 1;
	cls_printf("����������:�����ɹ�");
	if((ret=NDK_VoiceLoadFile(VoiceId,RIGHTFILE1)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_START,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(2);//��ʱΪ����������Ч��
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_STOP,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "��������Ϊ:�����ɹ�,��ȷ[Enter],����[����]");
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case7:Ҫ֧��mp3��ʽ��Ƶ�ļ�(Ŀǰδ֧��,֮����)
	
	//��������
	send_result("%s����ͨ��(NDK_VoiceLoadFile��ʱ:%f)", TESTAPI, loadtime);
	return;
}
#endif

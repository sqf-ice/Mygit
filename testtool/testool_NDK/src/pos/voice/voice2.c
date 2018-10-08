/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��Ƶ
* file name			: 2.c
* Author 			: 
* version			: 
* DATE				:20131029
* directory 			: 
* description			: ����NDK_VoiceCtrl�Ƿ�����������в��ſ���
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_VoiceCtrl"
#define    RIGHTFILE   "yuyin.wav"
#define  MAXWAITTIME 30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: voice2
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*					  jiangym		20131030		created
*
*****************************************************************/
#if K21_ENABLE
void voice2(void)
{
	/*private & local definition*/
	int ret = 0, nKeyin=0;
	uint VoiceId = 5, i = 0, memTmp = 0;
	uint outvalue =0,err_value=0;
	char tmp[256] = {0};
#if defined ME68
	int j=0;
#endif

	/*process body*/	
	cls_show_msg1(2,"����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:����Ĭ������Ϊ���ֵ6
	if(((ret=NDK_VoiceCtrl(0,VOICE_CTRL_GET_VOLUME,&outvalue))!=NDK_OK)/*||(outvalue!=6)*/)
	{
		send_result("line %d:%s����ʧ��(ret=%d, volume=%d)", __LINE__, TESTAPI, ret, outvalue);
		RETURN;
	}
#if defined ME68 //����Ĭ�����ֵΪ5
	if(outvalue!=5)
#else
	if(outvalue!=6)
#endif
	{
		send_result("line %d:%s����ʧ��(ret=%d, volume=%d)", __LINE__, TESTAPI, ret, outvalue);
		RETURN;
	}
		
	//case2:��֧�ֵ���������0~5Ԥ�ڷ��ز�������
	for(i=VOICE_CTRL_START;i<VOICE_CTRL_VOLUME;i++)
	{
		if((ret=NDK_VoiceCtrl(0,i,&outvalue)) != NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(i=%d,ret=%d)", __LINE__, TESTAPI, i, ret);
			RETURN;
		}
	}

	//case3:��������ֵΪ�Ƿ�ֵ0��7Ԥ�ڷ���ʧ��
	err_value = 0;
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_VOLUME,&err_value)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	err_value = 7;
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_VOLUME,&err_value)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#if defined ME68 //ME68�������÷�������������
	err_value = 7;
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_BEPP_VOLUME,&err_value)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

	//case4:��ȡ����ʱ,���ֵ�ĵ�ַΪNULLԤ�ڷ���ʧ��
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_GET_VOLUME,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case5:��������Ϊ1~6Ӧ�óɹ�,���ú��ȡ��ֵӦ��Ϊ1~6,����Ƶ���ŵ�ʵ��Ч��Ϊ����������
	for(i=1;i<=6;i++)
	{
		if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_VOLUME,&i)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(invalue=%d,ret=%d)", __LINE__, TESTAPI, i, ret);
			RETURN;
		}
		if(((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_GET_VOLUME,&outvalue))!=NDK_OK)||(outvalue!=i))
		{
			send_result("line %d:%s����ʧ��(ret=%d, outvalue=%d, invalue=%d)", __LINE__, TESTAPI, ret, outvalue, i);
			RETURN;
		}
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "���������ŵ�����Ϊ%d", i);
		if((ret=NDK_VoiceLoadFile(12, tmp)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(invalue=%d,ret=%d)", __LINE__, TESTAPI, i, ret);
			RETURN;
		}
		NDK_SysDelay(30);
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "ǰ�沥�ŵ����������Ƿ�������.��[ENTER],����[����].");
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	sprintf(tmp, "��ȷ�ϱ��������ŵ�����ÿ2������1��");//��������ֻ֧�����37�ֽ����ݣ��������17����
	if((ret=NDK_VoiceLoadFile(12, tmp)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//��������ֻ֧�����37�ֽ����ݣ��������17��   20180717 add by wangkai
	sprintf(tmp, "�����������������ֻ֧��37�ֽ�����.");
	if((ret=NDK_VoiceLoadFile(12, tmp)) != NDK_ERR_WLM_SEND_AT_FAIL)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
#if defined ME68 
	//case7:���÷���������Ϊ0~6Ӧ�óɹ�,�ҷ��������ŵ�ʵ��Ч��Ϊ����������
	for(i=0;i<=6;i++)
	{
		cls_printf("���·�������������Ϊ%d",i);
		if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_BEPP_VOLUME,&i)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(invalue=%d,ret=%d)", __LINE__, TESTAPI, i, ret);
			RETURN;
		}
		for(j=0;j<20;j++)
			NDK_SysBeep();
		NDK_SysMsDelay(100);
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "ǰ�沥�ŵ����������Ƿ�������.��[ENTER],����[����].");
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
#endif
	
	//case6:��֤�������Ź����е�����������������Ч
	for(i=1;i<=6;i++)
	{
		
		//NDK_SysDelay(20);
		if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_VOLUME,&i)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(i=%d,ret=%d)", __LINE__, TESTAPI, i, ret);
			RETURN;
		}
		if(((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_GET_VOLUME,&outvalue))!=NDK_OK)||(outvalue!=i))
		{
			send_result("line %d:%s����ʧ��(ret=%d, volume=%d, i=%d)", __LINE__, TESTAPI, ret, outvalue, i);
			RETURN;
		}
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "ǰ�沥�ŵ����������Ƿ�������.��[ENTER],����[����].");
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//��������	
	send_result("%s����ͨ��", TESTAPI);
	return;
}
#else
void voice2(void)
{
	/*private & local definition*/
	int ret = 0,nKeyin =0;
	uint VoiceId = 5, ivalue = 0;
		
	/*process body*/	
	cls_show_msg1(2,"����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	//��������:��ˢ��,����ˢ,����,ˢ���ɹ�
	cls_show_msg(MAXWAITTIME, "ȷ����Ƶ�ļ�yuyin.wav�Ѿ����ص����Գ���·��Ŀ¼��,���������");
	
	//Ϊ�������������ʾ��Ϣ,�������ʾ��Ϣ����ʾ���ļ����سɹ��������ź�
	cls_printf("��������:��ˢ��,����ˢ,����,ˢ���ɹ�,ˢ���ɹ�,лл...");
	
	//case1:δ����NDK_VoiceLoadFile,����ʧ��
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_START,2)) != NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:����װ����Ƶ�ļ�������Ƿ�:emCtrlId�Ƿ� unValue�Ƿ�(����0-4)
	VoiceId = 3;
	if((ret=NDK_VoiceLoadFile(VoiceId,RIGHTFILE)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_START-1,2)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_VOLUME+1,2)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_START,5)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:���²���--ֹͣ����
	cls_printf("���Բ���-ֹͣ");
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_START,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(2);
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_STOP,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(1);
	nKeyin=cls_show_msg1(MAXWAITTIME, "��������\"��ˢ��\"��ֹͣ,��ȷ[Enter],����[����]");
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case4:����-��ͣ-�ָ�-ֹͣ
	cls_printf("���Բ���-��ͣ-�ָ�-ֹͣ");
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_START,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(2);
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_PAUSE,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(2);
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_RESUME,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(1);
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_STOP,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(1);
	nKeyin=cls_show_msg1(MAXWAITTIME, "����\"��ˢ��\"����ͣ,�ָֻ�����\"����ˢ\"��ֹͣ,��ȷ[Enter],����[����]");
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case5:����-�򿪾���-�رվ���-ֹͣ
	//�򿪾���ʱ��Ҳ�ڶ����Ѿ���"����ˢ,����"������,�رվ�����Ͷ�"ˢ���ɹ�"
	cls_printf("���Բ���-�򿪾���-�رվ���-ֹͣ");
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_START,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(2);
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_MUTE,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(3);
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_NMUTE,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(2);
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_STOP,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(1);
	nKeyin=cls_show_msg1(MAXWAITTIME, "����\"��ˢ��\"����,�ָֻ�����\"ˢ���ɹ�\"��ֹͣ,��ȷ[Enter],����[����]");
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case6:����-��������-ֹͣ
	cls_printf("���Բ���-��������-ֹͣ");
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_START,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	for(ivalue=0;ivalue<=4;ivalue++)
	{
		if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_VOLUME,ivalue)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		sleep(2);
	}
	if((ret=NDK_VoiceCtrl(VoiceId,VOICE_CTRL_STOP,2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME, "����������С����,��ȷ[Enter],����[����]");
	if( nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//��������	
	send_result("%s����ͨ��", TESTAPI);
	return;
}
#endif


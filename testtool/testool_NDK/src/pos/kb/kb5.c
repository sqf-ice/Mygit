/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name			: 5.c
* Author 			:
* version			: 
* DATE				: 
* directory 			: 
* description			: ����NDK_KbGetPad��ȡ���������Ĵ���״̬
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10
#define  TESTAPI		"NDK_KbGetPad"
#define  WUCHASEC 0.03//0.03 //���ֵ����
#define	 MAXTIMEOUT	10000  //10000ms=10s

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: kb5
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        jiangym		20131128	created
*****************************************************************/
void kb5(void)
{
	/*private & local definition*/
#include "kbtable.h"

	int cnt = 0,ret = 0;
	ST_PADDATA pad_data;
	float tmp = 0;
#if K21_ENABLE
	char str[32] = {0};
#endif
#if (defined ME50||defined SP610)	
		time_t oldtime = 0;
#else
	uint x = 0, y = 0;
	char *type[]={"����","����","����"};
	int laststatus=-1;
#endif

	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:������Ĳ���:pstPaddataΪNULL
	if ((ret=NDK_KbGetPad(NULL,MAXTIMEOUT))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#if !(defined ME50||defined SP610)
	//case2:���԰��µ���Ķ���timeoutΪ0���:���ⰴ����Ļ�ϵĵ�
	//(������ø�����Ļ�ϣ��£����ң��У����ϣ����ϣ����£����£��Ÿ�λ��),����״ֵ̬
	cls_printf("������Ļ��,��,��,��,��,����,����,����,���¾Ÿ�λ�����ⰴ�µ���򰴼�Ӧ���Ի�ȡ����Ӧ��״̬,ȡ������������.");
	NDK_ScrGetxy(&x,&y);
	while(1)
	{
		memset(&pad_data,0,sizeof(pad_data));
		if((ret=NDK_KbGetPad(&pad_data,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
		if(laststatus!=pad_data.emPadState)
		{
			laststatus = pad_data.emPadState;
			NDK_ScrGotoxy(x, y);
			NDK_ScrPrintf("״̬:%s",type[pad_data.emPadState]);
			NDK_ScrRefresh();
		}
		if (pad_data.emPadState == PADSTATE_KEY  && pad_data.unKeycode == ESC)
			break;
	}
	if(cls_show_msg("��ȡ��״̬��Ԥ��,[Enter]ͨ��,[����]ʧ��")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
#endif

	//case3:ʹ��ѭ�������жϰ��µ���Ķ���timeout=0 ,ͬʱʹ�õ�Ĺ켣��ʽ������ֵ�ж�
	// 5810xƽ̨û�а���\����\����״̬��ȡ,ֻ�л�ȡ����,��Ҫ��ʹ�÷�ʽ
#if  (defined ME50||defined SP610)
	cls_printf("������Ļ�ϻ���Ĺ켣,30s���������");
	oldtime = time(NULL);
	while(1)
	{
		if(time(NULL)-oldtime>30)
			break;
		memset(&pad_data,0,sizeof(pad_data));
		if((ret=NDK_KbGetPad(&pad_data,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
		NDK_ScrPutPixel(pad_data.unPadX,pad_data.unPadY, BLACK);
		NDK_ScrRefresh();
	}
#else
	cls_printf("������Ļ�ϻ���Ĺ켣,ȡ������������");
	while(1)
	{
		while(1)
		{
			memset(&pad_data,0,sizeof(pad_data));
			if((ret=NDK_KbGetPad(&pad_data,0))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				return;
			}
			if(pad_data.emPadState == PADSTATE_DOWN) 
			{
				NDK_ScrPutPixel(pad_data.unPadX,pad_data.unPadY, BLACK);
				NDK_ScrRefresh();
			}
			if(pad_data.emPadState ==PADSTATE_UP  || pad_data.emPadState == PADSTATE_KEY)
				break;
		
		}
		if (pad_data.unKeycode == ESC)
			break;
	}
#endif
	if(cls_show_msg("�㻭�Ĺ켣����ָ�ƶ��Ĺ켣,[Enter]ͨ��,[����]ʧ��")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

#if !(defined ME50||defined SP610)
	//case4:���°���ʱ�����ذ���ֵtimeout=30s
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("����10s�ڰ���%s��", table[cnt].keyname);
	memset(&pad_data,0,sizeof(pad_data));
	if ((ret=NDK_KbGetPad(&pad_data,MAXTIMEOUT))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�ж�״̬�Ƿ���ȷ,����ֵ������
	if(pad_data.emPadState != PADSTATE_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pad_data.emPadState);
		RETURN;
	}
	if (pad_data.unKeycode!=table[cnt].keycode)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pad_data.unKeycode);
		RETURN;
	}

	//case5:timeout=0 ���°������ؼ�ֵ
	cnt = rand()%(SZ_ARRAY(table));
	cls_printf("�밴��%s��", table[cnt].keyname);
	memset(&pad_data,0,sizeof(pad_data));
	if ((ret=NDK_KbGetPad(&pad_data,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�ж�״̬�Ƿ���ȷ,����ֵ������
	if(pad_data.emPadState != PADSTATE_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pad_data.emPadState);
		RETURN;
	}
	if (pad_data.unKeycode!=table[cnt].keycode)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pad_data.unKeycode);
		RETURN;
	}
#if !defined ME50S //ME50S û��F1 F2��
	//case8:timeout=0���°������ؼ�ֵ
	cls_printf("�밴��F1��");
	memset(&pad_data,0,sizeof(pad_data));
	if ((ret=NDK_KbGetPad(&pad_data,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(pad_data.emPadState != PADSTATE_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pad_data.emPadState);
		RETURN;
	}
	if (pad_data.unKeycode!=KEY_F1)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pad_data.unKeycode);
		RETURN;
	}
	
	//case9:timeout=0���°������ؼ�ֵ
	cls_printf("�밴��F2��");
	memset(&pad_data,0,sizeof(pad_data));
	if ((ret=NDK_KbGetPad(&pad_data,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(pad_data.emPadState != PADSTATE_KEY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pad_data.emPadState);
		RETURN;
	}
	if (pad_data.unKeycode!=KEY_F2)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, pad_data.unKeycode);
		RETURN;
	}
#endif
#endif
	//case6:��ʱ����
	cls_printf("����10s������Ļ�������ϲ����κζ���,�����ĵȴ�...");
	lib_StartStopwatch();
	ret=NDK_KbGetPad(&pad_data,MAXTIMEOUT);//��ʱ����NDK_ERR_TIMEOUT
	if (ret!=NDK_ERR_TIMEOUT||(tmp=fabs(lib_ReadStopwatch()-MAXWAITTIME))>WUCHASEC)
	{
#if !K21_ENABLE
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, tmp);
#else
		memset(str,0,sizeof(str));
		ftos(tmp, str);
		send_result("line %d: %s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, str);
#endif
		RETURN;
	}

#if !(defined ME50||defined SP610)
	//case7:����case6��ʱ�쳣�󣬴����Ƿ��ܹ��������� 4.1�汾�з���case6ִ�к�case7�������ѭ��
	//(������ø�����Ļ�ϣ��£����ң��У����ϣ����ϣ����£����£��Ÿ�λ��),����״ֵ̬
	cls_printf("������Ļ��,��,��,��,��,����,����,����,���¾Ÿ�λ�����ⰴ�µ���򰴼�Ӧ���Ի�ȡ����Ӧ��״̬,ȡ������������.");
	NDK_ScrGetxy(&x,&y);
	while(1)
	{
		memset(&pad_data,0,sizeof(pad_data));
		if((ret=NDK_KbGetPad(&pad_data,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
		NDK_ScrGotoxy(x, y);
		NDK_ScrPrintf("״̬:%s",type[pad_data.emPadState]);
		NDK_ScrRefresh();
			
		if (pad_data.emPadState == PADSTATE_KEY  && pad_data.unKeycode == ESC)
			break;
	}
	if(cls_show_msg("��ȡ��״̬��Ԥ��,[Enter]ͨ��,[����]ʧ��")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
#endif

	send_result("%s����ͨ��", TESTAPI);
	return;
}


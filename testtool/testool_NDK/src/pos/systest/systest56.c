/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	����/���߽���
* file name			: systest56.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: ����/���߽������
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20140220  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"����/���߽���"

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
* history 				: author			date			remarks
					 chensj			20140220	created
*****************************************************************/	
int CreatePoint(void)
{
	int start_x1_point = 0, start_y1_point = 0, ret = 0, wide = 20, height = 20;
	static int prev_x1_point , prev_y1_point = 0; //��¼��һ�εĺڿ�λ�ã���������ڿ�(Ϊ�˲���ȫ����)
	ST_PADDATA pad_data;
	uint unWidth = 0, unHeight = 0;

	NDK_ScrGetViewPort(NULL, NULL, &unWidth, &unHeight);
	//unWidth = GUI_GetLCDWidth();
	//unHeight = GUI_GetLCDHeight();
	
	while(1)
	{
		memset(&pad_data,0,sizeof(pad_data));
		if((ret=NDK_KbGetPad(&pad_data,0))!=NDK_OK)
		{
			cls_show_msg1(2, "%s,line %d����ʧ��(%d)", __FILE__,__LINE__,ret);
			return FAIL;
		}

		if(prev_x1_point!=0||prev_y1_point!=0) //�״ν��벻����
		{
			if((ret = NDK_ScrRectangle(prev_x1_point, prev_y1_point ,wide,height,RECT_PATTERNS_SOLID_FILL,0xFFFF))!=NDK_OK)
			{
				cls_show_msg1(2, "%s,line %d����ʧ��(%d)", __FILE__,__LINE__,ret);
				return FAIL;
			}
		}
		
		if (pad_data.emPadState == PADSTATE_KEY  && pad_data.unKeycode == ESC)
		{
			return ESC;
		}

		//ʹ��ȷ�ϼ��ᱻʶ��Ϊ������,�ʸĳ��˸��
		if (pad_data.emPadState == PADSTATE_KEY  && pad_data.unKeycode == BACKSPACE) 
		{
			return BACKSPACE;
		}

		if(pad_data.emPadState == PADSTATE_UP)
		{
			/*�Դ������������,��ֹ̫��Ե��ʾ�ķ�������*/
			start_x1_point = pad_data.unPadX;
			start_y1_point = pad_data.unPadY;
			
			if(unWidth - pad_data.unPadX < 10)
				start_x1_point = unWidth - 10-1;

			if(pad_data.unPadX < 10)
				start_x1_point = 10;

			if(unHeight - pad_data.unPadY < 10)
				start_y1_point = unHeight - 10-1;

			if(pad_data.unPadY < 10)
				start_y1_point = 10;
			
			if((ret = NDK_ScrRectangle(start_x1_point-10, start_y1_point-10 ,wide,height,RECT_PATTERNS_SOLID_FILL,0))!=NDK_OK)
			{
				cls_show_msg1(2, "%s,line %d����ʧ��(%d)", __FILE__,__LINE__,ret);
				return FAIL;
			}
			prev_x1_point = start_x1_point-10;
			prev_y1_point = start_y1_point-10;
			NDK_ScrRefresh();
			break;
		}
	}
	return SUCC;
}

void systest56(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	int ret = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);

	//����ǰ��:����ǰ��Ļ��ӦƯ��
	if(systest_touch()!=SUCC)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}

	cls_printf("1.���ⰴ������\n2.[�˸�]������\n3.��������\n4.[ȡ��]�˳�");
	while(1)
	{
		if((ret = CreatePoint())==ESC)
		{
			break;
		}
		else if(ret == FAIL)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
			return;
		}	
		if(ret == BACKSPACE)//ʹ��ȷ�ϼ��ᱻʶ��Ϊ������,�ʸĳ��˸��
		{
#if ANDIROD_ENABLE
			if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
#else
			if((ret=NDK_SysGoSuspend()) != NDK_OK)
#endif
			{
				cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
				return;
			}
		}
	}
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	if(cls_show_msg("����ǰ�����߻��Ѻ�[�ڿ�]��λ���Ƿ�һ��\n[ȷ��]�� [����]��")!=ENTER)
	{
		cls_show_msg1(g_keeptime,"%s ����ʧ��,���ߺ�����Ȼ�ڹ���",TESTITEM);
		return;
	}
	
	//���߻��Ѻ���Ļ��ӦƯ��
	if(systest_touch()!=SUCC)
	{
		cls_show_msg1(g_keeptime,"%s line: %d����ʧ��", TESTITEM, __LINE__);
		return;
	}
	
	cls_show_msg("%s ����ͨ��", TESTITEM );
	return;
}


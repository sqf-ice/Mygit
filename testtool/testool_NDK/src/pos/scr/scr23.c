/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: scr23.c
* Author 			: jiangym
* version			: 
* DATE			: 20130218
* directory 		: 
* description		: ���� NDK_ScrStatusbar�ܷ���ȷ������Ļ�ײ���ʾ״̬��
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScrStatusbar"
#if STABAR_LED_ENABLE		
#if OVERSEAS_ENABLE
#define	FILETEST		"typeAunMaster.NLD"
#define	TESTAppName		"typeAunMaster" 
#else
#define  FILETEST  "App_U_NonMaster.NLD" 
#define	TESTAppName		"App_U_NonMaster" 
#endif
#endif
#define     MAXWAITTIME  10//30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr23
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	     jiangym         20130224  	   created
*
*****************************************************************/
void scr23(void)
{
	/*private & local definition*/
	int ret = 0, k = 0;
	float oldtime = 0.0, diff = 0.0;
	EM_STATUSBAR displayposition = STATUSBAR_POSITION_TOP;
	uint getx = 0, gety = 0, ViewWidth = 0, ViewHeight = 0;
	
	/*process body*/
#if STABAR_LED_ENABLE		
	if(cls_show_msg1(30, "����%s...,��ȷ��%s����,ESC�˳�", TESTAPI, FILETEST)==ESC)
		return;
#else
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
#endif
	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(&getx, &gety, &ViewWidth, &ViewHeight);
	 
	//case1:�����쳣:unFlagΪ-1������涨�Ĳ����ڵ�����3��Ӧ�÷���ʧ��
	if((ret=NDK_ScrStatusbar(-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrStatusbar(3))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	for(k = 0; k < 2 ; k++)
	{
		if(k==0)
			displayposition=STATUSBAR_POSITION_TOP;//״̬����ʾ�ڶ���
		else
			displayposition=STATUSBAR_POSITION_BOTTOM;//״̬����ʾ�ڵײ�
		
		//case2:��������:����0,Ӧ�ò���ʾ״̬��
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "%s����ʾ״̬��,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		
		//case3:��������:����2,״̬��Ӧ��ֻ��ʾʱ��
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_TIME|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%sֻ��ʾʱ��,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		
		//case4:��������:����4,״̬��Ӧ��ֻ��ʾ����
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_BATTERY|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%sֻ��ʾ����,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		
		//case5:��������:����8,״̬��Ӧ��ֻ��ʾ�����ź�
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_WLSIGNAL|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%sֻ��ʾ�����ź�,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case9:��������:����0x10,״̬��Ӧ��ֻ��ʾwifi�ź�
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_WIFI|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		/* WIFI��Ҫ����������²�����ʾWIFIͼ�꣬��WIFI״̬��ʾ�����WIFI��Ԫģ�������֤
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%sֻ��ʾwifi�ź�,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}*/
#if !(defined ME50S||defined ME50NS)	
		//case10:��������:����0x20,״̬��Ӧ��ֻ��ʾMODEM״̬
		//���ڿ���δ����MODEMʱ������ʾMODEMͼ���������ܽ�����ʾͼ��
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_MODEM|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%sֻ��ʾMODEM״̬(�������δ������MDM����ʾ),��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case11:��������:����0x40,״̬��Ӧ��ֻ��ʾETH״̬
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_ETH|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%sֻ��ʾETH״̬,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
#endif
		//case:��������:����0x100,״̬����ʾ����ͼ��,����������δ����ʱ����ʾ����ͼ��,���Ծ�����ʾЧ��������ģ�������֤,����ֻҪ���Ե������쳣���سɹ�����
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_BLUETOOTH|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case14:����0x80,״̬��Ӧ����ʾ����,STATUSBAR_DISP_TIMEδ�����������Ч
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_DATE|displayposition))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%sû����ʾ����,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case12:��������:����0x80,״̬��Ӧ����ʾ����,��Ҫ��STATUSBAR_DISP_TIME�����������Ч
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_TIME|STATUSBAR_DISP_DATE|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%s��ʾ���ں�ʱ��,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case13:��ʾʱ����״̬ͼ��ʱ��ʱ�俿��,ͼ�꿿��
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_TIME|STATUSBAR_DISP_ETH|STATUSBAR_DISP_WLSIGNAL|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%s��ʾ�Ƿ�ʱ�俿��,ͼ�꿿��,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		
		//case6:��������:STATUSBAR_DISP_ALL,״̬��Ӧ����ʾ�����źš�ʱ�䡢����,״̬��Ӧ��Ĭ����ʾ
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_ALL|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%s��ʾ�����źš�ʱ�䡢����,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case15:������ʾ����ͼ��
		//cls_show_msg("0x%8x",STATUSBAR_DISP_WLSIGNAL|STATUSBAR_DISP_BATTERY|STATUSBAR_DISP_DATE|STATUSBAR_DISP_TIME|STATUSBAR_DISP_ETH|STATUSBAR_DISP_MODEM|STATUSBAR_DISP_WIFI|displayposition);
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_WLSIGNAL|STATUSBAR_DISP_BATTERY|STATUSBAR_DISP_DATE|STATUSBAR_DISP_TIME|STATUSBAR_DISP_ETH|STATUSBAR_DISP_MODEM|STATUSBAR_DISP_WIFI|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//����ʾ״̬ͼ����ʱ���ͻʱ������ʱ�䣬������ʾͼ��  (��֤�����Ƿ�������)
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%s�Ƿ�������ʾ״̬ͼ��,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case7:���Խ�����ƶ���״̬�����귶Χ�ڣ�Ӧ�ò�����״̬����Χ����ʾ��Ϣ
		cls_printf("״̬����Χ��Ӧ����ʾ������Ϣ");
		oldtime = time(NULL);
		while((diff=time(NULL)-oldtime)<MAXWAITTIME && (lib_kbhit()==0))//��30��ʱ�䷶Χ��
		{	
			if(displayposition==STATUSBAR_POSITION_BOTTOM)//״̬���ڵײ�
				NDK_ScrDispString(0,ViewHeight-GUI_GetFontHeight(),"����Ϣ��Ӧ����ʾ��״̬����Χ��",0);
			else
				NDK_ScrDispString(0,0,"����Ϣ��Ӧ����ʾ��״̬����Χ��",0);
			show_stopwatch(ENABLE, diff);//����ʱ
		}
		show_stopwatch(DISABLE, 0);
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����Ӧ�ó����쳣��Ϣ,��[ȷ��]����ͨ��,[����]ʧ��")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		
		//case8:��֤״̬���رպ�ԭ״̬��λ�ÿ���������ʾ������Ϣ
		cls_show_msg1(30, "����������ر�״̬����ȫ����ʾ�ڵ㣬����ԭ״̬��λ��");
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_ScrRefresh();
		NDK_KbGetCode(1,NULL);
		if((ret=NDK_ScrRectangle(0,0,GUI_GetLCDWidth(),GUI_GetLCDHeight(),RECT_PATTERNS_SOLID_FILL,BLACK))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI,ret);
			GOTOERR;
		}
		NDK_ScrRefresh();
		NDK_KbGetCode(3,NULL);
		if(ENTER != cls_show_msg1(MAXWAITTIME, "ȫ���ڵ��޿հ�.��ȷ��[ENTER].������ʧ��")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		//case16:ģ��led�ƶ�״̬��ͼ���Ӱ��20180416 linying added
#if STABAR_LED_ENABLE		
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_LED|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%s��������ʾģ��LED���ĸ�����,�ǰ�[Enter],��[����]", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case16.1��֤��Ӧ���ڿ���ģ��LED�������ִ����Ӧ�ã���Ӧ���в�Ӧ�ó���ģ��led����˸
		if(( ret = NDK_AppLoad((const char *)FILETEST,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(( ret = NDK_AppRun((const char *)TESTAppName))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "��Ӧ����û����ʾ״̬����ģ��LED�Ʒ���,�ǰ�[Enter],��[����]")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case16.2:ģ��led�ƺ͵���(����ͻͼ��)ͬʱ����
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_LED|STATUSBAR_DISP_BATTERY|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%s��������ʾģ��LED���ĸ�����͵���ͼ��,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		
		//case16.3:ģ��led�ƺ�ʱ��ͼ��(��ͻͼ��)������ʾLED��
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_LED|STATUSBAR_DISP_TIME|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%s��������ʾģ��LED���ĸ�����,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}

		//case16.4 �ر�ģ��led״̬�����ٿ���״̬����ʾʱ������ڣ�Ӧ����������ʾ
		NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
		if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_TIME|STATUSBAR_DISP_DATE|displayposition))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(ENTER != cls_show_msg1(MAXWAITTIME, "״̬����%s��ʾ���ں�ʱ��,��[ȷ��]����ͨ��,[����]ʧ��", (displayposition==0)?"����":"�ײ�")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}	
#endif
	}
	send_result("%s����ͨ��", TESTAPI);
//�ָ�Ĭ������
	goto ERR;
ERR:
	if(gety!=0)//y�����0˵��״̬���ڶ�����
		NDK_ScrStatusbar(STATUSBAR_DISP_ALL);
	else
		NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
	NDK_ScrFullRefresh();
#if STABAR_LED_ENABLE			
	NDK_AppDel(TESTAppName);
#endif
	return;
}

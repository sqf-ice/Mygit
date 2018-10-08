/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 19.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrDispString�Ƿ���ָ����������ʾ�ַ���
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrDispString"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr19
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr19(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	uint  x = 0, y = 0;
	const char pszS1[] = "NEWLAND\r\nPOS";	
	const char pszS2[] = "�´�½��������:(��)(��)\r\nPOS";//20130925 ����GB2312�ֿ���Ƨ�֣��� �� GB18030�ֿ���Ƨ�֣��� ����ʾ����
#if !K21_ENABLE
	uint  puncd = 0;
	const char str[] = "newland����";
	char stroverbuf[8192] = {0};
#else
	char stroverbuf[4096] = {0};//K21�ײ��ջ��С
#endif
	uint ViewWidth = 0, ViewHeight = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	
	//case1:�����쳣:x,yΪ-1;unModeΪ-1,2
	if((ret=NDK_ScrDispString(-1,y,pszS1,0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	NDK_ScrRefresh();
	NDK_SysDelay(10);
	if((ret=NDK_ScrDispString(x,-1,pszS1,0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(10);
	if((ret=NDK_ScrDispString(x,y,pszS1,-1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(10);
#if !K21_ENABLE //K21��֧��unMode����Ϊ1������Ӧ�÷��ز�������
	if((ret=NDK_ScrDispString(x,y,pszS1,2))!=NDK_ERR_PARA)
#else
	if((ret=NDK_ScrDispString(x,y,pszS1,1))!=NDK_ERR_PARA)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(10);

	//case11:��֤�����ַ������ڵײ㻺�������,����Ӧ���ܷ�,��ʾ���岿��,�������ֶ��� 20131128 linwl
	memset(stroverbuf,'A',sizeof(stroverbuf)-1);
	if((ret=NDK_ScrDispString(0,0,stroverbuf,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(20);
	if(cls_show_msg1(MAXWAITTIME, "ȫ��Ļ��ʾ�ַ�A,��ȷ��;����������.")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case2:�����ʾ�ַ���ΪNULL
	NDK_ScrClrs(); 
	if((ret=NDK_ScrDispString(x,y,NULL,0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(20);//�ȴ���ȷ���Ƿ�����Ļ���
	if(cls_show_msg1(MAXWAITTIME, "����Ļû�г����ַ���,��ȷ��;����������.")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case3:�����ʾ����Խ��Ĳ���
	NDK_ScrClrs(); 
	if((ret=NDK_ScrDispString(ViewWidth,ViewHeight,pszS1,0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(20);
	if(cls_show_msg1(MAXWAITTIME, "����Ļû�г���\"%s\",��[ȷ��];����������.", pszS1)!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ
	
	NDK_ScrClrs(); 
	if((ret=NDK_ScrDispString(ViewWidth,ViewHeight,pszS2,0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(20);//sleep(2);
	if(cls_show_msg1(MAXWAITTIME, "����Ļû�г���\"%s\",��[ȷ��];����������.", pszS2)!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ

#if !K21_ENABLE		//K21��֧��unMode����Ϊ1�������������������
	//case4:unMode����Ϊ1
	NDK_ScrClrs(); 
	if((ret=NDK_ScrDispString(x,y,pszS1,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(20);//�ȴ���ȷ���Ƿ�����Ļ���
	if(cls_show_msg1(MAXWAITTIME, "����Ļ����\"%s\",��[ȷ��];����������.", pszS1)!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ

	//case5:�����������
	if((ret=NDK_ScrGetColorDepth(&puncd))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	NDK_ScrClrs(); 
	if((ret=NDK_ScrDispString(x,y,str,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(20);//�ȴ���ȷ���Ƿ�����Ļ���

	if(puncd==1||puncd==4) //����ģʽΪ1��ʱ�����������
	{	
		if(cls_show_msg1(MAXWAITTIME, "����Ļû�г�������\"����\",��[ȷ��];����������.")!=ENTER&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}
	}
	else //�����������
	{	
		if(cls_show_msg1(MAXWAITTIME, "����Ļ����%s,��[ȷ��];����������.",str)!=ENTER&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ
#endif

	//��Ӧ�ߵ�״̬��(730׷�Ӳ���)
#if 0//STATUSBAR_ENABLE
	NDK_ScrClrs(); 
	if((ret=NDK_ScrDispString(0,GUI_GetLCDHeight(),pszS1,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(20);//sleep(2);
	if(cls_show_msg1(MAXWAITTIME, "��״̬������\"%s\",��ȷ��;����������.", str)==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}	
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ
#endif
	
	//case6:unMode����Ϊ0
	NDK_ScrClrs(); 
	if((ret=NDK_ScrDispString(x,y,pszS2,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(10);//�ȴ���ȷ���Ƿ�����Ļ���
	if(cls_show_msg1(MAXWAITTIME, "����Ļ����\"%s\",��[ȷ��];����������.", pszS2)!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case7:POS�ϵ�\r�ǻس�Ч��,\n�ǻس������е�Ч��
	NDK_ScrClrs();
	if((ret=NDK_ScrDispString(ViewWidth/2,0,"\r",0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(10);//�ȴ���ȷ���Ƿ�����Ļ���
	if(show_msg1(MAXWAITTIME, "�������ڵ�һ�ж�����ʾ,��[ȷ��];����������.")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	NDK_ScrClrs();
	if((ret=NDK_ScrDispString(ViewWidth/2,0,"\n",0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(10);//�ȴ���ȷ���Ƿ�����Ļ���
	if(show_msg1(MAXWAITTIME, "�������ڵڶ��ж�����ʾ,��[ȷ��];����������.")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ 

	//case8:�����ڱ߽�λ�ò�Ӧ����ʾ�������
	NDK_ScrClrs();
	NDK_ScrDispString(ViewWidth-(GUI_GetFontWidth()/2),0,"��",0);
	NDK_ScrDispString(0,ViewHeight-(GUI_GetFontHeight()/2),"��",0);
	NDK_ScrRefresh();
	NDK_SysDelay(10);//�ȴ���ȷ���Ƿ�����Ļ���
	if(cls_show_msg1(MAXWAITTIME, "��Ļ�����ְ������,��[ȷ��];����������.")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ 
	
	//case 9:�����ڷ�˫�ַ�λ����ʾӦ����
	NDK_ScrClrs();
	NDK_ScrDispString(8,0,"����Ϣ��ʼλ��(8,0)��ʼ��ʾ",0);
	if(show_msg1(MAXWAITTIME, ",��[ȷ��];����������.")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");
	
	//case10:��������ַ�
	lib_kbflush();		//�������������
	lib_StartStopwatch();//Ϊʵ���Զ������ԣ�����������
	while ((nKeyin=lib_kbhit())==0)
	{
		x = rand()%(ViewWidth-(GUI_GetFontWidth()/2));
		y = rand()%(ViewHeight-GUI_GetFontHeight());
		cls_show_msg1(1, "����(%d, %d)��ʾ\"%s\".��������,�ɰ�ENTER;���쳣,�밴������", x, y, pszS2);
		NDK_ScrClrs(); 
		if((ret=NDK_ScrDispString(x,y,pszS2,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysBeep();
		NDK_ScrRefresh();
		NDK_SysDelay(20);
		if (30<lib_ReadStopwatch())
			break;
	}
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);

	return;
}


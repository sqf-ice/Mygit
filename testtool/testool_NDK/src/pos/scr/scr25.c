/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: scr25.c
* Author 			: chensj
* version			: 
* DATE			: 20130722
* directory 		: 
* description		: ���� NDK_ScrPrintf�ӿڹ���
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		 "NDK_ScrPrintf"
#define	MAXWAITTIME	10//30.0

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr25
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	   chensj          20130722  	     created
*
*****************************************************************/
void scr25(void)
{
	/*private & local definition*/
	int ret = 0, i = 999;
	char c = 'a';
	char temp[4] = {0};
	char str1[] = {
		"���Զ����̢��ϻ�"
	   "�������ܼ���¼"
	   "�ݳ��������Ӳ��"
	   "�߰汾����������"
	};
#if defined ME68 //ME68�Ĵ�С���Ʋ��ܳ���511�ֽڣ��������� 20180416 sull modify
	char BigStr[511+1] = {0};
#else
	char BigStr[512+1] = {0};
#endif
	float f = 0.0;
	char *p=NULL;
	uint ViewWidth=0 ,ViewHeight=0;
#if K21_ENABLE
	char str[32] = {0};
#endif		
	/*process body*/
	cls_show_msg1(2,"����%s...", TESTAPI); 

	//case 1:��ʽ�����%s������ֵ,��Ӧ����ʾ�������
	if((ret = NDK_ScrPrintf("\n����ֵ��%s"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	sleep(1);
	if((ret = NDK_ScrPrintf("\n����ָ��%s",p))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	sleep(1);
	if((ret = NDK_ScrPrintf("\nֱ�Ӹ�ֵ��%s",NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	sleep(1);
	
	//case 2:�쳣��������
	if((ret = NDK_ScrPrintf(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case 3:�����ڱ߽�λ�ò�Ӧ����ʾ�������
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	NDK_ScrClrs();
	NDK_ScrGotoxy(ViewWidth-(GUI_GetFontWidth()/2),0);
	NDK_ScrPrintf("��");
	NDK_ScrGotoxy(0,ViewHeight-(GUI_GetFontHeight()/2));
	NDK_ScrPrintf("��");
	NDK_ScrRefresh();
	NDK_SysDelay(10);//�ȴ���ȷ���Ƿ�����Ļ���
	if(cls_show_msg1(MAXWAITTIME, "��Ļ�����ְ������,��[ȷ��];����������.")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	cls_show_msg1(2, "����������ͨ��!");//��Ļ�б仯,��������֮�����Ļ��ʾ 

	//case 4:mNDK�ĵ���˵��,���ME30�ַ������ܳ���511�ֽ�,�����Ӵ���������511�ֽڲ�Ӧ�ܷ� 20140219����,������ƷҲ��Ӧ���ܷ�
	NDK_ScrClrs();
	memset(BigStr,'a',sizeof(BigStr)-1);
	if((ret = NDK_ScrPrintf("%s", BigStr))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	sleep(3);
	if(cls_show_msg1(MAXWAITTIME, "��Ļ��ʾСa? \n[ȷ��]��  [����]��")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case 6:�����ַ�(�������з�)�����ʾ�󳬹�Һ������Ƿ�ỻ��//���ݹ��̲�����20180521 linying added
	NDK_ScrClrs();
	for (i=0; i<strlen(str1); i += 2) {
		temp[0] = str1[i];
		temp[1] = str1[i + 1];
		NDK_ScrPrintf("%s", temp);
	}
	NDK_ScrRefresh();
	sleep(3);
	if(cls_show_msg1(MAXWAITTIME, "�Ƿ�������ʾ���в˵���? \n[ȷ��]��  [����]��")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case7:���Զ���һ������ʾʱ���Ƿ�ỻ��//20180521 linying added
	NDK_ScrClrs();
	NDK_ScrPrintf("%s", str1);
	NDK_ScrRefresh();
	sleep(3);
	if(cls_show_msg1(MAXWAITTIME, "�Ƿ�������ʾ���в˵���? \n[ȷ��]��  [����]��")!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case 5:������������
	NDK_ScrClrs();
#if !K21_ENABLE
	if((ret = NDK_ScrPrintf("���������ַ���:%s;����:%d;�ַ�:%c;����:%f\n", "AB��", i, c, f))!=NDK_OK)
#else
	memset(str,0,sizeof(str));
	ftos(f,str);
	if((ret = NDK_ScrPrintf("���������ַ���:%s;����:%d;�ַ�:%c;����:%s\n", "AB��", i, c, str))!=NDK_OK)
#endif
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//���Խ���
	ret = show_msg1(MAXWAITTIME, "[ENTER]ͨ��,[����]��ͨ��");
	if (ret==ENTER || ret==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	return;
}


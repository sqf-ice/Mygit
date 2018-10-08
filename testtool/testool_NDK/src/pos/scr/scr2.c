/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 2.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrSetAttr�Ƿ������ȷȡԭ������ʾ���ԣ��������µ���ʾ����
* related document	: NDK.chm
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	MAXWAITTIME 10//30
#define	TESTAPI		"NDK_ScrSetAttr"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr2
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr2(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	EM_TEXT_ATTRIBUTE OldAttr[3] = {0};
	
	/*process body*/
	nKeyin = cls_show_msg1(MAXWAITTIME,"����%s.��ѡ����ʾ����:1.����,2.��չ", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	switch(nKeyin)
	{
		case '1':
			NDK_ScrSetFontType(DISPFONT_CUSTOM);
			break;
		case '2':
			NDK_ScrSetFontType(DISPFONT_EXTRA);
			break;
		default:
			NDK_ScrSetFontType(DISPFONT_CUSTOM);
			break;
	}
	
	//����ǰ�ã�����������ʾģʽ������ȡԭ������ʾģʽӦ�óɹ�
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_NORMAL,OldAttr))!=NDK_OK)//Ԥ��Ϊ����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME,"�ı���ʾ����Ϊ����ģʽ,��ȷ�ϲ���ͨ����������ʧ��\n");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case1:�������úϷ���ʾģʽ,�᷵�ؾɵ���ʾģʽ(3��ģʽ)
	//����ģʽ
	NDK_ScrClrs();
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE,OldAttr+1))!=NDK_OK)//Ԥ��Ϊ����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin = show_msg1(MAXWAITTIME,"�ı���ʾ����Ϊ����ģʽ,��ȷ�ϲ���ͨ����������ʧ��\n");
	if((OldAttr[1]!=TEXT_ATTRIBUTE_NORMAL)||(ENTER!=nKeyin&&auto_flag!=1))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, OldAttr[1]);
		GOTOERR;
	}
	
	//�»���ģʽ
#if !K21_ENABLE //K21ƽ̨��֧���»��߹���
	NDK_ScrClrs();
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE,OldAttr+1))!=NDK_OK)//Ԥ��Ϊ����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_UNDERLINE,OldAttr+1))!=NDK_OK)//Ԥ��Ϊ������»���
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin = show_msg1(MAXWAITTIME,"�ı���ʾ����Ϊ���Դ��»���ģʽ,��ȷ�ϲ���ͨ����������ʧ��\n");
	if((OldAttr[1]!=TEXT_ATTRIBUTE_REVERSE)||(ENTER!=nKeyin&&auto_flag!=1))
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, OldAttr[1]);
		GOTOERR;
	}
#endif
	//����ģʽ
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_NORMAL,OldAttr+1))!=NDK_OK)//Ԥ��Ϊ����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin = show_msg1(MAXWAITTIME,"�ı���ʾ����Ϊ����ģʽ,��ȷ�ϲ���ͨ����������ʧ��\n");
#if !K21_ENABLE
	if((OldAttr[1]!=(TEXT_ATTRIBUTE_UNDERLINE|TEXT_ATTRIBUTE_REVERSE))||(ENTER!=nKeyin&&auto_flag!=1))
#else
	if((OldAttr[1]!=(TEXT_ATTRIBUTE_REVERSE))||(ENTER!=nKeyin&&auto_flag!=1))
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, OldAttr[1]);
		GOTOERR;
	}
	
	//case2:����ԭģʽΪNULLʱ��Ӧ������ֵ
#if !K21_ENABLE
	NDK_ScrClrs();
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_UNDERLINE,NULL))!=NDK_OK)//ԭģʽΪNULL
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin = show_msg1(MAXWAITTIME,"�ı���ʾ����Ϊ�»���ģʽ,��ȷ�ϲ���ͨ����������ʧ��");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
#endif
	//case3:���ԷǷ�����-1,Ӧ����ʧ��
	if((ret=NDK_ScrSetAttr(-1,OldAttr+1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:����NDK_ScrClrs��ָ�TEXT_ATTRIBUTE_NORMALģʽ
	NDK_ScrClrs();//ע��:NDK_ScrClrs�󣬻�ָ�TEXT_ATTRIBUTE_NORMAL���Ұѹ���Ƶ�(0,0)�����Ա���Ҫ����set֮ǰ
	if((ret=NDK_ScrSetAttr(TEXT_ATTRIBUTE_REVERSE,OldAttr+2))!=NDK_OK)//Ԥ��Ϊ����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(OldAttr[2]!=TEXT_ATTRIBUTE_NORMAL)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, OldAttr[2]);
		goto ERR;
	}
	nKeyin = show_msg1(MAXWAITTIME,"�ı���ʾ����Ϊ����ģʽ,��ȷ�ϲ���ͨ����������ʧ��\n");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	NDK_ScrSetAttr(OldAttr[2],OldAttr+1);
	send_result("%s����ͨ��", TESTAPI);
	//�ָ�Ĭ��ֵ
ERR:
	NDK_ScrSetFontType(DISPFONT_CUSTOM);
	NDK_ScrSetAttr(OldAttr[0],OldAttr+1);
	return;
}


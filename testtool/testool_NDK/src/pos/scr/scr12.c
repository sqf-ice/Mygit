/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 12.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		:  ����NDK_ScrAutoUpdate�ܷ�������Ļˢ��ģʽ������NDK_ScrFullRefresh�ܷ�����ȫ��Ļˢ��ģʽ,����NDK_ScrRefresh�ܷ�ˢ����Ļ
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrAutoUpdate,NDK_ScrFullRefresh,NDK_ScrRefresh"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr12
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr12(void)
{
	/*private & local definition*/
	int  ret = 0;
	int pnOldauto = 0, pnOldauto_set = 0; 
	uint punX = 0, punY = 0, punWidth = 0, punHeight = 0;

	/*process body*/
	cls_show_msg1(2, "����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ�ã���ȡ��ǰ�������úõ���ʾ�����ˢ��ģʽ
	NDK_ScrGetViewPort(&punX,&punY,&punWidth,&punHeight);
	if((ret=NDK_ScrAutoUpdate(0,&pnOldauto_set))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	NDK_ScrClrs();//������ʾ��Ҫ��������
	//case1:��0ֵ�Ĳ��ԣ�Ӧ�ñ���ÿ����ĻAPI���ἴʱ��ʾ
	if((ret=NDK_ScrAutoUpdate(-1,&pnOldauto))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrPrintf("��Ļֱ����ʾ����Ϣ,��ȷ�ϲ���ͨ��,������ʧ��");
	if(lib_getkeycode(5)!=ENTER&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	NDK_ScrClrs();

	if(auto_flag==0||auto_flag==2)
	{
		//case2:0ֵ�Ĳ��ԣ����Զ�ˢ�£�ֻ�е���NDK_ScrRefresh()����ʾ�Դ��е�����
		NDK_ScrPrintf("�����������Ļ������ʾ��һ����Ϣ");//�Զ�ˢ����ʾ����Ϣ���޸ĳɲ��Զ�ˢ��
		if((ret=NDK_ScrAutoUpdate(0,&pnOldauto))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_ScrClrs();
		NDK_ScrPrintf("�����������Ļ��ʾ����Ϣ,");
		lib_getkeycode(0);//������ˢ��
		NDK_ScrRefresh();
		if(ENTER != show_msg1(MAXWAITTIME,"��ȷ�ϲ���ͨ��,������ʧ��")&&auto_flag!=1)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
		NDK_ScrClrs();
	}
	
	//case3:����NDK_ScrAutoUpdate���óɹ��󷵻�ԭ����ģʽ
	if((ret=NDK_ScrAutoUpdate(0,&pnOldauto))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrAutoUpdate(1,&pnOldauto))!=NDK_OK || pnOldauto!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pnOldauto);
		GOTOERR;
	}
	if((ret=NDK_ScrAutoUpdate(0,&pnOldauto))!=NDK_OK || pnOldauto!=1)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pnOldauto);
		GOTOERR;
	}

	//case4:���ó�0��Ҫ����update_viewport�����Ŷ���ʾ���ݽ��и���
	NDK_ScrAutoUpdate(0,&pnOldauto);
	cls_show_msg1(5, "3��󽫺���,6�����ְ�ɫ����");
	NDK_ScrRectangle(0,0,punWidth,punHeight,RECT_PATTERNS_SOLID_FILL,BLACK);	
	if((ret=NDK_ScrSetViewPort(10+punX,10+punY,punWidth-20,punHeight-20))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysDelay(30);
	NDK_ScrFullRefresh();
	NDK_ScrRectangle(0,0,punWidth-20,punHeight-20,RECT_PATTERNS_SOLID_FILL,WHITE);
	NDK_SysDelay(30);
	NDK_ScrRefresh();
	NDK_SysDelay(30);
	if(ENTER != cls_show_msg1(MAXWAITTIME,"��ȷ�ϲ���ͨ����������ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	send_result("%s����ͨ��", TESTAPI);

	//�ָ�Ĭ������
ERR:
	NDK_ScrSetViewPort(punX,punY,punWidth,punHeight);
	NDK_ScrAutoUpdate(pnOldauto_set,&pnOldauto);
	return;
}


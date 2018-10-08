/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 21.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrDispPic�ܷ�����Ļ����ʾͼƬ
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrDispPic"
#define SHUTDOWNPIC "shutdown.jpg"	//���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ�������� 20160513
#define ERRPIC "err.jpg"
#define BIGPIC "big.jpg"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr21
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr21(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	uint ViewWidth = 0, ViewHeight = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	
	//����ǰ��
	if(NDK_FsExist(SHUTDOWNPIC)!=NDK_OK&&NDK_FsExist(BIGPIC)!=NDK_OK)
	{
		send_result("%s����ͼƬδ����,����POS�Ĳ��Գ���·��Ŀ¼����shutdown,big��jpgͼƬ�����²���", TESTAPI);
		return;
	}
	
	//case1:ͼƬ·��������
	if((ret=NDK_ScrDispPic(0,0,GUI_GetLCDWidth(),GUI_GetLCDHeight(),ERRPIC))!=NDK_ERR_PATH)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	
	//case2:ͼƬ·��ΪNULL
	if((ret=NDK_ScrDispPic(0,0,GUI_GetLCDWidth(),GUI_GetLCDHeight(),NULL))!=NDK_ERR_DECODE_IMAGE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	
	//case3:�����쳣��unX,unY,unWidth,unHeight�ֱ�Ϊ-1,>��Ļ,unX+unWidth>��Ļ,unY+unHeight>��Ļ
	if((ret=NDK_ScrDispPic(-1,0,GUI_GetLCDWidth(),GUI_GetLCDHeight(),SHUTDOWNPIC))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if((ret=NDK_ScrDispPic(0,-1,GUI_GetLCDWidth(),GUI_GetLCDHeight(),SHUTDOWNPIC))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if((ret=NDK_ScrDispPic(0,0,-1,GUI_GetLCDHeight(),SHUTDOWNPIC))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if((ret=NDK_ScrDispPic(0,0,GUI_GetLCDWidth(),-1,SHUTDOWNPIC))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if((ret=NDK_ScrDispPic(0,0,GUI_GetLCDWidth()+1,GUI_GetLCDHeight(),SHUTDOWNPIC))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispPic(0,0,GUI_GetLCDWidth(),GUI_GetLCDHeight()+1,SHUTDOWNPIC))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispPic(10,0,GUI_GetLCDWidth()-9,GUI_GetLCDHeight(),SHUTDOWNPIC))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispPic(0,10,GUI_GetLCDWidth(),GUI_GetLCDHeight()-9,SHUTDOWNPIC))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(10);
	NDK_ScrClrs();
	
	//case4:����NDK_ScrDispPicȫ��Ļ��ʾͼƬ
	if((ret=NDK_ScrDispPic(0,0,ViewWidth,ViewHeight,BIGPIC))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(30);	
	if(ENTER != cls_show_msg1(MAXWAITTIME,"����ʾ����ͼƬ,��ȷ�ϲ���ͨ����������ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//NDK_ScrClrs();

	//case4:����NDK_ScrDispPicȫ��Ļ��ʾͼƬ
	if((ret=NDK_ScrDispPic(0,0,ViewWidth,ViewHeight,SHUTDOWNPIC))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(30);
	nKeyin = cls_show_msg1(MAXWAITTIME, "��ȷ�ϲ���ͨ��,������ʧ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	return;
}


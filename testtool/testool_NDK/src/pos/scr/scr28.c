/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 28.c
* Author 			: linwl
* version			: 
* DATE			: 20141208
* directory 		: 
* description		: ����NDK_ScrDispImg�ܷ�����Ļ����ʾͼƬ
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrDispImg"
#define BIGPIC "big.png"	//���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ�������� 20160513
#define ERRPIC "err.jpg"

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
*			  	  	        linwl         20141208  	   created
*
*****************************************************************/
void scr28(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	uint ViewWidth = 0, ViewHeight = 0;
	char *imgbuf1 = NULL;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	
	//����ǰ��
	if(NDK_FsExist(BIGPIC)!=NDK_OK)
	{
		send_result("%s����ͼƬ������,����POS�Ĳ��Գ���·��Ŀ¼���ز���ͼƬ���²���", TESTAPI);
		return;
	}
	
	//case1:ͼƬδ������ȷ����,������ʾ����Ӧ��ʧ��
	if((ret=NDK_ScrImgDecode(ERRPIC,&imgbuf1))!=NDK_ERR_PATH)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImg(0,0,GUI_GetLCDWidth(),GUI_GetLCDHeight(),imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	
	//case2:ͼƬ��������ΪNULL,������ʾ����Ӧ��ʧ��
	if((ret=NDK_ScrDispImg(0,0,GUI_GetLCDWidth(),GUI_GetLCDHeight(),NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	
	//case3:�����쳣��unX,unY,unWidth,unHeight�ֱ�Ϊ-1,>��Ļ,unX+unWidth>��Ļ,unY+unHeight>��Ļ
	if((ret=NDK_ScrImgDecode(BIGPIC,&imgbuf1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispImg(-1,0,GUI_GetLCDWidth(),GUI_GetLCDHeight(),imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_ScrDispImg(0,-1,GUI_GetLCDWidth(),GUI_GetLCDHeight(),imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_ScrDispImg(0,0,-1,GUI_GetLCDHeight(),imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_ScrDispImg(0,0,GUI_GetLCDWidth(),-1,imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_ScrDispImg(0,0,GUI_GetLCDWidth()+1,GUI_GetLCDHeight(),imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispImg(0,0,GUI_GetLCDWidth(),GUI_GetLCDHeight()+1,imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispImg(10,0,GUI_GetLCDWidth()-9,GUI_GetLCDHeight(),imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispImg(0,10,GUI_GetLCDWidth(),GUI_GetLCDHeight()-9,imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(10);
	NDK_ScrClrs();
	
	//case4:��������
	if((ret=NDK_ScrImgDecode(BIGPIC,&imgbuf1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispImg(0,0,ViewWidth,ViewHeight,imgbuf1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(30);
	nKeyin = cls_show_msg1(MAXWAITTIME,"����ʾbigͼƬ,��ȷ�ϲ���ͨ����������ʧ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);

ERR:
	NDK_ScrImgDestroy(imgbuf1);
	return;
}


/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 27.c
* Author 			: linwl
* version			: 
* DATE			: 20141208
* directory 		: 
* description		: ����NDK_ScrImgDecode�ܷ�ͼƬ�������ݱ��浽ָ������,NDK_ScrImgDestroy�����ܷ��ͷ�ָ����ͼƬ����
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 10//30
#define 	TESTAPI		"NDK_ScrImgDecode,NDK_ScrImgDestroy"
#define SHUTDOWNPIC "shutdown.jpg"	//���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ�������� 20160513
#define BIGPIC "big.png"
#define BMPPIC "desktop.bmp"
#define LOGPIC "log.bmp"
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
*****************************************************************/
void scr27(void)
{
	/*private & local definition*/
	int ret = 0;
	uint ViewWidth = 0, ViewHeight = 0;
	char *imgbuf1 = NULL, *imgbuf2 = NULL, *imgbuf3 = NULL;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	
	//����ǰ��
	if(NDK_FsExist(SHUTDOWNPIC)!=NDK_OK&&NDK_FsExist(BIGPIC)!=NDK_OK&&NDK_FsExist(BMPPIC)!=NDK_OK&&NDK_FsExist(LOGPIC)!=NDK_OK)
	{
		send_result("%s����ͼƬ������,����POS�Ĳ��Գ���·��Ŀ¼���ز���ͼƬ���²���", TESTAPI);
		return;
	}
	
	//case1:ͼƬ·��������,ͼƬ����δ�����ɹ�ʱ,
	if((ret=NDK_ScrImgDecode(ERRPIC,&imgbuf1))!=NDK_ERR_PATH)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrImgDestroy(imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:ͼƬ·���ͻ������ΪNULL
	if((ret=NDK_ScrImgDecode(NULL,&imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrImgDestroy(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:��������jpg��ʽͼƬ����Ӧ�óɹ�,���ܹ�������ʾ
	if((ret=NDK_ScrImgDecode(SHUTDOWNPIC,&imgbuf1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrDispImg(0,0,ViewWidth,ViewHeight,imgbuf1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(30);	
	if(ENTER != cls_show_msg1(MAXWAITTIME,"����ʾshutdownͼƬ,��ȷ�ϲ���ͨ����������ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case4:��������png��ʽͼƬ����Ӧ�óɹ�,���ܹ�������ʾ
	if((ret=NDK_ScrImgDecode(BIGPIC,&imgbuf2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if((ret=NDK_ScrDispImg(0,0,ViewWidth,ViewHeight,imgbuf2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(30);	
	if(ENTER != cls_show_msg1(MAXWAITTIME,"����ʾbigͼƬ,��ȷ�ϲ���ͨ����������ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR2;
	}
	
	//case7:��������bmp��ʽͼƬ����Ӧ�óɹ�,���ܹ�������ʾ
	if((ret=NDK_ScrImgDecode(LOGPIC,&imgbuf3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR3;
	}
	if((ret=NDK_ScrDispImg(0,0,ViewWidth,ViewHeight,imgbuf3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR3;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(30);	
	if(ENTER != cls_show_msg1(MAXWAITTIME,"����ʾLOGͼƬ,��ȷ�ϲ���ͨ����������ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR3;
	}

	//case5:��������bmp��ʽͼƬ����Ӧ�óɹ�,���ܹ�������ʾ
	if((ret=NDK_ScrImgDecode(BMPPIC,&imgbuf3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR3;
	}
	if((ret=NDK_ScrDispImg(0,0,ViewWidth,ViewHeight,imgbuf3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR3;
	}
	NDK_ScrRefresh();
	NDK_SysDelay(30);	
	if(ENTER != cls_show_msg1(MAXWAITTIME,"����ʾbmpͼƬ,��ȷ�ϲ���ͨ����������ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR3;
	}
	
	//case6:�����ͷ�ָ����ͼƬ����Ӧ�óɹ�,�ҵ�����ʾӦ��ʧ��
	if((ret=NDK_ScrImgDestroy(imgbuf3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR3;
	}
	imgbuf3=NULL;//�ͷ���Դ���轫ָ��ָNULL,��������ܷ�
	if((ret=NDK_ScrDispImg(0,0,ViewWidth,ViewHeight,imgbuf3))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if((ret=NDK_ScrImgDestroy(imgbuf2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	imgbuf2=NULL;
	if((ret=NDK_ScrDispImg(0,0,ViewWidth,ViewHeight,imgbuf2))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScrImgDestroy(imgbuf1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	imgbuf1=NULL;
	if((ret=NDK_ScrDispImg(0,0,ViewWidth,ViewHeight,imgbuf1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	send_result("%s����ͨ��", TESTAPI);
	return;

	goto ERR3;
ERR3:
	NDK_ScrImgDestroy(imgbuf3);
	imgbuf3=NULL;
	goto ERR2;
ERR2:
	NDK_ScrImgDestroy(imgbuf2);
	imgbuf2=NULL;
	goto ERR;
ERR:
	NDK_ScrImgDestroy(imgbuf1);
	imgbuf1=NULL;
	return;
}


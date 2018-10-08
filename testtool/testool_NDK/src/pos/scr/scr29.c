/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 29.c
* Author 			: linwl
* version			: 
* DATE			: 20141208
* directory 		: 
* description		: ����NDK_ScrGetFontBuf�ܷ���������ַ���ȡ��ϵͳ��ǰ������Ӧ�ĵ��󻺳�
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 30
#define 	TESTAPI		"NDK_ScrGetFontBuf"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/
typedef struct {
	unsigned int width;  			/**< ͼ���ȣ����أ� */
	unsigned int height; 			/**< ͼ��߶ȣ����أ�*/
	unsigned int bytes_per_pixel; /**< ÿ�����ض����ֽ� */
	void * image_buf;		/**< ͼ��ɫ������ */
	void * noused;
}disp_test;

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
void scr29(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	uint ViewWidth = 0, ViewHeight = 0, unBuf_x = 0, unBuf_y = 0;
	char str[]="��ӭʹ��:1234590!@#()*&";
	unsigned short img[320*240*2] = {0};
	disp_test * dispbuf = {0};
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	//����ǰ��:��ʼ�����Ի���
	memset(img,0xff,ViewWidth*ViewHeight*2);
	
	//case1:����������ΪNULL����£�Ӧ�÷��ز�������
	if((ret=NDK_ScrGetFontBuf(NULL, img, unBuf_x, unBuf_y, ViewWidth, ViewHeight, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetFontBuf(str, NULL, unBuf_x, unBuf_y, ViewWidth, ViewHeight, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//X>=W��Y>=H�����Ӧ�÷���NDK_ERR_OVERFLOW
	if((ret=NDK_ScrGetFontBuf(str, img, ViewWidth, unBuf_y, ViewWidth, ViewHeight, 0))!=NDK_ERR_OVERFLOW)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetFontBuf(str, img, unBuf_x, ViewHeight, ViewWidth, ViewHeight, 0))!=NDK_ERR_OVERFLOW)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//W,HΪ0�����Ӧ�÷��ز�������
	if((ret=NDK_ScrGetFontBuf(str, img, unBuf_x, unBuf_y, 0, ViewHeight, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrGetFontBuf(str, img, unBuf_x, unBuf_y, ViewWidth, 0, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:ͼƬ��������ΪNULL,������ʾ����Ӧ��ʧ��
	dispbuf = calloc(1,sizeof(disp_test));
	if(dispbuf==NULL)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	lib_kbflush();	//�������������
	//Ϊʵ���Զ������ԣ�����������
	lib_StartStopwatch();
	while ((nKeyin=lib_kbhit())==0)
	{	
		
		unBuf_x = rand()%ViewWidth;
		unBuf_y = rand()%ViewHeight;
		cls_show_msg1(2, "����(%d, %d)��ʾ%s.��������,�ɰ�ENTER;���쳣,�밴������", unBuf_x, unBuf_y, str);
		memset(img,0xff,ViewWidth*ViewHeight*2);//�ѿռ��ʼ����ȫ��
		if((ret = NDK_ScrGetFontBuf(str,img,unBuf_x,unBuf_y,ViewWidth,ViewHeight,0))!=NDK_OK)//����ȡ���ַ�����ŵ�ȫ��������
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			free(dispbuf);
			return;
		}
		memset(dispbuf, 0, sizeof(dispbuf));
		dispbuf->width = ViewWidth;
		dispbuf->height = ViewHeight;
		dispbuf->bytes_per_pixel = 2;
		dispbuf->image_buf = img;
		NDK_ScrDispImg(0,0,ViewWidth,ViewHeight,dispbuf);
		NDK_ScrRefresh();
		NDK_SysDelay(20);	
		if (MAXWAITTIME<lib_ReadStopwatch())
			break;
	}
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��" ,TESTAPI);
	else
		send_result("%s����ʧ��" ,TESTAPI);
	free(dispbuf);
	return;
}


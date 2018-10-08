/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 4.c
* Author 			: zhuq
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ����NDK_ScrSetbgPic�Ƿ�������Ļ����ͼƬ������NDK_ScrClrbgPic�Ƿ����ȡ������ͼƬ
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  MAXWAITTIME 5
#define 	TESTAPI		"NDK_ScrSetbgPic,NDK_ScrClrbgPic"

#define HEIGHT_HLJNX	(51)
#define SHUTDOWNPIC "shutdown.png"	//���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ�������� 20160513
#define SMALLPIC "set.png"
#define ERRPIC "err.png"
#define BIGPIC "big.png"
#define TESTBGPIC "test_bg.png"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr4
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        zhuq         20121224  	   created
*
*****************************************************************/
void scr4(void)
{
	/*private & local definition*/
	int  ret = 0, nKeyin = 0;
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ��
	if(cls_show_msg1(MAXWAITTIME,"��ȷ���Ѿ��ڲ��Գ���·��������shutdown,set,big��pngͼƬ,�����������,ESC���˳�")==ESC)
		return;
	
	//case1:����ͼƬ·��ΪNULL
	if((ret=NDK_ScrSetbgPic(NULL))!=NDK_ERR_DECODE_IMAGE)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	
	//case2:����ͼƬ·����ȷ,ͼƬ������
	if((ret=NDK_ScrSetbgPic(ERRPIC))!=NDK_ERR_PATH)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_ScrClrs();

	//case3:���ñ���ͼƬ��С������Ļ��С
	if((ret=NDK_ScrSetbgPic(BIGPIC)&&auto_flag!=1)!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME,"�����ô�������ͼƬ,��ȷ�ϲ���ͨ����������ʧ��");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	NDK_ScrClrs();
	
	//case4:ȡ������ͼƬ
	if((ret=NDK_ScrClrbgPic())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME,"��ȡ����������ͼƬ,��ȷ�ϲ���ͨ����������ʧ��");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	NDK_ScrClrs();

	//case5:���ñ���ͼƬ��С����Ļ��С��ͬ
	//�����ô���Ļ����,�����ý�С������,ɾ��ȫ������,���Ƿ������һ����Ч 20140603 added by jiangym
	if((ret=NDK_ScrSetbgPic(BIGPIC))!=NDK_OK&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME,"�����ô�������ͼƬ,��ȷ�ϲ���ͨ����������ʧ��");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	NDK_ScrClrs();
	if((ret=NDK_ScrSetbgPic(SHUTDOWNPIC))!=NDK_OK&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME,"������С����ͼƬ,��ȷ�ϲ���ͨ����������ʧ��");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	NDK_ScrClrs();
	
	//case6:ȡ������ͼƬ	
	if((ret=NDK_ScrClrbgPic())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	nKeyin=cls_show_msg1(MAXWAITTIME,"��ȡ��С����ͼƬ,��ȷ�ϲ���ͨ����������ʧ��");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	NDK_ScrClrs();

	//case7:���ñ���ͼƬ�Ĵ�СС����Ļ��С
	if((ret=NDK_ScrSetbgPic(SMALLPIC))!=NDK_OK&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME,"������С����ͼƬ,��ȷ�ϲ���ͨ����������ʧ��");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	NDK_ScrClrs();
	
	//case8:ȡ������ͼƬ
	if((ret=NDK_ScrClrbgPic())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	nKeyin=cls_show_msg1(MAXWAITTIME,"��ȡ��С����ͼƬ,��ȷ�ϲ���ͨ����������ʧ��");
	if(ENTER!=nKeyin&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	send_result("%s����ͨ��", TESTAPI);

ERR:
	NDK_ScrClrbgPic();
	if(cls_show_msg1(5, "��������Ƿ������ñ���ͼƬ:��[ȷ��],��[����]")==ENTER)
		NDK_ScrSetbgPic(TESTBGPIC);
	return;
}


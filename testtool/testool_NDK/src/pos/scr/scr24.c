/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: scr24.c
* Author 			: jiangym
* version			: 
* DATE			: 20121224
* directory 		: 
* description		: ���� NDK_ScrLoadBDF��NDK_ScrDispBDFText�ӿڹ���
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		 "NDK_ScrLoadBDF,NDK_ScrDispBDFText"
#define 	BDFFILE		 "10x20.bdf"	//���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ�������� 20160513
#define 	BDFFILE1	 "18x20.bdf"
#define 	ERRFILE		 "err.bdf"
#define     MAXWAITTIME   10//30
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr24
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	    jiangym        20130224  	   created
*
*****************************************************************/
void scr24(void)
{
	/*private & local definition*/
	int ret = 0;
	uint pw = 0, ph = 0, pw1 = 0, ph1 = 0, x=0, y=0;
	ushort text[] = {0x0023, 0x0033, 0x0043, 0x0066,0x0000};//��ӦBDF�ַ�#3Cf
	ushort text1[] = {0x0024, 0x0034, 0x0044, 0x0064,0x0000};//��ӦBDF�ַ�$4Dd
	uint ViewWidth = 0, ViewHeight = 0;
	
	/*process body*/
	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);
	
	if(NDK_FsExist(BDFFILE)!=NDK_OK&&NDK_FsExist(BDFFILE1)!=NDK_OK)
	{
		send_result("%s����bdf�ֿⲻ����,����POS�Ĳ��Գ���·��Ŀ¼���غ����²���", TESTAPI);
		return;
	}
	
	//NDK_ScrLoadBDF
	//case1:�쳣����:BDF·������ΪNULL��������ļ�������ʱ,Ӧ���ز�������
	if((ret=NDK_ScrLoadBDF(0,NULL,&pw,&ph))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrLoadBDF(1,ERRFILE,&pw,&ph))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:����punWidth��punHeight֧��NULL,���سɹ�
	if((ret=NDK_ScrLoadBDF(0,BDFFILE,NULL,&ph))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrLoadBDF(0,BDFFILE,&pw,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:���Ե�һ������Ϊ-1,���سɹ�,������ֻΪ��߲��Ը��Ƕ�
	if((ret=NDK_ScrLoadBDF(-1,BDFFILE,&pw,&ph))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//NDK_ScrDispBDFText
	//case4:����unFontID BDF����δ����
	if((ret=NDK_ScrDispBDFText(60000,x,y,text))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispBDFText(0,x,y,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case5:Խ�����:unX��unYԽ�磬����ʧ��
	if((ret=NDK_ScrDispBDFText(0,ViewWidth+1,ViewHeight,text))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispBDFText(0,ViewWidth,ViewHeight+1,text))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case6:��������:����BDF�����ʹ�ü��غ��BDF������ʾ
	if((ret=NDK_ScrLoadBDF(0,BDFFILE,&pw,&ph))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(pw!=10 || ph!=20)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, pw, ph);
		RETURN;
	}
	if((ret=NDK_ScrLoadBDF(1,BDFFILE1,&pw1,&ph1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(pw1!=18 || ph1!=20)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, pw1, ph1);
		RETURN;
	}
	//���ID0��BDF����
	NDK_ScrClrs();
	if((ret=NDK_ScrDispBDFText(0,x,y,text))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//ˢ��
	NDK_ScrRefresh();
	sleep(3);
	//�����˹�ȷ������������Ƿ���ȷ
	if(ENTER != cls_show_msg1(MAXWAITTIME, "��Ļ����ʾ:#3Cf,��[ȷ��]����ͨ��,[����]ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//���ID1��BDF����
	NDK_ScrClrs();
	if((ret=NDK_ScrDispBDFText(1,x,y,text1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//ˢ��
	NDK_ScrRefresh();
	sleep(3);
	//�����˹�ȷ������������Ƿ���ȷ
	if(ENTER != cls_show_msg1(MAXWAITTIME, "��Ļ����ʾ:$4Dd,��[ȷ��]����ͨ��,[����]ʧ��")&&auto_flag!=1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}


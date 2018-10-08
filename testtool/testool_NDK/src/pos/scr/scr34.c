/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 34.c
* Author 			: zhengry
* version			: 
* DATE			: 20170220
* directory 		: 
* description		: ����NDK_ScrDispImgWithFlashͨ��Flash���Ե�ַ��ʾͼƬ
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define		MAXWAITTIME 30
#define 	TESTAPI		"NDK_ScrDispImgWithFlash"
#define 	ADDR 		0x03788000
#define 	PICLEN 		139520
#define		PIC_XLEN	320
#define 	PIC_YLEN	218

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr10
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	        linwl         20160720  	   created
*****************************************************************/
void scr34(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0;
	uint ViewWidth = 0, ViewHeight = 0;
	
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//״̬�����������Ҫ��ȡ��Ļʵ���ӿڴ�С��Ϊ���߽����ֵ,״̬��δ������¾���Һ���Ŀ�͸�
	NDK_ScrGetViewPort(NULL, NULL, &ViewWidth, &ViewHeight);

	//����ǰ��
	if((nKeyin=cls_show_msg1(MAXWAITTIME,"���ڿ�����������غò�������ͼƬ,�����ú�����ʾͼƬ�ľ��Ե�ַ��ͼƬ��С������,[��]��ȷ��,��������")!=ENTER) && nKeyin !=0)
		return;

	//case1:�����쳣��unX,unY,unWidth,unHeight,offset,len�ֱ�Ϊ-1,>��Ļ,unX+unWidth>��Ļ,unY+unHeight>��Ļ
	if((ret=NDK_ScrDispImgWithFlash(-1,0,PIC_XLEN,PIC_YLEN, ADDR, PICLEN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithFlash(0,-1,PIC_XLEN,PIC_YLEN, ADDR, PICLEN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithFlash(0,0,-1,PIC_YLEN, ADDR, PICLEN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if((ret=NDK_ScrDispImgWithFlash(0,0,PIC_XLEN,-1, ADDR, PICLEN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithFlash(0,0,GUI_GetLCDWidth()+1,PIC_YLEN, ADDR, PICLEN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithFlash(0,0,PIC_XLEN,GUI_GetLCDHeight()+1, ADDR, PICLEN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithFlash(10,0,GUI_GetLCDWidth()-9,PIC_YLEN, ADDR, PICLEN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithFlash(0,10,PIC_XLEN,GUI_GetLCDHeight()-9, ADDR, PICLEN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithFlash(0,0,PIC_XLEN,PIC_YLEN, -1, PICLEN))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;//���ܷ�;���Ե�ַӦ��Ҫ�з�Χ
	}
	NDK_ScrRefresh();
	if((ret=NDK_ScrDispImgWithFlash(0,0,PIC_XLEN,PIC_YLEN, ADDR, -1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);//ͼƬһ��;ͼƬ��СӦ��Ҫ������
		//RETURN;ret=0
	}
	NDK_SysDelay(10);

	//case2:���Բ�����ͼƬ��͸���ʵ��ͼƬ�Ŀ�߲�һ�µ����,Ԥ�ڲ�Ӧ���ܷ�,���������κ��������쳣,������ʾ����
	cls_show_msg1(3,"3�����10,20λ����ʾһ��%d*%d����ͼƬ", PIC_XLEN/2, PIC_YLEN/2);
	NDK_ScrClrs();
	if((ret=NDK_ScrDispImgWithFlash(10, 20, PIC_XLEN/2, PIC_YLEN/2, ADDR, PICLEN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(10);
	if ((nKeyin=cls_show_msg1(MAXWAITTIME,"��ʾ��ͼƬ�Ƿ�����,��ȷ�ϲ���ͨ��,������ʧ��"))!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//cls_show_msg1(3,"3�����0,0λ�ÿ�ʼ��ʾһ��ȫ������ͼƬ");
	if((ret=NDK_ScrDispImgWithFlash(0, 0, ViewWidth, ViewHeight, ADDR, PICLEN))!=NDK_ERR_PARA/*NDK_OK*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(10);

	//case3:����������ʾͼƬ
	cls_show_msg1(3,"3�����0,0λ����ʾͼƬ");
	if((ret=NDK_ScrDispImgWithFlash(0, 0, PIC_XLEN, PIC_YLEN, ADDR, PICLEN))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(10);
	nKeyin = cls_show_msg1(MAXWAITTIME,"��ʾ��ͼƬ�Ƿ�����,��ȷ�ϲ���ͨ��,������ʧ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	return;
}


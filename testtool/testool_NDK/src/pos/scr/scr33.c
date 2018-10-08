/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: 33.c
* Author 			: linwl
* version			: 
* DATE			: 20160720
* directory 		: 
* description		: ����NDK_ScrDispImgWithOutFS�Ƿ���ָ����������ʾ״̬��ͼƬ
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define		MAXWAITTIME 30
#define 	TESTAPI		"NDK_ScrDispImgWithOutFS"
#define 	BATT_OFFSET		0	//���ͼƬ��ƫ�Ƶ�ַ	���ͼƬ�Ŀ�͸߷ֱ���22 12
#define 	GPRS_OFFSET 	3960//GPRSͼƬ��ƫ�Ƶ�ַ	GPRSͼƬ�Ŀ�͸߷ֱ���19 20
#define 	WIFI_OFFSET 	9660//WIFIͼƬ��ƫ�Ƶ�ַ	WIFIͼƬ�Ŀ�͸߷ֱ���20 14
#define 	NET_OFFSET 		13020//NETͼƬ��ƫ�Ƶ�ַ	NETͼƬ�Ŀ�͸߷ֱ���13 14
#define 	BATT_SIZE 		792	//���ͼƬ�ĳ���
#define 	GPRS_SIZE 		1140
#define 	WIFI_SIZE 		840
#define 	NET_SIZE 		546


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
void scr33(void)
{
	/*private & local definition*/
	int nKeyin = ENTER, ret = 0, i =0;
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
	if(cls_show_msg("�Ƿ��Ѿ�����statusicon.NLPӦ�ó���,[��]��ȷ��,��������")!=ENTER)
	{
		//case1:ͼƬӦ��δ��������²���Ӧ��ʧ��
		if((ret=NDK_ScrDispImgWithOutFS(80, 200, 22, 12, BATT_OFFSET, BATT_SIZE))!=NDK_ERR)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		send_result("%s����ͨ��,������statusicon.NLPӦ�ó�������²���", TESTAPI);
		return;
	}

	//case2:�����쳣��unX,unY,unWidth,unHeight,offset,len�ֱ�Ϊ-1,>��Ļ,unX+unWidth>��Ļ,unY+unHeight>��Ļ
	if((ret=NDK_ScrDispImgWithOutFS(-1,0,22,12, BATT_OFFSET, BATT_SIZE))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if((ret=NDK_ScrDispImgWithOutFS(0,-1,22,12, BATT_OFFSET, BATT_SIZE))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithOutFS(0,0,-1,12, BATT_OFFSET, BATT_SIZE))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if((ret=NDK_ScrDispImgWithOutFS(0,0,22,-1, BATT_OFFSET, BATT_SIZE))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if((ret=NDK_ScrDispImgWithOutFS(0,0,GUI_GetLCDWidth()+1,12, BATT_OFFSET, BATT_SIZE))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithOutFS(0,0,22,GUI_GetLCDHeight()+1, BATT_OFFSET, BATT_SIZE))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithOutFS(10,0,GUI_GetLCDWidth()-9,12, BATT_OFFSET, BATT_SIZE))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScrDispImgWithOutFS(0,10,22,GUI_GetLCDHeight()-9, BATT_OFFSET, BATT_SIZE))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if((ret=NDK_ScrDispImgWithOutFS(0,0,22,12, -1, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_ScrRefresh();
	if((ret=NDK_ScrDispImgWithOutFS(0,0,22,12, 0, -1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(10);

	//case3:���Բ�����ͼƬ��͸���ʵ��ͼƬ�Ŀ�߲�һ�µ����,Ԥ�ڲ�Ӧ���ܷ�,���������κ��������쳣,������ʾ����,����Ӧ���ڴ��ͼƬ�ļ�ʱ����������Լ���ͼƬ�Ŀ��
#if !(defined ME50NX||defined ME68) //ME50NX��width��  unheight��lenһ��Ҫ�������¹�ϵ: unwidth*unheight*2=len��unwidth*unheight*3=len����������Ʒ�������� 20171113
	cls_show_msg1(3,"3�����20,200λ����ʾһ��20*10����ͼƬ");
	if((ret=NDK_ScrDispImgWithOutFS(20, 200, 20, 10, BATT_OFFSET, BATT_SIZE))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(10);
	//cls_show_msg1(3,"3�����0,0λ�ÿ�ʼ��ʾһ��ȫ������ͼƬ");
#endif
	if((ret=NDK_ScrDispImgWithOutFS(0, 0, ViewWidth, ViewHeight, BATT_OFFSET, BATT_SIZE))!=NDK_ERR_PARA/*NDK_OK*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(10);
	
	//case4:����������ʾ��ص���,��0��ʼ�����ӵ�3��
	cls_show_msg1(3,"3�����80,200λ����ʾ������0����3��");
	for(i=0;i<4;i++)
	{
		if((ret=NDK_ScrDispImgWithOutFS(80, 200, 22, 12, BATT_OFFSET+i*BATT_SIZE, BATT_SIZE))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysDelay(10);
	}
	//case5:����������ʾ�����ź�,��0��ʼ�����ӵ�4��
	cls_show_msg1(3,"3�����140,200λ����ʾ�����źŴ�0��ǿ��4");
	for(i=0;i<5;i++)
	{
		if((ret=NDK_ScrDispImgWithOutFS(140, 200, 19, 20, GPRS_OFFSET+i*GPRS_SIZE, GPRS_SIZE))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysDelay(10);
	}
	//case6:����������ʾWIFI�ź�,��0��ʼ�����ӵ�3��
	cls_show_msg1(3,"3�����200,200λ����ʾWIFI�źŴ�0��ǿ��3");
	for(i=0;i<4;i++)
	{
		if((ret=NDK_ScrDispImgWithOutFS(200, 200, 20, 14, WIFI_OFFSET+i*WIFI_SIZE, WIFI_SIZE))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysDelay(10);
	}
	//case7:����������ʾG���ı�־
#if defined ME68  // ME68��Һ�����Ϊ240*320,20180416 sull modify
	cls_show_msg1(3,"3�����200,200λ����ʾG����־");
	if((ret=NDK_ScrDispImgWithOutFS(200, 200, 13, 14, NET_OFFSET, NET_SIZE))!=NDK_OK)
#else
	cls_show_msg1(3,"3�����260,200λ����ʾG����־");
	if((ret=NDK_ScrDispImgWithOutFS(260, 200, 13, 14, NET_OFFSET, NET_SIZE))!=NDK_OK)
#endif	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(10);
	nKeyin = cls_show_msg1(MAXWAITTIME,"��ʾ��״̬��ͼƬ�Ƿ�����,��ȷ�ϲ���ͨ��,������ʧ��");
	if (nKeyin==ENTER || nKeyin==0)
		send_result("%s����ͨ��", TESTAPI);
	else
		send_result("%s����ʧ��", TESTAPI);
	return;
}


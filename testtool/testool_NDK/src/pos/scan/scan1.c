/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ɨ��
* file name			: 1.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_ScanInit�Ƿ���Խ���ɨ��ͷ��ʼ��
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScanInit"
#define    MAXTIMEOUT  10

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: scan1
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*										jiangym			20130917		created
*
*****************************************************************/
void scan1(void)
{
	/*private & local definition*/
	int ret = 0, plen = 0;
	char pvalue[500] = {0};
		
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:��ʼ��Ӧ�óɹ�,��ʼ�������ܽŵ�ƽ��ʼ��������Ĭ�����á� 
	if((ret=NDK_ScanInit())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("�����ɨ��");
	if((ret=NDK_ScanDoScan(MAXTIMEOUT,pvalue,&plen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����Ĭ������(�����Խ�Ϊ��˸��������Ϊ11))
	if(cls_show_msg("ɨ������������Խ���Ϊ��˸,[Enter]ͨ��,����ʧ��")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//��������
	send_result("%s����ͨ��", TESTAPI);	
ERR:
#if SCAN_SOFT_ENABLE	
	NDK_ScanExit();
#endif
	return;
}


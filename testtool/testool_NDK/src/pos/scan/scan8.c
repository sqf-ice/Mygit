/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ɨ��
* file name			: 8.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_ScanExit�Ƿ���������˳�ɨ��
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScanExit"
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
*					 	  sull			   20180409		    created
*****************************************************************/
void scan8(void)
{
	/*private & local definition*/
	int ret = 0, plen = 0;
	char pvalue[500] = {0};
		
	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	#if 0//����û����ȷ˵���˺���ʹ���������ʱ����˵�
	//case1:û�г�ʼ�����˳�Ӧ��ʧ��
	if((ret = NDK_ScanExit()) == NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	#endif

	//case2:�˳���Ӧ��������ɨ��
	if((ret = NDK_ScanInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_ScanExit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("����10���ڽ���ɨ��");
	if((ret = NDK_ScanDoScan(MAXTIMEOUT,pvalue,&plen)) != NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:�˳����ٽ��г�ʼ����Ӧ�ܳɹ�ɨ��
	if((ret = NDK_ScanInit()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	plen=0;
	memset(pvalue,0,sizeof(pvalue));
	cls_printf("����20���ڽ���ɨ��");
	if((ret=NDK_ScanDoScan(MAXTIMEOUT*2,pvalue,&plen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("ɨ�赽�ĳ���Ϊ:%d������:%s,ȷ�ϼ�ͨ��,������ʧ��", plen, pvalue)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//��������
	send_result("%s����ͨ��", TESTAPI);	
	
ERR:	
	NDK_ScanExit();
	return;
}



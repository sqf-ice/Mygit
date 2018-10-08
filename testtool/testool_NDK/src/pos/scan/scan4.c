/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ɨ��
* file name			: 4.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_ScanInit����ͷ��ʼ����NDK_ScanClose����ͷ�رչ���(������ʽ)
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScanInit��NDK_ScanClose"
#define 	MAXTIME     60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: scan4
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*							zhengry			20160526		created
*
*****************************************************************/
void scan4(void)
{
	/*private & local definition*/
	int ret=0, plen = 0;
	char pvalue[50] = {0};

	/*process body*/
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:δ��ʼ������ͷ��Ӧ��������ͷδ��ʼ��NDK_ERR_CAMERA_INIT
	if((ret=NDK_ScanClose())!=NDK_ERR_CAMERA_INIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_ScanDoScan(20, pvalue, &plen))!=NDK_ERR_CAMERA_INIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		//return; ret=0
	}

	//case2:��������:�������κζ���
	if((ret=NDK_ScanInit())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=lib_doscan(1, MAXTIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:�ر�����ͷ��Ӧ�޷�ɨ������
	if((ret=NDK_ScanClose())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScanStart()) != NDK_ERR_CAMERA_INIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScanDoScan(20, pvalue, &plen))!=NDK_ERR_CAMERA_INIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//NDK_ScanInit: NDK_ERR_CAMERA_LOAD_DRV ��������ʧ�� 
	//NDK_ScanInit: NDK_ERR_CAMERA_ENUMERATE  ����ö��ʧ�� 
	//NDK_ScanInit��NDK_ScanClose: NDK_ERR_CAMERA_SYSCMD ϵͳ����ִ�г���
	
	//��������
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_ScanClose();
	return;
}

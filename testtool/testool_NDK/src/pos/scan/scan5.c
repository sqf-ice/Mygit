/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ɨ��
* file name			: 5.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_ScanSet����ͷ��ز�����������(������ʽ)
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_ScanSet"
#define 	MAXTIME     60
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: scan5
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*							zhengry			20160526		created
*
*****************************************************************/
void scan5(void)
{
	//����ǰ��
	int ret=0;

	//case1:δ��ʼ������ͷ��Ӧ��������ͷδ��ʼ��
	if((ret=NDK_ScanSet(CAMERA_SETTYPE_IMAGE, 0))!=NDK_ERR_CAMERA_INIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return; 
	}
	
	if((ret=NDK_ScanInit())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	//case2:�쳣��������
	if((ret=NDK_ScanSet(CAMERA_SETTYPE_IMAGE-1, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScanSet(CAMERA_SETTYPE_GAMMA+1, 0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScanSet(CAMERA_SETTYPE_IMAGE, -1))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_ScanSet(CAMERA_SETTYPE_IMAGE, 2))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//����
	
	//case3:����ͷ��ʾCAMERA_SETTYPE_IMAGE�ֱ�Ϊ0��1��ʱ��ɨ��ʱ����ͷӦ��Ӧ�Ļ�򿪻��߹ر�
	if((ret=NDK_ScanSet(CAMERA_SETTYPE_IMAGE, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=lib_doscan(0, MAXTIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(cls_show_msg("ɨ�����������ͷ���ڹر�״̬,��[ENTER],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI);
		RETURN;
	}

	if((ret=NDK_ScanSet(CAMERA_SETTYPE_IMAGE, 1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=lib_doscan(1, MAXTIME))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_ScanClose())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4:����ͷ����CAMERA_SETTYPE_SATURATION�ֱ�ΪXX��XX��ʱ��ɨ��ʱ����ͷӦ��Ӧ�Ļ�...

	//case5:����ͷ�ع�CAMERA_SETTYPE_SHARPNESS�ֱ�ΪXX��XX��ʱ��ɨ��ʱ����ͷӦ��Ӧ�Ļ�...

	//case6:����ͷ֡��CAMERA_SETTYPE_GAMMA�ֱ�Ϊ0��1��ʱ��ɨ��ʱ����ͷӦ��Ӧ�Ļ�...
	
	//��������
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_ScanClose();
	return;
	
}

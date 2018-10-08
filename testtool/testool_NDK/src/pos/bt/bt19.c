/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt19.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTSetType �ܷ���ȷ������������
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define   TESTAPI		"NDK_BTSetType"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt19
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20150319		created
*****************************************************************/
void bt19(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}

	/*private & local definition*/
	int ret = -1;		
	char ph_type[3]={0x0c,0x02,0x5a}, ear_type[3]={0x04,0x04,0x24};
	char outbuf[16] = {0};
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);
	
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	//case1:δ���������ڽ�������Ӧ�÷���ʧ��(ʵ�ʲ�������)
	//��������:������һ�� OPEN֮ǰ ����ʧ��(δ��patch)������OPEN֮ǰ���� ����ɹ�.
	//��Ϊ�ڴ�֮ǰ���ڽ��������õ��ù�open�� ������δopenǰ���þͻ�ɹ���
	if((ret=NDK_BTSetType(ph_type))!=/*NDK_ERR_IOCTL*/NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalName(outbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, outbuf);
		GOTOERR;
	}
	//case2:����ΪNULL Ӧ�÷���ʧ��
	if((ret=NDK_BTSetType(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:����Ϊ�ֻ�ͼ����ô����ʱ��ʾ��Ӧ��Ϊ�ֻ�ͼ��	
	if((ret=NDK_BTSetType(ph_type))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("���ֻ���������������:%s��ͼ���Ƿ�Ϊ�ֻ�ͼ��,��[Enter],��[����]",outbuf)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case4:����Ϊ����ͼ����ô�ѵ�ʱ��ʾ��Ӧ��Ϊ����ͼ��
	if((ret=NDK_BTSetType(ear_type))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("���ֻ�������������������:%s��ͼ���Ƿ�Ϊ����ͼ��,��[Enter],��[����]",outbuf)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case5:�������һ��������Ч,�����ö���,�������ֻ�,Ӧ���ѵ����ֻ�ͼ��
	if((ret=NDK_BTSetType(ear_type))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTSetType(ph_type))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("��������������������:%s��ͼ���Ƿ�Ϊ�ֻ�ͼ��,��[Enter],��[����]",outbuf)!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}

	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_PortClose(PORT_NUM_BT);
	return;
}



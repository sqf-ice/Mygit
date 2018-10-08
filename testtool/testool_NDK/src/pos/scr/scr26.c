/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ʾ
* file name		: scr26.c
* Author 			: huangjianb
* version			: 
* DATE				: 20140617
* directory 	: 
* description	: ����NDK_ScrTSCalibrate��������У�鹦��
* 
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		 "NDK_ScrTSCalibrate"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: scr26
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	  	 huangjianb   20140617 	created
*
*****************************************************************/
void scr26(void)
{
	int ret = -1;
	unsigned int unLen=0;
	char szBuf[24]={0};

	if(auto_flag==1)
	{
		send_result("%s��֧���Զ�����,�������ֶ�����", TESTAPI);
		return;
	}
	//����ǰ��
	//����ǰ��Ҫ�޸Ĵ��������ļ����������޸ĳɴ�����������ʹ�ã������ļ��޸ĺ���Ҫ����������Ч
	if(cls_show_msg("����ǰ��ȷ���ѽ���/guiapp/etc·���޸�pointercal�ļ��޸ĳɴ�����׼ȷ:��[ȷ��],��[����]")!=ENTER)
		return;
	
	//��ȡposӲ����Ϣ�ӿ� 
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_HARDWARE_INFO, &unLen, szBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//case1���쳣����,�Ǵ�������Ӧ����NDK_ERR_NO_DEVICES
	if (szBuf[11] == 0xFF)
	{
		if((ret = NDK_ScrTSCalibrate())!=NDK_ERR_NO_DEVICES)
		{
			send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		send_result("%s����ͨ��(���豸Ϊ�Ǵ����豸)", TESTAPI);
		return;
	}

	//���Թ����в������������,����yuyanfei��������ȷ�ϵ�20180608 modify
	NDK_SysSetSuspend (0);
	//case2����������,У׼������,�û�ȡ�����˳�
	cls_show_msg("У׼�������밴ESC��Ӧ�ܹ��˳�У׼����,�������ʼ����");
	if((ret = NDK_ScrTSCalibrate())!=NDK_ERR_QUIT)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3����������У׼������Ӧ��ֹ���浯�����ܣ����Ի���21%�ĵ�أ�������У׼���水�ػ���
	cls_show_msg("���ڴ���У׼�����а��ػ������߹���͵絯��,�������ʼ����");
	if((ret = NDK_ScrTSCalibrate())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("У׼�������Ƿ���ֵ���:��[ȷ��],��[����]")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//case4����������Ӧ֧����������У��
	cls_show_msg("�������������У��,�������ʼ����");
	if((ret = NDK_ScrTSCalibrate())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:����У׼�������˳�����Ӱ����Ļ׼ȷ��(��У׼���˳�����ʹ����һ��У׼�Ľ��)
	cls_show_msg("У׼�������밴ESC��Ӧ�ܹ��˳�У׼����,�������ʼ����");
	if((ret = NDK_ScrTSCalibrate())!=NDK_ERR_QUIT)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��,����Ӧ��������ʹ��", TESTAPI);
ERR:	
	NDK_SysSetSuspend (1);
	return;
}

/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys18.c
* Author 			: chensj
* version			: 
* DATE				: 20130718
* directory 		: 
* description		: ����NDK_SysGetFirmwareInfo�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysGetFirmwareInfo"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		      chensj	           20130718	 	created
*****************************************************************/
void sys18(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	int nRet = 0;
	EM_SYS_FWINFO emFWinfo;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//case1:�쳣����
	if((nRet = NDK_SysGetFirmwareInfo(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}

	//case2:��������
	if((nRet = NDK_SysGetFirmwareInfo(&emFWinfo)) != NDK_OK||(emFWinfo!=SYS_FWINFO_PRO&&emFWinfo!=SYS_FWINFO_DEV))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, nRet, emFWinfo);
		RETURN;
	}
	send_result("�̼��汾Ϊ%s�汾,���ж��Ƿ���ȷ",(emFWinfo==SYS_FWINFO_PRO)?"��ʽ":"����");

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}

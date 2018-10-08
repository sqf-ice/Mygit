/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ϵͳ
* file name			: 39.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_SysBeepVoiceCtrl�Ƿ�����������ð�������С(�Ͷ�֧��)
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_SysBeepVoiceCtrl"   
#define  MAXWAITTIME 30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: voice2
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*					      sull		      20180115		    created
*
*****************************************************************/
void sys39(void)
{
	/*private & local definition*/
	int ret = -1, i = 0, nKeyin = 0;


	/*process body*/	
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ��������ģ��������

	//case1:�����Ƿ�
	if((ret = NDK_SysBeepVoiceCtrl(0)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysBeepVoiceCtrl(11)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
		GOTOERR;
	}

	//case2:��������Ϊ1-10Ӧ�óɹ����Ұ�������Ӧ������
	for(i=1; i<=10; i++)
	{
		if((ret = NDK_SysBeepVoiceCtrl(i)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
		cls_show_msg1(MAXWAITTIME,"�밴���������������ӦΪ%d", i);
	}
	nKeyin=cls_show_msg1(MAXWAITTIME,"�����������Ƿ�������?��[ENTER],����[����]");
	if(nKeyin != ENTER && nKeyin != 0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//��������	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_SysBeepVoiceCtrl(10);//Ĭ��ֵΪ10
	return;
}


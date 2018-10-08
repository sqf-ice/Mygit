/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys25.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ����NDK_SysGetPowerVolRange�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"
//#if defined ME30||defined ME30MH
//#define VOL_LOWEST  1650 //30S���Ǳ��ֺ�������Ʒ��һ����1650
//#else
//#define VOL_LOWEST  1750//20150116 �������ظ� ����Ӳ������޸�30����ʹ��1650����������Ʒһ��ͳһ��1750
//#endif
//#define VOL_FULL    2100
#if ANDIROD_ENABLE
#define VOL_LOWEST  380//20160222 �޸�
#define VOL_FULL    469
#elif defined ME30MH||defined ME30GX||defined ME32||defined ME32GX||defined ME65
#define VOL_LOWEST  3300
#define VOL_FULL    4200
#else
#define VOL_LOWEST  1750//20150116 �������ظ� ����Ӳ������޸�30����ʹ��1650����������Ʒһ��ͳһ��1750
#define VOL_FULL    2100
#endif

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysGetPowerVolRange"

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
*			  		     jiangym		20140530	created
*****************************************************************/
void sys25(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	int nRet = 0;
	uint punmax = 0, punmin = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//case1:�쳣����
	if((nRet = NDK_SysGetPowerVolRange(NULL,&punmin))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	if((nRet = NDK_SysGetPowerVolRange(&punmax,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}

	//case2:��������
	if((nRet = NDK_SysGetPowerVolRange(&punmax,&punmin)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	if(((punmin != VOL_LOWEST)&&(punmin != 1650)) || (punmax != VOL_FULL))
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, punmax, punmin);
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}


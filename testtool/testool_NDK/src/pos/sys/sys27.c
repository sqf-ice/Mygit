/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys27.c
* Author 			: zhengry
* version			: 
* DATE				: 20150722
* directory 		: 
* description		: ��������NDK_SysGetRemainRAMInfo��ȡ�̼�ʣ��RAM�ռ�ĵ�ַ�ʹ�С(��K21֧��)
* related document	: NDK.chm
*
************************************************************************/ 
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysGetRemainRAMInfo"	 

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	  zhengry       20150722       created
*****************************************************************/
void sys27(void)//�޲����޷���
{
	/*private & local definition*/
	int ret=0, ram_addr=0, ram_size=0;
	/*process body*/
	cls_printf("����%s...", TESTAPI);  

	//case1:�����Ƿ� 
	if((ret=NDK_SysGetRemainRAMInfo(NULL, &ram_size))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysGetRemainRAMInfo(&ram_addr, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
 
	//case2:��ȡ�̼�ʣ��RAM�ռ�ĵ�ַ�ʹ�С���ж��Ƿ����
	if((ret=NDK_SysGetRemainRAMInfo(&ram_addr, &ram_size))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(auto_flag==0 || auto_flag==2)
	{
		if(cls_show_msg("��ȡ���̼�ʣ��RAM�ռ�ĵ�ַ�Ǵ�%d��Ԫ��ʼ����СΪ%d�����ж��Ƿ����?��[Enter],��[����]", ram_addr, ram_size)!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return; 
		}
	}
	else
		send_result("��ȡ���̼�ʣ��RAM�ռ�ĵ�ַ�Ǵ�%d��Ԫ��ʼ����СΪ%d������ʵ���������Ƚϡ�", ram_addr, ram_size);

	send_result("%s����ͨ��", TESTAPI);
	return;	
}

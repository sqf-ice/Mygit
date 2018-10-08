/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys30.c
* Author 			: zhengry
* version			: 
* DATE				: 20150827
* directory 		: 
* description		: ����NDK_SysGetEvent��ȡK21�˸�CPU֪ͨ�¼�
* related document	: NDK.chm
*
************************************************************************/ 
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysGetEvent"	 
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
*			  	  	      zhengry       20150827       created
*****************************************************************/
void sys30(void)
{
	/*private & local definition*/
	int ret=0, len=0;
	uint event=0;
	uchar out_data[128]={0};
	
	/*process body*/
	if(auto_flag==1)
	{
		//send_result("line %d:%s�޴��Զ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);  

	cls_show_msg("��ȷ�������״����и�����,�����������...");  
	//case1:��������
	if((ret=NDK_SysGetEvent(NULL, &len, out_data))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysGetEvent(&event, NULL, out_data))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysGetEvent(&event, &len, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:��������:����K21�˰�ȫ��������ȡ��CPU֪ͨ�¼���ʵ���Ƿ�һ�¡�
 	memset(out_data, 0, sizeof(out_data));
	cls_show_msg("�빹�찲ȫ����,���������...");
	if((ret=NDK_SysGetEvent(&event, &len, out_data))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(event!=SYS_EVENT_TAMPER|| len<=0|| len>128||(out_data[0]==0&&out_data[1]==0&&out_data[2]==0&&out_data[3]==0))//��ȫ�������ؼĴ���ֵΪ4���ֽ�,���������ĳ���ֽڿ϶���Ϊ0
	{
		send_result("line %d:%s��ȡ��ȫ�����¼�ʧ��(%d,%d,0x%2x,0x%2x,0x%2x,0x%2x)", __LINE__, TESTAPI, event, len, out_data[0], out_data[1], out_data[2], out_data[3]);
		RETURN;
	} 

	//case4:SYS_EVENT_REBOOT�¼���ʱ�޷�����
	
	//case3:��������:���¼�
	memset(out_data, 0, sizeof(out_data));
	if((ret=NDK_SysGetEvent(&event, &len, out_data))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(event!=SYS_EVENT_NONE|| len!=0|| strlen(out_data)!=0)
	{
		send_result("line %d:%s��ȡ���¼�ʧ��(%d,%d,%s)", __LINE__, TESTAPI, event, len, out_data);
		RETURN;
	}

 	send_result("%s����ͨ��", TESTAPI);
	return;	
}


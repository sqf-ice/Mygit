/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		     : 
* Author 			:
* version			: 
* DATE			:
* directory 		: 
* description		: ���NDK_AppStartChildProcess������Ƶ���Ӧ��,ʹ�ú���NDK_AppGetDatafromParentProcess,NDK_AppChildProcessExit�����ӽ���֮���Ƿ��ܹ�����ͨѶ
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "NDK.h"
#include <fcntl.h>
/*---------------constants/macro definition---------------------*/
//#define   TESTAPI		"NDK_AppStartChildProcess"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : ģ���12������������ʹ�õ���Ӧ��
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*						  jiangym			20170613   created
*****************************************************************/
void main(void)
{
	int ret = -1, data = 0, len = 0;
	char text[] = "�����ӽ���3";

	if(NDK_ScrInitGui() != NDK_OK)
		   return ;

	NDK_ScrAutoUpdate(0, NULL);
	NDK_ScrClrs();
	NDK_ScrPrintf("�����ӽ���3,���������");
	NDK_ScrRefresh();
	NDK_KbGetCode(0,NULL);
	//�Ӹ����̶�ȡ��Ӧ�õĲ���
        if( (ret=NDK_AppGetDatafromParentProcess(&data, &len))!=NDK_OK)
	{
		NDK_ScrPrintf("line:%d�Ӹ����̶�ȡ����ʧ��%d",__LINE__,ret);
		NDK_ScrRefresh();
		NDK_KbGetCode(0,NULL);
		return;
	}
	NDK_ScrClrs();
	NDK_ScrPrintf("�յ������̴���������:%d,���������",data);
	NDK_ScrRefresh();
	NDK_KbGetCode(0,NULL);
	//ִ������˳�,���������ݳ���Ϊ0�����ݸ�������,�������ݷ���
	if((ret=NDK_AppChildProcessExit(0, text,0 ))!=NDK_OK)
	{
		NDK_ScrPrintf("line:%d��Ӧ���˳�ʧ��%d",__LINE__,ret);
		NDK_ScrRefresh();
		NDK_KbGetCode(0,NULL);
		return;
	}
	return;
}


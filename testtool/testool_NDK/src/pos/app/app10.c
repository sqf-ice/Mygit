/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App10.c
* Author 			:
* version			: 
* DATE			:
* directory 		: 
* description		: ����NDK_AppLoad�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#define  TESTAPI	  "NDK_AppLoad"
#if CPU5810X_ENABLE ||defined ME50NX||defined ME68
#define  FILETEST1   "mapp.NLP"
#else
#define  FILETEST1   "/appfs/mapp.NLP" 
#endif

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���10������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*
*****************************************************************/
void app10(void)
{
	/*private & local definition*/
	int  ret = 0;
	
	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	if(cls_show_msg1(30, "����%s...��ȷ��%s�����ļ�����,ESC�˳�", TESTAPI,FILETEST1)==ESC)
		return;//��Ļ��ʾ��ǰ��������ģ��������
	
	//case1:����װ��
	if(( ret = NDK_AppLoad((char *)FILETEST1,1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
		RETURN;
	}
	
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);
	return;
}


/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt6.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnGetType�ӿڹ���
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_PrnGetType"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void prnt6(void)
* functional description 	: ��ȡ����ӡ������ 
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt6(void)
{
	//����Ϊ�ֲ�����
	EM_PRN_TYPE emType;
	//int nRet;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	
	//case1:����NDK_PrnGetType�ܷ��ȡ��ӡ������:TP��HIP��FIPδ֪����
	emType = lib_getprintertype();
	switch (emType)
	{
	case PRN_TYPE_TP:
		cls_printf("����:������ӡ");
		break;
	case PRN_TYPE_HIP:
		cls_printf( "����:�������");
		break;
	case PRN_TYPE_FIP:
		cls_printf("����:Ħ�����");
		break;
	default:
		cls_printf("δ�ܻ�ȡ��ӡ������");
		break;
	}
	if (show_msg( "\n����ӡ�������Ƿ�ƥ��,ENTERͨ��,��������ͨ��")!=ENTER)//zhangnw20061031 modify
	{
		send_result("%s����ʧ��", TESTAPI);
		return;
	}

#if 0 //�ӿ���ȥ��
	//case2:���Ϊ��
	if((nRet = NDK_PrnGetType(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
		return;
	}
#endif
	
	//���Խ���	
	send_result("%s����ͨ��", TESTAPI);
	return;
}


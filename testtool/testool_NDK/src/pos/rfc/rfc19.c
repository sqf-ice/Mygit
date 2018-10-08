/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc19.c
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description		: ����NDK_MifareActiveѰ��������ײ��ѡ���ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_MifareActive"

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
*			  		    jiangym			    20180110		created  
*****************************************************************/
void rfc19(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	uchar UIDLen = 0,UIDLen1 = 0, sak;
	uchar UID[LEN_UID]={0}, UID1[LEN_UID]={0};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_RfidPiccDeactivate(0);
	//case1:�쳣���ԣ��������	     
	if((ret=NDK_MifareActive(REQA, NULL, &UIDLen,&sak))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MifareActive(REQA, UID, NULL,&sak))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:δ���ÿ�Ƭ,Ѱ������ͻʧ�ܷ����޿�-2030
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_ERR_MI_NOTAGERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:���ô���Ŀ�Ƭ,����ʧ�� 
	cls_show_msg("���ڸ�Ӧ������1�ű�׼B��,���������...");
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_ERR_MI_NOTAGERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:�������ſ�Ӧ�÷��ض࿨��ͻ -2048 
	cls_show_msg("���ڸ�Ӧ������2��M0��,���������...");
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_ERR_MI_COLLERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:��������һ��M��,Ӧ�÷��سɹ�
	cls_show_msg("���ڸ�Ӧ������1��M0��,���������...");
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��Ҫ���µ��ٵ���Ѱ������Ч
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case6:ͬһ�ſ���ȡ��uidӦ����ͬ
	if((ret=NDK_MifareActive(REQA, UID1, &UIDLen,&sak))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(UID, UID1, sizeof(UID)))
	{
		send_result("line %d:ͬ�ſ�UID��һ��", __LINE__);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case7:��ͬһ�ſ���ȡ��uidӦ�ò�ͬ
	cls_show_msg("���ڸ�Ӧ����������һ��M0��,���������...");
	if((ret=NDK_MifareActive(REQA, UID1, &UIDLen1,&sak))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(!memcmp(UID, UID1, sizeof(UID)))
	{
		send_result("line %d:��ͬ��UIDӦ��һ��", __LINE__);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);
	return;
}


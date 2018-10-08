/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc20.c
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description		: ����NDK_M0Authen��֤�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_M0Authen"

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
*			  		     jiangym			    20180110		created
*****************************************************************/
void rfc20(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	uchar  UID[LEN_UID] = {0}, UIDLen = 0, sak;
	uchar errkey[16] = {"\x11\x22\x33\x44\x55\x11\x22\x33\x44\x55\x11\x22\x33\x44\x55\x46"};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_RfidPiccDeactivate(0);
	//˵��:��C��M0�����Խ�����֤,��֤��Ȩ�ޱȽϴ�;����C��M0������Ҫ��֤
	//case1:δ����Ѱ������ͻ�ͽ�����֤Ӧ��ʧ��
	cls_show_msg("���ڸ�Ӧ������1�Ŵ�C��M0��,���������...");
	if((ret=NDK_M0Authen(AUTHKEY_M0))!=NDK_ERR_MI_WriteFifo)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//Ѱ������ͻ
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_OK)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2:�쳣����:�������	     
	if((ret=NDK_M0Authen(NULL))!=NDK_ERR_PARA)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//��߿�Ƭʹ��Mifare UltraLight C����֤,ע�������ȷ����Կ�ǳ�ʼ��Կ,û�иĹ�
	//���ʹ�ò���C�����ֿ�Ƭ������ȷ����Կ��֤Ҳ��ʧ�ܵ�,����C�Ŀ�����Ҫ��֤
	//case3:ʹ�ô�������������֤��ʧ��
	if((ret=NDK_M0Authen(errkey))!=NDK_ERR_MI_CRCERR) //-2031 crcУ���
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:ʹ����ȷ�����������֤(ע��:ʧ�ܺ���Ҫ���½���Ѱ������ͻ)
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_OK)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Authen(AUTHKEY_M0))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�µ�	
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:ʹ�ò���C��M0��������֤Ԥ��ʧ��
	cls_show_msg("���ڸ�Ӧ������1�Ų���C��M0��,���������...");
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_OK)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Authen(AUTHKEY_M0))!=NDK_ERR_MI_NOTAGERR) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	//case5:��֤��֤��Կ���Ըĳ��Լ���,��ʱ����֤

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);
	return;
}


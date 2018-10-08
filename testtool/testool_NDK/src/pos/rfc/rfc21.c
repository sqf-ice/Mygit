/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc21.c
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description		: ����NDK_M0Write��NDK_M0Read��д�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_M0Write,NDK_M0Read"

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
void rfc21(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, rec_len = 0, i = 0;
	uchar UIDLen = 0, sak;
	uchar UID[10] = {0};
	uchar rec_data[30] = {0}, wr_data[32] = {0};

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_RfidPiccDeactivate(0);
	//case1:δ����Ѱ������ͻ�ͽ��ж�дӦ��ʧ��
	if((ret=NDK_M0Write(8,4,wr_data))!=NDK_ERR_MI_WriteFifo)//-2041 д����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Read(8,&rec_len,rec_data))!=NDK_ERR_MI_WriteFifo)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//Ѱ������ͻ
	cls_show_msg("���ڸ�Ӧ������1�Ų���C��M0��,���������...");
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case2:�쳣��������	     
	if((ret=NDK_M0Write(8,4,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Write(8,5,wr_data))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Write(8,17,wr_data))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Read(8,NULL,rec_data))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Read(8,&rec_len,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:��������:(��ʼ��Ƭ�ǲ���Ҫ��֤��)����֤���ж�д
	for (i=0; i<sizeof(wr_data); i++)
		wr_data[i]=rand()%256;
	//��Ҫʹ��δ�Ĺ���ʼ��֤��Կ�Ŀ�
	if((ret=NDK_M0Write(8,4,wr_data))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Write(9,4,wr_data+4))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Write(10,16,wr_data+8))!=NDK_OK)	//ʵ��ֻд��4�ֽ� 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Write(11,16,wr_data+12))!=NDK_OK)  //ʵ��ֻд��4�ֽ� 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Read(8,&rec_len,rec_data))!=NDK_OK)//��8-11����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(rec_data,wr_data,16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:��֤���޸�AUTH0����ַ42��ֵ,��Ϊ20H,�޸ĺ�Ƭ20H��ĵ�ַ���в�����Ҫ��֤����Ȩ��,��������ٻָ�ԭ��
	cls_show_msg("���ڸ�Ӧ������1�Ŵ�C��M0��,���������...");
	//��������Ҫ�����ϵ�
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Authen(AUTHKEY_M0))!=NDK_OK) //-2041
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Write(42,4,(uchar*)"\x20\x20\x20\x20"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�޸�AUTH0����������Ч,�µ��������Ѱ��
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//δ��֤��20H��Ķ�д������ʧ��,�����25H(��37��ַ)Ӧ��ʧ��
	if((ret=NDK_M0Write(37,4,wr_data))!=NDK_ERR_MI_NOTAUTHERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ʧ�ܺ���Ҫ����Ѱ��
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//������֤,��֤���дӦ�óɹ�21H--24H(33-36)
	if((ret=NDK_M0Authen(AUTHKEY_M0))!=NDK_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	for(i=33;i<37;i++)
	{
		if((ret=NDK_M0Write(i,4,wr_data+(i-33)*4))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, i);
			GOTOERR;
		}
	}	
	if((ret=NDK_M0Read(33,&rec_len,rec_data))!=NDK_OK)//��33-36����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(memcmp(rec_data,wr_data,16))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	//�ָ�ԭ���Ĳ���֤
	if((ret=NDK_M0Write(42,4,(uchar*)"\x30\x30\x30\x30"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:��֤�����ɲ�����ҳ֧��43ҳ,�����ַ��43��ʼ���ͷ��ʼѭ��,�����44��ʼ����ʧ��
	if((ret=NDK_MifareActive(REQA, UID, &UIDLen,&sak))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_M0Read(43,&rec_len,rec_data))!=NDK_OK)//��43-0 1 2����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��44-47����Ԥ��ʧ��
	if((ret=NDK_M0Read(44,&rec_len,rec_data))!=NDK_ERR_MI_CRCERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
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


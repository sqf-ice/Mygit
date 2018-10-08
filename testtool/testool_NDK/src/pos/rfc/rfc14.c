/************************************************************************
*
* copyright(c)2012-2015 Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc14.c
* Author 			:  zhengry
* version			: 
* DATE				:  20150811
* directory 			: 
* description			: ����NDK_RfidFelicaApdu ��Felica�������ݶ�д
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_RfidFelicaApdu"

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
*			  		       zhengry	        20150811	 	created
*****************************************************************/
void rfc14(void)
{
	int ret=0, rec_len=0, pUIDlen=0;
	uchar rec_buf[300]={0}, send_buf[300]={0}, UID[300]={0};
	
	//����send_buf
	send_buf[0] = 16;
	send_buf[1] = 0x06;
	send_buf[10] = 0x01;
	send_buf[11] = 0x09;
	send_buf[12] = 0x00;
	send_buf[13] = 0x01;
	send_buf[14] = 0x80;
	send_buf[15] = 0x00;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//Ѱ��
	cls_show_msg("���ڸ�Ӧ������Felica���������������...");
	if((ret=NDK_RfidFelicaPoll(UID, &pUIDlen))!=NDK_OK)
	{
		send_result("line %d:%sѰ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case1:��������
	if((ret=NDK_RfidFelicaApdu(-1, send_buf, &rec_len, rec_buf))!=NDK_ERR_PARA)//-1��������ܷ�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_RfidFelicaApdu(2, send_buf, &rec_len, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_RfidFelicaApdu(send_buf[0], NULL, &rec_len, rec_buf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_RfidFelicaApdu(send_buf[0], send_buf, NULL, rec_buf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_RfidFelicaApdu(send_buf[0], send_buf, &rec_len, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�쳣���ԣ�IDM��δ��ֵ����дӦ��ʧ��
	if((ret=NDK_RfidFelicaApdu(send_buf[0], send_buf, &rec_len, rec_buf))!=NDK_ERR_MI_NOTAGERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:��������:������֤��
	memcpy(&send_buf[2], &UID[2], 8);
	if((ret=NDK_RfidFelicaApdu(send_buf[0], send_buf, &rec_len, rec_buf))!=NDK_OK||rec_len!=rec_buf[0])
	{
		send_result("line %d:%s����֤��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(rec_buf[1]!=0x07||memcmp(send_buf+2, rec_buf+2, 8))
	{
		send_result("line %d:%s����֤��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:�쳣���ԣ����ͳ���Ϊ3��ʱ�򣬼��㹹�����sendbuf��ҲӦ��ʧ��
	if((ret=NDK_RfidFelicaApdu(3, send_buf, &rec_len, rec_buf))!=NDK_ERR_MI_NOTAGERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:�쳣���ԣ���Ӧ�������дӦ��ʧ��
	send_buf[1] = 0x20;
	if((ret=NDK_RfidFelicaApdu(send_buf[0], send_buf, &rec_len, rec_buf))!=NDK_ERR_MI_NOTAGERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�µ�
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	//case6:��δѰ��������£���дӦ��ʧ��
	if((ret=NDK_RfidFelicaApdu(send_buf[0], send_buf, &rec_len, rec_buf))!=NDK_ERR_MI_WriteFifo)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);
	return;

}



/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: nfcģ��
* file name			: nfc2.c
* Author 			:  huangjianbin
* version			: 
* DATE				:  20141215
* directory 			: 
* description			: ����NDK_Nfc_Activate����
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_Nfc_Activate"

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
*			  		       huangjianb	  20141215	created
*****************************************************************/

void nfc2(void)
{
	int ret=0, pnRecvlen=0, nSeekCnt=3;
	uchar psRecebuf[64] = {0};
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);
	
	//��������
	cls_show_msg("����ֻ�NFC���Թ��߲�����pos...,���������");
	if((ret=NDK_Nfc_Activate(NULL,&pnRecvlen,nSeekCnt)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_Nfc_Activate(psRecebuf,NULL,nSeekCnt)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if((ret=NDK_Nfc_Activate(psRecebuf,&pnRecvlen,0)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if((ret=NDK_Nfc_Activate(psRecebuf,&pnRecvlen,11)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//NFC������������
	if((ret=NDK_Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��ret=%d", __LINE__, TESTAPI, ret);
		RETURN;
	}

	/*
	//ѭ��ɨ�����
	for (j=0; j<5; j++) 
	{
		if ((NDK_Nfc_Activate(psRecebuf,&pnRecvlen)) == NDK_OK) 
			break;
	 
		sleep(1);
	}
	
	if (j == 5) 
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	*/
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}

/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util17.c
* Author 			: jiangym
* version			:
* DATE				:  
* directory 	:
* description	: ����NDK_Utf8ToGbk�ӿ��ܷ�UTF8ת��GBK
* related document	: NDK.chm
*
************************************************************************
* log			:
* Revision
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_Utf8ToGbk"

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
*			  	  	     jiangym         20150530 	   created
*****************************************************************/
void util17(void)
{
	//����Ϊ�ֲ�����
	int ret = 0, i = 0;
	uchar bufin[][64] = {
						{0XE4,0XB8,0XAD,0XE6,0X96,0X87,0XE7,0X83,0XAD,0XE7,0X82,0XB9,0XE7,0X9A,0X84,0XE9,0X80,0X9A,0XE8,0XAE,0XAF},//��������
						{0XE7,0XB9,0X81,0XE9,0XAB,0X94,0XE5,0XAD,0X97,0XE7,0X95,0XAB,0XE5,0XBB,0XA3,0XE5,0X9C,0X8B,0XE6,0XA8,0X82},	//�����֮��V����
						{0XE4,0XB8,0X80,0XE4,0XB8,0X81,0XE4,0XB8,0X83,0XE4,0XB8,0X87,0XE4,0XBB,0XAC,0XE4,0XBB,0XB0,0XE4,0XBB,0XB2},//���ǰ��:һ������������
						{0XE5,0XA2,0XBC,0XE5,0XA3,0X81,0XE5,0XA3,0X85,0XE5,0XA3,0X91,0XE6,0XAA,0XAC,0XE6,0XAC,0XA0,0XE6,0XAF,0XB3},//�в���Ƨ��:�Ա�������Ƿ�
						{0XE8,0XB7,0X84,0XE8,0XB7,0X86,0XE8,0XB7,0X8B,0XE9,0XBE,0X8C,0XE9,0XBE,0X99,0XE9,0XBE,0X9B,0XE9,0XBE,0X9F},//β��:���̰��������
						};
	uchar  bufin1[][64] = {"`~!\"@$#[^%]*() -_=+|\\&{}:;?,/><.",//�����ַ� 
						   "test"};//Ӣ���ַ�
	uchar  bufout[32] = {0};
	char  *cmpbuf[32] = {"�����ȵ��ͨѶ",
						 "���w�֮��V����",
						 "һ������������",
						 "�Ա�������Ƿ�",
						 "���̰��������"
						};
	uchar bufin_err[64] = {0XE4,0XB8,0XA0,0XE6,0X96,0X85,0XE7,0X8A,0XAD,0XE7,0X82,0XBE};//"�����ȵ�"��Ӧ�ı������
	uint bufinlen = 1, bufoutlen = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣����:�����쳣����ʧ��
	if((ret=NDK_Utf8ToGbk(NULL,bufinlen,bufout,&bufoutlen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_Utf8ToGbk((uchar *)"����",bufinlen,bufout,&bufoutlen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_Utf8ToGbk(bufin[0],0,bufout,&bufoutlen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_Utf8ToGbk(bufin[0],bufinlen,NULL,&bufoutlen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_Utf8ToGbk(bufin[0],bufinlen,bufout,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:��������:��������,������,Ӣ��,�����ַ��Ƿ�����ת��
	for(i=0;i<SZ_ARRAY(bufin);i++)
	{
		memset(bufout,0,sizeof(bufout));
		bufoutlen = 0;
		if((ret=NDK_Utf8ToGbk(bufin[i],strlen((char *)bufin[i]),bufout,&bufoutlen))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if(strcmp((char *)bufout,cmpbuf[i]) || bufoutlen != strlen((char *)bufout))
		{
			send_result("line %d:%s����ʧ��(%d,%s,%d)", __LINE__, TESTAPI, i, bufout, bufoutlen);
			RETURN;
		}
	}
	for(i=0;i<SZ_ARRAY(bufin1);i++)
	{
		memset(bufout,0,sizeof(bufout));
		bufoutlen = 0;
		if((ret=NDK_Utf8ToGbk(bufin1[i],strlen((char *)bufin1[i]),bufout,&bufoutlen))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if(strcmp((char *)bufout,(char *)bufin1[i]) || bufoutlen != strlen((char *)bufout))
		{
			send_result("line %d:%s����ʧ��(%d,%s,%d)", __LINE__, TESTAPI, i, bufout, bufoutlen);
			RETURN;
		}
	}

	//case3:������������£���Ӧ�ܷ�
	if((ret=NDK_Utf8ToGbk(bufin_err,strlen((char *)bufin_err),bufout,&bufoutlen))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
		
	send_result("%s����ͨ��", TESTAPI);
	return;
}


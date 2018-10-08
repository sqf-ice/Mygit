/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name		: util4.c
* Author 			: jiangym
* version			:
* DATE				: 20130108
* directory 	:
* description	: ���� NDK_AscToHex ��NDK_HexToAsc     �ӿڹ���
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
#define 	TESTAPI		"NDK_AscToHex,NDK_HexToAsc "

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
* history 		 		:
*****************************************************************/
void util4(void)
{
	//����Ϊ�ֲ�����
	uchar asciibuf[13] = {0}, asciibuf2[13] = {0}, bcdbuf[7] = {0};
	int len = 8, ret = 0;
	uchar type = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������

	//case1:�쳣����:��������ΪNULL�����Ӧ����ʧ��
	if((ret=NDK_AscToHex(NULL,len,type,bcdbuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AscToHex((uchar *)"30313233",-1,type,bcdbuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AscToHex((uchar *)"30313233",len,-1,bcdbuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AscToHex((uchar *)"30313233",len,type,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_HexToAsc(NULL,len,type,asciibuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_HexToAsc((uchar *)"13ab",-1,type,asciibuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_HexToAsc((uchar *)"13ab",4,-1,asciibuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_HexToAsc((uchar *)"13ab",4,type,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�����ַ���Ϊż�������,ʹ��0ģʽ
	memset(asciibuf,0,sizeof(asciibuf));
	memcpy(asciibuf,"3039617A415A",strlen("3039617A415A"));//ע��:�ַ��������Ǵ�д�ַ���,���򾭹�����ת��֮��Сд�ַ����ɴ�д�ַ����磺asciibuf="7a",asciibuf2="7A"
	memset(bcdbuf,0,sizeof(bcdbuf));
	if((ret=NDK_AscToHex(asciibuf,strlen((char *)asciibuf),0,bcdbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("09azAZ",(char *)bcdbuf))//ת����bcdbuf = {0x30,0x39,0x61,0x7a,0x41,0x5a}��Ӧ�ַ�"09azAZ"
	{
		send_result("line %d:%s����ʧ��,bcdbuf=%s", __LINE__, TESTAPI, bcdbuf);
		RETURN;
	}
	memset(asciibuf2,0,sizeof(asciibuf2));
	if((ret=NDK_HexToAsc(bcdbuf,12,0,asciibuf2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp((char *)asciibuf,(char *)asciibuf2))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case3:�����ַ���Ϊż�������,ʹ��1ģʽ(��BUG)
	memset(bcdbuf,0,sizeof(bcdbuf));
	if((ret=NDK_AscToHex(asciibuf,strlen((char *)asciibuf),1,bcdbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("09azAZ",(char *)bcdbuf))//ת����bcdbuf = {0x30,0x39,0x61,0x7a,0x41,0x5a}��Ӧ�ַ�"09azAZ"
	{
		send_result("line %d:%s����ʧ��,bcdbuf=%s", __LINE__, TESTAPI, bcdbuf);
		RETURN;//�˴���BUG��asciibuf����1ģʽת���������bcdbuf���"???039617A415A"
	}
	memset(asciibuf2,0,sizeof(asciibuf2));
	if((ret=NDK_HexToAsc(bcdbuf,12,1,asciibuf2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp((char *)asciibuf2,(char *)asciibuf))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case4:�����ַ���Ϊ���������,ʹ��0ģʽ(��BUG)
	memset(asciibuf,0,sizeof(asciibuf));
	memcpy(asciibuf,"50524",strlen("50524"));
	memset(bcdbuf,0,sizeof(bcdbuf));
	if((ret=NDK_AscToHex(asciibuf,strlen((char *)asciibuf),0,bcdbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("PR@",(char *)bcdbuf))//ת����bcdbuf = {0x50,0x52,0x40}��Ӧ�ַ�"PR@"
	{
		send_result("line %d:%s����ʧ��,bcdbuf=%s", __LINE__, TESTAPI, bcdbuf);
		RETURN;//�˴���BUG������bcdbuf��Ӧ�ַ���Ϊ"PR@"
	}
	memset(asciibuf2,0,sizeof(asciibuf2));
	if((ret=NDK_HexToAsc(bcdbuf,6,0,asciibuf2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp((char *)asciibuf2,"505240"))//������Ҳ�0
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case5:�����ַ���Ϊ���������,ʹ��1ģʽ
	memset(bcdbuf,0,sizeof(bcdbuf));  
	if((ret=NDK_AscToHex(asciibuf,strlen((char *)asciibuf),1,bcdbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp("$",(char *)bcdbuf))//ת����bcdbuf = {0x05,0x05,0x24}��Ӧ�ַ�"$"
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	memset(asciibuf2,0,sizeof(asciibuf2));
	if((ret=NDK_HexToAsc(bcdbuf,6,1,asciibuf2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(strcmp((char *)asciibuf2,"050524"))//�Ҷ�����0
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
      
	send_result("%s����ͨ��", TESTAPI);
	return;
}


/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt3.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����NDK_BTSetLocalName,NDK_BTGetLocalName�Ƿ�����������û�ȡ�����ı�������
* related document	: NDK.chm
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  TESTAPI		"NDK_BTSetLocalName,NDK_BTGetLocalName"
#if BTMFI_ENABLE||defined ME15CHX||defined ME30MH||defined ME30GX||defined ME15B||defined ME32||defined ME32GX||defined ME65||defined ME30S
#define  MAXNAME 		23
#elif defined ME30THM   //ͬ��΢�������Ƴ�����ԭ����15λ��Ϊ31λ 20180820
#define  MAXNAME 		31
#else
#define  MAXNAME 		15
#endif

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt3
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						jiangym			20140217		created
*****************************************************************/
void bt3(void)
{
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}

	/*private & local definition*/ 
	char *inbuf2[] ={"`~!\"@$#[^%]",  /* " ��Ҫ�и�\"  */
					 "*() -_=+|\\&",  /* \ ��Ҫ�и�\\  */
					 "{}:;?/><.,",	  /*��������ʱ��֧��*/
					 "ne#w;l@",		  /*�����ַ�+Ӣ��*/
					 "��b@��#t½",	  /*�����ַ�+Ӣ��+����*/
					 "����"};  		  /*������*/
#if BTMFI_ENABLE||defined ME15CHX ||defined ME30MH||defined ME30GX||defined ME15B||defined ME32||defined ME32GX||defined ME65||defined ME30S
	char inbuf1[MAXNAME+1] = {"newlandcompany012345678"}, abnorname[MAXNAME+2] = {"newlandcomputers12345678"};//����汾����������󳤶�Ϊ23, ME15CHX�Ⱥ�����Ʒ�����պ����׼Ҫ����ʵ��
#elif defined ME51
	char inbuf1[MAXNAME+1] = {"newlandcompany012345678"}, abnorname[MAXNAME+3] = {"newlandcomputers012345678"};//ME51 V1.4.0�̼��汾����֧������ֽ���24
#elif defined ME30THM
	char inbuf1[MAXNAME+1] = {"newlandcompany0123456789newland"}, abnorname[MAXNAME+2] = {"newlandcomputers123456789newland"};//ME30ͬ��΢���֧��31�ֽ�
#else
	char inbuf1[MAXNAME+1] = {"newlandcompany0"}, abnorname[MAXNAME+2] = {"newlandcomputers"};//���ڰ汾Ϊ15
#endif
	char inbuf[MAXNAME+1] = {"newland"}, outbuf[MAXNAME+1] = {0}, oldbuf[MAXNAME+1] = {0};
	int ret = -1, i = 0;

	/*process body*/
	cls_printf("����%s...", TESTAPI);
	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

#if !K21_ENABLE
	//case1:��������δ�򿪽������û�ȡӦ��ʧ��
	if((ret=NDK_BTSetLocalName(inbuf))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_BTGetLocalName(outbuf))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

	//����ǰ��:����������
	if((ret=NDK_PortOpen(PORT_NUM_BT, NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if(g_at_enable)//�����BM77ģ��������²�����֧��AT����ģʽ��BTģ�������ͬ
	{	
		//case3:δ��������ģʽ���� ��ȡʧ��
		if((ret=NDK_BTGetLocalName(outbuf))!=NDK_ERR_TIMEOUT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR3;
		}
		if((ret=NDK_BTSetLocalName(inbuf))!=NDK_ERR_TIMEOUT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR3;
		}
	}
	
	
	//����ǰ��:��������ģʽ����ܽ��л�ȡĬ�ϵ�������������
	if((ret=NDK_BTEnterCommand())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR2;
	}
	memset(oldbuf,0,sizeof(oldbuf));
	if((ret=NDK_BTGetLocalName(oldbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret, oldbuf);
		goto ERR2;
	}

	//case 4:�쳣����
	//case 4.1:name=NULL�����
	if((ret=NDK_BTSetLocalName(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTGetLocalName(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case 4.2:name�Ĵ�С������Χ���֧��15���ֽ� //����汾���֧��23�ֽ�
	if((ret=NDK_BTSetLocalName(abnorname))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:name֧�������ַ���������:!@#$%^<>�ȷ���
	for(i=0;i<SZ_ARRAY(inbuf2);i++)
	{
		if((ret=NDK_BTSetLocalName(inbuf2[i]))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//�������flash���·�������������������open���������Ʊ�ɾ����Ƶ����� 20160728 linwl
		NDK_PortClose(PORT_NUM_BT);
		NDK_PortOpen(PORT_NUM_BT, NULL);
		NDK_BTEnterCommand();
		memset(outbuf,0,sizeof(outbuf));
		if((ret=NDK_BTGetLocalName(outbuf))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(strcmp(outbuf, inbuf2[i]))
		{
			send_result("line %d:%s����ʧ��(%s,%s)", __LINE__, TESTAPI, outbuf, inbuf2[i]);
			GOTOERR;
		}
	}
	
	//case6:�����������ã�Ӧ�ö��ɹ�����ȡ��ʱ��Ӧ�û�ȡ����2�����õ�ֵ�����ÿ���ѡ�������ñȽϳ��ģ�������һ���̵�
	if((ret=NDK_BTSetLocalName(inbuf1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_BTSetLocalName(inbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalName(outbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(outbuf, inbuf))
	{
		send_result("line %d:%s����ʧ��(%s,%s)", __LINE__, TESTAPI, outbuf, inbuf);
		GOTOERR;
	}
	
	//case 7:��������//�ĳ�ʹ�ó��ֽ����û�ȡ�Ƚ� 20180820 modify
	if((ret=NDK_BTSetLocalName(inbuf1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalName(outbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(outbuf, inbuf1))
	{
		send_result("line %d:%s����ʧ��(%s,%s)", __LINE__, TESTAPI, outbuf, inbuf);
		goto ERR;
	}

	/*
	//case8:�������ֵ������Ƿ�֧������"����"  ͬʱʹ���ֻ���������Ӧ��������
	if((ret=NDK_BTSetLocalName(inbuf3))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(outbuf,0,sizeof(outbuf));
	if((ret=NDK_BTGetLocalName(outbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(outbuf, inbuf3))
	{
		send_result("line %d:%s����ʧ��(%s,%s)", __LINE__, TESTAPI, outbuf, inbuf3);
		GOTOERR;
	}
	//�˳�����ģʽBM77�����ѵ�����
	NDK_BTExitCommand();
	cls_show_msg("ʹ���ֻ���������������Ϊ\"����\"��pos,�����������");
	if(cls_show_msg("�ֻ���������pos���������Ƿ�Ϊ\"����\",��[Enter],��[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}*/
	
	//���Ժ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_BTEnterCommand();
	NDK_BTSetLocalName(oldbuf);
ERR2:
	NDK_BTExitCommand();
ERR3:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return;
}



/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name		: rfc3.c
* Author 			: chenfm
* version			: 
* DATE			: 20130108
* directory 		: 
* description		: ����NDK_RfidPiccActivate,NDK_RfidPiccDeactivate,NDK_RfidPiccState�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_RfidPiccActivate,NDK_RfidPiccDeactivate"

//,NDK_RfidPiccState�Ѳ�ʹ��,��Զ����OK,�ʲ�����

/*------------global variables definition-----------------------*/
#define WUCHASEC 0.03
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
*			  		    chenfm	        20130108	 	created
*****************************************************************/
void rfc3(void)
{
	//����Ϊ�ֲ�����
	uchar psPicctype=0x00, psDatabuf[256]={0}; 
	int ret= -1, ret1= -1, ret2= -1, pnDatalen = 0;
	float tmp = 0.0,unTime = 0.0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_RfidPiccDeactivate(0);
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case9:�쳣���ԣ��������	     
	if((ret=NDK_RfidPiccActivate(NULL, &pnDatalen, psDatabuf))!=NDK_ERR_PARA
	||(ret1=NDK_RfidPiccActivate(&psPicctype, NULL, psDatabuf))!=NDK_ERR_PARA
	||(ret2=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d,%d,%d)", __LINE__, TESTAPI, ret, ret1, ret2);
		GOTOERR;
	}

	//��������ֻ���ڲ���������ͬʱ(��,��Ҫ�Ų�һ���Ŀ�),����Ҫ��ʾһ��
	//case1:"����(�ϵ�)ǰδѰ��"
	//Ѱ���ɹ�,����ʧ��(��ָ���в��ɹ������),Ӧ��Ѱ��(�ײ����FSM������!)
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������...");
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("�뽫��Ӧ���ı�׼A���ƿ�,���������...");
	ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf);
	if(ret!=NDK_ERR_RFID_NOCARD)//&& state!=0x0a)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������...");
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_ERR_RFID_NOTDETE)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}
	
	//cls_show_msg1(2, "����������ͨ��!");//����������case����ͬ�Ĳ�����ʾ��Ϣ

	//case2:"��֧��ISO14443-4Э��Ŀ�"
	cls_show_msg("���ڸ�Ӧ������MIfare-1��,���������...");//MIfare-1��ֻ��A��һ������
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_ERR_RFID_PROTOCOL)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:"A����ͻ(���ſ�����)"
	//��Ѱ�������ϵ缤��ʱ��⵽0x03������
	//��ʵ�϶���A����ͻ����,���������,Ӧ��Ѱ��OK,���ϵ缤��ʱ��⵽��ͻ
	//���ڲ����з�����Щ���̵Ŀ�����Ѱ��ʱ����ǰ���ؼ�⵽��ͻ��
	//����B����ͻ����,�������Ӧ��Ѱ���׶ξͷ��ؼ�⵽��ͻ��
	cls_show_msg("���ڸ�Ӧ�����ö��ű�׼A��,���������...");
	if((ret=NDK_RfidPiccDetect(&psPicctype))==NDK_OK)//��Ѱ������0x00,�����ϵ缤��ʱһ��Ҫ��⵽����A��
	{
		if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_ERR_RFID_AANTI)
		{
			send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
			GOTOERR;
		}
	}
	else if(ret!=NDK_ERR_RFID_AANTI)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_RfidPiccDeactivate(0);//���ӱ���������ֹ�ɹ�Ѱ��һ�ſ�����º������������Ѽ���
	cls_show_msg1(2, "����������ͨ��!");

	//case4:����A��
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������...");
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
#if 0
	if((ret=NDK_RfidPiccState())!=NDK_ERR_RFID_NOTACTIV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}	
#endif
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=RFCARD_TYPE_A)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}
#if 0
	if((ret=NDK_RfidPiccState())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
#endif
	//��Ϊ�����������Ҫ����,����Ҫ���µ�.�����ڼ���̬(��״̬�������Խ��ж�д)��,
	//ֻ��ͨ���µ�ص���ʼ̬.����Ѱ��,�������ɹ�.
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if 0
	if((ret=NDK_RfidPiccState())!=NDK_ERR_RFID_NOTACTIV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
#endif
	//cls_show_msg1(2, "����������ͨ��!");//��case����ͬ�Ĳ�����ʾ��Ϣ��ͬ,������ʾ

	//case5:����B��
	cls_show_msg("���ڸ�Ӧ������1�ű�׼B��,���������...");
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
#if 0
	if((ret=NDK_RfidPiccState())!=NDK_ERR_RFID_NOTACTIV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}	
#endif
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=RFCARD_TYPE_B)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}
#if 0
	if((ret=NDK_RfidPiccState())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}	
#endif
	//cls_show_msg1(2, "����������ͨ��!");

	//case6:"�Ѿ�����(�ϵ�)"//�������Ѱ�������߼�����ټ���
	//cls_show_msg("���ڸ�Ӧ������1��B��,���������...");
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_ERR_RFID_UPED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_ERR_RFID_UPED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7:��RFCARD+1��ʽ�ر����������ǿ,����RFCARD+1���������ƺ�����
	//RFCARD+1:�ر�6ms����ٶȴ�,�����ܿ��Լӿ��ٴ�Ѱ��ʱ���ٶ�
#if !defined ME15B
	lib_StartStopwatch();
	unTime = 0;
#else
	unTime = lib_ReadStopwatch();
#endif
	if((ret=NDK_RfidPiccDeactivate(6))!=NDK_OK||((tmp=(lib_ReadStopwatch()-unTime-0.006))>WUCHASEC))
	{
		send_result("line %d:%s����ʧ��(%d,%f)", __LINE__, TESTAPI, ret, tmp);
		GOTOERR;
	}
#if 0
	if((ret=NDK_RfidPiccState())!=NDK_ERR_RFID_NOTACTIV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;		
	}
#endif
	cls_show_msg1(2, "����������ͨ��!");

	//case8:��RFCARD��ʽ�ر����������ǿ(��ȫ�ر�)
	//cls_show_msg("���ڸ�Ӧ������1��B��,���������...");
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=RFCARD_TYPE_B)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	ERR:NDK_RfidPiccDeactivate(0);
	return;
}


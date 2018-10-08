/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfidģ��
* file name		: rfc6.c
* Author 			: chenfm
* version			: 
* DATE			: 20130108
* directory 		: 
* description		: ����NDK_SetIgnoreProtocol,NDK_GetIgnoreProtocol�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SetIgnoreProtocol,NDK_GetIgnoreProtocol"

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
*			  		    chenfm	        20130108	 	created
*****************************************************************/
void rfc6(void)
{
	//����Ϊ�ֲ�����
	uchar psPicctype, psDatabuf[256]={0}, psRecebuf[256]={0};
	int ret= -1, ret1= -1, loop=0, pnDatalen=0, rcvlen=0;
	ushort sw=0;
#if 1	//���ŵĿ���֧��Select_1pay����,֧��ȡ���������
	uchar Select_1pay[]= {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
#else
	uchar Select_1pay[]={0x00, 0xA4, 0x04, 0x00, 0x0E, '1', 'P', 'A', 'Y', '.', 'S', 'Y', 'S', '.', 'D', 'D', 'F', '0', '1', 0x00};//��ѯCPU�����ļ�Ŀ¼�ṹ,mifare-1����֧��
#endif	

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_RfidPiccDeactivate(0);
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case1:�쳣����    
	if((ret=NDK_GetIgnoreProtocol(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:����Ϊ�����ж�,�Ա�/�Ǳ꿨���в���,���ֿ���Ӧ�����ɹ�
	//�������������÷�����(0),��ȡӦ�û�ȡ��������(0)  20140603 added by jiangym
	ret1= -1;
	if((ret=NDK_SetIgnoreProtocol(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SetIgnoreProtocol(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_GetIgnoreProtocol(&ret1))!=NDK_OK||ret1!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret);
		GOTOERR;
	}
	//����������(��0),��ȡӦ��������(��0),����Ҫ�������� 20140603 added by jiangym
	if((ret=NDK_SetIgnoreProtocol(rand()%MAX_SIGNINT+1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_GetIgnoreProtocol(&ret1))!=NDK_OK||ret1==0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret);
		GOTOERR;
	}
	//��������
	for(loop=0;loop<4;loop++)
	{
		if(loop<2)//��׼���Ĳ���
			cls_show_msg("���ڸ�Ӧ������1�ű�׼%c��,���������...", (loop%2==0)?'A':'B');
		else//�Ǳ꿨�Ĳ���//����û�зǱ���Կ�,�Ǳ�A��B���ɲ�����(����������ʹ��M-1ģ��Ǳ�A��,����iccrw_newʱ���ܻᱨ��)
		{
			if(ESC==cls_show_msg("���ڸ�Ӧ������1�ŷǱ�׼%c��,[ESC]�Թ�[����]����", (loop%2==0)?'A':'B'))
				continue;
		}
		if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=((loop%2==0)?RFCARD_TYPE_A:RFCARD_TYPE_B))
		{
			send_result("line %d:%s����ʧ��(%d, 0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
			GOTOERR;
		}
		if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, psRecebuf))!=NDK_OK||((sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x9000&&(sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x6d00))
		{
			send_result("line %d:%s����ʧ��(%d, 0x%04x)", __LINE__, TESTAPI, ret, sw);
			GOTOERR;
		}
		if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

	//case2:����Ϊ�����ж�,�Ա�/�Ǳ꿨���в���,�꿨������������,�Ǳ꿨�����
	//����
	ret1= -1;
	if((ret=NDK_SetIgnoreProtocol(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_GetIgnoreProtocol(&ret1))!=NDK_OK||ret1!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, ret1);
		GOTOERR;
	}
	//��������
	//��׼���Ĳ���
	for(loop=0;loop<2;loop++)
	{
		cls_show_msg("���ڸ�Ӧ������1�ű�׼%c��,���������...", loop==0?'A':'B');
		if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(0x%02x)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=(loop==0?RFCARD_TYPE_A:RFCARD_TYPE_B))
		{
			send_result("line %d:%s����ʧ��(%d, 0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
			GOTOERR;
		}
		if((ret=NDK_RfidPiccApdu(sizeof(Select_1pay), Select_1pay, &rcvlen, psRecebuf))!=NDK_OK||((sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x9000&&(sw=SMART_GETSW(psRecebuf+rcvlen-2))!=0x6d00))
		{
			send_result("line %d:%s����ʧ��(%d, 0x%04x)", __LINE__, TESTAPI, ret, sw);
			GOTOERR;
		}
		if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	//�Ǳ꿨�Ĳ���
	//����û�зǱ���Կ�,���Է����ɵ���Ϊʹ��M-1ģ��Ǳ�A��,�Ǳ�B��������
	for(loop=0;loop<2;loop++)
	{
		if(ESC==cls_show_msg("���ڸ�Ӧ������1�ŷǱ�׼%c��,[ESC]�Թ�[����]����", loop==0?'A':'B'))
			continue;
		//�Ǳ�(A��B)����Ѱ�������ϵ缤��ʱ����ʧ�ܶ�����
		//�����ж����������ſ�,��ʵ�϶���B��Ӧ��Ѱ��ʱ����ʧ��,�����ǵ��Ǳ꿨�Ķ�����,���Ƿſ��ж�����
		if((ret=NDK_RfidPiccDetect(&psPicctype))==NDK_OK)//��Ѱ������0x00,�����ϵ缤��ʱһ��Ҫ��⵽����A��?
		{
			if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_ERR_RFID_PROTOCOL)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
		}
		else if(ret!=NDK_ERR_RFID_PROTOCOL)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}	

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	goto ERR;
ERR:
	NDK_SetIgnoreProtocol(0);//�������Ա�,�ָ�Ĭ��//����Э���ж�
	NDK_RfidPiccDeactivate(0);
	return;
}


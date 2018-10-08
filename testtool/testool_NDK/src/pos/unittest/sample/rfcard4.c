/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: RF��ģ��
* file name		: 
* Author 			: zhangnw
* version			: V1.0
* DATE			: 20081107
* directory 		: .\SRC\RFcard\
* description		: ����iccrw_new������Ĺ���(APDU��д)
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  20081107 zhangnw
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  UCID  4
#define	TESTAPI		"iccrw_new"
#define	GetDS5002state()	((getDS5002state()&0x0000ff00)>>8)	//RFʹ��byte1

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
*			  		    zhangnw	        20091209	 	created
*****************************************************************/
void rfcard4(void)
{
	/*private & local definition*/
	int state = 0, rcvlen = 0;
	char atrbuf[300] = {0};
	char Select_1pay[] = 
#if 1	//���ŵĿ���֧��Select_1pay����,֧��ȡ���������
	{'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
#else
	uchar Select_1pay[] = 
	{0x00, 0xA4, 0x04, 0x00, 0x0E,
	  '1',  'P',  'A',  'Y',  '.',  'S',  'Y',  'S',  '.',  'D',  'D',  'F',  '0',  '1',
	  0x00
	};//��ѯCPU�����ļ�Ŀ¼�ṹ,mifare-1����֧��
#endif

	/*process body*/
	cls_printf("����%s...", TESTAPI);

	//����ǰ��
	icpowerdown(RFCARD);//����
	if(SetICType(0xff, RFCARD_TYPE_AB)!=SUCC)
	{
		send_result("line %d:SetICType(RFCARD_TYPE_AB)ʧ��", __LINE__);
		GOTOERR;
	}
	
	//case1:"-7 δ�ϵ�(Ѱ������)"
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������...");
	if((state=iccrw_new(RFCARD, sizeof(Select_1pay), Select_1pay, &rcvlen, atrbuf))!=-7)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, state);
		GOTOERR;
	}
	cls_show_msg1(2, "����������ͨ��!");//����������case����ͬ�Ĳ�����ʾ��Ϣ

	//case2:"-1 ��ʾ��дʧ��"
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������...");
	if((state=GetDS5002state())!=0x00)
	{
		send_result("line %d:Ѱ��ʧ��(0x%02x)", __LINE__, state);
		GOTOERR;
	}	
	if((state=icpowerup(RFCARD, atrbuf))!=0x00||atrbuf[0]!=RFCARD_TYPE_A)
	{
		send_result("line %d:���ʧ��(0x%02x, 0x%02x)", __LINE__, state, atrbuf[0]);
		GOTOERR;
	}	
	cls_show_msg("Ѱ���ɹ�!�뽫��Ӧ���ı�׼A���ƿ�,���������...");
	if((state=iccrw_new(RFCARD, sizeof(Select_1pay), Select_1pay, &rcvlen, atrbuf))!=-1)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, state);
		GOTOERR;	
	}
	//��Ȼiccrw_newʧ��,���Ѽ���ɹ�(�ײ㴦�ڼ���̬),����Ѱ������ʾ"�Ѿ�����(�ϵ�)"
	//��Ʊ����!(��EMV��ȫҪ��ִ��),����һ�������޷����Ե�
	if((state=GetDS5002state())!=0x0d)//��Ϊ�ײ�����ΪAPDU��������,���µ�,�ص���ʼ̬
	{
		send_result("line %d:Ѱ��Ӧʧ��(0x%02x)", __LINE__, state);
		GOTOERR;
	}
#if 0 //(����EMVҪ��),Ŀǰ����������ɵײ����!
	//"�����µ磨����Ѽ���״̬��־����Ȼ����Ѱ�������"
	if((state=icpowerdown(RFCARD))!=SUCC)
	{
		send_result("line %d:icpowerdownʧ��(%d)", __LINE__, state);
		GOTOERR;
	}
#endif

	//case3:A���Ķ�д
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������...");
	if((state=GetDS5002state())!=0x00)
	{
		send_result("line %d:Ѱ��ʧ��(0x%02x)", __LINE__, state);
		GOTOERR;
	}	
	if((state=icpowerup(RFCARD, atrbuf))!=0x00||atrbuf[0]!=RFCARD_TYPE_A)
	{
		send_result("line %d:���ʧ��(0x%02x, 0x%02x)", __LINE__, state, atrbuf[0]);
		GOTOERR;
	}	
	if((state=iccrw_new(RFCARD, sizeof(Select_1pay), Select_1pay, &rcvlen, atrbuf))!=SUCC
		||atrbuf[rcvlen-2]!=0x90||atrbuf[rcvlen-1]!=0x00)
	{
		send_result("line %d:%s����ʧ��(%d, 0x%02x, 0x%02x)", __LINE__, TESTAPI, state, atrbuf[rcvlen-2], atrbuf[rcvlen-1]);
		GOTOERR;
	}
	//�����������Ҫ��B�����ԣ�Ӧ���µ�رճ�
	if((state=icpowerdown(RFCARD))!=SUCC)
	{
		send_result("line %d:icpowerdownʧ��(%d)", __LINE__, state);
		GOTOERR;
	}	

	//case4:��׼B���Ķ�д
	cls_show_msg("���ڸ�Ӧ������1�ű�׼B��,���������...");
	if((state=GetDS5002state())!=0x00)
	{
		send_result("line %d:Ѱ��ʧ��(0x%02x)", __LINE__, state);
		GOTOERR;
	}
	if((state=icpowerup(RFCARD, atrbuf))!=0x00||atrbuf[0]!=RFCARD_TYPE_B)
	{
		send_result("line %d:���ʧ��(0x%02x, 0x%02x)", __LINE__, state, atrbuf[0]);
		GOTOERR;
	}
	if((state=iccrw_new(RFCARD, sizeof(Select_1pay), Select_1pay, &rcvlen, atrbuf))!=SUCC
		||atrbuf[rcvlen-2]!=0x90||atrbuf[rcvlen-1]!=0x00)
	{
		send_result("line %d:%s����ʧ��(%d, 0x%02x, 0x%02x)", __LINE__, TESTAPI, state, atrbuf[rcvlen-2], atrbuf[rcvlen-1]);
		GOTOERR;
	}
	//�����������Ҫ�������ԣ�Ӧ���µ�رճ�
	if((state=icpowerdown(RFCARD))!=SUCC)
	{
		send_result("line %d:icpowerdownʧ��(%d)", __LINE__, state);
		GOTOERR;
	}	

#if defined GP730	//||defined NL8510GP||defined GP730CP
	//case5:����B���Ķ�д
	/*
	Ҫ�����е�ͳһAPI��д���ŵĿ�����Ҫ�ڳ�ʼʱ(GetDS5002stateǰ)����һ������
	setpiccquickseek(1);�����׵Ŀ���Ѱ��
	Ԥ������ӿ���Ҫ��2��Ŀ�ģ�
	1�����������ķǱ�׼��
	2��ĳЩ��EMV���ԣ�ǿ��Ѱ���ٶȵȵĲ���
	��Ȼ,�Ա�׼��B�����ǵ���������ӿ�,Ҳ���������������.�����ŵĿ����Ǳ�׼��,����������ΪΪ��׼��,������һ�㲻����ʹ�õ��˽ӿ�
	*/
	if(cls_show_msg("���ڸ�Ӧ������1�ź���B��\n[ESC]�Թ�,[����]����")!=ESC)
	{
		if((state=GetDS5002state())!=0x0b)//0x0b��ʾѰ(ѡ)��ʧ��//==0x00)
		{
			send_result("line %d:Ѱ��Ӧʧ��(0x%02x)", __LINE__, state);
			GOTOERR;
		}
		if((state=setpiccquickseek(1))!=SUCC)
		{
			send_result("line %d:���׿���Ѱ��ʧ��(0x%02x)", __LINE__, state);
			GOTOERR;
		}
		if((state=GetDS5002state())!=0x00)
		{
			send_result("line %d:Ѱ��ʧ��(0x%02x)", __LINE__, state);
			GOTOERR;
		}
		if((state=icpowerup(RFCARD, atrbuf))!=0x00||atrbuf[0]!=RFCARD_TYPE_B)
		{
			send_result("line %d:���ʧ��(0x%02x, 0x%02x)", __LINE__, state, atrbuf[0]);
			GOTOERR;
		}	
		if((state=iccrw_new(RFCARD, sizeof(Select_1pay), Select_1pay, &rcvlen, atrbuf))!=SUCC
			||atrbuf[rcvlen-2]!=0x90||atrbuf[rcvlen-1]!=0x00)
		{
			send_result("line %d:%s����ʧ��(%d, 0x%02x, 0x%02x)", __LINE__, TESTAPI, state, atrbuf[rcvlen-2], atrbuf[rcvlen-1]);
			GOTOERR;
		}
	}
#endif

	//test over
	send_result("%s����ͨ��", TESTAPI);
	goto ERR;
ERR:
#if defined GP730	//||defined NL8510GP||defined GP730CP
	setpiccquickseek(0);//�رռ��׿���Ѱ������,�����µ��Ҳ��ָ�Ĭ������0.�˴���Ϊ�˷�ֹʹ�ú���B�������׼B��ʱ���Ի�ͨ��
#endif
	icpowerdown(RFCARD);
	return;
}


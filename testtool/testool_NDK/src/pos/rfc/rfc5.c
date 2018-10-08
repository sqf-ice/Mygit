/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name		: rfc5.c
* Author 			: chenfm
* version			: 
* DATE			: 20130108
* directory 		: 
* description		: ����NDK_RfidPiccState,NDK_RfidPiccDetect�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_RfidPiccDetect"
#define MAXWAITTIME  10

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
void rfc5(void)
{
	//����Ϊ�ֲ�����
	uchar psPicctype= 0x00, psDatabuf[256] ={ 0}; 
	int ret= -1, pnDatalen = 0, loop = 0;
	time_t oldtime = 0, diff = 0;
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_RfidPiccDeactivate(0);
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//ע��:case1����case6��֤����
#if 0	//�ýӿ���ƵĲ�������(����������ύ���̵�ԭ��,�Ա���������ò���),�ʲ�����NULL����
	//case1:�쳣����
	if((ret=NDK_RfidPiccDetect(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case1:"�޿�"
	cls_show_msg("��ȷ����Ӧ����RF��,�����������...");
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_ERR_RFID_NOCARD)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	/*
	A����B����һ����A����(�ϵ�)��������лᷢ�ֳ�ͻ��
	A�������Ͽ�����Ѱ���׶η��ֳ�ͻ����һ�����������Ϳ�������һ�������⣬������������
	ʵ�ʵķ���ͻ����Ҳ�����ϵ�����вŽ��С�
	*/
#if 0	//�ŵ��ϵ缤���в��Ըô�����
	//case2:"A��Ѱ��ʧ��(���ܶ��ſ�����)"
	//"A����ͻ(���ſ�����)"
	cls_show_msg("���ڸ�Ӧ�����ö��ű�׼A��,���������...");
	cls_show_msg("line %d:%d", __LINE__, NDK_RfidPiccDetect(&psPicctype));
	ret = NDK_RfidPiccDetect(&psPicctype);
	if(ret!=NDK_ERR_RFID_ASEEK /* && state!=0x03*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	//case3:"A��B��ͬʱ����"
	cls_show_msg("���ڸ�Ӧ�����ñ�׼A����B��,�����������...");
#if 1
	if((ret=NDK_RfidPiccDetect(&psPicctype))==NDK_OK)//��Ѱ������0x00,�����ϵ缤��ʱһ��Ҫ��⵽���ſ�
	{
		if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_ERR_RFID_ABON)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			NDK_RfidPiccDeactivate(0);//��������,��һ�ϵ�ɹ�Ϊ��Ӱ��������� ���µ翨Ƭ 20160726 linwl
			GOTOERR;
		}
	}
	else if(ret!=NDK_ERR_RFID_ABON)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
#else
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_ERR_RFID_ABON)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//�ŵ�icpowerup�в��Ըô�����0x0f,��getDS5002state�������Ϊ����:
	//��A����,Ҫ�ڼ���ʱ�ŷ���0x0f(BYTE0),����B����,��Ѱ��ʱ��BYTE1����0x0f
#if 0 
	//case4:"��֧��ISO14443-4Э��Ŀ�"
	cls_show_msg("���ڸ�Ӧ������MIfare-1��,���������...");//MIfare-1��ֻ��A��һ������
	//cls_show_msg("line %d:0x%8x", __LINE__, getDS5002state());
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_ERR_RFID_PROTOCOL)
	{
		send_result("line %d:%s����ʧ��(0x%02x)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case5:"�п�"(��Ȼ�ӷ���ֵ���޷�������Ѱ��A����B,�����Ƿֱ�����ֿ�Ѱһ��,��֤�����걸��)
	for(loop=0;loop<2;loop++)
	{
		cls_show_msg("���ڸ�Ӧ������1�ű�׼%s��,�����������...", (loop==0)?"A":"B");
		if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
			GOTOERR;
		}	
	}

	//case6:Ѱ��
	//ͨ����ʽ���Գ��Զ���,����������.�����ɿ������ۺϲ�����ʵ��
	cls_show_msg("��ȷ����Ӧ����RF��,�����������...��ȴ�%ds", MAXWAITTIME);
	oldtime=time(NULL);
	while((diff=time(NULL)-oldtime)<MAXWAITTIME)
	{
		cls_printf("��ȷ����Ӧ����RF��,��ȴ�%ds", MAXWAITTIME-diff);
		if((ret=NDK_RfidPiccDetect(NULL/*&psPicctype*/))!=NDK_ERR_RFID_NOCARD)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	for(loop=0;loop<2;loop++)
	{
		cls_show_msg("����%ds����1�ű�׼%s�����޸���ͨ����Ӧ��,�����������ʱ��ʼ!", MAXWAITTIME, (loop==0)?"A":"B");
		cls_printf("Ѱ��...");
		oldtime=time(NULL);
		ret=0xff;
		while(time(NULL)-oldtime<MAXWAITTIME)//GetDS5002state����������,��ҪӦ�ò��ϲ�ѯ
		{
			if((ret=NDK_RfidPiccDetect(&psPicctype))==NDK_OK)
				break;
		}
		if(ret==NDK_OK)
		{
			NDK_SysBeep();
			show_msg1(1, "�ɹ�!");
		}
		else
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

#if 1
	//case7:"B��Ѱ��ʧ��(���ܶ��ſ�����)"
	cls_show_msg("���ڸ�Ӧ�����ö��ű�׼B��,�����������...");
	if((ret=NDK_RfidPiccDetect(&psPicctype))!=NDK_ERR_RFID_BSEEK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	
#endif

	//����ֵһ������²��׳���,����.���в���ֵ�ڼ��������в�
	//����,resetDS��RF�Ƿ���Ӱ��δ֪,RFģ�鲻���Ǵ�����

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);
	return;
}


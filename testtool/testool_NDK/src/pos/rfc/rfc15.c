/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: rfcģ��
* file name			: rfc15.c
* Author 			:  
* version			: 
* DATE				:  
* directory 			: 
* description		: ����NDK_RfidPiccDetect_Atq�ܷ���������ATQA��ATQB(����Ѱ���Ļ�����������������ֵ)
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_RfidPiccDetect_Atq"
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
*			  		   jiangym				20171110		created
*****************************************************************/
void rfc15(void)
{
	//����Ϊ�ֲ�����
	uchar psPicctype= 0x00, Atqbuf[24] = {0}, psDatabuf[256] ={ 0}; 
	int Atqlen = 0;
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

	//case1:�����쳣
	if((ret=NDK_RfidPiccDetect_Atq(NULL,&Atqlen,Atqbuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDetect_Atq(&psPicctype,NULL,Atqbuf))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_RfidPiccDetect_Atq(&psPicctype,&Atqlen,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:�޿�ƬʱӦ�÷���NoCard
	cls_show_msg("��ȷ����Ӧ����RF��,�����������...");
	if((ret=NDK_RfidPiccDetect_Atq(&psPicctype,&Atqlen,Atqbuf))!=NDK_ERR_RFID_NOCARD)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:"A��B��ͬʱ����"
	cls_show_msg("���ڸ�Ӧ�����ñ�׼A����B��,�����������...");
	if((ret=NDK_RfidPiccDetect_Atq(&psPicctype,&Atqlen,Atqbuf))==NDK_OK)//��Ѱ������0x00,�����ϵ缤��ʱһ��Ҫ��⵽���ſ�
	{
		if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_ERR_RFID_ABON)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			NDK_RfidPiccDeactivate(0);//��������,��һ�ϵ�ɹ�Ϊ��Ӱ��������� ���µ翨Ƭ
			GOTOERR;
		}
	}
	else if(ret!=NDK_ERR_RFID_ABON)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	

	//case5:�п�,A������2�ֽ�,B������12�ֽ�
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,�����������...");
	memset(Atqbuf,0,sizeof(Atqbuf));
	if((ret=NDK_RfidPiccDetect_Atq(&psPicctype,&Atqlen,Atqbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}	
	//�������
	if(Atqlen != 2)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, Atqlen);
		GOTOERR;
	}
	if(cls_show_msg("���ص�ATQֵΪ:0x%02x:0x%02x,��ȷ���Ƿ���ȷ,��ȷ��[Enter],����ȷ��[����]",Atqbuf[0],Atqbuf[1])!=ENTER)
	{
		send_result("line %d:%s����ʧ��(0x%02x:0x%02x)", __LINE__, TESTAPI, Atqbuf[0],Atqbuf[1]);
		GOTOERR;
	}
	
	cls_show_msg("���ڸ�Ӧ������1�ű�׼B��,�����������...");
	memset(Atqbuf,0,sizeof(Atqbuf));
	if((ret=NDK_RfidPiccDetect_Atq(&psPicctype,&Atqlen,Atqbuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		GOTOERR;
	}	
	//�������
	if(Atqlen != 12)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, Atqlen);
		GOTOERR;
	}
	if(cls_show_msg("���ص�ATQֵΪ:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x,��ȷ���Ƿ���ȷ,��ȷ��[Enter],����ȷ��[����]",Atqbuf[0],Atqbuf[1],Atqbuf[2],Atqbuf[3],Atqbuf[4],Atqbuf[5],Atqbuf[6],Atqbuf[7],Atqbuf[8],Atqbuf[9],Atqbuf[10],Atqbuf[11])!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x)", __LINE__, TESTAPI, Atqbuf[0],Atqbuf[1],Atqbuf[2],Atqbuf[3],Atqbuf[4],Atqbuf[5],Atqbuf[6],Atqbuf[7],Atqbuf[8],Atqbuf[9],Atqbuf[10],Atqbuf[11]);
		GOTOERR;
	}

	//case6:Ѱ��
	//ͨ����ʽ���Գ��Զ���,����������.�����ɿ������ۺϲ�����ʵ��
	cls_show_msg("��ȷ����Ӧ����RF��,�����������...��ȴ�%ds", MAXWAITTIME);
	oldtime=time(NULL);
	while((diff=time(NULL)-oldtime)<MAXWAITTIME)
	{
		cls_printf("��ȷ����Ӧ����RF��,��ȴ�%ds", MAXWAITTIME-diff);
		memset(Atqbuf,0,sizeof(Atqbuf));
		if((ret=NDK_RfidPiccDetect_Atq(&psPicctype,&Atqlen,Atqbuf))!=NDK_ERR_RFID_NOCARD)
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
		memset(Atqbuf,0,sizeof(Atqbuf));
		while(time(NULL)-oldtime<MAXWAITTIME)//GetDS5002state����������,��ҪӦ�ò��ϲ�ѯ
		{
			if((ret=NDK_RfidPiccDetect_Atq(&psPicctype,&Atqlen,Atqbuf))==NDK_OK)
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

	//case7:"B��Ѱ��ʧ��(���ܶ��ſ�����)"
	cls_show_msg("���ڸ�Ӧ�����ö��ű�׼B��,�����������...");
	if((ret=NDK_RfidPiccDetect_Atq(&psPicctype,&Atqlen,Atqbuf))!=NDK_ERR_RFID_BSEEK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	

	//����ֵһ������²��׳���,����.���в���ֵ�ڼ��������в�
	//����,resetDS��RF�Ƿ���Ӱ��δ֪,RFģ�鲻���Ǵ�����

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);
	return;
}


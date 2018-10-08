/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����ģ��
* file name		: wlm9.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ����NDK_WLCheckSIM�ӿڹ���
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_WLCheckSIM"	
#define	CHECK_TIMEOUT	2000
#define	MAXWAITTIME	30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlm9(void)
* functional description 			:   
* input parameter	 				:��
* output parameter	 			:��
* return value		 			:��
* history 		 				:  author		date		remarks
*			  	  	  			jiangym		20160726 created
*****************************************************************/
void wlm9(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = -1;
	int nKeyin = 0;
	char skip = 0x00;
	
#if !(defined ME50NX||defined ME68)
	int flag_4g = 0;
	char *p=NULL;
	char szOutbuf[1024] = {0};//������������������������Ӱ�죬Ӧ���Ǹ�BUG
	ST_ATCMD_PACK STatcmdpack = {0};
	EM_WLM_STATUS emStatus = 0;
	//�ж��Ƿ���EC20ģ�飬EC20�����ͳ�ʼ��ʱ��ϳ�����λ���ʱ���ݶ�20S  linying20180628 added
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CGMR;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((ret = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((p=strstr(szOutbuf,"EC20"))!=NULL)
		flag_4g = 1;
	else
		flag_4g = 0;
#endif

	if (cls_show_msg1(MAXWAITTIME, "����%s...����ȷ���������Ѳ���������SIM��,ENTERȷ��", TESTAPI)==ENTER)//��Ϊ��������ȷ���˲���������
	{
		//case1:����ģ���޹رյĺ���,��������ڴ��Ѿ��г�ʼ����  �����⿨�᷵�سɹ�
		if((ret=NDK_WLCheckSIM(NULL,CHECK_TIMEOUT))!= NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		//����Ӳ����λ
#if !(defined ME50NX||defined ME68) //ME50NX��֧��NDK_WlModemReset 20171113
		if((ret=NDK_WlModemReset())!=NDK_OK)//�忨�����������ϵ�
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		//case2:���ϵ��ü�⿨�᷵��GPRSδ��ʼ��
		if((ret=NDK_WLCheckSIM(NULL,CHECK_TIMEOUT))!= NDK_ERR_GPRS_UNINIT)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}	
		sleep(2);//��ʱ2s,���ʱ����ʱ���ܻ�Ӱ����Խ��,������Ա����ʱ�������������ʵ�����:nRet==NDK_ERR_GPRS_UNINITҪ�Ĵ�ʱ��,nRet==NDK_OKҪ��Сʱ��
		//case3:��λδ���ʱ��⿨�᷵��ϵͳ������δ����
        if(flag_4g) //EC20ģ������ܼ�⵽NDK_ERR_GPRS_BUSY״̬�ʻ��ǻ�ȡNDK_ERR_GPRS_UNINIT
		{
			if((ret=NDK_WLCheckSIM(NULL,CHECK_TIMEOUT))!= NDK_ERR_GPRS_UNINIT)
			{
				send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}	
        	}
		else
		{
			if((ret=NDK_WLCheckSIM(NULL,CHECK_TIMEOUT))!= NDK_ERR_GPRS_BUSY)
		    {
				send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				RETURN;
	        }
		}
		sleep(15);//�ȴ���λ���
#endif

		//case4:��λ��ɺ�Ӧ���ܹ���⵽��
		if((ret=NDK_WLCheckSIM(NULL,CHECK_TIMEOUT))!= NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}	
	}
		
	//case5:�޿�����¼�ⲻ����
	nKeyin = cls_show_msg1(MAXWAITTIME, "����%s...����ȷ��������û��SIM��(�ػ��ٰο�),ENTERȷ���޿�", TESTAPI);//�ο�
	if (nKeyin==ENTER)//��Ϊ��������ȷ�����޿�
	{
		if((ret=NDK_WLCheckSIM("1234",CHECK_TIMEOUT))!= NDK_ERR_NO_SIMCARD)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}	
	}
	else if (nKeyin==0)//��ʱ
	{
		skip |= 0x01;
	}
	else
	{
		send_result("line %d:�û��жϲ���",  __LINE__);
		RETURN;
	}
	
	//�������Ŀ�Ƭ����
	nKeyin = cls_show_msg1(MAXWAITTIME, "����%s...����ȷ���������Ѳ����˱���SIM��(�ػ��ٲ忨),ENTERȷ�ϲ���", TESTAPI);//���ϱ�����SIM��
	if (nKeyin==ENTER)//��Ϊ��������ȷ���˲��뱻����
	{
		//case6:����NDK_ERR_PIN_LOCKED,PIN������(�ڿ���������ΪNULL��PIN��ȥ���������)
		if((ret=NDK_WLCheckSIM(NULL,CHECK_TIMEOUT))!= NDK_ERR_PIN_LOCKED)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}

		//case7:����NDK_ERR_PIN,PIN���������(�ڿ���������Ϊ�����PIN ��ȥ���������)
		if((ret=NDK_WLCheckSIM("test",CHECK_TIMEOUT))!= NDK_ERR_PIN)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		
		//case8:����NDK_OK,��������(һ�㳧Ĭ����1234),�����ĺ�ʹ����ȷ��PIN����н���Ӧ�óɹ�
		if((ret=NDK_WLCheckSIM("1234",CHECK_TIMEOUT))!= NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}		
	}
	else if (nKeyin==0)
	{
		skip |= (0x01<<1);
	}
	else
	{
		send_result("line %d:�û��жϲ���", __LINE__);
		RETURN;
	}
	
	//test over 
	if (skip!=0x00)
		send_result("%s����δ����(%d)", TESTAPI, skip);
	else
		send_result("%s����ͨ��", TESTAPI);
	return;		
}


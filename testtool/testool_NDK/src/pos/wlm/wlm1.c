/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����ģ��
* file name		: wlm1.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_WlInit�ӿڹ���
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
#define TESTAPI	"NDK_WlInit"	
#define	WLM_TIMEOUT	2000
#define	MAXWAITTIME	30

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlm1(void)
* functional description 	: ����MODEM��ʼ�����л����ڵ����߲��ж�ģ��ATָ���ܷ�������Ӧ�����SIM�� 
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void wlm1(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	/*private & local definition*/
	int nKeyin = 0;
	char skip = 0x00;
	EM_WLM_STATUS emStatus = -1;
	int nRet=0;
	
	/*process body*/
	/*���β����������:
	-201  �޿�   ��Ӧ�ò�һ�����������ֵʱ�п�����ģ����𿨻�û���ϵ磬�����ü��ν�����֤��
	��������������Ǽ��ʱ���ֵ�������������ڽ���ʱ���ֵ����
	-203  PIN������(�ڿ���������ΪNULL��PIN��ȥ���������)
	-202  PIN���������(�ڿ���������Ϊ�����PIN��ȥ���������)
	-204  δ��������
	*/
	//case1:�쳣��������
	if((nRet = NDK_WlInit(-1,NULL,&emStatus)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	if((nRet = NDK_WlInit(WLM_TIMEOUT,NULL,NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}

	//������Ƭ�Ĳ���
	if((nRet=NDK_WlModemClose())!=NDK_OK)//�ر�����ģ��
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "����%s...����ȷ���������Ѳ���������SIM��,ENTERȷ��", TESTAPI);//��������SIM��
	if (nKeyin==ENTER)//��Ϊ��������ȷ���˲���������
	{
#if  !(CPU5810X_ENABLE||defined ME50NX||defined ME68) //�Ͷ�����ģ����ƿ������Զ��ϵ��ʼ��,�Ҳ��ṩ�رպ���,������������
		//case7:����ģ��δ������£���ʼ��Ӧ�÷���AT������Ӧʧ��NDK_ERR_WLM_SEND_AT_FAIL 20140617 added linwl
		if ((nRet = NDK_WlInit(WLM_TIMEOUT,NULL,&emStatus))!=NDK_ERR_WLM_SEND_AT_FAIL||emStatus==WLM_STATUS_OK)//NDK_ERR_WLM_SEND_AT_FAIL
		{
			send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, nRet, emStatus);
			RETURN;
		}
#endif
#if !(defined ME68 || defined ME50NX) //AC35ƽ̨��Ʒ��֧�ָ�λ  20180416 sull modify
		if((nRet=NDK_WlModemReset())!=NDK_OK)//�忨�����������ϵ�
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		sleep(20);//�ȴ���λ��� ��λʱ���ģ�鲻һ����Ŀ�����Ҫ20s 20170921 
#endif
		//case6:��������SIM��Ӧ���ܹ���ʼ���ɹ�
		if ((nRet = NDK_WlInit(WLM_TIMEOUT,NULL,&emStatus))!=NDK_OK||emStatus!=WLM_STATUS_OK)
		{
			send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, nRet, emStatus);
			RETURN;
		}
	}
	
	//�޿��������
	if((nRet=NDK_WlModemClose())!=NDK_OK)//�ر�����ģ��
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "����%s...����ȷ��������û��SIM��(�ػ���ο�),ENTERȷ���޿�", TESTAPI);//�ο�
	if (nKeyin==ENTER)//��Ϊ��������ȷ�����޿�
	{
#if !(defined ME68 || defined ME50NX) //AC35ƽ̨��Ʒ��֧�ָ�λ  20180416 sull modify
		if((nRet=NDK_WlModemReset())!=NDK_OK)//�ο������������ϵ�
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		sleep(20);//�ȴ���λ��� ��λʱ���ģ�鲻һ����Ŀ�����Ҫ20s 20170921 
#endif
		//case2:����NDK_ERR_NO_SIMCARD,�޿�
		if ((nRet = NDK_WlInit(WLM_TIMEOUT,"1234",&emStatus))!=NDK_ERR_NO_SIMCARD)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
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
	if((nRet=NDK_WlModemClose())!=NDK_OK)//�ر�����ģ��
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "����%s...����ȷ���������Ѳ����˱���SIM��(�ػ���β忨),ENTERȷ��", TESTAPI);//���ϱ�����SIM��
	if (nKeyin==ENTER)//��Ϊ��������ȷ���˲��뱻����
	{
#if !(defined ME68 || defined ME50NX) //AC35ƽ̨��Ʒ��֧�ָ�λ  20180416 sull modify
		if((nRet=NDK_WlModemReset())!=NDK_OK)//�忨�����������ϵ�
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		sleep(20);//�ȴ���λ��� ��λʱ���ģ�鲻һ����Ŀ�����Ҫ20s 20170921 
#endif

		//case3:����NDK_ERR_PIN_LOCKED,PIN������(�ڿ���������ΪNULL��PIN��ȥ���������)
		if ((nRet = NDK_WlInit(WLM_TIMEOUT,NULL,&emStatus))!=NDK_ERR_PIN_LOCKED)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}

		//case4:����NDK_ERR_PIN,PIN���������(�ڿ���������Ϊ�����PIN ��ȥ���������)
		if ((nRet = NDK_WlInit(WLM_TIMEOUT,"test",&emStatus))!=NDK_ERR_PIN)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		
		//case5:����NDK_OK,��������(һ�㳧Ĭ����1234),�����ĺ�ʹ����ȷ��PIN����н���Ӧ�óɹ�
		if ((nRet = NDK_WlInit(WLM_TIMEOUT,"1234",&emStatus))!=NDK_OK)
		{
			send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
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
		NDK_WlModemReset();//����ȡ������ʱ����ģ��û�и�λ�����ڹر�״̬�����������ʧ�� 20140707 linwl
		sleep(20);//�ȴ���λ���  ��λʱ���ģ�鲻һ����Ŀ�����Ҫ20s 20170921 
		RETURN;
	}
	
	//NDK_ERR_PIN_UNDEFINEδ����������ʱ�޷�����
	//emStatus״̬��������״ֵ̬��ô����

	//test over 
	if (skip!=0x00)
		send_result("%s����δ����(%d)", TESTAPI, skip);
	else
		send_result("%s����ͨ��", TESTAPI);
	return;
	
}


/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys24.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ����NDK_SysSetShutdownDelay�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysSetShutdownDelay"

#if defined ME50C
#define CONFIG_RTC_AUTO_SHUTDOWN (2*128)
#elif defined ME50N||defined SP610
#define CONFIG_RTC_AUTO_SHUTDOWN (5*60)
#else
#define CONFIG_RTC_AUTO_SHUTDOWN (128)
#endif

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
*			  		    	  jiangym		20140514   created
*****************************************************************/
#if 1//defined ME11||defined ME15||defined ME20||defined ME15C
void sys24(void)
{
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	
	//����Ϊ�ֲ�����
	int nRet = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 
	
#if defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B	//������
	NDK_PortOpen(PORT_NUM_BT, NULL);
	NDK_BTEnterCommand();
	NDK_BTSetLocalName("ME15");
	NDK_BTExitCommand();
#endif 

#if !(defined SP610||defined ME15B||defined ME66)	//SP610Ĭ�����߲��ػ�
	if(cls_show_msg("�Ƿ����:Ĭ������5���Ӻ�ػ��������ԣ��ǰ�ȷ�ϼ�������������")==ENTER)
	{
		if(cls_show_msg("�������迪���״ν�����������в���,[Enter]��,[����]��")==ENTER)
		{
			//case1:δ�������ߺ�ػ�ʱ��Ĭ����5����
			NDK_LedStatus(LED_RFID_BLUE_ON);//ͨ��ָʾ���ж��Ƿ�ػ�
#if defined ME65//�뿪��ȷ�ϣ�ME65���ߺ�LED�ƻ����          		20180725    wangk     modify		
			cls_show_msg("��ʱ���Ƴ���(�޵��豸����Ե�),��������������ߺ�,�������,�ε����,����5���Ӻ�Ӧ���Զ��ػ�");
#else
			cls_show_msg("���Ƴ���(�޵��豸����Ե�),���������������,�ε����,����5���Ӻ�Ӧ���Զ��ػ�");
#endif
			if((nRet=NDK_SysGoSuspend()) != NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
				RETURN;
			}
			sleep(1);
		}
	}
#endif

	if(cls_show_msg("�Ƿ����:���߲��ػ��������ԣ��ǰ�ȷ�ϼ�������������")==ENTER)
	{
		//case2:����0����ػ������Ӧ�ó���5��������
		NDK_LedStatus(LED_RFID_BLUE_ON);//ͨ��ָʾ���ж��Ƿ�ػ�
#if defined ME65//�뿪��ȷ�ϣ�ME65���ߺ�LED�ƻ����          		20180725    wangk     modify
		cls_show_msg("���������������,�������(�޵��豸����Ե�),����5���Ӻ���(���ѷ������ݲ�Ʒ����ѡ������:��������,�������ѵ�)");
#else
		cls_show_msg("���������������,���Ƴ���5��������(�޵��豸����Ե�),����5���Ӻ���(���ѷ������ݲ�Ʒ����ѡ������:��������,�������ѵ�)�����Ѻ����ƽ���˸");
#endif
#if !defined SP610	//SP610Ĭ�ϲ����������Զ��ػ�,Ӧ�ò���������²���
		if((nRet = NDK_SysSetShutdownDelay(0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
#endif		
		if((nRet=NDK_SysGoSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		sleep(1);
#if !(defined ME15CHX||defined ME15B||defined ME66)
		cls_show_msg1(2,"���ѳɹ�,2���������˸(�޵��豸����Ե�)");
		NDK_LedStatus(LED_RFID_BLUE_FLICK);
#else
		cls_show_msg1(2,"���ѳɹ�,2�����������(�޵��豸����Ե�)");
		nRet = NDK_LedOpen("blue");
		NDK_LedOn(nRet);
#endif
	}
#if CPU5810X_ENABLE||defined ME50NX||defined ME68 //5810ƽ̨�ϣ�NDK_SysSetShutdownDelay��Ҫ128S��������,zhengry add
	if(cls_show_msg("�Ƿ����:����%d�������ģʽ�������ԣ��ǰ�ȷ�ϼ�������������",CONFIG_RTC_AUTO_SHUTDOWN)==ENTER)
	{
		//case3:���Բ���USB�� ����ʱ����ܷ��Զ��ػ�
		NDK_LedStatus(LED_RFID_BLUE_ON);//ͨ��ָʾ���ж��Ƿ�ػ�
		cls_show_msg("���Ƴ���(�޵��豸����Ե�),ȷ����USB��,�������%d�����Ƴ�������˸������״̬",CONFIG_RTC_AUTO_SHUTDOWN);	//������״̬������ε������豸�͹ػ���
		if((nRet = NDK_SysSetShutdownDelay(CONFIG_RTC_AUTO_SHUTDOWN/*128*/))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		//NDK_LedStatus(LED_RFID_RED_OFF);//ͨ��ָʾ���ж��Ƿ�ػ�
		if((nRet=NDK_SysGoSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		sleep(1);
	}

	if(cls_show_msg("�Ƿ����:����%d���ػ��������ԣ��ǰ�ȷ�ϼ�������������",CONFIG_RTC_AUTO_SHUTDOWN)==ENTER)
	{
		//case4:�������ò���USB��
		NDK_LedStatus(LED_RFID_BLUE_ON);//ͨ��ָʾ���ж��Ƿ�ػ�
		cls_show_msg("���Ƴ���(�޵��豸����Ե�),�밴����������ϰε�USB��,%d���Ӻ����ƽ�����ػ�",CONFIG_RTC_AUTO_SHUTDOWN);
		if((nRet = NDK_SysSetShutdownDelay(CONFIG_RTC_AUTO_SHUTDOWN/*128*/))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		if((nRet=NDK_SysGoSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN; 
		}
		sleep(1);
	}
#else

#if !(defined ME15CHX||defined ME15B||defined ME66)//��������ȷ��ME15CHX��֧�ֽ�����ģʽ
	if(cls_show_msg("�Ƿ����:����10�������ģʽ�������ԣ��ǰ�ȷ�ϼ�������������")==ENTER)
	{
		//case3:���Բ���USB�� ����ʱ����ܷ��Զ��ػ�
		NDK_LedStatus(LED_RFID_BLUE_ON);//ͨ��ָʾ���ж��Ƿ�ػ�
#if defined ME65//�뿪��ȷ�ϣ�ME65���ߺ�LED�ƻ����          		20180725    wangk     modify
		cls_show_msg("��ʱ���Ƴ���(�޵��豸����Ե�),ȷ����USB��,�������10�����Ƴ���������״̬");	//������״̬������ε������豸�͹ػ�
#else
		cls_show_msg("���Ƴ���(�޵��豸����Ե�),ȷ����USB��,�������10�����Ƴ�������˸������״̬");	//������״̬������ε������豸�͹ػ���
#endif
		if((nRet = NDK_SysSetShutdownDelay(10))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		//NDK_LedStatus(LED_RFID_RED_OFF);//ͨ��ָʾ���ж��Ƿ�ػ�
		if((nRet=NDK_SysGoSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		sleep(1);
	}
#endif	
	if(cls_show_msg("�Ƿ����:����5���ػ��������ԣ��ǰ�ȷ�ϼ�������������")==ENTER)
	{
		//case4:�������ò���USB��		
#if defined ME15CHX||defined ME15B||defined ME66	//ME15CHX������Ҳ��ػ�,������߻ỽ������,����Ҫ�ٵ�һ������,�����Ż�ػ�
		nRet = NDK_LedOpen("blue");
		NDK_LedOn(nRet);
		cls_show_msg("���Ƴ���(�޵��豸����Ե�),�밴�������ȴ�5��,5���Ӻ����ƽ�����ػ�");
#else
		NDK_LedStatus(LED_RFID_BLUE_ON);//ͨ��ָʾ���ж��Ƿ�ػ�
#if defined ME65//�뿪��ȷ�ϣ�ME65���ߺ�LED�ƻ����          		20180725    wangk     modify
		cls_show_msg("��ʱ���Ƴ���(�޵��豸����Ե�),���������,�������,���ϰε�USB��,5���Ӻ�ػ�");
#else
		cls_show_msg("���Ƴ���(�޵��豸����Ե�),�밴����������ϰε�USB��,5���Ӻ��ƽ�����ػ�");
#endif
#endif
		if((nRet = NDK_SysSetShutdownDelay(5))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		if((nRet=NDK_SysGoSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		sleep(1);
#if defined ME15CHX||defined ME15B	||defined ME66		
		if((nRet=NDK_SysGoSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
#endif		
	}
#endif
}
#else
void sys24(void)
{
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
	int nRet = 0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

#if defined ME30||defined ME30MH||defined ME32||defined ME50||defined ME50N||defined SP610
	//case1:δ�������ߺ�ػ�ʱ��Ĭ����5����
	cls_show_msg("ȷ��δ��USB��,���������");
	if(cls_show_msg("�������迪���״ν�����������в���,[Enter]��,[����]��")==ENTER)
	{
		cls_printf("����5���Ӻ��Զ��ػ�(��������USB��)");
		if((nRet=NDK_SysGoSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		sleep(1);
	}
#endif

	//case2:����0����ػ�
	cls_printf("����������5���Ӻ󲻻��Զ��ػ�,����5���Ӻ󰴼�����");
	if((nRet = NDK_SysSetShutdownDelay(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	if((nRet=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	if(cls_show_msg("�ȴ�5���Ӻ�δ�Զ��ػ�,[Enter]��ȷ,[����]ʧ��")!=ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI);
		RETURN;
	}

	//case3:���Բ���USB�� ����ʱ����ܷ��Զ��ػ�
	cls_show_msg("ȷ����USB��,���������");
	if((nRet = NDK_SysSetShutdownDelay(10))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	cls_printf("��һ�β�����ȴ�10s����ʾ���ڳ�����,�ڶ��β�������10s�ڰ�������");//�����������������10��ʱ�䵽����������(������)��������10���ڻ��ѻص��������� 20140812 zengjj ˵���øù���
	if((nRet=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	
	//case4:�������ò���USB��
	cls_show_msg("ȷ��δ��USB��,���������");
	if((nRet = NDK_SysSetShutdownDelay(5))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	cls_printf("����5s���Զ��ػ�");
	if((nRet=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	NDK_SysDelay(5);
	//���Խ���
	//send_result("%s����ͨ��", TESTAPI);
	//return;
}
#endif

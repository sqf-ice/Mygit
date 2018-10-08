/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys15.c
* Author 			: linw
* version			: 
* DATE				: 20130109
* directory 		: 
* description		: ��������NDK_LedStatus�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if !(defined ME15CHX||defined ME15B||defined ME66)
#define TESTAPI	"NDK_LedStatus"	
#else
#define TESTAPI	"NDK_LedOpen,NDK_LedOn,NDK_LedOff,NDK_LedClose"	
#endif
#define	MAXWAITTIME 30
#define WUCHASEC  1 //���ֵ����

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	      linw          20130109	created
*****************************************************************/
#if !(defined ME15CHX||defined ME15B||defined ME66)
void sys15(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	unsigned int unLen=0;
	char szBuf[128]={0};
	float setledtime = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//����ǰ��
	NDK_SysGetPosInfo (SYS_HWINFO_GET_HARDWARE_INFO, &unLen, szBuf);
	if (szBuf[12] != 0x01)//szBuf[12]:�Ƿ�����ƵLED�� 0xFF :�� 0x01 :�� 
	{
		//���û��LED�ƻ�ͨ������״̬����ģ��LED��20171011 modify ���ݿ���˵��
#if STABAR_ENABLE		
		if(cls_show_msg("�Ƿ����ʹ��״̬��LCDģ��LED�Ʋ���,�ǰ�[Enter],��[����]")==ENTER)
		{
			if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_LED|STATUSBAR_POSITION_TOP))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				GOTOERR;
			}
			cls_show_msg("�������Ĳ�����鿴״̬����ģ����ʾ��LED���������ж�,���������");
		}
		else
		{
			send_result("%s,��ȷ���Ƿ����ʹ��״̬��LCD����ģ��LED����",TESTAPI);
			return;
		}
#else	
#if 0  //�з���SP610��led�Ʒ���ֵ��ͬ,��ֱ�Ӹĳ���LED�ƾ�ֱ����ʾ��LED���������ж� 20180320 modify
		lib_StartStopwatch();
		if(((ret=NDK_LedStatus(LED_RFID_RED_ON | LED_RFID_YELLOW_ON | LED_RFID_GREEN_ON | LED_RFID_BLUE_ON | LED_COM_ON | LED_ONL_ON)) != NDK_ERR_NO_DEVICES)||((setledtime=lib_ReadStopwatch())>WUCHASEC))
		{
			send_result("line %d:%s����ʧ��(%d,%f)", __LINE__, TESTAPI, ret, setledtime);
			GOTOERR;
		}
#endif
		send_result("%s����ͨ��(���豸��LED���豸)", TESTAPI);
		return;
#endif
	}
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	
	//case10:�쳣����,�������,���ΪLED_ONL_FLICK = 0xc00
#if !K21_ENABLE
	if((ret=NDK_LedStatus(0xf000)) != NDK_ERR_PARA)
#else
    if((ret=NDK_LedStatus(0xf0000)) != NDK_OK)//ö��ֵ���Ϊ16λ������ʱ���Ϸ�ֵΪ0xffff������0xffff�Ļ��������ضϳ�Ϊ��16λ����������᷵�سɹ�
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	if(cls_show_msg("���豸�Ƿ�֧��LED_X?֧��[ȷ��],����[����]") == ENTER)
	{
		//case1: led��ȫ��
		if((ret=NDK_LedStatus(LED_RED_X_ON | LED_YELLOW_X_ON | LED_GREEN_X_ON | LED_BLUE_X_ON )) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("LED_X��ȫ��,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//case2: led��ȫ��
		if((ret=NDK_LedStatus(LED_RED_X_FLICK | LED_YELLOW_X_FLICK | LED_GREEN_X_FLICK | LED_BLUE_X_FLICK )) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("LED_X��ȫ��,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		//case3: led��ȫ��
		if((ret=NDK_LedStatus(LED_RED_X_OFF | LED_YELLOW_X_OFF | LED_GREEN_X_OFF | LED_BLUE_X_OFF)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("LED_X��ȫ��,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

	if(cls_show_msg("���豸�Ƿ�֧��LED��Ƶ��?֧��[ȷ��],����[����]") == ENTER)
	{
		//case1: led��ȫ��
		if((ret=NDK_LedStatus(LED_RFID_RED_ON | LED_RFID_YELLOW_ON | LED_RFID_GREEN_ON | LED_RFID_BLUE_ON/* \
			| LED_COM_ON | LED_ONL_ON*/)) != NDK_OK)//�����в�ͨѶ�ƺ������� ����ɲ�ҪҲΪ����ӦME30MH
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("LED��ȫ��,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case2: led��ȫ��
		if((ret=NDK_LedStatus(LED_RFID_RED_FLICK | LED_RFID_YELLOW_FLICK | LED_RFID_GREEN_FLICK | LED_RFID_BLUE_FLICK /*| \
			LED_COM_FLICK | LED_ONL_FLICK*/)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("LED��ȫ��,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}

		//case3: led��ȫ��
		if((ret=NDK_LedStatus(LED_RFID_RED_OFF | LED_RFID_YELLOW_OFF | LED_RFID_GREEN_OFF | LED_RFID_BLUE_OFF/* \
			| LED_COM_OFF | LED_ONL_OFF*/)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("LED��ȫ��,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

	//case4: ���������
	cls_show_msg("���������,���������...");
	if((ret=NDK_LedStatus(LED_RFID_RED_ON)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysMsDelay(2000);
	if((ret=NDK_LedStatus(LED_RFID_RED_FLICK)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysMsDelay(4000);
	if((ret=NDK_LedStatus(LED_RFID_RED_OFF)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("���������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5: �Ƶ�������
	if(cls_show_msg("���豸�Ƿ�֧��LED�Ƶ�?֧��[ȷ��],����[����]") == ENTER)
	{
		cls_show_msg("�Ƶ�������,���������...");
		if((ret=NDK_LedStatus(LED_RFID_YELLOW_ON)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_SysMsDelay(2000);
		if((ret=NDK_LedStatus(LED_RFID_YELLOW_FLICK)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_SysMsDelay(4000);
		if((ret=NDK_LedStatus(LED_RFID_YELLOW_OFF)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("�Ƶ�������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

	//case6: �̵�������
	if(cls_show_msg("���豸�Ƿ�֧��LED�̵�?֧��[ȷ��],����[����]") == ENTER)
	{
		cls_show_msg("�̵�������,���������...");
		if((ret=NDK_LedStatus(LED_RFID_GREEN_ON)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_SysMsDelay(2000);
		if((ret=NDK_LedStatus(LED_RFID_GREEN_FLICK)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_SysMsDelay(4000);
		if((ret=NDK_LedStatus(LED_RFID_GREEN_OFF)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		if(cls_show_msg("�̵�������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	
	//case7: ����������
	if(cls_show_msg("���豸�Ƿ�֧��LED����?֧��[ȷ��],����[����]") == ENTER)
	{
		cls_show_msg("����������,���������...");
		if((ret=NDK_LedStatus(LED_RFID_BLUE_ON)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysMsDelay(2000);
		if((ret=NDK_LedStatus(LED_RFID_BLUE_FLICK)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysMsDelay(4000);
		if((ret=NDK_LedStatus(LED_RFID_BLUE_OFF)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if(cls_show_msg("����������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	
	//case8: ͨѶ��������
	if(cls_show_msg("���豸�Ƿ�֧��ͨѶ��?֧��[ȷ��],����[����]") == ENTER)
	{
		cls_show_msg("ͨѶ��������,���������...");
		if((ret=NDK_LedStatus(LED_COM_ON)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysMsDelay(2000);
		if((ret=NDK_LedStatus(LED_COM_FLICK)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysMsDelay(4000);
		if((ret=NDK_LedStatus(LED_COM_OFF)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if(cls_show_msg("ͨѶ��������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

	//case9: ������������
	if(cls_show_msg("���豸�Ƿ�֧��������?֧��[ȷ��],����[����]") == ENTER)
	{
		cls_show_msg("������������,���������...");
		if((ret=NDK_LedStatus(LED_ONL_ON)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysMsDelay(2000);
		if((ret=NDK_LedStatus(LED_ONL_FLICK)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysMsDelay(4000);
		if((ret=NDK_LedStatus(LED_ONL_OFF)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if(cls_show_msg("������������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}

	//case10:�鳮��������
	if(cls_show_msg("���豸�Ƿ�֧���鳮��?֧��[ȷ��],����[����]") == ENTER)
	{
		cls_show_msg("�鳮��������,���������...");
		if((ret=NDK_LedStatus(LED_DETECTOR_ON)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysMsDelay(2000);
		if((ret=NDK_LedStatus(LED_DETECTOR_FLICK)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysMsDelay(4000);
		if((ret=NDK_LedStatus(LED_DETECTOR_OFF)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if(cls_show_msg("�鳮��������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	
	//case11: �ſ���������
	if(cls_show_msg("���豸�Ƿ�֧�ִſ���?֧��[ȷ��],����[����]") == ENTER)
	{
		cls_show_msg("�ſ���������,���������...");
		if((ret=NDK_LedStatus(LED_MAG_ON)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysMsDelay(2000);
		if((ret=NDK_LedStatus(LED_MAG_FLICK)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		NDK_SysMsDelay(4000);
		if((ret=NDK_LedStatus(LED_MAG_OFF)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if(cls_show_msg("�ſ���������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
	}
	
	send_result("%s����ͨ��", TESTAPI);
	goto ERR;
ERR:
	NDK_LedStatus(LED_RFID_RED_OFF | LED_RFID_YELLOW_OFF | LED_RFID_GREEN_OFF | LED_RFID_BLUE_OFF \
		| LED_COM_OFF | LED_ONL_OFF);//��ȫ��
#if K21_ENABLE
	NDK_LedStatus(0x2000 | 0x8000);//��ȫ��
#endif
#if defined ME11||defined ME15||defined ME15C||defined ME20
	NDK_LedStatus(LED_RFID_BLUE_FLICK);//����LED�򿪲���,������Һ�����豸ȷ�ϳ������гɹ�
#endif
#if STABAR_ENABLE		
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
	return;
}
#else
void sys15(void)
{
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
	int handle = 0, handle2 = 0, ret = -1, i = 0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);
	//����ǰ��,�ȹر����еĵ�
	NDK_LedOff(0);
	NDK_LedClose(0);
	NDK_LedOff(1);
	NDK_LedClose(1);

	//case1: ���������
	cls_show_msg("���������,���������...");
	handle = NDK_LedOpen("red");
	if((ret=NDK_LedOn(handle)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysMsDelay(2000);
	for(i=0;i<2;i++)
	{
		if((ret=NDK_LedOff(handle)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_SysMsDelay(1000);
		if((ret=NDK_LedOn(handle)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_SysMsDelay(1000);
	}
	if((ret=NDK_LedOff(handle)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("���������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedClose(handle)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2: ����������
#if defined ME66
	cls_show_msg("�̵�������,���������...");
	handle2 = NDK_LedOpen("green");
#else
	cls_show_msg("����������,���������...");
	handle2 = NDK_LedOpen("blue");
#endif
	if((ret=NDK_LedOn(handle2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysMsDelay(2000);
	for(i=0;i<2;i++)
	{
		if((ret=NDK_LedOff(handle2)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_SysMsDelay(1000);
		if((ret=NDK_LedOn(handle2)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_SysMsDelay(1000);
	}
	if((ret=NDK_LedOff(handle2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if defined ME66
	if(cls_show_msg("�̵�������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
#else
	if(cls_show_msg("���������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedClose(handle2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if defined ME66
	//�����������
	cls_show_msg("�����������,���������...");
	handle = NDK_LedOpen("cam");
	if((ret=NDK_LedOn(handle)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysMsDelay(2000);
	for(i=0;i<2;i++)
	{
		if((ret=NDK_LedOff(handle)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_SysMsDelay(1000);
		if((ret=NDK_LedOn(handle)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;
		}
		NDK_SysMsDelay(1000);
	}
	if((ret=NDK_LedOff(handle)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("���������,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedClose(handle)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case4:�����,������,�����,������
#if defined ME66
	cls_show_msg("�����,�̵���,�����,�̵���,���������...");
	handle2 = NDK_LedOpen("green");
#else
	cls_show_msg("�����,������,�����,������,���������...");
	handle2 = NDK_LedOpen("blue");
#endif
	handle = NDK_LedOpen("red");
	if((ret=NDK_LedOn(handle)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysMsDelay(1000);
	if((ret=NDK_LedOn(handle2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysMsDelay(1000);
	if((ret=NDK_LedOff(handle)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysMsDelay(1000);
	if((ret=NDK_LedOff(handle2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysMsDelay(1000);
	if((ret=NDK_LedClose(handle)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedClose(handle2)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3:�쳣����,�򿪵ĵ�����Ϊred,blue�����ֵ���߿մ�����NULL
#define MAXHANDLE 5	
	if((ret=NDK_LedOpen(NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedOpen("")) != NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedOpen("white")) != NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#if defined ME66
	if((ret=NDK_LedOpen("blue")) != NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case4:�쳣����,���ܷɼ���
	if((ret=NDK_LedOn(-1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedOn(MAXHANDLE)) != NDK_OK)//0
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedOff(-1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedOff(MAXHANDLE)) != NDK_OK)//0
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedClose(-1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedClose(MAXHANDLE)) != NDK_OK)//0
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	send_result("%s����ͨ��", TESTAPI);
ERR:
	//���Ժ��ùرյƺ��ͷž��
	//�򿪺��
	NDK_LedOff(handle);
	NDK_LedClose(handle);
	NDK_LedOff(handle2);
	NDK_LedClose(handle2);
#if defined ME66
	handle = NDK_LedOpen("green");
	NDK_LedOn(handle);
	handle2 = NDK_LedOpen("cam");
	NDK_LedOn(handle2);
#else
	handle = NDK_LedOpen("blue");
	NDK_LedOn(handle);
#endif
	return;
}
#endif


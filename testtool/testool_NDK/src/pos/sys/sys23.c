/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys23.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ��������NDK_LedSetFlickParam �ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_LedSetFlickParam"	

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
*			  	  	    jiangym			20140422		created
*****************************************************************/
void sys23(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	unsigned int unLen=0;
	char szBuf[128]={0};
	ST_LED_FLICK flick_param;
	flick_param.unFlickOn = 5; //0.5s
	flick_param.unFlickOff = 5;//0.5s	
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//����ǰ��

	NDK_SysGetPosInfo (SYS_HWINFO_GET_HARDWARE_INFO, &unLen, szBuf);
	if (szBuf[12] != 0x01)//szBuf[12]:�Ƿ�����ƵLED�� 0xFF :�� 0x01 :�� 
	{
		//���û��LED�ƻ�ͨ������״̬����ģ��LED��20180409 modify ���ݿ���˵��
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
		send_result("line %d:%s���Ըû���û��LED��", __LINE__,TESTAPI);
		return;
#endif
		}	
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}

	//case1:�쳣����,�������,���ΪLED_ONL_FLICK = 0xc00
	if((ret=NDK_LedSetFlickParam(0xf000,flick_param)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//����Ϊ0,����ok
	if((ret=NDK_LedSetFlickParam(0x00,flick_param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2: ����flick�Ĳ�����Ч��������,����ok,���õ�ʱ����5sӦ����Ч,��ư�Ĭ�ϵ�100ms������˸
	cls_show_msg("�����˸,��100ms��100ms,���������...");
	flick_param.unFlickOn = 50; //5s
	flick_param.unFlickOff = 50;//5s	
	if((ret=NDK_LedSetFlickParam(LED_RFID_RED_ON,flick_param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_RED_FLICK)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("�����˸,������ԼΪ100ms,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_RED_OFF)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3: �����˸,��0.5s��0.5s
	cls_show_msg("�����˸,��0.5s��0.5s,���������...");
	flick_param.unFlickOn = 5; //0.5s
	flick_param.unFlickOff = 5;//0.5s	
	if((ret=NDK_LedSetFlickParam(LED_RFID_RED_FLICK,flick_param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_RED_FLICK)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("�����˸,��0.5s��0.5s,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_RED_OFF)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case4: �Ƶ���˸,��1s��1s
	cls_show_msg("�Ƶ���˸,��1s��1s,���������...");
	flick_param.unFlickOn = 10; //1s
	flick_param.unFlickOff = 10;//1s
	if((ret=NDK_LedSetFlickParam(LED_RFID_YELLOW_FLICK,flick_param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_YELLOW_FLICK)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("�Ƶ���˸,��1s��1s,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_YELLOW_OFF)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5: �̵���˸,��0.5s��0.5s
	cls_show_msg("�̵���˸,��0.5s��0.5s,���������...");
	flick_param.unFlickOn = 5; //0.5s
	flick_param.unFlickOff = 5;//0.5s
	if((ret=NDK_LedSetFlickParam(LED_RFID_GREEN_FLICK,flick_param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_GREEN_FLICK)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("�̵���˸,��0.5s��0.5s,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_GREEN_OFF)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6: ������˸,��1s��1s
	cls_show_msg("������˸,��1s��1s,���������...");
	flick_param.unFlickOn = 10; //1s
	flick_param.unFlickOff = 10;//1s
	if((ret=NDK_LedSetFlickParam(LED_RFID_BLUE_FLICK,flick_param)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_BLUE_FLICK)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("������˸,��1s��1s,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_BLUE_OFF)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case7: ͨѶ����˸,��1s��1s
    if(cls_show_msg("���豸�Ƿ�֧��ͨѶ��?֧��[ȷ��],����[����]") == ENTER)
	{
	    cls_show_msg("ͨѶ����˸,��1s��1s,���������...");
	    flick_param.unFlickOn = 10; //1s
	    flick_param.unFlickOff = 10;//1s
	    if((ret=NDK_LedSetFlickParam(LED_COM_FLICK,flick_param)) != NDK_OK)
	    {
		    send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		    GOTOERR;
	    }
	    if((ret=NDK_LedStatus(LED_COM_FLICK)) != NDK_OK)
	    {
		    send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		    GOTOERR;
	     }
	    if(cls_show_msg("ͨѶ����˸,��1s��1s,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	    {
		    send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		    GOTOERR;
	    }
	    if((ret=NDK_LedStatus(LED_COM_OFF)) != NDK_OK)
	    {
		    send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		    GOTOERR;
	    }
	}
	//case8: ��������˸,��0.5s��0.5s
    if(cls_show_msg("���豸�Ƿ�֧��������?֧��[ȷ��],����[����]") == ENTER)
	{
	    cls_show_msg("��������˸,��0.5s��0.5s,���������...");
	    flick_param.unFlickOn = 5; //0.5s
	    flick_param.unFlickOff = 5;//0.5s
	    if((ret=NDK_LedSetFlickParam(LED_ONL_FLICK,flick_param)) != NDK_OK)
	    {
		    send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		    GOTOERR;
	    }
	    if((ret=NDK_LedStatus(LED_ONL_FLICK)) != NDK_OK)
	    {
		    send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		    GOTOERR;
	    }
	    if(cls_show_msg("��������˸,��0.5s��0.5s,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	    {
		    send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		    GOTOERR;
	    }
	    if((ret=NDK_LedStatus(LED_ONL_OFF)) != NDK_OK)
	    {
		    send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		    GOTOERR;
	    }
    }
	
	//case9:��ȫ��˸,��0.5s��0.5s,����ͬʱ���ö���ȵ���˸ʱ����
	cls_show_msg("LED��ȫ��˸,��0.5s��0.5s,���������...");
	flick_param.unFlickOn = 5; //0.5s
	flick_param.unFlickOff = 5;//0.5s
	if((ret=NDK_LedSetFlickParam(LED_RFID_RED_FLICK | LED_RFID_YELLOW_FLICK | LED_RFID_GREEN_FLICK | LED_RFID_BLUE_FLICK/* | \
		LED_COM_FLICK | LED_ONL_FLICK*/,flick_param)) != NDK_OK)//���ڶ����Ĳ�Ʒ���߱�ͨѶ�ƺ������� ������20180412
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_RED_FLICK | LED_RFID_YELLOW_FLICK | LED_RFID_GREEN_FLICK | LED_RFID_BLUE_FLICK /* | \
		LED_COM_FLICK | LED_ONL_FLICK*/)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("LED��ȫ��˸,��0.5s��0.5s,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_LedStatus(LED_RFID_RED_OFF | LED_RFID_YELLOW_OFF | LED_RFID_GREEN_OFF | LED_RFID_BLUE_OFF /* | \
		LED_COM_FLICK | LED_ONL_FLICK*/)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case10:�������õ���Ч���Ƕ�εģ��صƺ��ٴ���Ȼ��֮ǰ���õ�ʱ�������˸
	cls_show_msg("���Բ������õ���Ч��,���������...");
	if((ret=NDK_LedStatus(LED_RFID_RED_FLICK | LED_RFID_YELLOW_FLICK | LED_RFID_GREEN_FLICK | LED_RFID_BLUE_FLICK/* | \
		LED_COM_FLICK | LED_ONL_FLICK*/)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(cls_show_msg("LED��ȫ��˸,���ϴ�һ����0.5s��0.5s,[ȷ��]ͨ��,[����]ʧ��") != ENTER)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_LedStatus(LED_RFID_RED_OFF | LED_RFID_YELLOW_OFF | LED_RFID_GREEN_OFF | LED_RFID_BLUE_OFF/* | \
		LED_COM_FLICK | LED_ONL_FLICK*/);//��ȫ��
	flick_param.unFlickOn = 1; //100ms
	flick_param.unFlickOff = 1;//100ms  �ָ�ԭʼ����
	NDK_LedSetFlickParam(LED_RFID_RED_FLICK | LED_RFID_YELLOW_FLICK | LED_RFID_GREEN_FLICK | LED_RFID_BLUE_FLICK /* | \
		LED_COM_FLICK | LED_ONL_FLICK*/,flick_param);
#if STABAR_ENABLE		
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
	return;
}


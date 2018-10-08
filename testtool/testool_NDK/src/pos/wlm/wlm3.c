/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ����ģ��
* file name		: wlm3.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_WlSendATCmd�ӿڹ���
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
#define TESTAPI	"NDK_WlSendATCmd"	
#define	MAXWAITTIME (40)

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
 
/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void wlm6(void)
* functional description 	: ������ģ�鷢�ͺͽ���ATָ���pATCmdPackΪ��NULLʱ����ֱ�ӵȴ�����,������ATָ��;���pcOutputΪ��NULL�����������ATָ�� 
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void wlm3(void)
{
	/*private & local definition*/
	char szOutbuf[1024] = {0};//������������������������Ӱ�죬Ӧ���Ǹ�BUG
	ST_ATCMD_PACK STatcmdpack = {0};
	char *pos = NULL;
	//time_t diff = 0, oldtime = 0;
	LinkType type = getlinktype();
	EM_WLM_STATUS emStatus = 0;
	int nRet = 0;
	
	/*process body*/
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI);
	
	memset(&STatcmdpack,0,sizeof(ST_ATCMD_PACK));
	//case1:�쳣����
	memset(szOutbuf, 0, sizeof(szOutbuf));
	if((nRet = NDK_WlSendATCmd(NULL, szOutbuf,sizeof(szOutbuf), 30000,&emStatus)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}	

	STatcmdpack.AtCmdNo = WLM_CMD_E0;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, NULL,0, 30000,&emStatus)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	
	memset(szOutbuf, 0, sizeof(szOutbuf));
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf,0, 30000,NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	
	//�����������в���
	//case2:����WLM_CMD_E0
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_E0;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf,sizeof(szOutbuf), 30000,&emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//NDK_OK,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{
		if(strstr(szOutbuf, "OK"))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}		
	}

	//case3:����WLM_CMD_CSQ
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CSQ;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{		
		if(strstr(szOutbuf, "OK"))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}	
	}	

	//case4:����WLM_CMD_CPIN
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CPIN;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{
		if(strstr(szOutbuf, "READY"))//���ԵĿ�Ĭ����û����PIN��,����READY
				;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}		
	}	

#if 0 //��Ҫ����PIN,��������PIN������
	//����WLM_CMD_CPIN0
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CPIN0;
	STatcmdpack.pcAddParam = "1234";
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{
		if(strstr(szOutbuf, "OK"))
				;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}		
	}
#endif

	//case5:����WLM_CMD_CREG(��ָGSM�����ע��״̬,����GPRS,����һ����˵,ģ��һ�ϵ���Զ�ע����)
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CREG;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{		
		//�ϵ�ʱ�Զ�ע��,���Բ�ѯ���ӦΪ:һ����ע��1(ע�᱾��)��5(����)
		if(strchr(szOutbuf, '1')||strchr(szOutbuf, '5'))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}	
	}

	//case6:����WLM_CMD_CGATT1
	//����̬
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CGATT1;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{	
		if(strstr(szOutbuf, "OK"))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}	
	}

	//case7:����WLM_CMD_COPSȡ����ע����Ϣ(CDMA��������(��֧��),Ŀǰ����GPRS���Կ�Ϊ׼Ԥ����)
	if(type==GPRS)
	{
		memset(szOutbuf, 0, sizeof(szOutbuf));
		STatcmdpack.AtCmdNo = WLM_CMD_COPS;
		STatcmdpack.pcAddParam = NULL;
		STatcmdpack.pcAtCmd = NULL;
		if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
		{			
			if(strstr(szOutbuf, "CHINA MOBILE")||strstr(szOutbuf, "CHN-UNICOM")||strstr(szOutbuf, "CHINA  MOBILE")||strstr(szOutbuf, "China Mobile")||strstr(szOutbuf, "china mobile")||strstr(szOutbuf, "CHINA UNICOM")||strstr(szOutbuf, "China Unicom")||strstr(szOutbuf, "china unicom")||strstr(szOutbuf, "46000")||strstr(szOutbuf, "46001"))
				;
			else
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				RETURN;
			}	
		}
	}

	//case8:����WLM_CMD_CGATT0
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CGATT0;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{		
		if(strstr(szOutbuf, "OK"))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}	
	}
#if 0//20131107,��������Ҫ��رղ���ͨ��AT������в��ŵ�����
	//case9:����WLM_CMD_D2ͨ���κ�������̬
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_D2;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{		
		if(strstr(szOutbuf, "OK"))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}	
	}	
	
	sleep(5);//TDģʽ��WM_D2���ܻ�Ӱ�쵽���������,��GPRS�����ڴ�����.Ŀǰ��ʱ��ʱ�����
	
	//case10:����WLM_CMD_DIAL����,��ͨ״̬�²���Ӧ����
	//�κ�ǰ,����ע������(�ɲ����ֹ�ע��)
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_DIAL;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{
		if(strstr(szOutbuf, "CONNECT"))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}
		
#if 0	//zhangnw20130418:��Ϊ���õķ��ز�ȷ��,���ٲ���.
		//һ���,ģ������̬ͨ��,������ӦAT����
		memset(szOutbuf, 0, sizeof(szOutbuf));
		STatcmdpack.AtCmdNo = WLM_CMD_E0;
		STatcmdpack.pcAddParam = NULL;
		STatcmdpack.pcAtCmd = NULL;
		if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		if(emStatus!=WLM_STATUS_NO_CARRIER/*WLM_STATUS_ERROR*/)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,emStatus);
			RETURN;
		}
#endif
	}
	
	//�Ͽ�PPP,���������в�������,�����ݵ�,��resetwirelessmodem
	if(type==GPRS)
	{
		oldtime = time(NULL);
		while((diff=time(NULL)-oldtime)<2*MAXWAITTIME)
			cls_printf("���ڶϿ���·,��ȴ�%d��...", 2*MAXWAITTIME-diff);//����40��,���ƶ���̨�Զ��ͷ���·//�����Դ�,����//�����ϸ�����������40��ʱ�䣬�˴��Ŵ�ȴ�ʱ��
	}
	else if(type==CDMA)
	{
		NDK_PppHangup(0);
	}

	//��ʱӦ�ص�����̬,��AT����Ӧ��Ӧ
	//֧��WLM_CMD_CGMR,������;�������WLM_CMD_E0
#if 1
	//case11:����WLM_CMD_CGMR
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CGMR;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{
		if(strstr(szOutbuf, "OK"))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}	
	}
#else
	//WLM_CMD_E0
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_E0;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf,sizeof(szOutbuf), 30000,&emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//NDK_OK,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{
		if(strstr(szOutbuf, "OK"))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}		
	}
#endif
#endif
	//case12:����WLM_CMD_CGSN
	// 1.�������GPRSģ����˵����ȡIMEI.
	//���ֻ������� *#06# ������5 ���������ֻ���Ļ�ϻ����һ�а���������(15��)����Ϊ���ֻ��Ĵ��š�
	//����˵��ʹ�÷���ΪG���ֻ���C���ֻ��������취���޷��鿴��
	// 2.�������CDMAģ����˵����ȡESN,32bits����.
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CGSN;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{
		if(type==GPRS)//�ٽ�һ������
		{
			if((pos=strstr(szOutbuf, "\x0D\x0A"))==NULL) //û��0D0A  ��������Ϊ:862996013998543....
			{
				*(szOutbuf+15) = '\0';//15λ����
				if(is_digit_str(szOutbuf))
					;
				else
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					RETURN;
				}
			}
			else if((pos=strstr(szOutbuf, "\""))!=NULL)//�����ţ��򷵻���������Ϊ(0d)(0a)+GSN:(20)"862996013998543"(0d)(0a)(0d)(0a)OK(0d)(0a)
			{
				pos++;//ָ���Ƶ����ź�һ��λ��  //linwl20130515������G610ģ�鷵����Ϣ����ȡ
			}
			else if((pos=strstr(szOutbuf, "\x0D\x0A"))!=NULL)//û��" �������û�� �򷵻���������Ϊ 
			{
				if(pos==szOutbuf)//�ջ�������0d0a��ʼ��û�����ŵĻ�,���˵���ʼ��0d0a  (0d)(0a)862996013998543....
					pos+=strlen("\x0D\x0A");
				else
					pos = szOutbuf;//0D0A���ַ������棬��862996013998543(0d)(0a)
			}
			
			*(pos+15) = '\0';//15λ����
			if(is_digit_str(pos))
				;
			else
			{
				send_result("line %d:%s����ʧ��%s", __LINE__, TESTAPI, pos);
				RETURN;
			}
		}
	}	
	
#if 0
	//case:����WLM_CMD_CCID
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_CCID;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = NULL;
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{
		if(strstr(szOutbuf, "OK"))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}	
	}	
#endif

	//case13:"�Ƿ�"����WLM_CMD_END(��ʵ�Ͽ��Ÿ�Ӧ�õ�h�ļ��в�Ӧ���������!),Ӧʧ��(���ǰ�UNDEFINE����)
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_END;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = "E1";
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}

	//case14:����WLM_CMD_UNDEFINE
	//subcase1:�ĵ�˵" �����δ����ָ����ж�pszAtCmd�����Ƿ�����������Ӧ��AtCmdNo�ţ� �������תΪ��Ӧ��atcmdno���д���"
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_UNDEFINE;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = "+CSQ";
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{		
		if(strstr(szOutbuf, "OK"))
			;
		else
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}	
	}		
	
	//subcase2:ȷʵ��δ���������,Ӧִ��pszAtCmd
	if(type==GPRS)//CDMA�޴�����
	{
		memset(szOutbuf, 0, sizeof(szOutbuf));
		STatcmdpack.AtCmdNo = WLM_CMD_UNDEFINE;
		STatcmdpack.pcAddParam = NULL;
		STatcmdpack.pcAtCmd = "+CIMI";
		if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
			RETURN;
		}
		else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
		{
			//46001�������й���ͨ,46000�����й��ƶ�
			//460��Ӧ��86��00��Ӧ��135---139�Ŷ�...
			if((pos=strstr(szOutbuf, "460"))==NULL)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				RETURN;
			}
			*(pos+15) = '\0';//15λ����
			if(is_digit_str(pos))
				;
			else
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				RETURN;
			}
		}	
	}

	//case15:��Ч����"welcome"
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_UNDEFINE;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = "welcome";
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}

	//case16:��Ч���� "+CPIN="
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_UNDEFINE;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = "+CPIN=";
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	
	//case17:�����Ѷ��������,������AtCmdNo
	memset(szOutbuf, 0, sizeof(szOutbuf));
	STatcmdpack.AtCmdNo = WLM_CMD_E0;
	STatcmdpack.pcAddParam = NULL;
	STatcmdpack.pcAtCmd = "+CPIN?";
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	else//SUCC,����Ҫ��һ����һ�½�����ȷ��send����ȷ��
	{	
		if(strstr(szOutbuf, "READY"))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			RETURN;
		}
		else//eg. "OK"
			;	
	}
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
	time_t oldtime = 0, diff = 0;
	//case18:Ӳ����λ�����ϵ��û᷵��NDK_ERR_GPRS_UNINIT
	if((nRet=NDK_WlModemReset())!=NDK_OK)//Ӳ����λ
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	if ((nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 30000, &emStatus)) != NDK_ERR_GPRS_UNINIT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
		RETURN;
	}
	
	//case19:Ӳ����λδ��ɵ��û᷵��NDK_ERR_GPRS_BUSY
	oldtime = time(NULL);
	while(1)
	{
		NDK_SysMsDelay(50);
		nRet = NDK_WlSendATCmd(&STatcmdpack, szOutbuf, sizeof(szOutbuf), 1000, &emStatus);
		if(nRet==NDK_ERR_GPRS_BUSY)
			break;
		if(nRet==NDK_ERR_GPRS_UNINIT)
			;
		if(nRet==NDK_OK)
			break;
		if((diff=time(NULL)-oldtime)>20)
		{
			send_result("line %d:��ʱ��δ����NDK_ERR_GPRS_BUSY(%d)", __LINE__, nRet);
			return;
		}
	}
	sleep(3);//�����㹻����ʱȷ����λ���
#endif
	
	send_result("%s����ͨ��", TESTAPI);
	return;
}


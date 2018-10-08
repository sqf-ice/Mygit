/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys10.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysReadyToSuspend,NDK_SysGoSuspend_Extern�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"
#include <linux/delay.h>

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysReadyToSuspend,NDK_SysGoSuspend_Extern"

#define	MAXWAITTIME 30

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
*						linwl		 20150713		modify
*****************************************************************/
//ѡ�����߻��ѵķ�ʽ
//IC ��SAM�ϵ�����в��ܽ�����,�Լ�����Ƕ��
static int ic_sam_suspend(int flag_wakeup)
{
	int ret = - 1, buflen = 0, buflen1 = 0;
	int nKeyin;
	uchar buf[256]={0}, buf1[256]={0};
	
	//case5:IC/SAM������������(�ϵ��)GotoSleep��Ч
	//IC������
	cls_show_msg("�����IC1��,�������������");
	memset(buf,0,sizeof(buf));
	if((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&buflen))!=NDK_OK)
	{
		send_result("line %d: IC1�ϵ�ʧ��(%d)", __LINE__, ret);
		return FAIL;  
	}
	if((ret = NDK_SysReadyToSuspend())!=NDK_ERR_ICCARD_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_IccPowerDown(ICTYPE_IC);
		return FAIL;
	}
	if((ret = NDK_SysGoSuspend_Extern())!=NDK_ERR_ICCARD_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_IccPowerDown(ICTYPE_IC);
		return FAIL;
	}
	if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{
		send_result("line %d: IC1�µ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}	
	if(cls_show_msg("����IC1���󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	//�µ���Ҳ忨������ܹ���������
	if(flag_wakeup==0)
	{
		cls_show_msg("���������������������,����������,�����������");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
		if (nKeyin!=ENTER && nKeyin!=0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	else
	{
		cls_show_msg1(3, "3s���������,��ȴ�5s�󣬽��Զ�����");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		if((ret=NDK_SysWakeUp()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if(cls_show_msg("�ѻ���,û���κΰ���Ҳ�����߻���,��ȷ[Enter],����[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	//���Ѻ�����������
	memset(buf,0,sizeof(buf));
	if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&buflen))!=NDK_OK)
	{
		send_result("line %d: IC1�ϵ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}
	if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{
		send_result("line %d: IC1�µ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}	

	//sam������
	cls_show_msg("�����SAM1��,�������������(�����Զ�����ǰ)");
	memset(buf,0,sizeof(buf));
	if ((ret=NDK_IccPowerUp(ICTYPE_SAM1,buf,&buflen))!=NDK_OK)
	{
		send_result("line %d: SAM1�ϵ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_ICCARD_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_IccPowerDown(ICTYPE_SAM1);
		return FAIL;
	} 
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_ICCARD_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_IccPowerDown(ICTYPE_SAM1);
		return FAIL;
	} 
	if((ret=NDK_IccPowerDown(ICTYPE_SAM1))!=NDK_OK)
	{
		send_result("line %d: SAM1�µ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}	
	if(cls_show_msg("����SAM1���󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	//�µ���ܹ���������
	if(flag_wakeup==0)
	{
		cls_show_msg("���������������������,����������,�����������");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
		if (nKeyin!=ENTER && nKeyin!=0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	else
	{
		cls_show_msg1(3, "3s���������,��ȴ�5s�󣬽��Զ�����");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		if((ret=NDK_SysWakeUp()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if(cls_show_msg("�ѻ���,û���κΰ���Ҳ�����߻���,��ȷ[Enter],����[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	//���Ѻ�����������
	memset(buf,0,sizeof(buf));
	if ((ret=NDK_IccPowerUp(ICTYPE_SAM1,buf,&buflen))!=NDK_OK)
	{
		send_result("line %d: SAM1�ϵ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}
	if((ret=NDK_IccPowerDown(ICTYPE_SAM1))!=NDK_OK)
	{
		send_result("line %d: SAM1�µ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}

	//case9:�������豸IC/SAM,�ر�һ���豸�� ����һ���豸��Ȼ��æ�����ǲ�Ӧ�ý������߶��رպ���ܽ�������
	cls_show_msg("��ȷ���Ѳ���IC1��SAM1��,�������������");
	memset(buf,0,sizeof(buf));
	if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&buflen))!=NDK_OK)
	{
		send_result("line %d: IC1�ϵ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}
	memset(buf1,0,sizeof(buf1));
	if ((ret=NDK_IccPowerUp(ICTYPE_SAM1,buf1,&buflen1))!=NDK_OK)
	{
		send_result("line %d: SAM1�ϵ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_ICCARD_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_IccPowerDown(ICTYPE_IC);
		NDK_IccPowerDown(ICTYPE_SAM1);
		return FAIL;
	} 
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_ICCARD_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_IccPowerDown(ICTYPE_IC);
		NDK_IccPowerDown(ICTYPE_SAM1);
		return FAIL;
	} 
	if((ret=NDK_IccPowerDown(ICTYPE_SAM1))!=NDK_OK)
	{
		send_result("line %d: SAM1�µ�ʧ��(%d)", __LINE__, ret);
		NDK_IccPowerDown(ICTYPE_IC);
		return FAIL;
	}	
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_ICCARD_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_IccPowerDown(ICTYPE_IC);
		return FAIL;
	} 
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_ICCARD_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_IccPowerDown(ICTYPE_IC);
		return FAIL;
	} 
	if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{
		send_result("line %d: IC1�µ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}	
	if(cls_show_msg("���뿨�󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	if(flag_wakeup==0)
	{
		cls_show_msg("���������������������,����������,�����������");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
		if (nKeyin!=ENTER && nKeyin!=0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	else
	{
		cls_show_msg1(3, "3s���������,��ȴ�5s�󣬽��Զ�����");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		if((ret=NDK_SysWakeUp()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if(cls_show_msg("�ѻ���,û���κΰ���Ҳ�����߻���,��ȷ[Enter],����[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	return SUCC;
}

//��ӡ��ſ���Ƕ��,�ڴſ�������´�ӡ�в��ܽ�����
static int prn_mag_suspend(int flag_wakeup)
{
	int ret = -1, nBusy = FALSE, nErrorCode = 0;
	int nKeyin;
	EM_PRN_STATUS emStatus;
	time_t diff = 0, oldtime = 0;
	char pszTk1[MAXTRACKLEN] = {0}, pszTk2[MAXTRACKLEN] = {0}, pszTk3[MAXTRACKLEN] = {0};
	
	//�ȴ򿪴ſ�����Ƕ�ײ���
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	//case4:���������ܷ��ϴ�ӡ
	cls_show_msg("�밲װ�ô�ӡֽ,���������(�����Զ�����ǰ)");
	if (NDK_OK!=NDK_PrnInit(1))
	{
		send_result("line %d: ��ӡ����ʼ��ʧ��", __LINE__);
		goto ERR;
	}
	
	while(1)
	{
		if((ret=NDK_PrnGetStatus(&emStatus))!=NDK_OK )
		{
			send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,ret);
			goto ERR;
		}
		if(emStatus != PRN_STATUS_BUSY)
			show_stopwatch(DISABLE, 0);
		
		switch(emStatus)
		{
		case PRN_STATUS_BUSY:
			if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_PRN_BUSY)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				goto ERR;
			} 
			if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_PRN_BUSY)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				goto ERR;
			} 
			if(nBusy==FALSE)
			{
				oldtime = time(NULL);//�ӵ�һ���յ�æ״̬��ʼ��ʱ
				nBusy = TRUE;
			}
			if((diff=time(NULL)-oldtime)>MAXWAITTIME)
			{
				send_result("line %d:��ӡ������æ״̬ʱ�����", __LINE__);
				goto ERR;
			}
			show_stopwatch(ENABLE, diff);
			if((ret=NDK_PrnStr("����֮��>"))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				goto ERR;
			}		
			NDK_PrnStr("\n---��ӡ���---\n");	
			NDK_PrnStart();
			if(cls_show_msg("��ӡ�����Ƿ���������\n[ENTER]��  [����]��")==ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto ERR;
			}
			goto END;
			show_stopwatch(ENABLE, diff);
			continue;
			break;

		case PRN_STATUS_OK:
			nBusy=FALSE;
			NDK_PrnSetFont(PRN_HZ_FONT_24x24,PRN_ZM_FONT_8x16);
			if((ret=NDK_PrnStr("����֮ǰ>\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20"))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
				goto ERR;
			}		
			//NDK_PrnStart();
			
			break;
		case PRN_STATUS_NOPAPER:
			cls_printf("��ӡ��ȱֽ...װֽ��,���������,ESC����");
			lib_kbflush(); //��KB BUF
			while ((nKeyin=lib_kbhit())==0)
			{
				NDK_SysBeep();
				//����װֽ
				NDK_PrnFeedByPixel(10);
				sleep(2);
			}
			if (nKeyin==ESC)
			{
				send_result("line %d:��ӡ��ȱֽ,�û�����", __LINE__);
				goto ERR;
			}
			NDK_PrnStart();
			cls_printf("����%s...", TESTAPI);
			break;
		case PRN_STATUS_OVERHEAT:
			send_result("line %d: ��ӡ������", __LINE__);
			goto ERR;
			break;
		case PRN_STATUS_VOLERR:
			send_result("line %d: ��ӡ����Դ����", __LINE__);
			goto ERR;
			break;
		default:
			//δ֪״̬����Ϊ��
			send_result("line %d: UNKNOWN STATE(tpstatus=%d)", __LINE__, emStatus);
			goto ERR;
			break;
		}	
	}
END:	
	if(flag_wakeup==0)
	{
		cls_show_msg("���������������������,����������,�����������");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
		if (nKeyin!=ENTER && nKeyin!=0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}
	}
	else
	{
		cls_show_msg1(3, "3s���������,��ȴ�5s�󣬽��Զ�����");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		if((ret=NDK_SysWakeUp()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		if(cls_show_msg("�ѻ���,û���κΰ���Ҳ�����߻���,��ȷ[Enter],����[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			goto ERR;
		}
	}
	NDK_MagReset();
	cls_show_msg("��ˢ��,�����������(�����Զ�����ǰ)");
	memset(pszTk1,0,sizeof(pszTk1));
	memset(pszTk2,0,sizeof(pszTk2));
	memset(pszTk3,0,sizeof(pszTk3));
	if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &nErrorCode))!=NDK_OK)	
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	//��ʾ�ŵ���Ϣ
	if((ret=check_stripe_result_tk1(pszTk1))!=0x00)
	{
		send_result("line %d:����ʧ��(0x%x)", __LINE__, ret);
		goto ERR;
	}
	if((ret=check_stripe_result_tk2(pszTk2))!=0x00)
	{
		send_result("line %d:����ʧ��(0x%x)", __LINE__, ret);
		goto ERR;
	}
	if((ret=check_stripe_result_tk3(pszTk3))!=0x00)
	{
		send_result("line %d:����ʧ��(0x%x)", __LINE__, ret);
		goto ERR;
	}
	if(flag_wakeup==0)
	{
		cls_show_msg("���������������������,����������,�����������");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
		if (nKeyin!=ENTER && nKeyin!=0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	else
	{
		cls_show_msg1(3, "3s���������,��ȴ�5s�󣬽��Զ�����");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		if((ret=NDK_SysWakeUp()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if(cls_show_msg("�ѻ���,û���κΰ���Ҳ�����߻���,��ȷ[Enter],����[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	NDK_MagClose();	
	return SUCC;
ERR:
	NDK_MagClose();
	return FAIL;
}

//��Ƶ��usb��U���豸������²��ܽ�����,�Լ���Ƶ��USB��Ƕ��
static int rfid_tc_suspend(int flag_wakeup)
{
	int ret = -1,flag = 0, pnDatalen = 0;
	int nKeyin, i = 0;
	time_t diff = 0, oldtime = 0;
	uchar ucPicctype = 0, psPicctype = 0x00, psDatabuf[256] = {0}, UID[LEN_UID]={0},psSakbuf[10]={0};

	//case6:������Ƶ��(A.B.M)����������(Ѱ���ɹ���,��д)GotoSleep��Ч,
	cls_show_msg("��֤��Ӧ���������κο�,���������");
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:Ӧ�ü�⵽�޿�(%d)", __LINE__, ret);
		return FAIL;
	}	
	if((ret=NDK_RfidPiccDetect(&ucPicctype))!=NDK_ERR_RFID_NOCARD)
	{
		send_result("line %d:Ѱ��Ӧ��ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}	
	if(flag_wakeup==0)
	{
		cls_show_msg("���������������������,����������,�����������");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
		if (nKeyin!=ENTER && nKeyin!=0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	else
	{
		cls_show_msg1(3, "3s���������,��ȴ�5s�󣬽��Զ�����");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		if((ret=NDK_SysWakeUp()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if(cls_show_msg("�ѻ���,û���κΰ���Ҳ�����߻���,��ȷ[Enter],����[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	for(i=0;i<2;i++)//����B ���Ĳ���Ѱ�����Ͳ���������� NDK5.0
	{
		cls_show_msg("���ڸ�Ӧ������1�ű�׼%s��,���������(�����Զ�����ǰ)",i==0?"A":"B");
		if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}	
		oldtime = time(NULL);
		while((diff = time(NULL)-oldtime)<10)  //����10��Ѱ��ʱ��
		{
			if((ret=NDK_RfidPiccDetect(&ucPicctype))==NDK_OK)
			{
				flag=1;
				break;
			}	
		}
		if(flag != 1)
		{
			send_result("line %d:Ѱ��ʧ��", __LINE__);
			NDK_RfidPiccDeactivate(0);//�رճ�ǿ
			return FAIL;
		}
		if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_RFID_BUSY)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			NDK_RfidPiccDeactivate(0);
			return FAIL;
		} 
		if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_RFID_BUSY)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			NDK_RfidPiccDeactivate(0);
			return FAIL;
		} 
		if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=(i==0?RFCARD_TYPE_A:RFCARD_TYPE_B))
		{
			send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
			NDK_RfidPiccDeactivate(0);
			return FAIL;
		}
		if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_RFID_BUSY)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			NDK_RfidPiccDeactivate(0);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_RFID_BUSY)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			NDK_RfidPiccDeactivate(0);
			return FAIL;
		}
		if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
		{
			send_result("line %d:�µ�ʧ��(%d)", __LINE__, ret);
			return FAIL;
		}	
		if(cls_show_msg("����%s���󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��",i==0?"A":"B")==ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	//��M���Ĳ���Ѱ�����Ͳ���������� NDK5.0
	cls_show_msg("���ڸ�Ӧ������һ��Mifare-1��,���������...");
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_A))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}	
	oldtime = time(NULL);
	while((diff = time(NULL)-oldtime)<10)  //����10��Ѱ��ʱ��
	{
		if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))==NDK_OK)
		{
			flag=1;
			break;
		}	
	}
	if(flag != 1)
	{
		send_result("line %d:Ѱ��ʧ��", __LINE__);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	}
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_RFID_BUSY)//Ѱ���ɹ�������ʧ�ܷ�����Ƶ��״̬æ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	} 
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_RFID_BUSY)//Ѱ���ɹ�������ʧ�ܷ�����Ƶ��״̬æ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	} 
	if((ret=NDK_M1Anti(&pnDatalen, UID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	}
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	} 
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	} 
	if(((ret=NDK_M1Select(pnDatalen, UID, psSakbuf))!=NDK_OK)||((psSakbuf[0]&0x20)!=0x00))//����psSakbuf[0] bit5λ�ж��Ƿ�ΪM1��
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	}
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_RFID_BUSY)//����Ӧ����������ʧ�ܷ�����Ƶ��״̬æ
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	}
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	}
	if((ret=NDK_M1ExternalAuthen(pnDatalen, UID, AUTHKEY_TYPE_B, AUTHKEY, 0x01))!=NDK_OK)	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	}
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	}
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);
		return FAIL;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:�µ�ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}	
	if(cls_show_msg("����M1���󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	
	//case 8:����������֤K21�ӹܴ�������²�������
	cls_show_msg("�����K21���ӹܴ���");
	if((ret=NDK_TSKbd_Ctrl(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_TSKbd_Ctrl(0);
		return FAIL;
	}
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_PIN_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_TSKbd_Ctrl(0);
		return FAIL;
	}
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_PIN_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_TSKbd_Ctrl(0);
		return FAIL;
	} 
	//K21���ͷŴ���
	if((ret=NDK_TSKbd_Ctrl(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if(cls_show_msg("��������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	
	//case9:�������豸��������Ƶ,�ر�һ���豸�� ����һ���豸��Ȼ��æ�����ǲ�Ӧ�ý������߶��رպ���ܽ�������
	cls_show_msg("�����K21���ӹܴ���");
	if((ret=NDK_TSKbd_Ctrl(1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	cls_show_msg1(20,"����20s���ڸ�Ӧ������1�ű�׼A��");
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_A))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	oldtime = time(NULL);
	while((diff = time(NULL)-oldtime)<10)  //����10��Ѱ��ʱ��
	{
		if((ret=NDK_RfidPiccDetect(&ucPicctype))==NDK_OK)
		{
			flag=1;
			break;
		}	
	}
	if(flag != 1)
	{
		send_result("line %d:Ѱ��ʧ��", __LINE__);
		goto ERR1;
	}
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR1;
	} 
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR1;
	} 
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=RFCARD_TYPE_A)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		goto ERR1;
	}
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR1;
	}
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR1;
	}
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:�µ�ʧ��(%d)", __LINE__, ret);
		goto ERR1;
	}
	if((ret=NDK_SysReadyToSuspend())!=NDK_ERR_PIN_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if((ret=NDK_SysGoSuspend_Extern())!=NDK_ERR_PIN_BUSY)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	} 
	//K21���ͷŴ���
	if((ret=NDK_TSKbd_Ctrl(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if(cls_show_msg("��������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	if(flag_wakeup==0)
	{
		cls_show_msg("���������������������,����������,�����������");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
		if (nKeyin!=ENTER && nKeyin!=0)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	else
	{
		cls_show_msg1(3, "3s���������,��ȴ�5s�󣬽��Զ�����");
		if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		//NDK_SysDelay(5);
		usleep(5000000);
		if((ret=NDK_SysWakeUp()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return FAIL;
		}
		if(cls_show_msg("�ѻ���,û���κΰ���Ҳ�����߻���,��ȷ[Enter],����[����]")!=ENTER)
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			return FAIL;
		}
	}
	return SUCC;
ERR1:
	NDK_RfidPiccDeactivate(0);
	return FAIL;
}

void sys29(void)
{
	//����Ϊ�ֲ�����
	int ret = -1, flag_wakeup=0;
	int nKeyin;
	
	if(auto_flag==1)
	{
		//send_result("line %d:%s�޴��Զ���������", __LINE__, TESTAPI);
		return;
	}
	/*process body*/
	//case1: NDK_SysSetSuspend���������Զ��������ߣ������������� 
#if !KB_DISABLE	
	cls_show_msg("���������������������,����������,�����������");
	NDK_SysSetSuspend(1);
	if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//NDK_SysDelay(5);
	usleep(5000000);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
#endif

	cls_show_msg1(3, "3s���������,��ȴ�5s�󣬽��Զ�����");
	NDK_SysSetSuspend(1);
	if((ret=NDK_SysReadyToSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//NDK_SysDelay(5);
	usleep(5000000);
	if((ret=NDK_SysWakeUp()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(cls_show_msg("�ѻ���,û���κΰ���Ҳ�����߻���,��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}

	//case2:������������ʱ�����Ѱ�����Ӧ��Ӧ�û�ȡ��(Ӧ����)
#if !KB_DISABLE	
	cls_show_msg("���������������������,���ߺ󰴼�����");
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	nKeyin = cls_show_msg1(MAXWAITTIME, "���Ѻ�������ʾ������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
#endif

	//case3: NDK_SysSetSuspend���ò������Զ��������ߣ������������� 
#if !KB_DISABLE	
	cls_show_msg("���������������������,����������,�����������");//������Ա��Ҫע��������ֱ����水���޷��������ߣ�˵�����ڶ�����������
	NDK_SysSetSuspend(0);
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	//NDK_SysDelay(5);
	usleep(5000000);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR2;
	}
	NDK_SysSetSuspend(1);//ǰ���йر��Զ��������߿��أ�������Ҫ�򿪣����������ִ��������������޷��������� 20140509 linwl
#endif

	cls_show_msg1(3, "3s���������,��ȴ�5s�󣬽��Զ�����");
	NDK_SysSetSuspend(0);
	if((ret=NDK_SysGoSuspend_Extern()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//NDK_SysDelay(5);
	usleep(5000000);
	if((ret=NDK_SysWakeUp()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(cls_show_msg("�ѻ���,û���κΰ���Ҳ�����߻���,��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
	//������������
#if !KB_DISABLE	
	cls_show_msg("����׼����IC����SAM������Ƶ�����ſ�,���������");
	if((ret=ic_sam_suspend(flag_wakeup)) != SUCC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if((ret=prn_mag_suspend(flag_wakeup)) != SUCC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if((ret=rfid_tc_suspend(flag_wakeup)) != SUCC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
#endif
	//���ú�����������
	flag_wakeup=1;
	cls_show_msg("����׼����IC����SAM������Ƶ�����ſ�,���������");
	if((ret=ic_sam_suspend(flag_wakeup)) != SUCC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if((ret=prn_mag_suspend(flag_wakeup)) != SUCC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if((ret=rfid_tc_suspend(flag_wakeup)) != SUCC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR2:
#if !K21_ENABLE
	NDK_SysSetSuspend(1);//����������߿��أ��������������޷��������� 20140509 linwl
#endif
	return;
}


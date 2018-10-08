/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys10.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: ����NDK_SysGoSuspend�ӿ�
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI		"NDK_SysGoSuspend"

#define	MAXWAITTIME 30
#if defined SP600G||defined ME51//SP600G ֻ֧��SAM2
#define TESTSAMTYPE ICTYPE_SAM2
#else
#define TESTSAMTYPE ICTYPE_SAM1
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
*			  		      linwei	        20130121	 	created
						jiangym		 20140821		modify
*****************************************************************/
#if !K21_ENABLE  //K21ƽ̨������ȫ��Ӧ���Լ����ƣ�NDK_SysGoSuspend���������˾�һ��������ߣ����Բ�֧�ֹ���״̬�²����߹���
//IC ��SAM�ϵ�����в��ܽ�����,�Լ�����Ƕ��
static int ic_sam_suspend(void)
{
	int ret = - 1, buflen = 0, buflen1 = 0;
	int nKeyin;
	uchar buf[256]={0}, buf1[256]={0};
	
	//case5:IC/SAM������������(�ϵ��)GotoSleep��Ч
	//IC������
	cls_show_msg("�����IC1��,�������������(�����Զ�����ǰ)");
	memset(buf,0,sizeof(buf));
	if (NDK_IccPowerUp(ICTYPE_IC,buf,&buflen)!=NDK_OK)
	{
		send_result("line %d: IC1�ϵ�ʧ��", __LINE__);
		return FAIL;  
	}
	if((ret = NDK_SysGoSuspend())!=NDK_ERR_ICCARD_BUSY/*NDK_ERR*/)//����Ӧ����������ʧ�ܷ���IC��״̬æ20140613 jiangym modify
	{
		send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI,ret);
		NDK_IccPowerDown(ICTYPE_IC);
		return FAIL;
	} 
	if(NDK_IccPowerDown(ICTYPE_IC)!=NDK_OK)
	{
		send_result("line %d: IC1�µ�ʧ��", __LINE__);
		return FAIL;
	}	
	if(cls_show_msg("����IC1���󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	//�µ���Ҳ忨������ܹ���������20140613  jiangym add
	cls_show_msg("���������������������,����������,�����������");
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	NDK_SysDelay(5);
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	//���Ѻ�����������20140613  jiangym add
	memset(buf,0,sizeof(buf));
	if (NDK_IccPowerUp(ICTYPE_IC,buf,&buflen)!=NDK_OK)
	{
		send_result("line %d: IC1�ϵ�ʧ��", __LINE__);
		return FAIL;
	}
	if(NDK_IccPowerDown(ICTYPE_IC)!=NDK_OK)
	{
		send_result("line %d: IC1�µ�ʧ��", __LINE__);
		return FAIL;
	}	
	
#if SAM1_ENABLE
	//sam������
	cls_show_msg("�����SAM1��,�������������(�����Զ�����ǰ)");
	memset(buf,0,sizeof(buf));
	if (NDK_IccPowerUp(TESTSAMTYPE,buf,&buflen)!=NDK_OK)
	{
		send_result("line %d: SAM1�ϵ�ʧ��", __LINE__);
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_ICCARD_BUSY/*NDK_ERR*/)//����Ӧ����������ʧ�ܷ���IC��״̬æ20140613 jiangym modify
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_IccPowerDown(TESTSAMTYPE);
		return FAIL;
	} 
	if(NDK_IccPowerDown(TESTSAMTYPE)!=NDK_OK)
	{
		send_result("line %d: SAM1�µ�ʧ��", __LINE__);
		return FAIL;
	}	
	if(cls_show_msg("����SAM1���󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	//�µ���ܹ���������20140616  jiangym add
	cls_show_msg("���������������������,����������,�����������");
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	NDK_SysDelay(5);
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	//���Ѻ�����������20140616  jiangym add
	memset(buf,0,sizeof(buf));
	if (NDK_IccPowerUp(TESTSAMTYPE,buf,&buflen)!=NDK_OK)
	{
		send_result("line %d: SAM1�ϵ�ʧ��", __LINE__);
		return FAIL;
	}
	if(NDK_IccPowerDown(TESTSAMTYPE)!=NDK_OK)
	{
		send_result("line %d: SAM1�µ�ʧ��", __LINE__);
		return FAIL;
	}

	//case9:�������豸IC/SAM,�ر�һ���豸�� ����һ���豸��Ȼ��æ�����ǲ�Ӧ�ý������߶��رպ���ܽ�������
	cls_show_msg("��ȷ���Ѳ���IC1��SAM1��,�������������");
	memset(buf,0,sizeof(buf));
	if (NDK_IccPowerUp(ICTYPE_IC,buf,&buflen)!=NDK_OK)
	{
		send_result("line %d: IC1�ϵ�ʧ��", __LINE__);
		return FAIL;
	}
	memset(buf1,0,sizeof(buf1));
	if (NDK_IccPowerUp(TESTSAMTYPE,buf1,&buflen1)!=NDK_OK)
	{
		send_result("line %d: SAM1�ϵ�ʧ��", __LINE__);
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_ICCARD_BUSY)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_IccPowerDown(ICTYPE_IC);
		NDK_IccPowerDown(TESTSAMTYPE);
		return FAIL;
	} 
	if(NDK_IccPowerDown(TESTSAMTYPE)!=NDK_OK)
	{
		send_result("line %d: SAM1�µ�ʧ��", __LINE__);
		NDK_IccPowerDown(ICTYPE_IC);
		return FAIL;
	}	
	if(NDK_SysGoSuspend()!=NDK_ERR_ICCARD_BUSY)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_IccPowerDown(ICTYPE_IC);
		return FAIL;
	} 
	if(NDK_IccPowerDown(ICTYPE_IC)!=NDK_OK)
	{
		send_result("line %d: IC1�µ�ʧ��", __LINE__);
		return FAIL;
	}	
	if(cls_show_msg("���뿨�󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	cls_show_msg("���������������������,����������,�����������");
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	NDK_SysDelay(5);
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
#endif
	return SUCC;
}

//��ӡ��ſ���Ƕ��,�ڴſ�������´�ӡ�в��ܽ�����
static int prn_mag_suspend(void)
{
	int ret = -1, nBusy = FALSE, nErrorCode = 0;
	int nKeyin;
	EM_PRN_STATUS emStatus;
	time_t diff = 0, oldtime = 0;
	char pszTk1[MAXTRACKLEN] = {0}, pszTk2[MAXTRACKLEN] = {0}, pszTk3[MAXTRACKLEN] = {0};
	
	//�ȴ򿪴ſ�����Ƕ�ײ���
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
#if PRN_ENABLE
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
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI,ret);
			goto ERR;
		}
		if(emStatus != PRN_STATUS_BUSY)
			show_stopwatch(DISABLE, 0);
		
		switch(emStatus)
		{
		case PRN_STATUS_BUSY:
			if(NDK_SysGoSuspend()!=NDK_ERR_PRN_BUSY/*NDK_ERR*/)//��֣��ȷ���ڲ��ܽ����ߵ�����£�����ʧ��
			//����Ӧ����������ʧ�ܷ��ش�ӡæ20140613 jiangym modify
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
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
			if(NDK_PrnStr("����֮��>")!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
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
			if(NDK_PrnStr("����֮ǰ>\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n")!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
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
#endif
	cls_show_msg("���������������������,����������,�����������");
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	NDK_SysDelay(5);//0.5s��ʱ
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
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
	cls_show_msg("���������������������,����������,�����������");
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	NDK_SysDelay(5);//0.5s��ʱ
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}
	NDK_MagClose();	
	return SUCC;
ERR:
	NDK_MagClose();
	return FAIL;
}
//��Ƶ��usb��U���豸������²��ܽ�����,�Լ���Ƶ��USB��Ƕ��
static int rfid_usb_suspend(void)
{
	int ret = -1,flag = 0, pnDatalen = 0;
	int nKeyin, i = 0;
	time_t diff = 0, oldtime = 0;
	uchar ucPicctype = 0, psPicctype = 0x00, psDatabuf[256] = {0}, UID[LEN_UID]={0},psSakbuf[10]={0};
#if 0//ANDIROD_ENABLE
	iso15693ProximityCard_t cards[20];
	iso15693PiccMemoryBlock_t r_memBlock;
	uint cardcount = 0, maxcards = 20;
	uchar ucBlocknum = 0;
	iso15693PiccSystemInformation_t sysinfo;
#endif
	//case6:������Ƶ��(A.B.M)����������(Ѱ���ɹ���,��д)GotoSleep��Ч,
	cls_show_msg("��֤��Ӧ���������κο�,���������");
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:Ӧ�ü�⵽�޿�", __LINE__, ret);
		return FAIL;
	}	
	if((ret=NDK_RfidPiccDetect(&ucPicctype))!=NDK_ERR_RFID_NOCARD)
	{
		send_result("line %d:Ѱ��Ӧ��ʧ��(%d)", __LINE__, ret);
		return FAIL;
	}	
	cls_show_msg("���������������������,����������,�����������");
	if(NDK_SysGoSuspend()!=NDK_OK)//Ѱ��ʧ�ܺ���������20140613 jiangym add
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	} 
	NDK_SysDelay(5);
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
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
		if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY/*NDK_ERR*/)//Ѱ���ɹ�������ʧ�ܷ�����Ƶ��״̬æ20140616 jiangym modify
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			NDK_RfidPiccDeactivate(0);//�رճ�ǿ
			return FAIL;
		} 
		if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=(i==0?RFCARD_TYPE_A:RFCARD_TYPE_B))
		{
			send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
			NDK_RfidPiccDeactivate(0);//�رճ�ǿ
			return FAIL;
		}
		if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY/*NDK_ERR*/)//����Ӧ����������ʧ�ܷ�����Ƶ��״̬æ20140613 jiangym modify
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			NDK_RfidPiccDeactivate(0);//�رճ�ǿ
			return FAIL;
		}
		if(NDK_RfidPiccDeactivate(0)!=NDK_OK)
		{
			send_result("line %d:�µ�ʧ��", __LINE__);
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
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY)//Ѱ���ɹ�������ʧ�ܷ�����Ƶ��״̬æ
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	} 
	if((ret=NDK_M1Anti(&pnDatalen, UID))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	} 
	if(((ret=NDK_M1Select(pnDatalen, UID, psSakbuf))!=NDK_OK)||((psSakbuf[0]&0x20)!=0x00))//����psSakbuf[0] bit5λ�ж��Ƿ�ΪM1��
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY)//����Ӧ����������ʧ�ܷ�����Ƶ��״̬æ
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	}
	if((ret=NDK_M1ExternalAuthen(pnDatalen, UID, AUTHKEY_TYPE_B, AUTHKEY, 0x01))!=NDK_OK)	
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY)//����Ӧ����������ʧ�ܷ�����Ƶ��״̬æ
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	}
	if(NDK_RfidPiccDeactivate(0)!=NDK_OK)
	{
		send_result("line %d:�µ�ʧ��", __LINE__);
		return FAIL;
	}	
	if(cls_show_msg("����M1���󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}

#if 0//felica����ʱδ֧��
	//��felica���Ĳ���Ѱ�����Ͳ����������
	cls_show_msg("���ڸ�Ӧ������Felica���������������...");
	oldtime = time(NULL);
	while((diff = time(NULL)-oldtime)<10)  //����10��Ѱ��ʱ��
	{
		if((ret=NDK_RfidFelicaPoll(UID, &pnDatalen))==NDK_OK)
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
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	}
	//�µ�
	if((ret=NDK_RfidPiccDeactivate(0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if(cls_show_msg("����felica���󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
#endif

#if 0 //��δ�в�Ʒ����,Ϊ�˲�Ӱ��������Ʒ�����ȹر�,��������ʱ����
	//��M0���Ĳ���Ѱ�����Ͳ���������� 
	cls_show_msg("���ڸ�Ӧ������һ��Mifare-0��,���������...");
	oldtime = time(NULL);
	while((diff = time(NULL)-oldtime)<10)  //����10��Ѱ��ʱ��
	{
		if((ret=NDK_MifareActive(REQA, UID, &pnDatalen,psSakbuf))==NDK_OK)
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
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY)//Ѱ���ɹ�������ʧ�ܷ�����Ƶ��״̬æ
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	} 
	if((ret=NDK_M0Authen(AUTHKEY_M0))!=NDK_OK)	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY)//����Ӧ����������ʧ�ܷ�����Ƶ��״̬æ
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_RfidPiccDeactivate(0);//�رճ�ǿ
		return FAIL;
	}
	if(NDK_RfidPiccDeactivate(0)!=NDK_OK)
	{
		send_result("line %d:�µ�ʧ��", __LINE__);
		return FAIL;
	}	
	if(cls_show_msg("����M0���󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
#endif
#if 0//ANDIROD_ENABLE ��ʱ�����ڸ߶˲�Ʒ,�߶˲�Ʒ������δ������ж���֧�ְѿ��ش�
	//15693��ƬѰ�����Ͳ����������
	NDK_ISO15693_init();
	cls_show_msg("���ڸ�Ӧ������һ��15693��,���������...");
	oldtime = time(NULL);
	while((diff = time(NULL)-oldtime)<10)  //����10��Ѱ��ʱ��
	{
		if((ret=NDK_ISO15693_Inventory(ISO15693_NUM_SLOTS_16,maxcards,&cardcount,cards))==NDK_OK)
		{
			flag=1;
			break;
		}	
	}
	if(flag != 1)
	{
		send_result("line %d:Ѱ��ʧ��", __LINE__);
		NDK_ISO15693_Deinit();//�رճ�ǿ
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY)//Ѱ���ɹ�������ʧ�ܷ�����Ƶ��״̬æ
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_ISO15693_Deinit();//�رճ�ǿ
		return FAIL;
	} 
	if((ret=NDK_iso15693SelectPicc(cards[0])) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_ISO15693_Deinit();//�رճ�ǿ
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY) 
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_ISO15693_Deinit();//�رճ�ǿ
		return FAIL;
	}
	if((ret=NDK_iso15693GetPicc_SystemInfo(cards[0],&sysinfo)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_ISO15693_Deinit();//�رճ�ǿ
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY) 
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_ISO15693_Deinit();//�رճ�ǿ
		return FAIL;
	}
	if(NDK_ISO15693_Deinit()!=NDK_OK)
	{
		send_result("line %d:�µ�ʧ��", __LINE__);
		return FAIL;
	}	
	if(cls_show_msg("����15693���󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
#endif

#if USB_ENABLE
	//case 8:����������֤U�̴�����²�������
	//����Ӧ����������ʧ�ܷ���U��״̬æ20140617 jiangym add
	cls_show_msg("�����U��,���������(�����Զ�����ǰ)");
	if((ret=NDK_DiskOpen(UDISK,10))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_USB_BUSY)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_DiskClose(UDISK);
		return FAIL;
	} 
	//�ر��豸
	if((ret=NDK_DiskClose(UDISK))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if(cls_show_msg("����U�̺󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
#endif
	//case 7: usb�򿪺󲻸ý�������
	cls_show_msg("�뽫POS��PCͨ��USB������,���������(�����Զ�����ǰ)");
	if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_USB_BUSY/*NDK_ERR*/)//����Ӧ����������ʧ�ܷ���USB״̬æ20140613 jiangym modify
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_PortClose(PORT_NUM_USB);
		return FAIL;
	} 
	NDK_PortClose(PORT_NUM_USB);
	if(cls_show_msg("����USB�ߺ󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}

	//case9:�������豸usb����Ƶ,�ر�һ���豸�� ����һ���豸��Ȼ��æ�����ǲ�Ӧ�ý������߶��رպ���ܽ�������
	cls_show_msg("�뽫POS��PCͨ��USB������,���������");
	if ((ret=NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	cls_show_msg("���ڸ�Ӧ������1�ű�׼A��,���������");
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_A))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		NDK_PortClose(PORT_NUM_USB);
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
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR1;
	} 
	if((ret=NDK_RfidPiccActivate(&psPicctype, &pnDatalen, psDatabuf))!=NDK_OK||psPicctype!=RFCARD_TYPE_A)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, psPicctype);
		goto ERR1;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_RFID_BUSY)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR1;
	}
	if(NDK_RfidPiccDeactivate(0)!=NDK_OK)
	{
		send_result("line %d:�µ�ʧ��", __LINE__);
		goto ERR1;
	}
	if(NDK_SysGoSuspend()!=NDK_ERR_USB_BUSY)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		NDK_PortClose(PORT_NUM_USB);
		return FAIL;
	} 
	NDK_PortClose(PORT_NUM_USB);
	if(cls_show_msg("����USB�ߺ󵽿�������Ϣǰ�Ƿ���������\n[ENTER]��,[����]��")==ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	cls_show_msg("���������������������,����������,�����������");
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return FAIL;
	}
	NDK_SysDelay(5);
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		return FAIL;
	}
	return SUCC;
ERR1:
	NDK_RfidPiccDeactivate(0);
	NDK_PortClose(PORT_NUM_USB);
	return FAIL;
}

#if BT_ENABLE
//����ģ�������������������߷�������æ״̬ 20150128 linwl
static int bt_suspend(void)
{
	int ret = -1,recvLen = 0;
	char sendBuf[12]={0},recvBuf[12]={0};

	conf_conn_BT();

	cls_show_msg("���BTFWATS������[�ֶ�����],��ʹ���ֻ��������߽��������������,���������ʼ����");	
	if((ret = NDK_PortOpen(PORT_NUM_BT,"115200,8,N,1"))!=NDK_OK)
	{
    	send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
    	goto END;
	}
	if((ret=lib_bt_pairing(1))!=NDK_OK)
	{
		send_result("line %d:%s�������ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto END;
	}
	//��Գɹ������Ͻ�������Ӧ�÷���NDK_ERR_BT_BUSY
	if((ret=NDK_SysGoSuspend()) != NDK_ERR_BT_BUSY)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		goto END;
	}
	NDK_ScrPrintf(",�����������");
	NDK_ScrRefresh();
	NDK_KbGetCode(0, NULL);
	memset(sendBuf, 0x38, sizeof(sendBuf));
	if((ret=NDK_PortWrite(PORT_NUM_BT, sizeof(sendBuf),sendBuf))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto END;
	}
	if((ret=NDK_SysGoSuspend()) != NDK_ERR_BT_BUSY)
	{
		send_result("line %d:����ʧ��(%d)", __LINE__, ret);
		goto END;
	}
	cls_show_msg("BleTest�㿪ʼ�ش����������ڷ��ͽ��յ������ݺ�,���������");
	if((ret = NDK_PortRead(PORT_NUM_BT,sizeof(sendBuf),recvBuf, MAXWAITTIME*1000,&recvLen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret:%d)", __LINE__, TESTAPI, ret);
		goto END;
	}
	if(recvLen!=sizeof(sendBuf))
	{
		send_result("line %d:%s����ʧ��(ʵ��:%d,Ԥ��:%d)", __LINE__, TESTAPI, recvLen, sizeof(sendBuf));
		goto END;
	}
	if(memcmp(sendBuf, recvBuf, recvLen))
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto END;
	}
	cls_show_msg("����������ͨ��,���������");
	return SUCC;
END:
	NDK_BTDisconnect();
	NDK_PortClose(PORT_NUM_BT);
	return FAIL;
}
#else
static int bt_suspend(void){return SUCC;}
#endif
#endif

#if defined  ME11||defined ME15||defined ME20||defined ME15C //��Ҫ�ô��������ڵĻ����⣬ͬʱ������Ҫ�޸�Ϊ���������ͽ��
void sys10(void)
{ 
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	int ret = -1;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);
#if defined ME11		
	cls_printf("��Ƴ���5s���������,���ߺ�LED�Ʋ���,ʹ���ֻ�app���Ѻ�����3s,�̵���˸3s,�˹��ж��Ƿ�ͨ��");
#else
	cls_printf("��Ƴ���5s���������,���ߺ�LED�Ʋ���,ʹ���ֻ�������������\"ME15sys10\"����(BM77ģ���跢��������:12345����ܻ���)����ʹ��[��Դ��]����,���Ѻ�����3s,������˸3s,�˹��ж��Ƿ�ͨ��");
#endif
	if((ret=NDK_LedStatus(LED_RFID_RED_ON)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(5);
#if defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B
	NDK_PortOpen(PORT_NUM_BT, NULL);
	NDK_BTEnterCommand();
	NDK_BTSetLocalName("ME15sys10");
	NDK_BTExitCommand();
#endif
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(1);//���ߺ�����Ҫʱ����Ч
	//�����3s
	cls_printf("�������ѳɹ�,�����˸3������");
	if((ret=NDK_LedStatus(LED_RFID_RED_FLICK)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(3);
	NDK_LedStatus(LED_RFID_RED_OFF);
#if defined ME11
	//"�̵ƽ���˸3s"
	cls_printf("�̵���˸3������");
	if((ret=NDK_LedStatus(LED_RFID_GREEN_FLICK)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(3);
	NDK_LedStatus(LED_RFID_GREEN_OFF);
#else
	//"���ƽ���˸3s"
	cls_printf("������˸3������,�����˳�ǰ����");
	if((ret=NDK_LedStatus(LED_RFID_BLUE_FLICK)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(3);
	NDK_LedStatus(LED_RFID_BLUE_OFF);
#endif
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	NDK_LedStatus(LED_RFID_BLUE_FLICK);//���Ʊ�ʾӦ��������
	return;
}
#else
void sys10(void)
{
	if(auto_flag==1)	
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
	int ret = -1;
	int nKeyin;
#if defined ME15CHX||defined ME15B||defined ME66
	int handle;
#endif	
	
	/*process body*/
#if !K21_ENABLE //K21���ò˵���û������ģʽ��ʱ�����
	//[����]�е�����ʱ�������Ӱ��GotoSleep�ĵ���,ֻ��"��""ǳ"ģʽ��Ӱ��,����GotoSleep���ܿ���Ӱ��,���������
	if(ENTER!=cls_show_msg("���ڹ����зֱ���������ģʽΪ:��,��(��),��(ǳ)3��!��ENTER��ʼ����"))
	{
		send_result("line %d:%s�����ж�", __LINE__, TESTAPI);
		return;
	}
#endif
	cls_show_msg1(2, "����%s(����[����]�е�ģʽ��������)...", TESTAPI);

	//case1: NDK_SysSetSuspend���������Զ��������ߣ������������� 
	cls_show_msg("���������������������,����������,�����������");
#if !K21_ENABLE
	NDK_SysSetSuspend(1);
#endif
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
#if defined ME15CHX||defined ME15B||defined ME66
	lib_kbflush();//�����ǴӴ��ڶ�������,���Իᱻ������ж�������,Ҫ����һ��
#endif
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��:0x%2x", __LINE__, TESTAPI, nKeyin);
		RETURN;
	}

#if !(defined SP60||defined ME31S||defined ME31SLS||defined SP33||defined SP600||defined SP60G||defined SP33G||defined SP600G||defined ME51||CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG)
	//case2:������������ʱ�����Ѱ�����Ӧ��Ӧ�û�ȡ��(Ӧ����)
	cls_show_msg("���������������������,���ߺ󰴼�����");
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(1);
#if defined ME15CHX||defined ME15B||defined ME66
	lib_kbflush();//�����ǴӴ��ڶ�������,���Իᱻ������ж�������,Ҫ����һ��
#endif
	nKeyin = cls_show_msg1(MAXWAITTIME, "���Ѻ�������ʾ������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		RETURN;
	}
#endif

	//case3: NDK_SysSetSuspend���ò������Զ��������ߣ������������� 
	cls_show_msg("���������������������,����������,�����������");//������Ա��Ҫע��������ֱ����水���޷��������ߣ�˵�����ڶ�����������
#if !K21_ENABLE
	NDK_SysSetSuspend(0);
#endif
	if((ret=NDK_SysGoSuspend()) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	NDK_SysDelay(5);//���⻽�ѵĵ�Դ�������� [����]������������ʶ���������ʧ��
#if defined ME15CHX||defined ME15B||defined ME66
	lib_kbflush();//�����ǴӴ��ڶ�������,���Իᱻ������ж�������,Ҫ����һ��
#endif
	nKeyin = cls_show_msg1(MAXWAITTIME, "��������������,[ȷ��]ͨ��,[����]ʧ��");
	if (nKeyin!=ENTER && nKeyin!=0)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR2;
	}
#if !K21_ENABLE
	NDK_SysSetSuspend(1);//ǰ���йر��Զ��������߿��أ�������Ҫ�򿪣����������ִ��������������޷��������� 20140509 linwl
#endif
	
#if !K21_ENABLE  //K21ƽ̨������ȫ��Ӧ���Լ����ƣ�NDK_SysGoSuspend���������˾�һ��������ߣ����Բ�֧�ֹ���״̬�²����߹���
	cls_show_msg("����׼����IC����SAM������Ƶ�����ſ���U��,���������");
	if((ret=ic_sam_suspend()) != SUCC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if((ret=prn_mag_suspend()) != SUCC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if((ret=rfid_usb_suspend()) != SUCC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if((ret=bt_suspend()) != SUCC)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
#endif
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	goto ERR2;
ERR2:
#if !K21_ENABLE
	NDK_SysSetSuspend(1);//����������߿��أ��������������޷��������� 20140509 linwl
#endif

#if defined ME15CHX||defined ME15B
	handle = NDK_LedOpen("blue");
	NDK_LedOn(handle);
#endif
	return;
}
#endif


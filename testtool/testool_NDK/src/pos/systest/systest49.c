/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �����濨����
* file name			: systest49.c
* Author 			: 	chensj
* version			: 
* DATE				: 
* directory 			: 
* description			:  �����濨����
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20130822  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"�����濨����"
#define	DEFAULT_CNT_STR	"10"//"100"
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)
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
* history 				: author			date			remarks
					 chensj 			2013/09/17     created
*****************************************************************/
void threecard_test(SMART_t nType)
{
	int ret = 0, ret1 = -1, ret2 = -1, ret3 = -1, bak = 0, cnt = 0, nErrorCode = 0, succ = 0, pUIDlen = 0, nSta = 0, buflen = 0, rcvlen = 0;
	char CntStr[8] = DEFAULT_CNT_STR, pszTk1[MAXTRACKLEN] = {0}, pszTk2[MAXTRACKLEN] = {0}, pszTk3[MAXTRACKLEN] = {0};
	uint unLen = 0;
	char card[][8] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
	uchar UID[LEN_UID] = {0}, buf[128] = {0}, cSwiped = 0;;
	uchar sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
	ushort sw=0;
	
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	//Ϊ�˼����޼��̵Ͷ˲�ƷME11 ME15ʹ�÷�װ���� 20141031 jiangym modify
	if((ret=lib_kbgetinput(CntStr, 1, sizeof(CntStr)-1, &unLen,2, 0, INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)		
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);

	NDK_MagClose();//���ӱ�������
	//�򿪴ſ��豸
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
		return;
	}
	
	while(1)
	{
		//��������
		RFID_deactive(nType,0);
		NDK_IccPowerDown(ICTYPE_IC);
		NDK_MagReset();//����Ҷ��ͳ�ṩ�������ڶ���֮��Ҫ��λ��ͷ������ſ����������� 20140304 linwl
		if(bak++==cnt) //����˳��  ������ʾ��Ϣ 20180412 wangkai modify
			break;
		if(ESC==cls_show_msg1(2, "���ڽ���%d��%s����,�ɹ�%d��,��ˢ��/�忨/�ӿ�,ESC�˳�", bak, TESTITEM, succ))
			break;
		
		NDK_KbFlush();
		ret1=-1;
		ret2=-1;
		ret3=-1;
		cSwiped=-1;
		nSta=-1;
		while(1)
		{
			//Ѱ��
			ret3=NDK_IccDetect(&nSta);
			ret2=NDK_MagSwiped(&cSwiped);
			ret1=RFID_detect_poll(nType, UID, &pUIDlen, 1);
			
			if(lib_kbhit()==ESC||ret1==SUCC||(ret2==NDK_OK&&cSwiped==1)||(ret3==NDK_OK&&nSta==0x01))
				break;
		}

		if(ret1==SUCC)
		{
			//��Ƶ������
			if ((ret=RFID_active(nType, UID,&pUIDlen))!=SUCC)
			{	
				cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d)", __LINE__, ret);
				RFID_deactive(nType, 0);
				continue;
			}
			//��Ƶ����д
			if ((ret=RFID_APDUrw(nType, UID, &rcvlen))!=SUCC)
			{	
				cls_show_msg1(g_keeptime, "line %d:��%d��:��дʧ��(%d)", __LINE__, bak, ret);
				RFID_deactive(nType, 0);
				continue;
			}
			if((ret=RFID_deactive(nType, 0))!=NDK_OK)
			{	
				cls_show_msg1(g_keeptime, "line %d:��%d��:�µ�ʧ��(%d)", __LINE__, bak, ret);
				continue;
			}
			cls_show_msg1(5,"%s����д�ɹ�,�����߿�Ƭ",card[nType]);
			succ++;
		}	
			
		if(ret2==NDK_OK&&cSwiped==TRUE)
		{
			//ˢ������
			memset(pszTk1,0,sizeof(pszTk1));
			memset(pszTk2,0,sizeof(pszTk2));
			memset(pszTk3,0,sizeof(pszTk3));
			if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &nErrorCode))!=NDK_OK)	
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(0x%x)", __LINE__, bak, ret);
				continue;
			}
#if !TK1_DISABLE
			//��ʾ�ŵ���Ϣ
			if((ret=check_stripe_result_tk1(pszTk1))!=0x00)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(0x%x)", __LINE__, bak, ret);
				continue;
			}
#endif			
			if((ret=check_stripe_result_tk2(pszTk2))!=0x00)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(0x%x)", __LINE__, bak, ret);
				continue;
			}
			if((ret=check_stripe_result_tk3(pszTk3))!=0x00)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(0x%x)", __LINE__, bak, ret);
				continue;
			}
			succ++;
		}
			
		if(ret3==NDK_OK&&nSta==0x01)
		{
			//IC���ϵ�
			if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&buflen))!=NDK_OK)
			{	
				cls_show_msg1(g_keeptime, "line %d:��%d��:IC�ϵ�ʧ��(%d)", __LINE__, bak, ret);
				NDK_IccPowerDown(ICTYPE_IC);
				continue;
			}
			//ʹ��iccrw��ȡ���������
			memset(buf,0,sizeof(buf));
			buflen=0;
			if((ret=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf), sendbuf, &buflen, buf))!=NDK_OK||((sw=SMART_GETSW(buf+buflen-2))!=0x9000&&(sw=SMART_GETSW(buf+buflen-2))!=0x6d00))//���濨Ƭ �޷�ȡ�����������������6D00
			{						
				cls_show_msg1(g_keeptime, "line %d:��%d��:IC1����ʧ��(%d,0x%04x)", __LINE__, bak,  ret, sw);
				NDK_IccPowerDown(ICTYPE_IC);
				continue;
			}
			//�µ�
			if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
			{	
				cls_show_msg1(g_keeptime, "line %d:��%d��:IC���µ�ʧ��(%d)", __LINE__, bak, ret);
				continue;
			}
			cls_show_msg1(5,"IC����д�ɹ�,��γ�IC��");
			succ++;
		}
	}
	
	NDK_MagClose();
	RFID_deactive(nType,0);
	NDK_IccPowerDown(ICTYPE_IC);
	
	cls_show_msg("%s�������,�ܹ�����%d�γɹ�%d��\n", TESTITEM, bak-1, succ);
	return;
}

void systest49(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	int ret = 0;
	SMART_t type = CPU_A;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};
	
	cls_show_msg1(1, "%s������...", TESTITEM);

	while (1)
	{	
		ret = cls_show_msg("1.�����濨����\n"
							"0.��Ƶ������");
		switch(ret)
		{
		case '1':
			threecard_test(type);
			break;
		case '0':
			switch(type=select_smarttype())
			{
			case CPU_A:
			case CPU_B:
			case MIFARE_1:
			case FELICA:
			case MIFARE_0:	
			case ISO15693:
				if ((ret=RFID_init(type))!=SUCC)
					cls_show_msg("line %d:��ʼ��ʧ��(%d)!���������Ƿ���ȷ.", __LINE__, ret);
				else
					cls_show_msg1(2, "%s��ʼ���ɹ�!", card[type]);
				break;
			default:
				cls_show_msg("line %d:���ô���!������RFID(%d).", __LINE__, type);
				break;
			}
			break;		
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
}


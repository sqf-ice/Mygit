/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: 
* file name			: systest39.c
* Author 			: chensj
* version			: 
* DATE				: 20130722
* directory 		: 
* description		: Ӳ��ͳ�Ʒ���
* related document	: NDK.chm
*
************************************************************************/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM "Ӳ��ͳ�Ʒ���"
#define	MAXTIMEOUT 30
#define 	TEMPFILE_NUM	"temp_OnDownNum"
#define 	TEMPFILE_TIME	"temp_OnTime"
#define STRCNT "30"
#define	MAXCNT atoi (STRCNT)
#define interval_time 5  //���ʱ��ÿ�μ��5s 

/*------------global variables definition-----------------------*/
typedef enum
{
	KK_F1= 		0x01,
	KK_F2=		0x02,
	KK_F3=		0x03,
	KK_F4=		0x04,
	KK_BASP=	0x0a ,     /**<�˸��*/
	KK_ENTER=	0x0D,	/**<ȷ�ϼ�*/
	KK_ESC=		0x1B,	/**<ȡ����*/
	KK_ZMK=		0x1C,	/**<��ĸ��*/
	KK_DOT=		0x2E,	/**<С����*/
	KK_ZERO=	0x30 ,     /**<����0��*/
	KK_ONE=		0x31,      /**<����1��*/
	KK_TWO=		0x32,	/**<����2��*/
	KK_THREE=	0x33,	/**<����3��*/
	KK_FOUR=	0x34,	/**<����4��*/
	KK_FIVE=		0x35,	/**<����5��*/
	KK_SIX=		0x36,	/**<����6��*/
	KK_SEVEN=	0x37,	/**<����7��*/
	KK_EIGHT=	0x38,	/**<����8��*/
	KK_NINE=	0x39,	/**<����9��*/
}K_VALUE;

EM_SS_DEV_ID devid[] ={SS_KEYBOARD_ENTER, SS_KEYBOARD_BASP,SS_KEYBOARD_ESC,SS_KEYBOARD_ZMK,SS_KEYBOARD_ZERO,SS_KEYBOARD_DOT,\
		SS_KEYBOARD_NINE,SS_KEYBOARD_EIGHT,SS_KEYBOARD_SEVEN,SS_KEYBOARD_SIX,SS_KEYBOARD_FIVE,SS_KEYBOARD_FOUR,SS_KEYBOARD_THREE,\
		SS_KEYBOARD_TWO,SS_KEYBOARD_ONE,SS_KEYBOARD_F1,SS_KEYBOARD_F2,SS_KEYBOARD_F3,SS_KEYBOARD_F4,SS_KEYBOARD_0_ID,SS_KEYBOARD_1_ID,\
		SS_KEYBOARD_2_ID,SS_KEYBOARD_3_ID,/*SS_KEYBOARD_4_ID,SS_KEYBOARD_5_ID,SS_KEYBOARD_6_ID,SS_KEYBOARD_7_ID,SS_KEYBOARD_11_ID,\
		SS_KEYBOARD_15_ID,SS_KEYBOARD_19_ID,SS_KEYBOARD_27_ID,*/SS_KEYBOARD_TOTAL};

char devstr[][20]={"ȷ�ϼ�","�˸��","ȡ����","��ĸ��","���ּ�0","С�����",\
		"���ּ�9","���ּ�8","���ּ�7","���ּ�6","���ּ�5","���ּ�4","���ּ�3",\
		"���ּ�2","���ּ�1","�����F1","�����F2","�����F3","�����F4","δ�����0","δ�����1",\
		"δ�����2","δ�����3",/*"δ�����4","δ�����5","δ�����6","δ�����7","δ�����11",\
		"δ�����15","δ�����19","δ�����27",*/"���а���"};

K_VALUE devkey[]={KK_ENTER,KK_BASP,KK_ESC,KK_ZMK,KK_ZERO,KK_DOT,\
			KK_NINE,KK_EIGHT,KK_SEVEN,KK_SIX,KK_FIVE,KK_FOUR,KK_THREE,\
			KK_TWO,KK_ONE,KK_F1,KK_F2,KK_F3,KK_F4};

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

static void KeyValue_stat(void)
{
	ulong  stOutPut,stOutPut2;
	int nRet = 0, i = 0;
	
	//case1:�������Թ���
	for(i=0;i<(sizeof(devid)/sizeof(devid[0]));i++)
	{
		stOutPut = 0;
		if((nRet = NDK_SysGetStatisticsData(devid[i],&stOutPut))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
			return;
		}
		switch(devid[i])
		{
			case SS_KEYBOARD_ENTER:
			case SS_KEYBOARD_BASP:
			case SS_KEYBOARD_ESC:
			case SS_KEYBOARD_ZMK:
			case SS_KEYBOARD_ZERO:
			case SS_KEYBOARD_DOT:
			case SS_KEYBOARD_NINE:
			case SS_KEYBOARD_EIGHT:
			case SS_KEYBOARD_SEVEN:
			case SS_KEYBOARD_SIX:
			case SS_KEYBOARD_FIVE:
			case SS_KEYBOARD_FOUR:
			case SS_KEYBOARD_THREE:
			case SS_KEYBOARD_TWO:
			case SS_KEYBOARD_ONE:
			case SS_KEYBOARD_F1:
			case SS_KEYBOARD_F2:
			case SS_KEYBOARD_F3:
			case SS_KEYBOARD_F4:
				//��֤�Ѷ���İ���
				{
					while(1)
					{
						nRet = cls_show_msg("������һ��[%s],���޸ü���%s��������",devstr[i],(devkey[i]==K_ESC?"ENTER":"ESC"));
						if(nRet == devkey[i])
							break;
						if(devkey[i]==K_ESC&&nRet==ENTER)
							goto END;
						if(nRet == ESC)
							goto END;
					}
					stOutPut2 = 0;
					if((nRet = NDK_SysGetStatisticsData(devid[i],&stOutPut2))!=NDK_OK)
					{
						cls_show_msg1(g_keeptime,"line %d:%s����ͳ��%sʧ��(%d)", __LINE__, TESTITEM, devstr[i], nRet);
						return;
					}
					if(stOutPut2-stOutPut != 1)
					{
						cls_show_msg1(g_keeptime,"line %d:%s����ͳ��%sʧ��(%ld,%ld)", __LINE__, TESTITEM, devstr[i], stOutPut2, stOutPut);
						return;
					}
					
					END: ;
				}
				break;
			case SS_KEYBOARD_0_ID:
			case SS_KEYBOARD_1_ID:
			case SS_KEYBOARD_2_ID:
			case SS_KEYBOARD_3_ID:
				//��֤δ����İ���
				{
					stOutPut2 = 0;
					if((nRet = NDK_SysGetStatisticsData(devid[i],&stOutPut2))!=NDK_OK)
					{
						cls_show_msg1(g_keeptime,"line %d:%s����ͳ��%sʧ��(%d)", __LINE__, TESTITEM, devstr[i], nRet);
						return;
					}
					if(stOutPut2!=stOutPut)
					{
						cls_show_msg1(g_keeptime,"line %d:%s����ͳ��%sʧ��(%ld,%ld)", __LINE__, TESTITEM, devstr[i], stOutPut2, stOutPut);
						return;
					}
				}
				break;
			case SS_KEYBOARD_TOTAL:
				//��֤�ܰ�������
				{
					cls_show_msg("�����������");
					stOutPut2 = 0;
					if((nRet = NDK_SysGetStatisticsData(devid[i],&stOutPut2))!=NDK_OK)
					{
						cls_show_msg1(g_keeptime,"line %d:%s����ͳ��%sʧ��(%d)", __LINE__, TESTITEM, devstr[i], nRet);
						return;
					}
					if(stOutPut2-stOutPut!=1)
					{
						cls_show_msg1(g_keeptime,"line %d:%s����ͳ��%sʧ��(%ld,%ld)", __LINE__, TESTITEM, devstr[i], stOutPut2, stOutPut);
						return;
					}
				}
				break;
			default:
				break;
		}
	}
	return;
}
#if PRN_ENABLE//ME51��֧�ִ�ӡ20170315 linying modified
static void Prnt_stat(void)
{
	int nRet = 0;
	ulong printlong = 0, heattime = 0, printlong2 = 0, heattime2 = 0, stitch = 0, stitch2 = 0;
	
	//ͳ�ƴ�ӡ������ ��λ���� mm
	if((nRet = NDK_SysGetStatisticsData(SS_PRT_PAPER_ID,&printlong))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	//ͳ�Ƽ���ʱ��
	if((nRet = NDK_SysGetStatisticsData(SS_PRT_HEAT_ID,&heattime))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	//ͳ�ƴ�ӡ�������
	if((nRet = NDK_SysGetStatisticsData(SS_PRT_STITCH_ID,&stitch))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	cls_show_msg("ȷ���д�ӡֽ,�����������");

	if(NDK_PrnInit(0)!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}
/*
	if(NDK_PrnStr("0��������\n1��������\n2��������\n3��������\n4��������\n5��������\n6��������\n")!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}
	if(NDK_PrnStart()!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}
*/
	NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE,1);//���ֺ����ܲ��Բ���Ч��һ��
	print_fill();//ʹ����䵥�������ӡ���֣����������������ʱ���ʹ�ӡ������� 20140708 linwl
	
	//ͳ�ƴ�ӡ������ ��λ���� mm
	if((nRet = NDK_SysGetStatisticsData(SS_PRT_PAPER_ID,&printlong2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	if(cls_show_msg("��ӡ�ĳ����Ƿ�Ϊ%ld����,��:[ȷ��],��:[������]", printlong2-printlong)!=ENTER)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, printlong, printlong2);
		return;
	}
	
	//case2:ͳ����������ͷ����ʱ��		//���ͳ�� ����ײ㷵�ص���0���Ҳ�����
	if((nRet = NDK_SysGetStatisticsData(SS_PRT_HEAT_ID,&heattime2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	if(cls_show_msg("����ʱ���Ƿ�Ϊ%ld����,��:[ȷ��],��:[������]", heattime2-heattime)!=ENTER)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, heattime, heattime2);
		return;
	}

	//case2:ͳ�����������		//������ͳ�� ����ײ㷵�ص���0���Ҳ�����
	if((nRet = NDK_SysGetStatisticsData(SS_PRT_STITCH_ID,&stitch2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	if(cls_show_msg("����������Ƿ�Ϊ%ld��,��:[ȷ��],��:[������]", stitch2-stitch)!=ENTER)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, stitch, stitch2);
		return;
	}
	
	cls_show_msg("ͳ�����ӡ�豸����ͨ��,���������");
	return;
}
#else
static void Prnt_stat(void) {cls_show_msg("��֧�ִ�ӡ!"); return;}
#endif
static void Rfid_stat(void)
{
	int nRet = 0, pUIDlen = 0, i = 0, cnt = 0, succ = 0, rcvlen = 0;
	ulong seektimes = 0, seektimes2 = 0;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA"};
	uchar UID[LEN_UID] = {0};
	SMART_t cardtype = CPU_A;
	
	cardtype = select_smarttype();
	switch(cardtype)
	{
		case CPU_A:
		case CPU_B:
		case MIFARE_1:
		case FELICA:
		case MIFARE_0:	
			break;
		case SAM:
		case IC:
			cls_show_msg1(3,"Ĭ����ƵA��,���������");
			cardtype = CPU_A;
			break;
	}
	
	if(RFID_init(cardtype)!=SUCC)
		cls_show_msg("line %d:��ʼ��ʧ��!���������Ƿ���ȷ.", __LINE__);
	else
		cls_show_msg1(2, "%s��ʼ���ɹ�!", card[cardtype]);
	
	//case1:ͳ��Ѱ������	 
	if((nRet = NDK_SysGetStatisticsData(SS_RFID_TIMES_ID,&seektimes))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}

	cnt = rand()%20+1;
	for(i=0;i<cnt;i++)
	{	
		memset(UID,0,sizeof(UID));
		pUIDlen = 0;
		RFID_deactive(cardtype, 0);
		cls_printf("���һ��%s��,�����������,����%d��,�ܹ�%d��",card[cardtype],i+1,cnt);
		//Ѱ��
		if((nRet=RFID_detect(cardtype, UID, &pUIDlen))!=SUCC)
		{
			cls_show_msg1(2,"Ѱ��ʧ��");
			continue;
		}	
		//����
		if ((nRet=RFID_active(cardtype, UID,&pUIDlen))!=SUCC)
		{	
			continue;
		}
		//��д
		if ((nRet=RFID_APDUrw(cardtype, UID, &rcvlen))!=SUCC)
		{	
			continue;
		}
		//�رճ�
		if ((nRet=RFID_deactive(cardtype, 0))!=SUCC)
		{	
			continue;
		}
		succ++;
	}
	if((nRet = NDK_SysGetStatisticsData(SS_RFID_TIMES_ID,&seektimes2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM,nRet);
		return;
	}
	//�ײ����һ��Ѱ��������Ѱ����Σ��޷���ȷ�������,��ģ���Ƚϴ�С,jiangym
	if(seektimes2<=seektimes)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, seektimes2, seektimes);
		return;
	}
	
	cls_show_msg("ͳ��%s������ͨ��,�����ܴ�������������,���������",card[cardtype]);
	return;
}

//IC���忨��������δ֧��
static void Ic_stat(void)
{
	int nRet = 0, nAtrlen=0, i=0, cnt=0, succ=0;
	ulong ictimes = 0, ictimes2 = 0;
	uchar psAtrbuf[256]={0};
	
	//case1:ͳ�Ʋ忨����
	 
	if((nRet = NDK_SysGetStatisticsData(SS_ICCARD_BASE_ID,&ictimes))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM,nRet);
		return;
	}
	cnt = rand()%20+1;
	
	for(i=0;i<cnt;i++)
	{
		memset(&psAtrbuf, 0, sizeof(psAtrbuf));
		nAtrlen = 0;
		cls_show_msg("�����IC����,���������,����%d��,�ܹ�%d��", i+1, cnt);
		if((nRet=NDK_IccPowerUp(ICTYPE_IC, psAtrbuf, &nAtrlen))!=NDK_OK)
		{
			continue;
		}
		if((nRet=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
		{
			continue;
		}		
		//cls_show_msg("��ε�IC��,���������");4.1�汾�̼�:���ϵ������Ϊ�忨�������������Բ��Թ����в��ο� 20140623 linwl
		succ++;
	}
	if((nRet = NDK_SysGetStatisticsData(SS_ICCARD_BASE_ID,&ictimes2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	if((ictimes2-ictimes) !=succ)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, ictimes2, ictimes);
		return;
	}

	cls_show_msg("ͳ����IC������ͨ��,���������");
	return;
}

static void Mag_stat(void)
{
	int nRet = 0, pnErrorCode = 0, i = 0, cnt = 0, succ = 0;
	ulong magtimes = 0, magtimes2 = 0;
	uchar pcSwiped = 0;
	char pszTk1[128] = {0}, pszTk2[128] = {0}, pszTk3[128] = {0};

	//case1:ͳ��ˢ������	 
	if((nRet = NDK_SysGetStatisticsData(SS_MAG_TIMES_ID,&magtimes))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM,nRet);
		return;
	}

	if((nRet=NDK_MagOpen())!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	
	cnt = rand()%20+1;
	
	for(i=0;i<cnt;i++)
	{
		pcSwiped = 0;
		pnErrorCode = 0;
		memset(pszTk1,0,sizeof(pszTk1));
		memset(pszTk2,0,sizeof(pszTk2));
		memset(pszTk3,0,sizeof(pszTk3));
		NDK_MagReset();//����Ҷ��ͳ�ṩ�������ڶ���֮��Ҫ��λ��ͷ������ſ����������� 20140304 linwl
		cls_show_msg("��ˢ�������������,��%d��,�ܹ�%d��",i+1,cnt);
		
		if((nRet=NDK_MagSwiped(&pcSwiped))!=NDK_OK||pcSwiped!=0x01)
		{
			continue;
		}		
		
		if((nRet=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode))!=NDK_OK)
		{
			continue;
		}
		succ++;
	}
	NDK_MagClose();
	
	if((nRet = NDK_SysGetStatisticsData(SS_MAG_TIMES_ID,&magtimes2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		NDK_MagClose();
		return;
	}
	if((magtimes2-magtimes) != succ)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, magtimes2, magtimes);
		return;
	}
	cls_show_msg("ͳ�������������ͨ��,���������");
	return;
}

static void Card_stat(void)
{
	int nKeyin=0;
	
	while(1)
	{
		nKeyin = cls_show_msg("ͳ�ƿ����豸\n"
								"1.������ 2.IC��\n"
								"3.��Ƶ��\n");
		switch(nKeyin)
		{
			case '1':
				Mag_stat();
				break;
			case '2':
				Ic_stat();
				break; 
			case '3':
				Rfid_stat();
				break;
			case ESC:
				return;
			default:
				break;
		}
	}
}

static void Wireless_stat(void)
{
	int nRet = 0,cnt = 0,i = 0,succ=0,nLen = 0;
	ulong ppptime=0, ppptime2=0, wlstimes=0, wlstimes2=0, failtimes=0, failtimes2=0;
	char CntStr[3]=STRCNT;
	
	if(getlinktype()!=GPRS && getlinktype()!=CDMA && getlinktype()!=TD)
	{
		cls_show_msg("���ô���");
		return;
	}

	cls_printf("ͳ�������߲���,Ĭ��ѹ�����Դ���:");
	if ((nRet=lib_kbgetinput(CntStr,0, sizeof(CntStr)-1,(uint *)&nLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||nRet==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = MAXCNT;
	else
		cnt = atoi(CntStr);
	
	//case1:ͳ����������ʱ�� 
	if((nRet = NDK_SysGetStatisticsData(SS_WLS_PPPTIME_ID,&ppptime))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	
	//case2:ͳ���������Ӵ���
	 
	if((nRet = NDK_SysGetStatisticsData(SS_WLS_TIMES_ID,&wlstimes))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}

	//case3:ͳ����������ʧ�ܴ���	
	if((nRet = NDK_SysGetStatisticsData(SS_WLS_FAILTIMES_ID,&failtimes))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}

	for(i=0;i<cnt;i++)
	{
		cls_printf("ͳ������������,��%d��,�ܹ�%d��",i+1,cnt);
		if(NetUp()!=SUCC)
		{
			continue;
		}
		sleep(10);
		NetDown();
		succ++;
	} 

	//����ʱ�����
	if((nRet = NDK_SysGetStatisticsData(SS_WLS_PPPTIME_ID,&ppptime2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	if(ppptime >= ppptime2)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, ppptime2, ppptime);
		return;
	}

	//���Ӵ������
	if((nRet = NDK_SysGetStatisticsData(SS_WLS_TIMES_ID,&wlstimes2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM,nRet);
		return;
	}
	if(wlstimes2-wlstimes != cnt)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, wlstimes2, wlstimes);
		return;
	}
 	
	//����ʧ�ܴ������
	if(succ != cnt)
	{
		if((nRet = NDK_SysGetStatisticsData(SS_WLS_FAILTIMES_ID,&failtimes2))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
			return;
		}
		if((failtimes2-failtimes) != (cnt - succ))
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, failtimes2, failtimes);
			return;
		}
	}
	if((cls_show_msg("�Ƿ������������ʧ�ܴ���ͳ��\n[ȷ��]�� [����]��"))==ENTER)
	{
		cls_show_msg("�����������ô���ķ�������ַ�������д˰���,���������");
		return;
	}
	cls_show_msg("ͳ�������߲��Գɹ�,���������");
	return;
}

#if (SDLC_ENABLE||ASYN_ENABLE)
static void Modem_stat(void)
{
	int nRet = 0,cnt = 0,i = 0,succ=0,nLen = 0, ret = -1;
	ulong modemtimes=0, modemtimes2=0, connectime=0, connectime2=0, failtimes=0, failtimes2=0;
	char CntStr[3]=STRCNT;

	if(getlinktype()!=ASYN&&getlinktype()!=SYNC)
	{
		cls_show_msg("���ô���");
		return;
	}

	cls_printf("ͳ����modem����,Ĭ��ѹ�����Դ���:");
	if ((nRet=lib_kbgetinput(CntStr,0, sizeof(CntStr)-1,(uint *)&nLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||nRet==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = MAXCNT;
	else
		cnt = atoi(CntStr);
	
	//case1:ͳ��modem���Ӵ���
	if((nRet = NDK_SysGetStatisticsData(SS_MODEM_TIMES_ID,&modemtimes))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	
	if(getlinktype()==SYNC)
	{
		//case2:ͳ��modemͬ��ʱ�� 
		if((nRet = NDK_SysGetStatisticsData(SS_MODEM_SDLCTIME_ID,&connectime))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
			return;
		}
	}
	else
	{
		//case3:ͳ��modem�첽ʱ��
		if((nRet = NDK_SysGetStatisticsData(SS_MODEM_ASYNTIME_ID,&connectime))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
			return;
		}
	}

	//case4:ͳ��modem����ʧ�ܴ���
	if((nRet = NDK_SysGetStatisticsData(SS_MODEM_FAILTIMES_ID,&failtimes))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	
	for(i=0;i<cnt;i++)
	{	
		cls_printf("modem���ӶϿ���,��%d��,�ܹ�%d��",i+1,cnt);
		if((ret=mdm_init())!=SUCC)
		{
			cls_show_msg1(3, "line %d:��ʼ��MODEM(��%d��)ʧ��(%d)", __LINE__, i+1, ret);
			continue;
		}
		if(mdm_dial(MDMDialStr)!=SUCC)
		{
			cls_show_msg1(3, "line %d:MODEM����(��%d��)ʧ��", __LINE__, i+1);
			continue;
		}
		if(mdm_hangup()!=SUCC)
		{
			cls_show_msg1(3, "line %d:MODEM�Ҷ�(��%d��)ʧ��", __LINE__, i+1);
			//continue;
		}
		sleep(3);
		succ++;
	}

	//modem���Ӵ������
	if((nRet = NDK_SysGetStatisticsData(SS_MODEM_TIMES_ID,&modemtimes2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	if(modemtimes2-modemtimes != cnt)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, modemtimes2, modemtimes);
		return;
	}

	//modem����ʱ�����
	if(getlinktype()==SYNC)
	{
		if((nRet = NDK_SysGetStatisticsData(SS_MODEM_SDLCTIME_ID,&connectime2))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
			return;
		}
		if(connectime >= connectime2)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, connectime2, connectime);
			return;
		}
	}
	else
	{	 
		if((nRet = NDK_SysGetStatisticsData(SS_MODEM_ASYNTIME_ID,&connectime2))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
			return;
		}
		if(connectime >= connectime2)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, connectime2, connectime);
			return;
		}
	}

	//modem����ʧ�ܴ���
	if(cnt != succ)
	{
		if((nRet = NDK_SysGetStatisticsData(SS_MODEM_FAILTIMES_ID,&failtimes2))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM,nRet);
			return;
		}
		if((failtimes2-failtimes) != (cnt-succ))
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, failtimes2, failtimes);
			return;
		}
	}
	else
	{	
		mdm_init();
		mdm_dial("5840");	
		mdm_hangup();
		
		modemtimes2 = 0;
		if((nRet = NDK_SysGetStatisticsData(SS_MODEM_TIMES_ID,&modemtimes2))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM,nRet);
			return;
		}
		if(modemtimes2-modemtimes != (cnt+1))
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, modemtimes2, modemtimes);
			return;
		}
		
		if((nRet = NDK_SysGetStatisticsData(SS_MODEM_FAILTIMES_ID,&failtimes2))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM,nRet);
			return;
		}
		if((failtimes2-failtimes) != 1)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, failtimes2, failtimes);
			return;
		}
	}
	cls_show_msg("ͳ����modem���Գɹ�,���������");
	return;
}
#else
static void Modem_stat(void){cls_show_msg("����Ʒ��֧������mdm,������˳�");return;}
#endif

static void Wifi_stat(void)
{
	int nRet = 0,nLen = 0,cnt = 0,i = 0;
	ulong wifitimes=0, wifitimes2=0, connectime=0, connectime2=0;
	ST_WIFI_PARAM param;
	time_t diff = 0, oldtime = 0;
	EM_WPA_CONSTATE state;
	char CntStr[3]=STRCNT;
	float con_time = 0.0, total_time = 0.0;

	if(getlinktype()!=WLAN)
	{
		cls_show_msg("���ô���");
		return;
	}
	
	cls_printf("ͳ����wifi����,Ĭ��ѹ�����Դ���:");
	if ((nRet=lib_kbgetinput(CntStr,0, sizeof(CntStr)-1,(uint *)&nLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||nRet==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = MAXCNT;
	else
		cnt = atoi(CntStr);
	
	//case1:ͳ��wifi���Ӵ���
	if((nRet = NDK_SysGetStatisticsData(SS_WIFI_TIMES_ID,&wifitimes))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	
	//case2:ͳ��wifi����ʱ��
	if((nRet = NDK_SysGetStatisticsData(SS_WIFI_TIME_ID,&connectime))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}

	if(cls_show_msg("��ȷ��������AP...ESC�˳���ģ�����")==ESC)
		return;

	memset(&param,0,sizeof(param));
	param.pszKey =  linksetting.WLANPwd;
	param.emKeyType = linksetting.WLANKeyType;
	param.emSecMode = linksetting.WLANEncMode;
	param.psEthIp = netsetting.LocalIP.Str;
	param.psEthGateway = netsetting.LocalGateway.Str;
	param.psEthNetmask =  netsetting.LocalMask.Str;
	param.ucIfDHCP = netsetting.DHCPenable;
	param.psEthDnsPrimary = netsetting.DNSHost[0];
	param.psEthDnsSecondary = netsetting.DNSHost[1];

	for(i = 0;i < cnt;i++)
	{
		cls_printf("Wifiͳ�Ʋ�����,��%d��,�ܹ�%d��",i+1,cnt);
		NDK_WiFiShutdown();
		if(NDK_WiFiInit()!=NDK_OK)
		{
			continue;
		}	

		if ((nRet=NDK_WiFiConnect(WlanEssid, &param)) != NDK_OK)
		{
			continue;
		}

		oldtime = time(NULL);
		state = -1;
		while(1)
		{
			if ((nRet=NDK_WiFiConnectState(&state)) != NDK_OK||(diff=time(NULL)-oldtime)>MAXTIMEOUT)
			{
	    		cls_show_msg1(10, "line %d:����%sʧ��!(%d)", __LINE__, WlanEssid, nRet);
	    		NDK_WiFiShutdown();
		    	break ;
	    	}
			show_stopwatch(ENABLE, diff);
			
			if (state == WIFI_WPA_CONSTATE_CONTEXT)
			{
				lib_StartStopwatch();
				break;
			}
			else
				NDK_SysMsDelay(10);//�ȶ�һ��ʱ���ٿ���
		}
		show_stopwatch(DISABLE, diff);
		sleep(3);
		NDK_WiFiShutdown();
		con_time=lib_ReadStopwatch();
		total_time=total_time+con_time;
	}

	//���Ӵ������
	if((nRet = NDK_SysGetStatisticsData(SS_WIFI_TIMES_ID,&wifitimes2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	if(wifitimes2-wifitimes != cnt)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, wifitimes2, wifitimes);
		return;
	}
	
	//����ʱ�����
	if((nRet = NDK_SysGetStatisticsData(SS_WIFI_TIME_ID,&connectime2))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	if(((connectime2 - connectime)/1000- total_time)>interval_time)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%ld,%ld)", __LINE__, TESTITEM, connectime2, connectime);
		return;
	}
	cls_show_msg("ͳ����wifi���Գɹ�,���������");
	return;
}

void Commconf(void)
{
	int nKeyin=0;
	
	while(1)
	{
		nKeyin = cls_show_msg("ͨѶ����\n"
								"1.��������\n"
								"2.modem����\n"
								"3.Wifi����\n");
		switch(nKeyin)
		{
			case '1':
				conf_conn_WLM(TRUE);
				break;
			case '2':
				conf_conn_MDM();
				break;
			case '3':
				conf_conn_WLAN();
				break;
			case ESC:
				return;
			default:
				break;
		}
	}
}

static void Comm_stat(void)
{
	int nKeyin=0;
	
	while(1)
	{
		nKeyin = cls_show_msg("ͳ��ͨѶ��\n"
								"1.����  2.modem\n"
								"3.Wifi  4.����\n");
		switch(nKeyin)
		{
			case '1':
				Wireless_stat();
				break;
			case '2':
				Modem_stat();
				break;
			case '3':
				Wifi_stat();
				break;
			case '4':
				Commconf();
				break;
			case ESC:
				return;
			default:
				break;
		}
	}
}

static void Record_Read(int *Record)
{
	int fp;
	char szBuf[20] = {0};
	int nRet = 0;
	int nNum=0;
	
	if((fp = NDK_FsOpen(TEMPFILE_NUM,"r"))<0)
	{
		NDK_FsClose(fp);	
		return;
	}
	nRet = NDK_FsSeek(fp, 0, SEEK_SET);
	if(nRet != NDK_OK)
	{
		NDK_FsClose(fp);	
		return ;
	}
	nRet = NDK_FsRead(fp, szBuf,sizeof(szBuf));
	if(nRet != sizeof(szBuf))
	{
		NDK_FsClose(fp);	
		return ;
	}
	NDK_C4ToInt((unsigned int * )&nNum, (unsigned char * )szBuf);
	*Record=nNum;
	NDK_FsClose(fp);	
	return;
}

static void Record_Write(int Record)
{
	int fp;
	char szBuf[20] = {0};
	int nRet = 0;
	
	if((fp = NDK_FsOpen(TEMPFILE_NUM,"w"))<0)
	{
		NDK_FsClose(fp);	
		return ;
	}
	nRet = NDK_FsSeek(fp, 0, SEEK_SET);
	if(nRet != NDK_OK)
	{
		NDK_FsClose(fp);	
		return ;
	}
	NDK_IntToC4((unsigned char * )szBuf, Record);
	
	nRet = NDK_FsWrite(fp, szBuf,sizeof(szBuf));
	if(nRet != sizeof(szBuf))
	{
		NDK_FsClose(fp);	
		return ;
	}
	NDK_FsClose(fp);	
	return ;
}

static void OnDownNum(void)
{
	int nRet = 0,nNum=-1;
	ulong pstOutPut ;
	
	//case1:ͳ�ƿ��ػ�����:�������ٴν���˰��������״ν���
	if((cls_show_msg("�״ν���˰���\n[ȷ��]�� [����]��"))==ENTER)
	{
		cls_show_msg("��������,�ٽ���˰���,ͬʱҪѡ���,���������");
		NDK_FsDel(TEMPFILE_NUM);
	}
	nRet = NDK_FsExist(TEMPFILE_NUM);
	if(nRet == NDK_OK)
	{
		Record_Read(&nNum);
	}
	
	if((nRet = NDK_SysGetStatisticsData(SS_POWER_TIMES_ID,&pstOutPut))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
		return;
	}
	
	Record_Write(pstOutPut);
	if(nNum!=-1)
	{
		if(pstOutPut-nNum!=1)
		{
			cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d,%ld)", __LINE__, TESTITEM, nNum, pstOutPut);
			return;
		}
		cls_show_msg("���ش�������ͨ��,���������");
	}
	
}

static void OnTime(void)
{
	int nRet = 0;
	ulong pstOutPut ;

	//case1:ͳ�ƿ��ػ�ʱ��
	 
	if((nRet = NDK_SysGetStatisticsData(SS_POWERUP_TIME_ID,&pstOutPut))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��(%d)", __LINE__, TESTITEM,nRet);
		return;
	}
	if(cls_show_msg("�Ƿ��״ν���˰���,[ȷ��]��,����¼POS�豸��Ϣͳ��ֵ%ld��,�ٴν���˰���,[����]��",pstOutPut)==ENTER)
		return;
	if(cls_show_msg("POS�豸��Ϣͳ��ֵ%ld��,�Ƿ�����״μ�¼��,[ȷ��]��,[����]��",pstOutPut) != ENTER)
	{
		cls_show_msg1(g_keeptime,"line %d:%s����ʧ��", __LINE__, TESTITEM);
		return;
	}
}

static void Pos_stat(void)
{
	int nKeyin=0;

	while(1)
	{
		nKeyin = cls_show_msg("1.���ػ�����\n"
							  "2.��������\n"
							  "3.����ʱ��");
		switch(nKeyin)
		{
			case '1':
				OnDownNum();
				break;
			case '2':
				NDK_SysReboot();
				break;
			case '3':
				OnTime();
				break;
			case ESC:
				return;
			default:
				break;
		}
	}
}

//�ϵ��쳣���ϵ�������ִ����������Ӧ������
static void powerdown_stat(void)
{
	int nRet = 0;
	
	cls_printf("��ε�Դ,���������������������Ӧͨ��");
	while(1) //��ѭ��
	{
		if((nRet = NDK_SysInitStatisticsData())!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, nRet);
			return;
		}
	}	
	return;
}

//������
static void reboot_stat(void)
{
	int nRet = 0 , i = 0, nKey = 0;
	ulong stOutPut = 0;

	nKey = cls_show_msg("�밴��ѡ����Ҫͳ�Ƶļ�");
	for(i = 0; i<SZ_ARRAY(devkey) ;i++)
	{
		if(nKey == devkey[i])
			break;
	}

	if((nRet = NDK_SysGetStatisticsData(devid[i],&stOutPut))!=NDK_OK)
	{
		cls_show_msg("line %d:%s����ʧ��(%s,%d)", __LINE__, TESTITEM,devstr[i],nRet);
		return;
	}

	cls_show_msg("ͳ��%s�İ�������Ϊ%d��,��¼������������,���������",devstr[i],stOutPut);

	cls_show_msg("��ػ�������������,��ȡ%s�İ�������Ӧ�������ڵĴ���",devstr[i]);
	return;
}

static void Abnormal_stat(void)
{
	int nKeyin=0;

	while(1)
	{	
		nKeyin = cls_show_msg("1.�����쳣\n"
							  "2.�ϵ��쳣\n");
		switch(nKeyin)
		{
			case '1':
				reboot_stat();
				break;
			case '2':
				powerdown_stat();
				break;
			case ESC:
				return;
			default:
				break;
		}
	}
}

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		   chensj	          20130722	 	created
*****************************************************************/
void systest39(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	//����Ϊ�ֲ�����
	int nKeyin=0;

	//����Ϊ������
	cls_printf("����%s...", TESTITEM); 

	while(1) 
	{
		nKeyin = cls_show_msg("ͳ���ຯ������\n"
								"1.��ֵ    2.��ӡ\n"
								"3.����    4.ͨѶ\n"
								"5.���ػ�  6.�쳣\n");
		switch(nKeyin)
		{
			case '1':
				KeyValue_stat();
				break;
			case '2':
				Prnt_stat();
				break;
			case '3':
				Card_stat();
				break;
			case '4':
				Comm_stat();
				break;
			case '5':
				Pos_stat();
				break;
			case '6':
				Abnormal_stat();
				break;
			case ESC:
				return;
			default:
				break;
		}
	}
	return;
}


/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��Ƶ������,ѹ������
* file name		: 13.c
* Author 			: 
* version			: V1.0
* DATE			: 20130118
* directory 		: 
* description		: ��Ƶ������,ѹ������
* related document	: NL8200����Ա�ο��ֲ�.doc
*				  
************************************************************************
* log			: 
* Revision 1.0  20130118 10.26 chenfm
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"��Ƶ������,ѹ��"
#define TESTBGPIC "test_bg.png"	//���ݿ���Ҫ�󽫲����ļ��ŵ�main�ļ�����·������������һ�������� 20160513

#define	DEFAULT_CNT_STR	"15000"	//"3000"//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(1000)

#define	DEFAULT_CNT_STR2	"5000"	//"3000"//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE2	atoi(DEFAULT_CNT_STR2)	//(1000)

#define	DEFAULT_CNT_STR3	"4000"	//"1000"//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE3	atoi(DEFAULT_CNT_STR3)	//(1000)

#define	TIMEOUT	30
#define	DEFAULT_WAVE_NUM		"100"

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
*			  		  chenfm 	20130118		created
*****************************************************************/
//���˵��:����ע���ܡ�ѹ��(������һ��ǿ�����Ƿ�ɹ�)��,����ע����
//Ҳ����עability����,����ֵ�ں������й����и���
static void RFID_ability(SMART_t type)
{
	/*private & local definition*/
#define	nTestTime	(10)	//(5)

	//int nTestTime = 5;
	int count = 0, pUIDlen = 0, rcvlen = 0;
	float fTimeElapsed = 0.0;
#if K21_ENABLE
	char str[32] = {0};
#else
	time_t oldtime = 0, diff = 0;
#endif

/*
	uchar Select_1pay[] = //�˴�����Ӧ��RFID_iccrw��ʹ�õ�һ��
#if 1	//���ŵĿ���֧��Select_1pay����,֧��ȡ���������
	{'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
#else
	{0x00, 0xA4, 0x04, 0x00, 0x0E,
	  '1',  'P',  'A',  'Y',  '.',  'S',  'Y',  'S',  '.',  'D',  'D',  'F',  '0',  '1',
	  0x00
	};//��ѯCPU�����ļ�Ŀ¼�ṹ,mifare-1����֧��
#endif
*/

	uchar UID[LEN_UID] = {0};
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};

	/*process body*/
	cls_show_msg1(g_keeptime, "���ڸ�Ӧ������%s��,���������...", card[type]);
	//Ѱ��
#if 0
	cls_printf("���ڲ���Ѱ���ٶ�...\n");
	//count = 0;
	lib_StartStopwatch();
	while(RFID_detect(type, UID,&pUIDlen)==SUCC)
	{
		count++;
		if((fTimeElapsed=lib_ReadStopwatch())>nTestTime)
			break;
		if(RFID_deactive(type, 10)!=SUCC)
			break;
	}
	if(count>0)//��Ϊ���ܿ����ڴ˲��Գ�����һ��100%�ɹ�,��ֻ���������ɹ���ƽ��ʱ�伴��//if(fTimeElapsed>nTestTime)
		cls_show_msg("Ѱ���ٶ�:%.0f��/��", fTimeElapsed/count);
	else
	{
		cls_show_msg("line %d:Ѱ���з���ʧ��(cnt=%d, err=%x)\n", __LINE__, count, GetErrorCode());
		return;
	}
#else	//������һ��Ѱ����ʱ��
	cls_printf("���ڲ���Ѱ���ٶ�...\n");
	lib_StartStopwatch();
	if(RFID_detect(type, UID,&pUIDlen)!=SUCC)//����ע�ɹ���,��ѹ������ȥ��ע
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:Ѱ��ʧ��", __LINE__);
		goto ERR;
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME,"Ѱ%s���ٶ�:%.0f��/��", card[type],lib_ReadStopwatch());//ʱ���С,�����ⲻ����
#else
	memset(str,0,sizeof(str));
	ftos(lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME,"Ѱ%s���ٶ�:%s��/��",card[type], str);//ʱ���С,�����ⲻ����
#endif
#endif
	
	//����
	if (RFID_active(type, UID,&pUIDlen)!=SUCC)//����ע�ɹ���,��ѹ������ȥ��ע
	{	
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:����ʧ��", __LINE__);
		goto ERR;
	}
	
	//����iccrw����
	cls_printf("���ڲ��Զ�д�ٶ�...\n");
	count = 0;
#if !K21_ENABLE//
	oldtime = time(NULL);//�ӵ�һ���յ�æ״̬��ʼ��ʱ
	while(1)
	{
		lib_StartStopwatch();
		if(RFID_APDUrw(type, UID, &rcvlen)==SUCC)
		{
			count++;//�ۼӳɹ�����
			fTimeElapsed+=lib_ReadStopwatch();//�ۼӳɹ�ʱ��   //K21ƽ̨�ܱ�����20ms���ϲ��ܶ���ʱ�䣬���Դ˴�������ʱ�䶼��0������������ͳ�Ʒ�ʽ20150325 linwl guoc
		}
		else
		{
			//����м�ʧ�����µ����ϵ�
			NDK_RfidPiccDeactivate(0);
			RFID_detect(type, UID,&pUIDlen);
			RFID_active(type, UID,&pUIDlen);
		}
	
		if(fTimeElapsed>nTestTime)//�ۼƳɹ�����10�����˳�
			break;
		if((diff=time(NULL)-oldtime)>5*nTestTime)//�ܶ�дʱ�䳬��50�����˳�
			break;
	}
#else
	lib_StartStopwatch();
	while(RFID_APDUrw(type, UID, &rcvlen)==SUCC)
	{
		count++;
		if((fTimeElapsed=lib_ReadStopwatch())>nTestTime)
			break;
	}
#endif

#if !K21_ENABLE
	if(fTimeElapsed>nTestTime)
		cls_show_msg_record(FILENAME,FUNCNAME,"ÿ���%s��:%.0f�ֽ�",card[type], count*((type==MIFARE_1||type==MIFARE_0)?LEN_BLKDATA:(type==FELICA?29:rcvlen))/fTimeElapsed);//20150728 ����FELICA��Ƭ�����ܣ��ÿ�����֤�������ݳ���Ϊ29
#else
	memset(str,0,sizeof(str));
	ftos(count*((type==MIFARE_1||type==MIFARE_0)?LEN_BLKDATA:(type==FELICA?29:rcvlen))/fTimeElapsed,str);//Select_1pay[4]�޸ĳ�rcvlen 20161116 linwl
	if(fTimeElapsed>nTestTime)
		cls_show_msg_record(FILENAME,FUNCNAME,"ÿ���%s��:%s�ֽ�",card[type], str);
#endif
	else 
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:�ۼƳɹ���дʱ�䲻��10��(fTimeElapsed=%f)\n", __LINE__, fTimeElapsed);
	
	//�رճ�
	if (RFID_deactive(type, 0)!=SUCC)//����ע�ɹ���,��ѹ������ȥ��ע
	{	
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:�رճ�ʧ��", __LINE__);
		return;
	}
	return;
ERR:
	NDK_RfidPiccDeactivate(0);//�˳�ǰ�رճ�
	return;
}

//��Ƶ��ѹ������
//���˵��:��ע��һ��ǿ�����Ƿ�����������
//���ڽӴ�/�ǽӵ�ѹ������,����Ҫ�ж�iccrw��Ľӿڷ��سɹ���,��Ҫ������״̬��(SW)��״̬����н�һ���ж�!!!
static void RFID_press(SMART_t type)
{
	/*private & local definition*/
	int ret = 0, succ = 0, cnt = DEFAULT_CNT_VLE2, bak = 0,pUIDlen = 0, rcvlen = 0;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR2;
	uchar UID[LEN_UID] = {0};
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};

	/*process body*/
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)	
		bak = cnt = DEFAULT_CNT_VLE2;
	else	
		bak = cnt = atoi(CntStr);

	cls_show_msg1(g_keeptime, "���ڸ�Ӧ������%s��,���������...", card[type]);
	while(cnt)
	{
		if (ESC==cls_show_msg1(1, "ѹ��������...\n��ʣ%d��(�ѳɹ�%d��),ESC�жϲ���", cnt, succ))
			break;
		cnt--;

		//Ѱ��
		if((ret=RFID_detect(type, UID,&pUIDlen))!=SUCC)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d��:Ѱ��ʧ��(%d)", __LINE__, bak-cnt, ret);
			continue;//break;			
		}	
		//����
		if ((ret=RFID_active(type, UID,&pUIDlen))!=SUCC)
		{	
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d��:����ʧ��(%d)", __LINE__, bak-cnt, ret);
			continue;//break;
		}
		//��д
		if ((ret=RFID_APDUrw(type, UID, &rcvlen))!=SUCC)
		{	
			RFID_deactive(type, 10);
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d��:��дʧ��(%d)", __LINE__, bak-cnt, ret);
			continue;//break;
		}
		//�����Զ����ߺ�������1s���һ���Ƿ���æ״̬,��Ƶ����ز����ܿ�,�ᵼ���ڼ��ʱ���⵽��Ƶ���ڹ�����״̬,
		//Ϊ�˴����,������ʱ,��æ��״̬��һ��,�ͱ��ⱻ��׽������״̬,�Ͳ���������߶����º���Ѱ��ʧ��(������� 20180118)
		NDK_SysDelay(10);//��ʱ1s
		//�رճ�
		if ((ret=RFID_deactive(type, 0))!=SUCC)
		{	
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��:�رճ�ʧ��(%d)", __LINE__, bak-cnt, ret);
			continue;//break;
		}
		succ++;
	}

	RFID_deactive(type, 0);//�˳�ǰ�رճ�
	cls_show_msg_record(FILENAME,FUNCNAME,"ѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;
}

static void RFID_rw_press(SMART_t type)
{
	/*private & local definition*/
	int ret = 0, succ = 0, cnt = DEFAULT_CNT_VLE, bak = 0,pUIDlen = 0, rcvlen = 0;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uchar UID[LEN_UID] = {0};
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};

	/*process body*/
	//����ʹ�ô���������,����ʱ��(��Ϊ��дSAM�й�ʱ���ж�,��ʱ�нϴ�ƫ��)
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)	
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	cls_show_msg1(g_keeptime, "���ڸ�Ӧ������%s��,���������...", card[type]);
	while(cnt)
	{
		//Ѱ��
		if((ret=RFID_detect(type, UID,&pUIDlen))!=SUCC)
		{
			cnt--;
			if(ESC==cls_show_msg1(g_keeptime, "line %d:��%d��:Ѱ��ʧ��(%d)", __LINE__, bak-cnt, ret))
				break;
			else
				continue;//break;
		}
		//����
		if ((ret=RFID_active(type, UID,&pUIDlen))!=SUCC)
		{	
			cnt--;
			if(ESC==cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%d)", __LINE__, bak-cnt, ret))
				break;
			else
				continue;//break;
		}		

		while(cnt)
		{
			cls_printf("��дѹ��������...\n��ʣ%d��(�ѳɹ�%d��),�����ĵȴ�", cnt, succ);
			cnt--;

			//��д
			if ((ret=RFID_APDUrw(type, UID, &rcvlen))!=SUCC)
			{	
				RFID_deactive(type, 10);
				cls_show_msg1(g_keeptime, "line %d:��%d��:��дʧ��(%d)", __LINE__, bak-cnt, ret);
				break;					
			}
			succ++;
		}
	}

	//�رճ�
	if ((ret=RFID_deactive(type, 0))!=SUCC)//�������,���������ùر�
	{	
		cls_show_msg1(g_keeptime, "line %d:�رճ�ʧ��(0x%x)", __LINE__, ret);
		//��ʱ�µ�ʧ��,��������ѹ�����Գɹ���return;
	}

	cls_show_msg_record(FILENAME,FUNCNAME,"��дѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;
}

//��Ƶ���ӿ�����
//���˵��:��ע��Ӧ�ó������豸���Ǵӳ��⵽���ڣ��ڴӳ��ڵ������ģ�����
static void RFID_wave_press(SMART_t type)
{
	/*private & local definition*/
	int ret = 0, succ = 0, cnt = 200, bak = 0, pUIDlen = 0, i = 0, rcvlen = 0;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_WAVE_NUM;
	uchar UID[LEN_UID] = {0};
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0", "15693"};

	/*process body*/
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)	
		bak = cnt = 100;
	else	
		bak = cnt = atoi(CntStr);
		
	cls_show_msg("�ӿ�����ʱ�볢�Խ���Ƭ�ӽ�Զ���루�뿪��Ӧ���ˣ��Ĳ�ͬ�ķ��������Ӧ��,���������ʼ����");
	while(cnt)
	{
		if (ESC==cls_show_msg1(1, "����3���ڻ�%s��,��ʣ%d��(�ѳɹ�%d��),ESC�жϲ���", card[type], cnt, succ))
			break;
		cnt--;

		//Ѱ��
		if((ret=RFID_detect_poll(type, UID, &pUIDlen, 3))!=SUCC)
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��:Ѱ��ʧ��(%d)", __LINE__, bak-cnt, ret);
			continue;//break;			
		}	
		//����
		if ((ret=RFID_active(type, UID,&pUIDlen))!=SUCC)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:����ʧ��(%d)", __LINE__, bak-cnt, ret);
			continue;//break;
		}
		for(i = 0;i<3;i++)
		{
			//��д
			if ((ret=RFID_APDUrw(type, UID, &rcvlen))!=SUCC)
			{	
				RFID_deactive(type, 10);
				cls_show_msg1(g_keeptime, "line %d:��%d��:��дʧ��(%d)(i=%d)", __LINE__, bak-cnt, ret, i);
				continue;//break;
			}
		}
		//�رճ�
		if ((ret=RFID_deactive(type, 0))!=SUCC)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:�رճ�ʧ��(%d)", __LINE__, bak-cnt, ret);
			continue;//break;
		}
		cls_show_msg1(3,"����3���ڽ�%s���Ƴ���Ӧ��",card[type]);
		succ++;
	}

	RFID_deactive(type, 0);//�˳�ǰ�رճ�
	cls_show_msg("�ӿ�ѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;
}

static void RFID_compatibility(SMART_t type)
{
	/*private & local definition*/
	int ret = -1, pUIDlen = 0,keyin = 0, rcvlen = 0;
	uchar UID[LEN_UID] = {0};

	/*process body*/
	switch(type)
	{
#if 0	//����rf6��
	case MIFARE_1:// 4k M-1���Ĳ���
		cls_show_msg("���ڸ�Ӧ������1��4k M1��,���������...");
		//Ѱ��
		if((ret=RFID_detect(type, UID,&pUIDlen))!=SUCC)
		{
			cls_show_msg("line %d:Ѱ��ʧ��(ret=%d)", __LINE__, ret);
			goto ERR;
		}		
		//��֤/����,�˴�ʹ���ⲿ��֤(�ڲ���֤����,����Կ��Ĵ洢��������Ӱ��)
		//���ڲ�֪��4k����B��Կ�Ƕ���,ʹ��A��Կ(������A��ԿΪȫ0xff)
		if((ret= NDK_M1ExternalAuthen(4,UID,AUTHKEY_TYPE_A, AUTHKEY,0xfe))!=NDK_OK)//��֤�Ŀ��ڵ�4K�ڵ�����
		{
			cls_show_msg("line %d:�ⲿ��֤ʧ��(ret=%d)", __LINE__, ret);
			goto ERR;	
		}
		//��д,���ǵ����Կ�����,����������
		if((ret= NDK_M1Read(0xfd,&pnDatalen,out))!=NDK_OK)//0xfe���������Ŀ�Ӧ���ܶ�
		{
			cls_show_msg("line %d:���ʧ��(ret=%d)", __LINE__, ret);
			goto ERR;
		}
		//�رճ�
		if ((ret=RFID_deactive(type, 0))!=SUCC)
		{	
			cls_show_msg("line %d:�رճ�ʧ��(ret=%d)", __LINE__, ret);
			goto ERR;
		}
		break;
#endif
	case CPU_A:
	case CPU_B:
		do
		{
			keyin = cls_show_msg("_�Ǳ꿨����_\n"
								"1.����Э��\n"
		  						"2.����Ѱ��\n");
		  						
			switch (keyin)
			{
			//case 0://0Ϊ��ʱ,Ĭ������GPRS
			case '1':
				NDK_SetIgnoreProtocol(1);
				break;
			case '2':
				NDK_PiccQuickRequest(1);
				break;
			case ESC:
				return;
				break;
			default:
				cls_show_msg1(1, "��Ҫ�˳�,�밴ESC...");
				continue;
				break;
			}
		}while(keyin!=ESC && (keyin<'1'||keyin>'2'));
	  							
	  	cls_show_msg("���ڸ�Ӧ������1�ŷǱ�׼%c��,���������...", (type==CPU_A)?'A':'B');

		//Ѱ��
		if((ret=RFID_detect(type, UID,&pUIDlen))!=SUCC)
		{
			cls_show_msg("line %d:Ѱ��ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}	
		//����
		if ((ret=RFID_active(type, UID,&pUIDlen))!=SUCC)
		{	
			cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		//��д
		if ((ret=RFID_APDUrw(type, UID, &rcvlen))!=SUCC)
		{	
			cls_show_msg("line %d:��дʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		//�رճ�
		if ((ret=RFID_deactive(type, 0))!=SUCC)
		{	
			cls_show_msg("line %d:�رճ�ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}

		//����(�ָ�Э���жϵ�)
		//NDK_SetIgnoreProtocol(0);//�������Ա�,�ָ�Ĭ��	
		//NDK_PiccQuickRequest(0);//�رռ��׿���Ѱ������
		break;
	case MIFARE_1:
	case FELICA:
	case ISO15693:
	case MIFARE_0:	
		cls_show_msg("���޼����Բ�����Ŀ!��ѡ������������");
		return;
		break;		
	default:
		cls_show_msg("��֧�ֵĿ�����!");
		return;
		break;
	}

	cls_show_msg("�����Բ��������");
	//return;
ERR:
	NDK_SetIgnoreProtocol(0);// �������Ա�,�ָ�Ĭ��	
	NDK_PiccQuickRequest(0);//�رռ��׿���Ѱ������
	RFID_deactive(type, 0);//�˳�ǰ�رճ�
	return;
}

//M1����������д����20170409 linying added
static void M1_multisection_rw(SMART_t type)
{
    /*private & local definition*/
	int ret = -1,s=1,k=0,succ=0, pnDatalen=16,i=0;
	int pUIDlen = 0, rcvlen = 0;
	uchar UID[LEN_UID] = {0},out[MAXLEN_DATA] = {0},out1[MAXLEN_DATA] = {0};
	int cnt = DEFAULT_CNT_VLE3, bak = 0;
	char CntStr[8] = DEFAULT_CNT_STR3;
	uint unLen = 0;	
	uchar RF_bak[64][LEN_BLKDATA+8];
	for(i=0;i<64;i++)
	{
	    memcpy(RF_bak[i],DATA16,LEN_BLKDATA);
	}
    /*process body*/
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)	
		bak = cnt = DEFAULT_CNT_VLE3;
	else	
		bak = cnt = atoi(CntStr);
	cls_show_msg1(g_keeptime, ("�뽫M-1�������ڸ�Ӧ��,���������ʼ����"));
	while(cnt)
	{
		//Ѱ��
		if((ret=RFID_detect(type, UID,&pUIDlen))!=SUCC)
		{
			cnt--;
			if(ESC==cls_show_msg1(g_keeptime, "line %d:��%d��:Ѱ��ʧ��(%d)", __LINE__, bak-cnt, ret))
				break;
			else
				continue; 
		}
	    	while(cnt)
		{
		   	cls_printf("��дѹ��������...\n��ʣ%d��(�ѳɹ�%d��),�����ĵȴ�", cnt, succ);
			cnt--;
		        s=rand()%15;//���ѡȡ��ȡ������
		        //����
		        if ((ret=NDK_M1ExternalAuthen(pUIDlen, UID, AUTHKEY_TYPE_A, AUTHKEY,4*s))!=SUCC)
		        {	 
				RFID_deactive(type, 0);
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d��:����ʧ��(%d)", __LINE__, bak-cnt, ret);
				break;
		         }
		        //˳���ȡ������ǰ�������,ʧ�ܲ��µ�
		        for(k=4*s;k<4*s+2;k++)
		        {
				if((ret=NDK_M1Read(k, &rcvlen, out))!=NDK_OK) 
				 {
					  //RFID_deactive(type, 0); 
					  cls_show_msg1(g_keeptime, "%s,line %d:M-1���ʧ��(%d)",__FILE__ , __LINE__, ret);
					  break;
				  }
				 //Ԥ��rcvlenӦ����LEN_BLKDATA
				 if(!memcmp(RF_bak[k],DATA16, rcvlen))//�״�
					memcpy(RF_bak[k], out, rcvlen);
				 else if(memcmp(RF_bak[k], out, rcvlen))//������
				 { 
					 //RFID_deactive(type, 0); 
					 cls_show_msg1(g_keeptime, "%s,line %d:M-1��������У��ʧ��(%d)",__FILE__ , __LINE__,  rcvlen);
					 break;
				 }
			}
			//����һ��������һ�����д����������д������У�飬��дʧ�ܲ��µ�
			//����
			if ((ret=NDK_M1ExternalAuthen(pUIDlen, UID, AUTHKEY_TYPE_A, AUTHKEY,4*(s+1)))!=SUCC)
			{	
				RFID_deactive(type, 0);
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:��%d��:����ʧ��(%d)", __LINE__, bak-cnt, ret);
				break;
			}
			//�����ÿ�ԭ������
			if((ret=NDK_M1Read(4*(s+1), &rcvlen, out))!=NDK_OK) 
			{
				//RFID_deactive(type, 0); 
				cls_show_msg1(g_keeptime, "%s,line %d:M-1���ʧ��(%d)",__FILE__ , __LINE__, ret);
				break;
			}
			//д��ԭ������
			if((ret=NDK_M1Write(4*(s+1), &pnDatalen, out))!=NDK_OK)
			{
				//RFID_deactive(type, 0); 
				cls_show_msg1(g_keeptime, "%s,line %d:M-1������дʧ��(%d)",__FILE__ , __LINE__,  ret);
				break;
			}
			//��ȡ��д������
			if((ret=NDK_M1Read(4*(s+1), &rcvlen, out1))!=NDK_OK) 
			{
				//RFID_deactive(type, 0); 
				cls_show_msg1(g_keeptime, "%s,line %d:M-1���ʧ��(%d)",__FILE__ , __LINE__, ret);
				break;
			}
			//�Ա�����
			if(memcmp(out1, out, rcvlen))//�Ƚ�ʵ��д�������Ƿ��Ԥ��д������һ��
			{ 
				//RFID_deactive(type, 0); 
				cls_show_msg1(g_keeptime, "%s,line %d:M-1��д����У��ʧ��(%d)",__FILE__ , __LINE__,  rcvlen);
				break;
			}
			succ++;
		}
	}
	RFID_deactive(type, 0);//�˳�ǰ�رճ�
	cls_show_msg_record(FILENAME,FUNCNAME,"ѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;	
}

void systest13(void)
{
	/*private & local definition*/
	int keyin = 0;
	SMART_t type = CPU_A;
	char *card[] = {"CPU_A", "CPU_B", "M-1", "FELICA", "M-0","15693"};
	
	/*process body*/	
#if !K21_ENABLE
	int ret = -1;
	//20140611 linwl added ���䱳��ͼƬ��֤�б���ͼƬ�����Һ�Ժ������ò������𱳾�ͼƬ��ʾ�쳣
	if(cls_show_msg1(g_keeptime,"�Ƿ�Ҫ���ñ���ͼƬ,�ǰ�[ȷ��],����[����]")==ENTER)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"��������%s����ͼƬ�����������",TESTBGPIC);
		if((ret=NDK_ScrSetbgPic(TESTBGPIC))!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ʾ����ͼƬʧ��(%d)", __LINE__, ret);
			return;
		}
	}
#endif
	cls_show_msg1(g_keeptime, "%s������...���Ƚ�������,���������ʼ", TESTITEM);
	if(auto_flag==1)
	{
		memcpy(g_RF_bak/*gen31_bak*/, DATA16, LEN_BLKDATA);//����ǰ,�ָ�һ��ȫ���Ա��⻻M-1���ܽ�����ȷ����������
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		type = select_smarttype();
		if(RFID_init(type)!=SUCC)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:��ʼ��ʧ��!���������Ƿ���ȷ.", __LINE__);
			return;
		}
		else
			cls_show_msg1(2, "%s��ʼ���ɹ�!", card[type]);
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ��ѹ������
		RFID_press(type);

		//ִ�����ܲ���
		RFID_ability(type);

		//ִ�ж�д����
		RFID_rw_press(type);
		
		//M1����������д����
		if(type==MIFARE_1)	
			M1_multisection_rw(type);
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		NDK_SetIgnoreProtocol(0);//�������Ա�,�ָ�Ĭ��	
		NDK_PiccQuickRequest(0);//�رռ��׿���Ѱ������,�����µ��Ҳ��ָ�Ĭ������0.�˴���Ϊ�˷�ֹʹ�ú���B�������׼B��ʱ���Ի�ͨ�� 
		RFID_finish(type);//��ESC��֧���������Ϊ�˺�case '0'�е�RFID_init��Ӧ
		return;
	}
	while(1)
	{
		keyin = cls_show_msg("1.�ۺ�ѹ�� 2.����\n"
							"3.��д   4.�ӿ�\n"
							"5.������ 0.����\n"
							"6.M1������ ");
		
		memcpy(g_RF_bak/*gen31_bak*/, DATA16, LEN_BLKDATA);//����ǰ,�ָ�һ��ȫ���Ա��⻻M-1���ܽ�����ȷ����������
		switch(keyin) 
		{
		case '1':// 1.һ��ǿ�ȵ�ѹ������;
			cls_show_msg1(2, "RFID�ۺ�ѹ������...");
			RFID_press(type);
			break;
		case '2':// 2.���ܲ���
			cls_show_msg1(2, "RFID���ܲ���...");
			RFID_ability(type);
			break;
		case '3':
			if(ENTER!=cls_show_msg("RFID��дר��ѹ������(���Կ�ʼ��,������ESC�жϲ���),ENTER������..."))
				break;
			RFID_rw_press(type);
			break;
		case '4':
			if(ENTER!=cls_show_msg("RFD�ӿ�ѹ������,ENTER������..."))
				break;
			RFID_wave_press(type);
			break;
		case '5':
			cls_show_msg1(2, "RFID�����Բ���...");
			RFID_compatibility(type);
			break;
		case '6':
			cls_show_msg1(2, "M1��������д����...");
			M1_multisection_rw(MIFARE_1);
			break;
		case '0':
			type = select_smarttype();
			//cls_show_msg("���ڸ�Ӧ��������Ӧ�Ŀ�!���������...");
			if(RFID_init(type)!=SUCC)
				cls_show_msg("line %d:��ʼ��ʧ��!���������Ƿ���ȷ.", __LINE__);
			else
				cls_show_msg1(2, "%s��ʼ���ɹ�!", card[type]);
			break;
		case ESC:
			NDK_SetIgnoreProtocol(0);//�������Ա�,�ָ�Ĭ��	
			NDK_PiccQuickRequest(0);//�رռ��׿���Ѱ������,�����µ��Ҳ��ָ�Ĭ������0.�˴���Ϊ�˷�ֹʹ�ú���B�������׼B��ʱ���Ի�ͨ��	
			RFID_finish(type);//��ESC��֧���������Ϊ�˺�case '0'�е�RFID_init��Ӧ
			cls_show_msg("�뻻����RFID���²���!���������...");
#if !K21_ENABLE
			NDK_ScrClrbgPic();
#endif
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}


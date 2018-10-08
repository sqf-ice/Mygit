/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: IC������,ѹ������
* file name		: 5.c
* Author 			: chenfm
* version			: V1.0
* DATE			: 20130115
* directory 		: 
* description		: IC������,ѹ������
* related document	: NL8200����Ա�ο��ֲ�.doc
*				  
************************************************************************
* log			: 
* Revision 1.0  20130115 10:10 chenfm
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"IC������,ѹ��"

#define	DEFAULT_CNT_STR	"15000"	//"3000"//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(1000)

#define	DEFAULT_CNT_STR2	"3000"	//"3000"//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE2	atoi(DEFAULT_CNT_STR2)	//(1000)

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
*			  		  chenfm 		   	20130115		created
*****************************************************************/
//���˵��:����ע���ܡ�ѹ��(������һ��ǿ�����Ƿ�ɹ�)��,����ע����
//Ҳ����עability����,����ֵ�ں������й����и���
static void IC_SAM_ability(void)
{
	/*private & local definition*/
#include "icsam.h"
#define	nTestTime	(10)	//(5)

	//int nTestTime = 5;
	int ret = -1,i = 0, rlen = 0, count = 0,pnAtrlen = 0;
	uchar buf[128] = {0};
#if K21_ENABLE
	char str[32] = {0};
#else
	time_t oldtime = 0, diff = 0;
#endif
	float fTimeElapsed = 0.0;
#if defined ME15CHX||defined ME15B
	uint time1 = 0, time2 = 0;
#endif

	uchar sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������

	/*process body*/
	do
	{
		//�ϵ�
		if ((ret=NDK_IccPowerUp(CardNo[i], buf,&pnAtrlen))!=NDK_OK)//����ע�ɹ���,��ѹ������ȥ��ע
		{	
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s�ϵ�ʧ��", __LINE__, CardNoStr[i]);
			continue;
		}
		//����iccrw����
		cls_printf("���ڲ���NDK_Iccrw��%s��д�ٶ�...\n", CardNoStr[i]);
		count = 0;
		fTimeElapsed = 0;
		memset(buf,0,sizeof(buf));
#if !K21_ENABLE
		oldtime = time(NULL);//�ӵ�һ���յ�æ״̬��ʼ��ʱ
		while(1)
		{
			lib_StartStopwatch();
			if(NDK_Iccrw(CardNo[i],sizeof(sendbuf),sendbuf,&rlen,buf)==NDK_OK)
			{
				count++;//�ۼӳɹ�����
				fTimeElapsed+=lib_ReadStopwatch();//�ۼӳɹ�ʱ��
			}
			else
			{
				NDK_IccPowerDown(CardNo[i]);
				NDK_IccPowerUp(CardNo[i], buf,&pnAtrlen);
			}

			if(fTimeElapsed>nTestTime)//��дʱ�䳬��10�����˳�
				break;
			if((diff=time(NULL)-oldtime)>5*nTestTime)//�ۼƶ�дʱ�䳬��50�����˳�
				break;
		}
#elif defined ME15CHX||defined ME15B
		NDK_SysReadWatch(&time1);
		while((ret=NDK_Iccrw(CardNo[i],sizeof(sendbuf),sendbuf,&rlen,buf))==NDK_OK)
		{
			count++;
			NDK_SysReadWatch(&time2);
			if((fTimeElapsed=(time2-time1)/1000)>nTestTime)	
				break;
		}
#else	
		lib_StartStopwatch();
		while((ret=NDK_Iccrw(CardNo[i],sizeof(sendbuf),sendbuf,&rlen,buf))==NDK_OK)
		{
			count++;
			if((fTimeElapsed=lib_ReadStopwatch())>nTestTime)	
				break;
		}
#endif
		if(fTimeElapsed>nTestTime)
#if !K21_ENABLE
			cls_show_msg_record(FILENAME,FUNCNAME,"NDK_Iccrwÿ���%s��:%.0f�ֽ�", CardNoStr[i], count*rlen/fTimeElapsed);//zhangnw20110308����:ԭ���㷽��Ϊcount*sizeof(sendbuf)/fTimeElapsed, sendbuf[4]�ĳ�rlen 20161116 linwl
#else
		{
			memset(str,0,sizeof(str));
			ftos(count*rlen/fTimeElapsed,str);
			cls_show_msg_record(FILENAME,FUNCNAME,"NDK_Iccrwÿ���%s��:%s�ֽ�", CardNoStr[i], str);
		}
#endif
		else 
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:�ۼƳɹ���дʱ�䲻��10��(%s��, fTimeElapsed=%f)\n", __LINE__, CardNoStr[i], fTimeElapsed);

		//�µ�
		if (NDK_IccPowerDown(CardNo[i])!=NDK_OK)//����ע�ɹ���,��ѹ������ȥ��ע
		{	
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:%s�µ�ʧ��", __LINE__, CardNoStr[i]);
			continue;
		}
	}while(++i!=(ICSAMNUM/*SZ_ARRAY(CardNo)-1*/));
	
	return;
}

//IC/SAM ��ѹ������
//���˵��:��ע��һ��ǿ�����Ƿ�����������
//���ڽӴ�/�ǽӵ�ѹ������,����Ҫ�ж�iccrw��Ľӿڷ��سɹ���,��Ҫ������״̬��(SW)��״̬����н�һ���ж�!!!
static void IC_SAM_press(void)
{
	/*private & local definition*/
#include "icsam.h"
	
	int i = 0, ret = 0, rlen = 0, succ = 0,pnAtrlen = 0;
	uint unLen = 0;
	uchar buf[128] = {0};
	int cnt = DEFAULT_CNT_VLE2, bak = 0;
	char CntStr[8] = DEFAULT_CNT_STR2;
	ushort sw=0;
	uchar sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
	//���������Ŀ��Ĳ�ͬ,�еĿ���֧�����������,������޹�,��ʹ�õĲ��Կ��й�
	//char sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 0};

	/*process body*/
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE2;
	else
		bak = cnt = atoi(CntStr);
	//NDK_IccSetType(ICTYPE_ISO7816);//ISO7816�Ŀ�Ƭ����ѹ������ʱ��Ҫ���Ӹ�����
	do
	{		
		cnt = bak;
		succ = 0;
		while(cnt)
		{
			if (ESC==cls_show_msg1(1, "%sѹ��������...\n��ʣ%d��(�ѳɹ�%d��),ESC�жϲ���", CardNoStr[i], cnt, succ))
				break;
			cnt--;
					
			//�ϵ�
			//memset(buf,0,sizeof(buf));
			if ((ret=NDK_IccPowerUp(CardNo[i], buf,&pnAtrlen))!=NDK_OK)
			{	
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��:%s�ϵ�ʧ��(%d)", __LINE__, bak-cnt, CardNoStr[i], ret);
				continue;//break;
			}
			//ʹ��iccrw��ȡ���������
			memset(buf,0,sizeof(buf));
			if((ret=NDK_Iccrw(CardNo[i], sizeof(sendbuf), sendbuf, &rlen, buf))!=NDK_OK||((sw=SMART_GETSW(buf+rlen-2))!=0x9000&&(sw=SMART_GETSW(buf+rlen-2))!=0x6d00))
			{	
				NDK_IccPowerDown(CardNo[i]);						
				cls_show_msg1_record(FILENAME,FUNCNAME, g_keeptime, "line %d:��%d��:%s����ʧ��(%d,0x%04x)", __LINE__, bak-cnt, CardNoStr[i], ret,sw);
				continue;//break;					
			}		
			//�µ�
			if ((ret=NDK_IccPowerDown(CardNo[i]))!=NDK_OK)
			{	
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��:%s�µ�ʧ��(%d)", __LINE__, bak-cnt, CardNoStr[i], ret);
				continue;//break;
			}
			succ++;
		}
		cls_show_msg_record(FILENAME,FUNCNAME,"%sѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", CardNoStr[i], bak-cnt, succ);
	}while(++i!=(ICSAMNUM/*SZ_ARRAY(CardNo)-1*/));//������0xff
	
	return;
}

static void IC_SAM_rw_press(void)
{
	/*private & local definition*/
#include "icsam.h"
	
	int i = 0, ret = 0, rlen = 0, succ = 0,pnAtrlen = 0;
	uint unLen = 0;
	uchar buf[128] = {0};
	int cnt = DEFAULT_CNT_VLE, bak = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	ulong sw = 0;
	uchar sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 8};//ȡ���������
	//���������Ŀ��Ĳ�ͬ,�еĿ���֧�����������,������޹�,��ʹ�õĲ��Կ��й�
	//char sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 0};

	/*process body*/
	if(g_SequencePressFlag)
	{
		i = 0;  //���������ѹ�����Դ�IC1��ʼ�������
	}
	else
	{
		if((i=select_ICSAM())==NDK_ERR_QUIT)
			return;	
		else
			cls_show_msg1(2, "��ѡ��%s",  CardNoStr[i]);		
	}
	
	//����ʹ�ô���������,����ʱ��(��Ϊ��дSAM�й�ʱ���ж�,��ʱ�нϴ�ƫ��)
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	//if ((ret=getnumstr(CntStr, sizeof(CntStr)-1, 2, 0))==QUIT||ret==TIMEOUT)//��ȡ��,��ʹ��default
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	while(1)
	{
		if(g_SequencePressFlag)  
		{
			if(cnt == 0) /*һ�ֿ�ѭ������, ���Լ�׼�����б��ֿ���ѭ��*/
			{
				if(++i==(ICSAMNUM))
					break;
				else
				{
					if ((ret=NDK_IccPowerDown(CardNo[i-1]))!=NDK_OK) //�µ���һ��δ�µ�Ŀ�
					{	
						cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:%s�µ�ʧ��(%d)", __LINE__, CardNoStr[i-1], ret);
					}
					cls_show_msg_record(FILENAME,FUNCNAME,"%s��дѹ�����,��ִ�д���Ϊ%d,�ɹ�%d��", CardNoStr[i-1],bak, succ);
					cnt = bak;   /*ÿ�ֿ�Ƭ�������ֵ��������*/
					succ = 0;    /*ÿ�ֿ�Ƭ���������*/
				}
			}
		}
		else
		{
			if(cnt == 0) 
				break;
		}
		//�ϵ�
		//memset(buf,0,sizeof(buf));
		if ((ret=NDK_IccPowerUp(CardNo[i], buf,&pnAtrlen))!=NDK_OK)
		{
			cnt--;
			if(ESC==cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��:%s�ϵ�ʧ��(%d)", __LINE__, bak-cnt, CardNoStr[i], ret))
				break;
			else
				continue;
		}
			
		while(cnt)
		{
			cls_printf("%s��дѹ����...\n��ʣ%d��(�ѳɹ�%d��),�����ĵȴ�", CardNoStr[i], cnt, succ);
			cnt--;
			
			//ʹ��iccrw��ȡ���������
			//memset(buf,0,sizeof(buf));
			if((ret=NDK_Iccrw(CardNo[i], sizeof(sendbuf), sendbuf, &rlen, buf))!=NDK_OK||((sw=SMART_GETSW(buf+rlen-2))!=0x9000&&(sw=SMART_GETSW(buf+rlen-2))!=0x6d00))
			{	
				NDK_IccPowerDown(CardNo[i]);						
				cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��:%s����ʧ��(%d,0x%04x)", __LINE__, bak-cnt, CardNoStr[i], ret,sw);
				break;//continue,�˴�����ʹ��continue���������д�����Ѿ��µ��˻�����ʧ�ܣ������޷��˳�����	
			}
						
			succ++;
		}
	}
	
	//�µ�
	if(g_SequencePressFlag)  
		--i;  /* ��������ѹ������������ѭ��ʱ�����������������������ѹ������������Ҫ�Լ� */
	
	if ((ret=NDK_IccPowerDown(CardNo[i]))!=NDK_OK)
	{	
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime,"line %d:%s�µ�ʧ��(%d)", __LINE__, CardNoStr[i], ret);
		//��ʱ�µ�ʧ��,��������ѹ�����Գɹ���return;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"%s��дѹ�����,��ִ�д���Ϊ%d,�ɹ�%d��", CardNoStr[i],bak-cnt, succ);
	return;
}

void IC_abnormal1(void)
{
	int i = 0, ret = 0, buflen=0, nSta = 0, pnAtrlen = 0;
	uchar buf[128] = {0};
	uchar sendbuf[] = {'\x00', '\x84', '\x00', '\x00', 8};
	ulong sw = 0;
	
	for(i=0;i<2;i++)
	{
		//��ȷ�忨�ϵ�
		cls_printf("������(��ȷ��ʽ)����IC��...");	

		while(1)
		{
			nSta = 0;
			if((ret=NDK_IccDetect(&nSta))!=NDK_OK)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��:̽��ʧ��(%d)", __LINE__, i+1, ret);
				return;
			}
			if(nSta==0x01)
				break;
			sleep(1);
		}
		
		if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&buflen))!=NDK_OK)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:IC�ϵ�ʧ��(%d)", __LINE__, i+1, ret);
			return;
		}
		//ʹ��iccrw��ȡ���������
		memset(buf,0,sizeof(buf));
		buflen=0;
		if((ret=NDK_Iccrw(ICTYPE_IC, sizeof(sendbuf), sendbuf, &buflen, buf))!=NDK_OK||((sw=SMART_GETSW(buf+buflen-2))!=0x9000&&(sw=SMART_GETSW(buf+buflen-2))!=0x6d00))
		{	
			NDK_IccPowerDown(ICTYPE_IC);						
			cls_show_msg1(g_keeptime, "line %d:��%d��:IC1����ʧ��(%d,0x%04x)", __LINE__, i+1,  ret,sw);
			return;//break;					
		}
		//�µ�
		if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:IC���µ�ʧ��(%d)", __LINE__, i+1, ret);
			return;
		}

		cls_show_msg("�뽫���γ�,�����������..");
		if((ret=NDK_IccDetect(&nSta))!=NDK_OK||(nSta&0xff)!=0x00)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:IC��δ�γ�(%d)", __LINE__, i+1, ret);
			return;
		}
		if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:IC���µ�ʧ��(%d)", __LINE__, i+1, ret);
			return;
		}
		if((ret=NDK_IccDetect(&nSta))!=NDK_OK||(nSta&0xff)!=0x00)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:IC��δ�γ�(%d)", __LINE__, i+1, ret);
			return;
		}
		
		//����忨�ϵ�
		cls_printf("�뷴��(����ʽ)����IC��...");
		while(1)
		{
			nSta = 0;
			if((ret=NDK_IccDetect(&nSta))!=NDK_OK)
			{
				cls_show_msg1(g_keeptime, "line %d:��%d��:̽��ʧ��(%d)", __LINE__, i+1, ret);
				return;
			}
			if(nSta==0x01)
				break;
			sleep(1);
		}
		
		memset(buf,0,sizeof(buf));
		buflen = 0;
		if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&buflen))==NDK_OK)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:IC�ϵ�Ӧʧ��(%d)", __LINE__, i+1, ret);
			return;
		}

		//����忨�µ磬�������µ��Ƿ�ɹ�
		NDK_IccPowerDown(ICTYPE_IC);	
		cls_show_msg("�뽫���γ�,�����������..");
		if((ret=NDK_IccDetect(&nSta))!=NDK_OK||(nSta&0xff)!=0x00)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:IC��δ�γ�(%d)", __LINE__, i+1, ret);
			return;
		}
		NDK_IccPowerDown(ICTYPE_IC);	
		if((ret=NDK_IccDetect(&nSta))!=NDK_OK||(nSta&0xff)!=0x00)
		{	
			cls_show_msg1(g_keeptime, "line %d:��%d��:IC��δ�γ�(%d)", __LINE__, i+1, ret);
			return;
		}
	}
	
	//�����쳣������������MEMORY���ϵ���µ� ����IC��Ӧ��Ҫ���� 20140901 linwl	
	memset(buf, 0, sizeof(buf));
	if((ret=NDK_IccPowerUp(ICTYPE_M_1, buf, &pnAtrlen))==NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:MEMORY���ϵ�Ӧ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	cls_show_msg("�����IC��,�����������..");
	if ((ret=NDK_IccPowerUp(ICTYPE_IC,buf,&buflen))!=NDK_OK)
	{	
		cls_show_msg1(g_keeptime, "line %d:IC�ϵ�Ӧʧ��(%d)", __LINE__, ret);
		return;
	}
	NDK_IccPowerDown(ICTYPE_IC);
	NDK_IccPowerDown(ICTYPE_M_1);

	cls_show_msg("�쳣�忨����ͨ��");
	return;
}

void IC_abnormal2(void)
{
#if !K21_ENABLE||defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME30GX||defined ME32GX||defined ME32THM||ANDIROD_ENABLE||defined ME15B//||defined N900||defined IM81
	int i = 0, rlen = 0, pnAtrlen = 0, ret = 0;
	uchar buf[256] = {0};
	uchar recvbuf[256] = {0};
	EM_ICTYPE MEMTYPE[] = {ICTYPE_M_1,ICTYPE_M_1_1,ICTYPE_M_1_64, ICTYPE_M_2, ICTYPE_M_3, ICTYPE_M_3, ICTYPE_M_4, ICTYPE_M_5, ICTYPE_M_6,ICTYPE_M_7};
	char MEM_TYPE[][16] = {"at24c32", "at24c01", "at24c64", "sle44x2", "sle44x8", "sle5528", "at88sc102", "at88sc1604", "at88sc1608","at88sc153"};
	ushort sw = 0;
#endif

	/*process body*/
#if !K21_ENABLE||defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME30GX||defined ME32GX||defined ME32THM||ANDIROD_ENABLE||defined ME15B//||defined N900||defined IM81//K21ƽ̨��֧��MEMORY�� IM81�Ѿ�֧��Memory��
	//case1:���Բ���Ŀ�ΪMEMORY�����ϵ����IC�����Ͳ�һ�£�Ӧ�÷���ʧ��
	for(i=0;i<SZ_ARRAY(MEMTYPE);i++)
	{
		cls_show_msg("��ȷ��%s�Ѳ���,���������...", MEM_TYPE[i]);//ʵ�ʲ������MEM������IC������Ӧ��ʧ��
		memset(buf, 0, sizeof(buf));
		if((ret=NDK_IccPowerUp(ICTYPE_IC, buf, &pnAtrlen))!=NDK_ERR_IOCTL/*NDK_ERR_OPEN_DEV*/)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(%s:%d)", __LINE__, MEM_TYPE[i], ret);
			NDK_IccPowerDown(ICTYPE_IC);
			return;
		}
		if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK/*NDK_ERR_OPEN_DEV*/)//����ʲô�����ͣ��µ綼�ɹ�
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(%s:%d)", __LINE__, MEM_TYPE[i], ret);
			NDK_IccPowerDown(ICTYPE_IC);
			return;
		}
	}

	//case2:����IC������MEMORY���ϵ����:�����޷��жϲ���Ŀ����ͺ��ϵ����Ͳ�һ�µ�������ϵ���Գɹ�Ҳ����ʧ�ܣ��������д����ʱ���쳣
	cls_show_msg("��ȷ���Ѳ���IC��,���������...");
	for(i=0;i<SZ_ARRAY(MEMTYPE);i++)
	{
		memset(buf, 0, sizeof(buf));
		if((ret=NDK_IccPowerUp(MEMTYPE[i], buf, &pnAtrlen))!=NDK_OK/*NDK_ERR_OPEN_DEV*/)
			;
		else//�ϵ�ɹ�����£���дӦ��ʧ��
		{
			if((ret=NDK_Iccrw(ICTYPE_M_1, 9,(uchar *)"\x00\xD0\x00\x01\x04\x31\x32\x33\x34",&rlen,recvbuf))==NDK_OK||(sw=SMART_GETSW(recvbuf+rlen-2))!=0x6581)//������ȷ���ж��Ƿ���NDK_ERR_PARA �Ͷ˷���NDK_ERR_IOCTL Ϊ��ͳһ�ĳ��������NDK_OK����20150413 modify		
			{
			
				cls_show_msg1(g_keeptime, "line %d:����ʧ��(%d,0x%04x)", __LINE__, ret, sw);
				NDK_IccPowerDown(MEMTYPE[i]);
				return;
			}
		}
		
		if((ret=NDK_IccPowerDown(MEMTYPE[i]))!=NDK_OK/*NDK_ERR_OPEN_DEV*/)//����ʲô�����ͣ��µ綼�ɹ����µ緵�س����������ȶ�Ӧ�ܽ����� Ȼ���ٶ�ȡ�Ƿ������� ���û���� �ж��µ�ʧ��
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(%s:%d)", __LINE__, MEM_TYPE[i], ret);
			NDK_IccPowerDown(MEMTYPE[i]);
			return;
		}
	}

	//case3:�쳣��������������IC�����µ����Ӧ�óɹ�
	if((ret=NDK_IccPowerUp(ICTYPE_IC, buf, &pnAtrlen))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(IC��:%d)", __LINE__, ret);
		NDK_IccPowerDown(ICTYPE_IC);
		return;
	}
	if((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:����ʧ��(IC��:%d)", __LINE__, ret);
		NDK_IccPowerDown(ICTYPE_IC);
		return;
	}
	//case4:�쳣����������MEMORY�����µ����Ӧ�óɹ�
	cls_show_msg1(2, "������MEMORY������,���������...");
	for(i=0;i<SZ_ARRAY(MEMTYPE);i++)
	{
		cls_show_msg("��ȷ��%s�Ѳ���,���������...", MEM_TYPE[i]);
		memset(buf, 0, sizeof(buf));
		if((ret=NDK_IccPowerUp(MEMTYPE[i], buf, &pnAtrlen))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(%s:%d)", __LINE__, MEM_TYPE[i], ret);
			NDK_IccPowerDown(MEMTYPE[i]);
			return;
		}
		if((ret=NDK_IccPowerDown(MEMTYPE[i]))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:����ʧ��(%s:%d)", __LINE__, MEM_TYPE[i], ret);
			NDK_IccPowerDown(MEMTYPE[i]);
			return;
		}
	}
#else
	cls_show_msg("��ȷ�ϱ���Ʒ�Ƿ�֧��MEMORY��,�����֧����������Ա�����");
	return;
#endif
	
	cls_show_msg("���Ƭ����ͨ��");
	return;
}

void IC_abnormal(void)
{
	int keyin = '0';
	while(1)
    {
		keyin = cls_show_msg("1.�쳣�忨����\n"
							 "2.���Ƭ����\n");
		switch(keyin)
		{
		case '1':
			IC_abnormal1();
			break;
		case '2':
			IC_abnormal2();
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

void systest5(void)
{
	/*private & local definition*/
	int keyin = '0';
	
	/*process body*/
	//cls_show_msg("%s������...���������IC/SAM��,���������ʼ\n", TESTITEM);
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s������...���������IC/SAM��,���������ʼ", TESTITEM);
	
	while(1)
    {
       	if(auto_flag==1)  //�ж�ͳһģ�����Ƿ���������ѹ�����ܲ���
		{
			if( ++keyin == '4' ) /*�˴�ֻ���ۺ�ѹ����дѹ�������ܲ��ԣ��쳣����Ҫ�ֹ�����*/
			{
				cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����ѹ�����Ժ����ܲ��Խ���\n",TESTITEM);
				return ;
			}
			if(cls_show_msg1(5, "�����������ۺ�ѹ���Ͷ�дѹ�����Ժ����ܲ���,��Ҫ�˳��밴ESC")==ESC)
				return;
		}
		else
		{
			keyin = cls_show_msg("1.IC/SAM�ۺ�ѹ��\n"
								"2.IC/SAM��дѹ��\n"
								"3.IC/SAM���ܲ���\n"
								" 4.�쳣");  //0.����
		}
		switch(keyin) 
		{
		case '1':// 1.һ��ǿ�ȵ�ѹ������;
			cls_show_msg1(2, "IC/SAM�ۺ�ѹ������...");
			IC_SAM_press();
			break;
		case '2':
			if(!g_SequencePressFlag)  
			{	
				if(ENTER!=cls_show_msg("IC/SAM��дר��ѹ������(������Բ�����ESC�жϲ���),ENTER������..."))
					break;
			}
			IC_SAM_rw_press();
			break;
		case '3':// 2.���ܲ���
			cls_show_msg1(2, "IC/SAM���ܲ���...");
			IC_SAM_ability();
			break;
		case '4':
			IC_abnormal();
			break;
		case '0':
			//conf_icsam_pps();
			break;
		case ESC:
			cls_show_msg("��ػ�������IC/SAM�����ò�ͬPPS���²���!���������...");
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	//test over
	return;
}


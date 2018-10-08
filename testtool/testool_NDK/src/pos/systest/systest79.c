/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: NFC/��ӡ����
* file name		: systest79.c
* Author 			: sunwx
* version			: V1.0
* DATE			: 20141008
* directory 		: 
* description		: ��ӡ/NFC����
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20130221  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"��ӡ/NFC����"

#define	DEFAULT_CNT_STR	"10"//"100"
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)
#define	MAXWAITTIME	(180)	//30->180 for ���

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
*						sunwx		2014/10/08     created
*****************************************************************/
static void nfc_print(void)
{
	/*private & local definition*/

	int ret = 0, ret1 = 0, succ = 0, cnt = 0, bak = 0, pnRecvlen = 0, nSeekCnt = 3;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint unLen=0;
	uchar psRecebuf[64] = {0};
	
	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr, 1,sizeof(CntStr)-1,&unLen, INPUTDISP_OTHER, 0,INPUT_CONTRL_LIMIT))==ESC||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	
	cls_show_msg("��ȷ���Ѵ��ֻ�NFC���Թ��߲�����pos,���������...");
	
	//if(NDK_PrnInit(g_PrnSwitch)!=NDK_OK)//if (FAIL==clrprintbuf())
	if(lib_initprn(g_PrnSwitch)!=NDK_OK)//��ʼ����ʹ������Ч
	{
		cls_show_msg("%s, line %d:��ӡ����λʧ��", __FILE__, __LINE__);
		return;
	}
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//�������������ĸ���
	{
		cls_show_msg("line %d:��ӡ��״̬�쳣!", __LINE__);
		return;                         
	}
	
	while(cnt)
	{
		if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�",TESTITEM, cnt, succ))
			break;
		
		print_bill();
		//ȡ��ӡ״̬��nfc����
		if ((ret=GetPrinterStatus(MAXWAITTIME))!=SUCC||(ret1=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cnt--;
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(0x%x,%d)",__LINE__,bak-cnt,TESTITEM,ret,ret1);
			continue;
		}
			
			succ++;
			cnt--;
	}
		
	cls_show_msg("%s���,��ִ�д���Ϊ%d,�ɹ�%d��",TESTITEM, bak-cnt, succ);
	return;
}

void systest79(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	
	int ret = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	while(1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"9.��ӡ����");
		
		switch(ret)
		{
		case '1':
			nfc_print();
			break;
		case '9':
			conf_prn();
			break;			
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}


/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: SMART/��ӡ����
* file name		: 
* Author 			: chensj
* version			: 
* DATE			: 2013/06/18
* directory 		: 
* description		: SD/��ӡ����
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20030618
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"(SD/U��)/��ӡ����"

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
*					chensj		2013/06/18			created
*****************************************************************/
static void sdcard_print(int disktype)
{
	/*private & local definition*/
	
	int ret = 0, succ = 0, cnt = 0, bak = 0;
	char CntStr[8] = DEFAULT_CNT_STR,msg[32] = {0};
	uint unLen=0;
	
	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 5,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		bak  = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	
	cls_show_msg1(g_keeptime, "��ȷ���Ѱ�װ%s��,���������...", (disktype==UDISK)?"U��":"SD");	
	sprintf(msg, "%s/��ӡ����", (disktype==UDISK)?"U��":"SD");

	if(lib_initprn(g_PrnSwitch)!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"%s, line %d:��ӡ����ʼ��ʧ��",__FILE__,__LINE__);
		return;
	}
	while(cnt)
	{
		//����
		NDK_DiskClose(disktype);		
		if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��),��ESC�˳�", msg, cnt, succ))
			break;	
		cnt--;
		
		//ȡ��ӡ״̬
		print_bill();
		if ((ret=GetPrinterStatus(MAXWAITTIME))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sʧ��(0x%x)", __LINE__, bak-cnt, msg, ret);
			continue;
		}		
		//SD��
		if((ret=systest_SDCard(disktype))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, msg, ret);
			continue;
		}
		succ++;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"%s���,��ִ�д���Ϊ%d,�ɹ�%d��", msg, bak-cnt, succ);
	return;
}

void systest42(void)
{
	/*private & local definition*/	
	int ret = 0, disktype = SDDISK;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	if(auto_flag==1)
	{
		//�Զ�����������
		//�Զ����ò�����ʧ�����˳�
		if(conf_disktype()!=SUCC)
		{
			cls_show_msg_record(FILENAME ,FUNCNAME ,"%s����,SD/U������ʧ��,���������ļ�",TESTITEM);
			return;
		}
		disktype=get_disktype();
		//��ӡ����
		conf_prn();
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н���ѹ��
		sdcard_print(disktype);
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"0.SD/U������\n"
							"9.��ӡ����");
	
		switch(ret)
		{
		case '1':
			sdcard_print(disktype);
			break;
		case '0':
			if((ret=conf_disktype())!=SUCC)
			{
				cls_show_msg1(2, "�豸ѡ��ʧ��,������ѡ��", ret);
				return;
			}
			disktype=get_disktype();
			break;
		case '9':
			conf_prn();
			break;			
		case ESC:
			cls_show_msg("��ѡ������SD/USB���²���!");
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}


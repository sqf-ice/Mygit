/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ӡ/ȡ��ص�������
* file name		: systest43.c
* Author 			: linwl
* version			: V1.0
* DATE			: 20130707
* directory 		: 
* description		: ��ӡ/ȡ��ص�������
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
#define	TESTITEM	"��ӡ/ȡ��ص�������"

#define	DEFAULT_CNT_STR	"10"//"300"
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
*					linwl			2013/02/21     created
*****************************************************************/
static void print_getpowervol(void)
{
	/*private & local definition*/
	int ret = 0, succ = 0, cnt = 0, bak = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uint unLen = 0;
	unsigned int unVol = 300;
	
	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr, 1,sizeof(CntStr)-1,&unLen, INPUTDISP_OTHER, 5,INPUT_CONTRL_LIMIT))==ESC||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	cls_show_msg_record(FILENAME ,FUNCNAME ,"������ֳ��豸��ȷ��ֻͨ����ع���,��������ӵ�Դ,û�����Ӵ�����USB�ߵ�,���������...");
	
	//if(NDK_PrnInit(g_PrnSwitch)!=NDK_OK)
	if(lib_initprn(g_PrnSwitch)!=NDK_OK)//��ʼ����ʹ֮ǰ��������Чmodify 20131122 jiangym
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��ӡ����λʧ��", __LINE__);
		return;
	}
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//�������������ĸ���
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��ӡ��״̬�쳣", __LINE__);
		return;                         
	}
	if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK )
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:%sʧ��(ret=%d,vol=%d)", __LINE__, TESTITEM, ret, unVol);
		return;
	}
	while(cnt)
	{
		if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��)(��ص���:%d),��ESC�˳�", TESTITEM, cnt, succ, unVol))
			break;	
		cnt--;
		
		//ȡ��ӡ״̬
		print_bill();
		//ȡ��ص���
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK )
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sʧ��(ret=%d,vol=%d)", __LINE__, bak-cnt, TESTITEM, ret, unVol);
			continue;
		}
		if ((ret=GetPrinterStatus(MAXWAITTIME))!=SUCC)
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sʧ��(%d)", __LINE__, bak-cnt, TESTITEM, ret);
			continue;
		}		
		//ȡ��ص���
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK )
		{
			cls_show_msg1_record(FILENAME ,FUNCNAME ,g_keeptime, "line %d:��%d��%sʧ��(ret=%d,vol=%d)", __LINE__, bak-cnt, TESTITEM, ret, unVol);
			continue;
		}
		succ++;
	}
	cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���,��ִ�д���Ϊ%d,�ɹ�%d��", TESTITEM, bak-cnt, succ);
	return;
}

void systest43(void)
{
	/*private & local definition*/
	int ret = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	if(auto_flag==1)
	{
		//�Զ�����������		
		//��ӡ����
		conf_prn();
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н���ѹ��
		print_getpowervol();

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		ret = cls_show_msg("1.����ѹ��\n"
							"9.��ӡ����");
		
		switch(ret)
		{
		case '1':
			print_getpowervol();
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


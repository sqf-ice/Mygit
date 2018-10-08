/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: 	NFC/ȡ��ص���
* file name			: systest81.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: NFC/ȡ��ص�������
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20131202  chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"NFC/ȡ��ص���"
#define	DEFAULT_CNT_STR	"10"
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)

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
*					 huangjianb		20141013	created
*****************************************************************/	
void systest81(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	int ret = 0, bak = 0,cnt = 0, succ = 0, nSeekCnt = 3 ,pnRecvlen = 0;
	uint unLen = 0;
	char  CntStr[8] = DEFAULT_CNT_STR;
	unsigned int unVol = 300;
	uchar psRecebuf[64] = {0};

	/*process body*/
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	
	cls_show_msg("�ֳ��豸��ȷ��ֻͨ����ع���,��������ӵ�Դ,û�����Ӵ�����USB�ߵ�,�����ֻ�NFC���Թ��߲�����pos���������...");
	if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK )
	{
		cls_show_msg1(g_keeptime, "line %d:%sʧ��(ret=%d,vol=%d)", __LINE__, TESTITEM, ret, unVol);
		return;
	}
	
	while(cnt)
	{
		if(ESC==cls_show_msg1(2, "%s��...\n��ʣ%d��(�ѳɹ�%d��)(��ص���:%d),��ESC�˳�", TESTITEM, cnt, succ, unVol))
			break;	
		cnt--;

		//NFC
		//����NFC����֮����Ҫ�и�2S������ʱ�䣬�����ֻ�����������Ӧ
		sleep(2);
		if((ret=Nfc_Activate(psRecebuf,&pnRecvlen,nSeekCnt)) != NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:NFC����ʧ��(%d)", __LINE__, ret);
			continue;
		}

		//ȡ��ص���
		if((ret=NDK_SysGetPowerVol(&unVol))!=NDK_OK )
		{
			cls_show_msg1(g_keeptime, "line %d:��%d��%sʧ��(ret=%d,vol=%d)", __LINE__, bak-cnt, TESTITEM, ret, unVol);
			continue;
		}
		succ++;
	}
	cls_show_msg("%s���,��ִ�д���Ϊ%d,�ɹ�%d��", TESTITEM, bak-cnt, succ);
	return;
}


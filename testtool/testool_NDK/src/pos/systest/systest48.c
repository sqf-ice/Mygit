/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �ſ�/���̽���
* file name			: systest48.c
* Author 			: 	chensj
* version			: 
* DATE				: 
* directory 			: 
* description			:  �ſ�/���̽���
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
#define	TESTITEM	"�ſ�/���̽���"
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
					 chensj 			2013/08/22     created
*****************************************************************/
void systest48(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	int ret = 0, bak = 0,cnt = 0, nErrorCode = 0, succ = 0;
	char szBuf[201] = {0}, CntStr[8] = DEFAULT_CNT_STR, pszTk1[MAXTRACKLEN] = {0}, pszTk2[MAXTRACKLEN] = {0}, pszTk3[MAXTRACKLEN] = {0};
	uint unLen = 0;
	uchar cSwiped = 0;
		
	cls_printf("Ĭ�Ͻ�����Դ���:\n");
	if ((ret=NDK_KbGetInput(CntStr,1, sizeof(CntStr)-1,&unLen, 2, 0,INPUT_CONTRL_LIMIT_ERETURN))==NDK_ERR||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
		cnt = DEFAULT_CNT_VLE;
	else
		cnt = atoi(CntStr);
	
	//ģ��Ӧ�����俨�Ų���,���Դ򿪴ſ��豸���������Ƿ�Ῠ��,���������
	if((ret=NDK_MagOpen())!=NDK_OK)
	{
		cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
		return;
	}

	while(1)
	{
		if(ESC==cls_show_msg1(2, "���ڽ���%d��%s����,�ɹ�%d��,ESC�˳�", bak+1, TESTITEM, succ))
			break;
		if(bak++==cnt)
			break;
		//ˢ������
		NDK_MagReset();//����Ҷ��ͳ�ṩ�������ڶ���֮��Ҫ��λ��ͷ������ſ����������� 20140304 linwl
		memset(pszTk1,0,sizeof(pszTk1));
		memset(pszTk2,0,sizeof(pszTk2));
		memset(pszTk3,0,sizeof(pszTk3));
#if !(defined ME30MH||defined ME32)	
		cls_show_msg("��ˢ�ſ�,���������");
		if((ret=NDK_MagSwiped(&cSwiped))!=NDK_OK||cSwiped!=1)
		{
			cls_show_msg("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTITEM, ret, cSwiped);
			continue;
		}
#else
		cls_printf("��ˢ�ſ�");
		while(1)
		{
			NDK_MagSwiped(&cSwiped);
			if(cSwiped==1)
				break;
		}
#endif
		if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &nErrorCode))!=NDK_OK||nErrorCode!=0)
		{
			cls_show_msg("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTITEM, ret, nErrorCode);
			continue;
		} 
#if !TK1_DISABLE		
		//��ʾ�ŵ���Ϣ
		if((ret=check_stripe_result_tk1(pszTk1))!=0x00)
		{
			cls_show_msg("line %d:%s����ʧ��(0x%x)", __LINE__, TESTITEM, ret);
			continue;
		}
#endif
		if((ret=check_stripe_result_tk2(pszTk2))!=0x00)
		{
			cls_show_msg("line %d:%s����ʧ��(0x%x)", __LINE__, TESTITEM, ret);
			continue;
		}
		if((ret=check_stripe_result_tk3(pszTk3))!=0x00)
		{
			cls_show_msg("line %d:%s����ʧ��(0x%x)", __LINE__, TESTITEM, ret);
			continue;
		}
		
		//�����������//��������ʱ��������Ӧ���ܹ���ȷʶ������İ���,������ͣ��,������ʾ,�������쳣����
		cls_printf("���������:");
		if((ret=NDK_KbGetInput(szBuf,20, 200, &unLen,INPUTDISP_NORMAL, 0,INPUT_CONTRL_NOLIMIT_ERETURN))!=NDK_OK)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			continue;
		}
		if(cls_show_msg("�Ƿ����ֿ�������\nȷ��:��  ����:��")==ENTER)
		{
			cls_show_msg("line %d:%s����ʧ��(%d)", __LINE__, TESTITEM, ret);
			continue;
		}
		succ++;
	}
	NDK_MagClose();
	cls_show_msg("%s�������,�ܹ�����%d�γɹ�%d��\n", TESTITEM, bak, succ);
	return;
}

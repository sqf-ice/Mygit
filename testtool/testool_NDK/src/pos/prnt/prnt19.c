/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt19.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnFontRegister,NDK_PrnSetUsrFont�ӿڹ���
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_PrnFontRegister,NDK_PrnSetUsrFont"	
#define MAXWAITTIME 70
#define HANZI12 "hzk12"
#define ZIMU12 "asc12"
#define NOTEXIST "hzk90" 

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
static int func12(char m[], ST_PRN_RECMSG *p)
{
	p->nOffset = (94*((m[0]-0xa0)-1) +(m[1]-0xa0)-1)*24;
	p->nFontByte = 24;
	return 1;
}

static int funcasc12(char m[], ST_PRN_RECMSG *p)
{
	p->nOffset = (m[0]-32)*12;
	p->nFontByte = 12;
	return 1;
}

/****************************************************************
* function name 	 				:void prnt19(void)
* functional description 	:ע�������ӡ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt19(void)
{
	//����Ϊ�ֲ�����
	ST_PRN_FONTMSG stMsg,stMsg2,stMsg3;
	int nRet = 0, nRet1 = 0;
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	/*if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)*/
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;	
	}

	if(cls_show_msg("��ȷ����ǰ·���´���ASC12��HZK12�ֿ�,[ȷ��]����[����]����")==ENTER)
	{
		if((nRet = NDK_FsExist(HANZI12))!=NDK_OK||(nRet1=NDK_FsExist(ZIMU12))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d%d)",__LINE__,TESTAPI,nRet,nRet1);
			goto prnt_error;
		}
	}
	else
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;
	}

	while(1)
	{
		if((nRet=NDK_PrnGetStatus(&emStatus))!=NDK_OK )
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI,nRet);
			goto prnt_error;
		}
		if(emStatus != PRN_STATUS_BUSY)
			show_stopwatch(DISABLE, 0);
		
		switch(emStatus)
		{
		case PRN_STATUS_BUSY:
			if(nBusy==FALSE)
			{
				nOldtime = time(NULL);//�ӵ�һ���յ�æ״̬��ʼ��ʱ
				nBusy = TRUE;
			}
			if((nDiff=time(NULL)-nOldtime)>MAXWAITTIME)
			{
				send_result("line %d:��ӡ������æ״̬ʱ�����", __LINE__);
				goto prnt_error;
			}
			show_stopwatch(ENABLE, nDiff);
			continue;
			break;

		case PRN_STATUS_OK:
			nBusy=FALSE;	

			//case1:ע�����Ϊ��
			if((nRet = NDK_PrnFontRegister(NULL))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}	
			
			//case2: �ֿ������ֿ�����·�� Ϊ��
			memset( &stMsg, 0, sizeof(ST_PRN_FONTMSG));
			stMsg.nNum = 0;
			stMsg.w = 12;
			stMsg.h = 12;
			stMsg.nDirection = 0;
			stMsg.nIsHZ = 1;
			stMsg.pszName = NULL;
			stMsg.func=func12;
			if((nRet = NDK_PrnFontRegister(&stMsg))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}	
			
			//case3:�ֿ������ֿ�����·�� ��Ϊ�յ��ǲ�����
			memset( &stMsg, 0, sizeof(ST_PRN_FONTMSG));
			stMsg.nNum = 0;
			stMsg.w = 12;
			stMsg.h = 12;
			stMsg.nDirection = 0;
			stMsg.nIsHZ = 1;
			stMsg.pszName = NOTEXIST;
			stMsg.func=func12;
			if((nRet = NDK_PrnFontRegister(&stMsg))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}
			
			//case4:ע��ŷ�0-7
			memset( &stMsg, 0, sizeof(ST_PRN_FONTMSG));
			stMsg.nNum =16;
			stMsg.w = 12;
			stMsg.h = 12;
			stMsg.nDirection = 0;
			stMsg.nIsHZ = 1;
			stMsg.pszName = HANZI12;
			stMsg.func=func12;
			if((nRet = NDK_PrnFontRegister(&stMsg))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}
			
			//case5:Direction ��0-1
			memset( &stMsg, 0, sizeof(ST_PRN_FONTMSG));
			stMsg.nNum = 0;
			stMsg.w = 12;
			stMsg.h = 12;
			stMsg.nDirection = 8;
			stMsg.nIsHZ = 1;
			stMsg.pszName = HANZI12;
			stMsg.func=func12;
			if((nRet = NDK_PrnFontRegister(&stMsg))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}
			
			//case6:��ȡ���Ƶ�ַ��ռ���ֽ����ĺ���Ϊ��
			memset( &stMsg, 0, sizeof(ST_PRN_FONTMSG));
			stMsg.nNum = 0;
			stMsg.w = 12;
			stMsg.h = 12;
			stMsg.nDirection = 0;
			stMsg.nIsHZ = 1;
			stMsg.pszName = HANZI12;
			stMsg.func=NULL;
			if((nRet = NDK_PrnFontRegister(&stMsg))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}
			
			//case7:ע����ֿ��Ǻ����ֿ⻹��ASCII�ֿ⣬1:HZ 0:ZM    ��0-1
			memset( &stMsg, 0, sizeof(ST_PRN_FONTMSG));
			stMsg.nNum = 0;
			stMsg.w = 12;
			stMsg.h = 12;
			stMsg.nDirection = 0;
			stMsg.nIsHZ = 8;
			stMsg.pszName = HANZI12;
			stMsg.func=func12;
			if((nRet = NDK_PrnFontRegister(&stMsg))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}

			//case8: ����ע���ֿ�stMsg2,stMsg3
			//case8.1:
			memset( &stMsg2, 0, sizeof(ST_PRN_FONTMSG));
			stMsg2.nNum = 0;
			stMsg2.w = 12;
			stMsg2.h = 12;
			stMsg2.nDirection = 0;
			stMsg2.nIsHZ = 1;
			stMsg2.pszName = HANZI12;
			stMsg2.func=func12;
			if((nRet = NDK_PrnFontRegister(&stMsg2))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}
			
			//case8.2:
			memset( &stMsg3, 0, sizeof(ST_PRN_FONTMSG));
			stMsg3.nNum = 1;
			stMsg3.w = 8;
			stMsg3.h = 12;
			stMsg3.nDirection = 0;
			stMsg3.nIsHZ = 0;
			stMsg3.pszName = ZIMU12;
			stMsg3.func=funcasc12;
			if((nRet = NDK_PrnFontRegister(&stMsg3))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;	
			}

			//case8.3:0,1����,��֤�ظ�ע��
			stMsg2.nNum = 1;//hz
			stMsg3.nNum = 0;//zm
			if((nRet=NDK_PrnFontRegister(&stMsg2))!=NDK_OK||(nRet1=NDK_PrnFontRegister(&stMsg3))!=NDK_OK)
			{
				send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI, nRet, nRet1);
				goto prnt_error;	
			}
		
			//case9:��֤"�����ú�Ḳ��NDK_PrnSetFont���趨������"
			//������ϵͳ���岢��ӡ����ӡ��Ϣ
			//�������û����岢��ӡ
			NDK_PrnSetFont(PRN_HZ_FONT_24x24C,PRN_ZM_FONT_16x32);
			NDK_PrnStr("ϵͳ����:[24x24C]����;ϵͳ����:[16x32]Zm\n");			
			NDK_PrnSetUsrFont(0);
			NDK_PrnStr("ϵͳ����:[24x24C]����;�û�����:[12x8]Zm\n");
			NDK_PrnSetUsrFont(1);
			NDK_PrnStr("�û�����:[12x12]����;�û�����:[12x8]Zm\n");
			NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x8);
			NDK_PrnStr("ϵͳ����:[16x16]����;ϵͳ����:[8x8]Zm\n");	
			NDK_PrnStart();

			nKeyin = cls_show_msg1(MAXWAITTIME, "����ӡЧ��(�Ƿ����Ԥ�������ӡ),[ȷ��]��,[����]û��");
			if (nKeyin==ENTER||nKeyin==0)
				send_result("%s����ͨ��", TESTAPI);
			else
				send_result("%s����ʧ��", TESTAPI);
			goto prnt_error;
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
				goto prnt_error;
			}
			NDK_PrnStart();
			cls_printf("����%s...", TESTAPI);
			break;
		case PRN_STATUS_OVERHEAT:
			send_result("line %d: ��ӡ������", __LINE__);
			goto prnt_error;
			break;
		case PRN_STATUS_VOLERR:
			send_result("line %d: ��ӡ����Դ����", __LINE__);
			goto prnt_error;
			break;
		case PRN_STATUS_DESTROYED:
			send_result("line %d: ��ӡ����", __LINE__);
			goto prnt_error;
			break;
		default:
			//δ֪״̬����Ϊ��
			send_result("line %d: UNKNOWN STATE(tpstatus=%d)", __LINE__, emStatus);
			goto prnt_error;
			break;
		}
		
	}

	//���Խ���	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);
	return;
}


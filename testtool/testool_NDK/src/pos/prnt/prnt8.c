/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt8.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnSetFont�ӿڹ���
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
#define TESTAPI	"NDK_PrnSetFont"	
#define MAXWAITTIME 180

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt8(void)
* functional description 	:���ô�ӡ����
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt8(void)
{
	//����Ϊ�ֲ�����
#include	"dotfont.h"
#include	"hzfont.h"

	int nRet = 0;
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,i=-2,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int j=0;
	int nRet1=0;

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	/*if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)*/
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;	
	}
	
	while(1)
	{
		if((nRet=NDK_PrnGetStatus(&emStatus))!=NDK_OK )
		{
			send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
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
			switch(i)
			{
			case -2:
				if ((nRet = NDK_PrnSetFont(-1,-1))!= NDK_ERR_PARA||(nRet1=NDK_PrnSetFont(100,100))!=NDK_ERR_PARA)
				{
					send_result("line %d:%s����ʧ��(%d,%d)", __LINE__,nRet,nRet1);
					goto prnt_error;//return;
				}
				i++;
				break;
			case -1:
				NDK_PrnStr("[Ĭ��,Ĭ��]045NewLand�´�½\n\n");

				if((nRet =NDK_PrnSetFont(PRN_HZ_FONT_32x32,0)) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__,nRet);
					goto prnt_error;//return;
				}				
				NDK_PrnStr("[HZ24x24,Ĭ��]045NewLand�´�½\n\n");			
				if((nRet =NDK_PrnSetFont(0,PRN_ZM_FONT_24x32)) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__,nRet);
					goto prnt_error;//return;
				}
				NDK_PrnStr("[HZ24x24,ZM24x32]045NewLand�´�½\n\n");
				
				if (cls_show_msg("����ӡЧ��,��ENTERͨ��,��������ͨ��")!=ENTER)
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);

				i++;
				break;				
			default:		
				if(j==0)
					print_Ex("---��%s��ʽ�����%d��HZ������д�ӡ(ESC���������ִ�ӡ)---\n", dotfontstr[i], SZ_ARRAY(hzfont));

				if(j<SZ_ARRAY(hzfont))
				{
					if(NDK_PrnSetFont(hzfont[j],dotfont[i]) != NDK_OK)
					{
						send_result("line %d:���ô�ӡ����[%s,%s]ʧ��!", __LINE__, dotfontstr[i], hzfontstr[j]);
						goto prnt_error;//return;
					}
					print_Ex("��[%s,%s]��ʽ��ӡ\"045NewLand$$�´�½��������ҷ���\xA3\xA4\xA3\xA4\"\n", hzfontstr[j], dotfontstr[i]);
					NDK_PrnStr("045NewLand�´�½\n\n");
					
					if(wait_anykey(1)==ESC)//if (lib_kbhit()==ESC)
					{
						j = 0;
						i++;
					}
					else
						j++;
				}
				else
				{
					j = 0;
					i++;
				}
				break;	
			case SZ_ARRAY(dotfont):	/**<���Խ����ָ�Ĭ��ֵ*/
				if(NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x16 )!=NDK_OK)
				{
					send_result("line %d:���ô�ӡ����[DOT8x16,HZ16x16]ʧ��!", __LINE__);
					goto prnt_error;//return;
				}
				NDK_PrnStr("NewLand��ӡ��������������[DOT8x16,HZ16x16]");
				
				i++;
				break;
			case SZ_ARRAY(dotfont)+1:
				nKeyin = cls_show_msg1(MAXWAITTIME, "����ӡЧ��,��ENTERͨ��,��������ͨ��");
				if (nKeyin!=ENTER && nKeyin!=0)
					send_result("%s����ʧ��", TESTAPI);
				else
					send_result("%s����ͨ��", TESTAPI);
				goto prnt_error;//return;
				break;
			}		
			NDK_PrnStart();
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
				goto prnt_error;//return;
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
			goto prnt_error;//return;
			break;
		}
	
	}
	
	//���Խ���	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);
	return;
}


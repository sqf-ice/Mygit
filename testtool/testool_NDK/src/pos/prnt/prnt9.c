/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt9.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnSetMode�ӿڹ���
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
#define TESTAPI	"NDK_PrnSetMode"	
#define	MAXWAITTIME	(180)	
#define	NUM_GUO	(36)

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				:void prnt9(void)
* functional description 	:���ô�ӡģʽ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt9(void)
{
	/*private & local definition*/
#include	"hzfont.h"

	//����Ϊ�ֲ�����
	int nRet = 0;
	int i = -1, k = 0, nBusy = FALSE;
	time_t nDiff = 0, nOldtime = 0;
	int nKeyin = 0, nIndex = 0;
	EM_PRN_STATUS emStatus=0;
	int loop = 0,j=0;
	int sErr_args[4][2] = {{-1, 0}, {0, -1}, {0, 5}, {10, 0}};
	int nType = lib_getprintertype();
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	/*if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)*/
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;	
	}

	NDK_PrnSetFont(PRN_HZ_FONT_16x16,PRN_ZM_FONT_8x8);	
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
			switch(i)
			{
			case -1:
				for(; loop<SZ_ARRAY(sErr_args); loop++)
				{
					if((nRet = NDK_PrnSetMode(sErr_args[loop][0],sErr_args[loop][1])) != NDK_ERR_PARA)
					{
						send_result("line %d:���ô�ӡģʽӦʧ��(%d,%d)", __LINE__,nRet,loop);
						goto prnt_error;//return;
					}
				}
				i++;
				break;
			default:
				
				if(j<2)
				{
					if (NDK_PrnSetMode(i, j) != NDK_OK)
					{
						send_result("line %d:���ô�ӡģʽʧ��(%d,%d)", __LINE__,i,j);
						goto prnt_error;//return;
					}
					nIndex = rand()%(SZ_ARRAY(hzfont));
					NDK_PrnSetFont(hzfont[nIndex], 0);
					if((nRet=print_Ex("��[%s,ģʽ%d,%s��]��ʽ��ӡ%d��\"��\"\n", hzfontstr[nIndex], i, j==0?"��":"˫", NUM_GUO))<0)
					{
						send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
						goto prnt_error;//return;
					}
					for (k=1; k<=NUM_GUO; k++)
					{
						if((NDK_PrnStr("��"))!=NDK_OK)
						{
							send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,k);
							goto prnt_error;//return;
						}
					
					}
					NDK_PrnStr("\r");
					if (lib_kbhit()==ESC)
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
			case PRN_MODE_NORMAL +1: //�ָ�Ĭ�ϵ�����
				switch (nType)
				{
				case PRN_TYPE_HIP:
				case PRN_TYPE_FIP:
					if(NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE,1) != NDK_OK)
					{
						send_result("line %d:���ô�ӡģʽ[PRN_MODE_ALL_DOUBLE, 1]ʧ��", __LINE__);
						goto prnt_error;//return;
					}
					NDK_PrnStr("045NewLand�´�½,�ָ�Ĭ�ϴ�ӡģʽ[PRN_MODE_ALL_DOUBLE, 1]");
					break;
				case PRN_TYPE_TP:
					if(NDK_PrnSetMode(PRN_MODE_NORMAL ,0) != NDK_OK)
					{
						send_result("line %d:���ô�ӡģʽ[PRN_MODE_NORMAL, 0]ʧ��", __LINE__);
						goto prnt_error;//return;
					}
					NDK_PrnStr("045NewLand�´�½,�ָ�Ĭ�ϴ�ӡģʽ[PRN_MODE_NORMAL, 0]");
					break;
				default:
					send_result("line %d:δ�ܻ�ȡ����ӡ������", __LINE__);
					goto prnt_error;//return;					
					break;
				}
				i++;
				break;
			case PRN_MODE_NORMAL +2:
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


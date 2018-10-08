/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt21.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnSetAlignment �ӿڹ���
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
#define TESTAPI	"NDK_PrnLoadBDFFont,NDK_PrnBDFStr,NDK_PrnSetBDF"	
#define MAXWAITTIME 60
#define NOTEXIST "BDF12345" 
#define MINXPOS		0
#define MAXXPOS		288
#define MIXLINESPACE 0
#define MAXLINESPACE 255
#define MINWORDSPACE 0
#define MAXWORDSPACE 255
#define PERSIANBDF	"persian.bdf"
#define FARSITEXBDF "farsitex.bdf"
#define BDF1020		"10x20.bdf"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
static int testcase(int subcase)
{
	/*private & local definition*/
	int nRet = SUCC;
	
	/*process body*/
	switch (subcase)
	{
	case 0:
		//case1:�쳣����NULL
		if((nRet = NDK_PrnLoadBDFFont(NULL))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI,nRet);
			return FAIL;
		}
		
		//case2:·�� ��Ϊ�յ��ǲ�����
		if((nRet = NDK_PrnLoadBDFFont(NOTEXIST))!=NDK_ERR_PATH/*NDK_ERR_PARA*/)//20130717zhengdc�ύ�޸ķ���ֵ
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI,nRet);
			return FAIL;
		}		
		
		//case3:��ӡBDF�����е�����Ϊ��
		if((nRet = NDK_PrnBDFStr(NULL))!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI,nRet);
			return FAIL;
		}

		//case4:����BDF ���������쳣����
		if(NDK_PrnSetBDF(MINXPOS-1,MIXLINESPACE,MINWORDSPACE,1,1)!=NDK_ERR_PARA||NDK_PrnSetBDF(MINXPOS,MIXLINESPACE-1,MINWORDSPACE,1,1)!=NDK_ERR_PARA||NDK_PrnSetBDF(MINXPOS,MIXLINESPACE,MINWORDSPACE-1,1,1)!=NDK_ERR_PARA\
			||NDK_PrnSetBDF(MAXXPOS+1,MIXLINESPACE,MINWORDSPACE,1,1)!=NDK_ERR_PARA||NDK_PrnSetBDF(MAXXPOS,MAXLINESPACE+1,MINWORDSPACE,1,1)!=NDK_ERR_PARA||NDK_PrnSetBDF(MAXXPOS,MIXLINESPACE,MAXWORDSPACE+1,1,1)!=NDK_ERR_PARA)
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI);
			return FAIL;
		}	

		//case5:δ���������ȴ�ӡ
		if((nRet = NDK_PrnBDFStr((ushort *)"\xfc\x00\x1c\x20\xa4\x00\xe4\x00"))!=NDK_ERR/*NDK_ERR_PARA*/)//20130717zhengdc�ύ�޸ķ���ֵ
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI, nRet);
			return FAIL;
		}
		break;
	case 1:  //case6:�����ܷ��ظ�ע��,�Ҳ�����
		if((nRet=NDK_PrnLoadBDFFont(PERSIANBDF))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI, nRet);
			return FAIL;
		}
		if((nRet=NDK_PrnLoadBDFFont(BDF1020))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI, nRet);
			return FAIL;
		}
		if((nRet=NDK_PrnBDFStr((ushort *)"\x40\x00\x41\x00\x42\x00\x43\x00\x44\x00\x45\x00"))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI, nRet);
			return FAIL;
		}
		break;	
	case 2: //case7:��������
		if((nRet = NDK_PrnSetBDF(50,30,40,2,2))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI, nRet);
			return FAIL;
		}
		if((nRet=NDK_PrnBDFStr((ushort *)"\x40\x00\x41\x00\x42\x00\x43\x00\x44\x00\x45\x00"))!=NDK_OK)//if(NDK_PrnBDFStr((ushort *)"\xfc\x00\x1c\x20\xa4\x00\xe4\x00")!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI, nRet);
			return FAIL;
		}
		break;
	default:
		return 1;  /* ����1˵�����Խ���*/
	}	
	
	return SUCC;  /* ����0����������ͨ��*/
}

/****************************************************************
* function name 	 				:void prnt21(void)
* functional description 	:���ء��������塢��ӡ
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt21(void)
{
	//����Ϊ�ֲ�����
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int nRet=0,i=0,nRet1=0,nRet2=0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	/*if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)*/
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;	
	}
	
	if(cls_show_msg("��ȷ���ǵ�һ��ִ�б�����,[ȷ��]����[����]����")!=ENTER)
	{
		send_result("line %d:%s�û�ȡ������,���˳����Գ��������½���ִ�б�����",__LINE__,TESTAPI);
		return;//��case5:δ���������ȴ�ӡ����,��Ҫ�ڽ�Ӧ�ó�����һ�β���,���Ӹ���ʾ
	}	
		
	if(cls_show_msg("��ȷ����ǰ·���´���BDF�ֿ�,[ȷ��]����[����]����")==ENTER)
	{
		if((nRet = NDK_FsExist(PERSIANBDF))!=NDK_OK||(nRet1=NDK_FsExist(FARSITEXBDF))!=NDK_OK||(nRet2=NDK_FsExist(BDF1020))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%d,%d)",__LINE__,TESTAPI,nRet,nRet1,nRet2);
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

			if((nRet = testcase(i++))==SUCC) /*chensj modified 2013/06/18 testcase:�����ַ���ֵ0:һ��case�ɹ�,1:����ȫ��ͨ��,-1���ô���*/
			{
				NDK_PrnStart();
				//continue;
			}
			else//����FAIL����1������ж��ǲ�����ɻ��ǲ��Գ���
			{
				if(nRet == FAIL)//���Գ���
				{
					//send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}//������ǲ������
				NDK_PrnStart();
				if ((nKeyin = cls_show_msg("����ӡЧ��,[ȷ��]ͨ��,[����]ʧ��"))!=ENTER)
					send_result("%s����ʧ��", TESTAPI);
				else
					send_result("%s����ͨ��", TESTAPI);
				goto prnt_error;
			}
			
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


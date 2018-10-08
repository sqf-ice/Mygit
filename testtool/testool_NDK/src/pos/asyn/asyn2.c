/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: asynģ��
* file name		: asyn2.c
* Author 			: chenjings
* version			: 
* DATE			: 20130108
* directory 		: src/pos/asyn
* description		: ����NDK_MdmDial �ܷ���н����첽���ӵķ���������asyn����
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmDial"
#define	MAXWAITTIME	60

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	chenjings      20130114	created
*****************************************************************/
void asyn2(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
	EM_MDMSTATUS MdmStatus;	//mdm״̬
	int j = 0, ret = 0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0}, overload[100] = {0};
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); //��Ļ��ʾ��ǰ���ڵĲ�����
	//��ʼ��buf
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;
		
	if((ret=NDK_MdmReset())!=NDK_OK )
	{
		send_result("line %d:%s��λʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		return;
	}
	
	//case1:δ��ʼ������ʧ�����
#if 0 //20141202 reset���������޸�ֱ�ӷ���NDK_OK�����α����� 
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_ERR_MODEM_INIT_NOT)
	{
		send_result("line %d:%sδ��ʼ��������ش���(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif
	//����ʼ��
	if((ret=NDK_MdmAsynInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ����(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�������(�κųɹ�,�����ܽ�������,Ӧ�첽��⵽���ز�)
	if((ret=NDK_MdmDial("123456"))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	lib_StartStopwatch();
	while(1)
	{
		if((ret=NDK_MdmCheck(&MdmStatus))!=NDK_OK)//δ����ȷ��ȡmodem״̬
		{
			send_result("line %d:%s���״̬ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		else if(MdmStatus!=MDMSTATUS_NORETURN_AFTERPREDIAL)// �ж�modem״̬�Ƿ�ı�
		{
			if((MdmStatus==MDMSTATUS_MS_BUSY )){// �ж�modem״̬�Ƿ�Ϊæ��
				NDK_MdmHangup();
				break;
			}
			else {
				send_result("line %d:%sè״̬�쳣(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, MdmStatus);
				goto ERR;
			}
		}
		else if(lib_ReadStopwatch()>MAXWAITTIME)//60 s ��ʱδ��⵽æ��
		{
			send_result("line %d:%sè���״̬��ʱ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, MdmStatus);
			goto ERR;
		}
	}
	
	//case3:���Ų���������ΪNULL
	if((ret=NDK_MdmDial(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����Ӧʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:���Գ���(��������ֽ���47)
	memset(overload, 0x61, sizeof(overload));//20141212���ݾ�ˮ���� MDM�ײ��޸Ĳ��Ż��峤��������󳤶�Ϊ80
	if((ret=NDK_MdmDial(overload))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����Ӧʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case5:���Բ���ʱ������Ӧ��⵽����
	cls_show_msg("�벢��(����绰��ժ��)�����������");
	if((ret=NDK_MdmAsynInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ������(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_ERR_MODEM_OTHERMACHINE)//��ʱ����Ӧ�ò��ɹ�
	{
		send_result("line %d:%s����״̬�²���Ӧʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("����������ͨ��,�뽫�绰�һ������������");
	
#if 0	
	//case6:�ڳɹ��κź󣬽��в������ٽ������ݷ��ͣ�Ӧ��⵽������
	if((ret=NDK_MdmAsynInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ��ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_MdmClrbuf();		
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����(%s)ʧ��(ret=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%sèδ�ܲ�ͨ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	cls_show_msg("�벢��(����绰��ժ��)�����������");
	if((ret=NDK_MdmWrite(buf,sizeof(buf)))!=NDK_ERR_MODEM_OTHERMACHINE)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);			
		GOTOERR;
	}
	cls_show_msg("����������ͨ��,�뽫�绰�һ������������");
	NDK_MdmHangup();
#endif
	
	//case7:����δ��绰�ߵ����	
	cls_show_msg("�����POS�ϵĵ绰�ߺ����������");
	if((ret=NDK_MdmAsynInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ������(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_ERR_MODEM_NOLINE)//��ʱ����Ӧ�ò��ɹ�
	{
		send_result("line %d:%s����״̬�²���Ӧʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case9:��֤�ε��绰��ʱ״̬��ͼ��Ӧ����ʾΪ:δ�ӵ绰�� ͼ�� 20140618  jiangym
	if(cls_show_msg("״̬��ͼ���Ƿ���ʾδ�ӵ绰��ͼ��,��[Enter]����,��[����]ʧ��")!=ENTER)
	{
		send_result("line %d:״̬��ͼ��Ӧ����ʾδ�ӵ绰��", __LINE__);
		GOTOERR;
	}
	//case10:��֤�ε��绰��ʱ���ùҶϺ�����״̬��ͼ��Ӧ����ʾΪ:δ�ӵ绰�� ͼ�� 20140623 jiangym
	NDK_MdmHangup();
	if(cls_show_msg("�ҶϺ��Ƿ���ʾδ�ӵ绰��ͼ��,��[Enter]����,��[����]ʧ��")!=ENTER)
	{
		send_result("line %d:״̬��ͼ��Ӧ����ʾδ�ӵ绰��", __LINE__);
		GOTOERR;
	}
	cls_show_msg("����������ͨ��,����ϵ绰�ߺ����������");
	
	//case8:��ȷ����
	if((ret=NDK_MdmAsynInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ��ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case11:��֤��绰��ʱ�Ҳ��ųɹ�ǰͼ����ʾΪ:�ѽӵ绰�� ����״̬��ͼ����ж�20140618  jiangym ����δ���ò��ź���MDM�޷�ʵʱ����MDM״̬�����Դ˴�״̬��Ȼ��δ����
	if(cls_show_msg("״̬��ͼ���Ƿ���ʾδ�ӵ绰��ͼ��,��[Enter]����,��[����]ʧ��")!=ENTER)
	{
		send_result("line %d:״̬��ͼ��Ӧ����ʾ�ѽӵ绰��", __LINE__);
		GOTOERR;
	}
	NDK_MdmClrbuf();		
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����(%s)ʧ��(ret=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%sèδ�ܲ�ͨ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	//case12:��֤�ε��ɹ���Ҷ�ǰ״̬ͼ����ʾ:����ͨѶͼ�� ����״̬��ͼ����ж�20140618  jiangym
	if(cls_show_msg("״̬��ͼ���Ƿ���ʾ����ͨѶͼ��,��[Enter]����,��[����]ʧ��")!=ENTER)
	{
		send_result("line %d:״̬��ͼ��Ӧ����ʾ����ͨѶ", __LINE__);
		GOTOERR;
	}
#if 0		
	NDK_MdmClrbuf();//�첽MODEM���ź����������շ�ǰ��������øú������첽���ڻ�����������Ӱ����Խ��������ʱ���Ӹú���
#endif
	if((ret=NDK_MdmWrite(buf,sizeof(buf)))!=NDK_OK)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);			
		GOTOERR;
	}
	memset(rbuf, 0, sizeof(rbuf));
	rlen = sizeof(buf)+1;//������ȡ�������ݳ���Ҫ���ڵ���ʵ���ܶ�ȡ�������ݳ��ȡ�
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s������(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((sizeof(buf)!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, sizeof(buf), rlen);
		GOTOERR;
	}
	//case13:��֤�ε��ɹ���Ҷ�ǰ״̬ͼ����ʾ:����ͨѶͼ�� ����״̬��ͼ����ж�20140618  jiangym
	if(cls_show_msg("״̬��ͼ���Ƿ���ʾ����ͨѶͼ��,��[Enter]����,��[����]ʧ��")!=ENTER)
	{
		send_result("line %d:״̬��ͼ��Ӧ��ʾ����ͨѶ", __LINE__);
		GOTOERR;
	}
	if((ret=NDK_MdmHangup())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case14:��֤��绰��ʱ�ҹҶϺ�ͼ����ʾΪ:�ѽӵ绰�� ����״̬��ͼ����ж�20140618  jiangym
	if(cls_show_msg("״̬��ͼ���Ƿ���ʾ�ѽӵ绰��ͼ��,��[Enter]����,��[����]ʧ��")!=ENTER)
	{
		send_result("line %d:״̬��ͼ��Ӧ����ʾ�ѽӵ绰��", __LINE__);
		GOTOERR;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
	goto ERR;
ERR:
	NDK_MdmHangup();
	return;
}


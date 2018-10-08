/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: sdlc11.c
* Author 			: linwl
* version			: 
* DATE			: 20150106
* directory 		: src/pos/sdlc
* description		: ����NDK_MdmAdapt�ܷ������MODEM��������Ӧ
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmAdapt"
#define	MAXWAITTIME	60	//60��
#define	PCKTMAXLEN	350

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
*			  	  	linwl      20150106	created
*****************************************************************/
void sdlc11(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ�ֲ�����
	int j = 0, ret = 0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0}, overload[100/*52*/] = {0};
	//EM_MDMSTATUS MdmStatus;	//mdm״̬
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 
	
	//case1:�쳣��������
	//case1.1:����Ĳ�������MODEM_DIAL_TYPE_NOTӦ�÷���NDK_ERR_PARA
	if((ret=NDK_MdmAdapt(MODEM_DIAL_TYPE_NOT, MDMDialStr))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case1.2:����Ĳ�������MODEM_DIAL_TYPE_ASYN+1Ӧ�÷���NDK_ERR_PARA
	if((ret=NDK_MdmAdapt(MODEM_DIAL_TYPE_ASYN+1, MDMDialStr))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case1.3:���ź���ΪNULLӦ�÷���NDK_ERR_PARA
	if((ret=NDK_MdmAdapt(MODEM_DIAL_TYPE_SDLC, NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case1.4:��������(�������)Ӧ�÷���NDK_ERR_PARA
	memset(overload, 0x61, sizeof(overload));
	if((ret=NDK_MdmAdapt(MODEM_DIAL_TYPE_SDLC, overload))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:��������Ӧʧ���������NDK_ERR_MODEM_SELFADAPTFAIL
	cls_printf("���Դ��������,�����ĵȴ�...");
	if((ret=NDK_MdmAdapt(MODEM_DIAL_TYPE_SDLC, "errno123"))!=NDK_ERR_MODEM_SELFADAPTFAIL)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:����Ӧʱ������Ӧ��⵽����NDK_ERR_MODEM_OTHERMACHINE
	cls_show_msg("�벢��(����绰��ժ��)�����������");
	if((ret=NDK_MdmAdapt(MODEM_DIAL_TYPE_SDLC, MDMDialStr))!=NDK_ERR_MODEM_OTHERMACHINE)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg("����������ͨ��,�뽫�绰�һ������������");
	
	//case4:��������Ӧʱδ��绰�ߵ����Ӧ����NDK_ERR_MODEM_NOLINE
	cls_show_msg("�����POS�ϵĵ绰�ߺ����������");
	if((ret=NDK_MdmAdapt(MODEM_DIAL_TYPE_SDLC, MDMDialStr))!=NDK_ERR_MODEM_NOLINE)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case5:��֤�ε��绰��ʱ״̬��ͼ��Ӧ����ʾΪ:δ�ӵ绰�� ͼ��
	if(cls_show_msg("״̬��ͼ���Ƿ���ʾδ�ӵ绰��ͼ��,��[Enter]����,��[����]ʧ��")!=ENTER)
	{
		send_result("line %d:״̬��ͼ��Ӧ����ʾδ�ӵ绰��", __LINE__);
		RETURN;
	}
	cls_show_msg("����������ͨ��,����ϵ绰�ߺ����������");
	
	//case6:������Ӧ�����п��԰�ȡ�����˳�,������NDK_ERR_MODEM_SELFADAPTCANCEL
	cls_printf("���ڿ���״̬��MDMͼ�괦������״̬ʱ������[ȡ��]��");
	if((ret=NDK_MdmAdapt(MODEM_DIAL_TYPE_SDLC, MDMDialStr))!=NDK_ERR_MODEM_SELFADAPTCANCEL)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg1(2,"����������ͨ��,�����������");
	
	//case7:������������Ӧ֮��Ӧ�ò�Ӱ���������ź������շ�
	cls_printf("������ȷ��������Ӧ��,�����ĵȴ�...");
	if((ret=NDK_MdmAdapt(MODEM_DIAL_TYPE_SDLC, MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_MdmSdlcInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ����(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	NDK_MdmClrbuf();
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����(%s)ʧ��(ret=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		RETURN;
	}
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%sδ�ܽ�ͨ(DialNum=%s,MdmStatus=%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		RETURN;
	}
	//��ʼ�����ͻ���������
	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;
	update_SDLC_packet(buf, sizeof(buf)); //���TPDU��ͷ		
	//д
	if((ret=NDK_MdmWrite(buf, PCKTMAXLEN))!=NDK_OK)
	{
		send_result("line %d:%sд����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//��                                     		
	memset(rbuf, 0, sizeof(rbuf));
	rlen = 0;//��ֹSDLC_readû�ж�rlen����0
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//�Ƚ�
	if((PCKTMAXLEN!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, PCKTMAXLEN, rlen);
		RETURN;
	}
	//����
	if(NDK_MdmHangup()!=NDK_OK)
	{
		send_result("line %d:%s�������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
	return;
}


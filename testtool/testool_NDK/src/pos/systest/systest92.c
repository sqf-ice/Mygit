/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: �ۺ�ģ��
* file name			: 92.c
* Author 			: 
* version			: V1.0
* DATE				:
* directory 			: 
* description			: SP10_T����
* related document	: �й���������ǩ�ְ�淶.pdf�͵���ǩ�����Զ���ָ��.doc�ĵ�
*				  
************************************************************************
* log			: 
* Revision 1.0  20110620 10:10 jennywone163
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"SP10_Tǩ����"

#define	MAXWAITTIME  135  //135s
#define TESTIMAGE     "test.bmp"
#define DEFAULTIMAGE  "ico_nc.bmp"

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
* history 		 		: author			date			remarks
*			  		  jiangym 		   	2017/04/26		created
*****************************************************************/
static int select_port(void)
{
	/*private & local definition*/
	int nKeyin = 0, port = 0;

	/*process body*/
	while(1)
	{
		nKeyin = cls_show_msg("��ѡ����������Ĵ���:\n�ٴ���1\n�ڴ���2\n");
		switch (nKeyin)
		{
		case '1':
			port = PORT_NUM_COM1;
			break;
		case '2':
			port = PORT_NUM_COM2;
			break;
		default:
			continue;
			break;
		}
		return port;
	}
}

static int set_test_bps(void)
{
	/*private & local definition*/
	int ret = -1;
	
	/*process body*/
	// 1.������SP10_T���ӵ�POS���ڲ���
	cls_show_msg1(2, "��ѡ������豸���ӵ������");
	g_devicecom = select_port();

	if((ret=NDK_PortOpen(g_devicecom, "115200,8,N,1,20"))!=NDK_OK)
	{
		cls_show_msg("line %d:��ʼ��POS����ʧ��ret=%d", __LINE__, ret);
		return FAIL;
	}
	cls_show_msg1(2,"�򿪴��ڳɹ�");
	return SUCC;
}

//������������, ������Ӧ// ���flag
static ushort SP10_T_SendHandshake(uchar *flag)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, append[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0};
	
	/*process body*/
	memcpy(data,SP10_T_HANDSHAKE_REQUEST,SP10_T_LEN_CMD);
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;	
	if(memcmp(answer,SP10_T_HANDSHAKE_RESPONSE,1))
		return SP10_T_ERR_HANDSHAKE_RESPONSE;
	else
	{
		if(append[0]==0x00)
			return SP10_T_ERR_HANDSHAKE_APPEND;//ǩ����δ׼����,POS����ʾ"û��������ǩ�ְ�"
	}
	flag[0] = append[appendlen-1];
	return SP10_T_SUCC;
}

//ǰһ��״̬֪ͨ����Ӧ
//����cmdappend: ��Ҫ��Ϊ�������,���Բ�ͬ������в�ͬ����Ӧ ,��Ҫ����ǩ�ֱ����ǩ�ֳɹ���Ӧ�ı�Ŷ�Ӧ
//���append:��Ӧ��CMD�����״ֵ̬������ֵ:0x01 0x02 0x03 0x04
static ushort SP10_T_FrontStatus(uchar *cmdappend,uchar *append)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0};
	
	/*process body*/
	memcpy(data,SP10_T_FRONT_STATUS_INFORM,SP10_T_LEN_CMD);
	memcpy(data+SP10_T_LEN_CMD,cmdappend,SP10_T_FRONT_STATUS_APPENDLEN);
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD+SP10_T_FRONT_STATUS_APPENDLEN, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;	
	if(!memcmp(answer,SP10_T_FRONT_STATUS_RESPONSE,1))
		return SP10_T_SUCC;
	else
		return SP10_T_ERR_FRONT_STATUS_RESPONSE;	
}

//ǩ������������Ӧ�����ɹ��򲻳ɹ�//����cmdappend(��ǩ�ְ�����ʾ��)���signnum:����ǩ�ֱ��
static ushort SP10_T_SignInput(uchar *cmdappend,uchar *signnum,uchar *data2,char *response)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, append[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0};	
	
	/*process body*/
	memcpy(data,SP10_T_SIGN_INPUT_REQUEST,SP10_T_LEN_CMD);
	memcpy(data+SP10_T_LEN_CMD,cmdappend,SP10_T_SIGN_INPUT_APPENDLEN);
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD+SP10_T_SIGN_INPUT_APPENDLEN , answer, append, &appendlen, MAXWAITTIME))!=SP10_T_SUCC)//��߳�ʱʱ����Ҫ��ǩ�����ϵ����ʱʱ���Ӧ����Ҫ����һ��
		return ret;	
	if(!memcmp(answer,response,1) || !memcmp(response,"",1)) //��Ԥ����Ӧֵһ���ٷֱ���
	{
		if(!memcmp(answer,SP10_T_SIGN_SUCC_RESPONSE,1))//�ɹ���Ӧ�������ǩ�ֱ��
		{
			memcpy(signnum,append,3);
			memcpy(data2,append+3,appendlen-3);
			return SP10_T_SUCC;
		}		
		else if(!memcmp(answer,SP10_T_SIGN_FAIL_RESPONSE,1) && (appendlen == 0))//��Ӧʧ���ж�appendlen�Ƿ�Ϊ0
			return SP10_T_SUCC;
		else
			return SP10_T_ERR_SIGN_RESPONSE;	
	}
	else		
		return SP10_T_ERR_SIGN_RESPONSE;	
}

//ǩ�ֽ���������Ӧ
static ushort SP10_T_SignEnd(void)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, append[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0};
	
	/*process body*/
	memcpy(data,SP10_T_SIGN_END_REQUEST,SP10_T_LEN_CMD);
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;	
	if(memcmp(answer,SP10_T_SIGN_END_RESPONSE,1))
		return SP10_T_ERR_SIGN_END_RESPONSE;	
	else
	{
		if(appendlen != 0)
			return SP10_T_ERR_SIGN_END_APPENDLEN;
	}
	return SP10_T_SUCC;
}

//��������������Ӧ//���signinfnum1λsignnum3λ
static ushort SP10_T_BulkTransfer(uchar *signinfnum, uchar *signnum)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, append[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0};
	
	/*process body*/
	memcpy(data,SP10_T_BULK_TRANSFER_REQUEST,SP10_T_LEN_CMD);
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;	
	if(!memcmp(answer,SP10_T_BULK_TRANSFER_RESPONSE,1))//��ȷ��Ӧ
	{
		signinfnum[0] = append[0];
		memcpy(signnum,append+1,3);
		return SP10_T_SUCC;
	}
	else
		return SP10_T_ERR_BULK_TRANSFER_RESPONSE;	
}

//����������ճɹ�Ӧ�����Ӧ������������Ӧ//���signinfnum1λsignnum3λ
static ushort SP10_T_BulkTransferSucc(uchar *signinfnum,uchar *signnum)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, append[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0};
	
	/*process body*/
	memcpy(data,SP10_T_BULK_TRANSFER_SUCC,SP10_T_LEN_CMD);
	memcpy(data+1,signnum,SP10_T_BULK_TRANSFER_APPENDLEN);
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD+SP10_T_BULK_TRANSFER_APPENDLEN, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;	
	if(!memcmp(answer,SP10_T_BULK_TRANSFER_RESPONSE,1))//��ȷ��Ӧ
	{
		signinfnum[0] = append[0];
		memcpy(signnum,append+1,3);
		return SP10_T_SUCC;
	}
	else
		return SP10_T_ERR_BULK_TRANSFER_RESPONSE;	
}

//�����������ʧ��//���signinfnum1λsignnum3λ
static ushort SP10_T_BulkTransferFail(uchar *signinfnum,uchar *signnum)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, append[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0};
	
	/*process body*/
	memcpy(data,SP10_T_BULK_TRANSFER_FAIL,SP10_T_LEN_CMD);
	memcpy(data+1,signnum,SP10_T_BULK_TRANSFER_APPENDLEN);
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD+SP10_T_BULK_TRANSFER_APPENDLEN, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;	
	if(!memcmp(answer,SP10_T_BULK_TRANSFER_RESPONSE,1))//��ȷ��Ӧ
	{
		signinfnum[0] = append[0];
		memcpy(signnum,append+1,3);
		return SP10_T_SUCC;
	}
	else
		return SP10_T_ERR_BULK_TRANSFER_RESPONSE;	
}

//��������������Ӧ
static ushort SP10_T_BulkEnd(void)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, append[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0};
	
	/*process body*/
	memcpy(data,SP10_T_BULK_END_REQUEST,SP10_T_LEN_CMD);
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;	
	if(memcmp(answer,SP10_T_BULK_END_RESPONSE,1))
		return SP10_T_ERR_BULK_END_RESPONSE;	
	else
	{
		if(appendlen != 0)
			return SP10_T_ERR_BULK_END_APPENDLEN;
	}
	return SP10_T_SUCC;
}

//�����������һ�㶼�ǽ����쳣ָ�����û�������ָ�����ú�ͨ��ǩ����������Ч��
//��������������룬����ͬ�������
//�����л�ǩ����ģʽ:���óɹ����ټ��������Ĳ���,������ʧ��ֱ�ӷ��ؾ���ʧ��ֵ
static ushort SP10_T_SwitchSignMode(uchar cmdappend)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0}, append[SP10_T_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, SP10_T_SWITCH_SIGN_MODE, SP10_T_LEN_CMD);
	data[1] = cmdappend;
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD+1, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;
	if(!memcmp(answer,SP10_T_SWITCH_SIGN_MODE,1))
	{
		if(append[0] != SP10_T_RESPONSE_STATUS_SUCC)
			return SP10_T_ERR_SET_RESPONSE_STATUS;
	}
	return SP10_T_SUCC;
}

//����ǩ����Ĵ���ͼƬ //�������ڳ��Ȳ�����Ҫ�������볤�Ȳ���
static ushort SP10_T_SignStandbyImage(uchar *cmdappend, int cmdappendlen)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0}, append[SP10_T_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, SP10_T_SIGN_STANDBY_IMAGE, SP10_T_LEN_CMD);	
	memcpy(data+SP10_T_LEN_CMD,cmdappend,cmdappendlen);
	if((ret=SP10_T_frame_factory(data, cmdappendlen+SP10_T_LEN_CMD, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;
	if(!memcmp(answer,SP10_T_SIGN_STANDBY_IMAGE,1))
	{
		if(append[0] != SP10_T_RESPONSE_STATUS_SUCC)
			return SP10_T_ERR_SET_RESPONSE_STATUS;
	}
	return SP10_T_SUCC;
}

//����ǩ����ǩ����ʱʱ�� //����2λ
static ushort SP10_T_SignTimeout(uchar *cmdappend)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0}, append[SP10_T_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, SP10_T_SET_SIGN_TIMEOUT, SP10_T_LEN_CMD);
	memcpy(data+SP10_T_LEN_CMD,cmdappend,SP10_T_SET_TIMEOUT_APPENDLEN);
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD+SP10_T_SET_TIMEOUT_APPENDLEN, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;
	if(!memcmp(answer,SP10_T_SET_SIGN_TIMEOUT,1))
	{
		if(append[0] != SP10_T_RESPONSE_STATUS_SUCC)
			return SP10_T_ERR_SET_RESPONSE_STATUS;
	}
	return SP10_T_SUCC;
}

//����ǩ����ǩ���Ĵ������� 1λ
static ushort SP10_T_SignNum(uchar cmdappend)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0}, append[SP10_T_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, SP10_T_SET_SIGN_NUM, SP10_T_LEN_CMD);
	data[1] = cmdappend;
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD+1, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;
	if(!memcmp(answer,SP10_T_SET_SIGN_NUM,1))
	{
		if(append[0] != SP10_T_RESPONSE_STATUS_SUCC)
			return SP10_T_ERR_SET_RESPONSE_STATUS;
	}
	return SP10_T_SUCC;
}

//����ǩ���ʼ���ϸ//����1λ
static ushort SP10_T_SignWriting(uchar cmdappend)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0}, append[SP10_T_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, SP10_T_SET_SIGN_WRITING, SP10_T_LEN_CMD);
	data[1] = cmdappend;
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD+1, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;
	if(!memcmp(answer,SP10_T_SET_SIGN_WRITING,1))
	{
		if(append[0] != SP10_T_RESPONSE_STATUS_SUCC)
			return SP10_T_ERR_SET_RESPONSE_STATUS;
	}
	return SP10_T_SUCC;
}

//�������ͼƬ��С // �����λ
static ushort SP10_T_OutImageSize(uchar *cmdappend)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0}, append[SP10_T_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, SP10_T_SET_IMAGE_SIZE, SP10_T_LEN_CMD);
	memcpy(data+SP10_T_LEN_CMD,cmdappend,SP10_T_IMAGE_SIZE_APPENDLEN);
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD+SP10_T_IMAGE_SIZE_APPENDLEN, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;
	if(!memcmp(answer,SP10_T_SET_IMAGE_SIZE,1))
	{
		if(append[0] != SP10_T_RESPONSE_STATUS_SUCC)
			return SP10_T_ERR_SET_RESPONSE_STATUS;
	}
	return SP10_T_SUCC;
}

//�������ͼƬ���� //����1λ
static ushort SP10_T_OutBackImage(uchar cmdappend)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0}, append[SP10_T_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, SP10_T_SET_IMAGE_BACKGROUND, SP10_T_LEN_CMD);
	data[1] = cmdappend;
	if((ret=SP10_T_frame_factory(data, SP10_T_LEN_CMD+1, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;
	if(!memcmp(answer,SP10_T_SET_IMAGE_BACKGROUND,1))
	{
		if(append[0] != SP10_T_RESPONSE_STATUS_SUCC)
			return SP10_T_ERR_SET_RESPONSE_STATUS;
	}
	return SP10_T_SUCC;
}

//��ȡ��������  Ŀǰ��֧�� û��������Ӳ��
#if 0
static ushort SP10_T_ReadWaveData(uchar *cmdappend)
{
	/*private & local definition*/
	ushort ret = 0;
	int appendlen = 0;
	uchar data[SP10_T_MAXLEN_DATA] = {0}, answer[1] = {0}, append[SP10_T_MAXLEN_DATA] = {0};
	
	/*process body*/
	memcpy(data, SP10_T_READ_WAVE_DATA, SP10_T_LEN_CMD);
	memcpy(data+SP10_T_LEN_CMD,cmdappend,SP10_T_READ_WAVE_APPENDLEN);
	if((ret=SP10_T_frame_factory(data, SP10_T_READ_WAVE_APPENDLEN+SP10_T_LEN_CMD, answer, append, &appendlen, 0))!=SP10_T_SUCC)
		return ret;
	if(!memcmp(answer,SP10_T_READ_WAVE_DATA,1))
	{
		if(append[0] != SP10_T_RESPONSE_STATUS_SUCC)
			return SP10_T_ERR_SET_RESPONSE_STATUS;
		//else  ���԰Ѷ�ȡ�������ݴ�����
	}
	return SP10_T_SUCC;
}
#endif

//�ϲ���ý��в���
//��������:��������--ǩ����������--ǩ�ֽ������� �����ǰǩ�ֱ�ż�����������
static void test_base_sign(uchar *signnum,uchar *data,char *response)
{
	ushort ret=SP10_T_FAIL;
	uchar inputdata[8]="newland8",saveflag[1] = {0};
	
	//��������
	if((ret=SP10_T_SendHandshake(saveflag))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:��������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	//ǩ����������
	if((ret=SP10_T_SignInput(inputdata,signnum,data,response))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ����������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	//ǩ�ֽ�������
	if((ret=SP10_T_SignEnd())!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ�ֽ�������ʧ��(%04x)", __LINE__, ret);
		return;
	}
}

//����ǩ�����봫�����,����ǩ�����������ǩ����ɳɹ���Ӧ������ǩ������쳣��Ӧ�Լ�û�з���ǩ�ֽ�������
static void test_sign_flow(void)
{
	uchar signnum[3] = {0},data[SP10_T_MAXLEN_DATA] = {0};

	//case1:ǩ����������ǩ����ɳɹ���Ӧ
	cls_show_msg1(2,"ǩ������ʾǩ��ʱ����ǩ����ȷ��");	
	test_base_sign(signnum,data,SP10_T_SIGN_SUCC_RESPONSE);
	cls_show_msg1(2,"ǩ������ʾǩ��ʱ��������ǩ����ȷ��");	
	test_base_sign(signnum,data,SP10_T_SIGN_SUCC_RESPONSE);
	
	//case2:ǩ����������ǩ������쳣��Ӧ
	cls_show_msg1(2,"ǩ������ʾǩ��ʱ���밴ȡ��");	
	test_base_sign(signnum,data,SP10_T_SIGN_FAIL_RESPONSE);
	cls_show_msg1(2,"ǩ������ʾǩ��ʱ��������ǩ����ȡ��");	
	test_base_sign(signnum,data,SP10_T_SIGN_FAIL_RESPONSE);
	
	//case3:ǩ�����������ʱ�쳣��Ӧ
	cls_show_msg1(2,"ǩ������ʾǩ��ʱ����ȴ���ʱ");	
	test_base_sign(signnum,data,SP10_T_SIGN_FAIL_RESPONSE);

	cls_show_msg1(2,"ǩ�����봫�����ͨ��!");
	return;
}

//ǰһ��״̬֪ͨ������ֲ�ͬ��Ӧ�Ĳ���
static void test_front_status(void)
{
	ushort ret=SP10_T_FAIL;
	uchar signnum[3] = {0}, cmdappend[4] = {0}, status[1] = {0};
	uchar data[SP10_T_MAXLEN_DATA] = {0},signinfnum[1] = {0};
	int i = 0,cnt = 0;

	//����ǰ��:
	//����һ������ǩ�����������������ǩ�ֱ�Ŵ���ǰһ��״̬����
	cls_show_msg1(2,"��ʾǩ��ʱ��Ҫǩ����ȷ��");
	test_base_sign(signnum,data,SP10_T_SIGN_SUCC_RESPONSE);
	
	//�������� (ֻҪ�й�������������ɲ���Ҫ����)
	//case1:ǰһ��״̬֪ͨ:ǩ�ְ屣��ǩ����ӦΪ����ǩ�ֳɹ��洢
	memcpy(cmdappend,signnum,3);
	cmdappend[3] = 0x00;//ǩ�ְ屣��ǩ��
	if((ret=SP10_T_FrontStatus(cmdappend,status))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǰһ��״̬����ʧ��(%04x)", __LINE__, ret);
		return;
	}
	if(status[0] != 0x02)
	{
		cls_show_msg("line %d:ǰһ��״̬����ʧ��(0x%2x)", __LINE__, status[0]);
		return;
	}
	
	//case2:ǰһ��״̬֪ͨ:ǩ�ְ岻����ǩ����ӦΪ����ǩ����ɾ��
	cmdappend[3] = 0x01;//ǩ�ְ岻����ǩ��
	if((ret=SP10_T_FrontStatus(cmdappend,status))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǰһ��״̬����ʧ��(%04x)", __LINE__, ret);
		return;
	}
	if(status[0] != 0x01)
	{
		cls_show_msg("line %d:ǰһ��״̬����ʧ��(0x%2x)", __LINE__, status[0]);
		return;
	}
	
	//case3:������������(ǩ�ְ���ɾ��ǩ��)�ٶ� ͬһǩ�ֱ��������Ҫ�󱣴�ǩ�ֵĻ��᷵��ǩ�ִ洢ʧ��
	cmdappend[3] = 0x00;//ǩ�ְ屣��ǩ��
	if((ret=SP10_T_FrontStatus(cmdappend,status))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǰһ��״̬����ʧ��(%04x)", __LINE__, ret);
		return;
	}
	if(status[0]!=0x04)
	{
		cls_show_msg("line %d:ǰһ��״̬����ʧ��(0x%2x)", __LINE__, status[0]);
		return;
	}
	
	//case4:��ӦΪǩ�ֳɹ��洢,�洢�ռ�����10�� //��Ҫǩ��90��  
	//ͨ�����������ȡ��ǰ��ʣ���ǩ����
	if((ret=SP10_T_BulkTransfer(signinfnum,signnum))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:������������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	cnt=90-signinfnum[0];//������Ҫ����ǩ�ֵı���
	cls_show_msg("��Ҫ�ٽ���%d��ǩ��,�������ʼ����",cnt);
	for(i=0;i<cnt;i++)
	{
		cls_printf("���е�%d��ǩ��,��ʾǩ��ʱ��ǩ����ȷ��",i+1);
		test_base_sign(signnum,data,SP10_T_SIGN_SUCC_RESPONSE);
		memcpy(cmdappend,signnum,3);
		cmdappend[3] = 0x00;//ǩ�ְ屣��ǩ��
		if((ret=SP10_T_FrontStatus(cmdappend,status))!=SP10_T_SUCC)
		{	
			cls_show_msg("line %d:ǰһ��״̬����ʧ��(%04x)", __LINE__, ret);
			return;
		}
		if(status[0] != 0x02)
		{
			cls_show_msg("line %d:ǰһ��״̬����ʧ��(0x%2x)", __LINE__, status[0]);
			return;
		}
	}
	//ִ��91-100�ʾ�����0x03:ʣ��洢�ռ����10��
	for(i=0;i<10;i++)
	{
		cls_printf("���е�%d��ǩ��,��ʾǩ��ʱ��ǩ����ȷ��",91+i);
		test_base_sign(signnum,data,SP10_T_SIGN_SUCC_RESPONSE);
		memcpy(cmdappend,signnum,3);
		cmdappend[3] = 0x00;//ǩ�ְ屣��ǩ��
		if((ret=SP10_T_FrontStatus(cmdappend,status))!=SP10_T_SUCC)
		{	
			cls_show_msg("line %d:ǰһ��״̬����ʧ��(%04x)", __LINE__, ret);
			return;
		}
		if(status[0] != 0x03)
		{
			cls_show_msg("line %d:ǰһ��״̬����ʧ��(0x%2x)", __LINE__, status[0]);
			return;
		}
	}
	//ִ�е�101�ʻ᷵��0x04 �洢ʧ��
	cls_printf("��ʾǩ��ʱ��ǩ����ȷ��");
    test_base_sign(signnum,data,SP10_T_SIGN_SUCC_RESPONSE);
	memcpy(cmdappend,signnum,3);
	cmdappend[3] = 0x00;//ǩ�ְ屣��ǩ��	
	if((ret=SP10_T_FrontStatus(cmdappend,status))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǰһ��״̬����ʧ��(%04x)", __LINE__, ret);
		return;
	}
	if(status[0]!=0x04)
	{
		cls_show_msg("line %d:ǰһ��״̬����ʧ��(0x%2x)", __LINE__, status[0]);
		return;
	}
	cls_show_msg("ǰһ��״̬����ͨ��,������ڲ˵���ѡ�������������,���������");
	return;
}

//�����������
static void test_bulk_transfer(void)
{
	ushort ret=SP10_T_FAIL;
	uchar signnum[3] = {0}, signinfnum[1] = {0}, signinfnum1[1] = {0},saveflag[1] = {0};
	int i = 0;

	cls_printf("�������������...");
	//��������
	if((ret=SP10_T_SendHandshake(saveflag))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:��������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	//case1:������������--����������ճɹ�Ӧ�� --ǩ�ְ���Ӧ��signinfnumҪ��1,��������������ʱ��Ӧ�Ĳ�һ��
	if((ret=SP10_T_BulkTransfer(signinfnum,signnum))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:������������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	//��ȡsigninfnum��ֵ,���Ϊ0��ʾǩ�ְ���ǩ����Ϣ��û�еĴ���Ͳ�ִ��Ӧ��,�������0��ִ��Ӧ��
	//���ֵΪ0xF1��ʾ��֧�ִ洢��ôҲ�����贫��;���ֻʣһ���˷��ͽ��ճɹ�Ӧ����ٷ���(��������Ͼ�����)
	if(signinfnum[0]>1 && signinfnum[0] != 0xF1)
	{
		//����������ճɹ�Ӧ��
		if((ret=SP10_T_BulkTransferSucc(signinfnum1,signnum))!=SP10_T_SUCC)
		{	
			cls_show_msg("line %d:��������ɹ�Ӧ��ʧ��(%04x)", __LINE__, ret);
			return;
		}
		if(signinfnum[0] != signinfnum1[0]+1)
		{
			cls_show_msg("line %d:��������ɹ�Ӧ��ʧ��(0x%2x,0x%2x)", __LINE__, signinfnum[0], signinfnum1[0]);
			return;
		}
	}	
	//case2:������������--�����������ʧ��Ӧ�� --ǩ�ְ���Ӧ��signinfnumû�б仯,��������������ʱ��Ӧ��һ��
	if((ret=SP10_T_BulkTransfer(signinfnum,signnum))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:������������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	if(signinfnum[0]>0 && signinfnum[0] != 0xF1) //ÿ�����������������Ҫ�鿴signinfnum�Ƿ����0������Ӧ��
	{
		//�����������ʧ��Ӧ�� 
		if((ret=SP10_T_BulkTransferFail(signinfnum1,signnum))!=SP10_T_SUCC)
		{	
			cls_show_msg("line %d:��������ʧ��Ӧ��ʧ��(%04x)", __LINE__, ret);
			return;
		}
		if(signinfnum[0] != signinfnum1[0])
		{
			cls_show_msg("line %d:��������ʧ��Ӧ��ʧ��(0x%2x,0x%2x)", __LINE__, signinfnum[0], signinfnum1[0]);
			return;
		}
	}
	//case3:ѭ�������������󼰽��ճɹ�Ӧ��
	if((ret=SP10_T_BulkTransfer(signinfnum,signnum))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:������������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	if(signinfnum[0]>0 && signinfnum[0] != 0xF1) 
	{	
		if(signinfnum[0]>1)
		{	
			for(i=0;i<(signinfnum[0]-1);i++)
			{
				if((ret=SP10_T_BulkTransferSucc(signinfnum1,signnum))!=SP10_T_SUCC)
				{	
					cls_show_msg("line %d:��������ɹ�Ӧ��ʧ��(%04x)", __LINE__, ret);
					return;
				}
			}
		}
		//ֻ��һ�ʻ������һ�ʷ���Ӧ��ɹ������з��ص�ʵ����ִ��һ�����������ȡ��signinfnumҪΪ0
		if((ret=SP10_T_BulkTransferSucc(signinfnum1,signnum))!=SP10_T_ERR_FRAME_STX)
		{	
			cls_show_msg("line %d:��������ɹ�Ӧ��ʧ��(%04x)", __LINE__, ret);
			return;
		}
	}
	if((ret=SP10_T_BulkTransfer(signinfnum,signnum))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:������������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	if(signinfnum[0] != 0)
	{
		cls_show_msg("line %d:��������������Ӧʧ��(0x%2x)", __LINE__, signinfnum[0]);
		return;
	}
	//������������
	if((ret=SP10_T_BulkEnd())!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:������������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	cls_show_msg1(2,"�����������ͨ��!");
	return;
}

//������ָ�����
//�����л�ǩ����ģʽ,����ǩ���岻����ǩ���ͱ���ǩ��
static void test_switch_mode(void)
{
	ushort ret=SP10_T_FAIL;
	uchar  saveflag[1] = {0};

	//case1:��֤Ĭ���Ǳ���ǩ����
	//����������Ӧ�е�flagλӦ����0x00 ��ʾǩ����֧�ִ洢
	if((ret=SP10_T_SendHandshake(saveflag))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:��������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	if(saveflag[0] != 0x01)
	{	
		cls_show_msg("line %d:ǩ�����Ƿ�֧�ִ洢����ʧ��(0x%2x)", __LINE__, saveflag[0]);
		return;
	}	
	
	//case2:�쳣��������
	if((ret=SP10_T_SwitchSignMode(0x02))!=SP10_T_ERR_SET_RESPONSE_STATUS)
	{	
		cls_show_msg("line %d:ǩ����ģʽ����ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	
	//case2:����0x00:������ǩ�� ͨ������������Ӧ���ж�
	if((ret=SP10_T_SwitchSignMode(0x00))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ����ģʽ����ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}	
	//����������Ӧ�е�flagλӦ����0x00 ��ʾǩ���岻֧�ִ洢
	if((ret=SP10_T_SendHandshake(saveflag))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:��������ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	if(saveflag[0] != 0x00)
	{	
		cls_show_msg("line %d:ǩ�����Ƿ�֧�ִ洢����ʧ��(0x%2x)", __LINE__, saveflag[0]);
		goto ERR;
	}
	
	//case3:����0x01:����ǩ��
	if((ret=SP10_T_SwitchSignMode(0x01))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ����ģʽ����ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	//����������Ӧ�е�flagλӦ����0x00 ��ʾǩ����֧�ִ洢
	if((ret=SP10_T_SendHandshake(saveflag))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:��������ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	if(saveflag[0] != 0x01)
	{	
		cls_show_msg("line %d:ǩ�����Ƿ�֧�ִ洢����ʧ��(0x%2x)", __LINE__, saveflag[0]);
		goto ERR;
	}	
	cls_show_msg1(2,"ǩ����ģʽ���ò���ͨ��!");
ERR:
	if((ret=SP10_T_SwitchSignMode(0x01))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ����ģʽ����ʧ��(%04x)", __LINE__, ret);
		return;
	}
	return;
}

//����ǩ�������ͼƬ
static void test_standby_image(void)
{
	ushort ret=SP10_T_FAIL;
	uchar cmdappend[SP10_T_MAXLEN_DATA] = {0};
	uint punSize = 0, readlen = 0; 
	int fp = 0,cnt = 0,num = 0,value = 0,i = 0, j = 0;
	char buf[256] = {0};
	char *imagestr[] = {"����POS��������","�´�½logoͼ��"};

	if(cls_show_msg("ȷ��������ͼƬ%s,%s��main����Ŀ¼��,ESC�˳�",TESTIMAGE,DEFAULTIMAGE)==ESC)
		return;
	for(j=0;j<2;j++)
	{	
		if(j==0) //����ͼƬ����
		{
			//�ȼ���ͼƬ��С,���ּ��������
			fp=NDK_FsOpen(TESTIMAGE, "r");
			if((ret=NDK_FsFileSize(TESTIMAGE, &punSize))!= NDK_OK)
				cls_show_msg("��ȡ�ļ���Сʧ��(ret:%d)",ret);
		}
		else //��ΪĬ��ͼƬ
		{
			fp=NDK_FsOpen(DEFAULTIMAGE, "r");
			if((ret=NDK_FsFileSize(DEFAULTIMAGE, &punSize))!= NDK_OK)
				cls_show_msg("��ȡ�ļ���Сʧ��(ret:%d)",ret);
		}
		cnt = punSize/256;
		value = punSize%256;
		if(value != 0)
			num=cnt;
		else
			num=cnt-1;
		cls_printf("ͼƬ������...���ĵȴ�...");
		for(i=0;i<cnt;i++)
		{
			cmdappend[0]=(i&0xffff)>>8;
			cmdappend[1]=(i)&0xff;
			cmdappend[2]=((num-i)&0xffff)>>8;
			cmdappend[3]=(num-i)&0xff;
			if((readlen=NDK_FsRead(fp,buf,256))!=256)
			{	
				cls_show_msg("���ļ�ʧ��(readlen:%d)",readlen);
				return;
			}
			memcpy(cmdappend+4,buf,256);
			if((ret=SP10_T_SignStandbyImage(cmdappend,260))!=SP10_T_SUCC)
			{	
				cls_show_msg("line %d:����ͼƬ����ʧ��(%04x)", __LINE__, ret);
				return;
			}
		}
		if(value != 0)
		{
			cmdappend[0]=(cnt&0xffff)>>8;
			cmdappend[1]=cnt&0xff;
			cmdappend[2]=0x00;
			cmdappend[3]=0x00;
			if((readlen=NDK_FsRead(fp,buf,value))!=value)
			{	
				cls_show_msg("���ļ�ʧ��(readlen:%d)",readlen);
				return;
			}
			memcpy(cmdappend+4,buf,value);
			if((ret=SP10_T_SignStandbyImage(cmdappend,value+4))!=SP10_T_SUCC)
			{	
				cls_show_msg("line %d:����ͼƬ����ʧ��(%04x)", __LINE__, ret);
				return;
			}
		}	
		//ͼƬ������ɺ��ֱ��ˢ�¹ʾͲ���Ҫǩ��������֤
		if(cls_show_msg("ǩ�����ϵĴ���ͼƬ�ѱ�Ϊ%s,��ȷ��[Enter],����[����]",imagestr[j])!=ENTER)
		{
			cls_show_msg("line %d:����ǩ����ǩ����������ʧ��", __LINE__);
			return;
		}
		//�ر��ļ�
		NDK_FsClose(fp);
	}	
	cls_show_msg1(2,"����ͼƬ���ò���ͨ��!");
	return;
}

//����ǩ����ǩ����ʱʱ��
static void test_sign_timeout(void)
{
	ushort ret=SP10_T_FAIL;
	int i = 0;
	uchar cmdappend[2] = {0},signnum[3] = {0},data[SP10_T_MAXLEN_DATA] = {0};
	char *cmdpara[] = {
						 "\x00\x00", 
						 "\x00\x01",
						 "\x01\x2C",
						 "\x05\xDC",
						 };
	char *parastr[] = {
						 "�޳�ʱ", 
						 "1s",
						 "30s",
						 "150s",
						 };
	//case1:��֤Ĭ��ʱ��Ϊ150s
	cls_show_msg1(2,"�۲�ǩ�������Ͻǳ�ʱʱ���Ƿ�Ϊ150s,��ǩ����ȷ�ϻ�ȡ��");
	test_base_sign(signnum,data,"");
	if(cls_show_msg("ǩ���峬ʱʱ���Ƿ�Ϊ150s,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ǩ���峬ʱʱ�����ʧ��", __LINE__);
		goto ERR;
	}
	
	//���ó�ʱʱ���ͨ������--ǩ�������������鿴����ǩ�����ϵ�ʱ��
	//case2:���ó�ʱʱ��Ϊ0  ������ʱ//��ʱ1s//��ʱʱ��Ϊ30s
    for(i=0;i<(SZ_ARRAY(cmdpara)-1);i++)
	 {	
		memcpy(cmdappend,cmdpara[i],2);
		if((ret=SP10_T_SignTimeout(cmdappend))!=SP10_T_SUCC)
		{	
			cls_show_msg("line %d:ǩ���峬ʱʱ������ʧ��(%04x)", __LINE__, ret);
			goto ERR;
		}
		cls_show_msg1(2,"�۲�ǩ�������Ͻǳ�ʱʱ���Ƿ�Ϊ%s,��ǩ����ȷ�ϻ�ȡ��",parastr[i]);
		test_base_sign(signnum,data,"");
		if(cls_show_msg("ǩ���峬ʱʱ���Ƿ�Ϊ%s,��ȷ��[Enter],����[����]",parastr[i])!=ENTER)
		{
			cls_show_msg("line %d:����ǩ���峬ʱʱ�����ʧ��", __LINE__);
			goto ERR;
		}
	}
	cls_show_msg1(2,"ǩ����ʱʱ�����ò���ͨ��!");
ERR:  //�ָ���Ĭ��
	memcpy(cmdappend,cmdpara[3],2);
	if((ret=SP10_T_SignTimeout(cmdappend))!=SP10_T_SUCC)
	{	 
		cls_show_msg("line %d:ǩ���峬ʱʱ������ʧ��(%04x)", __LINE__, ret);
		return;
	}		 
	return;
}

//����ǩ����ǩ������:���ú�ͨ��ǩ�����ϵ�����������ж�
static void test_sign_num(void)
{
	ushort ret=SP10_T_FAIL;
	uchar signnum[3] = {0},data[SP10_T_MAXLEN_DATA] = {0};

	//case1:����Ĭ�ϵ�ǩ������Ϊ2
	cls_show_msg1(2,"��ʾǩ��ʱ��ǩ�ֺ�����1��,�鿴���䰴ťӦ��ʧ,��ǩ��ȷ�Ϻ��˳�");
	test_base_sign(signnum,data,"");
	if(cls_show_msg("ǩ����ǩ������һ�κ�������ʧ,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ǩ����ǩ����������ʧ��", __LINE__);
		goto ERR;
	}
	
	//case2:����ǩ������Ϊ������
	if((ret=SP10_T_SignNum(0x00))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ����������ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}	
	cls_show_msg1(2,"��ʾǩ��ʱ��ǩ�ֺ����䲢�ظ�5������,�˳�ʱ��ȷ��");
	test_base_sign(signnum,data,"");
	if(cls_show_msg("ǩ�����ϵ�ǩ����������ɷ�������5��,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ǩ����ǩ����������ʧ��", __LINE__);
		goto ERR;
	}
	
	//case3:����ǩ������Ϊ1��
	if((ret=SP10_T_SignNum(0x01))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ����������ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}	
	cls_show_msg1(2,"��֤ǩ�����ϵ�ǩ����������,��ʾǩ��ʱ��ǩ�ֺ�ȷ��");
	test_base_sign(signnum,data,"");
	if(cls_show_msg("ǩ������������ѡ��,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ǩ����ǩ����������ʧ��", __LINE__);
		goto ERR;
	}
	
	cls_show_msg1(2,"ǩ���������ò���ͨ��!");
ERR:	//�ָ�Ĭ��
	if((ret=SP10_T_SignNum(0x02))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ����������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	return;
}

//����ǩ����ǩ���ʼ���ϸ�̶�
static void test_sign_writing(void)
{
	ushort ret=SP10_T_FAIL;
	uchar signnum[3] = {0},data[SP10_T_MAXLEN_DATA] = {0};

	//case1:��֤ǩ����Ĭ�ϱʼ�
	cls_show_msg1(2,"��֤ǩ�����ϵ�Ĭ�ϱʼ�(��ϸ),��ʾǩ��ʱ��ǩ�ֲ�ȷ��");
	test_base_sign(signnum,data,"");
	if(cls_show_msg("ǩ�����ϵ�Ĭ�ϱʼ���ϸ,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ǩ���ʼ���ϸ����ʧ��", __LINE__);
		goto ERR;
	}

	//case2:�쳣��������
	if((ret=SP10_T_SignWriting(0x03))!=SP10_T_ERR_SET_RESPONSE_STATUS)
	{	
		cls_show_msg("line %d:ǩ���ʼ���ϸ����ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	
	//case3:���ñʼ���ϸΪĬ��*1/2   �ĵ���ʾ֧��ʵ���ǲ�֧��
	if((ret=SP10_T_SignWriting(0x02))!=SP10_T_ERR_SET_RESPONSE_STATUS)
	{	
		cls_show_msg("line %d:ǩ���ʼ���ϸ����ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	
	//case4:���ñʼ���ϸΪĬ��*2
	if((ret=SP10_T_SignWriting(0x01))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ���ʼ���ϸ����ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	cls_show_msg1(2,"��֤ǩ�����ϵıʼ��Դ�,��ʾǩ��ʱ��ǩ�ֲ�ȷ��");
	test_base_sign(signnum,data,"");
	if(cls_show_msg("ǩ�����ϵıʼ��Դ�,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ǩ���ʼ���ϸ����ʧ��", __LINE__);
		goto ERR;
	}
	
	//case5:���ñʼ���ϸΪĬ��
	if((ret=SP10_T_SignWriting(0x00))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ���ʼ���ϸ����ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	cls_show_msg1(2,"��֤ǩ�����ϵıʼ���ϸ,��ʾǩ��ʱ��ǩ�ֲ�ȷ��");
	test_base_sign(signnum,data,"");
	if(cls_show_msg("ǩ�����ϵıʼ���ϸ,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:����ǩ���ʼ���ϸ����ʧ��", __LINE__);
		goto ERR;
	}
	cls_show_msg1(2,"ǩ���ʼ����ò���ͨ��!");
ERR:  //�ָ�Ĭ��
	if((ret=SP10_T_SignWriting(0x00))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ���ʼ���ϸ����ʧ��(%04x)", __LINE__, ret);
		return;
	}
	return;
}

//�������ͼƬ��С
static void test_out_image_size(void)
{
	ushort ret=SP10_T_FAIL;
	int i = 0;
	uchar cmdappend[4] = {0},signnum[3] = {0},data[SP10_T_MAXLEN_DATA] = {0};
	char *err_para[] = { 
						  "\x00\x77\x00\x32", //119*50
						  "\x00\x77\x00\x64", //119*100
						  "\x01\x41\x00\x32", //321*50
						  "\x01\x41\x00\x64", //321*100
						  "\x00\x78\x00\x31", //120*49
						  "\x00\x78\x00\x65", //120*101
						  "\x01\x40\x00\x31", //320*49
						  "\x01\x40\x00\x65", //320*101
						 };
	char *cmd_para[] = { 
						  "\x00\x78\x00\x32", //120*50
						  "\x01\x40\x00\x64", //320*100
						  "\x01\x2C\x00\x64", //300*100
						 };
	char *parastr[] = { 
						  "120*50", 
						  "320*100",
						  "300*100", 
					    };
	
	//case1:��֤Ĭ�ϵ����ͼƬ��СΪ300*100
	cls_show_msg1(2,"�鿴ǩ�������ͼƬ��С�Ƿ�Ϊ%s,��ʾǩ��ʱ��ǩ�ֲ�ȷ��",parastr[2]);
	test_base_sign(signnum,data,"");
	if(cls_show_msg("ǩ��������ͼƬ��С�Ƿ�Ϊ%s,��ȷ��[Enter],����[����]",parastr[2])!=ENTER)
	{
		cls_show_msg("line %d:�������ͼƬ��С����ʧ��", __LINE__);
		goto ERR;
	}
	
	//case2:�쳣��������
    for(i=0;i<SZ_ARRAY(err_para);i++)
	{
		memcpy(cmdappend,err_para[i],4);
		if((ret=SP10_T_OutImageSize(cmdappend))!=SP10_T_ERR_SET_RESPONSE_STATUS)
		{	
			cls_show_msg("line %d:���ͼƬ����ʧ��(%04x)", __LINE__, ret);
			goto ERR;
 		}
	}
	
	//case2:�������ú�ͨ��ǩ���������������������ͼƬ��С
	//120*50 //320*100 
        for(i=0;i<(SZ_ARRAY(cmd_para)-1);i++)
	{
		memcpy(cmdappend,cmd_para[i],4);
		if((ret=SP10_T_OutImageSize(cmdappend))!=SP10_T_SUCC)
		{	
			cls_show_msg("line %d:���ͼƬ����ʧ��(%04x)", __LINE__, ret);
			goto ERR;
		}
		cls_show_msg1(2,"�鿴ǩ�������ͼƬ��С�Ƿ�Ϊ%s,��ʾǩ��ʱ��ǩ�ֲ�ȷ��",parastr[i]);
		test_base_sign(signnum,data,"");
		if(cls_show_msg("ǩ��������ͼƬ��С�Ƿ�Ϊ%s,��ȷ��[Enter],����[����]",parastr[i])!=ENTER)
		{
			cls_show_msg("line %d:�������ͼƬ��С����ʧ��", __LINE__);
			goto ERR;
		}
	}
	cls_show_msg1(2,"���ͼƬ��С���ò���ͨ��!");
ERR:   //�ָ���Ĭ��300*100
	memcpy(cmdappend,cmd_para[2],4);
	if((ret=SP10_T_OutImageSize(cmdappend))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:���ͼƬ����ʧ��(%04x)", __LINE__, ret);
		return;
	}
	return;
}

//�������ͼƬ����
static void test_background_image(void)
{
	ushort ret=SP10_T_FAIL;
	uchar signnum[3] = {0},data[SP10_T_MAXLEN_DATA] = {0};

	//case1:��֤Ĭ���Ƿ�Ϊ�׵׺���
	cls_show_msg("�밲װ��ӡֽ�����������");
	cls_printf("ǩ������ʾǩ��ʱ��ǩ��...");
	test_base_sign(signnum,data,SP10_T_SIGN_SUCC_RESPONSE);
	//conf_prn();
	lib_initprn(1);//ֱ�ӿ������ͱߴ�
	if((ret=NDK_PrnImage(320, 100, 0, (char *)data+62))!=NDK_OK)//ʵ��ͼƬ��data2���е�63���ֽڿ�ʼ��62�ǿ�����æ��λ�����ģ��ĵ���û����ȷ˵����
	{
		cls_show_msg("line %d:�����������ͼƬ����ʧ��", __LINE__);
		goto ERR;
	}	
	if(cls_show_msg("��ӡ�������Ƿ�Ϊ�׵׺���,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:�����������ͼƬ����ʧ��", __LINE__);
		goto ERR;
	}
	
	//case1:�쳣��������
	if((ret=SP10_T_OutBackImage(0x11))!=SP10_T_ERR_SET_RESPONSE_STATUS)
	{	
		cls_show_msg("line %d:���ͼƬ��������ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	if((ret=SP10_T_OutBackImage(0x01))!=SP10_T_ERR_SET_RESPONSE_STATUS)
	{	
		cls_show_msg("line %d:���ͼƬ��������ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	
	//case2:���úڵװ���
	cls_printf("ǩ������ʾǩ��ʱ��ǩ��...");
	if((ret=SP10_T_OutBackImage(0x10))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:���ͼƬ��������ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	test_base_sign(signnum,data,SP10_T_SIGN_SUCC_RESPONSE);
	if((ret=NDK_PrnImage(320, 100, 0, (char *)data+62))!=NDK_OK)
	{
		cls_show_msg("line %d:�����������ͼƬ����ʧ��", __LINE__);
		goto ERR;
	}	
	if(cls_show_msg("��ӡ�������Ƿ�Ϊ�ڵװ���,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:�����������ͼƬ����ʧ��", __LINE__);
		goto ERR;
	}
	
	//case2:���ð׵׺���
	cls_printf("ǩ������ʾǩ��ʱ��ǩ��...");
	if((ret=SP10_T_OutBackImage(0x00))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:���ͼƬ��������ʧ��(%04x)", __LINE__, ret);
		goto ERR;
	}
	test_base_sign(signnum,data,SP10_T_SIGN_SUCC_RESPONSE);
	if((ret=NDK_PrnImage(320, 100, 0, (char *)data+62))!=NDK_OK)//ʵ��ͼƬ��data2���е�63���ֽڿ�ʼ��62�ǿ�����æ��λ�����ģ��ĵ���û����ȷ˵����
	{
		cls_show_msg("line %d:�����������ͼƬ����ʧ��", __LINE__);
		goto ERR;
	}	
	if(cls_show_msg("��ӡ�������Ƿ�Ϊ�׵׺���,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:�����������ͼƬ����ʧ��", __LINE__);
		goto ERR;
	}
	cls_show_msg1(2,"���ͼƬ�������ò���ͨ��!");
ERR:  //�ָ���Ĭ��
	if((ret=SP10_T_OutBackImage(0x00))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:���ͼƬ��������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	return;
}

static void abnomal_test(void)
{
	ushort ret=SP10_T_FAIL;
	uchar inputdata[8]="newland8", data[SP10_T_MAXLEN_DATA] = {0}, saveflag[1] = {0};
	uchar signnum[3] = {0},signinfnum[1] = {0},cmdappend[4] = {0}, status[1] = {0};

	//case1:��������--ǩ�����������û�з���ǩ�ֽ�������ǩ������ڳ�ʱʱ�䵽��ص���������
	cls_show_msg1(2,"ǩ������ʾǩ��ʱ��ǩ�ֲ�ȷ��");	
	if((ret=SP10_T_SendHandshake(saveflag))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:��������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	if((ret=SP10_T_SignInput(inputdata,signnum,data,""))!=SP10_T_SUCC)
	{	
		cls_show_msg("line %d:ǩ����������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	sleep(4);//ǩ����ĳ�ʱʱ��Ϊ3s
	if(cls_show_msg("ǩ�����Ƿ�ص���������,��ȷ��[Enter],����[����]")!=ENTER)
	{
		cls_show_msg("line %d:�쳣����ʧ��", __LINE__);
		return;
	}
	
	//case2:δ�������־ͽ�������ָ������Ӧ�þ����ɹ�
	cls_show_msg("��ǩ�ְ���POS�������°β�����������.");
	//Ҫ�ȴ���ʱ���� ���Ҫ�ȴ�155s+20s*3=215s
	cls_printf("�쳣���Խ�����...��ȴ�215s");
	//δ�������־ͽ���ǩ����������
	if((ret=SP10_T_SignInput(inputdata,signnum,data,""))!=SP10_T_ERR_FRAME_STX)
	{	
		cls_show_msg("line %d:ǩ����������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	//δ�������ֽ���ǰһ��״̬֪ͨӦ��ʧ����Ҫ�ȴ�20s	
	if((ret=SP10_T_FrontStatus(cmdappend,status))!=SP10_T_ERR_FRAME_STX)
	{	
		cls_show_msg("line %d:ǰһ��״̬����ʧ��(%04x)", __LINE__, ret);
		return;
	}
	//δ�������ֽ���ǩ�ֽ�������
	if((ret=SP10_T_SignEnd())!=SP10_T_ERR_FRAME_STX)
	{	
		cls_show_msg("line %d:ǩ�ֽ�������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	//δ�������ֽ���������������
	if((ret=SP10_T_BulkTransfer(signinfnum,signnum))!=SP10_T_ERR_FRAME_STX)
	{	
		cls_show_msg("line %d:������������ʧ��(%04x)", __LINE__, ret);
		return;
	}
	cls_show_msg1(2,"�쳣����ͨ��!");
	return;
}

static void test_set_function(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	while(1)
       {
		keyin = cls_show_msg("1.ǩ����ģʽ 2.����ͼƬ\n"
							 "3.��ʱʱ�� 4.ǩ������\n"
							 "5.�ʼ���ϸ 6.ͼƬ��С\n"
							 "7.ͼƬ����\n");
		switch(keyin) 
		{
		case '1':
			test_switch_mode();
			break;
		case '2':
			test_standby_image();
			break;
		case '3':
			test_sign_timeout();
			break;	
		case '4':
			test_sign_num();
			break;		
		case '5':
			test_sign_writing();
			break;
		case '6':
			test_out_image_size();
			break;
		case '7':
			test_background_image();
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

static void test_sign_function(void)
{
	/*private & local definition*/
	int keyin = 0;

	/*process body*/
	while(1)
       {
		keyin = cls_show_msg("1.ǩ�����봫��\n"
							 "2.ǰһ��״̬\n"
							 "3.��������\n"
							 "4.�쳣����\n");
		switch(keyin) 
		{
		case '1':
			test_sign_flow();
			break;
		case '2':
			test_front_status();
			break;
		case '3':
			test_bulk_transfer();
			break;	
		case '4':
			abnomal_test();
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

void systest92()
{
	int keyin = 0;

	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);
	cls_show_msg("����ǰ�Ƚ��д�������(���Ȳ��Զ���ָ��),���������");
	
	while(1)
    {
        keyin = cls_show_msg("1.����ָ��\n"
							 "2.�Զ���ָ��\n"
							 "0.��������\n");
		switch(keyin) 
		{
		case '1':
			test_sign_function();
			break;
		case '2':
			test_set_function();
			break;
		case '0':
			set_test_bps();
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



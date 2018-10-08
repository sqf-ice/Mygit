/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ������ӡ���ۺϲ���	
* file name			: systest90.c
* Author 			: 	
* version			: 
* DATE				: 
* directory 			: 
* description			: ������ӡ���ۺϲ���
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
#include <pthread.h>

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"������ӡ���ۺ�"
#define PRNT_DATALEN   1024
#define CMD_LEN   3
#define TIMEOUT   10

#define FS   0x1c



/*------------global variables definition-----------------------*/
int comm_type = 0;
char bt_mac[7]={0};
int initflag=0;
/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/


/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	:
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 				: author			date			remarks
					 	jiangym			20150611	created
*****************************************************************/	
static int ME50P_data_factory(char *cmd, int cmdlen)
{
	if(comm_type == PORT_NUM_BT) //����ͨѶ��ʽ
	{
		int ret=0;
		//��ָ���豸������NDK_BTMasterClrBuf
		if((ret = NDK_BTMasterClrBuf(bt_mac)) != NDK_OK)
			return ret;
#if 0		
		for(i=0; i<cmdlen; i++) //�������ָ������
			fprintf(stderr,"0x%02X ",cmd[i]);
#endif
		//�����豸д����
		if((ret=NDK_BTMasterWrite(bt_mac, cmdlen, cmd)) != NDK_OK)
			return ret;
		
		return NDK_OK;
	}
	else
	{
		cls_show_msg("ͨѶ��ʽ���ô�������������");
		return NDK_ERR;
	}
}

static int ME50P_data_recv(int *len, char *recvbuf, int *recvlen)
{
	if(comm_type == PORT_NUM_BT) //����ͨѶ��ʽ
	{
		int ret=0;
		//�����豸������
		if((ret=NDK_BTMasterRead(bt_mac, len, recvbuf, TIMEOUT, recvlen)) != NDK_OK)
			return ret;	
		return NDK_OK;
	}
	else
	{
		cls_show_msg("ͨѶ��ʽ���ô�������������");
		return NDK_ERR;
	}
}

static int ME50P_cmd_Prnt(char *databuf, int datalen) //��ӡ����
{
	char data[PRNT_DATALEN]={0};
	int ret=0;
	
	memset(data, 0, sizeof(data));
	memcpy(data, databuf, datalen);
	data[datalen]=0x0a; //���ݷ�����Ҫ��0a
	if( (ret=ME50P_data_factory(data, datalen+1)) != NDK_OK )
		return ret;
	
	return NDK_OK;
}

static int ME50P_cmd_Send(char *databuf, int datalen) //�����ݷ��͵�������
{
	char data[PRNT_DATALEN]={0};
	int ret=0;
	
	memset(data, 0, sizeof(data));
	memcpy(data, databuf, datalen);
	if( (ret=ME50P_data_factory(data, datalen)) != NDK_OK )
		return ret;
	
	return NDK_OK;
}

static int ME50P_cmd_SetHT(void) //����ˮƽλ��
{
	char cmd[CMD_LEN-2]={0};
	int ret=0;
	
	cmd[0]=0x09;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetLF(void) //���ô�ӡ������
{
	char cmd[CMD_LEN-2]={0};
	int ret=0;
	
	cmd[0]=0x0A;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_TransRealState(char n, int *len, char *recvbuf, int *recvlen) //ʵʱ״̬����
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	cmd[0]=0x10;
	cmd[1]=0x04;
	cmd[2]=n;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	memset(recvbuf,0,sizeof(recvbuf));
	if( (ret=ME50P_data_recv(len, recvbuf, recvlen)) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetSpaceRight(char n) //�����ַ��Ҽ��
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1b;
	cmd[1]=0x20;
	cmd[2]=n;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetPrntMode(char mode) //���ô�ӡģʽ
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1b;
	cmd[1]=0x21;
	cmd[2]=mode;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetDefaultLS(void) //����Ĭ���м��
{
	char cmd[CMD_LEN-1]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1b;
	cmd[1]=0x32;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetLS(char n) //�����м��
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1b;
	cmd[1]=0x33;
	cmd[2]=n;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_InitPrnt(void)   //��ӡ��ʼ��
{
	char cmd[CMD_LEN-1]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1b;
	cmd[1]=0x40;
	//cls_show_msg("0x%2x,0x%2x",cmd[0],cmd[1]);
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetPrntPaper(char n)   //��ӡ����ֽ
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1B;
	cmd[1]=0x4A;
	cmd[2]=n;	
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetPrntFont(char f) //��������
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1b;
	cmd[1]=0x4d;
	cmd[2]=f;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetR90DC(char n) //ѡ��/ȡ��˳ʱ����ת90��
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=ESC;
	cmd[1]='V';
	cmd[2]=n;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetAlignment(char n) //ѡ����뷽ʽ
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1b;
	cmd[1]=0x61;
	cmd[2]=n;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetInvertedPrntMode(char mode) //ѡ��/ȡ�����ô�ӡģʽ
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1b;
	cmd[1]=0x7b;
	cmd[2]=mode;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetFontSize(char size)	//���������С
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1d;
	cmd[1]=0x21;
	cmd[2]=size;
	//cls_show_msg("0x%2x,0x%2x,0x%2x",cmd[0],cmd[1],cmd[2]);
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetReShowMode(char mode) //ѡ��/ȡ���ڰ׷��Դ�ӡģʽ
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1d;
	cmd[1]=0x42;
	cmd[2]=mode;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetLeftMargin(char nL, char nH) //������߾�
{
	char cmd[CMD_LEN+1]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1d;
	cmd[1]=0x4c;
	cmd[2]=nL;
	cmd[3]=nH;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetHVMovingUnit(char x, char y) //���ú���������ƶ���λ
{
	char cmd[CMD_LEN+1]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1d;
	cmd[1]=0x50;
	cmd[2]=x;
	cmd[3]=y;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetPrntAreaWidth(char nL, char nH) //���ô�ӡ������
{
	char cmd[CMD_LEN+1]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1d;
	cmd[1]=0x57;
	cmd[2]=nL;
	cmd[3]=nH;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetCUnderlineMode(char n) //ѡ��/ȡ�������»���ģʽ
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1c;
	cmd[1]=0x2d;
	cmd[2]=n;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

static int ME50P_cmd_SetCHeightWidth(char n) //ѡ��/ȡ�����ֱ��߱���
{
	char cmd[CMD_LEN]={0};
	int ret=0;
	
	memset(cmd, 0, sizeof(cmd));
	cmd[0]=0x1c;
	cmd[1]=0x57;
	cmd[2]=n;
	if( (ret=ME50P_data_factory(cmd, sizeof(cmd))) != NDK_OK )
		return ret;
	return NDK_OK;
}

//ָ��1
static void test_ME50P_cmd_SetHT(void)
{
	int ret=0;
	char *databuf[]={"����ˮƽ��λ��...",
					  "������0123456789ǰӦ��������96���㣬�ڶ�������Ӧ����������ӡ������",
					  "����ˮƽ��λ"};
	//ǰ��
	cls_printf("����ˮƽ��λ��...");
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}

	//case1.1:��׼ģʽ�£�����Ӧ����96����
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK ) 
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_SetHT()) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}

	//����ģʽ�µ�ˮƽ��λ������,�Ƿ���Ҫ����

	//����
	ME50P_cmd_Prnt(databuf[2], strlen(databuf[2]));
	return;
}

//ָ��2
//����Ĭ���м��󣬻��еľ���Ϊ5mm����3.75mm��һ��
static void test_ME50P_cmd_SetLF(void)
{
	int ret=0;
	char *databuf[]={"����LFָ���ӡ����0123456789abc",
					   "��������Ӧ�Ǵ��µ�һ�п�ʼ��ӡ"};
	
	cls_printf("���Դ�ӡ��������...");
	//case1:�ȷ��ʹ�ӡ���ݵ�������ӡ��������,����SetLFָ���ŻῪʼ��ӡ,���Ҵ�ӡ��ỻ��
	if( (ret=ME50P_cmd_Send(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ʧ��(%d)", __LINE__, ret);
		return;
	}
	if(cls_show_msg("������ӡ��Ӧ����ӡ���κ�����,��[ENTER],��[����]") != ENTER)
	{
		cls_show_msg("line %d:����ʧ��", __LINE__);
		return;
	}
	if( (ret=ME50P_cmd_SetLF()) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}

	return;
}

//ָ��3
static void test_ME50P_cmd_TransRealState(char n, char *databuf, int datalen, int *len, char *recvbuf, int *recvlen)
{
	//������δʵ��,�������ȿ�ʼ���

	//case1:δ�ʹ�ӡ��������»�ȡ����״̬Ӧ����׼���õ�״̬

	//case2:���ʹ�����ӡ����,�ô�ӡ���ڴ�ӡ��ʱ���ȡ��ӡ״̬,Ӧ��Ҫ��ȡ����ӡ��æ��״̬

	//case3:��������ͨ������Щ��ӡ��״̬,������Ӧ������ȥ��֤��Щ״̬�Ļ�ȡ�Ƿ���ȷ
	int ret=0;
	if( (ret=ME50P_cmd_TransRealState(n, len, recvbuf, recvlen)) != NDK_OK )
		return;
	if(n==2)
	if( (ret=ME50P_cmd_Prnt(databuf, datalen)) != NDK_OK )
		return;
	
	return;
}

//ָ��6
static void test_ME50P_cmd_SetSpaceRight(void)
{
	int ret=0;
	char *databuf[]={"��������Ҽ��31.91mm",
					 "��ӡ������12345678abc�Ҽ��Ϊ10mm"};

	cls_printf("�����Ҽ����...");
	//case4.1:�ڱ�׼ģʽ�£������Ҽ��Ϊ����Ҽ�࣬ʵ��Ӧ��Ԥ�����
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK ) 
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_SetSpaceRight(255)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	//case4.3:�����Ҽ��Ϊ10mm(n=80)��ʵ��Ӧ��Ԥ�����
	if( (ret=ME50P_cmd_SetSpaceRight(80)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	//case4.2:�����ó�������Ҽ���ʱ�򣬻��Զ�ת��Ϊ����Ҽ��.����btwrite�����bufΪchar�ͣ����ֻ��255���޷������255�����

	//�Ŵ�ģʽ��,�Ҽ��Ŵ���Ӧ����,������ľ��뻻��Ӧ��Ҫ���빫ʽ����,��Ҫʹ�ó���,Ӧ����n�ͻ������������

ERR:
	//���û�ԭ�ȵ��Ҽ��
	ME50P_cmd_SetSpaceRight(0);
	return;
}

//ָ��7 
static void test_ME50P_cmd_SetPrntMode(void) //����֤
{
	int ret=0, i=0;
	char mode[4]={0x00,0x10,0x20,0x80};

	//���ﻹ��ѹ��ASCII������B������ģʽû���ǵ�?0x01
	char *databuf[]={"���д�ӡ��������Ӧ���Ǳ�׼ASCII��",
					  "���д�ӡ��������Ӧ���Ǳ���ģʽ",
					  "���д�ӡ��������Ӧ���Ǳ���ģʽ",
					  "���д�ӡ��������Ӧ�����»���ģʽ"};
	
	cls_printf("���Դ�ӡģʽ��...");
	//case5:���Ա�׼/����/����/�»��ߴ�ӡģʽ
	for(i=0;i<4;i++)
	{
		if( (ret=ME50P_cmd_SetPrntMode(mode[i])) != NDK_OK )
		{
			cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		if( (ret=ME50P_cmd_Prnt(databuf[i], strlen(databuf[i]))) != NDK_OK )		
		{
			cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
	}

ERR:
	ME50P_cmd_SetPrntMode(0x00);
	return;
}

//ָ��13��ָ��14
//����м�ಢ��Ϊ956mm��ʵ��ֻ��670mm����ʾ����������
static void test_ME50P_cmd_SetLS(void)
{
	int ret=0;
	char *databuf[]={"��ӡ����0123456789abc�����Ҹö����ݵ��м��ӦΪ956mm",
					 "��ӡ����abc9876543210�����Ҹö����ݵ��м��ӦΪ500mm",
					 "��ӡ����0123456789abc�����Ҹö����ݵ��м��ΪĬ���м��3.75mm",
					 "���������м��"};

	cls_printf("�����м����...");
	//case6.1:��������:�ڱ�׼ģʽ�£��м������Ϊ����м�࣬ʵ��Ӧ��Ԥ�����
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_SetLS(255)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	//case6.2:��������:�ڱ�׼ģʽ�£��м������Ϊ�еȼ�࣬ʵ��Ӧ��Ԥ����� 
	if( (ret=ME50P_cmd_SetLS(133)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	//case6.3:��������:�ڱ�׼ģʽ�£��м�����ó���������ֵ��Ӧȡ����ࡣ����btwrite�����bufΪchar�ͣ����ֻ��255���޷������255�����

	//case6.4:����ΪĬ���м��
	if( (ret=ME50P_cmd_SetDefaultLS()) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[2], strlen(databuf[2]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	if( (ret=ME50P_cmd_Prnt(databuf[3], strlen(databuf[3]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
ERR:
	ME50P_cmd_SetLS(0);
	return;
}

//ָ��17
//��ʼ����ԭ���ڻ�������������Ȼ�ᱻ��ӡ��������������������ԭ�Ȼ����������ݻᱻ��ա�
static void test_ME50P_cmd_InitPrnt(void)
{
	int ret=0;
	char *databuf[]={"Ӧ���ô�ӡ������abc0123456789\n",
					 "Ӧ�ܴ�ӡ������9876543210abc"};
	
	cls_printf("���Դ�ӡ����ʼ����...");
	//case1:��ӡ����ʼ�������������ӡ������������
	if( (ret=ME50P_cmd_InitPrnt()) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Send(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_InitPrnt()) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret); 
		return;
	}

	return;
}

//ָ��21
//�޴�ӡ��ֽ��Ч��
static void test_ME50P_cmd_SetPrntPaper(void)
{
	int ret=0;
	char *databuf[]={"��ӡ����ֽ,��ֽ����ӦΪ956mm",
					 "��ӡ����ֽ,��ֽ����ӦΪ375mm",
					 "��ӡ����ֽ,��ֽ����ӦΪ375mm���Ҵ�ʱ�м��Ϊ����м��\n���Խ���"};
	ME50P_cmd_SetHVMovingUnit(0,0);
	
	cls_printf("���Դ�ӡ����ֽ��...");
	//case8.1:��׼ģʽ�£�������ֽ����Ϊ�����ֽ���룬ʵ��Ӧ��Ԥ��һ��
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_SetPrntPaper(255)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	//case8.2:������ֽ����n=100��ʵ��Ӧ��Ԥ��һ��
	if( (ret=ME50P_cmd_SetPrntPaper(100)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	
	//case8.3:���ó��������ֽ���룬Ӧȡ������.����btwrite�����bufΪchar�ͣ����ֻ��255���޷������255�����
	//case8.4:�м������Ϊ���ʱ����ֽ���벻Ӧ����Ӱ��
	if( (ret=ME50P_cmd_SetLS(255)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[2], strlen(databuf[2]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	
ERR:
	//����
	ME50P_cmd_SetPrntPaper(0);
	ME50P_cmd_SetLS(0);
	return;
}

//ָ��22
//ѹ��������»���
static void test_ME50P_cmd_SetPrntFont(void)
{
	int ret=0, i=0, nKeyin=0;
	char prntdata[MAXDISPBUFSIZE]="1234567890123456789";//��ӡ����ѡ�񲻺���,���һ��,����ο�test_ME50P_cmd_SetR90DC������ӡ����
	int mode[4]={1,49,0,48};
	
	cls_printf("�������ô�ӡ������...");
	//case9.1:�������������С
	for(i=0; i<4; i++)
	{
		if( (ret=ME50P_cmd_SetPrntFont(mode[i])) != NDK_OK )
		{
			cls_show_msg("line %d:����ָ��ʧ��(i=%d, ret=%d)", __LINE__, i, ret);
			goto ERR;
		}
		if( (ret=ME50P_cmd_Prnt(prntdata, strlen(prntdata))) != NDK_OK )		
		{
			cls_show_msg("line %d:����ָ��ʧ��(i=%d, ret=%d)", __LINE__, i, ret);
			goto ERR;
		}
		if(i==0 || i==1)
			nKeyin=cls_show_msg("�Ƿ��ӡ��ѹ��ASCII��������Ϊ%s,��[ENTER],��[����]", prntdata);
		else
			nKeyin=cls_show_msg("�Ƿ��ӡ����׼ASCII��������Ϊ%s,��[ENTER],��[����]", prntdata);  
		if(nKeyin != ENTER)
		{
			cls_show_msg("line %d:����ʧ��(i=%d)", __LINE__, i);
			ME50P_cmd_SetPrntFont(0);
			return;
		}
	}

	//case9.2:�쳣����,�������n��Χ�е���ֵ��Ӧ�������һ�����õ��ַ�����
	if( (ret=ME50P_cmd_SetPrntFont(2)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(prntdata, strlen(prntdata))) != NDK_OK )		
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if(cls_show_msg("�Ƿ��ӡ����׼ASCII��������Ϊ%s,��[ENTER],��[����]", prntdata) != ENTER)
	{
		cls_show_msg("line %d:����ʧ��", __LINE__);
		ME50P_cmd_SetPrntFont(0);
		return;
	}
	
ERR:	
	ME50P_cmd_SetPrntFont(0);
	return;
}

//ָ��24
//1.n=48��Ӧȡ����ת����ʵ�ʲ�û��ȡ��2.nΪ�Ƿ�Χ�ڵ�ֵ������ת90��3.��������ת�ķ����������һ��
static void test_ME50P_cmd_SetR90DC(void)
{
	int ret=0, i=0;
	char *databuf[]={"��ӡ������12345678ABC˳ʱ����ת90��",
					  "��ӡ������ABC87654321ȡ��˳ʱ����ת90��",
					  "��ӡ������12345678abc˳ʱ����ת90�������»���",
					  "��ӡ������abc87654321˳ʱ����ת90���ұ��߱���,�ҷ���������������ת�ķ����෴"};
	char mode[4]={1,49,0,48};
	
	cls_printf("����˳ʱ����ת90����...");
	//���ڱ�׼ģʽ����Ч��
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK ) 
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	
	//case10.1:��������
	for(i=0; i<4; i++)
	{
		if( (ret=ME50P_cmd_SetR90DC(mode[i])) != NDK_OK )
		{
			cls_show_msg("line %d:����ָ��ʧ��(i=%d, ret=%d)", __LINE__, i, ret);
			goto ERR;
		}
		if( (ret=ME50P_cmd_Prnt(databuf[i/2], strlen(databuf[i/2]))) != NDK_OK )		
		{
			cls_show_msg("line %d:����ָ��ʧ��(i=%d, ret=%d)", __LINE__, i, ret);
			goto ERR;
		}
	}
	//case10.2:�쳣����,�������n��Χ�е���ֵ��Ӧ�������һ�����õ�ȡ��90����ת
	if( (ret=ME50P_cmd_SetR90DC(50)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;//ʵ��Ϊ����˳ʱ����ת
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )		
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	//case10.3:��ѡ���»���ģʽʱ,�»��߲���˳ʱ��ѡ��90��,��ӡʱ�����»���
	if( (ret=ME50P_cmd_SetCUnderlineMode(1)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_SetR90DC(1)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetCUnderlineMode(0);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[2], strlen(databuf[2]))) != NDK_OK )		
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetCUnderlineMode(0);
		goto ERR;
	}
	//ȡ���»���	
	if( (ret=ME50P_cmd_SetCUnderlineMode(0)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetCUnderlineMode(0);
		goto ERR;
	}

	//���ߺͱ���  Ҫ�ֱ����GS ! n (0x1D 0x21 n)  ָ�����ú���в���,ʵ��Ч�����ñ���,��ת90�Ⱥ��ɱ���
	
	//case10.4:˳ʱ����ת90��ģʽ�µı��ߺͱ���������ģʽ�µķ����෴��  //ʵ��Ч����û��
	if( (ret=ME50P_cmd_SetCHeightWidth(1)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[3], strlen(databuf[3]))) != NDK_OK )		
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetCHeightWidth(0);
		goto ERR;
	}
	//ȡ�����߱���ģʽ	
	if( (ret=ME50P_cmd_SetCHeightWidth(0)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetCHeightWidth(0);
		goto ERR;
	}

ERR:
	ME50P_cmd_SetR90DC(0);//ȡ��˳ʱ����ת90��
	ME50P_cmd_SetCUnderlineMode(0);
	return;
}

//ָ��26
//n=49��Ӧ�м����,n=50��Ӧ�Ҷ���,��ʵ���������;nΪ�Ƿ�Χ�ڵ�ֵ���������
static void test_ME50P_cmd_SetAlignment(void)
{
	int ret=0, i=0;
	char *databuf[]={"��ӡ������1234567890\n���м���뷽ʽ��ӡ",
					 "��ӡ������0987654321\n���Ҷ��뷽ʽ��ӡ",
					 "��ӡ������1234567890\n������뷽ʽ��ӡ"};
	int mode[6]={1,49,2,50,0,48};

	cls_printf("���������ж�����...");
	//���ڱ�׼ģʽ����Ч��
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK ) 
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	
	//case11.1:��������
	for(i=0; i<6; i++)
	{
		if( (ret=ME50P_cmd_SetAlignment(mode[i])) != NDK_OK )
		{
			cls_show_msg("line %d:����ָ��ʧ��(i=%d, ret=%d)", __LINE__, i, ret);
			goto ERR;
		}
		if((ret=ME50P_cmd_Prnt(databuf[i/2], strlen(databuf[i/2]))) != NDK_OK)
		{
			cls_show_msg("line %d:����ָ��ʧ��(i=%d, ret=%d)", __LINE__, i, ret);
			goto ERR;
		}
	}
	//case11.2:�쳣����,�������n��Χ�е���ֵ��Ӧ�������һ�����õĶ��뷽ʽ�����
	if( (ret=ME50P_cmd_SetAlignment(3)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[2], strlen(databuf[2]))) != NDK_OK )		
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}	

ERR:
	ME50P_cmd_SetAlignment(0);	
	return;
}

//ָ��33
//n�����λΪ0ʱԤ��Ӧȡ�����ã�ʵ�ʲ�û��ȡ��,����n=0��ȡ��
static void test_ME50P_cmd_SetInvertedPrntMode(void)
{
	int ret=0;
	char *databuf[]={"��ӡ������1234567890�ǵ��ô�ӡ",
					 "��ӡ������0987654321ȡ�����ô�ӡ"};

	cls_printf("���Դ�ӡ������...");
	//���ڱ�׼ģʽ����Ч��
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK ) 
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	
	//case12.1:�������ԣ����ô�ӡ
	if( (ret=ME50P_cmd_SetInvertedPrntMode(255)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )		
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	
	//case12.2:ȡ�����ô�ӡ
	if( (ret=ME50P_cmd_SetInvertedPrntMode(0)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )		
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	//case12.3:nȡֵ������Χ����Ӧ���ܷ�.����btwrite�����bufΪchar�ͣ����ֻ��255���޷������255�����

ERR:
	ME50P_cmd_SetInvertedPrntMode(0);
	return;
}
//ָ��36
static void test_ME50P_cmd_SetFontSize(void)
{
	int ret=0, i=0;
	char *databuf[]={"��ӡ12345678abcΪ2����2����,���Ҽ��Ҳ��֮�Ŵ�",
					 "��ӡ87654321abcΪ2����������,���Ҽ��Ҳ��֮�Ŵ�",
					 "��ӡ12345678abcΪ������2����,�Ҽ���޷Ŵ�",
					 "��ӡ87654321abcΪ������������",
				     "��ӡ12345678abcΪ2����������,��˳ʱ����ת90��",
				     "��ӡ87654321abcΪ������2����,��˳ʱ����ת90��",};
	char mode[4]={0x11,0x10,0x01,0x00};
	
	cls_printf("�������ô�ӡ�����С��...");
	//case13.1:���ַ��Ŵ�ʱ���Ҽ����֮�Ŵ���ͬ�ı���
	for(i=0;i<4;i++)
	{
		if( (ret=ME50P_cmd_SetFontSize(mode[i])) != NDK_OK )
		{
			cls_show_msg("line %d:����ָ��ʧ��(i=%d, ret=%d)", __LINE__, i, ret);
			goto ERR;
		}
		if( (ret=ME50P_cmd_Prnt(databuf[i], strlen(databuf[i]))) != NDK_OK )
		{
			cls_show_msg("line %d:����ָ��ʧ��(i=%d, ret=%d)", __LINE__, i, ret);
			goto ERR;
		}
	}
	//case13.2:�쳣����,�������n��Χ�е���ֵ�����������.����btwrite�����bufΪchar�ͣ����ֻ��255���޷������255�����

	//case13.3:˳ʱ��ѡ��90��ʱ�����������ߵ���
	if( (ret=ME50P_cmd_SetR90DC(1)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_SetFontSize(0x10)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[4], strlen(databuf[4]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_SetFontSize(0x01)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[5], strlen(databuf[5]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	//����
ERR:
	ME50P_cmd_SetFontSize(0x00);
	ME50P_cmd_SetR90DC(0);
	return;
}

//ָ��39
//�����Ҽ��󣬼��Ԥ��Ӧ���ԣ�ʵ��δ���ԡ�
static void test_ME50P_cmd_SetReShowMode(void)
{
	int ret=0;
	char *databuf[]={"��ӡ������12345678abcӦ�ڰ׷���",
					 "��ӡ������87654321abcΪ�ڰ׷���,���»���",
					 "��ӡ������12345678abcȡ������",
					 "��ӡ������87654321abc�޺ڰ׷���,���»���",
					 "��ӡ������12345678abc�Ҽ��Ӵ�,�ַ��ͼ�඼�ڰ׷���",
					 "��ӡ������87654321abc�ַ�ǰ�հ״���Ӧ�ڰ׷���\n\n��һ�пհ��в�Ӧ�÷���"};

	cls_printf("�������ô�ӡ�����С��...");
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK ) 
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	//case14.1:n���λΪ1ʱ,��ʾѡ��ڰ׷��Դ�ӡģʽ
	if( (ret=ME50P_cmd_SetReShowMode(255)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	//case14.2:�ںڰ׷��Դ�ӡģʽ�£������»��ߣ��»���Ӧ�ò�������
	if( (ret=ME50P_cmd_SetCUnderlineMode(1)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	
	//case14.6:�ںڰ׷��Դ�ӡģʽ�£������ַ���࣬�ַ����ҲӦ���ԡ�
	if( (ret=ME50P_cmd_SetSpaceRight(100)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[4], strlen(databuf[4]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetSpaceRight(0);
		goto ERR;
	}
	if( (ret=ME50P_cmd_SetSpaceRight(0)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetSpaceRight(0);
		goto ERR;
	}
	
	//case14.7:�ںڰ׷��Դ�ӡģʽ�£�HT���ã��հײ��ֲ��᷵�ԡ�
	if( (ret=ME50P_cmd_SetHT()) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[5], strlen(databuf[5]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	
	//case14.3:n���λΪ0ʱ,��ʾȡ���ڰ׷��Դ�ӡģʽ
	if( (ret=ME50P_cmd_SetReShowMode(0)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[2], strlen(databuf[2]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	
	//case14.4:��ȡ���ڰ׷��Դ�ӡģʽ�£������»��ߣ��»���Ӧ��������
	if( (ret=ME50P_cmd_SetCUnderlineMode(2)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[3], strlen(databuf[3]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	//case14.5:��n����0~255��Χ�ڣ���Ӧ�ܷ�.����btwrite�����bufΪchar�ͣ����ֻ��255���޷������255�����

	//����
ERR:
	ME50P_cmd_SetReShowMode(0);//ȡ���ڰ׷���ģʽ
	ME50P_cmd_SetCUnderlineMode(0);//ȡ���»���ģʽ
	return;
}

//ָ��41, �ֲ�������ֻ����nL�ֽ�
//�ı���������ƶ���λ��Ԥ��Ӧ��Ӱ�쵱ǰ��߾࣬��ʵ����Ӱ��
static void test_ME50P_cmd_SetLeftMargin(void)
{
	int ret=0;
	char *databuf[]={"�������ݵ���߾�Ϊ32.8mm",
					 "�ı���������ƶ���λ����Ӱ�쵱ǰ��߾�"};

	cls_printf("������߾���...");
	ME50P_cmd_SetHVMovingUnit(0,0);
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK ) 
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	//case15.1:������߾�
 	if( (ret=ME50P_cmd_SetLeftMargin(255,255)) != NDK_OK ) //255/200Ӣ��ô
 	{
 		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
 		goto ERR;
 	}
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
 		goto ERR;
	}

	//case15.3:�ı���������ƶ���λ����Ӱ�쵱ǰ��߾�
	if( (ret=ME50P_cmd_SetHVMovingUnit(255,255)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
 		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetHVMovingUnit(0,0);
 		goto ERR;
	}
	//case15.2:���ó��������ô�ӡ��ȣ�ȡ�����ô�ӡ���.����btwrite�����bufΪchar�ͣ����ֻ��255���޷������255�����
	
	
	//���Ժ���
ERR:
	ME50P_cmd_SetLeftMargin(0,0);
	return;
}

//ָ��42,Ĭ��x=1,y=0.5
//Ϊ�β���仯
static void test_ME50P_cmd_SetHVMovingUnit(void)
{
	int ret=0;
	char *databuf[]={"���Ժ���������ƶ���λ��...",
					 "�����ݺ������Ϊ1/8mm,��������ԼΪ1/7mm,���Ҽ��,����ʹ�ӡ���򲻱�",
					 "�����ݵ���,�����������Ϊ1/8mm,���������Ϊ1/7mm",
					 "������˳ʱ����ת90��,�����������Ϊ1/8mm,���������Ϊ1/7mm"};	
	
	cls_printf("���Ժ���������ƶ���λ��...");
	//�ָ���Ĭ��
	ME50P_cmd_SetHVMovingUnit(0,0);
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	
	//case16.1:�ı����������ƶ���λ�����ı䵱ǰ�Ҽ��,�Ҽ��ʹ�ӡ����
	if( (ret=ME50P_cmd_SetHVMovingUnit(1,1)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	//case16.2:���õ���,�ַ��ƶ���λҲ����ı�
	if( (ret=ME50P_cmd_SetInvertedPrntMode(1)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[2], strlen(databuf[2]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetInvertedPrntMode(0);
		goto ERR;
	}
	if( (ret=ME50P_cmd_SetInvertedPrntMode(0)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetInvertedPrntMode(0);
		goto ERR;
	}

	//case16.3:����˳ʱ����ת90��,�ַ��ƶ���λҲ����ı�
	if( (ret=ME50P_cmd_SetR90DC(1)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[3], strlen(databuf[3]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetR90DC(0);
		goto ERR;
	}	
	if( (ret=ME50P_cmd_SetR90DC(0)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetR90DC(0);
		goto ERR;
	}
	
	//�ָ���Ĭ��
ERR:	
	ME50P_cmd_SetHVMovingUnit(0,0);
	return;
}

//ָ��43
//�޷���ӡ���κ�����������Ӱ������ӡ����
static void test_ME50P_cmd_SetPrntAreaWidth(void)
{
	int ret=0;
	char *databuf[]={"���Դ�ӡ������...",
					 "���Դ�ӡ������1..."};
	
	cls_printf("���Դ�ӡ��������...");
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK ) 
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	//case:Ĭ������¿���ӡ������ʲô����
	if( (ret=ME50P_cmd_SetPrntAreaWidth(1,1)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}	

	//case17.1:��������ɴ�ӡ�������ӡ������Ϊ�ɴ�ӡ����ȼ�ȥ��߾�
	if( (ret=ME50P_cmd_SetPrntAreaWidth(255,255)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}	
	//case:�ı����������ƶ���λ��Ӱ�쵱ǰ��߾��������
	return;
}

//ָ��53
//ֻ��n=0��ʱ��Ż��ӡ���»���
static void test_ME50P_cmd_SetCUnderlineMode(void)
{
	int ret=0, i=0;
	char n[6]={2, 50, 1, 49, 0, 48};
	char *databuf[]={"��ӡ������12345678abc������С���»���Ϊ2���",
					 "��ӡ������abc87654321������С���»���Ϊ1���",
					 "��ӡ������12345678abc���߱������»���Ϊ1���",
					 "��ӡ������abc87654321������С���»���Ϊ1�����HT����Ŀո�û���»���",
					 "��ӡ������12345678abc������С���»���Ϊ1������Ҽ��Ҳ���»���",
					 "��ӡ������12345678abc������С�����»���"};
	
	cls_printf("���Դ�ӡ�»�����...");
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK ) 
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	
	//case18.1:�����»���(2����1���)
	for(i=0;i<4;i++)
	{
		if( (ret=ME50P_cmd_SetCUnderlineMode(n[i])) != NDK_OK )
		{
			cls_show_msg("line %d:����ָ��ʧ��(i=%d, ret=%d)", __LINE__,i, ret);
			goto ERR;
		}
		if( (ret=ME50P_cmd_Prnt(databuf[i/2], strlen(databuf[i/2]))) != NDK_OK )
		{
			cls_show_msg("line %d:����ָ��ʧ��(i=%d, ret=%d)", __LINE__,i, ret);
			goto ERR;
		}	
	}

	//case18.2:�ı������С���»����߿�Ӧ�ò���
	if( (ret=ME50P_cmd_SetFontSize(0x11)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[2], strlen(databuf[2]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetFontSize(0x00);
		goto ERR;
	}	
	if( (ret=ME50P_cmd_SetFontSize(0x00)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetFontSize(0x00);
		goto ERR;
	}

	//case18.3:HT��������Ŀո񲻻���»���
	if( (ret=ME50P_cmd_SetHT()) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[3], strlen(databuf[3]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}

	//case18.4:���Ҽ��Ӧ��Ҳ�����»���
	if( (ret=ME50P_cmd_SetSpaceRight(20)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[4], strlen(databuf[4]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetSpaceRight(0);
		goto ERR;
	}
	if( (ret=ME50P_cmd_SetSpaceRight(0)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		ME50P_cmd_SetSpaceRight(0);
		goto ERR;
	}

	//case18.5:ȡ���»���
	for(i=4;i<6;i++)
	{
		if( (ret=ME50P_cmd_SetCUnderlineMode(n[i])) != NDK_OK )
		{
			cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		if( (ret=ME50P_cmd_Prnt(databuf[5], strlen(databuf[5]))) != NDK_OK )
		{
			cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}	
	}
ERR:
	ME50P_cmd_SetCUnderlineMode(0);//ȡ���»���
	return;
}

//ָ��57
static void test_ME50P_cmd_SetCHeightWidth(void)
{
	int ret=0;
	char *databuf[]={"��ӡ������12345678abc�����ֱ��߱���,������ĸ��Ϊ������С",
					  "��ӡ������abc87654321���ֺ�������ĸ��������С"};

	cls_printf("���Ա��߱�����...");
	//case.1:��n�����λΪ1����ʾΪѡ���ֱ��߱���ģʽ
	if( (ret=ME50P_cmd_SetCHeightWidth(1)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[0], strlen(databuf[0]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	//case.1:��n�����λΪ0����ʾΪȡ�����ֱ��߱���ģʽ��Ӧ��ӡ��������С
	if( (ret=ME50P_cmd_SetCHeightWidth(0)) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		goto ERR;
	}
	if( (ret=ME50P_cmd_Prnt(databuf[1], strlen(databuf[1]))) != NDK_OK )
	{
		cls_show_msg("line %d:����ָ��ʧ��(%d)", __LINE__, ret);
		return;
	}

ERR:
	ME50P_cmd_SetCHeightWidth(0);
	return;
}
	
void ME50P_test_function(void)
{
	/*private & local definition*/
	PF pfunc[] = {test_ME50P_cmd_SetHT, test_ME50P_cmd_SetLF, test_ME50P_cmd_SetSpaceRight, test_ME50P_cmd_SetPrntMode, 
				  test_ME50P_cmd_SetLS, test_ME50P_cmd_InitPrnt, test_ME50P_cmd_SetPrntPaper, test_ME50P_cmd_SetPrntFont,
				  test_ME50P_cmd_SetR90DC, test_ME50P_cmd_SetAlignment, test_ME50P_cmd_SetInvertedPrntMode, test_ME50P_cmd_SetFontSize,
				  test_ME50P_cmd_SetReShowMode, test_ME50P_cmd_SetLeftMargin, test_ME50P_cmd_SetHVMovingUnit, /*test_ME50P_SetPrntAreaWidth,*/
				  test_ME50P_cmd_SetCUnderlineMode, test_ME50P_cmd_SetCHeightWidth};
	int id = 0;
	
	/*process body*/
	ME50P_cmd_SetPrntMode(2);
	if(initflag!=1)
	{
		cls_show_msg("δ���ò���������������");
		return;
	}
	for(id=0; id<SZ_ARRAY(pfunc); id++)
		pfunc[id]();
	
	cls_show_msg("����ͨ��");
	return;
}

void testpress_ME50P_testpage(void) //��ӡ����ҳ
{
	/*private & local definition*/
	char buf[1024] = {0};
	int i = 0, ret = 0, j=0;
	int mode[3]={0x01,0x10,0x11};

	/*process body*/
	memset(buf,0,sizeof(buf));
	for (i=0;i<sizeof(buf);i++)
	{
		if(!(buf[i]=rand()%256))
			break;
	}
	
	ME50P_cmd_Prnt("---���´�ӡ������С����ַ�---\n" , strlen("---���´�ӡ������С����ַ�---\n"));
	//case1:����������С����ӡ�����
	if( (ret=ME50P_cmd_SetPrntMode(0x00)) != NDK_OK )
	{
		cls_show_msg("line %d:��ӡ����ҳʧ��(%d)", __LINE__, ret);
		return;
	}
	if( (ret=ME50P_cmd_Prnt(buf , strlen(buf))) != NDK_OK ) 	
	{
		cls_show_msg("line %d:��ӡ����ҳʧ��(%d)", __LINE__, ret);
		return;
	}
	
	for(j=0;j<3;j++)
	{
		memset(buf,0,sizeof(buf));
		for (i=0;i<sizeof(buf);i++)
		{
			if(!(buf[i]=rand()%256))
				break;
		}
		//case2:������������ģʽ/���ñ���������ģʽ/���ñ��߱���ģʽ,��ӡ�����
		if(j==0)
			ME50P_cmd_Prnt("\n\n---���´�ӡ�������ߴ�С����ַ�---\n" , strlen("\n\n---���´�ӡ�������ߴ�С����ַ�---\n"));
		else if(j==1)
			ME50P_cmd_Prnt("\n\n---���´�ӡ���������ߴ�С����ַ�---\n" , strlen("\n\n---���´�ӡ���������ߴ�С����ַ�---\n"));
		else
			ME50P_cmd_Prnt("\n\n---���´�ӡ���߱����С����ַ�---\n" , strlen("\n\nk---���´�ӡ���߱����С����ַ�---\n"));
		if( (ret=ME50P_cmd_SetFontSize(mode[j])) != NDK_OK )
		{
			cls_show_msg("line %d:��ӡ����ҳʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		if( (ret=ME50P_cmd_Prnt(buf , strlen(buf))) != NDK_OK )		
		{
			cls_show_msg("line %d:��ӡ����ҳʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
	}
	
ERR:
	ME50P_cmd_SetFontSize(0x00);
	return;
}

void testpress_ME50P_prntguo(void) //��ӡ����ҳ���۲��Ƿ�ֽ���Ƿ����
{
	/*private & local definition*/
	char data[1]={0x0a};
	int i = 0, ret = 0;

	/*process body*/
	ME50P_cmd_SetPrntMode(0x00);
	ME50P_cmd_Prnt("---���´�ӡ240��\"��\"---\n" , strlen("---���´�ӡ240��\"��\"---\n"));
	NDK_BTMasterClrBuf(bt_mac);
	for (i=0;i<240;i++) //Ҫ�ȺܾòŴ�ӡ?
	{
		if( (ret=NDK_BTMasterWrite(bt_mac, strlen("��"), "��" )) != NDK_OK ) 	
		{
			cls_show_msg("line %d:��ӡ\"��\"��ҳʧ��!(%d)", __LINE__, ret);
			return;
		}
	}
	NDK_BTMasterWrite(bt_mac, 1, data);
	
	return;
}

void testpress_ME50P_prntbill(void) //��ӡƱ��
{
	/*private & local definition*/
	int i = 0;
    char billbuf[35][1024]={"����POSǩ����\n",
					        "--------------------------------\n",
					        "�ֿ��˴��         CUSTOMER COPY\n",
					        "--------------------------------\n",
					        "�̻�����(MERCHANT Name):\n  �����̻�����\n",
					        "�̻���(MERCHANT NO.):\n  123456789012345\n",
					        "�ն˱��(TERMINAL NO.):12345678\n",
					        "����Ա��(Operator NO.):01\n",
					        "--------------------------------\n",
					        "�յ���(ACQUIRER):\n  �յ����� 12345678\n",
					        "������(ISSUER):�������� 12345678\n",
					        "����(CARD NO.):\n",
					        "  6200123456789012345\n\n",
					        "��������(TRANS TYPE):����/SALE\n",
		 			        "��Ч��(EXP Date):2015/01\n",
					        "���κ�(BATCH  NO.):123456\n",
					        "ƾ֤��(VOUCHER  NO.):789012\n",
					        "��Ȩ��(AUTH  NO.):345678\n",
					        "���ײο���(REFER NO.):\n  901234567890\n",
					        "����/ʱ��(DATE/TIME):\n  2019/07/18  12:34:56\n",
					        "���׽��(AMOUNT):",
					        "12345678.90\n",
					        "--------------------------------\n",
					        "Ӧ�ñ�ʶ(AID)1234567890123456\n",
					        "��ע/REFERENCE\n",
					        "�����Ƿ������б�ע��Ϣ\n",
					        "�����Ƿ������б�ע��Ϣ\n",
					        "�����Ƿ������б�ע��Ϣ\n",
					        "֣������:�ñʽ���Ϊ����״̬,����Ч����!\n",
					        "--------------------------------\n",
					        "�ֿ���ǩ��:\n\n\n\n\n",
					        "����ȷ�����Ͻ��ף�ͬ�⽫����뱾���˻�\n",
					        "I ACKNOWLEDGE SATISFACTORY RECEIPT OF RELATIVE GOODS/SERVICE\n",
					        "\n\n",
					        "- - - - - - - X - - - - - - - X - - - - - - - \n\n\n\n\n\n"};

	/*process body*/
	ME50P_cmd_SetPrntMode(0x00);
	ME50P_cmd_Prnt("---���´�ӡƱ��---\n" , strlen("---���´�ӡƱ��---\n"));
	for(i=0;i<35;i++)
		ME50P_cmd_Prnt(billbuf[i] , strlen(billbuf[i]));
	
	return;
}

void testpress_ME50P_prntenter(void) //��ӡ�س�
{
	/*private & local definition*/
	int i=0;

	/*process body*/
	ME50P_cmd_SetPrntMode(0x00);
	
	ME50P_cmd_Prnt("---����������ӡ5���س�--\n" , strlen("---����������ӡ5���س�--\n"));
	for(i=0;i<5;i++)
		ME50P_cmd_SetLF();
	
	ME50P_cmd_Prnt("---����������ӡ20���س�--\n" , strlen("---����������ӡ20���س�--\n"));
	for(i=0;i<20;i++)
		ME50P_cmd_SetLF();
	
	ME50P_cmd_Prnt("----������ӡ�س�����----\n" , strlen("----������ӡ�س�����----\n"));
	
	return;
}

void testpress_ME50P_prntcompress(void) //��ӡѹ��,��ż�հ���,��ż�ı���,��ż�س���
{
	/*private & local definition*/
	int i=0, ret=0 ;
	char data[1]={0x0a};
	
	/*process body*/
	//��ż�հ���
	ME50P_cmd_SetPrntMode(0x01);
	ME50P_cmd_Prnt("---���´�ӡѹ��Ч��---\n" , strlen("---���´�ӡѹ��Ч��---\n"));
	
	ME50P_cmd_Prnt("          \r" , strlen("          \r"));//����һ��
	cls_printf("\n���ڴ�ӡ,���Ե�\n��ż��\n");
	for (i=1;i<=24;i++)
	{
		if( (ret=NDK_BTMasterWrite(bt_mac, strlen("��"), "��" )) != NDK_OK ) 	
		{
			cls_show_msg("line %d:��ӡ\"��\"��ҳʧ��!(%d)", __LINE__, ret);
			return;
		}
	}
	NDK_BTMasterWrite(bt_mac, 1, data);

	ME50P_cmd_Prnt("          \r" , strlen("          \r"));
	ME50P_cmd_Prnt("          \r" , strlen("          \r"));//���ж���
	for (i=1;i<=24;i++)
	{
		if( (ret=NDK_BTMasterWrite(bt_mac, strlen("��"), "��" )) != NDK_OK )	
		{
			cls_show_msg("line %d:��ӡ\"��\"��ҳʧ��!(%d)", __LINE__, ret);
			return;
		}
	}
	NDK_BTMasterWrite(bt_mac, 1, data);
	
	ME50P_cmd_Prnt("          \r" , strlen("          \r"));
	ME50P_cmd_Prnt("          \r" , strlen("          \r"));
	ME50P_cmd_Prnt("          \r" , strlen("          \r"));//��������
	for (i=1;i<=24;i++)
	{
		if( (ret=NDK_BTMasterWrite(bt_mac, strlen("��"), "��" )) != NDK_OK )	
		{
			cls_show_msg("line %d:��ӡ\"��\"��ҳʧ��!(%d)", __LINE__, ret);
			return;
		}
	}
	NDK_BTMasterWrite(bt_mac, 1, data);

	ME50P_cmd_Prnt("\n��ӡǰ��ż�հ���\n" , strlen("\n��ӡǰ��ż�հ���\n"));//��ʾ

	//��ż�ı���
	ME50P_cmd_Prnt("�м���ż�ո�\n" , strlen("�м���ż�ո�\n"));
	ME50P_cmd_Prnt("��  ��   ��������  ���� \r" , strlen("��  ��   ��������  ���� \r"));
	ME50P_cmd_Prnt(" �� ��  ������  ���� \r", strlen(" �� ��  ������  ���� \r"));

	//��ż�س���
	ME50P_cmd_Prnt("\r" , strlen("\r"));
	for (i=1;i<=24;i++)
	{
		if( (ret=NDK_BTMasterWrite(bt_mac, strlen("��"), "��" )) != NDK_OK )	
		{
			cls_show_msg("line %d:��ӡ\"��\"��ҳʧ��!(%d)", __LINE__, ret);
			return;
		}
	}
	NDK_BTMasterWrite(bt_mac, 1, data);
	
	ME50P_cmd_Prnt("��ӡǰһ���س���\n" , strlen("��ӡǰһ���س���\n"));//��ʾ

	ME50P_cmd_Prnt("\r\r\r" , strlen("\r\r\r"));
	for (i=1;i<=24;i++)
	{
		if( (ret=NDK_BTMasterWrite(bt_mac, strlen("��"), "��" )) != NDK_OK )	
		{
			cls_show_msg("line %d:��ӡ\"��\"��ҳʧ��!(%d)", __LINE__, ret);
			return;
		}
	}
	NDK_BTMasterWrite(bt_mac, 1, data);
	ME50P_cmd_Prnt("��ӡǰ�����س���\n" , strlen("��ӡǰ�����س���\n"));//��ʾ

	ME50P_cmd_Prnt("\r\r\r\r" , strlen("\r\r\r\r"));
	for (i=1;i<=24;i++)
	{
		if( (ret=NDK_BTMasterWrite(bt_mac, strlen("��"), "��" )) != NDK_OK )	
		{
			cls_show_msg("line %d:��ӡ\"��\"��ҳʧ��!(%d)", __LINE__, ret);
			return;
		}
	}
	NDK_BTMasterWrite(bt_mac, 1, data);
	ME50P_cmd_Prnt("��ӡǰ�ĸ��س���\n" , strlen("��ӡǰ�ĸ��س���\n"));//��ʾ
	
	return;
}

void testpress_ME50P_prntlandi(void) //��ӡ����
{
	int i=0;
	char databuf[12][1024]={"���ݵ�أ�3.02V\n",
						    "WELCOME TO POS EQUIPMENT\n",
						    "��������������������������������������������\n",
						    "��������������������������������������������\n",
						    "��λ��ӡЧ����\n",
						    "��λ4(#2)\n",
						    "||  ||  ||  ||  ||\n",
						    "||  ||  ||  ||  ||\n",
						    "||  ||  ||  ||  ||\n",
						    "�������Ч��\n",
						    " 1 2 3 4 5 6 7 8 9 \n",
						    "--------------------------------------------------------------------\n"};
	
	ME50P_cmd_SetPrntMode(0x00);
	for(i=0;i<12;i++)
		ME50P_cmd_Prnt(databuf[i] , strlen(databuf[i]));
	
	return;
}

void testpress_ME50P_prntxinguodu(void) //��ӡ�¹���
{
	int i=0;
	char databuf[12][1024]={"\r\r",
						    "�����в�ɽ�����Է��ε�\r\r",
						    "            309350156911410\r",
						    "       11100495         01\r\r",
						    "   622200*********7950\r\r",
						    "     01020000    ��ҵ����\r\r",
						    "   ����/SALE      2000/00\r\r",
						    "   000003          000934\r\r",
						    "   012345\r",
						    "   151019019346\r",
						    "   RMB 0.01\r\r",
						    "   RMB 0.01"};

	ME50P_cmd_SetPrntMode(0x00);
	for(i=0;i<12;i++)
		ME50P_cmd_Prnt(databuf[i] , strlen(databuf[i]));
	
	return;
}

void ME50P_test_press(void)
{
	/*private & local definition*/
#define	DEFAULT_CNT_STR	"1"//"200"	//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(200)
	int cnt = DEFAULT_CNT_VLE, bak = 0, ret = 0;
	uint len = 0;
	char CntStr[8] = DEFAULT_CNT_STR;

	//���ǿ�/��䵥/������/�հ׵�/ͼ�δ�ӡ��Ҫ��ӡͼ�Σ���֧��
	//��ӡ����ҳ�������������ҳ��Ʊ�ݡ�ģʽ������ϡ��س���ѹ�������ϵ��ݡ��¹�������
	PF pfunc[] = {testpress_ME50P_testpage, testpress_ME50P_prntguo, testpress_ME50P_prntbill, testpress_ME50P_prntenter, 
				  testpress_ME50P_prntcompress, testpress_ME50P_prntlandi, testpress_ME50P_prntxinguodu};
	int id = 0;
	
	/*process body*/
	cls_printf("Ĭ�ϲ��Դ���:\n");
	if ((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &len, INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	while(cnt)
	{
		if (ESC==cls_show_msg1(2, "ѹ��������...\n��ʣ%d��,��ע���ӡЧ��,ESC�жϲ���", cnt))
			break;
		cnt--;

		for(id=0; id<SZ_ARRAY(pfunc); id++)
		{
			pfunc[id]();
			if(wait_anykey(1)==ESC)
				goto OUT;
		}
	}

OUT:
	cls_show_msg_record(FILENAME ,FUNCNAME ,"ѹ���������,��ִ�д���Ϊ%d", bak-cnt);
	return;
}

void ME50P_test_abnormal(void)
{
	/*private & local definition*/
	int nKeyin = 0;

	/*process body*/
	while(1)
	{
		nKeyin = cls_show_msg("1.��ʱ�����\n"
							  "2.�����쳣\n");
		//case1:���Գ�ʱ�����������ӡ����Ȼ�����ӣ����Ƿ������ӳɹ�����ӡ��
		switch (nKeyin)
		{
		case '1':
			cls_show_msg("�볤ʱ�����������ӡ����,�ٽ���������ӡ,������ӡ����POSӦ�������ӣ����Ҵ�ӡӦ������");
 			break;
		case '2':
			cls_show_msg("��ȷ�����������߹��ܣ�POS���߻��Ѻ���������ӡ��Ӧ�������ӣ��Ҵ�ӡӦ������");
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

void init_connect(int conn_type)
{
	/*private & local definition*/
	int ret = -1;
	char btmac[7] = {0};
	
	if(comm_type == PORT_NUM_BT)
	{
		//����ǰ��
		cls_show_msg("���������ӡ����,���������");
		NDK_PortClose(PORT_NUM_BT);
		if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
		{
			cls_show_msg("line %d:����������ʧ��(%d)", __LINE__, ret);
			return ;
		}
		//ɨ���ȡmac
		if((ret=lib_btmaster_scan(btmac, conn_type))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸ɨ���ȡmacʧ��(%d)", __LINE__, ret);
			goto ERR;	
		}
		//����(���豸��ʾ����)
		if((ret=NDK_BTMasterConnect(btmac,conn_type))!=NDK_OK)
		{
			cls_show_msg("line %d:���豸���Ӵ��豸ʧ��(%d)", __LINE__, ret);
			goto ERR;	
		}
		//δ��ԵĻ�������,����Եľ�ֱ��������
		if((ret=connect_pair_process(btmac,1))!=NDK_OK)
		{
			cls_show_msg("line %d:�����豸�������ʧ��(%d)", __LINE__, ret);
			goto ERR;
		}
		memcpy(bt_mac, btmac, 8);
	}
	
	initflag=1;
	return;
	
ERR:
	//�رմ���
	NDK_PortClose(comm_type);
	return;
}

void bt_set(void)
{
	int keyin=0;
	int conn_type = 0;

	/*process body*/
	while(1)
	{
		keyin = cls_show_msg("��ѡ�����ӷ�ʽ:\n"
							 "1.Ĭ�Ϸ�ʽ\n"
							 "2.SPP��ʽ\n" 
							 "3.BLE ��ʽ");
		switch(keyin) 
		{
			case '1':
			case '2':
			case '3':
				conn_type = keyin-'1';
				init_connect(conn_type);
				return;
				break;
			case ESC:
				return;
				break;
			default:
				cls_show_msg1(3,"��ѡ����ȷ���������ӷ�ʽ���˳��밴ȡ����");
				break;
		}
	}
	return;
}

void comm_set(void)
{
	int nKeyin=0;

	/*process body*/
	while(1)
	{
		nKeyin = cls_show_msg("��ѡ��ͨѶ��ʽ:"
							  "1.����ͨѶ\n");
		switch(nKeyin) 
		{
			case '1':
				comm_type = PORT_NUM_BT;
				bt_set();				
				return;
				break;
			case ESC:
				return;
				break;
			default:
				cls_show_msg1(3,"��ѡ����ȷ��ͨѶ��ʽ���˳��밴ȡ����");
				break;
		}
	}	
	return;
}

void systest90(void)
{
	if(auto_flag == 1)
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}
	
	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);
	
	setlinktype(BT);//����ͨѶ����ΪBT���������ݰ�̫������ʧ��
	while(1)
	{
		int nKeyin = -1;
		nKeyin=cls_show_msg("1.���� 2.ѹ��\n"
							"3.�쳣\n"
							"0.����\n");
		switch(nKeyin)
		{
			case '1':
				ME50P_test_function();
				break;		
			case '2':
				ME50P_test_press();
				break;		
			case '3':
				ME50P_test_abnormal();
				break;		
			case '0':
				comm_set();
				break;
			case ESC:
				NDK_PortClose(comm_type);
				return;
			default:
				continue;
				break;
		}
	}
	return;
}


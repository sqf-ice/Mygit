/************************************************************************
*
* copyright(c)2005-2012 Newland Co. Ltd. All rights reserved
* module			: ��
* file name		:
* Author 			: 
* version			: V1.0
* DATE			: 
* directory 		: 
* description		: MEMORY������
* related document	: ����Ա�ο��ֲ�
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h" 

/*---------------constants/macro definition---------------------*/
#define	TESTITEM		"MEMORY������"
#define	DEFAULT_CNT_STR	"200"	
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(200)
#define ITEMPERPAGE 4
#define	DEFAULT_CNT_STR2	"50"	
#define	DEFAULT_CNT_VLE2	atoi(DEFAULT_CNT_STR2)	//(200)

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
*			  	          		   	 	    
*****************************************************************/
//ָ���װ��д����,����recbuf�ĳ���ָ���ʽ
static int rw_data(char *head,int headlen,char *data,int datalen,char *recbuf,MEMORY_t type)
{
	/*private & local definition*/
	uchar sendbuf[300] = {0},buf[300] = {0};
	int sendlen = 0,rlen = 0;
	EM_ICTYPE cardId[]={ICTYPE_M_1_1,ICTYPE_M_1_2,ICTYPE_M_1_4,ICTYPE_M_1_8,ICTYPE_M_1_16,ICTYPE_M_1_32,ICTYPE_M_1_64,
		ICTYPE_M_2,ICTYPE_M_3,ICTYPE_M_3,ICTYPE_M_4,ICTYPE_M_5,ICTYPE_M_6,ICTYPE_M_7};
    
	/*process body*/
	memcpy(sendbuf,head,headlen);
	sendbuf[headlen] = datalen&0xff;
	if(NULL == data)
		sendlen = headlen+1;
	else
	{
		memcpy(sendbuf+headlen+1,data,datalen);
		sendlen = headlen+1+datalen;
	}
	if( NDK_Iccrw(cardId[type]/*ICTYPE_IC*/,sendlen,sendbuf,&rlen,buf)!=NDK_OK)			
		return FAIL;  
	else
	{
		memcpy(recbuf,buf,rlen/*(int)buf[0]*/); 
		return rlen;
	}
}

//������:cla:һ��������ǹ̶�0x00�����������˰�ؿ��Ļ����ǲ�ȷ������ֵ�ˣ�lenΪҪ��ȡ�����ݳ���
//p1,p2���ݲ�ͬ�Ŀ��в�ͬ�����壬���Ƕ���������������recbufΪ���ص�����
static int memory_cmd_r(char cla,char p1,char p2,int len,char *recbuf,MEMORY_t type)
{
	/*private & local definition*/
	char head[300] = {0};
	
	/*process body*/
	head[0] = cla;
	head[1] = 0xB0;
	head[2] = p1;
	head[3] = p2;

	return rw_data(head,4,NULL,len,recbuf,type);
}

//д����cla:һ��������ǹ̶�0x00�����������˰�ؿ��Ļ����ǲ�ȷ������ֵ�ˣ�dataΪҪд�����ݣ�lenΪdata�ĳ���
static int memory_cmd_w(char cla,char p1,char p2,char *data,int len,char *recbuf,MEMORY_t type)
{
	/*private & local definition*/
	char head[300] = {0};

	/*process body*/
	head[0] = cla;
	head[1] = 0xD0;
	head[2] = p1;
	head[3] = p2;
	return rw_data(head,4,data,len,recbuf,type);
}

//У����Կ����:dataΪ��Կ��lenΪ��Կ����
static int memory_cmd_k(char p1,char p2,char *data,int len,char *recbuf,MEMORY_t type)
{
	/*private & local definition*/
	char head[300] = {0};

	/*process body*/
	head[0] = 0x00;
	head[1] = 0x20;
	head[2] = p1;
	head[3] = p2;
	return rw_data(head,4,data,len,recbuf,type);
}

//��֤У��:dataΪ��֤��Կ��lenΪ��֤��Կ����
static int memory_cmd_a(char p1,char p2,char *data,int len,char *recbuf,MEMORY_t type)
{
	/*private & local definition*/
	char head[300] = {0};

	/*process body*/
	head[0] = 0x00;
	head[1] = 0x82;
	head[2] = p1;
	head[3] = p2;
	return rw_data(head,4,data,len,recbuf,type);
}

//����:lenΪҪ�������ֽ�����lenΪҪ�����ĳ���
static int memory_cmd_e(char p1,char p2,int len,char *recbuf,MEMORY_t type)
{
	/*private & local definition*/
	char head[300] = {0};

	/*process body*/
	head[0] = 0x00;
	head[1] = 0x0E;
	head[2] = p1;
	head[3] = p2;
	return rw_data(head,4,NULL,len,recbuf,type);
}

//����4442���� 
static unsigned char test_4432_42_func(MEMORY_t type)
{
	/*private & local definition*/
	char keydata[] = "\xff\xff\xff";
	char writedata[] = "\x01\x02\x03\x04";
	char recbuf[300]={0};
	int reclen = 0;
	ushort sw=0;
	
	/*process body*/
	//case1:4442����д�û�������
	//step1:У����Կ
	if((reclen=memory_cmd_k(0x00,0x00,keydata, strlen(keydata),recbuf,SLE4432_42))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1(g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step2:д����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x00,0x20,writedata,strlen(writedata),recbuf,SLE4432_42))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step3:������
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x00,0x20,strlen(writedata),recbuf,SLE4432_42))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	if(memcmp(writedata,recbuf,reclen-2))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ƚ�����������ʧ��", __LINE__);
		return FAIL;
	}
	//case2:4442����д�û�����
	//step1:д����������
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x80,0x00,writedata,strlen(writedata),recbuf,SLE4432_42))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step2:������������
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x80,0x00,strlen(writedata),recbuf,SLE4432_42))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	return SUCC;
}

//1608����������
static unsigned char test_1608_func(MEMORY_t type)
{
	/*private & local definition*/
	char keydata[] = "\xFF\xFF\xFF";//\x01\x02\x03   //�����MEMORY��Ƭ ��ԿΪȫff
	char akeydata[] = "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	char writedata[] = "\x01\x02\x03\x04",recbuf[300] = {0};
	int reclen = 0;
	ushort sw=0;
	
	/*process body*/
	//case:1608���Ķ�д��������
	//step1:��֤����
	if((reclen=memory_cmd_a(0x00,0x00,akeydata,strlen(akeydata),recbuf,AT88SC1608))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step2:У����Կ
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_k(0x03,0x00,keydata,strlen(keydata),recbuf,AT88SC1608))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У���KEYʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_k(0x03,0x08,keydata,strlen(keydata),recbuf,AT88SC1608))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��дKEYʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}	
	//step3:�û�ҳ��ѡ�����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x02,0x01,NULL,0,recbuf,AT88SC1608))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��дKEYʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step4:д����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x00,0x0E,writedata,strlen(writedata),recbuf,AT88SC1608))==FAIL)//��ַ�� 0x20��Ϊ0x0E����Ϊд��16�ֽڵ�д��������ַ16֮��Ҫ��ҳ��Ҫ����д��ַ,��Ϊ����֤�Ƿ��ܹ���ȷ��ҳд���޸ĸ�������4�ֽ�����д����ʵ��ַΪ14��λ�ã���16��ʱ��ײ㷭ҳ���� jiangym 20150325 modify
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step5:������
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x01,0x0E,strlen(writedata),recbuf,AT88SC1608))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step6:�Ƚ�����
	if(memcmp(writedata,recbuf,reclen-2))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ƚ�����ʧ��", __LINE__);
		return FAIL;
	}
	return SUCC;

}

//sle4418_28����
static unsigned char test_4418_28_func(MEMORY_t type)
{
	/*private & local definition*/
	char keydata[] = "\xFF\xFF";
	char writedata[] = "\x31\x32\x33\x34",recbuf[12]={0},recbufold[12]={0};
	int reclen = 0;
	ushort sw=0;
	
	/*process body*/
	//case1:4418_28����д���ԣ�д�ޱ���λ����
	//step1:У����Կ
	if((reclen=memory_cmd_k(0x00,0x00,keydata, strlen(keydata),recbuf,SLE4418_28))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	
	//step2:�����ϵ�ԭʼ����,��������
	memset(recbufold,0,sizeof(recbufold));
	if((reclen=memory_cmd_r(0x00,0x02,0xff,4,recbufold,SLE4418_28))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbufold+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//Ϊ��֤ԭʼ���ݺͲ�������Ҫ��һ��
	if(!memcmp(recbufold,writedata,sizeof(writedata)))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:ԭʼ���ݺͲ�������һ��,���޸Ĳ�������", __LINE__);
		return FAIL;
	}
	
	//step3:д�ޱ���λ����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x02,0xff,writedata,strlen(writedata),recbuf,SLE4418_28))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	
	//step4:���ޱ���λ����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x02,0xff,strlen(writedata),recbuf,SLE4418_28))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//���ݱȽ�
	if(memcmp(writedata,recbuf,reclen-2))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ƚ�����ʧ��", __LINE__);
		return FAIL;
	}
	
	//step5:д�ؿ��ϵ�ԭʼ����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x02,0xff,recbufold,strlen(recbufold)-2,recbuf,SLE4418_28))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	return SUCC;  
}
//sle5528����
static unsigned char test_5528_func(MEMORY_t type)
{
	/*private & local definition*/
	char keydata[] = "\xff\xff";//"\xa9\xbc";  �¿���ԿΪȫffff  ֣�²�
	char writedata[] = "\x31\x32\x33\x34",recbuf[12]={0},recbufold[12]={0};
	int reclen = 0;
	ushort sw=0;
	
	/*process body*/
	//case1:5528����д���ԣ�д�ޱ���λ����
	//step1:У����Կ
	if((reclen=memory_cmd_k(0x00,0x00,keydata, strlen(keydata),recbuf,SLE5528))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(a9 bc)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	
	//step2:������ԭʼ����,��������
	memset(recbufold,0,sizeof(recbufold));
	if((reclen=memory_cmd_r(0x00,0x02,0xff,4,recbufold,SLE5528))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(a9 bc)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbufold+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//Ϊ��֤ԭʼ���ݺͲ�������Ҫ��һ��
	if(!memcmp(recbufold,writedata,sizeof(writedata)))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:ԭʼ���ݺͲ�������һ��,���޸Ĳ�������", __LINE__);
		return FAIL;
	}
	
	//step3:д�ޱ���λ����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x02,0xff,writedata,strlen(writedata),recbuf,SLE5528))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	
	//step4:���ޱ���λ����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x02,0xff,strlen(writedata),recbuf,SLE5528))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//���ݱȽ�
	if(memcmp(writedata,recbuf,reclen-2))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ƚ�����ʧ��", __LINE__);
		return FAIL;
	}
	
	//step5:д�ؿ��ϵ�ԭʼ����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x02,0xff,recbufold,strlen(recbufold)-2,recbuf,SLE5528))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	return SUCC;  
}

//AT24C64���Ĳ������  ͬ���ʺ�AT24C128��AT24C256
static unsigned char test_24C_func(MEMORY_t type)
{
	/*private & local definition*/
	char writedata[] = "\x31\x32\x33\x34",recbuf[300] = {0};
	int reclen = 0;
	ushort sw=0;
	
	/*process body*/
	//case:at24c64���Ķ�д������ͬ������(AT24C128��AT24C256)
	//д����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x00,0x01,writedata,strlen(writedata),recbuf,type))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//������
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x00,0x01,strlen(writedata),recbuf,type))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//���ݱȽ�
	if(memcmp(writedata,recbuf,reclen-2))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ƚ�ʧ��", __LINE__);
		return FAIL;
	}
	return SUCC;
}

#if 0
//AT45DB041˰�ؿ��������
static unsigned char test_at45_func(void)
{
	/*private & local definition*/
	char writedata[] = "\x31\x32\x33\x34",recbuf[300] = {0};
	int reclen = 0;
	ushort sw=0;
	
	/*process body*/
	//case:at45db041˰�ؿ��Ķ�д����
	//д����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x00,0x00,writedata,strlen(writedata),recbuf))==FAIL)
	{
		cls_show_msg1(g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1(g_keeptime, "line %d:д����ʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//������
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x00,0x00,strlen(writedata),recbuf))==FAIL)
	{
		cls_show_msg1(g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1(g_keeptime, "line %d:������ʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//���ݱȽ�
	if(memcmp(writedata,recbuf,reclen-2))
	{
		cls_show_msg1(g_keeptime, "line %d:�Ƚ�ʧ��", __LINE__);
		return FAIL;
	}
	return SUCC;

}
#endif

//����102�洢��
static unsigned char test_102_func(MEMORY_t type)
{
	/*private & local definition*/
	char writedata[] = "\x31\x32",recbuf[300] = {0};
	int reclen = 0;
	ushort sw=0;
	char keydata[] = "\xf0\xf0";//102��Ƭ��Ĭ����Կ,���Ĭ����Կ���������Կ���4�ο�Ƭ������,��������Ŀ�ƬҪ�������Կ
	
	/*process body*/
	//case:102�洢���Ķ�д����
	//step1:����������
	memset(recbuf,0,sizeof(recbuf)); 
	if((reclen=memory_cmd_e(0x00,0xB0,strlen(writedata),recbuf,AT88SC102))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step2:д����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x00,0xB0,writedata,strlen(writedata),recbuf,AT88SC102))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step3:������
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x00,0xB0,strlen(writedata),recbuf,AT88SC102))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step4:���ݱȽ�
	if(memcmp(writedata,recbuf,reclen-2))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ƚ�ʧ��", __LINE__);
		return FAIL;
	}

	//case2:102��Ƭ��Կ�޸�
	//step1:У����Կ
	if((reclen=memory_cmd_k(0x00,0x00,keydata, strlen(keydata),recbuf,AT88SC102))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(f0 f0)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1(g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step2:������Կ
	memset(recbuf,0,sizeof(recbuf)); 
	if((reclen=memory_cmd_e(0x00,0x0a,strlen(keydata),recbuf,AT88SC102))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(f0 f0)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step3:��ȡ��Կ
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x00,0x0A,strlen(keydata),recbuf,AT88SC102))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step4:�޸���Կ
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x00,0x0a,keydata,strlen(keydata),recbuf,AT88SC102))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(f0 f0)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step5:��ȡ��Կ
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x00,0x0A,strlen(keydata),recbuf,AT88SC102))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(f0 f0)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step6:���ݱȽ�
	if(memcmp(keydata,recbuf,reclen-2))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ƚ�ʧ��", __LINE__);
		return FAIL;
	}
	return SUCC;
}

//����1604���Կ�
static unsigned char test_1604_func(MEMORY_t type)
{
	/*private & local definition*/
	char writedata[] = "\x31\x32",recbuf[300] = {0};
	int reclen = 0;
	ushort sw=0;
	
	/*process body*/
	//case:1604�洢���Ķ�д����
	//step1:����������
	//����0xD4-0xD7�����ݣ���0xD6�޷�����0xD5������  20180706 modify
	memset(recbuf,0,sizeof(recbuf)); 
	if((reclen=memory_cmd_e(0x07,0xD4/*0xD6*/,4/*strlen(writedata)*/,recbuf,AT88SC1604))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:����ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step2:д����  (�µĿ�Ƭд�������������ƣ���D5�ĳ�D6  20180621 modify)
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x07,0xD5/*0xD6*/,writedata,strlen(writedata),recbuf,AT88SC1604))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step3:������
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x07,0xD5/*0xD6*/,strlen(writedata),recbuf,AT88SC1604))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step4:���ݱȽ�
	if(memcmp(writedata,recbuf,reclen-2))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ƚ�ʧ��", __LINE__);
		return FAIL;
	}
	return SUCC;
}
//����AT88SC153���Կ�
static unsigned char test_153_func(MEMORY_t type)
{
	/*private & local definition*/
	char keydata[] = "\xFF\xFF\xFF";//\x01\x02\x03
	char akeydata[] = "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	char writedata[] = "\x01\x02\x03\x04\x11\x12\x13\x14\x21\x22\x23\x24",recbuf[300] = {0};
	int reclen = 0;
	ushort sw=0;
	
	/*process body*/
	//case:153���Ķ�д��������
	//step1:��֤����(���Բ���֤)
	if((reclen=memory_cmd_a(0x00,0x00,akeydata,strlen(akeydata),recbuf,AT88SC153))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step2:У����Կ
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_k(0x0B,0x34,keydata,strlen(keydata),recbuf,AT88SC153))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У���KEYʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_k(0x03,0x30,keydata,strlen(keydata),recbuf,AT88SC153))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��дKEYʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step3:д����
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_w(0x00,0x00,0x06,writedata,strlen(writedata),recbuf,AT88SC153))==FAIL)//��ַʹ��0x06����Ϊд��8�ֽڵ�д��������ַ8��F֮��Ҫ��ҳ��Ҫ����д��ַ,Ϊ����֤�Ƿ��ܹ���ȷ��д��д��12�ֽڵ�����
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:д������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step4:������
	memset(recbuf,0,sizeof(recbuf));
	if((reclen=memory_cmd_r(0x00,0x01,0x06,strlen(writedata),recbuf,AT88SC153))==FAIL)
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��������ʧ��(ff ff)", __LINE__);
		return FAIL;
	}
	else
	{
		if((sw=SMART_GETSW(recbuf+reclen-2))!=0x9000)
		{
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:У��KEYʧ��(0x%04x)", __LINE__,sw);
			return FAIL;
		}
	}
	//step5:�Ƚ�����
	if(memcmp(writedata,recbuf,reclen-2))
	{
		cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:�Ƚ�����ʧ��", __LINE__);
		return FAIL;
	}
	return SUCC;
}

//�쳣����
void memroy_abnormal_test(MEMORY_t type)
{
	/*private & local definition*/
	uchar buf[300] = {0};
	int ret = 0,pnAtrlen = 0;
	
	char *card[] = {"AT24C01","AT24C02","AT24C04","AT24C08","AT24C016","AT24C032","AT24C064","SLE4432_42","SLE4418_28","SLE5528","AT88SC102","AT88SC1604","AT88SC1608","AT88SC153"};
	EM_ICTYPE cardId[]={ICTYPE_M_1_1,ICTYPE_M_1_2,ICTYPE_M_1_4,ICTYPE_M_1_8,ICTYPE_M_1_16,ICTYPE_M_1_32,ICTYPE_M_1_64,ICTYPE_M_2,ICTYPE_M_3,ICTYPE_M_3,ICTYPE_M_4,ICTYPE_M_5,ICTYPE_M_6,ICTYPE_M_7};
	EM_ICTYPE errcardID[]={ICTYPE_M_2,ICTYPE_M_3,ICTYPE_M_4,ICTYPE_M_5,ICTYPE_M_6,ICTYPE_M_6,ICTYPE_M_5,ICTYPE_M_1,ICTYPE_M_1,ICTYPE_M_5,ICTYPE_M_3,ICTYPE_M_6,ICTYPE_M_1,ICTYPE_M_5};//����Ŀ�����
	unsigned char (*pfunc[])(MEMORY_t type) = 
	{
		test_24C_func,
		test_24C_func,
		test_24C_func,
		test_24C_func, 
		test_24C_func,
		test_24C_func,
		test_24C_func,
		test_4432_42_func,
		test_4418_28_func,
		test_5528_func,
		test_102_func,
		test_1604_func,
		test_1608_func,
		test_153_func
	};
	
	/*process body*/
	cls_printf("�쳣���Կ�ʼ\n");

	//case1:�����ڿ�����û�п�������£��ϵ��Ƿ�ɹ�?(�ϵ�Ӧ����ʧ�ܵ�)
	cls_show_msg("�뽫%s���ӿ���������,�����������...",card[type]);
	//NDK_IccSetType(cardId[type]);
	if ((ret=NDK_IccPowerUp(cardId[type], buf,&pnAtrlen))!=NDK_ERR/*-64*/)
	{
		NDK_IccPowerDown(cardId[type]);	
		cls_show_msg1(5,"line %d:���󷵻�ֵ����ʧ��(Ԥ��:-1, ʵ��:%d)", __LINE__, ret);
		return;
	}
	cls_show_msg1(2, "����������ͨ��!");

	//case2:���Ե��ϵ�Ŀ����ʹ�ΪIC�����ͺͲ���Ŀ�Ƭ���Ͳ�һ��(�ϵ�һ��ʧ��)
	cls_show_msg("��ȷ��%s���Ѿ�����,�����������...",card[type]);
	//DK_IccSetType(ICTYPE_IC);
	if ((ret=NDK_IccPowerUp(ICTYPE_IC, buf,&pnAtrlen))==NDK_OK)
	{
		NDK_IccPowerDown(ICTYPE_IC);	
		cls_show_msg1(5,"line %d:�쳣����ʧ��,���������ϵ�ɹ�!", __LINE__);
		return;
	}
	cls_show_msg1(2, "����������ͨ��!");

	//case3:���ԵĿ����ʹ���������Ŀ�ƬΪMEM����Ӧ��ʧ��
	//��������memory�����ͣ��ϵ��п���ʧ��(����NDK_ERR_IOCTL)��Ҳ�п��ܳɹ�
	cls_show_msg("��ȷ��%s���Ѿ�����,�����������...",card[type]);
	if ((ret=NDK_IccPowerUp(errcardID[type], buf,&pnAtrlen))==NDK_OK)
	{
		//�ϵ��ܳɹ�����дҲӦ��ʧ��
		if(SUCC == pfunc[type](type))
		{
			if(type==AT88SC153)//||type==AT88SC1608 type==SLE4418_28||type==SLE5528||   ����SLE4428/SLE5528��������Ҫ�����ϸ���ϵ�ʱ�򣨼�ֻҪ����Ƭ���磩������������д��Ƭ(���ν���)20171023 modify
			{
				NDK_IccPowerDown(errcardID[type]);
				cls_show_msg1(5, "line %d:��д����Ӧ��ʧ��", __LINE__);
				return;
			}
			else
				;//��������AT24CXX��SLE4432_42��AT88SC102��AT88SC1604��AT88SC1608���ϵ�ֻ��Ҫ����vcc���߾ͺã���ʹ���κ�����MEM�������ϵ��д���ɹ�,���102���ڶ�д�ɹ�����²�����
		}
		NDK_IccPowerDown(errcardID[type]);
	}
	cls_show_msg1(2, "����������ͨ��!");

	//����������������ʧ������Ƭ�����������н���һ���������µ��д����
	NDK_IccPowerUp(cardId[type], buf,&pnAtrlen);
	pfunc[type](type);
	NDK_IccPowerDown(cardId[type]);
	
#if !defined ME30GX //��оû��8035оƬû�а취��⵽�β忨���� �ʲ�֧�� 20170726 ���ν���
	//case4:�����ϵ�󣬰β�memory�������ж�д����(Ӧ��ʧ��)
	cls_show_msg("��ȷ��%s���Ѿ�����,�����������...",card[type]);
	//NDK_IccSetType(cardId[type]);
	if ((ret=NDK_IccPowerUp(cardId[type], buf,&pnAtrlen))!=NDK_OK)
	{
		NDK_IccPowerDown(cardId[type]);	
		cls_show_msg1(5,"line %d:�ϵ�ʧ��!", __LINE__);
		return;
	}
	cls_show_msg("�β�%s��,�����������...",card[type]);
	//step:��д����֤����Ȳ���
	if(SUCC == pfunc[type](type))
	{ 
		NDK_IccPowerDown(cardId[type]);
		cls_show_msg1(5, "line %d:��д����ʧ��", __LINE__);
		return;
	}
	NDK_IccPowerDown(cardId[type]);
	cls_show_msg1(2, "����������ͨ��");
#endif	
	//����������������ʧ������Ƭ�����������н���һ���������µ��д����
	NDK_IccPowerUp(cardId[type], buf,&pnAtrlen);
	pfunc[type](type);
	NDK_IccPowerDown(cardId[type]);

 	cls_show_msg("�쳣���Խ���!��ִ����������");
	return;
}

//��дѹ������
void memory_rw_press(MEMORY_t type)
{
	/*private & local definition*/
	int ret = 0, succ = 0, cnt = DEFAULT_CNT_VLE, bak = 0,pnAtrlen = 0;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	uchar buf[300] = {0};
	char *card[] = {"AT24C01","AT24C02","AT24C04","AT24C08","AT24C016","AT24C032","AT24C064","SLE4432_42","SLE4418_28","SLE5528","AT88SC102","AT88SC1604","AT88SC1608","AT88SC153"};
	EM_ICTYPE cardId[]={ICTYPE_M_1_1,ICTYPE_M_1_2,ICTYPE_M_1_4,ICTYPE_M_1_8,ICTYPE_M_1_16,ICTYPE_M_1_32,ICTYPE_M_1_64,ICTYPE_M_2,ICTYPE_M_3,ICTYPE_M_3,ICTYPE_M_4,ICTYPE_M_5,ICTYPE_M_6,ICTYPE_M_7};
	unsigned char (*pfunc[])(MEMORY_t type) = 
	{
		test_24C_func, 
		test_24C_func,
		test_24C_func,
		test_24C_func, 
		test_24C_func,
		test_24C_func,
		test_24C_func,
		test_4432_42_func,
		test_4418_28_func,
		test_5528_func,
		test_102_func,
		test_1604_func,
		test_1608_func,
		test_153_func
	};
		
	/*process body*/
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)	
			bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);

	//�ϵ�
	//NDK_IccSetType(cardId[type]);
	if ((ret=NDK_IccPowerUp(cardId[type]/*ICTYPE_IC*/, buf,&pnAtrlen))!=NDK_OK)
	{
		if(ESC == cls_show_msg1(g_keeptime, "line %d:�ϵ�ʧ��", __LINE__))
			return;
	}

	while(cnt)
	{
	
		//if(ESC == cls_show_msg1(2,"%s��дѹ��������...\n��ʣ%d��(�ѳɹ�%d��),�����ĵȴ�", card[type], cnt, succ))
			//break;
		cls_printf("%s��дѹ��������...\n��ʣ%d��(�ѳɹ�%d��),�����ĵȴ�", card[type], cnt, succ);//linwl20130617Ϊ��߲���Ч�ʴ˴��޸ĳɽ�ֹ�˳�������2����ʱ
		cnt--;

		//��д�������еĿ�����Ҫ��Կ��֤����֤�����ȵ�
		switch(type) 
		{
			case AT24C01:
			case AT24C02:
			case AT24C04:	
			case AT24C08:	
			case AT24C016:	
			case AT24C032:	
			case AT24C064:	
			case SLE4432_42:
			case SLE4418_28:
			case SLE5528:	
			case AT88SC102:	
			case AT88SC1604:
			case AT88SC1608:
			case AT88SC153:
			
			if(SUCC != pfunc[type](type))
			{
				//NDK_IccPowerDown(cardId[type]/*ICTYPE_IC*/);//memory����дʧ�ܺ�ͽ����µ磬��û�������ϵ磬���½�������д��ʧ��,���м��дʧ�ܲ��µ� 20140124 zhuq
				continue;
			}
			break;
		default:
			continue;
			break;
		}

		succ++; 
	}
	//�µ�
	if ((ret=NDK_IccPowerDown(cardId[type]/*ICTYPE_IC*/))!=NDK_OK)
	{	
		cls_show_msg1(g_keeptime, "line %d:�µ�ʧ��", __LINE__);
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"%s��дѹ���������,�ܹ�����%d��,�ɹ�%d��", card[type],bak-cnt,succ);
	return;
}

//�ۺ�ѹ������
void memory_press(MEMORY_t type)
{
	/*private & local definition*/
	int ret = 0, succ = 0, cnt = DEFAULT_CNT_VLE2, bak = 0,pnAtrlen = 0;
	uint unLen = 0;
	char CntStr[8] = DEFAULT_CNT_STR2;
	uchar buf[300] = {0};
	char *card[] = {"AT24C01","AT24C02","AT24C04","AT24C08","AT24C016","AT24C032","AT24C064","SLE4432_42","SLE4418_28","SLE5528","AT88SC102","AT88SC1604","AT88SC1608","AT88SC153"};
	EM_ICTYPE cardId[]={ICTYPE_M_1_1,ICTYPE_M_1_2,ICTYPE_M_1_4,ICTYPE_M_1_8,ICTYPE_M_1_16,ICTYPE_M_1_32,ICTYPE_M_1_64,ICTYPE_M_2,ICTYPE_M_3,ICTYPE_M_3,ICTYPE_M_4,ICTYPE_M_5,ICTYPE_M_6,ICTYPE_M_7};
	unsigned char (*pfunc[])(MEMORY_t type) = 
	{
		test_24C_func, 
		test_24C_func,
		test_24C_func,
		test_24C_func, 
		test_24C_func,
		test_24C_func,
		test_24C_func,
		test_4432_42_func,
		test_4418_28_func,
		test_5528_func,
		test_102_func,
		test_1604_func,
		test_1608_func,
		test_153_func
	};

	/*process body*/
	cls_printf("Ĭ��ѹ�����Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen, INPUTDISP_OTHER, 0, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)	
		bak = cnt = DEFAULT_CNT_VLE2;
	else
		bak = cnt = atoi(CntStr);

	while(cnt)
	{
		if(ESC == cls_show_msg1(2,"%sѹ��������...\n��ʣ%d��(�ѳɹ�%d��),�����ĵȴ�", card[type], cnt, succ))
			break;
		cnt--;

		//�ϵ�

		//NDK_IccSetType(cardId[type]); 
		if ((ret=NDK_IccPowerUp(cardId[type]/*ICTYPE_IC*/, buf,&pnAtrlen))!=NDK_OK)
		{
			if(ESC==cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��:%s�ϵ�ʧ��(0x%x)", __LINE__, bak-cnt,card[type], ret))
				break;
			else
				continue;
		}
        
            
		//��д�������еĿ�����Ҫ��Կ��֤����֤�����ȵ�
		switch(type) 
		{
			case AT24C01:
			case AT24C02:
			case AT24C04:	
			case AT24C08:	
			case AT24C016:	
			case AT24C032:	
			case AT24C064:	
			case SLE4432_42:
			case SLE4418_28:
			case SLE5528:	
			case AT88SC102:	
			case AT88SC1604:
			case AT88SC1608:
			case AT88SC153:	

			if(SUCC != pfunc[type](type))
			{
				NDK_IccPowerDown(cardId[type]/*ICTYPE_IC*/); 
				continue;
			}
			break;
		default:
			continue;
			break;
		}
		//�µ�
		if ((ret=NDK_IccPowerDown(cardId[type]/*ICTYPE_IC*/))!=NDK_OK)
		{	
			cls_show_msg1_record(FILENAME,FUNCNAME,g_keeptime, "line %d:��%d��:%s�µ�ʧ��(0x%x)", __LINE__, bak-cnt, card[type], ret);
			continue;
		}	
		succ++; 
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"memory���������,�ܹ�����%d��,�ɹ�%d��",bak-cnt,succ);
}

static MEMORY_t select_memorytype(void)
{
	/*private & local definition*/
	int nKeyin = 0, nPageSel = 0;//��0��ʼ����(��һҳ��Ϊ0)
	int cnt = 0, menuid = 0, maxpage = 0, sn =0;
#if defined ME30MH ||defined ME30THM||defined ME32||defined ME32THM//��֧��102 1604��
	MEMORY_t type[] = {AT24C01,AT24C02,AT24C04,AT24C08,AT24C016,AT24C032,AT24C064,SLE4432_42,SLE4418_28,SLE5528,AT88SC1608,AT88SC153}; 
	char* menustr[] =
	{
		"AT24C01",
		"AT24C02",
		"AT24C04",
		"AT24C08",
		"AT24C016",
		"AT24C032",
		"AT24C064",
		"SLE4432_42",
		"SLE4418_28",
		"SLE5528",
		"AT88SC1608",
		"AT88SC153"
	};
#elif defined ME30GX||defined ME32//��֧��102 1604����5528����оֻ��3V�����Ҳ��֧��
	MEMORY_t type[] = {AT24C01,AT24C02,AT24C04,AT24C08,AT24C016,AT24C032,AT24C064,SLE4432_42,SLE4418_28,AT88SC1608,AT88SC153}; 
	char* menustr[] =
	{
		"AT24C01",
		"AT24C02",
		"AT24C04",
		"AT24C08",
		"AT24C016",
		"AT24C032",
		"AT24C064",
		"SLE4432_42",
		"SLE4418_28",
		"AT88SC1608",
		"AT88SC153"
	};
#else
	MEMORY_t type[] = {AT24C01,AT24C02,AT24C04,AT24C08,AT24C016,AT24C032,AT24C064,SLE4432_42,SLE4418_28,SLE5528,AT88SC102,AT88SC1604,AT88SC1608,AT88SC153};	
	char* menustr[] =
	{
		"AT24C01",
		"AT24C02",
		"AT24C04",
		"AT24C08",
		"AT24C016",
		"AT24C032",
		"AT24C064",
		"SLE4432_42",
		"SLE4418_28",
		"SLE5528",
		"AT88SC102",
		"AT88SC1604",
		"AT88SC1608",
		"AT88SC153"
	};
#endif
#if SCR_DISABLE
	char str[1024]={0};
#endif
	char* tmpstr = NULL;

	/*process body*/
	maxpage = (SZ_ARRAY(menustr)-1)/ITEMPERPAGE;//��0��ʼ����(��һҳ��Ϊ0)
	cls_show_msg1(2, "ѡ����Կ�����..\n" "����:���ط�ҳ\n" "����:����ҳ\n");

	while (1)
	{
		NDK_ScrClrs();
		for (cnt=0; cnt<ITEMPERPAGE; cnt++)
		{
			menuid = cnt+ITEMPERPAGE*nPageSel;
			if (menuid==-1)
				break;
			else if (menuid>SZ_ARRAY(menustr)-1)
				break;
			else
				tmpstr = menustr[menuid];

#if SCR_DISABLE	//�ض�λ��������ʾ
			sprintf(str,"%d.����%s\n", cnt+1, tmpstr);
#if 0//ANDIROD_ENABLE
			NDK_AZPortWrite(g_com, strlen(str), str);
#else
			NDK_PortWrite(g_com, strlen(str), str);
#endif
#endif
			NDK_ScrPrintf("%d.����%s\n", cnt+1, tmpstr);
			NDK_ScrRefresh();
		}

		//������
		switch ((nKeyin=lib_getch()))
		{
		case '1':
		case '2':
		case '3':
		case '4':
			if((sn=nKeyin-'1'+ITEMPERPAGE*nPageSel)>SZ_ARRAY(menustr)-1)
				break;
			else
				return type[sn];
			break;
#if K21_ENABLE||defined ME50S
		case '7':
#else
		case DOWNKEY:
#endif
			/*�·���*/
			if (nPageSel>=maxpage)
				nPageSel = 0;
			else
				nPageSel++;
			break;
#if K21_ENABLE||defined ME50S
		case '8':
#else
		case UPKEY:
#endif
			/*�Ϸ���*/
			if (nPageSel==0)
				nPageSel = maxpage;
			else
				nPageSel--;
			break;
		case ESC:
			cls_show_msg1(2, "������ΪSLE4432_42(Ĭ��)...");
			return SLE4432_42;
			break;
		default:
			continue;
			break;
		}
	}
}

//����41��
void systest34(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	/*private & local definition*/
	int keyin = 0;
	MEMORY_t type = SLE4432_42;
	char *card[] = {"AT24C01","AT24C02","AT24C04","AT24C08","AT24C016","AT24C032","AT24C064","SLE4432_42","SLE4418_28","SLE5528","AT88SC102","AT88SC1604","AT88SC1608","AT88SC153"};

	/*process body*/
	cls_show_msg("%s������...�����MEMORY��,���������ʼ\n", TESTITEM);

	while(1)
	{
		keyin = cls_show_msg("1.�ۺ�ѹ������\n"
			"2.��дѹ������\n"
			"3.�쳣����\n"
			"0.����");
		switch(keyin) 
		{
		case '1':
			cls_show_msg1(2, "�ۺ�ѹ������");
			memory_press(type);
			break;
		case '2':
			if(ENTER!=cls_show_msg("memory����дר��ѹ������(������Բ�����ESC�жϲ���),ENTER������..."))
				break;
			memory_rw_press(type);
			break;
		case '3':
			cls_show_msg1(4,"�쳣���ԣ�ִ��һ���쳣���Ժ���ִ��һ���������ԣ����⿨����");
			memroy_abnormal_test(type);
			break;
		case '0':
			type = select_memorytype(); 
			cls_show_msg1(2, "%s���óɹ�!", card[type]);
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

#if 0  //MEMORY��ԭ�����ԵĲ��Դ���!
/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��
* file name		: 1.c
* Author 			: jiangwx
* version			: V1.0
* DATE			: 2006117
* directory 		: .\SRC\card\
* description		: MEMORY������
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  2006117 jiangwx
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "general.h"

/*---------------constants/macro definition---------------------*/
#define  UCID  26

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
*			  	          jiangwx		   2006116	 	    created
*****************************************************************/
//////////////////////////////////////////////////////////////////////////
//===============����MEM������˵��=======================
static void testmemcard(void);   //����MEM��
static void test4442card(void);
static void test1608card(void);
static void test1021604card(void);
static void test4428card(void);
static void testat24c64card(void);
static void testat24c128card(void);
static void testat24c256card(void);
//void testmifare1(void);
static void testat45(void);
//=======================================================

//================================
void testmemcard(void)   //����MEM��
{
	while (1)
	{
		clrscr();
		printf(
			"�ٲ���Sle4432_42\n"
			"�ڲ���AT88SC1608\n"
			"�۲���102��1604\n"
			"���������Ϳ�\n"
			);
		switch(getkeycode(0)) 
		{
		case '1':
            		test4442card();
			break;
		case '2':
            		test1608card();
			break;
		case '3':
            		test1021604card();
			break;
		case '4':
			clrscr();
			printf(
				"�ٲ���Sle4418_28\n"
				"�ڲ���at24c64   \n"
				"�۲���Sle5528\n"
				"���������Ϳ�\n"
				);
			switch(getkeycode(0))
			{
			case '1':
				test4428card();
				break;
			case '2':
				testat24c64card();
				break;
			case '3':
//                		testmifare1();				
				break;
			case '4':
                		clrscr();
				printf(
					"�ٲ���AT45˰�ؿ�\n"
					"�ڲ���at24c128 \n"
					"�۲���at24c256 \n"
					"���������Ϳ�\n"
					);
				switch(getkeycode(0))
				{
				case '1':
					testat45();
					break;
				case '2':
					testat24c128card();
//					testat128();
					break;
				case '3':
					testat24c256card();
//					testat256();
					break;
				case ESC:
					return;
				default:
					break;
				}
				break;
			case ESC:
				return;
			default:
				break;
			}
			break;
		case ESC:
			return;
		default:
			break;
		}
	}
}

//=============================
//    09H��	Sle4432/4442  ��
//=============================
static void poweronsle4442(void);
static unsigned char comparesle4442key(unsigned char data_in1,unsigned char data_in2,unsigned char data_in3);
static void readsle4442maindata(void);
static void writesle4442maindata(void);
static void powerdownsle4442(void);
static void readsle4442securitymemory(void);
static void readsle4442protectionmemory(void);
static unsigned char writesle4442securitymemory(unsigned char data_in1,unsigned char data_in2,unsigned char data_in3);
static unsigned char comparesle4442maindata(unsigned char data_in);
static void writeandcompareandrenewkey(void);
static void writesle4442protectionmemory(void);

unsigned char readdata;
char icbuf[300],icreadbuf[300];
#define readaddress 0x08

static void test4442card(void)  //zjh ���� SLE4442 ��
{
	//int i;
	//int ret;
	    	
	while (1)
	{
		clrscr();
		printf(
			"���ϵ�  ���µ�\n"
			"�۶����ݢ޶�����\n"
			"��д���ݢ߶�����\n"
			"������Կ��д��Կ\n"
			);
		switch(getkeycode(0))
		{
		case '1':
			poweronsle4442();
			break;
		case '2':
			powerdownsle4442();
			break;
		case '3':
			readsle4442maindata();
			break;
		case '4':
			writesle4442maindata();
			break;
		case '5':
			comparesle4442key(0xff,0xff,0xff);
			break;
		case '6':
			readsle4442securitymemory();
			break;
		case '7':
			readsle4442protectionmemory();
			break;
		case '8':
			writeandcompareandrenewkey();
			break;
		case '9':
			writesle4442protectionmemory();
			break;
		case ESC:
			return;
		default:
			break;
		}
	}
}

static void poweronsle4442(void)
{
	int i;
	//int ret;

	SetICType(9,0); //�����ж�5002��״̬��ʼ�������û����䣬��˯�ߺ��޷��ϵ�
    	clrscr();
	if (icpowerup(0,icbuf)==SUCC)
	    printf("IC���ϵ�ɹ�\n");
	else
	{
        printf("IC���ϵ�ʧ��\n");
		getkeycode(0);
		return;
	}
    	for (i=1;i<(icbuf[0]+1);i++)
	{
		printf("%02x",icbuf[i]);
		printf(" ");
	}
	getkeycode(5);
}
///////////////////////////////////////////
////         У��SLE4442����Կ         ////
////     ��00 20 00 00 03 FF FF FF��   ////  
///////////////////////////////////////////
static unsigned char comparesle4442key(unsigned char data_in1,unsigned char data_in2,unsigned char data_in3)
{
	int i;
	int ret;
	
	icbuf[0]=0x00;
    	icbuf[1]=0x20;
	icbuf[2]=0x00;
	icbuf[3]=0x00;
	icbuf[4]=0x03;
	icbuf[5]=data_in1;  
	icbuf[6]=data_in2;
	icbuf[7]=data_in3;
	clrscr();
	ret = iccrw(0,8,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			printf("��ԿУ��ɹ�\n");
		else 
		{
			printf("��ԿУ��ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				printf(" ");
			}
			getkeycode(0);
			return SUCC+1;
		}
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
		    	printf(" ");
		}
 		printf(" \n");
 	}
	else 
	{
		printf("��ԿУ��ʧ��\n");
        	getkeycode(0);
		return SUCC+1;
	}
	getkeycode(5);
	return SUCC;
}
///////////////////////////////////////////
////       ��SLE4442�����ݴ�����       ////
////        ��00 B0 00 20 04��         ////	
///////////////////////////////////////////
static void readsle4442maindata(void)
{
	int i;
	int ret;
    	char buf[8];

    	icbuf[0]=0x00;
    	icbuf[1]=0xB0;
	icbuf[2]=0x00;

	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-255\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[3]=(ret&0xff);

    	strcpy(buf,"");          //���볤��
	clrscr();
	printf("�����ó���\n");
	printf("0-252\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[4]=ret;

	clrscr();
	icreadbuf[0] = 0;
	ret = iccrw(0,5,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			printf("���������ɹ�\n");
		else 
		{
			printf("��������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
		    	printf(" ");
		}
 		printf(" \n");
		readdata = icreadbuf[1]; //��������
 	}
	else 
	{
		printf("��������ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(5);
}
///////////////////////////////////////////
////       дSLE4442�����ݴ�����       ////
////   ��00 D0 00 20 04 01 02 03 04��  ////	
///////////////////////////////////////////
static void writesle4442maindata(void)
{
	int i;
	int ret;
    	char buf[8];

    	icbuf[0]=0x00;
    	icbuf[1]=0xD0;
	icbuf[2]=0x00;

	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-255\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[3]=(ret&0xff);

    	strcpy(buf,"");          //���볤��
	clrscr();
	printf("�����ó���\n");
	printf("0-252\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[4]=ret;

    	strcpy(buf,"");          //��������
	clrscr();
	printf("������Ҫд�����ֵ\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
    
	for (i=0;i<icbuf[4];i++)
		icbuf[5+i]=ret;

	clrscr();
	ret = iccrw(0,5+icbuf[4],icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
		{
            		if (comparesle4442maindata(readdata)==SUCC)
			    printf("д�������ɹ�\n");
			else 
			{
                		printf("д������ʧ��1\n");
				for (i=0;i<icreadbuf[0];i++)
				{
					printf("%02x",icreadbuf[i+1]);
					printf(" ");
				}
				getkeycode(0);
				return;
			}
		}
		else 
		{
			printf("д������ʧ��2\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				printf(" ");
			}
			getkeycode(0);
			return;
		}
 	}
	else 
	{
		printf("д������ʧ��3\n");
        	getkeycode(0);
		return;
	}
	
	getkeycode(0);
}	
///////////////////////////////////////////
////       �µ�SLE4442��               ////
////                                   ////	
///////////////////////////////////////////
static void powerdownsle4442(void)
{
	clrscr();
	if (icpowerdown(0)==SUCC)
	{
        	printf("IC���µ�ɹ�\n");
		getkeycode(2);
	}
	else
	{
        	printf("IC���µ�ʧ��\n");
		getkeycode(0);
	}
}

///////////////////////////////////////////
////       �Ƚ�SLE4442�����ݴ�����     ////
////        ��00 B0 00 20 04��         ////	
///////////////////////////////////////////
static unsigned char comparesle4442maindata(unsigned char data_in)
{
	int i;
	int ret;
//	char buf[8];

    	icbuf[0]=0x00;
    	icbuf[1]=0xB0;
	icbuf[2]=0x00;
	icbuf[3]=readaddress;  //����д����������&����д����������
	icbuf[4]=0x04;
	clrscr();
	ret = iccrw(0,5,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]!=0x90)||(icreadbuf[icreadbuf[0]]!=0x00))
			return SUCC+1;
		
		for (i=1;i<(icreadbuf[0]+1-2);i++)
		{
            		if (icreadbuf[i]!=data_in)
			    return SUCC+1;
		}
		return SUCC;
 	}
	else 
	    return SUCC+1;	
}
///////////////////////////////////////////
////       ��SLE4442��������           ////
////      ��00 B0 C0 00 04��           ////	
///////////////////////////////////////////
static void readsle4442securitymemory(void)
{
    	int i;
	int ret;

    	icbuf[0]=0x00;
    	icbuf[1]=0xB0;
	icbuf[2]=0xC0;
	icbuf[3]=0x00;  
	icbuf[4]=0x04;
	clrscr();
	ret = iccrw(0,5,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			printf("���������ɹ�\n");
		else 
		{
			printf("��������ʧ��\n");
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
		    	printf(" ");
		}
  	}
	else 
	{
		printf("��������ʧ��\n");
        	getkeycode(0);
		return;
	}
	getkeycode(5);
}
///////////////////////////////////////////
////       ��SLE4442��д������         ////
////        ��00 B0 80 00 04��         ////	
///////////////////////////////////////////
static void readsle4442protectionmemory(void)
{
    	int i;
	int ret;

    	icbuf[0]=0x00;
    	icbuf[1]=0xB0;
	icbuf[2]=0x80;
	icbuf[3]=0x00;  
	icbuf[4]=0x04;
	clrscr();
	ret = iccrw(0,5,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			printf("��д�������ɹ�\n");
		else 
		{
			printf("��д������ʧ��\n");
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
		    	printf(" ");
		}
  	}
	else 
	{
		printf("��д������ʧ��\n");
        	getkeycode(0);
		return;
	}
	getkeycode(5);
}
///////////////////////////////////////////
////       дSLE4442��������           ////
////   ��00 D0 C0 01 03 xx xx xx��     ////	
///////////////////////////////////////////
static unsigned char writesle4442securitymemory(unsigned char data_in1,unsigned char data_in2,unsigned char data_in3)
{
	int i;
	int ret;

    	icbuf[0]=0x00;
    	icbuf[1]=0xD0;
	icbuf[2]=0xC0;
	icbuf[3]=0x01;  
	icbuf[4]=0x03;
	icbuf[5]=data_in1;
	icbuf[6]=data_in2;
	icbuf[7]=data_in3;
	clrscr();
	ret = iccrw(0,8,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			printf("д��Կ�ɹ�\n");
		else 
		{
			printf("д��Կʧ��\n");
			getkeycode(0);
			return SUCC+1;
		}
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
		    	printf(" ");
		}
  	}
	else 
	{
		printf("д��Կʧ��\n");
        	getkeycode(0);
		return SUCC+1;
	}
	getkeycode(5);	
	return SUCC;
}

static void writeandcompareandrenewkey(void)
{
     	if (writesle4442securitymemory(0x88,0x89,0x8a)!=SUCC) 
     		return;
     	readsle4442securitymemory();
     	if (comparesle4442key(0x88,0x89,0x8a)!=SUCC) 
     		return;
	if (writesle4442securitymemory(0xff,0xff,0xff)!=SUCC) 
		return;
	readsle4442securitymemory();
     	if (comparesle4442key(0xff,0xff,0xff)!=SUCC) 
     		return;
}

///////////////////////////////////////////
////       дSLE4442��д������         ////
////    ��00 D0 80 Add Lc Data��       ////
///////////////////////////////////////////
//   	Add 00h��1Fh 
//      ��P2+Lc>32�򷵻أ�6A 86 ��P1 P2��������
//

static void writesle4442protectionmemory(void)
{
	int i;
	int ret;

    	icbuf[0]=0x00;
    	icbuf[1]=0xD0;
	icbuf[2]=0x80;
	icbuf[3]=0x04;  
	icbuf[4]=0x01;
	icbuf[5]=0x00;  //������4�ֽڱ���λ
	clrscr();
	ret = iccrw(0,6,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			printf("д�������ɹ�\n");
		else 
		{
			printf("д������ʧ��\n");
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
		    	printf(" ");
		}
  	}
	else 
	{
		printf("д������ʧ��\n");
        	getkeycode(0);
		return;
	}
	getkeycode(5);	
	return;
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//==========================================$
//  0CH:    AT88SC102  1604 ��
//==========================================

static void poweron1021604(void);
static void powerdown1021604(void);
static void read1021604(void);
static void write1021604(void);
static void erase1021604(void);
static void verify1021604password(void);


static void test1021604card(void)
{
	while (1)
	{
		clrscr();
		printf(
			"���ϵ�  ���µ�\n"
			"�۲���  ��\n"
			"�ܶ����ݢ�\n"
			"��д���ݢ�����Կ\n"
			);
		switch(getkeycode(0))
		{
		case '1':
			poweron1021604();
			break;
		case '2':
			powerdown1021604();
			break;
		case '3':
			erase1021604();
			break;
		case '4':
			read1021604();
			break;
		case '5':
			write1021604();
			break;
		case '6':
			
			break;
		case '7':
			
			break;
		case '8':
			verify1021604password();			
			break;
		case '9':
			
			break;
		case ESC:
			return;
		default:
			break;
		}
	}
}

//===============================================
static void poweron1021604(void)
{
	int i;

	clrscr();
	printf("��1ѡ102��\n");
	printf("��2ѡ1604��\n");
	switch(getkeycode(0))
	{
	case '1':
		SetICType(0x0a,0);
	    	break;
	case '2':
		SetICType(0x0b,0);
	    	break;
	default:
		return;
	}
	
    	clrscr();
	if (icpowerup(0,icbuf)==SUCC)
	    	printf("IC���ϵ�ɹ�\n");
	else
	{
        	printf("IC���ϵ�ʧ��\n");
		getkeycode(0);
		return;
	}
    	for (i=1;i<(icbuf[0]+1);i++)
	{
		printf("%02x",icbuf[i]);
		printf(" ");
	}
	getkeycode(5);
}
//=================================================
static void powerdown1021604(void)
{
	clrscr();
	if (icpowerdown(0)==SUCC)
	{
        	printf("IC���µ�ɹ�\n");
		getkeycode(2);
	}
	else
	{
        	printf("IC���µ�ʧ��\n");
		getkeycode(0);
	}
}
//=====================================================
//             ��102/1604
//    00  B0  highadd  lowadd  length
//=====================================================
static void read1021604(void)
{
	int i;
	int ret;
	char buf[5];

	icbuf[0]=0x00;
    	icbuf[1]=0xB0;

    	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[2] = (ret & 0xff00)>>8;	
	icbuf[3] = (ret & 0xff);  //��ַ  ret ��INT
		
	strcpy(buf,"");        //���볤��
	clrscr();
	printf("�����볤��\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 8;
	ret=atol(buf);	
    	icbuf[4]=(ret & 0xff);

	printf("%02x",icbuf[2]);  //debug
	portwrite(AUX1,1,icbuf+3);//debug
	printf("%02x",icbuf[3]);  //debug
	portwrite(AUX1,1,icbuf+4);//debug
	printf("%02x",icbuf[4]);  //debug
	portwrite(AUX1,1,icbuf+5);//debug
	getkeycode(0); //debug
   	
	clrscr();
	ret = iccrw(0,5,icbuf,icreadbuf);

    	if(ret==SUCC)
	{   
        if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("��ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0]-2;i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 		readdata = icreadbuf[i-1]; //��������
 	}
	else 
	{
		printf("��ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
//=====================================================
//             д102/1604
//      00   D0   00   Add   length  xx  xx  ....
//=====================================================
static void write1021604(void)
{
	int i;
	int ret;
	char buf[5];
	
	icbuf[0]=0x00;
    	icbuf[1]=0xD0;

    	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[2] = (ret & 0xff00)>>8;	
	icbuf[3] = (ret & 0xff);  //��ַ  ret ��INT

	strcpy(buf,"");          //ѡ��д�ĳ���
	clrscr();
	printf("��ѡ��Ҫд�ĳ���\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[4] = (ret & 0xff); 

    	strcpy(buf,"");          //����Ҫд����������
	clrscr();
	printf("������Ҫд������\n");
	printf("0--255\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=(atol(buf)& 0xff);

	for (i=0;i<icbuf[4];i++)
		 icbuf[5+i] = ret;  

	printf("%02x",icbuf[2]);  //debug
	portwrite(AUX1,1,icbuf+3);//debug
	printf("%02x",icbuf[3]);  //debug
	portwrite(AUX1,1,icbuf+4);//debug
	printf("%02x",icbuf[4]);  //debug
	portwrite(AUX1,1,icbuf+5);//debug
	getkeycode(0); //debug
	
	clrscr();
	ret = iccrw(0,5+icbuf[4],icbuf,icreadbuf);

    	if(ret==SUCC)
	{   
        if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("дʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		printf("д�ɹ�\n");
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 	}
	else 
	{
		printf("дʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
//=====================================================
//             ����102/1604
//      00   0e   highadd  lowadd  length
//=====================================================
static void erase1021604(void)
{
	int i;
	int ret;
	char buf[5];

	icbuf[0]=0x00;
    	icbuf[1]=0x0e;


	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[2] = (ret & 0xff00)>>8;	
	icbuf[3] = (ret & 0xff);  //��ַ  ret ��INT
	
	strcpy(buf,"");          //ѡ������ĳ���
	clrscr();
	printf("��ѡ��Ҫ�����ĳ���\n");
	printf("0--255\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[4] = (ret & 0xff); 

    	clrscr();
	ret = iccrw(0,5,icbuf,icreadbuf);

    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("����ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		printf("�����ɹ�\n");
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 	}
	else 
	{
		printf("����ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
//=====================================================
//              У��102/1604��Կ
//        00   20   P1  P2  length   data1 data2 ....
//=====================================================
static void verify1021604password(void)
{
	int i;
	int ret;
	char buf[5];

	icbuf[0]=0x00;
    	icbuf[1]=0x20;
	
	strcpy(buf,"");          //����P1ֵ
	clrscr();
	printf("������P1ֵ\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
    	icbuf[2]=(ret & 0xff);

    	strcpy(buf,"");          //����P2ֵ
	clrscr();
	printf("������P2ֵ\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[3] = (ret & 0xff);  //��Կ��  ret ��INT
	
    	strcpy(buf,"");          //������Կ����
	clrscr();
	printf("��������Կ����\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[4] = (ret & 0xff);
	
	for (i=0;i<icbuf[4];i++)
	{
		clrscr();
		strcpy(buf,"");
		printf("��������Կֵ\n");
		ret=getnumstr(buf,4,2,0);
		ret=atol(buf);
		icbuf[5+i]=(ret & 0xff);
	}
	
    	for (i=0;i<icbuf[4];i++)
	{
		printf("%02x",icbuf[5+i]);  //debug
		portwrite(AUX1,1,icbuf+5+i);//debug
	}
	
	getkeycode(0); //debug
   	
	clrscr();
	ret = iccrw(0,5+icbuf[4],icbuf,icreadbuf);

    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("��ԿУ��ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		printf("��ԿУ��ɹ�\n");
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 	}
	else 
	{
		printf("��ԿУ��ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//==========================================$
//  0CH:    AT88SC1608  ��
//==========================================
static void poweron1608(void);
static void powerdown1608(void);
static void Authentication1608(void);
static void read1608(void);
static void read1608userzone(void);
static void read1608configzone(void);
static void verifypassword(void);
static void write1608(void);
static void write1608userzone(void); 
static void write1608configzone(void);
static void select1608userzone(void);

static void test1608card(void)
{
	while (1)
	{
		clrscr();
		printf(
			"���ϵ�  ���µ�\n"
			"����֤  ��\n"
			"�ܶ����ݢ�\n"
			"��д���ݢ�����Կ\n"
			);
		switch(getkeycode(0))
		{
		case '1':
			poweron1608();
			break;
		case '2':
			powerdown1608();
			break;
		case '3':
			Authentication1608();
			break;
		case '4':
			read1608();
			break;
		case '5':
			write1608();
			break;
		case '6':
			
			break;
		case '7':
			
			break;
		case '8':
			verifypassword();			
			break;
		case '9':
			
			break;
		case ESC:
			return;
		default:
			break;
		}
	}
}

//===============================================
static void poweron1608(void)
{
	int i;

	SetICType(0x0c,0); //�����ж�5002��״̬��ʼ�������û����䣬��˯�ߺ��޷��ϵ�
    	clrscr();
	if (icpowerup(0,icbuf)==SUCC)
	    printf("IC���ϵ�ɹ�\n");
	else
	{
        	printf("IC���ϵ�ʧ��\n");
		getkeycode(0);
		return;
	}
    	for (i=1;i<(icbuf[0]+1);i++)
	{
		printf("%02x",icbuf[i]);
		printf(" ");
	}
	getkeycode(5);
}
//=================================================
static void powerdown1608(void)
{
	clrscr();
	if (icpowerdown(0)==SUCC)
	{
        	printf("IC���µ�ɹ�\n");
		getkeycode(2);
	}
	else
	{
        	printf("IC���µ�ʧ��\n");
		getkeycode(0);
	}
}
//=====================================================
//             ��֤ Authentication
// 00   82   00   00   10   Q0(0)��Q0(7)  Gc(0)��Gc(7)
//=====================================================
static void Authentication1608(void)
{
	int i;
	int ret;
	
	icbuf[0]=0x00;
    	icbuf[1]=0x82;
	icbuf[2]=0x00;
	icbuf[3]=0x00;
	icbuf[4]=0x10;

	icbuf[5]=0x88; icbuf[6]=0x88; icbuf[7]=0x88; icbuf[8]=0x88; icbuf[9]=0x88; icbuf[10]=0x88; icbuf[11]=0x88; icbuf[12]=0x88;
	icbuf[13]=0x11; icbuf[14]=0x11; icbuf[15]=0x11; icbuf[16]=0x11; icbuf[17]=0x11; icbuf[18]=0x11; icbuf[19]=0x11; icbuf[20]=0x11;

	clrscr();
	ret = iccrw(0,21,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			printf("��֤�ɹ�\n");
		else 
		{
			printf("��֤ʧ��\n");
			getkeycode(0);
		}
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
		    	printf(" ");
		}
 		printf(" \n");
 	}
	else 
	{
		printf("ͨѶʧ��\n");
        	getkeycode(0);
		return;
	}
	getkeycode(5);
}

//=====================================================
//             ��1608
// 
//=====================================================
static void read1608(void)
{
	while (1)
	{
		clrscr();
		printf(
			"�ٶ��û���\n"
			"�ڶ�������\n"
			"��ѡ���û���\n"
			);
		switch(getkeycode(0))
		{
		case '1':
			read1608userzone();
			break;
		case '2':
			read1608configzone();
			break;
		case '3':
			select1608userzone();
			break;
		case ESC:
			return;
		default:
			break;
		}
	}
}
//=====================================================
//             ��1608�û���
//    00  B0  01  add  length
//=====================================================
static void read1608userzone(void)
{
	int i;
	int ret;
	char buf[8];

	icbuf[0]=0x00;
    	icbuf[1]=0xB0;
	icbuf[2]=0x01;
    
	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-255\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	icbuf[3] = (ret & 0xff);  //��ַ  ret ��INT
		
	strcpy(buf,"");        //���볤��
	clrscr();
	printf("�����볤��\n");
	printf("0-250\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 8;
	ret=atol(buf);
	
    	icbuf[4]=(ret & 0xff);

	printf("%02x",icbuf[3]);  //debug
	portwrite(AUX1,1,icbuf+3);//debug
	printf("%02x",icbuf[4]);  //debug
	portwrite(AUX1,1,icbuf+4);//debug
	getkeycode(0); //debug
   	
	clrscr();
	ret = iccrw(0,5,icbuf,icreadbuf);

    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("���û���ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0]-2;i++)
		{
           		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 		readdata = icreadbuf[i-1]; //��������
 	}
	else 
	{
		printf("���û���ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
//=====================================================
//              ��1608������
//        00  B0  05  add  length
//=====================================================
static void read1608configzone(void)
{
	int i;
	int ret;
	char buf[5];

	icbuf[0]=0x00;
    	icbuf[1]=0xB0;
	icbuf[2]=0x05;
    
	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-128\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	icbuf[3] = (ret & 0xff);  //��ַ  ret ��INT
		
	strcpy(buf,"");         //���볤��
	clrscr();
	printf("�����볤��\n");
	printf("0-128\n");        //������˿
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 8;
	ret=atol(buf);
	
    	icbuf[4]=(ret & 0xff);

	printf("%02x",icbuf[3]);  //debug
	portwrite(AUX1,1,icbuf+3);//debug
	printf("%02x",icbuf[4]);  //debug
	portwrite(AUX1,1,icbuf+4);//debug
	getkeycode(0); //debug
   	
	clrscr();
	ret = iccrw(0,5,icbuf,icreadbuf);

    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("��������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0]-2;i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 		readdata = icreadbuf[i-1]; //��������
 	}
	else 
	{
		printf("��������ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}

//=====================================================
//              У��1608��Կ
//        00   20   03  P2  03   Pw(0)Pw(1)Pw(2)
//=====================================================
static void verifypassword(void)
{
	int i;
	int ret;
	char buf[5];

	icbuf[0]=0x00;
    	icbuf[1]=0x20;
	icbuf[2]=0x03;
    
	strcpy(buf,"");          //������Կ��  ָ��Ҫ��֤����Կ�� (00-0F)
	clrscr();
	printf("����Կ��\n");
	printf("0��7д8��f��\n");
	printf("0-15\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	icbuf[3] = (ret & 0xff);  //��Կ��  ret ��INT
	icbuf[4] = 3; 
	
	clrscr();
	strcpy(buf,"");
	printf("�������һ����Կ\n");
	printf("0-255\n");        //������˿
	ret=getnumstr(buf,4,2,0);
	ret=atol(buf);
	icbuf[5]=(ret & 0xff);

	clrscr();
	strcpy(buf,"");
	printf("������ڶ�����Կ\n");
	printf("0-255\n");        //������˿
	ret=getnumstr(buf,4,2,0);
	ret=atol(buf);
    	icbuf[6]=(ret & 0xff);

	clrscr();
	strcpy(buf,"");
	printf("�������������Կ\n");
	printf("0-255\n");        //������˿
	ret=getnumstr(buf,4,2,0);
	ret=atol(buf);
    	icbuf[7]=(ret & 0xff);


	printf("%02x",icbuf[5]);  //debug
	portwrite(AUX1,1,icbuf+5);//debug
	printf("%02x",icbuf[6]);  //debug
	portwrite(AUX1,1,icbuf+6);//debug
	printf("%02x",icbuf[7]);  //debug
	portwrite(AUX1,1,icbuf+7);//debug
	getkeycode(0); //debug
   	
	clrscr();
	ret = iccrw(0,8,icbuf,icreadbuf);

    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("��ԿУ��ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		printf("��ԿУ��ɹ�\n");
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 	}
	else 
	{
		printf("��ԿУ��ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
//=====================================================
//             д1608
// 
//=====================================================
static void write1608(void)
{
	while (1)
	{
		clrscr();
		printf(
			"��д�û���\n"
			"��д������\n"
			"��ѡ���û���\n"
			);
		switch(getkeycode(0))
		{
		case '1':
			write1608userzone();
			break;
		case '2':
			write1608configzone();
			break;
		case '3':
			select1608userzone();
			break;
		case ESC:
			return;
		default:
			break;
		}
	}
}
//=====================================================
//             д1608�û���
//      00   D0   00   Add   length  xx  xx  ������
//=====================================================
static void write1608userzone(void)
{
	int i;
	int ret;
	char buf[5];

	icbuf[0]=0x00;
    	icbuf[1]=0xD0;
	icbuf[2]=0x00;
    
	strcpy(buf,"");          //ѡ���ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0--255\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[3] = (ret & 0xff);  

    	strcpy(buf,"");          //���볤��
	clrscr();
	printf("�����ó���\n");
	printf("0-252\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[4]=ret;

    	strcpy(buf,"");          //��������
	clrscr();
	printf("������Ҫд�����ֵ\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
    
	for (i=0;i<icbuf[4];i++)
    	icbuf[5+i]=ret;

	clrscr();
	ret = iccrw(0,5+icbuf[4],icbuf,icreadbuf);

    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("�û���дʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		printf("�û���д�ɹ�\n");
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 	}
	else 
	{
		printf("�û���дʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
//=====================================================
//             д1608������
//         00   D0   04   Add   length=1  xx
//=====================================================
static void write1608configzone(void)
{
	int i;
	int ret;
	char buf[5];

	icbuf[0]=0x00;
    	icbuf[1]=0xD0;
	icbuf[2]=0x04;
    
	strcpy(buf,"");          //ѡ���ַ
	clrscr();
	printf("��ѡ����������ַ\n");
	printf("0--128\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	icbuf[3] = (ret & 0xff);  //�û�����ַ  ret ��INT
	icbuf[4] = 3;  //����

    	strcpy(buf,"");          //�����һ������
	clrscr();
	printf("�������һ���ֽ�\n");
	printf("0--255\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
    	icbuf[5] = (ret & 0xff);  
	
    	strcpy(buf,"");          //����ڶ�������
	clrscr();
	printf("������ڶ����ֽ�\n");
	printf("0--255\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
    	icbuf[6] = (ret & 0xff);  

	strcpy(buf,"");          //�������������
	clrscr();
	printf("������������ֽ�\n");
	printf("0--255\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
    	icbuf[7] = (ret & 0xff);  

	clrscr();
	ret = iccrw(0,8,icbuf,icreadbuf);

    	if(ret==SUCC)
	{   
	        if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("�û���дʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		printf("�û���д�ɹ�\n");
		for (i=0;i<icreadbuf[0];i++)
		{
	            	printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 	}
	else 
	{
		printf("�û���дʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
//=====================================================
//             ѡ��1608�û���
//     00   D0   02   Add   0  
//=====================================================
static void select1608userzone(void)
{
	int i;
	int ret;
	char buf[5];

	icbuf[0]=0x00;
    	icbuf[1]=0xD0;
	icbuf[2]=0x02;
    
	strcpy(buf,"");          //ѡ���û���
	clrscr();
	printf("��ѡ���û���\n");
	printf("0--7\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	icbuf[3] = (ret & 0xff);  //�û���  ret ��INT
	icbuf[4] = 0; 
	
	clrscr();
	ret = iccrw(0,5,icbuf,icreadbuf);

    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("�û���ѡ��ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				printf(" ");
			}
			getkeycode(0);
			return;
		}
		printf("�û���ѡ��ɹ�\n");
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 	}
	else 
	{
		printf("�û���ѡ��ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//=============================
//    08H��	Sle4428  ��
//=============================
static void poweronsle4428(void);
static unsigned char comparesle4428key(unsigned char data_in1,unsigned char data_in2);
static void readsle4428maindata(void);
static void writesle4428maindata(void);
static void powerdownsle4428(void);
static void readsle4428securitymemory(void);

static unsigned char writesle4428securitymemory(unsigned char data_in1,unsigned char data_in2,unsigned char data_in3);
static unsigned char comparesle4428maindata(unsigned char data_in);

static void test4428card(void)  //zjh ���� SLE4442 ��
{
	//int i;
	//int ret;
	    	
	while (1)
	{
		clrscr();
		printf(
			"���ϵ�  ���µ�\n"
			"�۶����ݢ޶�����\n"
			"��д���ݢߴ���Կ\n"
			"������Կ\n"
			);
		switch(getkeycode(0))
		{
		case '1':
			poweronsle4428();
			break;
		case '2':
			powerdownsle4428();
			break;
		case '3':
			readsle4428maindata();
			break;
		case '4':
			writesle4428maindata();
			break;
		case '5':
			comparesle4428key(0xff,0xff);
			break;
		case '6':
			readsle4428securitymemory();
			break;
		case '7':
			comparesle4428key(0x20,0x20);  //�������Կ
			break;
		
		case ESC:
			return;
		default:
			break;
		}
	}
}

static void poweronsle4428(void)
{
	int i;
	//int ret;

	SetICType(0x0d,0); //�����ж�5002��״̬��ʼ�������û����䣬��˯�ߺ��޷��ϵ�
    	clrscr();
	if (icpowerup(0,icbuf)==SUCC)
	    printf("IC���ϵ�ɹ�\n");
	else
	{
        printf("IC���ϵ�ʧ��\n");
		getkeycode(0);
		return;
	}
    	for (i=1;i<(icbuf[0]+1);i++)
	{
		printf("%02x",icbuf[i]);
		printf(" ");
	}
	getkeycode(0);
}
///////////////////////////////////////////
////         У��SLE4428����Կ         ////
////     ��00 20 00 00 02 67 c6��      ////  
///////////////////////////////////////////
static unsigned char comparesle4428key(unsigned char data_in1,unsigned char data_in2)
{
	int i;
	int ret;
	
	icbuf[0]=0x00;
    	icbuf[1]=0x20;
	icbuf[2]=0x00;
	icbuf[3]=0x00;
	icbuf[4]=0x02;
	icbuf[5]=data_in1;  
	icbuf[6]=data_in2;
	clrscr();
	ret = iccrw(0,7,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			printf("��ԿУ��ɹ�\n");
		else 
		{
			printf("��ԿУ��ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				printf(" ");
			}
			getkeycode(0);
			return SUCC+1;
		}
		for (i=0;i<icreadbuf[0];i++)
		{
            		printf("%02x",icreadbuf[i+1]);
		    	printf(" ");
		}
 		printf(" \n");
 	}
	else 
	{
		printf("��ԿУ��ʧ��\n");
        	getkeycode(0);
		return SUCC+1;
	}
	getkeycode(5);
	return SUCC;
}
///////////////////////////////////////////
////       ��SLE4428�����ݴ�����       ////
////        ��00 B0 80 00 0c�� ������λ////	
///////////////////////////////////////////
static void readsle4428maindata(void)
{
	int i;
	int ret;
	unsigned char bflag=1;
	char buf[5];

	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-1023\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	icbuf[2] = (ret >> 8);
    	icbuf[3] = (ret & 0xff);
	
	strcpy(buf,"");        //���볤��
	clrscr();
	printf("�����볤��\n");
	printf("0-126����0-252\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 12;
	ret=atol(buf);
    	icbuf[4]=(ret & 0xff);

	printf("���Ҫ������λ������1\n");  //Ҫ��Ҫ������λ
	if (getkeycode(0)=='1')
	    icbuf[2]=icbuf[2]|0x80;  

    	icbuf[0]=0x00;
    	icbuf[1]=0xB0;
    	
	clrscr();
	icreadbuf[0] = 0;
	ret = iccrw(0,5,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("��������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				if (bflag!=1)
				{ 
					bflag = 1;
					printf(" ");
				}
				else
					bflag = 0;
			}
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0]-2;i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
	            	if ( (icbuf[2]&0x80)==0x80 ) //����ж�����λ
	            	{
				if (bflag!=1)
				{ 
					bflag = 1;
					printf(" ");
				}
				else
					bflag = 0;
			}
			else
				printf(" ");
		}
 		printf(" \n");
		readdata = icreadbuf[i-1]; //��������
 	}
	else 
	{
		printf("��������ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
///////////////////////////////////////////
////       дSLE4428�����ݴ�����       ////
////   ��00 D0 00 08 04 01 02 03 04��  ////	
///////////////////////////////////////////
//        Add1 ----λ��ַ�����λ b1��b0 Ϊ��ʼ��ַ�ĸ��ֽڣ�ȡֵ��ΧΪ(0--3)
//             ----λ��ַb5~b2 Ϊ0
//             ----λ��ַ b7_b6 = 00ʱ��д���ݣ���д����λ
//                     	b7_b6 = 10ʱ��д���ݺͱ���λ
//                  	    b7_b6 = 11ʱ���Ƚ����ݺ�д����λ��
//        Add2 ---- Ϊ��ʼ��ַ�ĵ��ֽڣ�ȡֵ��Χ(0--0ffh)

static void writesle4428maindata(void)
{
	int i;
	int ret;
	char buf[8];

    	icbuf[0]=0x00;
    	icbuf[1]=0xD0;

    	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-1023\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[2]=(ret>>8);
	icbuf[3]=(ret&0xff);

    	strcpy(buf,"");          //���볤��
	clrscr();
	printf("�����ó���\n");
	printf("0-252\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	icbuf[4]=ret;

    	strcpy(buf,"");          //��������
	clrscr();
	printf("������Ҫд�����ֵ\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
    
	for (i=0;i<icbuf[4];i++)
		icbuf[5+i]=ret;
	
	clrscr();
	ret = iccrw(0,5+icbuf[4],icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
		{
#if 0
	            	if (comparesle4442maindata(readdata)==SUCC)
				    printf("д�������ɹ�\n");
			else 
			{
	                	printf("д������ʧ��1\n");
				getkeycode(0);
				return;
			}
#endif
		}
		else 
		{
			printf("д������ʧ��2\n");
			getkeycode(0);
			return;
		}
 	}
	else 
	{
		printf("д������ʧ��3\n");
        	getkeycode(0);
		return;
	}
	getkeycode(5);
}	
///////////////////////////////////////////
////       �µ�SLE4428��               ////
////                                   ////	
///////////////////////////////////////////
static void powerdownsle4428(void)
{
	clrscr();
	if (icpowerdown(0)==SUCC)
	{
        	printf("IC���µ�ɹ�\n");
		getkeycode(2);
	}
	else
	{
        	printf("IC���µ�ʧ��\n");
		getkeycode(0);
	}
}

///////////////////////////////////////////
////       �Ƚ�SLE4428�����ݴ�����     ////
////        ��00 B0 00 20 04��         ////	
///////////////////////////////////////////
static unsigned char comparesle4428maindata(unsigned char data_in)
{
	int i;
	int ret;

    icbuf[0]=0x00;
    icbuf[1]=0xB0;
	icbuf[2]=0x00;
	icbuf[3]=readaddress;  //����д����������&����д����������
	icbuf[4]=0x08;
	clrscr();
	ret = iccrw(0,5,icbuf,icreadbuf);
    if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]!=0x90)||(icreadbuf[icreadbuf[0]]!=0x00))
			return SUCC+1;
		
		for (i=1;i<(icreadbuf[0]+1-2);i++)
		{
            if (icreadbuf[i]!=data_in)
			    return SUCC+1;
		}
		return SUCC;
 	}
	else 
	    return SUCC+1;	
}
///////////////////////////////////////////
////       ��SLE4428��������           ////
////      ��00 B0 03 fd 03��           ////	
////       0x3fd ���������            ////
////       0x3fe,0x3fe��Կ             ////
///////////////////////////////////////////
static void readsle4428securitymemory(void)
{
    int i;
	int ret;

    icbuf[0]=0x00;
    icbuf[1]=0xB0;
	icbuf[2]=0x03;
	icbuf[3]=0xfd;  
	icbuf[4]=0x03;
	clrscr();
	ret = iccrw(0,5,icbuf,icreadbuf);
    if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			printf("���������ɹ�\n");
		else 
		{
			printf("��������ʧ��\n");
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0];i++)
		{
            printf("%02x",icreadbuf[i+1]);
		    printf(" ");
		}
  	}
	else 
	{
		printf("��������ʧ��\n");
        getkeycode(0);
		return;
	}
	getkeycode(0);
}

///////////////////////////////////////////
////       дSLE4442��������           ////
////   ��00 D0 C0 01 03 xx xx xx��     ////	
///////////////////////////////////////////
static unsigned char writesle4428securitymemory(unsigned char data_in1,unsigned char data_in2,unsigned char data_in3)
{
	int i;
	int ret;

    icbuf[0]=0x00;
    icbuf[1]=0xD0;
	icbuf[2]=0xC0;
	icbuf[3]=0x01;  
	icbuf[4]=0x03;
	icbuf[5]=data_in1;
	icbuf[6]=data_in2;
	icbuf[7]=data_in3;
	clrscr();
	ret = iccrw(0,8,icbuf,icreadbuf);
    if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			printf("д��Կ�ɹ�\n");
		else 
		{
			printf("д��Կʧ��\n");
			getkeycode(0);
			return SUCC+1;
		}
		for (i=0;i<icreadbuf[0];i++)
		{
            printf("%02x",icreadbuf[i+1]);
		    printf(" ");
		}
  	}
	else 
	{
		printf("д��Կʧ��\n");
        getkeycode(0);
		return SUCC+1;
	}
	getkeycode(5);	
	return SUCC;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//=============================
//    07H��	AT24c64  ��
//=============================
static void poweronat24c64(void);
static void readat24c64(void);
static void writeat24c64(void);
static void powerdownat24c64(void);

static void poweronat24c128(void);
static void readat24c128(void);
static void writeat24c128(void);
static void powerdownat24c128(void);

static void poweronat24c256(void);
static void readat24c256(void);
static void writeat24c256(void);
static void powerdownat24c256(void);

static void testat24c64card(void)  //zjh ���� at24c64 ��
{	    	
	while (1)
	{
		clrscr();
		printf(
			"���ϵ�  ���µ�\n"
			"�۶�����\n"
			"��д����\n"
			  );
		switch(getkeycode(0))
		{
		case '1':
			poweronat24c64();
			break;
		case '2':
			powerdownat24c64();
			break;
		case '3':
			readat24c64();
			break;
		case '4':
			writeat24c64();
			break;
				
		case ESC:
			return;
		default:
			break;
		}
	}
}

static void testat24c128card(void)  //zjh ���� at24c64 ��
{	    	
	while (1)
	{
		clrscr();
		printf(
			"���ϵ�  ���µ�\n"
			"�۶�����\n"
			"��д����\n"
			  );
		switch(getkeycode(0))
		{
		case '1':
			poweronat24c128();
			break;
		case '2':
			powerdownat24c128();
			break;
		case '3':
			readat24c128();
			break;
		case '4':
			writeat24c128();
			break;
				
		case ESC:
			return;
		default:
			break;
		}
	}
}
static void testat24c256card(void)  //zjh ���� at24c64 ��
{	    	
	while (1)
	{
		clrscr();
		printf(
			"���ϵ�  ���µ�\n"
			"�۶�����\n"
			"��д����\n"
			  );
		switch(getkeycode(0))
		{
		case '1':
			poweronat24c256();
			break;
		case '2':
			powerdownat24c256();
			break;
		case '3':
			readat24c256();
			break;
		case '4':
			writeat24c256();
			break;
				
		case ESC:
			return;
		default:
			break;
		}
	}
}

///////////////////////////////////////////
////       �ϵ�at24c64��               ////
////                                   ////	
///////////////////////////////////////////
static void poweronat24c64(void)
{
	int i;

	SetICType(7,0); //�����ж�5002��״̬��ʼ�������û����䣬��˯�ߺ��޷��ϵ�
    clrscr();
	if (icpowerup(0,icbuf)==SUCC)
	    printf("IC���ϵ�ɹ�\n");
	else
	{
        printf("IC���ϵ�ʧ��\n");
		getkeycode(0);
		return;
	}
    for (i=1;i<(icbuf[0]+1);i++)
	{
		printf("%02x",icbuf[i]);
		printf(" ");
	}
	getkeycode(0);
}
static void poweronat24c128(void)
{
	int i;

	SetICType(0x0e,0); //�����ж�5002��״̬��ʼ�������û����䣬��˯�ߺ��޷��ϵ�
    clrscr();
	if (icpowerup(0,icbuf)==SUCC)
	    printf("IC���ϵ�ɹ�\n");
	else
	{
        printf("IC���ϵ�ʧ��\n");
		getkeycode(0);
		return;
	}
    for (i=1;i<(icbuf[0]+1);i++)
	{
		printf("%02x",icbuf[i]);
		printf(" ");
	}
	getkeycode(0);
}
static void poweronat24c256(void)
{
	int i;

	SetICType(0x0f,0); //�����ж�5002��״̬��ʼ�������û����䣬��˯�ߺ��޷��ϵ�
    clrscr();
	if (icpowerup(0,icbuf)==SUCC)
	    printf("IC���ϵ�ɹ�\n");
	else
	{
        printf("IC���ϵ�ʧ��\n");
		getkeycode(0);
		return;
	}
    for (i=1;i<(icbuf[0]+1);i++)
	{
		printf("%02x",icbuf[i]);
		printf(" ");
	}
	getkeycode(0);
}

///////////////////////////////////////////
////       �µ�at24c64��               ////
////                                   ////	
///////////////////////////////////////////
static void powerdownat24c64(void)
{
	clrscr();
	if (icpowerdown(0)==SUCC)
	{
        printf("IC���µ�ɹ�\n");
		getkeycode(2);
	}
	else
	{
        printf("IC���µ�ʧ��\n");
		getkeycode(0);
	}
}
static void powerdownat24c128(void)
{
	clrscr();
	if (icpowerdown(0)==SUCC)
	{
        printf("IC���µ�ɹ�\n");
		getkeycode(2);
	}
	else
	{
        printf("IC���µ�ʧ��\n");
		getkeycode(0);
	}
}
static void powerdownat24c256(void)
{
	clrscr();
	if (icpowerdown(0)==SUCC)
	{
        printf("IC���µ�ɹ�\n");
		getkeycode(2);
	}
	else
	{
        printf("IC���µ�ʧ��\n");
		getkeycode(0);
	}
}

//////////////////////////////////////////////
////       ��at24c64�����ݴ�����          ////
////    ��00  B0  Add1 Add2  Le��         ////	
//////////////////////////////////////////////
static void readat24c64(void)
{
	int i;
	int ret;
	unsigned char bflag=1;
	char buf[5];

	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-8191\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	printf("%02x",ret);
	getkeycode(2);

	icbuf[2] = (ret >> 8);
    	icbuf[3] = (ret & 0xff);
	
	strcpy(buf,"");        //���볤��
	clrscr();
	printf("�����볤��\n");
	printf("0-252\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 12;
	ret=atol(buf);
	printf("%02x",ret);
	getkeycode(2);
	
    	icbuf[4]=(ret & 0xff);


    	icbuf[0]=0x00;
    	icbuf[1]=0xB0;
    	
	clrscr();
	icreadbuf[0] = 0;
	ret = iccrw(0,5,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("��������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				if (bflag!=1)
				{ 
					bflag = 1;
					printf(" ");
				}
				else
					bflag = 0;
			}
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0]-2;i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 		printf(" \n");
		readdata = icreadbuf[i-1]; //��������
 	}
	else 
	{
		printf("��������ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
static void readat24c128(void)
{
	int i;
	int ret;
	unsigned char bflag=1;
	char buf[6];

	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-16383\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	printf("%02x",ret);
	getkeycode(2);

	icbuf[2] = (ret >> 8);
    	icbuf[3] = (ret & 0xff);
	
	strcpy(buf,"");        //���볤��
	clrscr();
	printf("�����볤��\n");
	printf("0-252\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 12;
	ret=atol(buf);
	printf("%02x",ret);
	getkeycode(2);
	
    	icbuf[4]=(ret & 0xff);


    	icbuf[0]=0x00;
    	icbuf[1]=0xB0;
    	
	clrscr();
	icreadbuf[0] = 0;
	ret = iccrw(0,5,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("��������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				if (bflag!=1)
				{ 
					bflag = 1;
					printf(" ");
				}
				else
					bflag = 0;
			}
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0]-2;i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 		printf(" \n");
		readdata = icreadbuf[i-1]; //��������
 	}
	else 
	{
		printf("��������ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
static void readat24c256(void)
{
	int i;
	int ret;
	unsigned char bflag=1;
	char buf[6];

	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-32767\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	printf("%02x",ret);
	getkeycode(2);

	icbuf[2] = (ret >> 8);
    	icbuf[3] = (ret & 0xff);
	
	strcpy(buf,"");        //���볤��
	clrscr();
	printf("�����볤��\n");
	printf("0-252\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 12;
	ret=atol(buf);
	printf("%02x",ret);
	getkeycode(2);
	
    	icbuf[4]=(ret & 0xff);


    	icbuf[0]=0x00;
    	icbuf[1]=0xB0;
    	
	clrscr();
	icreadbuf[0] = 0;
	ret = iccrw(0,5,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("��������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				if (bflag!=1)
				{ 
					bflag = 1;
					printf(" ");
				}
				else
					bflag = 0;
			}
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0]-2;i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 		printf(" \n");
		readdata = icreadbuf[i-1]; //��������
 	}
	else 
	{
		printf("��������ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}


//////////////////////////////////////////////
////       дat24c64�����ݴ�����          ////
////    �� 00   D0  Add1 Add2  Lc  Data�� ////	
//////////////////////////////////////////////
static void writeat24c64(void)
{
	int ret,i;
	char buf[5];

	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-8191\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	icbuf[2] = (ret >> 8);
    	icbuf[3] = (ret & 0xff);

    	icbuf[0]=0x00;
    	icbuf[1]=0xD0;

    	strcpy(buf,"");          //���볤��
	clrscr();
	printf("�����ó���\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
  	icbuf[4]=(ret&0xff);  //����

    	strcpy(buf,"");          //��������
	clrscr();
	printf("������Ҫд������\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	for (i=0;i<icbuf[4];i++)
		icbuf[5+i]=(ret&0xff);
	
	clrscr();
	ret = iccrw(0,5+icbuf[4],icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
		    	printf("д�������ɹ�\n");
		else 
		{
			printf("д������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
			}
			getkeycode(0);
			return;
		}
 	}
	else 
	{
		printf("д������ʧ��\n");
        	getkeycode(0);
		return;
	}
	getkeycode(5);
}


static void writeat24c128(void)
{
	int ret,i;
	char buf[6];

	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-16383\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	icbuf[2] = (ret >> 8);
    	icbuf[3] = (ret & 0xff);

    	icbuf[0]=0x00;
    	icbuf[1]=0xD0;

    	strcpy(buf,"");          //���볤��
	clrscr();
	printf("�����ó���\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
  	icbuf[4]=(ret&0xff);  //����

    	strcpy(buf,"");          //��������
	clrscr();
	printf("������Ҫд������\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	for (i=0;i<icbuf[4];i++)
		icbuf[5+i]=(ret&0xff);
	
	clrscr();
	ret = iccrw(0,5+icbuf[4],icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
		    	printf("д�������ɹ�\n");
		else 
		{
			printf("д������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
			}
			getkeycode(0);
			return;
		}
 	}
	else 
	{
		printf("д������ʧ��\n");
        	getkeycode(0);
		return;
	}
	getkeycode(5);
}
static void writeat24c256(void)
{
	int ret,i;
	char buf[6];

	strcpy(buf,"");          //�����ַ
	clrscr();
	printf("�����õ�ַ\n");
	printf("0-32767\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	icbuf[2] = (ret >> 8);
    	icbuf[3] = (ret & 0xff);

    	icbuf[0]=0x00;
    	icbuf[1]=0xD0;

    	strcpy(buf,"");          //���볤��
	clrscr();
	printf("�����ó���\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
  	icbuf[4]=(ret&0xff);  //����

    	strcpy(buf,"");          //��������
	clrscr();
	printf("������Ҫд������\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

	for (i=0;i<icbuf[4];i++)
		icbuf[5+i]=(ret&0xff);
	
	clrscr();
	ret = iccrw(0,5+icbuf[4],icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
		    	printf("д�������ɹ�\n");
		else 
		{
			printf("д������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
			}
			getkeycode(0);
			return;
		}
 	}
	else 
	{
		printf("д������ʧ��\n");
        	getkeycode(0);
		return;
	}
	getkeycode(5);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//=============================
//    0dH��	AT45db041b  ��
//=============================
static void poweronat45(void);
static void readat45(void);
static void writeat45(void);
static void powerdownat45(void);

static void testat45(void)  
{	    	
	while (1)
	{
		clrscr();
		printf(
			"���ϵ�  ���µ�\n"
			"�۶�����\n"
			"��д����\n"
			  );
		switch(getkeycode(0))
		{
		case '1':
			poweronat45();
			break;
		case '2':
			powerdownat45();
			break;
		case '3':
			readat45();
			break;
		case '4':
			writeat45();
			break;
				
		case ESC:
			return;
		default:
			break;
		}
	}
}
///////////////////////////////////////////
////       �ϵ�at45db041b��            ////
////                                   ////	
///////////////////////////////////////////
static void poweronat45(void)
{
	int i;

	SetICType(0x08,0); //�����ж�5002��״̬��ʼ�������û����䣬��˯�ߺ��޷��ϵ�
    	clrscr();
	if (icpowerup(0,icbuf)==SUCC)
	    	printf("IC���ϵ�ɹ�\n");
	else
	{
        	printf("IC���ϵ�ʧ��\n");
		getkeycode(0);
		return;
	}
    	for (i=1;i<(icbuf[0]+1);i++)
	{
		printf("%02x",icbuf[i]);
		printf(" ");
	}
	getkeycode(0);
}

///////////////////////////////////////////
////       �µ�at45db041b��            ////
////                                   ////	
///////////////////////////////////////////
static void powerdownat45(void)
{
	clrscr();
	if (icpowerdown(0)==SUCC)
	{
        	printf("IC���µ�ɹ�\n");
		getkeycode(2);
	}
	else
	{
        	printf("IC���µ�ʧ��\n");
		getkeycode(0);
	}
}
////////////////////////////////////////////////////
////       ��at45db041b��                       ////
////    ��highadd  B0  midadd  lowadd  Le��     ////	
////////////////////////////////////////////////////
static void readat45(void)
{
	int i;
	int ret;
	unsigned char bflag=1;
	char buf[5];

	strcpy(buf,"0");          //����page��ַ
	clrscr();
	printf("������page��ַ\n");
	printf("0-2047\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	ret = (ret<<1);  //highadd�ĵ�4λ��midadd�ĸ�7λ��page��ַ
    	icbuf[0]=((ret>>8)&0xff);  //highadd
	icbuf[1]=0xB0;             //������
    	icbuf[2]=(ret&0xff);     //midadd

    	strcpy(buf,"0");          //����byte��ַ
	clrscr();
	printf("������byte��ַ\n");
	printf("0-263\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	if (ret>0xff)
		icbuf[2] = (icbuf[2]|1);
	    else
		icbuf[2] = (icbuf[2]&0xfe);
	icbuf[3] = (ret & 0xff);
	
    	strcpy(buf,"4");        //���볤��
	clrscr();
	printf("�����볤��\n");
	printf("0-252\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 12;
	ret=atol(buf);
	icbuf[4]=(ret & 0xff);
		
    	printf("%02x%02x%02x%02x",icbuf[0],icbuf[2],icbuf[3],icbuf[4]);
	getkeycode(0);   
    	
	clrscr();
	icreadbuf[0] = 0;
	ret = iccrw(0,5,icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
        	if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
			clrscr();
		else 
		{
			printf("��������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
				if (bflag!=1)
				{ 
					bflag = 1;
					printf(" ");
				}
				else
					bflag = 0;
			}
			getkeycode(0);
			return;
		}
		for (i=0;i<icreadbuf[0]-2;i++)
		{
            		printf("%02x",icreadbuf[i+1]);
			portwrite(AUX1,1,icreadbuf+i+1);//debug 2005-1-13
            		printf(" ");
		}
 		printf(" \n");
		readdata = icreadbuf[i-1]; //��������
 	}
	else 
	{
		printf("��������ʧ��\n");
		printf("%02x",ret);
		getkeycode(0);
		return;
	}
	getkeycode(0);
}
///////////////////////////////////////////////////////
////       дat45db041b��                          ////
////    �� highadd  D0  midadd  lowadd  Lc  Data�� ////	
///////////////////////////////////////////////////////
static void writeat45(void)
{
	int ret,i;
	char buf[5];

	strcpy(buf,"0");          //����page��ַ
	clrscr();
	printf("������page��ַ\n");
	printf("0-2047\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	ret = (ret<<1);  //highadd�ĵ�4λ��midadd�ĸ�7λ��page��ַ
    	icbuf[0]=((ret>>8)&0xff);  //highadd
	icbuf[1]=0xd0;             //д����
    	icbuf[2]=(ret&0xff);     //midadd

    	strcpy(buf,"0");          //����byte��ַ
	clrscr();
	printf("������byte��ַ\n");
	printf("0-263\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);
	if (ret>0xff)
		icbuf[2] = (icbuf[2]|1);
	    else
		icbuf[2] = (icbuf[2]&0xfe);
	icbuf[3] = (ret & 0xff);
	
    	strcpy(buf,"4");        //���볤��
	clrscr();
	printf("�����볤��\n");
	printf("0-252\n");
	ret=getnumstr(buf,4,2,0);
	if(ret<=0)
		ret = 12;
	ret=atol(buf);
	icbuf[4]=(ret & 0xff);
		
    	printf("%02x%02x%02x%02x",icbuf[0],icbuf[2],icbuf[3],icbuf[4]);
	getkeycode(0);

	strcpy(buf,"0");          //����Ҫд������
	clrscr();
	printf("������Ҫд������\n");
	ret=getnumstr(buf,5,2,0);
	if(ret<=0)
		ret = 0;
	ret=atol(buf);

    	for (i=0;i<icbuf[4];i++)  //��ֵ
		icbuf[5+i] = ret;
	
	clrscr();
	ret = iccrw(0,5+icbuf[4],icbuf,icreadbuf);
    	if(ret==SUCC)
	{   
		if((icreadbuf[icreadbuf[0]-1]==0x90)&&(icreadbuf[icreadbuf[0]]==0x00))
		    	printf("д�������ɹ�\n");
		else 
		{
			printf("д������ʧ��\n");
			for (i=0;i<icreadbuf[0];i++)
			{
				printf("%02x",icreadbuf[i+1]);
				portwrite(AUX1,1,icreadbuf+i);//debug 2005-1-13
			}
			getkeycode(0);
			return;
		}
 	}
	else 
	{
		printf("д������ʧ��\n");
        	getkeycode(0);
		return;
	}
	getkeycode(5);
}

void general26(void)
{
	testmemcard();
	return;
}

#endif


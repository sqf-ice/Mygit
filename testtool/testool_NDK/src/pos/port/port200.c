/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: portͨѶģ��
* file name			: 
* Author 			: liny
* version			: 
* DATE				: 20171201
* directory 			: 
* description			: ����port ͨѶģ�����������
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"
#define MAX_SIZE 4096
#define MAXWAITTIME	10

/*---------------constants/macro definition---------------------*/

#define	TESTAPI	"portģ����������"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef enum {
    	PortOpen,
	PortClose,
	PortRead,
	PortWrite,
	PortClrBuf,
	PortReadLen,
	MaxNum,
}EM_PORT;

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: 
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date			remarks
*						  liny			20171201		created
*****************************************************************/
static int port_randtest(int funnum,EM_PORT_NUM port_type)
{ 
    	char sendBuf[MAX_SIZE] = {0}, recvBuf[MAX_SIZE] = {0};
	int len=0;
    	switch(funnum)
	{	
	    case PortOpen:
			NDK_PortOpen(port_type, "115200,8,N,1");
			break;
		case PortClose:
            		NDK_PortClose(port_type);
			break;
		case PortRead:
			memset(recvBuf,0,sizeof(recvBuf));
	        	NDK_PortRead(port_type,MAX_SIZE, recvBuf, MAXWAITTIME*1000, &len);	
			break;
		case PortWrite:
			NDK_PortWrite(port_type,MAX_SIZE, sendBuf);
			break;
		case PortClrBuf:
		    	NDK_PortClrBuf(port_type);
			break;
		case PortReadLen:
			NDK_PortReadLen(port_type,&len);
			break;
    	}
	return SUCC;
}
static EM_PORT_NUM select_porttype()
{
	/*private & local definition*/
	EM_PORT_NUM type1 = PORT_NUM_COM1;
	int nKeyin = 0;
    	nKeyin= cls_show_msg("ѡ�񴮿�����:\n"
    						"1.����1\n"
						"2.����2\n"
						"3.USB����\n");
	switch(nKeyin) 
	{ 
	    case '1':
			type1=PORT_NUM_COM1;
			break;
		case '2':
			type1=PORT_NUM_COM2;
			break;
		case '3':
			type1=PORT_NUM_USB;
			break;	
		default:
			type1=PORT_NUM_COM1;
			break;
	}
	return type1;
}
void randtest(EM_PORT_NUM type)
{
   	 /*private & local definition*/
	int num[50] ={0}, i = 0, bak = 0;
	int ret = -1,cnt =50, len = 0,j=0;//cnt ����
	char *portstr[] = {"NDK_PortOpen","NDK_PortClose","NDK_PortRead","NDK_PortWrite","NDK_PortClrBuf","NDK_PortReadLen"};	

	/*process body*/
	len = (MaxNum>20)?MaxNum:20;
	 while(cnt)
	{	 
		 cnt--;
		 bak++;
	
		 //���������
		 srand((unsigned)time(NULL));
		 for(i=0;i<len;i++)
		 {		 
			  num[i] = rand()%MaxNum;
		 }
		 cls_printf("��%d�����˳��:",bak);
		 for(i=0;i<len;i++)
		 {
			  NDK_ScrPrintf("%d ", num[i]);
			  NDK_ScrRefresh();
		 }
	 
		 //�����������
		 for(i=0;i<len;i++)
		 {
			 if((ret = port_randtest(num[i],type)) != NDK_OK)
			 {
				  send_result("line %d:%s��%d�β���ʧ��(%d,%d)", __LINE__, TESTAPI, bak,ret,num[i]);
				  for(j=0;j<=i;j++)
				  send_result("%s ",portstr[num[j]]);
			  }
		 }
	 }
	 NDK_PortClose(type); //���Ժ���,��������,�رմ���
	 send_result("%s����ͨ��", TESTAPI);
}

void port200(void)
{
    	int keyin = 0;
    	EM_PORT_NUM type = PORT_NUM_COM1;

	cls_show_msg1(30,"����ǰ�Ƚ��д�������,���������");
	
	while(1)
	{
		keyin = cls_show_msg("1.�������\n"
							"0.����");
		switch(keyin) 
		{
		case '1':
			cls_show_msg1(2, "portģ���ں���������ò���");
			randtest(type);
	        break;
		case '0':
			type = select_porttype(); 
			cls_show_msg1(2, "�������óɹ�!");
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
	



/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module				: sdlcģ�� 
* file name				: 
* Author 				: 
* version				: 
* DATE					: 
* directory 				: 
* description			: sdlcģ���������ϲ���
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTAPI	"ͬ��ģ����������"
#define MAXWAITTIME 10


/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/
typedef enum {
	MdmSdlcInit,
	MdmDial,
	MdmCheck,
	MdmWrite,
	MdmRead,
	MdmHangup,
	MdmClrbuf,
	MdmGetreadlen,
	MdmExCommand,
	MdmReset,
	MdmAdapt,
	MdmGetBps,
	MdmSdlcSingleForceInit,
	MaxNum,//����ö��ֵʱ�ڴ�ֵǰ���,Ҫ��֤��ֵΪ���һ��ֵ
}EM_SDLC;

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: 
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						 sull				20180212		created
*****************************************************************/
static int sdlc_randtest(int funnum)
{
	int ret = -1, j = 0;
	EM_MDMSTATUS MdmStatus;	
	EM_MDM_BPS bps=0;
	char buf[ASYNPCKTMAXLEN+1] = {0}, rbuf[512] = {0};
	uint rlen = 0, GetReadLen = -1, rLen = -1, unTimeout = 0;
	uchar pucCmdstr[128]={0}, pszRespData[128]={0};

	memset(buf, 0, sizeof(buf));
	for(j=0; j<sizeof(buf); j++)
		buf[j] = rand()%256;
	switch(funnum)
	{
		case MdmSdlcInit:
			if((ret = NDK_MdmSdlcInit(rand()%5+1)) != NDK_OK)
				return ret;
			break;
		case MdmDial:
			NDK_MdmDial(MDMDialStr);
			break;
		case MdmCheck:
			NDK_MdmCheck(&MdmStatus);
			break;
		case MdmWrite:
			NDK_MdmWrite(buf,sizeof(buf));
			break;
		case MdmRead:
			memset(rbuf, 0, sizeof(rbuf));
			rlen = sizeof(buf)+1;//������ȡ�������ݳ���Ҫ���ڵ���ʵ���ܶ�ȡ�������ݳ��ȡ�
			NDK_MdmRead(rbuf, &rlen, MAXWAITTIME);
		case MdmHangup:
			NDK_MdmHangup();
			break;
		case MdmClrbuf:
			if((ret = NDK_MdmClrbuf()) != NDK_OK)
				return ret;
			break;
		case MdmGetreadlen:
			NDK_MdmGetreadlen(&GetReadLen);
			break;
		case MdmExCommand:
			sprintf((char*)pucCmdstr,"ATE1\r");
			if((ret=NDK_MdmExCommand(pucCmdstr,pszRespData,&rLen,unTimeout))!=NDK_OK)
				return ret;
			break;
		case MdmReset:
			if((ret = NDK_MdmReset()) != NDK_OK)
				return ret;
			break;
		case MdmAdapt:
			NDK_MdmAdapt(MODEM_DIAL_TYPE_SDLC, MDMDialStr);
			break;
		case MdmGetBps:
			NDK_MdmGetBps(&bps);
			break;
		case MdmSdlcSingleForceInit:
			//if((ret = NDK_MdmSdlcSingleForceInit(rand()%5+1, rand()%3+1)) != NDK_OK) �ڳɹ����Ӻ�̨������£����ʼ��ʧ�ܣ�ȥ���ж� 20180309 modify
				//return ret;
			NDK_MdmSdlcSingleForceInit(rand()%5+1, rand()%3+1);
			break;
	}		
	return SUCC;
}

void sdlc200(void)
{
	/*private & local definition*/
	int  i = 0, bak = 0, num[30] ={0}, j = 0; 
	int ret = -1,cnt =50, len = 0;//cnt ����
	char *sdlcstr[] = {"NDK_MdmSdlcInit", "NDK_MdmDial", "NDK_MdmCheck", "NDK_MdmWrite", "NDK_MdmRead", "NDK_MdmHangup", "NDK_MdmClrbuf",
				"NDK_MdmGetreadlen","NDK_MdmExCommand", "NDK_MdmReset", "NDK_MdmAdapt","NDK_MdmGetBps","NDK_MdmSdlcSingleForceInit"};

	/*process body*/
	len = (MaxNum>20)?MaxNum:20;
	while(cnt)
	{	
		cnt--;
		bak++;
		
		//���������
		cls_printf("��%d�����˳��:",bak);
		srand((unsigned)time(NULL));
		for(i=0;i<len;i++)
		{	
			num[i] = rand()%MaxNum;
			NDK_ScrPrintf("%d ", num[i]);
			NDK_ScrRefresh();
		}
		
		//�����������
		for(i=0;i<len;i++)
		{
			if((ret = sdlc_randtest(num[i])) != NDK_OK)
			{
				send_result("line %d:%s��%d�β���ʧ��(%d,%d)", __LINE__, TESTAPI, bak,ret,num[i]);
				for(j=0;j<=i;j++)
					send_result("%s ",sdlcstr[num[j]]);
				goto ERR;
			}
		}	
	}

	//test over
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_MdmHangup();
	return;
}



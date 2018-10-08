/************************************************************************
* copyright(c)2005-2015 Newland Co. Ltd. All rights reserved
* module			:����
* file name			: bt21.c
* Author 			: 
* version			: 
* DATE				:
* directory 			: 
* description			: ����������ģʽ��NDK_BTMasterGetScanResults �Ƿ����ɨ�赽��Ҫ��ɨ����
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_BTMasterGetScanResults"	
#define EXPMAXNUM 10

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt21
* functional description 	: 
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*						  zhengry			20150602		created
*****************************************************************/

void bt21(void)	//�޲����޷���
{
	/*private & local definition*/
	ST_BT_DEV sresult[50],sresult1[50];
	int ret=0,i=0,snum=0,snum1=0,snum2=0;	
	char *inbuf[3]={"newland",
				    "����",
				    "`~!\"@$#[^%]*()-_=+|&{}:;?/><,"};
	memset(sresult,0,sizeof(sresult));
	memset(sresult1,0,sizeof(sresult1));
	
	/*process body*/
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��
	NDK_PortClose(PORT_NUM_BT);

	//case1����������δ�򿪣���ȡɨ����Ӧ��ʧ��
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult,EXPMAXNUM,&snum))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;	
	}
	
	//����������
	if((ret=NDK_PortOpen(PORT_NUM_BT,NULL))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}

	//case2:�մ򿪴��ڣ�δ���ÿ�ʼɨ�躯������µ��û�ȡɨ�躯��Ԥ�ڳɹ�,��Ӧ��ɨ�赽0��
	memset(sresult,0,sizeof(sresult));
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult,5,&snum))!=NDK_OK||snum!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, snum);//���ִ����20��������ִ�б������ڴ˴�����snum=5
		GOTOERR;	
	}

	//case3�������쳣����ʧ��	
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_BREDR))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterGetScanResults(NULL,NULL,EXPMAXNUM,&snum))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult,EXPMAXNUM,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}		
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}

	//case4���趨ָ�����豸���ƣ����ƿ���Ϊ������Ӣ�ģ������ַ����������ƣ���ɨ����Ӧֻ�Ǹ����Ƶ��豸�һ�ȡ����Ϊ1 
	for(i=0;i<3;i++)// 3ֻ������������Ϊ29�ֽڵ���������
	{
		cls_show_msg("�뽫1̨�������豸�������޸�Ϊ:%s���޸ĺ��밴�����������",inbuf[i]); 
		if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;	
		}
		sleep(10);//��һ����ɨ��ʱ��
		memset(sresult,0,sizeof(sresult));//ÿ�λ�ȡǰ��� �Ա�������´εĽ��
		if((ret=NDK_BTMasterGetScanResults(inbuf[i],sresult,EXPMAXNUM,&snum))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;	
		}
		//cls_show_msg("snum=%d,i=%d",snum, i);
		//ɨ���һ���ַ���newland�����snum=0,i=0����ʵ��pos����ɨ�赽��̨�����豸����ɨ������ĿȴΪ0
		if(strcmp(sresult[0].sBtName,inbuf[i])) 
		{
			send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, sresult[0].sBtName);
			GOTOERR;
		}	
		if((ret=NDK_BTMasterStopScan())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			GOTOERR;	
		}
	}
	
	//case5���趨2̨��ͬ���Ƶ��豸��ɨ����Ӧ����ɨ�赽����̨�豸
	cls_show_msg("�뽫2̨�������豸�������޸�Ϊ:%s���޸ĺ��밴�����������",inbuf[0]); 
	cls_printf("����ɨ���У���ȴ�30��...");
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(30);//��һ����ɨ��ʱ��
	memset(sresult,0,sizeof(sresult));
	if((ret=NDK_BTMasterGetScanResults(inbuf[0],sresult,EXPMAXNUM,&snum))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	if(snum<2)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, snum);
		GOTOERR;	
	}
	for(i=0;i<snum;i++)
		cls_show_msg("%d.��������:%s\nMAC��ַ:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x:\nRSSI:%d\n�豸����:%d",i+1,sresult[i].sBtName,sresult[i].sBtMac[0],sresult[i].sBtMac[1],sresult[i].sBtMac[2],sresult[i].sBtMac[3],sresult[i].sBtMac[4],sresult[i].sBtMac[5],sresult[i].nRssi,sresult[i].emBtType);
	if(cls_show_msg("ɨ�赽��������Ϊ%s�������豸��Ϣ�Ƿ���ȷ:��[Enter],��[����]",inbuf[0])!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	
	//case6���趨ϣ�����������ĿΪ0̨ʱ���ж�ɨ��Ľ����Ŀ�Ƿ�Ϊ0̨
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(10);//��һ����ɨ��ʱ��
	memset(sresult,0,sizeof(sresult));
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult,0,&snum))!=NDK_OK||snum!=0)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, snum);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}

	//case7:�趨ϣ������������Ŀ����ʵ�ʵ���ĿС���趨��������Ŀ��ɨ��Ľ����ĿӦ����ʵ�ʵ�������Ŀ
	cls_show_msg("��ȷ����ʱ���豸С��20̨,ȷ�Ϻ��밴���������");
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(10);//��һ����ɨ��ʱ��
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult1,20,&snum1))!=NDK_OK||snum1>20)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, snum1);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}

	//ʵ�ʲ��Ե�ɨ����ĿΪ6̨����Ԥ��2̨�����ɨ����Ŀ������
	//case8���趨ϣ������������Ŀ����ʵ�ʵ���Ŀ�����趨��������Ŀ��ɨ��Ľ����ĿӦ�����趨��������Ŀ	
	cls_show_msg("��ȷ����ʱ���豸����%d̨,ȷ�Ϻ��밴���������",EXPMAXNUM);
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(10);//��һ����ɨ��ʱ��
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult1,2,&snum1))!=NDK_OK||snum1!=2)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, snum1);
		GOTOERR;	
	}
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}

	//case9:��ʼɨ���豸���ټ��������豸����ȡɨ����Ӧ���ܼ�ʱ�����������豸
	cls_show_msg("���1̨��Ϊ\"6bt\"���������豸���򿪺��밴���������");
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(10);//��һ����ɨ��ʱ��
	memset(sresult1,0,sizeof(sresult1));
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult1,50,&snum1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	for(i=0;i<snum1;i++)
	{
		if(!strcmp(sresult1[i].sBtName,"6bt"))
			break;
	}
	if(i==snum1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	//case9:�ر�ĳ̨�豸��,���û������ɨ����Ӧ����Ȼ����ɨ�赽�ùرյ��豸
	cls_show_msg("��ر���Ϊ\"6bt\"���豸���رպ��밴���������");
	sleep(10);//��һ����ɨ��ʱ��
	memset(sresult1,0,sizeof(sresult1));
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult1,50,&snum2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	for(i=0;i<snum2;i++)
	{
		if(!strcmp(sresult1[i].sBtName,"6bt"))
				break;
	}
	if(i==snum1)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

	//ɨ�����,ֹͣɨ��
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}

	//case10:�ر�ĳ̨�豸��,���·���ɨ��,Ӧ��ɨ�費�����رյ��豸
	cls_show_msg("��ر���Ϊ\"6bt\"���豸���رպ��밴���������");
	if((ret=NDK_BTMasterScan(BT_DEVICE_TYPE_DUMO))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	sleep(10);//��һ����ɨ��ʱ��
	memset(sresult1,0,sizeof(sresult1));
	if((ret=NDK_BTMasterGetScanResults(NULL,sresult1,50,&snum2))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;	
	}
	for(i=0;i<snum2;i++)
	{
		if(!strcmp(sresult1[i].sBtName,"6bt"))
		{
			send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
			GOTOERR;
		}
	}
	if((ret=NDK_BTMasterStopScan())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;	
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	NDK_BTMasterStopScan();
	NDK_PortClose(PORT_NUM_BT);
	return;
}


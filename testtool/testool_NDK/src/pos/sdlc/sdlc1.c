/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: sdlc1.c
* Author 			: chenjings
* version			: 
* DATE			: 20130106
* directory 		: src/pos/sdlc
* description		: ����NDK_MdmSdlcInit�ܷ������MODEM����ͬ����ʽ�ĳ�ʼ��
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmSdlcInit"
#define	MAXWAITTIME	60	//60��
#define PCKTMAXLEN 350

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
*			  	  	chenjings      20130106	created
*****************************************************************/
void sdlc1(void)
{
	//����Ϊ�ֲ�����
	int i = -1, j = 0, ret = 0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0};
	
	//����Ϊ������
	if(auto_flag==2)
	{
		//send_result("line %d:%s�޴��ֶ���������", __LINE__, TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); 

	if((ret=NDK_MdmReset())!=NDK_OK )
	{
		send_result("line %d:%s��λʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		return;
	}
	while (1)
	{           
		cls_printf("%s������(i=%d)...", TESTAPI, i);//��Ļ��ʾ��ǰ��������ģ��������
		ret = NDK_MdmSdlcInit(i);
		//case1: ����EM_MDM_PatchType ���ͣ��ж��Ƿ񷵻ز��ɹ�	
		if(ret!=NDK_OK)
		{
			if((i >=MDM_PatchType0) && (i <=MDM_PatchType5)) //case1.1: ��ȷ��EM_MDM_PatchType ����(i = 0 1 2 3 4 5)��Ӧ���سɹ�,���򱨴�
			{
				send_result("line %d:%sʧ��(%d,%d)", __LINE__, TESTAPI, i, ret);
				RETURN;
			}
			else if(ret!=NDK_ERR_PARA)	//case1.2: �����EM_MDM_PatchType (i = -1 6 7)���ͣ�Ӧ���سɹ�NDK_ERR_PARA
			{
				send_result("line %d:%sʧ��(%d,%d)", __LINE__, TESTAPI, i, ret);
				RETURN;
			}
		}
		else 
		{
			//case2: ѡȡ������ȷEM_MDM_PatchType ����ʱ�������ж��Ƿ�NDK_MdmSdlcInit������
			if((i >= 0) && (i <= 5))
			{
				//case2.1:�Ƿ񲦺ųɹ�
				NDK_MdmClrbuf();
				if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
				{
					send_result("line %d:%s��%d�β���ʧ��(DialNum:%s,ret:%d)", __LINE__, TESTAPI, i, MDMDialStr, ret);
					GOTOERR;
				}
				//case2.2:�ж�modem״̬���Ƿ����ӳɹ�
				if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
				{
					send_result("line %d:%s modem״̬δ��ͨ(DialNum:%s,MdmStatus:%d)", __LINE__, TESTAPI, MDMDialStr, ret);
					GOTOERR;
				}
				//case2.3:��ʼ��buf
				memset(buf, 0, sizeof(buf));
				for(j=0; j<sizeof(buf); j++)
					buf[j] = rand()%256;
				//case2.4:�Ƿ�������д����				
				update_SDLC_packet(buf, sizeof(buf)); //���TPDU��ͷ
				if((ret=NDK_MdmWrite(buf,PCKTMAXLEN))!=NDK_OK)
				{
					send_result("line %d:%s ��%d��д����ʧ��(ret=%d)", __LINE__, TESTAPI, i, ret);
					GOTOERR;
				}
				//case2.5:�Ƿ�������������
				memset(rbuf, 0, sizeof(rbuf));
				rlen = 0;//��ֹSDLC_readû�ж�rlen����0
				if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
				{
					send_result("line %d:%s ��%d�ζ�����ʧ��(ret=%d)", __LINE__, TESTAPI, i, ret);
					GOTOERR;
				}
				//case2.6:�Ƚ϶�д�����Ƿ�һ��				
				if((PCKTMAXLEN!=rlen)||MemCmp(buf, rbuf, rlen))
				{
					send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, PCKTMAXLEN, rlen);
					GOTOERR;
				}
				//case2.7:�Ƿ��������һ�
				if((ret=NDK_MdmHangup())!=NDK_OK)
				{
					send_result("line %d:%s��%d�ιҶ�ʧ��(ret=%d)", __LINE__, TESTAPI, i, ret);
					RETURN;
				}
				sleep(3);//�ȴ�MODEMоƬ�Ҷ�,�����һ��û��ȫ�ҶϾ�ȥ���Ŷ�ʧ��(��ʱ���Ӳ��оƬ�й�ϵ)
			}
			else//����EM_MDM_PatchType ����
			{
				send_result("line %d:%sʧ��(%d,%d)", __LINE__, TESTAPI, i, ret);
				RETURN;
			}
		}		
		if(++i>7)  //i=6��i=7��Ϊ����ֵ֤������Χ�����
			break;
	}

	//���Խ���
	NDK_MdmHangup();//����
	send_result("%s����ͨ��", TESTAPI);
	return;
	goto ERR;
ERR:
	NDK_MdmHangup();
	return;
}


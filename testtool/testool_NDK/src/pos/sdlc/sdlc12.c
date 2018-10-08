/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sdlcģ��
* file name		: sdlc12.c
* Author 			: zhengry
* version			: 
* DATE			: 20160912
* directory 		: src/pos/sdlc
* description		: ����NDK_MdmGetBps��ȡ��ǰ���ӵĲ����ʺ�NDK_MdmSdlcSingleForceInitͬ�����ų�ʼ��������������Ч��
* related document	: SDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_MdmGetBps,NDK_MdmSdlcSingleForceInit"
#define	PCKTMAXLEN	350
#define	MAXWAITTIME	60	//60��

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
static int mdm_link_com(void)
{
	int ret=0, i=0;
	uint rlen = 0;
	char buf[512] = {0}, rbuf[512] = {0};
	
	//��ʼ�����ݰ�
	memset(buf, 0, sizeof(buf));
	for(i=0; i<sizeof(buf); i++)
		buf[i] = rand()%256;
	update_SDLC_packet(buf, sizeof(buf)); //���TPDU��ͷ

	//�Ƿ񲦺ųɹ�
	NDK_MdmClrbuf();
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(DialNum:%s,ret:%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		return ret;
	}
	//�ж�modem״̬���Ƿ����ӳɹ�
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%s modem״̬δ��ͨ(DialNum:%s,MdmStatus:%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		return NDK_ERR;
	}
	//�Ƿ�������д����				
	if((ret=NDK_MdmWrite(buf,PCKTMAXLEN))!=NDK_OK)
	{
		send_result("line %d:%s д����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		return ret;
	}
	//�Ƿ�������������
	memset(rbuf, 0, sizeof(rbuf));
	rlen = 0;//��ֹSDLC_readû�ж�rlen����0
	if((ret=NDK_MdmRead(rbuf, &rlen, MAXWAITTIME))!=NDK_OK)
	{
		send_result("line %d:%s ������ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		return ret;
	}
	//�Ƚ϶�д�����Ƿ�һ��				
	if((PCKTMAXLEN!=rlen)||MemCmp(buf, rbuf, rlen))
	{
		send_result("line %d:%s�շ����ݲ�һ��(W=%d,R=%ld)", __LINE__, TESTAPI, PCKTMAXLEN, rlen);
		return NDK_ERR;
	}
	
	return NDK_OK;
}

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	   zhengry   20160704	created
*****************************************************************/
void sdlc12(void)
{
	//����Ϊ�ֲ�����
	int ret = 0;
	EM_MDM_BPS bps=0;
	EM_MDM_PatchType i=0;
	EM_MDMSTATUS MdmStatus;
	
	//����Ϊ������
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	cls_printf("����%s...", TESTAPI); 
	
	//����ǰ��:
	if((ret=NDK_MdmReset())!=NDK_OK )
	{
		send_result("line %d:%s��λʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		return;
	}
	//case1:��ʼ��ǰ����ȡbpsӦ��ʧ��
	if((ret=NDK_MdmGetBps(&bps))!=NDK_ERR) 
	{
		send_result("line %d:%s����ʧ��(ret=%d, bps=%d)", __LINE__, TESTAPI, ret, bps);
		return;
	}

	cls_show_msg("���ڿ���̨/mnt/hwinfo/sys.conf�ļ��У��鿴mdm��baud_freq��Ĭ�ϲ����ʲ���¼�����������");
	while (1)
	{           
		cls_printf("%s������(i=%d)...", TESTAPI, i);
		ret = NDK_MdmSdlcSingleForceInit(i, EM_MDM_BPS_2400);//��ʱNDK_MdmSdlcSingleForceInit���õĲ�����ӦΪ��Ĭ�ϲ����� 
		//case2: ����EM_MDM_PatchType ���ͣ��ж��Ƿ񷵻ز��ɹ�	
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
			//case3: ѡȡ������ȷEM_MDM_PatchType ����ʱ�������ж��Ƿ�NDK_MdmSdlcSingleForceInit������
			if((i >= 0) && (i <= 5))
			{
				//mdm����
				if((ret=mdm_link_com())!=NDK_OK)
				{
					send_result("line %d:%s��%d��mdm����ʧ��(ret=%d)", __LINE__, TESTAPI, i, ret);
					GOTOERR;
				}
				//case4:NDK_MdmGetBpsΪ��ȡʵ�����ӵ�BPS
				if(((ret=NDK_MdmGetBps(&bps))!=NDK_OK) || (bps!=EM_MDM_BPS_2400)) 
				{
					send_result("line %d:%s��%d�β���ʧ��(ret=%d, bps=%d)", __LINE__, TESTAPI, i, ret, bps);
					GOTOERR;
				}
				//�Ƿ��������һ�
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
	if(cls_show_msg("����/mnt/hwinfo/sys.conf�ļ��У��鿴mdm��baud_freq��ֵ��Ӧ���޸�Ϊ2(EM_MDM_BPS_2400),��[ENTER],����[����]") != ENTER)
	{
		send_result("line %d:%sĬ�ϲ����ʱ��޸�", __LINE__, TESTAPI);
		RETURN;
	}
	
	//case5:�ҶϺ󣬻�ȡbpsӦ��ʧ��
	if((ret=NDK_MdmGetBps(&bps))!=NDK_ERR) 
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case6:������NDK_MdmSdlcSingleForceInit����bps,������ʼ�������ղ��������ļ��е�Ĭ��bps���в���
	if((ret=NDK_MdmSdlcInit(MDMPatchType))!=NDK_OK)
	{
		send_result("line %d:%s��ʼ����(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN; 
	}	
	NDK_MdmClrbuf();
	if((ret=NDK_MdmDial(MDMDialStr))!=NDK_OK)
	{
		send_result("line %d:%s ����ʧ��(DialNum:%s,ret:%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	if((ret=mdm_detectCONNECT())!=MDMSTATUS_CONNECT_AFTERPREDIAL)
	{
		send_result("line %d:%s modem״̬δ��ͨ(DialNum:%s,MdmStatus:%d)", __LINE__, TESTAPI, MDMDialStr, ret);
		GOTOERR;
	}
	//case7:NDK_MdmGetBpsΪ��ȡʵ�����ӵ�BPS����ʱӦΪ���������ļ��е�Ĭ��bps
	if(((ret=NDK_MdmGetBps(&bps))!=NDK_OK) || (bps!=EM_MDM_BPS_1200)) 
	{
		send_result("line %d:%s����ʧ��(ret=%d, bps=%d)", __LINE__, TESTAPI, ret, bps);
		GOTOERR;
	}
	if((ret=NDK_MdmHangup())!=NDK_OK)
	{
		send_result("line %d:%s �Ҷ�ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case8:���Գ�ʼ����������bps=9600,Ӧ����������
	if((ret = NDK_MdmSdlcSingleForceInit(MDMPatchType, EM_MDM_BPS_9600))!=NDK_OK )
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//mdm����
	if((ret=mdm_link_com())!=NDK_OK)
	{
		send_result("line %d:%s mdm����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(((ret=NDK_MdmGetBps(&bps))!=NDK_OK) || (bps!=EM_MDM_BPS_9600))
	{
		send_result("line %d:%s����ʧ��(ret=%d, bps=%d)", __LINE__, TESTAPI, ret, bps);
		GOTOERR;
	}
	if((ret=NDK_MdmHangup())!=NDK_OK)
	{
		send_result("line %d:%s �Ҷ�ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(cls_show_msg("����/mnt/hwinfo/sys.conf�ļ��У��鿴mdm��baud_freq��ֵ��Ӧ���޸�Ϊ3(EM_MDM_BPS_9600),��[ENTER],����[����]") != ENTER)
	{
		send_result("line %d:%sĬ�ϲ����ʱ��޸�", __LINE__, TESTAPI);
		RETURN;
	}
	sleep(SLEEP_SDLC_HANGUP);//������λ�����������ʱ�ᵼ���¸����ŷ���æ״̬,���������ύBUG,���������޸�,�����Ͽ���Ҫ6��8��,�ۺϲ��ֹҶϺ�����5����ʱ,��������Ҳ�ĳ�5����ʱ
	//case9:��bps=9600���bps=2400,Ӧ���������Ŷ�д
	if((ret = NDK_MdmSdlcSingleForceInit(MDMPatchType, EM_MDM_BPS_2400))!=NDK_OK )
	{
		send_result("line %d:%s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//mdm����
	if((ret=mdm_link_com())!=NDK_OK)
	{
		send_result("line %d:%s mdm����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		GOTOERR; 
	}
	//case10:NDK_MdmGetBpsΪ��ȡʵ�����ӵ�BPS
	if(((ret=NDK_MdmGetBps(&bps))!=NDK_OK) || (bps!=EM_MDM_BPS_2400))
	{
		send_result("line %d:%s����ʧ��(ret=%d, bps=%d)", __LINE__, TESTAPI, ret, bps);
		GOTOERR;
	}
	if((ret=NDK_MdmHangup())!=NDK_OK)
	{
		send_result("line %d:%s �Ҷ�ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case11:�С���أ�Ӧ����ֻ֧��bps=1200��ʹ��
	if(cls_show_msg("��С���ػ���,����bps=1200����,����[ESC]")!=ESC)
	{
		//��������С���ز���
		if(conf_link(SYNC)!=SUCC)
		{
			send_result("line %d:%sС����ͬ������ʧ��", __LINE__, TESTAPI);
			return;
		}
		//bps=2400��9600��Ԥ�ڻ��Զ�Э�̻��䲨����,����ʵ�ʵ�1200������������
		//bps=1200��Ԥ���ܹ���������
		for(i=1; i<=EM_MDM_BPS_9600; i++)
		{
			if((ret = NDK_MdmSdlcSingleForceInit(MDMPatchType, i))!=NDK_OK )
			{
				send_result("line %d:%s����ʧ��(ret=%d, i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR;
			}
			if((ret=mdm_link_com())!=NDK_OK)
			{
				send_result("line %d:%s mdm����ʧ��(ret=%d, i=%d)", __LINE__, TESTAPI, ret, i);
				GOTOERR; 
			}
			if(((ret=NDK_MdmGetBps(&bps))!=NDK_OK) || (bps!=EM_MDM_BPS_1200))
			{
				send_result("line %d:%s����ʧ��(ret=%d, bps=%d, i=%d)", __LINE__, TESTAPI, ret, bps, i);
				GOTOERR;
			}
			if((ret=NDK_MdmHangup())!=NDK_OK)
			{
				send_result("line %d:%s �Ҷ�ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
				RETURN;
			}
		}
	}

	//case12:δ��������»�ȡ������Ӧ��ʧ��
	if((ret=NDK_MdmGetBps(&bps))!=NDK_ERR) 
	{
		send_result("line %d:%s����ʧ��(ret=%d, bps=%d)", __LINE__, TESTAPI, ret, bps);
		return;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_MdmHangup();
	return;
}


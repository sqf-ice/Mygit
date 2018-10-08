/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: socketģ��
* file name			: sckt10.c
* Author 			: 
* version			: 
* DATE				:
* directory 	: 
* description	: ����NDK_TcpSetKeepAlive socket�����Ƿ��ܹ������
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define 	TESTAPI		"NDK_TcpSetKeepAlive"

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
* history 		 				:  author		date		remarks
*			  	  	  			jiangym		20160726 created
*****************************************************************/
void sckt10(void)
{
	//����Ϊ�ֲ�����
	int ret = -1;
	uint h_tcp = 0; 

	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤",TESTAPI);
		return;
	}
	//����Ϊ������
	cls_printf("����%s...", TESTAPI);//��Ļ��ʾ��ǰ���ڵĲ�������
	
	//case1:�쳣��������	
	if((ret=NDK_TcpSetKeepAlive(-1,10,65,9)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TcpSetKeepAlive(2,10,65,9)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TcpSetKeepAlive(1,-1,65,9)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TcpSetKeepAlive(1,36001,65,9)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TcpSetKeepAlive(0,10,-1,9)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TcpSetKeepAlive(0,10,751,9)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TcpSetKeepAlive(1,10,65,-1)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_TcpSetKeepAlive(1,10,65,51)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:����ʹ�ò���:������δ����ǰ���ÿ���, ������ץ�������ܹ�ץ��
	if((ret=NDK_TcpSetKeepAlive(1,5,65,9)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(NetUp()!=SUCC)
	{
		send_result("line %d:�����UPʧ��", __LINE__);
		GOTOERR;
	}
	if(TransUp(&h_tcp)!=SUCC)
	{
		send_result("line %d:�����UPʧ��", __LINE__);
		GOTOERR;
	}
	cls_show_msg("ʹ��ץ�����߽��в鿴���������Ƿ�����ÿ���5sʱ�����յ����ְ�,Ԥ����");
	if(cls_show_msg("Ԥ��ÿ���5s�յ����ְ�,��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	//case3:�ڴ���㽨���������ȥ�޸�����Ϊ�رյĹ��� ץ�����ֲ�û����Ч
	if((ret=NDK_TcpSetKeepAlive(0,10,0,0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("ʹ��ץ�����߽��в鿴���������Ƿ�����ÿ���5sʱ�����յ����ְ�,Ԥ����");
	if(cls_show_msg("Ԥ��ÿ���5s�յ����ְ�,��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	
	if(TransDown(h_tcp)!=SUCC)
	{
		send_result("line %d:�����Downʧ��", __LINE__);
		GOTOERR;
	}
	
	//case4:�ڴ����Ҷϵ������ȥ�޸�����Ϊ�رյĹ��� ץ��Ӧ���ܷ���ץ������
	if((ret=NDK_TcpSetKeepAlive(0,10,0,0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(TransUp(&h_tcp)!=SUCC)
	{
		send_result("line %d:�����UPʧ��", __LINE__);
		GOTOERR;
	}
	cls_show_msg("ʹ��ץ�����߽��в鿴���������Ƿ�����ÿ���10sʱ�����յ����ְ�,Ԥ��û��");
	if(cls_show_msg("Ԥ��û���յ����ְ�,��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}	
	if(TransDown(h_tcp)!=SUCC)
	{
		send_result("line %d:�����Downʧ��", __LINE__);
		GOTOERR;
	}
	
	//case5:�ڴ����Ҷϵ���������ÿ����� ����ȫ����0 ��ʹ��Ĭ��ֵ ���10s
	if((ret=NDK_TcpSetKeepAlive(1,0,0,0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(TransUp(&h_tcp)!=SUCC)
	{
		send_result("line %d:�����UPʧ��", __LINE__);
		GOTOERR;
	}
	cls_show_msg("ʹ��ץ�����߽��в鿴���������Ƿ����ڼ��10sʱ�����յ����ְ�,Ԥ����");
	if(cls_show_msg("Ԥ��ÿ���10s�յ����ְ�,��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}

	//case6:��֤�ط�ʱ�估�ط�����,���ط���������Ҫʹ��clumsy���߶����������������������ݰ�
	if(TransDown(h_tcp)!=SUCC)
	{
		send_result("line %d:�����Downʧ��", __LINE__);
		GOTOERR;
	}	
	if((ret=NDK_TcpSetKeepAlive(1,5,2,9)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(TransUp(&h_tcp)!=SUCC)
	{
		send_result("line %d:�����UPʧ��", __LINE__);
		GOTOERR;
	}
	cls_show_msg("ʹ��clumsy���߶����������������������ݰ���鿴ץ������,���������");
	if(cls_show_msg("ץ�������Ƿ����յ�pos�ط�����(2sһ��,�ط�9��),��ȷ[Enter],����[����]")!=ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto ERR;
	}

	//���Խ���
	send_result("%s����ͨ��", TESTAPI); 
ERR:
	TransDown(h_tcp);
	NDK_TcpSetKeepAlive(0,0,0,0);
	NetDown();
	return;
}


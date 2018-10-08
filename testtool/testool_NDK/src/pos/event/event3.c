/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: eventģ��
* file name		: event3.c
* Author 		:  
* version		: 
* DATE			
* directory 		: 
* description		: ����NDK_SYS_ResumeEvent�����¼�����
* related document	: �߶�ƽ̨ͳһAPI��̬���ӿ����ֲ�V2.00.doc
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysResumeEvent"
#define MAXWAITTIME	30
#define PINTIMEOUT_MAX	(200*1000)

/*------------global variables definition-----------------------*/
static EM_SYS_EVENT flag = 0;

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
static int notifyEvent2(EM_SYS_EVENT eventNum, int msgLen, char * msg)
{
	flag = eventNum;
	//cls_printf("0x%08x",eventNum);
	return SUCC;
}

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	:
* return value		 	:
* history 		 		:  author		date		remarks
*			  	  	 	sull				20180522	created
*****************************************************************/
void event3(void)
{
	/*private & local definition*/
	int ret = -1, i = 0;
	uchar szPinOut[8+1]={0};
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;

	/*process body*/
	cls_printf("����%s...", TESTAPI);

	if(auto_flag==1)
	{
		send_result("line %d:%s�޴��Զ���������", __LINE__, TESTAPI);
		return;
	}

	//����ǰ��
	for(i=SYS_EVENT_MAGCARD; i<= SYS_EVENT_PRNTER; i++)
		NDK_SYS_UnRegisterEvent(i);
	NDK_MagClose();
	NDK_RfidPiccDeactivate(0);

	//case1.1:�����쳣���ԣ�Ӧ����NDK_ERR_PARA
	//case1.2:���������¼���(�����ڵ��¼��Ż����ȡֵ��Χ���¼���),Ӧ�÷���NDK_ERR_POSNDK_EVENT_NUM
	if((ret = NDK_SysResumeEvent(SYS_EVENT_NONE-1)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_SysResumeEvent(SYS_EVENT_MAX+1)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_SysResumeEvent(0x0001001)) != NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2:������ص��쳣
	cls_printf("��������쳣������...");
	//case2.1:δע����¼����޷������¼�(δע����¼����������¼����Ƿ���-3303)
	if((ret = NDK_SysResumeEvent(SYS_EVENT_ICCARD)) != NDK_ERR_EVENT_UNREALIZED/*NDK_ERR_POSNDK_EVENT_NUM*/)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

#if MAG_ENABLE
	//case2.2:ע����һ���¼������¼���δ����ʱ���������¼����᷵��NDK_ERR_EVENT_UNREALIZED 
	//�޸�Ϊע���¼������������¼����᷵��δע���¼�
	flag = SYS_EVENT_NONE;
	NDK_MagOpen();
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_MAGCARD, MAXWAITTIME*1000, notifyEvent2)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysResumeEvent(SYS_EVENT_MAGCARD)) !=NDK_ERR_EVENT_UNREALIZED/* NDK_OK*/)
	{
		send_result("line %d: %s�����쳣����ʧ��(%d)", __LINE__, TESTAPI, ret);  
		GOTOERR;
	}
	cls_show_msg("����30s��ˢ�ſ���ˢ�꿨�����������");
	if(flag != SYS_EVENT_MAGCARD)
	{
		send_result("line %d: %s�����⿨�¼�ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	NDK_MagClose();
	NDK_SYS_UnRegisterEvent(SYS_EVENT_MAGCARD);
#endif

	//case2.3:��������¼���Ӧ����NDK_ERR_POSNDK_BUSY�����ϱ������������⣬��������¼������سɹ�
	if(NDK_RfidInit(NULL)!=NDK_OK)
	{
		cls_show_msg("��Ƶ�ӿ�������ʼ��ʧ��,������˳�...");
		RETURN;
	}
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_AB))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	flag = SYS_EVENT_NONE;
	cls_show_msg("���ڸ�Ӧ��������ƵA�������������");
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_RFID, MAXWAITTIME*1000, notifyEvent2)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�ȴ��¼��ص����
	while(flag != SYS_EVENT_RFID);
	cls_show_msg("���ƿ���ƵA�������������");
	if((ret = NDK_SysResumeEvent(SYS_EVENT_RFID)) != NDK_OK)
	{
		send_result("line %d: %s�����쳣����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SysResumeEvent(SYS_EVENT_RFID)) != NDK_OK)
	{
		send_result("line %d: %s�����쳣����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SYS_UnRegisterEvent(SYS_EVENT_RFID);
	//�µ籣��
	NDK_RfidPiccDeactivate(0);

	//case3:��������
	if((ret = NDK_RfidPiccType(RFCARD_TYPE_AB)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//ԭ����registerע���¼��󣬻ص�������Ϻ����Deactivate���ٴ�ע���¼����¹̼��ù�����ȥ��
	flag = SYS_EVENT_NONE;
	cls_show_msg("���ڸ�Ӧ��������ƵA�������������");
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_RFID, 60*1000, notifyEvent2)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	while(flag != SYS_EVENT_RFID);
	flag = SYS_EVENT_NONE;
	NDK_RfidPiccDeactivate(0);
	if(flag == SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}
	if((ret = NDK_SYS_UnRegisterEvent(SYS_EVENT_RFID)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��", __LINE__, TESTAPI);
		GOTOERR;
	}

#if MAG_ENABLE
	//case4:���Դſ��������¼��������¼���Ӧ����������ע�������������¼�Ӧʧ�ܣ������޷��������¼�
	//case4.1:ע��mag�����¼�
	NDK_MagOpen();
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_MAGCARD, 60*1000, notifyEvent2)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("����30s��ˢ�ſ���ˢ�������������");
	if( flag != SYS_EVENT_MAGCARD)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}

	//case4.2:����mag�¼���Ӧ��������(��������ܲȷ�ϣ�ֻ���ߴſ������ǲ��������¼�����Ҫ�رմſ��ٿ����ſ��ſ��� modify 20180126)
	NDK_MagClose();
	NDK_MagOpen();
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SysResumeEvent(SYS_EVENT_MAGCARD)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);//-3303 Ԥ��Ҫ�ɹ�
		GOTOERR;
	}
	cls_show_msg("����30s��ˢ�ſ���ˢ�꿨�����������");
	if( flag != SYS_EVENT_MAGCARD)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	
	//case4.3:mag�¼�ע���������¼���Ч���Ҽ��������κ��¼�(�ϱ���˵����ֵҪ�޸�Ϊ-4006��ע���¼�֮�󶼷���-4006) ��߷���-3303
	NDK_SYS_UnRegisterEvent(SYS_EVENT_MAGCARD);
	NDK_MagClose();
	NDK_MagOpen();
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SysResumeEvent(SYS_EVENT_MAGCARD)) != NDK_ERR_EVENT_UNREALIZED/*NDK_ERR_POSNDK_EVENT_NUM*/)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("����30s��ˢ�ſ���ˢ�������������");
	if(flag == SYS_EVENT_MAGCARD)
	{
		send_result("line %d: %s����ʧ��(ע���¼����Կ��Լ������¼�)(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	NDK_MagClose();
#endif

#if IC1_ENABLE
	//case5:����IC���������¼��������¼���Ӧ�ɼ������¼�������ע�������������¼�Ӧʧ�ܣ������޷��������¼�	
	//case5.1:ע��IC�����¼�
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD, MAXWAITTIME*1000, notifyEvent2)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("����30���ڲ���IC�������������");
	if(flag == SYS_EVENT_ICCARD)
		cls_show_msg("������IC�����룬��γ�IC������ɺ����������");

	//case5.2:����IC���¼�����Ӧ��������
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SysResumeEvent(SYS_EVENT_ICCARD)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret); //-3303 Ԥ��Ҫ�ɹ�
		GOTOERR;
	}
	cls_show_msg("����30���ڲ���IC�������������");
	if(flag != SYS_EVENT_ICCARD)
	{
		send_result("line %d: %s����ʧ��(�����¼�δ������IC���¼�)(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}

	//case5.3:ICע���������¼�Ӧ��Ч���Ҽ��������κ��¼�
	NDK_SYS_UnRegisterEvent(SYS_EVENT_ICCARD);
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SysResumeEvent(SYS_EVENT_ICCARD)) != NDK_ERR_EVENT_UNREALIZED/*NDK_ERR_POSNDK_EVENT_NUM*/)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret); //-3303  ����ֵ��ͬ
		GOTOERR;
	}
	cls_show_msg("����30���ڲ���IC�������������");
	if(flag == SYS_EVENT_ICCARD)
	{
		send_result("line %d: %s����ʧ��(ע���¼����Կ��Լ������¼�)(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
#endif

	//case6:���Էǽӿ��������¼��������¼���Ӧ�ɼ������¼�������ע�������������¼�Ӧʧ�ܣ������޷��������¼�
	//case6.1:ע��ǽӿ��¼�
	if((ret = NDK_RfidPiccType(RFCARD_TYPE_AB)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_RFID, 60*1000, notifyEvent2)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("����30���ڷ�����ƵA�������������");
	if(flag != SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}

	//case6.2:�����¼���Ӧ�ɼ������ǽӿ�����
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SysResumeEvent(SYS_EVENT_RFID)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("����30���ڷ�����ƵA�������������");
	if(flag != SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��(�����¼�δ�������ǽӿ�����)(0x%08x)", __LINE__, TESTAPI,flag);
		GOTOERR;
	}

	//case6.3:ע���ǽӿ��¼���ע���������¼���Ч���Ҽ��������¼�����
	NDK_SYS_UnRegisterEvent(SYS_EVENT_RFID);
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SysResumeEvent(SYS_EVENT_RFID)) != NDK_ERR_EVENT_UNREALIZED/*NDK_ERR_POSNDK_EVENT_NUM*/)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);//-3303 ����ֵ��ͬ
		GOTOERR;
	}
	cls_show_msg("����30���ڷ�����ƵA�������������");
	if(flag == SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��(ע���¼����Կ��Լ������ǽ��¼�)(0x%08x))", __LINE__, TESTAPI, flag);
		GOTOERR;
	}

	//case7:����PIN�����¼��������¼���Ӧ�ɼ������¼�������ע�������������¼�Ӧʧ�ܣ������޷��������¼�
	//ǰ��://��װTPK1(16bytes), ID=1 ���İ�װ
	NDK_SecKeyErase();
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=16;
	memset(stKeyInfo.sDstKeyValue, 0x19, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//��ʼ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case7.1:ע��PIN�¼�
	flag = SYS_EVENT_NONE;
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_PIN, 120*100/*MAXWAITTIME*1000*/, notifyEvent2))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("�뾡�찴ȷ��...");//����ʽ������,������ʽҲ������ 
	if((ret=NDK_SecGetPin(1, (uchar *)"0", NULL, szPinOut, SEC_PIN_ISO9564_1, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);  
		GOTOERR;
	}
	NDK_SysDelay(5);//��ʱ0.5s,���ܵײ㴦����Ҫ��ʱ��
	if(flag != SYS_EVENT_PIN)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}	

	//case7.2:����PIN�¼����Կ��Լ�����PIN�¼�
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	} 
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SysResumeEvent(SYS_EVENT_PIN)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); //-3303 Ԥ��Ҫ�ɹ�
		GOTOERR;
	}
	cls_printf("�뾡���ٴΰ�ȷ��...");//����ʹ�÷�����ʽ�ĳ�ʹ������ʽ(��ע�����¼����ƶ�����getpinʹ���������Ƿ�����)
	if((ret=NDK_SecGetPin(1, (uchar *)"0", NULL, szPinOut, SEC_PIN_ISO9564_1, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}	
	NDK_SysDelay(5);
	if(flag != SYS_EVENT_PIN)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}

	//case7.3:PIN�¼�ע���������¼���Ч���Ҽ��������¼����� 
	NDK_SYS_UnRegisterEvent(SYS_EVENT_PIN);
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SysResumeEvent(SYS_EVENT_PIN)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret); //-3303 Ԥ��Ҫ�ɹ�
		GOTOERR;
	}
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_printf("�뾡�찴ȷ��...");//����ʽ������,������ʽҲ������ 
	if((ret=NDK_SecGetPin(1, (uchar *)"0", NULL, szPinOut, SEC_PIN_ISO9564_1, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysDelay(5);
	if(flag == SYS_EVENT_PIN)
	{
		send_result("line %d: %s����ʧ��(ע���¼����Կ��Լ�����PIN�¼�)(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}

#if PRN_ENABLE  //N700��֧�ִ�ӡģ��
	//case8:���Դ�ӡ�¼��������¼���Ӧ�ɼ������¼�������ע�������������¼�Ӧʧ�ܣ������޷��������¼�
	//cls_show_msg("��ȷ����ֽ�����������...");
	if(lib_initprn(g_PrnSwitch) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		NDK_PrnInit(g_PrnSwitch);
		GOTOERR;
	}
	cls_show_msg("��ȷ��װ����10cm�Ĵ�ӡֽ�����������...");
	//case8.1:ע���ӡ�¼�
	flag = SYS_EVENT_NONE;
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_PRNTER, MAXWAITTIME*1000, notifyEvent2))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}		
	NDK_PrnStr("��ӡ�´�½NEWLAND��ӡ����ҳ1:����ӡ1---80��\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n\
		11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n\
		31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n\
		51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n\
		71\n72\n73\n74\n75\n76\n77\n78\n79\n80\n");
	if(flag == SYS_EVENT_PRNTER)
		cls_show_msg("��������ӡ��״̬�仯����ȷ��װ����10cm�Ĵ�ӡֽ�����������");
	else
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	
	//case8.2:������ӡ�¼�
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SysResumeEvent(SYS_EVENT_PRNTER)) != NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret); //-3303 Ԥ��Ҫ�ɹ�
		GOTOERR;
	}
	NDK_PrnStr("��ӡ�´�½NEWLAND��ӡ����ҳ1:����ӡ1---80��\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n\
		11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n\
		31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n\
		51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n\
		71\n72\n73\n74\n75\n76\n77\n78\n79\n80\n");
	if(flag != SYS_EVENT_PRNTER)
	{
		send_result("line %d: %s����ʧ��(�����¼�δ��������ӡ״̬�仯)(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}

	//case8.3:ע����ӡ�¼���������ӡ�¼�Ӧ��Ч���Ҽ��������¼�����
	NDK_SYS_UnRegisterEvent(SYS_EVENT_PRNTER);
	flag = SYS_EVENT_NONE;
	cls_show_msg("��ȷ��װ����10cm�Ĵ�ӡֽ�����������...");
	if((ret = NDK_SysResumeEvent(SYS_EVENT_PRNTER)) != NDK_ERR_EVENT_UNREALIZED/*NDK_ERR_POSNDK_EVENT_NUM*/)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret); //-3303 ����ֵ��ͬ
		GOTOERR;
	}
	NDK_PrnStr("��ӡ�´�½NEWLAND��ӡ����ҳ1:����ӡ1---80��\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n\
		11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n\
		31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n\
		51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n\
		71\n72\n73\n74\n75\n76\n77\n78\n79\n80\n");
	if(flag != SYS_EVENT_NONE)
	{
		send_result("line %d: %s����ʧ��(ע���¼����Կ��Լ�������ӡ�¼�)(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
#endif
/**
  * ע���¼���ʱ���Լ������¼���ʱ�����(����Ԫ������ʱ��������ϵ�ͬһ������)
  */
	//����ǰ��:ע�������¼�
	for(i=SYS_EVENT_MAGCARD; i<= SYS_EVENT_PRNTER; i++)
		NDK_SYS_UnRegisterEvent(i);

	//case9:��Ƶ���������¼�ʱ��Ĳ���
	//case9.1:��Ƶ���������¼���ʱδ�����¼���Ӧ��ʱ
	cls_printf("���Էǽӿ������¼���ʱδ�����¼�...��ȴ�25s...");
	NDK_RfidInit(NULL);
	NDK_RfidPiccType(RFCARD_TYPE_AB);
	flag = SYS_EVENT_NONE;
	NDK_SYS_RegisterEvent(SYS_EVENT_RFID, 60*1000, notifyEvent2);
	NDK_SysMsDelay(25*1000); //��ʱ25s
	cls_show_msg("����5s�ڷ���A��,������Ϻ����������");
	if(flag != SYS_EVENT_RFID)
	{
		send_result("line %d:����ʧ��(δ�������ǽӿ������¼�)(0x%08x)", __LINE__, flag);
		GOTOERR;
	}
	else
		cls_show_msg("�Ѽ�⵽�ǽӿ�����,���ƿ��ǽӿ������������");
	//��case��ʱ�����⣬������δ�޸�
	flag = SYS_EVENT_NONE;
	if((ret = NDK_SysResumeEvent(SYS_EVENT_RFID)) != NDK_OK)
	{
		send_result("line %d:%s�����ǽӿ��¼�ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("����ʱ35s,35s���ע��Ļ��Ϣ...");
	NDK_SysMsDelay(35*1000); //��ʱ35sӦ��ʱ
	cls_show_msg("����2s�ڷ�����ƵA��,������Ϻ����������");
	if(flag == SYS_EVENT_RFID)
	{
		send_result("line %d:%s����ʧ��(�����¼���ʱ��������ǽӿ������¼�)(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	else
		cls_show_msg("���ƿ��ǽӿ������������");

	//case9.2:��Ƶ���������¼��������s�����¼���Ӧ�����������¼�
	cls_printf("���Էǽӿ������¼������5s����...�����ĵȴ�25s,25s���ע��Ļ��Ϣ...");
	flag = SYS_EVENT_NONE;
	NDK_SYS_RegisterEvent(SYS_EVENT_RFID, MAXWAITTIME*1000, notifyEvent2);
	if((ret = NDK_SysResumeEvent(SYS_EVENT_RFID)) != NDK_OK)
	{
		send_result("line %d:%s�����ǽӿ��¼�ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysMsDelay(25*1000); //��ʱ25sӦ��ʱ
	cls_show_msg("����5s�ڷ���A��,������Ϻ����������");
	if(flag != SYS_EVENT_RFID)
	{
		send_result("line %d:����ʧ��(�����¼���ʱδ�������ǽӿ������¼�)(0x%08x)", __LINE__, flag);
		goto ERR;
	}
	NDK_RfidPiccDeactivate(0);

	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_MagClose();
	NDK_RfidPiccDeactivate(0);
	for(i= SYS_EVENT_MAGCARD; i<=SYS_EVENT_PRNTER; i++)
		NDK_SYS_UnRegisterEvent(i);
	return;
}
  


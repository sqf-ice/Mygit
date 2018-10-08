/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: eventģ��
* file name		: event2.c
* Author 		:  
* version		: 
* DATE			: 20160914
* directory 		: 
* description		: ����NDK_SYS_RegisterEventע���¼��������ܺ�NDK_UnRegisterEvent�˳��¼���������
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
#define TESTAPI	"NDK_SYS_RegisterEvent,NDK_UnRegisterEvent"
#define MAXWAITTIME	30
#define PINTIMEOUT_MAX	(200*1000)

/*------------global variables definition-----------------------*/
static EM_SYS_EVENT flag = 0;

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
static int notifyEvent(EM_SYS_EVENT eventNum, int msgLen, char * msg)
{
	flag = eventNum;
	NDK_SYS_UnRegisterEvent(eventNum);

	return SUCC;
}
static int notifyEvent1(EM_SYS_EVENT eventNum, int msgLen, char * msg)
{
	flag = eventNum;
	//cls_printf("0x%08x",eventNum);
	return SUCC;
}

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author		date		remarks
*			  	  	 	
*****************************************************************/
void event2(void)
{
	if(auto_flag==1)
	{
		send_result("line %d:%s�޴��Զ���������", __LINE__, TESTAPI);
		return;
	}
	/*private & local definition*/
	int ret = -1, i = 0;
	ST_SEC_KCV_INFO stKcvInfoIn;
	ST_SEC_KEY_INFO stKeyInfo;
	uchar szPinOut[8+1]={0};
	int pnErrorCode=0;
	char pszTk1[MAXTRACKLEN]={0}, pszTk2[MAXTRACKLEN]={0}, pszTk3[MAXTRACKLEN]={0};
	EM_PRN_STATUS emStatus=0;

	/*process body*/
	cls_printf("����%s...", TESTAPI);

	//����ǰ��:
	for(i=SYS_EVENT_MAGCARD;i<=SYS_EVENT_PRNTER;i++)
		NDK_SYS_UnRegisterEvent(i);
	NDK_MagClose();//�ſ��ر�
	NDK_RfidPiccDeactivate(0);//��Ƶ���µ�

	//case1:�����쳣����
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_NONE-1,MAXWAITTIME*1000,notifyEvent))!=NDK_ERR_POSNDK_EVENT_NUM)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_MAX+1,MAXWAITTIME*1000,notifyEvent))!=NDK_ERR_POSNDK_EVENT_NUM)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD,MAXWAITTIME*1000,NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2: ע���¼�ͬʱ���ö���ʱӦ�÷���NDK_ERR_POSNDK_EVENT_NUM ������¼���
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_MAGCARD|SYS_EVENT_ICCARD,MAXWAITTIME*1000,notifyEvent))!=NDK_ERR_POSNDK_EVENT_NUM)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case3:��ͬ�¼�����ע��Ӧ�÷���NDK_ERR_ POSNDK EVENT_REG_TWICE
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD,MAXWAITTIME*1000,notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD,MAXWAITTIME*1000,notifyEvent))!=NDK_ERR_POSNDK_EVENT_REG_TWICE)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SYS_UnRegisterEvent(SYS_EVENT_ICCARD);

#if 1	
	//case4:��ͬ�¼�����ע���ɹ�:Ҫ����¼���ռ����Ҫ2����ͬ�Ľ���,���ﵥ�����޷�ʵ��
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_MAGCARD,MAXWAITTIME*1000,notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD,MAXWAITTIME*1000,notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SYS_UnRegisterEvent(SYS_EVENT_MAGCARD);
	NDK_SYS_UnRegisterEvent(SYS_EVENT_ICCARD);
	sleep(1);//�ȴ�ϵͳ������  ��Ҫһ������ʱ �볬ʱʱ���޹�  
#endif

	//case5:���Լ�����ʱʱ��<=0,����ʱ�����˳� ��Ҫ�����˳���������,��ʱ1s���ټ�����ͬ�¼�ʱӦ�û�ʧ��,˵��֮ǰ���¼�δע��	
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_MAGCARD,-10,notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	sleep(1);
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_MAGCARD,MAXWAITTIME*1000,notifyEvent))!=NDK_ERR_POSNDK_EVENT_REG_TWICE)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	NDK_SYS_UnRegisterEvent(SYS_EVENT_MAGCARD);

	//case6:����ע������¼���ʱ�˳�Ӧ�÷���SYS_EVENT_NONE(��ʱ����Զ�ע��)
	flag = SYS_EVENT_MAGCARD;
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD, 1*1000,notifyEvent1))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg1(2,"�ȴ�2s");
	if(flag != SYS_EVENT_NONE)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}	
	//case7:��������:�������¼����ص�������־λӦ�÷���SYS_EVENT_NONE
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_NONE, 1*1000, notifyEvent))!=NDK_ERR_POSNDK_PERMISSION_UNDEFINED)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);//-4021
		GOTOERR;
	}
	//case8:��������:�ſ���,�����ſ��¼�
	//�򿪴ſ�
	NDK_MagOpen();
	//case8.1:ע������ſ��¼�,��ʾˢ��,ˢ�����¼�Ӧ�ñ�������,�����ûص���������־λ���óɴſ��¼�,Ȼ���˳������¼�Ӧ�óɹ�
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_MAGCARD, MAXWAITTIME*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(30,"����30����ˢ�ſ�,ˢ�꿨�����������");
	if(flag != SYS_EVENT_MAGCARD)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	//case8.2:�˳������¼�����ˢ��,Ӧ�ò�����ûص�����,flag����0;
	flag = SYS_EVENT_NONE;
	cls_show_msg1(30,"����ˢ�ſ�,ˢ�꿨�����������");
	if(flag != SYS_EVENT_NONE)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	NDK_MagClose();

	//case9:����IC�������¼�
	//case9.1:ע�����IC���¼�,��ʾ����IC��,�忨���¼�Ӧ�ñ�������,�����ûص���������־λ���ó�IC���¼�,Ȼ���˳������¼�Ӧ�óɹ�
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD, MAXWAITTIME*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(30,"����30�����IC�������������");
	if(flag != SYS_EVENT_ICCARD)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}	
	//case9.2:�˳������¼����IC��(���IC�����ᴥ���¼���),Ӧ�ò�����ûص�����,flag����0;
	flag = SYS_EVENT_NONE;
	cls_show_msg1(30,"����30s�ڰε�IC�������������");
	if(flag != SYS_EVENT_NONE)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}

	//case10:�����ǽӿ��¼�
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
	//case10.1:ע������ǽӿ��¼�,��ʾ����Ƶ�����¼�Ӧ�ñ�������,�����ûص���������־λ���óɷǽӿ��¼�,Ȼ���˳������¼�Ӧ�óɹ�
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_RFID, MAXWAITTIME*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(30,"����30�����ڸ�Ӧ������A�������������");
	if(flag != SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}	
	cls_show_msg("����A�������������");
	//case10.2:�˳������¼����ٷŷǽӿ�,Ӧ�ò�����ûص�����,flag����0;
	flag = SYS_EVENT_NONE;
	cls_show_msg1(30,"���ڸ�Ӧ������A�������������"); 
	if(flag != SYS_EVENT_NONE)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	//�µ�
	NDK_RfidPiccDeactivate(0);
	cls_show_msg("����A�������������");

	//case11:����pin�����¼� 
	//ǰ��://��װTPK1(8bytes), ID=1 ���İ�װ
	stKeyInfo.ucScrKeyIdx=0;
	stKeyInfo.ucScrKeyType=0;
	stKeyInfo.ucDstKeyIdx=1;
	stKeyInfo.ucDstKeyType = SEC_KEY_TYPE_TPK;
	stKeyInfo.nDstKeyLen=8;
	memset(stKeyInfo.sDstKeyValue, 0x19, stKeyInfo.nDstKeyLen);
	stKcvInfoIn.nCheckMode=SEC_KCV_NONE;
	if ((ret=NDK_SecLoadKey(&stKeyInfo, &stKcvInfoIn))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//��ʼ��
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	//case11.1:ע�����pin�����¼�,��ʾ����֮�����ûص�������־λ����
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_PIN, MAXWAITTIME*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_printf("���찴ȷ��...");//����ʽ������,������ʽҲ������ 
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
	//case11.2:�˳������¼����ٵ���pin����,Ӧ�ò�����ûص�����,flag����0;
	flag = SYS_EVENT_NONE;
	if((ret=touchscrean_getnum())!=NDK_OK)
	{
		send_result("line %d:%s������ּ��̳�ʼ��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	} 
	cls_printf("�����ٴΰ�ȷ��...");//����ʹ�÷�����ʽ�ĳ�ʹ������ʽ(��ע�����¼����ƶ�����getpinʹ���������Ƿ�����)
	if((ret=NDK_SecGetPin(1, (uchar *)"0", NULL, szPinOut, SEC_PIN_ISO9564_1, PINTIMEOUT_MAX))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}	
	if(flag != SYS_EVENT_NONE)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		RETURN;
	}
	
#if PRN_ENABLE  
	//case12:������ӡ��״̬(״̬�ı�Ż��з���ֵ,��������״̬��æ��״̬��ⲻ��)
	//ǰ��:��ӡ����ʼ��
	cls_show_msg("��ȷ����ֽ�����������...");
	if(lib_initprn(g_PrnSwitch) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		NDK_PrnInit(g_PrnSwitch);
		RETURN;
	}
	//case12.1:ע�������ӡ��״̬,����ӡ������ֽ״̬�����ֽ״̬ʱ�ܹ��ûص�������־λ���ش�ӡ��״̬
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_PRNTER, MAXWAITTIME*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(30,"����30s��ȡ����ӡֽ�����������");
	if(flag != SYS_EVENT_PRNTER)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}	
	//case12.2:�˳������¼���,��ӡ����ȱֽ״̬����ֽ״̬������ʹ�ص�����������,��־λ����Ϊ0
	flag = SYS_EVENT_NONE;
	cls_show_msg1(30,"����30s�ڸ���ӡ��װֽ�����������");
	if(flag != SYS_EVENT_NONE)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}

	//case12.3:��ӡ��״̬��ok��busy,���ᴥ���¼����ù���,��ӡ���ֳ���ok,����busy��ok�ᴥ���¼�
	flag = SYS_EVENT_NONE;
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_PRNTER, MAXWAITTIME*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStr("��ӡ�´�½NEWLAND��ӡ����ҳ1:����ӡ1---80��\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n\
			11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n\
			31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n\
			51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n\
			71\n72\n73\n74\n75\n76\n77\n78\n79\n80\n");
	//if(flag != SYS_EVENT_NONE)
	//��ӡ����NDK_PrnStr�Ƿ�����,��ֱ������ִ��,����busy״̬,��Ҫ�ȵ���ok״̬�����20170912
	while(1)
	{
		NDK_PrnGetStatus(&emStatus);
		if(emStatus==PRN_STATUS_OK)
			break;
	}
	NDK_SysDelay (3);//��ʱ0.3s , û�������ʱflagֵ���ܸı�
	if(flag != SYS_EVENT_PRNTER) 
	{
		send_result("line %d: %s����ʧ��(0x%08x,%d)", __LINE__, TESTAPI, flag,emStatus);
		GOTOERR;
	}

	//case12.4:��ӡ��״̬��busy��ȱֽ״̬�ı�ᴥ���¼�
	flag = SYS_EVENT_NONE;
	cls_show_msg("װ����10cm��ֽ�����������");
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_PRNTER, MAXWAITTIME*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_PrnStr ("��ӡ����ҳ1:����ӡ1---120��\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n\
			11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n\
			31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n\
			51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n\
			71\n72\n73\n74\n75\n76\n77\n78\n79\n80\n81\n82\n83\n84\n85\n86\n87\n88\n89\n90\n\
			91\n92\n93\n94\n95\n96\n97\n98\n99\n100\n101\n102\n103\n104\n105\n106\n107\n108\n109\n110\n\
			111\n112\n113\n114\n115\n116\n117\n118\n119\n120\n");
	if(flag != SYS_EVENT_PRNTER)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}	
#endif

	//case13:ע����¼���IC���¼�,�¼���ʾ���дſ�ˢ��,flag���ֲ���	
	flag = SYS_EVENT_NONE;
	NDK_MagOpen();
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD, MAXWAITTIME*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg1(30,"����30����ˢ�ſ�,ˢ�꿨�����������");
	if(flag != SYS_EVENT_NONE)  
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	NDK_MagClose();
	NDK_SYS_UnRegisterEvent(SYS_EVENT_ICCARD);

	//case14:ע��ſ��¼�,�ص�������û���˳������¼�,����һ��ˢ����Ȼ���ٽ���ˢ��Ӧ���ܹ������¼��ѱ�־λ�óɴſ���ֵ
	NDK_MagOpen();
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_MAGCARD, 60*1000, notifyEvent1))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	cls_show_msg("����20����ˢ�ſ�,ˢ�꿨�����������");
	if(flag != SYS_EVENT_MAGCARD)  
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	//Ŀǰ�ſ���Ҫ�رմ򿪺���ˢ���Żᴥ��  ������Ҫ�ȴ�������Աȷ���Ƿ���Ҫ�޸�
	//����ȷ����Ҫ����NDK_MagReadNormal�Żᴥ��20170912
	//����ȷ����Ҫ���ùرմ򿪺���ˢ���Żᴥ��20180130
	if((ret=NDK_MagReadNormal(pszTk1, pszTk2, pszTk3, &pnErrorCode))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%d)", __LINE__, TESTAPI, ret, pnErrorCode);
		GOTOERR;
	}
	NDK_MagClose();
	NDK_MagOpen();
	flag = SYS_EVENT_NONE;
	cls_show_msg("��ȴ�5������ˢ�ſ�,ˢ�꿨�����������");
	if(flag != SYS_EVENT_MAGCARD)  
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	flag = SYS_EVENT_NONE;//����
	NDK_SYS_UnRegisterEvent(SYS_EVENT_MAGCARD);//�˳������¼�

	//case15:����ע����ʧ�ܷ���NDK_ERR_POSNDK_EVENT_UNREG_TWICE  -4008
	if((ret=NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD, 60*1000, notifyEvent))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SYS_UnRegisterEvent(SYS_EVENT_ICCARD))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	NDK_SysDelay(50);//��ʱ50ms��֤�Ѿ��˳��ɹ��� �ٴε����˳��Ż᷵��ʧ��
	if((ret=NDK_SYS_UnRegisterEvent(SYS_EVENT_ICCARD))!=NDK_ERR_POSNDK_EVENT_UNREG_TWICE)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
	
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_MagClose();//�ſ��ر�
	NDK_RfidPiccDeactivate(0);//��Ƶ���µ�
	for(i=SYS_EVENT_MAGCARD;i<=SYS_EVENT_PRNTER;i++)
		NDK_SYS_UnRegisterEvent(i);	
	return;	
}


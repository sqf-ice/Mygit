/************************************************************************
*
* module		   : �¼�����֮�¼�����
* file name 	   : Event5.c
* Author		   : linying
* version		   : 
* DATE			   : 20180521
* directory 	        : 
* description	   : �¼�����(NDK_SysSuspenedEvent(EM_SYS_EVENT event)) (���ϱ����ýӿ�ֻ֧����Ƶ�¼��������¼�ע����ù����¼����᷵��-3303)
* related document :
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysSuspenedEvent"
#define MAXWAITTIME	30

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
*					linying 		   20180521 	   created
*****************************************************************/
void event5(void)
{
	if(auto_flag==1)
	{
		send_result("line %d:%s�޴��Զ���������", __LINE__, TESTAPI);
		return;
	}
	/*private & local definition*/
    	int i=0,ret=0,pnDatalen=0,pnSta=-1;
	uchar psDatabuf[MAXLEN_DATA]={0};
	uchar sendbuf[]={'\x00', '\x84', '\x00', '\x00', 8};

	/*process body*/
	cls_printf("����%s...", TESTAPI);
    
	//����ǰ��:
	for(i=SYS_EVENT_MAGCARD;i<=SYS_EVENT_PRNTER;i++)
		NDK_SYS_UnRegisterEvent(i);
	NDK_RfidPiccDeactivate(0);//��Ƶ���µ�

	//case1:�����쳣����,���������¼��ţ�Ӧ���ش���
	if((ret = NDK_SysSuspenedEvent(SYS_EVENT_NONE-1)!=NDK_ERR_PARA))
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret =  NDK_SysSuspenedEvent(SYS_EVENT_MAX+1))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret =  NDK_SysSuspenedEvent(0X00000016))!=NDK_ERR_PARA)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case2.1: �����쳣��δ�������¼��������У����ù����¼�(���ǽ�)��Ӧ�����¼���֧��(-3303)
	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_MAGCARD))!=NDK_ERR_EVENT_UNREALIZED)
	{
		send_result("line %d: %s����ʧ��(ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_PIN))!=NDK_ERR_EVENT_UNREALIZED)
	{
		send_result("line %d: %s����ʧ��ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_PRNTER))!=NDK_ERR_EVENT_UNREALIZED)
	{
		send_result("line %d: %s����ʧ��ret=%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
    	//case2.2: �����쳣��δ�������¼��������У����÷ǽӹ����¼���Ӧ�����¼�δע��(-3301)
	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_RFID))!=NDK_ERR_EVENT_UNREGISTER)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
   	// case5:ע��ǽ��¼�(��ʱʱ��Ҫ�����ܵĴ�)��δ������¼�������M1����request�������᷵��SPI���߳�ͻ���������ô���(-5)(ʵ�ʲ����������Ϊ�ڷ����Ǳ��Ѿ������suspend�����¼������������޷�����SPI��ͻ�����)
    	if(NDK_RfidInit(NULL)!=NDK_OK)
	{
		cls_show_msg("��Ƶ�ӿ�������ʼ��ʧ��,������˳�...");
		RETURN;
	}
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_A))!=NDK_OK)
	{
		send_result("line %d:%s���ÿ�Ƭ����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_RFID,-1,notifyEvent))!=NDK_OK)
	{
		send_result("line %d:%s�ǽ�ע��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_M1Request(REQA, &pnDatalen, psDatabuf))!=NDK_ERR_MI_NOTAGERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
   	 if((ret= NDK_SYS_UnRegisterEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s�˳��¼�����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

#if IC1_ENABLE
	// case7:ע����Ƶ���¼����ι�����Ƶ�¼�����ӦӰ�쵽�����¼��Ĳ���
	if((ret=NDK_RfidPiccType(RFCARD_TYPE_A))!=NDK_OK)
	{
		send_result("line %d:%s���ÿ�Ƭ����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_RFID,MAXWAITTIME*1000,notifyEvent))!=NDK_OK)
	{
		send_result("line %d:%s�ǽ�ע��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
    	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
   	cls_show_msg("����30s�ڲ���IC�������������");
	if((ret=NDK_IccDetect(&pnSta))!=NDK_OK||(pnSta&0xff)!=0x01)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
		GOTOERR;
	}
	//IC�ϵ�
	if((ret=NDK_IccPowerUp(ICTYPE_IC,psDatabuf, &pnDatalen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
    	if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{				
		send_result("line %d:�µ�ʧ��(%d)", __LINE__,  ret);
		GOTOERR;
	}
    	if((ret= NDK_SYS_UnRegisterEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s�˳��¼�����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

   	// case3.1:ע����Ƶ���¼���������Ƶ���¼���Ӧ����������Ƶ�¼�������IC����ſ�������Ӧ����������IC����ſ�
   	 flag =SYS_EVENT_NONE;
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_RFID,MAXWAITTIME*1000,notifyEvent))!=NDK_OK)
	{
		send_result("line %d:%s�ǽ�ע��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
    	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg("�������ƵA��,����������������");
	if(flag==SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
    	cls_show_msg("����30s�ڲ���IC�������������");
	if((ret=NDK_IccDetect(&pnSta))!=NDK_OK||(pnSta&0xff)!=0x01)
	{
		send_result("line %d:%s����ʧ��(%d,0x%02x)", __LINE__, TESTAPI, ret, (pnSta&0xff));
		GOTOERR;
	}
	//IC�ϵ�
	if((ret=NDK_IccPowerUp(ICTYPE_IC,psDatabuf, &pnDatalen))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	memset(psDatabuf,0,sizeof(psDatabuf));
	if((ret=NDK_Iccrw(ICTYPE_IC,sizeof(sendbuf),sendbuf,&pnDatalen,psDatabuf))!=NDK_OK)
	{		
		send_result("line %d:IC1����ʧ��(%d)", __LINE__, ret);
		GOTOERR;															
	}
    	if ((ret=NDK_IccPowerDown(ICTYPE_IC))!=NDK_OK)
	{				
		send_result("line %d:�µ�ʧ��(%d)", __LINE__,  ret);
		GOTOERR;
	}
    	if((ret= NDK_SYS_UnRegisterEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s�˳��¼�����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	// case6:ע����Ƶ���¼���������Ƶ�����¼�������ע����Ƶ�����¼�����Ӱ���������
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_RFID,-10,notifyEvent))!=NDK_OK)
	{
		send_result("line %d:%s�ǽ�ע��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
    	if((ret= NDK_SYS_UnRegisterEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s�˳��¼�����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
		
	// case3.2:ע��IC���¼���������Ƶ���¼������IC��������Ӧ����������IC��
#if IC1_ENABLE
    	flag =SYS_EVENT_NONE;
     	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD,MAXWAITTIME*1000,notifyEvent1))!=NDK_OK)
	{
		send_result("line %d:%sICע��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
    	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_RFID))!=NDK_ERR_EVENT_UNREGISTER)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	} 
    	cls_show_msg("����30s�ڲ���IC�������������");
	if(flag!=SYS_EVENT_ICCARD)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
    	if((ret= NDK_SYS_UnRegisterEvent(SYS_EVENT_ICCARD))!=NDK_OK)
	{
		send_result("line %d: %s�˳��¼�����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
		
	// case4:ע����Ƶ���¼���������Ƶ�¼�����п���IC���¼���Ӧ������������IC�������¼�
    	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_RFID,MAXWAITTIME*1000,notifyEvent))!=NDK_OK)
	{
		send_result("line %d:%s�ǽ�ע��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
    	flag =SYS_EVENT_NONE;
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_ICCARD,MAXWAITTIME*1000,notifyEvent1))!=NDK_OK)
	{
		send_result("line %d:%sICע��ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
   	 cls_show_msg("����30s�ڲ���IC�������������");
	if(flag!=SYS_EVENT_ICCARD)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
	// ע����Ƶ��IC���¼�
   	if((ret= NDK_SYS_UnRegisterEvent(SYS_EVENT_ICCARD))!=NDK_OK)
	{
		send_result("line %d: %s�˳��¼�����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
    	if((ret= NDK_SYS_UnRegisterEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s�˳��¼�����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	// case5.1:ע����Ƶ���¼���������������¼���Ӧ��Ӱ�쵽��Ƶ���¼��ļ���
	flag=SYS_EVENT_NONE;
	//ע����Ƶ���¼�
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_RFID,MAXWAITTIME*1000,notifyEvent1))!=NDK_OK)
	{
		send_result("line %d:%s�ǽ�ע��ʧ��(%d)", __LINE__, TESTAPI, ret);
        	RETURN;
	}
	//������������¼�
	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_PIN))!=NDK_ERR_EVENT_UNREALIZED)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	cls_show_msg("����30s�ڷ�����ƵA�������������");
	if(flag!=SYS_EVENT_RFID)
	{
		send_result("line %d: %s����ʧ��(0x%08x)", __LINE__, TESTAPI, flag);
		GOTOERR;
	}
    	if((ret= NDK_SYS_UnRegisterEvent(SYS_EVENT_RFID))!=NDK_OK)
	{
		send_result("line %d: %s�˳��¼�����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
		
#if PRN_ENABLE
	// case5.2:ע���ӡ�¼���������Ƶ�¼���Ӧ��Ӱ�쵽��ӡ�¼��ļ���
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Ч
	{
		send_result("line %d:%s��ӡ��ʼ��ʧ��",__LINE__,TESTAPI);
		GOTOERR;
	}
	cls_show_msg("��ȷ��װ����10cm�Ĵ�ӡֽ�����������...");
	flag=SYS_EVENT_NONE;
	//ע���ӡ�¼�
	if((ret = NDK_SYS_RegisterEvent(SYS_EVENT_PRNTER,MAXWAITTIME*1000,notifyEvent1))!=NDK_OK)
	{
		send_result("line %d:%s�ǽ�ע��ʧ��(%d)", __LINE__, TESTAPI, ret);
        	RETURN;
	}
	if((ret =NDK_SysSuspenedEvent(SYS_EVENT_RFID))!=NDK_ERR_EVENT_UNREGISTER)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
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
    	if((ret= NDK_SYS_UnRegisterEvent(SYS_EVENT_PRNTER))!=NDK_OK)
	{
		send_result("line %d: %s�˳��¼�����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}
#endif

	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_RfidPiccDeactivate(0);//��Ƶ���µ�
	for(i=SYS_EVENT_MAGCARD;i<=SYS_EVENT_PRNTER;i++)
		NDK_SYS_UnRegisterEvent(i);	
	return;	
}


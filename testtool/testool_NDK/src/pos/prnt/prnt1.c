/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt1.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnGetStatus�ӿڹ���
* related document	: NDKapi.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_PrnGetStatus"	
#define MAXWAITTIME 90

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void prnt1(void)
* functional description 	: ��ȡ��ӡ��״̬
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt1(void)
{
	//����Ϊ�ֲ�����
	int nRet=0;
	int nKeyin=0;
	EM_PRN_STATUS  emStatus=0;
	time_t old = 0, diff = 0;
	
	//����Ϊ������
	cls_show_msg1(2, "����%s...", TESTAPI); 

	//����ǰ��:
	cls_show_msg("��ȷ����ֽ�����������...");
	/*if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)*/
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;
	}

	//case5:��ʼ������״̬ӦΪOK̬
	if((nRet = NDK_PrnGetStatus(&emStatus)) != NDK_OK ||emStatus != PRN_STATUS_OK)
	{
		send_result("line %d:%s����ʧ��(%d, %d)",__LINE__,TESTAPI,nRet,emStatus);
		goto prnt_error;
	}
	

	//case1:�ô�ӡ����ֽ,���״̬	
	cls_show_msg("ȷ�ϴ�ӡ��ȱֽ��,���������ʼ");
	//NDK_PrnStart();
	if((nRet = NDK_PrnGetStatus(&emStatus)) != NDK_OK ||emStatus != PRN_STATUS_NOPAPER)
	{
		send_result("line %d:%s����ʧ��(%d, %d)",__LINE__,TESTAPI,nRet,emStatus);
		goto prnt_error;
	}
#if 0//ANDIROD_ENABLE  �������Ƹ�������ͨ����˴����᷵��æ��״̬ 20170623
	//����NDK_PrnStr�������ش�ӡ��״̬������ 20170220 linwl
	if((nRet = NDK_PrnStr("��Ӧ�ô�ӡ�����к���,�����ӡ�����ʾ����ʧ��\n"))!=PRN_STATUS_NOPAPER)
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
		goto prnt_error;
	}
#endif
	
	//װ����10cm��ֽ���ڴ�ӡ�����м��ȱֽ
	nKeyin = cls_show_msg1(MAXWAITTIME, "װ����10cm��ֽ,���������.ESC����");
	if (nKeyin==0)
		;
	else if (nKeyin==ESC)
	{
		send_result("line %d:�û���������", __LINE__);
		return;
	}
	else
	{
		NDK_PrnStr ("��ӡ����ҳ1:����ӡ1---120��\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n\
			11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n\
			31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n\
			51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n\
			71\n72\n73\n74\n75\n76\n77\n78\n79\n80\n81\n82\n83\n84\n85\n86\n87\n88\n89\n90\n\
			91\n92\n93\n94\n95\n96\n97\n98\n99\n100\n101\n102\n103\n104\n105\n106\n107\n108\n109\n110\n\
			111\n112\n113\n114\n115\n116\n117\n118\n119\n120\n");

		NDK_PrnStart();
		sleep(5);//����һ��,��֤5���ڲ�ȡ��ӡ�м���ȶ�״̬
		if((nRet = NDK_PrnGetStatus(&emStatus))!=NDK_OK || emStatus != PRN_STATUS_NOPAPER)
		{
			send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,emStatus);
			goto prnt_error;
		}
#if ANDIROD_ENABLE
		//����NDK_PrnStr�������ش�ӡ��״̬������ 20170220 linwl
		if((nRet = NDK_PrnStr("��Ӧ�ô�ӡ�����к���,�����ӡ�����ʾ����ʧ��\n"))!=PRN_STATUS_NOPAPER)
		{
			send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
			goto prnt_error;
		}
#endif	
	}
	
	//case2:װ��ֽ,���״̬
	cls_show_msg("ȷ�ϴ�ӡ��װ��ֽ��,���������ʼ");
	if((nRet = NDK_PrnGetStatus(&emStatus)) != NDK_OK||emStatus!=PRN_STATUS_OK) 
	{
		send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
		goto prnt_error;
	}
	
	//case3:��ӡ�����ڹ���״̬(��ѹ��������)
	
	
	//case4:��ӡ��æ
	if(g_PrnSwitch==1)//�������ͱߴ�
	{
		NDK_PrnStr ("��ӡ����ҳ1:����ӡ1---50��\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n\
			11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n\
			31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n");

#if 0
		if((nRet = NDK_PrnStart()) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI,nRet);
			goto prnt_error;
		}
#endif		

		if(NDK_PrnGetStatus(&emStatus) != NDK_OK || emStatus != PRN_STATUS_BUSY)
		{
			send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,emStatus);
			goto prnt_error;
		}
#if 0//ANDIROD_ENABLE //�������Ƹ�������ͨ����˴����᷵��æ��״̬20180111
		//����NDK_PrnStr�������ش�ӡ��״̬������ 20170220 linwl
		if((nRet = NDK_PrnStr("��Ӧ�ô�ӡ�����к���,�����ӡ�����ʾ����ʧ��\n"))!=PRN_STATUS_BUSY)
		{
			send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
			goto prnt_error;
		}
#endif		
		old = time(NULL);
		while(1)
		{
			if(NDK_PrnGetStatus(&emStatus) != NDK_OK )
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			if(emStatus == PRN_STATUS_OK)
				break;
			else if((diff=time(NULL)-old)>60)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			show_stopwatch(ENABLE, diff);
		}
		show_stopwatch(DISABLE, 0);
	}

	//case5:��POS���ñ����е��»�����Ӧ���ܻ�ȡ��״̬PRN_STATUS_DESTROYED
	//���»����Ƚ��ѹ���,���Ͳ�ͬ,��Ҫ�ĵ��²�ͬ,�ʸĳɲ����ӡ������20180119 
	if(cls_show_msg("�Ƿ���в����ӡ������,[ȷ��]����,[����]ȡ��")==ENTER)
	{
		cls_show_msg("�Ѵ�ӡ�������,���������ʼ");
		if((nRet = NDK_PrnGetStatus(&emStatus)) != NDK_OK ||emStatus != PRN_STATUS_DESTROYED)
		{
			send_result("line %d:%s����ʧ��(%d, %d)",__LINE__,TESTAPI,nRet,emStatus);
			goto prnt_error;
		}
	}
	
	send_result("%s����ͨ��", TESTAPI);	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);
	return;
}


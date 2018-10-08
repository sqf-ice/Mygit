/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt4.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnInit�ӿڹ���
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
#define TESTAPI	"NDK_PrnInit"	
#define MAXWAITTIME 60 
#define MAXPRNSWITCH 1

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void prnt4(void)
* functional description 	: ��ӡ��ʼ��
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt4(void)
{
	//����Ϊ�ֲ�����
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	int nRet = 0;
	int  nFlag = 1;
#if !K21_ENABLE
	int nFirstRun=1;
#else
	int nFirstRun=0;
#endif
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;	
	}
	
	while(1)
	{
		if((nRet=NDK_PrnGetStatus(&emStatus))!=NDK_OK )
		{
			send_result("line %d:%s����ʧ��%d",__LINE__,TESTAPI,nRet);
			goto prnt_error;
		}
		if(emStatus != PRN_STATUS_BUSY)
			show_stopwatch(DISABLE, 0);
		
		switch(emStatus)
		{
		case PRN_STATUS_BUSY:
			if(nBusy==FALSE)
			{
				nOldtime = time(NULL);//�ӵ�һ���յ�æ״̬��ʼ��ʱ
				nBusy = TRUE;
			}
			if((nDiff=time(NULL)-nOldtime)>MAXWAITTIME)
			{
				send_result("line %d:��ӡ������æ״̬ʱ�����", __LINE__);
				goto prnt_error;
			}
			show_stopwatch(ENABLE, nDiff);
			if (nFlag==1)
			{
				cls_printf("����%s�жϴ�ӡ", TESTAPI);
				if (NDK_OK!=NDK_PrnInit(1))
				{
					send_result("line %d: ��ӡ����λʧ��", __LINE__);
					goto prnt_error;
				}
				NDK_SysDelay(20);
				nFlag = 2;
			}
			else if(nFlag==2)
				nFlag = 3;
			continue;
			break;
		case PRN_STATUS_OK:
			nBusy=FALSE;			
			
			if(nFirstRun == 1)
			{
				//case0:���Բ������󷵻�ֵ"	NDK_ERR_PARA"  20140922 jiangym add
				if((nRet=NDK_PrnInit(MAXPRNSWITCH+2)) != NDK_ERR_PARA)
				{
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;	
				}
				//"NDK_ERR_OPEN_DEV��ӡ�豸��ʧ��""NDK_ERR_IOCTL �������Դ���""NDK_ERR_MACLLOC �ڴ�ռ䲻��" ����������ֵ��ʱ�޷�����20140922 jiangym add
				
				//case1:����NDK_PrnInit �ܷ���ջ���
				//cls_show_msg("�������ʼ��ӡ...");
				if((nRet=NDK_PrnInit(0)) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;	
				}
				NDK_PrnStr("������仰��ʱ������Ѿ�ʧ��\n");
				if((nRet=NDK_PrnInit(0)) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;	
				}
				if((nRet = NDK_PrnStart()) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}
				if (cls_show_msg("����ӡ���κ�����,��[ȷ��]����[����]")==ENTER)
				{
					send_result("%s����ʧ��", TESTAPI);
					goto prnt_error;//return;
				}				

				//case2: ����Ϊ1�ܷ������ͱߴ�
				if(NDK_PrnInit(1) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
					goto prnt_error;	
				}
				
				if((nRet=NDK_PrnStr("---��������50����,�ٴ�ӡ\"��1��\"---\n")) != NDK_OK )
				{
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}
				NDK_PrnFeedByPixel(50);
				if((nRet=NDK_PrnStr("��1��\n��2��")) != NDK_OK )//�ڶ���(���һ��)��Ϊû��\n,��û��startʱ��������
				{
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}		
				
				if (cls_show_msg("�鿴��ӡЧ��,��[ȷ��]����[����]")!=ENTER)
				{
					send_result("line %d:%s����ʧ��",__LINE__, TESTAPI);
					goto prnt_error;//return;
				}
				cls_show_msg1(2, "������ͨ��!");

				if(NDK_PrnInit(1) != NDK_OK)//�������"�ڶ���"����
				{
					send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
					goto prnt_error;	
				}
				if((nRet = NDK_PrnStr("\n---���´�ӡ3��---\n��1��\n��2��\n��3��\n")) != NDK_OK )//���ﴮǰ����\n����Ϊ���������������Щ����,���ʱ��һ������ֻ�͵�"��1��"Ϊֹ,û��\n���ʴ˴�����\n�Ļ����ᵼ�������ص�
				{
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}
				if (cls_show_msg("�鿴��ӡЧ��,��[ȷ��]����[����]")!=ENTER)
				{
					send_result("%s����ʧ��", TESTAPI);
					goto prnt_error;//return;
				}
				cls_show_msg1(2, "������ͨ��!");
				
				//case3:����Ϊ0�ܷ�رձ��ͱߴ�
				cls_show_msg("������ӡ�κ�����,���������...");
				if(NDK_PrnInit(0) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
					goto prnt_error;	
				}
				
				if((NDK_PrnStr("---���´�ӡ2��---\n��1��\n��2��\n")) != NDK_OK )
				{
					send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
					goto prnt_error;
				}
				if (cls_show_msg("����ӡ���κ�����,��[ȷ��]����[����]")==ENTER)
				{
					send_result("%s����ʧ��", TESTAPI);
					goto prnt_error;//return;
				}
				cls_show_msg1(2, "������ͨ��!");

				cls_show_msg("����ӡ��������,���������...");
				if((nRet = NDK_PrnStart()) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}
				if (cls_show_msg("�鿴��ӡЧ��,��[ȷ��]����[����]")!=ENTER)
				{
					send_result("%s����ʧ��", TESTAPI);
					goto prnt_error;//return;
				}				
				
				nFirstRun++;
			}

			//case4:����NDK_PrnInit�ܷ��жϴ�ӡ
			if (nFlag==1)
			{
				if(NDK_PrnInit(1) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
					goto prnt_error;	
				}
				
				NDK_PrnStr("����ӡ����ҳ1�����ҳ2\n\n\n");//zhangnw20110608add
				//�����ӡҪ��һ����������
				NDK_PrnStr("��ӡ����ҳ1:����ӡ1---70��\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n\
					11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n\
					31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n\
					51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n");
#if !(K21_ENABLE||CPU5830X_ENABLE||defined SP930G||defined SP930PG||defined SP630PG) //�˴���ӡ�ȽϿ���豸������ʱ,��ӡ�Ƚ������豸��Ҫ��ʱ,�ɸ���ʵ���������:������̫��һֱѭ����ӡ�޷��жϾ�Ҫ������ʱ,���δ��ӡ���ж�����Ҫ�Ӵ���ʱ
				NDK_SysDelay(20);
#endif
				//NDK_PrnStart();//\f�Ǵ�ӡ���������Ա�־
				//delay(10);//�ӳ�һ���ò���ҳ1���һ����
			}
			else if (nFlag==2)
			{
				NDK_PrnStr("����ҳ1�ѱ��жϣ�\n��ӡ����ҳ2\n��ӡ����ҳ2\n��ӡ����ҳ2\n");
				//NDK_PrnStart();//\f�Ǵ�ӡ���������Ա�־
			}
			else if(nFlag==3)
			{
				//test over
				nKeyin = cls_show_msg1(MAXWAITTIME, "��鿴����ҳ1�����ҳ2.[ȷ��]����ͨ��,[����]ʧ��");
				if (nKeyin==ENTER || nKeyin==0)
					send_result("%s����ͨ��", TESTAPI);
				else
					send_result("%s����ʧ��", TESTAPI);
				goto prnt_error;//return;			
			}

			break;
		case PRN_STATUS_NOPAPER:
			cls_printf("��ӡ��ȱֽ...װֽ��,���������,ESC����");
			lib_kbflush(); //��KB BUF
			while ((nKeyin=lib_kbhit())==0)
			{
				NDK_SysBeep();
				//����װֽ
				NDK_PrnFeedByPixel(10);
				sleep(2);
			}
			if (nKeyin==ESC)
			{
				send_result("line %d:��ӡ��ȱֽ,�û�����", __LINE__);
				goto prnt_error;//return;
			}
			NDK_PrnStart();
			cls_printf("����%s...", TESTAPI);
			break;
		case PRN_STATUS_OVERHEAT:
			send_result("line %d: ��ӡ������", __LINE__);
			goto prnt_error;
			break;
		case PRN_STATUS_VOLERR:
			send_result("line %d: ��ӡ����Դ����", __LINE__);
			goto prnt_error;
			break;
		case PRN_STATUS_DESTROYED:
			send_result("line %d: ��ӡ����", __LINE__);
			goto prnt_error;
			break;
		default:
			//δ֪״̬����Ϊ��
			send_result("line %d: UNKNOWN STATE(tpstatus=%d)", __LINE__, emStatus);
			goto prnt_error;//return;
			break;
		}
	}
	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);
	return;
}


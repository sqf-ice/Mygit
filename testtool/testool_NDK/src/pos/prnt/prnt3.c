/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt3.c
* Author 		: chensj
* version		: 
* DATE			: 20130105
* directory 		: 
* description		: ����NDK_PrnStr�ӿڹ���
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
#define TESTAPI	"NDK_PrnStr"	
#define MAXWAITTIME 180

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void prnt3(void)
* functional description 	: ��ӡ�ִ�
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 						:  author		date		remarks
*			  	  	  						chensj   20130105	created
*****************************************************************/
void prnt3(void)
{
	//����Ϊ�ֲ�����
	EM_PRN_STATUS emStatus=0;
	int nBusy = FALSE,i=0,nKeyin=0;
	time_t nOldtime=0,nDiff=0;
	char sPrintBuf[1024+32] = {0};
	int j = 0;
	int nRet=0;
	int nType = lib_getprintertype();
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	/*if(NDK_PrnInit(g_PrnSwitch) != NDK_OK)*/
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
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
			continue;
			break;

		case PRN_STATUS_OK:
			//test begin
			nBusy=FALSE;

			switch(i)
			{
			case 0: /**<��ӡ�����*/
				cls_printf("��ӡ�����");
				NDK_PrnStr("\n---��ӡ�������ʼ---\n");
				
				for (j = 0; j<1024; j++)
				{
					if (!(sPrintBuf[j]=rand()%256))
						break;
				}
				
				if(NDK_PrnStr(sPrintBuf) != NDK_OK)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					return;
				}
				
				NDK_PrnStr("\n---��ӡ��������---\n");
				break;
				
			case 1:	/**<��ӡ�̶���*/
				cls_printf("��ӡ�̶���");
				NDK_PrnStr("\n---��ӡ�̶�����ʼ---\n");
				//added 20130905 ��ӡҪ�ܹ������Ƨ����́�
				NDK_PrnSetFont(PRN_HZ_FONT_24x24,PRN_ZM_FONT_8x16 );
				if(NDK_PrnStr("��۲� 1 I l �����ַ��ܷ���������,�ܹ���ӡ����Ƨ��:��������(��)��(��)")!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto prnt_error;//return;
				}
				
				NDK_PrnSetFont(0,PRN_ZM_FONT_8x16 );
				if(NDK_PrnStr("\n[8x16] !\"#%&()*/0")!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto prnt_error;//return;
				}

				NDK_PrnSetFont(0,PRN_ZM_FONT_8x8);	
				if(NDK_PrnStr("\n[8x8] 2369;<=>?") != NDK_OK)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto prnt_error;//return;
				}
				
				NDK_PrnSetFont(0,PRN_ZM_FONT_10x16);
				if(NDK_PrnStr("\n[10x16] ACIKPQUY\\")!= NDK_OK)
				{
		                	send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto prnt_error;//return;
				}
				
				NDK_PrnSetFont(0,PRN_ZM_FONT_5x16);
				if(NDK_PrnStr("\n[5x16] \"#$%&()*+,-./0123456789:;<=>?@AHIJOPUVXYZabdew\r")!= NDK_OK)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto prnt_error;//return;
				}

				NDK_PrnSetFont(0,PRN_ZM_FONT_5x7);
				if(NDK_PrnStr("\n[5x7] \"#$%&()*+,-./0123456789:;<=>?@AHIJOPUVXYZabdew\r")!= NDK_OK)
				{
					send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
					goto prnt_error;//return;
				}

				NDK_PrnSetFont(PRN_HZ_FONT_24x24,PRN_ZM_FONT_8x16);
				NDK_PrnStr(" [24x24]ϧԵ\n");

				NDK_PrnSetFont(PRN_HZ_FONT_16x16,0);
				NDK_PrnStr(" [16x16]ϧ��\n");

				NDK_PrnSetFont(PRN_HZ_FONT_12x16,0);
				NDK_PrnStr(" [12x16]����\n");

				NDK_PrnSetFont(PRN_HZ_FONT_16x32,0);
				NDK_PrnStr(" [16x32]�ж�\n");

				NDK_PrnSetFont(PRN_HZ_FONT_32x32,0);
				NDK_PrnStr(" [32x32]����");
	
				NDK_PrnStr("\n---��ӡ�̶������---\n");
				
				NDK_PrnStr("\n---��鿴��������Ĵ�ӡЧ��---\n");
				break;
				
			case 2:  /**<��ӡ�س������У��ո�*/
				cls_printf("��ӡ�س������У��ո�");
				NDK_PrnStr("\n---��ӡ�س������У��ո�ʼ---\n");

				//��ӡ\t Ӧ������Ч��(�ĵ�Ҳû�ᵽ)
				if ((nRet = NDK_PrnStr("��ӡ\\rЧ��(�ỻ��)\r��ӡ\\tЧ��(��Ч��)\t��ӡ\\nЧ��(�ỻ��)\n"))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
					goto prnt_error;//return;
				}
	
				if ((nRet = NDK_PrnStr("\nSTART: :END(����START/END֮��Ӧ��һ�ո�)\n"))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
					goto prnt_error;//return;
				}
	
				if ((nRet = NDK_PrnStr("NEWLand�´�½!#@%$%^&*()__+|><:;")!=NDK_OK))
				{
					send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,nRet);
					goto prnt_error;//return;
				}
				
				NDK_PrnStr("\n---��ӡ�س������У��ո����---\n");
				
				break;
				
			case 3:	/**< ���Ի�����Խ��*/
				cls_printf("��ӡ��������С����");
				
				switch (nType)
				{
				case PRN_TYPE_HIP:
				case PRN_TYPE_FIP:
					memset(sPrintBuf, 'H', sizeof(sPrintBuf));
					memcpy(sPrintBuf, "����㿴������,˵����ӡ����!", strlen("����㿴������,˵����ӡ����!"));

					if((nRet = NDK_PrnStr(sPrintBuf))!=NDK_ERR_PARA)
					{
						send_result("line %d: ��ӡ��������С����ʧ��(%d)", __LINE__,nRet);
						goto prnt_error;//return;
					}
					NDK_PrnInit(g_PrnSwitch);
					break;
				case PRN_TYPE_TP:
#if !K21_ENABLE //K21ƽֻ̨֧�ֱ��ͱߴ򣬲�֧�ִ�ӡ������
					nRet  = NDK_PrnStr ("��ӡ����ҳ1:����ӡ1---199��\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n\
							11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n\
							31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n\
							51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n\
							71\n72\n73\n74\n75\n76\n77\n78\n79\n80\n81\n82\n83\n84\n85\n86\n87\n88\n89\n90\n\
							91\n92\n93\n94\n95\n96\n97\n98\n99\n100\n101\n102\n103\n104\n105\n106\n107\n108\n109\n110\n\
							111\n112\n113\n114\n115\n116\n117\n118\n119\n120\n121\n122\n123\n124\n125\n126\n127\n128\n129\
							\n130\n131\n132\n133\n134\n135\n136\n137\n138\n139\n140\n141\n142\n143\n144\n145\n146\n147\n148\n149\
							\n150\n151\n152\n153\n154\n155\n156\n157\n158\n159\n160\n161\n162\n163\n164\n165\n166\n167\n168\n169\
							\n170\n171\n172\n173\n174\n175\n176\n177\n178\n179\n180\n181\n182\n183\n184\n185\n186\n187\n188\n189\
							\n190\n191\n192\n193\n194\n195\n196\n197\n198\n199\n200\n201\n");
#if OVERSEAS_OS70_ENABLE  //�����7.0�汾����ֵ�޸�  20180424 modify
					if(nRet !=NDK_ERR_MACLLOC)
#else
					if(nRet !=NDK_ERR_PARA)
#endif
					{
						send_result("line %d: ��ӡ��������С����ʧ��(%d)", __LINE__,nRet);				
						goto prnt_error;//return;
					}
					NDK_PrnInit(g_PrnSwitch);
#endif
					break;
				default:
					send_result("line %d:δ�ܻ�ȡ����ӡ������", __LINE__);
					goto prnt_error;
					break;
				}

				break;
				
			case 4: /**<���Դ�ӡ������*/
				sPrintBuf[0] = '\0';
				strcpy(sPrintBuf+1, "����㿴������,˵����ӡ����!");
				if((nRet = NDK_PrnStr(sPrintBuf))!=NDK_OK)
				{
					send_result("line %d:%s����ʧ��%d", __LINE__, TESTAPI,nRet);
					goto prnt_error;//return;		
				}
				break;
			case 5: /*case 5:����V4.3.3��������������������"\r\n"����Ϊ��8���й���Ӵ˰���*/
				if(nType==PRN_TYPE_TP)
				{
					cls_printf("�������������ӡ����ӡ��8��������");
					if((nRet=NDK_PrnStr("��ӡ\\r\\nЧ��Ϊ��8������\r\n����һ���ַ�����8������"))!=NDK_OK)
					{
						send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
						goto prnt_error;
					}
				}
				break;
			case 6: //����������֤��������NDK_PrnStr���м��NDK_PrnStrû��\nʱ�Ƿ�����һ�д�ӡ
				NDK_PrnStr("ð�ź����Ӧ����ͬһ��:\n");
				NDK_PrnStr("��������");
				NDK_PrnStr("ȫ��");
				NDK_PrnStr("����\n");
				NDK_PrnStr("�����ӡ������ͬһ��\n");
				break;
			default:
				nKeyin = cls_show_msg1(MAXWAITTIME, "����ӡЧ��,��ENTERͨ��,��������ͨ��");
				if (nKeyin!=ENTER && nKeyin!=0)//zhangnw20061031 modify
					send_result("%s����ʧ��", TESTAPI);
				else
					send_result("%s����ͨ��", TESTAPI);
				goto prnt_error;//return;
				break;
			}		
			NDK_PrnStart();	
			i++;
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

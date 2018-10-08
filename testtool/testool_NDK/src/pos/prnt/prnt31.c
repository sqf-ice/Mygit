/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt31.c
* Author 		: 
* version		: 
* DATE			: 
* directory 		: 
* description		: ģ���Լ��ӡ����
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
#define TESTAPI	"ģ���Լ��ӡ����"	
#define MAXWAITTIME 180

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 				: void prnt31(void)
* functional description 	: ģ���Լ��ӡ����
* input parameter	 				:��
* output parameter	 			:��
* return value		 				:��
* history 		 					:  author		date		remarks
*			  	  	  				 sull            20180712        created
*****************************************************************/
//ģ���Լ��ӡ���̣������ڴ�ӡʧ�ܵ�������ٴδ�ӡ�Ĵ�ӡЧ���쳣����ӡ���������쳣���µ�bug
static int onTest(void)
{
	int ret = 0;
	char buf[2000] = {0};
	
	if((ret=NDK_PrnInit(1)) != NDK_OK)
		return ret;
	if((ret=NDK_PrnSetForm(0, 0, 4)) != NDK_OK)
		return ret;
	//���ó�����24*24��Ӣ��12*24C�����壬��ӡ��;ȱֽ�󣬻����쳣����Ҫ����
	if((ret=NDK_PrnSetFont(1,18)) != NDK_OK)
		return ret;
	//��ӡ����
	sprintf(buf, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
	         "                                            �������� \n\n", " - - - - - - - - - - - - - - -\n",
		     "�̻����ƣ�              ǩ���� \n\n", "�̻���ţ�         01010045678 \n\n",
		     "���κţ�         55667894561235\n\n", "�̻������ţ�          098767890\n\n",
		     "�������ͣ�                 ����\n\n", "�������ڣ�  2015/06/29 16:27:21\n\n",
		     "�ο���:\n", "               1234567890123456\n\n", "Ӧ�ս�\n", "                  RMB  10000.00\n\n", 
		     "ʵ�ս�\n", "                   RMB  10000.00\n\n", "�ֿ���ǩ����\n\n\n", "��ע��         \n\n");
	if((ret = NDK_PrnStr(buf)) != NDK_OK)
		return ret;
	if((ret = NDK_PrnStart()) != NDK_OK)
		return ret;
	return NDK_OK;
}

void prnt31(void)
{
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}

	//����Ϊ�ֲ�����
	int nRet=0, i=0;
	
	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:
	if(lib_initprn(g_PrnSwitch) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;
	}
	
	cls_show_msg("������ģ���Լ��ӡ����,����ӡ������,��ʹ�ÿհ״�ӡֽ�۲��ӡЧ���Ƿ��쳣;\n�ڵ�һ�����ݴ�ӡ������,���ֶ���pos��ӡ���ӹ���ȱֽ״̬;\nװֽ��ɺ�ڶ�����������ӡ,��ע�ڶ�����ӡЧ��,���������������");

	//case1:���нű���ӡ����
	for(i=0; i<2; i++)
	{
		cls_printf("��ӡ��%d������...", i+1);
		nRet = onTest();
		switch(nRet)
		{
			case NDK_OK:
				break;
			case 2:
				cls_show_msg("��ӡ��ȱֽ,��װֽ...��ɵ����������");
				break;
			default:
				send_result("line %d:��%d��:%s����ʧ��(%d)", __LINE__, TESTAPI, nRet);
				goto prnt_error;
				break;
		}
	}
	if(cls_show_msg("��ӡЧ����Ԥ����ʾ��һ��?��[ENTER],��[����]") != ENTER)
	{
		send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
		goto prnt_error;
	}
	send_result("%s����ͨ��", TESTAPI);
	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);	
	return;
}


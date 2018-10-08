/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ��ӡ�ۺϲ���
* file name		: systest1.c
* Author 			: chensj
* version			: V1.0
* DATE			: 20130204
* directory 		: 
* description		: ��ӡ�ۺϲ���
* related document	: NDKapi.chm
*				  
************************************************************************
* log			: 
* Revision 1.0  20130204 chensj
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"��ӡ����,ѹ��"
#define	MAXPAGE	(3)

#define	MAXWAITTIME	(180)	//30->180 for ���
#define	MAXHOTTIME	(60*30)	//30min
#define	MINHOTTIME		(60*7)

#define	DEFAULT_MagePon_VLE	(0)
#define	DEFAULT_MagePon_STR	"0"
#define	DEFAULT_PW_VLE	(384)
#define	DEFAULT_PW_STR	"384"

//#define	FILL_XSIZE	(384)
//#define	FILL_YSIZE	(128)

/*------------global variables definition-----------------------*/
static int general1_slct_BMP = 1;
static int general1_MagePon = DEFAULT_MagePon_VLE;
static char general1_keybuf[12] = DEFAULT_MagePon_STR;
static int general1_nPageWidth = DEFAULT_PW_VLE;//200->384
static char general1_cReadBuf[20] = DEFAULT_PW_STR;

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
static void general1_print_triangle(void)
{
	/*private & local definition*/

	/*process body*/
	print_triangle(general1_nPageWidth);
}

static void general1_print_image(void)
{
	/*private & local definition*/

	/*process body*/
	if (general1_slct_BMP==1)
		print_image(128, 64, general1_MagePon, CCB_BMP);
	else
		print_image(144, 64, general1_MagePon, CCB_BMP_144);
}

//��Ҫ�����ӡ�ٶ�
static void prn_ability(void)
{
	/*private & local definition*/
	float stp = 0.0;
#if K21_ENABLE
	char str[32] = {0};
#endif
#define	TRIANGLE_ID	(0)
#define	IMAGE_ID		(9)
#define	ENTER_ID		(10)
	uint nLen=0;
	PF pfunc[] = {general1_print_triangle, print_testpage, print_rand, print_guo, print_fill, print_bill, 
				print_verticalbill, print_blank, print_font, general1_print_image, print_enter, print_compress, print_landi, print_xinguodu, print_signlfont};
	char *func_info[] = {"���ǿ�", "����ҳ", "�����", "����ҳ", "��䵥", "Ʊ��", "������", 
					"�հ׵�", "ģʽ�������", "ͼ�δ�ӡ", "�س�", "ѹ��", "���ϵ���", "�¹���", "��������"},
	*menu[] = {"1.���ǿ�2.����ҳ\n3.�����4.��ӡ��\n5.��䵥6.Ʊ��  \n7.������8.��ҳ\n", 
				"1.�հ׵�2.ģʽ��\n3.ͼ�δ�ӡ4.�س�\n5.ѹ��6.���ϵ���\n7.�¹���8.��ҳ\n",
				"1.������8.��ҳ\n"};
	int  line[] = {0, 55, 0, 21, 0, 41, 0,
				0, 0, 0, 0, 0, 0, 0, 0};//��Ҫ�����ӡ�ٶȵ�,����ӡ��������,������0//��������:16����Ϊ1�У�С��16���е�Ϊ1�У�����16���еĳ���16�õ�����
	int casekey = 0,  page = 1, ret = 0, type = lib_getprintertype();
	
	/*process body*/
	general1_nPageWidth = (PRN_TYPE_TP==type)?MAXPRLINEWIDE_TP:((PRN_TYPE_HIP ==type)?MAXPRLINEWIDE_HIP:MAXPRLINEWIDE_FIP);
	sprintf(general1_cReadBuf, "%d", general1_nPageWidth);
	//Ϊ�˱���������ӡ������Ӱ�죬����һ�´�ӡBUF��ͬʱ��λһ��
	/*if (FAIL==NDK_PrnInit(g_PrnSwitch))*/
	if(FAIL==lib_initprn(g_PrnSwitch))//��ʼ��ʹ��ʼ���õ�������Чmodify 20131122 jiangym
	{
		cls_show_msg("line %d: ��ӡ����λʧ��", __LINE__);
		return;
	}
	
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//�������������ĸ���
	{
		cls_show_msg("line %d:��ӡ��״̬�쳣!", __LINE__);
		return;                         
	}
	
	while(1)
	{
		NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE,1);//�ָ���ӡģʽ
    		casekey = cls_show_msg(menu[page-1]);
		switch(casekey)
		{
		case ESC:
			return;
			break;
		case '8'://��ҳ����
			if(page<MAXPAGE)
				page++;
			else if(page==MAXPAGE)
				page = 1;//��λ
	            	continue;
	            	break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':	
			casekey = casekey-'1'+7*(page-1);//ת��ҳ 
			break;
		default://'9',ENTER��
			continue;
			break;
		}

		cls_show_msg1(1, "��������%s��ӡ...\n��ע���ӡЧ��\n", func_info[casekey]);
		if(casekey==IMAGE_ID)//ͼ�δ�ӡ��������,���⴦��һ��
		{
			if(show_msg1(2, "ESC���������\n")==ESC)
			{
				cls_printf("Ĭ��ͼ�����:\n");
				if ((ret=lib_kbgetinput(general1_keybuf,0, sizeof(general1_keybuf)-1, &nLen,INPUTDISP_OTHER, 0,INPUT_CONTRL_LIMIT))==ESC||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
					general1_MagePon = DEFAULT_MagePon_VLE;
				else
					general1_MagePon = atoi(general1_keybuf);

				if(cls_show_msg("ͼ��ѡ��\n1.128\n2.144")=='1')
					general1_slct_BMP = 1;
				else
					general1_slct_BMP = 2;
			}
		}
		if(casekey==TRIANGLE_ID)//���Ǵ�ӡ��������,���⴦��һ��
		{
			if(show_msg1(2, "ESC���������\n")==ESC)
			{
				cls_printf("Ĭ��ҳ��:\n");
				if ((ret=lib_kbgetinput(general1_cReadBuf, 0,sizeof(general1_cReadBuf)-1,  &nLen,INPUTDISP_OTHER, 0,INPUT_CONTRL_LIMIT))==ESC||ret==NDK_ERR_TIMEOUT)//��ȡ��,��ʹ��default    
					general1_nPageWidth = (PRN_TYPE_TP==type)?MAXPRLINEWIDE_TP:((PRN_TYPE_HIP==type)?MAXPRLINEWIDE_HIP:MAXPRLINEWIDE_FIP);//general1_nPageWidth = DEFAULT_PW_VLE;
				else
					general1_nPageWidth = atoi(general1_cReadBuf);
			}
		}
		if(casekey==ENTER_ID)//���Ǵ�ӡ��������,���⴦��һ��
		{
			if(show_msg1(2, "ESC���������\n")==ESC)
			{
				if ('2' == cls_show_msg("ѡ���ݺ���Ŵ�ģʽ(Ĭ��˫��):1.˫��,2.����\n"))
					NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE ,0);
				else
					NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE ,1);
			}
		}		
		lib_StartStopwatch();
		pfunc[casekey]();
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//״̬OK��,��ֹ��ʱ
		{
			cls_show_msg("line %d:��ӡ��״̬�쳣!", __LINE__);
			return;                         
		}
		stp = lib_ReadStopwatch();
#if K21_ENABLE
		memset(str,0,sizeof(str));
		ftos(stp,str);
		cls_show_msg("%s��ӡִ��ʱ��:%ss\n", func_info[casekey], str);
		memset(str,0,sizeof(str));
		ftos(line[casekey]/stp,str);
		if(line[casekey])
			show_msg("��ӡ�ٶ�:%s��/s\n", str);
#else
		cls_show_msg("%s��ӡִ��ʱ��:%fs\n", func_info[casekey], stp);
		if(line[casekey])
			show_msg("��ӡ�ٶ�:%f��/s\n", line[casekey]/stp);
#endif
	}
	
	//test over
	return;
}

//��ӡ��䵥
static void print_fill1(void)
{
	/*private & local definition*/
	char printbuf[(MAXPRLINEWIDE_FIP*128)/8] = {0};//��������������Ҫ424*128/8�Ĵ�С
	//char printbuf[(FILL_XSIZE*FILL_YSIZE)/8] = {0};
	int type = lib_getprintertype();
	int xsize = 0;

	/*process body*/
	memset(printbuf,0xff,sizeof(printbuf));
	xsize = (PRN_TYPE_TP==type)?MAXPRLINEWIDE_TP:((PRN_TYPE_HIP ==type)?MAXPRLINEWIDE_HIP:MAXPRLINEWIDE_FIP);
	if(NDK_PrnImage(xsize,128,0,printbuf)!=NDK_OK)
	//if(NDK_PrnImage(FILL_XSIZE,FILL_YSIZE,0,printbuf)!=NDK_OK)//ԭ����FILL_XSIZE���������������⣬���ݴ�ӡ������������䵥�Ŀ��ֵ 20131122 linwl
	{
		cls_show_msg("line %d:��ӡ��䵥ʧ��!\n", __LINE__);
		//return;//������ȵ���ûȡ��״̬ʱ ����������ȱ����������ʧ�� 20140124
	}

	NDK_PrnStr("\n");
	NDK_PrnStart();
	return;
}

static void prn_hot(void)
{
	/*private & local definition*/
	int status = 0, cnt = 0, i = 0;
	time_t diff = 0, oldtime = time(NULL);
	
	/*process body*/
	while(1)
	{
		cls_printf("���Ȳ�����...");
		if((diff=time(NULL)-oldtime)>MAXHOTTIME)
		{
			cls_show_msg("line %d:����ʧ��!��ʱ��δ��⵽����̬", __LINE__);
			return;
		}

		status = GetPrinterStatus(MAXWAITTIME);
		if(status==SUCC)
			;
		else if(status==PRN_STATUS_OVERHEAT)
		{
			//if(i < 67)//���ݹ淶��������ӡ��׼��7���Ӹĳ�6�� 20140113 chensj
			//if(i < 44)//20160623 ���ºͽ�������´�ӡ��������ӡ���������ٴ�ӡ6�׽��͵����ٴ�ӡ4��,����ԭ���ǹ��ȵı����¶Ƚ��͵���70��
			//if(diff<MINHOTTIME)
			//{
			//	cls_show_msg("line %d:����ʧ��!��ʱ���ڲ�Ӧ��⵽����̬,��%d��", __LINE__,i);
			//	return;
			//}
			//else
			//{
				cls_show_msg("���Գɹ�!�Ѽ�⵽����̬(��ʱ%lds)", diff);//����20170426�����ٹ�ע����ʱ���ڳ�����ӡ��Ӧ������ױ����ȡ��Ĳ��Ե㣻����4���ڲ��ܹ��ȵ��ж���䣬ֻ��ע���Сʱ������ӡ�ܱ����ȼ���
				return;
			//}
		}
		else
		{
			cls_show_msg("line %d:��ӡ��״̬�쳣!(status=%d)", __LINE__, status);
			return;                         
		}
		
		for(cnt=0; cnt<5; cnt++)   /* �Ѳ��Դ�ӡ5���ڿ鳤��9����,��׼������6��,��Ҫ��ӡ����67��*/
			print_fill1();
		i++;
		if(lib_kbhit()==ESC)
			return;
	}		
}

static void prn_nopower(void)
{
	/*private & local definition*/
	int status = 0;
	time_t diff = 0, oldtime = time(NULL);
	
	/*process body*/
	while(1)
	{
		if((diff=time(NULL)-oldtime)>MAXHOTTIME)
		{
			cls_show_msg("line %d:����ʧ��!��ʱ��δ��⵽��Դ�쳣", __LINE__);
			return;
		}

		status = GetPrinterStatus(MAXWAITTIME);
		if(status==SUCC)
			;
		else if(status==PRN_STATUS_VOLERR)
		{
			cls_show_msg("���Գɹ�!�Ѽ�⵽��Դ�쳣(��ʱ%lds)", diff);
			return;
		}
		else
		{
			cls_show_msg("line %d:��ӡ��״̬�쳣!(status=%d)", __LINE__, status);
			return;                         
		}
		print_bill();
		if(lib_kbhit()==ESC)
			return;
	}	
}


static void prn_Invalid(void)
{
	/*private & local definition*/
	EM_PRN_STATUS emStatus=0;

	/*process body*/
	//���²���ȱֽ����װ�ķ�����ģ����ܳ��ֵ�ʧ��
	cls_show_msg("����ȷ����ӡ������ȱֽ̬,���������...");
	NDK_PrnStart();
	if(NDK_PrnGetStatus(&emStatus)!=NDK_OK||emStatus!=PRN_STATUS_NOPAPER )
	{
		cls_show_msg("line %d:δ��⵽ȱֽ̬!(status=%d)", __LINE__, emStatus);
		return;
	}
	cls_show_msg("��װ�ɾ��Ĵ�ӡֽ,���������...");
	if(NDK_PrnGetStatus(&emStatus)!=NDK_OK||emStatus!=PRN_STATUS_OK )
	{
		cls_show_msg("line %d:δ��⵽IPOK̬!(status=%d)", __LINE__, emStatus);
		return;
	}
	NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE, 1);
	if(rand()%2)
		print_guo();
	else
		print_font();//general1_print_triangle();
	cls_show_msg("������۲��ӡЧ��,��ȷ���Ƿ���ڴ�ӡʧ��");
}

static void __prn_attrib_unknown(void)
{
	/*private & local definition*/
	int type = lib_getprintertype();

	/*process body*/
	NDK_PrnImage(128, 64, 0, CCB_BMP);
	NDK_PrnImage(128, 64, 0, NL_BMP);
	NDK_PrnStr("NewLand�´�½00045\n00045�´�½NewLand\n�´�½NewLand00045\n");
	NDK_PrnImage(128, 64, type==PRN_TYPE_TP?56:4, CCB_BMP);
	NDK_PrnImage(128, 64, type==PRN_TYPE_TP?56:4, NL_BMP);
	NDK_PrnStr("NewLand�´�½00045\n00045�´�½NewLand\n�´�½NewLand00045\n");
	NDK_PrnStr("\f");
	NDK_PrnStart();
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
	{
		cls_show_msg("line %d:��ӡ��״̬�쳣!", __LINE__);
		return;
	}
	return;
}

static void __prn_attrib_exit(void)
{
	/*private & local definition*/

	/*process body*/
	NDK_PrnSetFont(PRN_HZ_FONT_24x24, PRN_ZM_FONT_16x16);
	NDK_PrnSetMode(PRN_MODE_ALL_DOUBLE, 1);
	NDK_PrnSetForm(8,32,16);//ֵ������̫��,ֵ+ͼ��xpos+xsize��Ҫ������ѹ��ģʽ������п�
	
	NDK_PrnStr("---���ô�ӡ����---\n"
				"��߽�: 8\n"
				"�ּ��: 32\n"
				"�м��: 16\n"
				"��������: 24x24\n"
				"��ĸ����: 16x16\n"
				"ѹ��ģʽ: ����Ŵ����Ŵ�˫����ֽ\n");
	__prn_attrib_unknown();
	NDK_PrnSetMode(PRN_MODE_NORMAL,0);//mode�ı�,����߽硢(��[��])����ֵӦ����Ӱ��
	NDK_PrnSetForm(8,32,16);//��Ϊsetprintmode�ڶ��������ķ�Χ��0-7,����Ҫ��16,����ʹ���½ӿ�
	NDK_PrnStr("---���ô�ӡ����---\n"
				"��߽�: 8\n"
				"�ּ��: 32\n"
				"�м��: 16\n"
				"��������: 24x24\n"
				"��ĸ����: 16x16\n"
				"ѹ��ģʽ: ����ѹ������ѹ��������ֽ\n");
	__prn_attrib_unknown();
	return;
}

static void prn_attrib_exit(void)
{
	/*private & local definition*/

	/*process body*/
	__prn_attrib_exit();
	cls_show_msg("�ֹ��˳�Ӧ��,�ٽ���\"δ֪���Դ�ӡ\",��ӡЧ��Ӧ��\"Ĭ�����Դ�ӡ\"һ��");//cls_show_msg("���ֹ��˳�����Ӧ�ó���,�ٽ���\"δ֪���Դ�ӡ\",��ӡЧ��Ӧ����");
	return;
}

static void prn_attrib_reboot(void)
{
	/*private & local definition*/

	/*process body*/
	__prn_attrib_exit();
	cls_show_msg("���������,�ٽ���\"δ֪���Դ�ӡ\",��ӡЧ��Ӧ��\"Ĭ�����Դ�ӡ\"һ��");
#if !K21_ENABLE
	NDK_SysExit(0);
#endif
	return;
}

static void prn_attrib_unknown(void)
{
	/*private & local definition*/

	/*process body*/
	NDK_PrnStr("---δ֪���Դ�ӡ---\n");
	__prn_attrib_unknown();
	return;
}

static void prn_attrib_default(void)
{
	/*private & local definition*/
	//int type=lib_getprintertype();

	/*process body*/
	/*
	��߽磺0
	�ּ�ࣺ0
	�м�ࣺ0
	Ĭ�Ϻ������壺16x16��6��
	Ĭ����ĸ���壺8x16��6��//8x8
	Ĭ��ģʽ��PRN_MODE_NORMAL ������������������
	*/
	NDK_PrnSetFont(PRN_HZ_FONT_16x16, PRN_ZM_FONT_8x16);
	
	NDK_PrnSetMode(PRN_MODE_NORMAL,1);//setprintmode(type==PRINTTYPE_TP?7:0, 0);
	NDK_PrnSetForm(0,0,0);
	print_Ex("---�ָ�Ĭ�ϴ�ӡ����---\n"
		"��߽�: 0\n"
		"�ּ��: 0\n"
		"�м��: 0\n"
		"��������: 16x16\n"
		"��ĸ����: 8x16\n"
		"ѹ��ģʽ: %s\n", /*type==PRN_TYPE_TP?"����������������������ֽ":*/"����Ŵ����Ŵ�˫����ֽ");
	__prn_attrib_unknown();
	return;
}

//���Բ���:����1��2���˳�Ӧ��,��ʱ�ٽ�Ӧ��ִ��3��4
static void prn_attrib(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	
	/*process body*/
	while(1)
	{
		nKeyin = cls_show_msg("1.�޸ĺ��˳�Ӧ��\n"
								"2.�޸ĺ�����\n"
								"3.δ֪���Դ�ӡ\n"
								"4.Ĭ�����Դ�ӡ");

		switch (nKeyin)
		{
		case '1':
			prn_attrib_exit();
			break;
		case '2':
			prn_attrib_reboot();
			break;
		case '3':
			prn_attrib_unknown();
			break;			
		case '4':
			prn_attrib_default();
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;
}

static void prn_buf_overload(void)
{
	/*private & local definition*/
	int i = 0;
	char PrintBuf[1000/*1024*/] = {0};

	/*process body*/
	memset(PrintBuf, 'H', sizeof(PrintBuf));
	for(; i<50; i++)//������ӡ״̬(�����Ƿ����ڴ�ӡ),��������ӡ����������,����\f
	{
		if(NDK_PrnStr(PrintBuf)!=SUCC)
		{
			cls_show_msg("���Գɹ�!�Ѽ�⵽��ӡ������!(%d)", i);
			return;
		}
	}

	NDK_PrnStr("\f");
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
	{
		cls_show_msg("line %d:��ӡ��״̬�쳣!", __LINE__);
		return;
	}
	cls_show_msg("line %d:δ��⵽��ӡ������!��ȷ���Ƿ�����(Ӧ��5w��H)", __LINE__);
}

static void prn_abnormity(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	
	/*process body*/
	while (1)
	{
#if 0 //��Ӱ��������"��ӡ����"(�Ḵλģʽ),�Ƴٵ�1-3��ȥ��
		//Ϊ�˱���������ӡ������Ӱ�죬����һ�´�ӡBUF��ͬʱ��λһ��
		if (FAIL==clrprintbuf())
		{
			cls_show_msg("line %d: ��ӡ����λʧ��", __LINE__);
			return;
		}
#endif
		if(GetPrinterStatus(MAXWAITTIME)!=SUCC)//�������������ĸ���
		{
			cls_show_msg("line %d:��ӡ��״̬�쳣!", __LINE__);
			return;                         
		}

		nKeyin = cls_show_msg("1.����2.��Դ�쳣\n"
								"3.ʧ��4.��ӡ����\n"
#if !K21_ENABLE //K21ƽ̨��֧�ֱ��ͱߴ�
		"5.��ӡ������"
#endif
		);

		switch (nKeyin)
		{
		case '1':
		//	if (FAIL==NDK_PrnInit(g_PrnSwitch))//Ϊ�˱���������ӡ������Ӱ�죬����һ�´�ӡBUF��ͬʱ��λһ��
			if(FAIL==lib_initprn(g_PrnSwitch))//modify 20131122 jiangym
			{
				cls_show_msg("line %d: ��ӡ����λʧ��", __LINE__);
				return;
			}
			prn_hot();
			break;
		case '2':
			if(ENTER==cls_show_msg("��ȷ��������װ���˵͵������,[ENTER]����,[����]����"))
			{
				//if (FAIL==NDK_PrnInit(g_PrnSwitch))//Ϊ�˱���������ӡ������Ӱ�죬����һ�´�ӡBUF��ͬʱ��λһ��
				if(FAIL==lib_initprn(g_PrnSwitch))//modify 20131122 jiangym
				{
					cls_show_msg("line %d: ��ӡ����λʧ��", __LINE__);
					return;
				}
				prn_nopower();
			}
			break;
		case '3':
			//if (FAIL==NDK_PrnInit(g_PrnSwitch))//Ϊ�˱���������ӡ������Ӱ�죬����һ�´�ӡBUF��ͬʱ��λһ��
			if(FAIL==lib_initprn(g_PrnSwitch))//modify 20131122 jiangym
			{
				cls_show_msg("line %d: ��ӡ����λʧ��", __LINE__);
				return;
			}			
			prn_Invalid();//���ڲ�������װֽ���Ƿ���ִ�ӡʧ��
			break;		
		case '4':
			prn_attrib();
			break;
#if !K21_ENABLE
		case '5':
			if(ENTER==cls_show_msg("��ȷ��������װ����ѭ����ӡֽ,[ENTER]����,[����]����"))
			{
				//if (FAIL==NDK_PrnInit(g_PrnSwitch))//Ϊ�˱���������ӡ������Ӱ�죬����һ�´�ӡBUF��ͬʱ��λһ��
				if(FAIL==lib_initprn(0))//��ӡ����������Ҫ��ǿ�ƹرձ��ͱߴ�
				{
					cls_show_msg("line %d: ��ӡ����λʧ��", __LINE__);
					return;
				}
				prn_buf_overload();
				if(FAIL==lib_initprn(g_PrnSwitch))//�������ָ�Ĭ�ϵı��ͱߴ�ģʽ
				{
					cls_show_msg("line %d: ��ӡ����λʧ��", __LINE__);
					return;
				}
			}
			break;
#endif
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	return;
}

int test_print_whitebill(void)
{
	/*private & local definition*/
	int succ = 0, cnt = 1000;
	
	/*process body*/
	if(lib_initprn(g_PrnSwitch)!=NDK_OK)
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s, line %d:��ӡ����λʧ��", __FILE__, __LINE__);
		return FAIL;
	}
	if(GetPrinterStatus(MAXWAITTIME)!=SUCC)
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"line %d:��ӡ��״̬�쳣!", __LINE__);
		return FAIL;                       
	}

	cls_show_msg("��׼���ðٷ�֮70���ҵĵ������(���ܽ����!)���в��ԡ�����Դ����ϴ󣬽������ѭ����ӡֽ���ԡ�ע�⣺���Թ���δ����\"��ӡ����ֽ\"��\"����\",��Ϊ����ͨ��;����ֽ�򿨶ٳ��ֺ���Ҫ�����豸");
	while(cnt)
	{
		//����
		cls_show_msg1(1,"���Դ�ӡ���ֿհ׵�������...\n��ʣ%d��(�����%d��)", cnt, succ);
		
		print_whitebill();
		cnt--;
		succ++;
	}
	
	cls_show_msg_record(FILENAME ,FUNCNAME ,"��ӡ���ֿհ׵�����������,δ��⵽����ֽ");

}
/****************************************************************
* function name 	 	:
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		  chensj     	   20130204     	created
*					
*****************************************************************/
void systest1(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	if(auto_flag==1)
	{
		//�Զ�����������
		//��ӡ����
		conf_prn();
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�н������
		prn_press("��ӡѹ��", NULL);
			
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	
	while(1)
       {
              keyin = cls_show_msg("1.ѹ������\n"
							"2.�������\n"
							"3.�쳣����\n"
							"4.����\n"
							"6.��ӡ�հ�����");
		switch(keyin) 
		{
		case '1':
			cls_show_msg1(1, "��ӡѹ��������...\n");
			prn_press("��ӡѹ��", NULL);
			break;
		case '2':
			cls_show_msg1(1, "��ӡ���ܲ�����...\n");
			prn_ability();
			break;
		case '3':
			cls_show_msg1(1, "��ӡ�쳣������...\n");
			prn_abnormity();
			break;
		case '4':
			conf_prn();
			break;
		case '6':
			test_print_whitebill();
			break;
		case ESC:
			cls_show_msg("�뻻�õĴ�ӡ���²���(ע���ӡЧ��)!���������...");
			return;
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}

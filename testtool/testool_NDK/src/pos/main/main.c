
/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: TCR��δ��Ʒ���Գ��������
* file name		: main.c
* Author 			: zhangnw
* version			: V1.0
* DATE			: 20060312
* directory 		: .\SRC\main\
* description		: 
* related document	: TCR690 API���Գ��� ��ϸ���˵����.doc
* 
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "apitest.h"	/*����Ϊ����API��д�ĳ�������������ڴ˴�*/
#include "version.h"
#if defined GUI_SUPPORT	//TBD
#include "guicore.h"
#include "widget.h"
#endif

/*---------------constants/macro definition---------------------*/

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
#if defined GUI_SUPPORT
extern void setuserscreen(int x, int y, int w, int h, image_t * img);
#endif

#if (DEBUGLEVEL==COVERAGE)
extern void com_end(void);//zhangnw20061108 add
#endif

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	: 
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  		zhangnw 		   20121203	 	porting from HOME version
*****************************************************************/
static int init(void)
{
	/*private & local definition*/
	time_t now = 0;
	char tmp[64] = {0};
	int nKeyin = 0, KeyValid = FALSE;

	/*process body*/
	//E90��ʹ��setlocale(LC_ALL, "C");���г�ʼ��
#if defined _CTYPE_INIT_SUPPORT
	_ctype_init();//zhangnw20060704 add for ��ʼ����׼C��(lingo������initusr.s������)
#endif
	//��׿��Ʒ��ʼ�����ö�̬��
#if ANDIROD_ENABLE
	dload_library();
#endif
	//memset(g_pps, 0x00, sizeof(g_pps));//zhangnw20110609add ���п���PPS����ʼΪ0(��1����)
	init_linksetting();
#if !(defined ME15C||defined ME15CHX||defined ME20||defined ME15B||defined ME66)	//ME15CΪ��ʡӦ�ÿռ䲻���벻�õĺ���
	init_netsetting();
#endif
	//zhangnw20060526 add Ϊ����TCR��������ͳһ��ʼ��һ������
	srand(time(&now));
#if defined ME11||defined ME15||defined ME15C||defined ME15CHX||defined ME20||defined ME15B||defined ME65||defined ME66//ME15�����˲��Է�ʽѡ����ʾ������,������ʱ����ʱ�ȴ�USB���ڴ���ɺ����������ʾ 20151019 linwl
	sleep(1);
#endif
	//�Զ������Ա�־ѡ��
	nKeyin = '1';
	nKeyin = cls_show_msg1(5,"_���Է�ʽ_\n"
							"0.���Զ����\n"
							"1.���Զ�����\n"
							"2.���ֶ�����");
	switch(nKeyin)
	{
	case '0':
		auto_flag = 0;
		break;
	case '1':
		auto_flag = 1;
		break;
	case '2':
		auto_flag = 2;
		break;
	default:
		auto_flag = 1;
		break;
	}
	
	//zhangnw20060410 add for select com
	while (!KeyValid)
	{
#if COMNUM==3
#if defined GP710
	nKeyin = cls_show_msg1(2, "ѡ�����ʹ���:\n���ֻ�����1\n����������1\n����������2\n");
#else
	nKeyin = cls_show_msg1(2, "ѡ�����ʹ���:\n�ٴ���1 �ڴ���2\n�۴���3\n");
#endif
#elif COMNUM==2
		nKeyin = cls_show_msg1(2, "ѡ�����ʹ���:\n�ٴ���1 �ڴ���2\n0 ������\n");
#elif COMNUM==0
		nKeyin = cls_show_msg1(2, "ѡ�����ʹ���:\n��USB���� \n0 ������\n");
#else //Ĭ��ֻ��һ������
#if defined ME15C
		nKeyin = cls_show_msg1(2, "ѡ�����ʹ���:\n�ٴ���2 \n0 ������\n");
#else
		nKeyin = cls_show_msg1(2, "ѡ�����ʹ���:\n�ٴ���1 \n0 ������\n");
#endif
#endif

		switch (nKeyin)
		{		
		case 0://Ĭ������  20150506δ֧��24Сʱ����ϲ��� �޸�Ĭ�ϰ��ղ˵�1�������ڻ�USB�������Ͳ��Խ��
		case ENTER:
		case '1':
#if COMNUM>0
			KeyValid = TRUE;
#if defined ME15C
			g_com = PORT_NUM_COM2;
#else
			g_com = PORT_NUM_COM1;		
#endif
			NDK_PortOpen(g_com, "115200,8,N,1");//initaux(g_com, BPS115200, DB8|STOP1|NP);
#else
			KeyValid = TRUE;//����ͨ���ڵĲ�Ʒ����֧�ִ�USB�������Ͳ��Խ��
			g_com = PORT_NUM_USB;
			NDK_PortOpen(g_com, "115200,8,N,1");
#endif
			break;
#if COMNUM>1	//zhangnw 20060801 modify
		case '2':
			KeyValid = TRUE;
			g_com = PORT_NUM_COM2;
			NDK_PortOpen(g_com, "115200,8,N,1");
			break;
#endif
#if COMNUM>2
		case '3':
			KeyValid = TRUE;
			g_com = PORT_NUM_COM3;
			NDK_PortOpen(g_com, "115200,8,N,1");
			break;
#endif
		case ESC:
			KeyValid = TRUE;
			return FAIL;
			break;	
		//case 0://Ĭ�ϲ�����
		case '0':
			KeyValid = TRUE;
#if defined NL829STD||defined IM81||defined SP10		//�䲻����,����ҪΪ��Ļ��ʾ�ض��򵽴�������ʼ��׼��
			g_com = PORT_NUM_COM1;
			NDK_PortOpen(g_com, "115200,8,N,1");	
#elif defined ME15C  //ANDIROD_ENABLE||defined N900||
			g_com = PORT_NUM_COM2;
			//NDK_AZPortOpen(g_com, "115200,8,N,1");
#elif defined ME11||defined ME15||defined ME20||defined ME15CHX||defined ME15B||defined ME65||defined ME66
			if(COMNUM==0)
				g_com = PORT_NUM_USB;//ME11 ֻ��USB���ڣ����Խ������������Ļ��ʾ����USB���� 
			else
				g_com = PORT_NUM_COM1;//����USB��ʹ��������
			//NDK_PortOpen(g_com, NULL);//��USB����λ���ƶ���������ڴ�������ME11 USB�����޷���������
			//sleep(5);
#else
			g_com = INVALID_COM;//zhangnw20061207 add ������COM����ʱ,ʹ��g_com����һЩ�ж�
#endif
			send_flag = FALSE;
			break;
		default:
			continue;
			break;
		}
		
		/*Start of zhangnw on 2006-12-15 9:30 1.6.6*/
#if DEBUGLEVEL==COVERAGE
		if (PORT_NUM_COM1==g_com)//if (AUX1==g_com) // 1 Ϊ���Ը�������ʹ�õĿ�(vlgtchk.c�ж�Ϊ1)
		{
			KeyValid = FALSE;//���ͽ���������͸������ݿڳ�ͻ
			cls_show_msg("�ͽ�������͸��������ݿڳ�ͻ!");
			return FAIL;
		}
#endif
		/*End of zhangnw on 2006-12-15 9:30 1.6.6*/
	}//zhangnw20060830 �������ֲ����

	//��PC�Ͳ��Ա���ͷ
	if (g_com!=INVALID_COM) //zhangnw20061215 add
	{
		strcpy(tmp, "��򿪴��ڹ���\n�Խ��ղ�������");//��Ϣ�Ӽ�,����ʾ���ٵ���ĻΪ����
		
#if 0
		while (BTN_OK!=ShowMessageBox(tmp, BTN_OK, -1));//�������Ԥ��һ���Զ������ĺ���
#else
		cls_show_msg1(1, tmp);
#endif
	}
#if !ANDIROD_ENABLE
	send_reporthead();
#endif
	return SUCC;
}

/****************************************************************
* function name 	 	: send_modtitle
* functional description 	: 
* input parameter		: 
* output parameter	 	: 
* return value			: 
* history 		 		: author		date			remarks
*			 		 zhangnw	 20060419     	created for ��func.h����ȡ���ú���
*					 zhangnw	 20060602   	   delete �����int port����,ֱ��ʹ��g_com
*					 zhangnw		 20060704		�޸ĺ�������(int row,int col)->(int cnt)
*****************************************************************/
static void send_modtitle(int cnt)
{
	/*private & local definition*/
#include "testmod.h" //����ļ�Ҫ����/*private & local definition*/�����

	/*process body*/
	if (send_flag)
	{
		while(NDK_PortTxSendOver(g_com)!=NDK_OK);//���з����ʱ��//clrportbuf(g_com, 0);
#if 0//ANDIROD_ENABLE
		NDK_AZPortWrite(g_com, strlen(testmod[cnt]), testmod[cnt]);
#else
		NDK_PortWrite(g_com, strlen(testmod[cnt]), testmod[cnt]);
#endif
	}
	else
		;
}

#if defined GUI_SUPPORT
void * Dorefresh(void * arg)
{
	do{
//		change_font(FONT_DOT_16);
		widget_refresh((widget_t *)arg,0,320-22,240,22);
//		change_font(FONT_DOT_24);//��ԭ
		sleep(2);
	}while(1);
}

void thread_to_refresh_statusbar(widget_t * win)
{
	pthread_t dorefresh;
	pthread_create(&dorefresh, NULL, Dorefresh, (void *)win);
	pthread_detach(dorefresh);
}

void change_user_view(void)
{
	extern guicore_t sys;

	sys.view.h-=22;
	statusbar_disp(0x0c,0);
}

static widget_t * create_windows(void)
{
	widget_t *win = NULL, *panel = NULL;
	
	if ((win=window_new(WIN_PROP_NONE, NULL, NULL))==NULL)//if ((win=window_new(WIN_PROP_TITLE_STRING, "API���Գ���", NULL))==NULL)
		return NULL;
	if ((panel=panel_new(win, NULL))==NULL)
	{
		window_destroy(win);
		return NULL;
	}

	change_user_view();
	thread_to_refresh_statusbar(win);
	return win;
}
#endif


#include "menu.h" //zhangnw20060417 modify
#include "list.h"	//zhangnw20111124�������ļ��ϲ�Ϊһ���ļ�
#define  SETCASE "/appfs/setcase.txt" 

//ʹ�ø������ļ�
#if CONFFLAG
#define	MAXMODNUM	256 //���֧��256��ģ�飬ÿ��ģ��256������ 
static int changecase(void)
{
	//����������ѡ����������ɵ��ļ�	
	int fp = 0;
	uint size = 0;
	int ret = 0, len = 0;
	char readbuf[4096] = {0};
	int i = 0, j = 0, k = 0;
	
	if(((ret=NDK_FsFileSize(SETCASE,&size))!=NDK_OK)||(size>sizeof(readbuf)))//��ȡSETCASE�ļ���С
	{
		cls_show_msg("%s line:%d�����ļ���С�쳣(ret=%d,size=%d)",__FILE__, __LINE__, ret, size);
		return FAIL;
	}
	if((fp=NDK_FsOpen(SETCASE, "r"))<0) //�������ļ�������ȡ������Ϣ
	{
		cls_show_msg("%s line:%d�������ļ�ʧ��(%d)",__FILE__, __LINE__, fp);
		return FAIL;
	}
	if((len=NDK_FsRead(fp, readbuf, size))!=size)
	{
		cls_show_msg("%s line:%d��ȡ�����ļ�ʧ��(%d)",__FILE__, __LINE__, len);
		return FAIL;
	}

#if 1
	//�����������Ҫ��list.h�д��ڣ����SP60.INI��û��������ô�������������Ҫ��Ӧ�ļ������б���������
	char** funstr[MAXMODNUM] = {funcstr0, funcstr1, funcstr2, funcstr3, funcstr4, funcstr5,
								funcstr6, funcstr7, funcstr8, funcstr9, funcstr10, funcstr11,
								funcstr12, funcstr13, funcstr14, funcstr15, funcstr16, funcstr17,
								funcstr18, funcstr19, funcstr20, funcstr21, funcstr22,NULL};

	PF* func[MAXMODNUM] = {func0, func1, func2, func3, func4, func5, 
							func6, func7, func8, func9, func10, func11,
							func12, func13, func14, func15, func16, func17,
							func18, func19, func20, func21, func22,NULL};
	
	uchar* id[MAXMODNUM] = {id0, id1, id2, id3, id4, id5,
							id6, id7, id8, id9, id10, id11,
							id12, id13, id14, id15, id16, id17,
							id18, id19, id20, id21, id22,NULL};
#else
	char** funstr[3] = {funcstr0,NULL};
	PF* func[3] = {func0,NULL};
	uchar* id[3] = {id0,NULL};
#endif
	
	for(k=0;k<MAXMODNUM;k++)
	{	
		if(funstr[k]==NULL)//�ﵽģ��������ʱ�˳�
			break;
		j = 0;
		for(i=0;i<MAXMODNUM;i++)
		{
			if(funstr[k][i]==NULL)//�ﵽ����������ʱ�˳�
				break;
			//cls_show_msg("k=%d,i=%d,j=%d,funstr=%s",k,i,j,funstr[k][i]);
			if(strstr(readbuf,funstr[k][i]))//�������ļ��в��Ҳ�����
			{
				func[k][j] = func[k][i];
				id[k][j] = id[k][i];
				j++;
			}
		}
		func[k][j] = NULL;
		id[k][j] = 0;
	}
	
	NDK_FsClose(fp);
	return SUCC;
}
#endif
/****************************************************************
* function name 	 	: nlmain
* functional description 	: 
* input parameter		:
* output parameter	 	: 
* return value			:
* history 		 		: author		date			remarks
*			 		 zhangnw	 	20121203     	porting from HOME version
*****************************************************************/
int nlmain(void)
{
	/*private & local definition*/
	int nKeyin = '0';//zhangnw20060704 add for �Զ�����(Ĭ��), auto_flag = TRUE
	int SelPage = 1, maxpage = 1;//zhangnw20060419 add
#if defined GUI_SUPPORT
	widget_t * win = NULL;
#endif

	/*process body*/
#if defined ME15CHX||defined ME15B
	int handle = 0;
	handle = NDK_LedOpen("blue");
	NDK_LedOn(handle);
#endif
#if defined ME66
	int handle = 0;
	handle = NDK_LedOpen("cam");
	NDK_LedOn(handle);
#endif
#if defined ME11||defined ME15||defined ME15C||defined ME20
	NDK_LedStatus(LED_RFID_BLUE_FLICK);//����LED�򿪲���,������Һ�����豸ȷ�ϳ������гɹ�
#endif
	NDK_SysBeep();
#if defined ME11||defined ME15||defined ME20||defined ME15CHX||defined ME15B||defined ME65||defined ME66//�ں�����ʼ��ֱ�Ӵ�USB���ڣ�����USB�����޷�������������������Ҫ����ȷ��
	if(COMNUM==0)
		NDK_PortOpen(PORT_NUM_USB, NULL);
	else	
		NDK_PortOpen(PORT_NUM_COM1, "115200,8,N,1");//����USB����,�������ڻ���3���ط���Ҫ��:lib.h��COMNUM�Ķ���,layer.c�е�init_linksetting,main.c�ĵ�127�� 
#elif 0//ANDIROD_ENABLE 
	NDK_AZPortOpen(PORT_NUM_COM2, "115200,8,N,1");
#elif defined ME15C//ME15Cʹ��USBת���ڶ�Ӧ����COM2  //defined N900||
	NDK_PortOpen(PORT_NUM_COM2, "115200,8,N,1");
//#elif defined IM81AZ
//	NDK_PortOpen(PORT_NUM_COM1, "115200,8,N,1");
#else
	//�����޷�����Ϣ��ʾ��Һ�����豸��ѡ����������������ڱ�������
#endif

#if !SCR_DISABLE||defined SP10	//û��Һ�����豸�����н����ʼ����������֧����ʾ
#if defined GUI_SUPPORT
    if ((win=create_windows())==NULL)
		return FAIL;

	window_show(win);
	set_autoupdate(1);//�����Զ�ˢ�½���,������Ҳ����,Ĭ�Ͼ���1
	setuserscreen(0, 0, COLU_NUMS+1, LINE_NUMS+1, NULL);
#else
	if(NDK_ScrInitGui()!=NDK_OK)
		return FAIL;
#if !K21_ENABLE
	if(NDK_ScrAutoUpdate(0, NULL)!=NDK_OK)//NDK_ScrAutoUpdate(1, NULL)��ʵʱˢ��,����Ļʱ���ܻ�����
		return FAIL;
#endif
	//���Ի���ʹ��ͳһ�Ŀ�������(ΪLCD�ߴ�)
	//������Ϣ��ʽ:��*��
	//ͬʱ����ʹ��Ĭ��Ӣ������Ϊ12*6,����12*12;С����(Ӣ��)Ϊ8*8;�����չ��Ӣ��16*16,����16*8
	//ͬʱ����ʹ��Ĭ��Ӣ������Ϊ16*8,����16*16;С����(Ӣ��)Ϊ16*8;�����չ��Ӣ��24*24,����24*12
#if !ANDIROD_ENABLE
	if((GUI_GetLCDSize()!=NDK_OK)||(GUI_GetFontSize()!=NDK_OK))
		return FAIL;
#endif
#if !K21_ENABLE
	if(NDK_ScrSetViewPort(0, 0, GUI_GetLCDWidth(), GUI_GetLCDHeight())!=NDK_OK)
		return FAIL;
#endif
#endif
#endif
	if (init()!=SUCC)
	{
#if 0 //zhangnw20061016 �����ʾ
		while(BTN_OK !=ShowMessageBox("���Ի�����ʼ��ʧ��!", BTN_OK, -1));
#endif
#if defined ME11||defined ME15||defined ME20||defined ME15CHX||defined ME15B||defined ME65||defined ME66//�����������ʼ��ʧ���˳�ʱҪ�ر�USB���ڣ����������PC��������
		if(COMNUM==0)
			NDK_PortClose(PORT_NUM_USB);
		else
			NDK_PortClose(PORT_NUM_COM1);//����USB����,�ر�������
#elif 0//ANDIROD_ENABLE
		NDK_AZPortClose(PORT_NUM_COM2);
#elif defined ME15C //defined N900||
		NDK_PortClose(PORT_NUM_COM2);
//#elif defined IM81AZ
//		NDK_PortClose(PORT_NUM_COM1);
#else
		//�����޷�����Ϣ��ʾ��Һ�����豸��ѡ����������������ڱ�������
#endif
		return FAIL;
	}

#if CONFFLAG
	//�ж������ļ��Ƿ���ڣ�����������������ļ��޸ĺ����б�
	if(NDK_FsExist(SETCASE)==NDK_OK)
	{
		if(changecase()!=SUCC)
		{
			cls_show_msg("%s line:%d�����ļ��쳣��������������ȷ�������ļ�",__FILE__, __LINE__);
			return FAIL;
		}
	}
	else//��������ļ���������,��ʾ���������˳�
	{
		if(cls_show_msg("%s line:%d�����ļ�������[ȷ�ϼ�]����,[����]�˳�",__FILE__, __LINE__)!=ENTER)
			return FAIL;
	}	
#endif

	//ȷ��TCRMENU �����ҳ
	maxpage = sizeof(tcrmenu)/sizeof(tcrmenu[0]);
	while (1)
	{
		auto_test(&auto_flag, &nKeyin, &SelPage, maxpage, tcrmenu, skip_modID);

		switch (nKeyin)
		{
			//��������ĺ���ȫ�������޲���ʽ��
#include "func.h"
			break;//zhangnw20060704 add ����,case 1--8û��break����������Ҳ��break

#if !(defined ME15CHX||defined ME15B||defined ME66)	//ME15C��Դ�������跭ҳ
			//����Է�ҳ�Ĵ���Ҳ���Ը�Ϊѭ����ҳ
#if defined IM81||defined IM81AZ
		case K_ZMK:
#endif
		case DOWNKEY:/*�·���*/
			if (SelPage==maxpage)
				while (BTN_OK!=ShowMessageBox("�ѵ�ĩҳ", BTN_OK, -1));
			else
				SelPage++;
			
			break;
#if defined IM81||defined IM81AZ
		case DOT:
#endif
		case UPKEY:/*�Ϸ���*/
			if (SelPage==1)
				while (BTN_OK!=ShowMessageBox("�ѵ���ҳ", BTN_OK, -1));
			else
				SelPage--;
			
			break;
#endif
			/*
			* ���ڲ�Ʒ���̴��ڲ���,����Ӧ����ѡ�񼸿��Ʒ���еļ�
			* ��,���ּ�0-9�� 1-8�ѱ���Ϊѡ��������
			*/
		case '0'://����һ������,���������Զ�����
			if(ShowMessageBox("������ʾ����\n�����Զ�������?", BTN_OK | BTN_CANCEL, -1)==BTN_OK)
			{
				auto_flag = TRUE;
				//nKeyin = '0';//zhangnw20060704 add for �Զ�����ʱ�ĳ�ʼֵ
			}
			break;
#if !(defined ME15CHX||defined ME15B||defined ME66)	//ME15C��Դ�������跭ҳ		
		case '9'://zhangnw20060704 add for ���ٷ�ҳ
			jump2page(maxpage, &SelPage);
			break;
#endif
		case ESC://cancel
			if(ShowMessageBox("�������桪��\n������ͼ�������?", BTN_OK | BTN_CANCEL, -1)==BTN_OK)
			{
#if defined ME15CHX||defined ME15B||defined ME66
				NDK_LedOff(handle);
				NDK_LedClose(handle);
#endif				
#if  0//ANDIROD_ENABLE
				NDK_AZPortClose(g_com);
#else
				NDK_PortClose(g_com);
#endif
#if defined ME11||defined ME15||defined ME15C||defined ME15CHX||defined ME20||defined ME15B||defined ME66
				sleep(5);//Ϊ������ʱ��رտ���̨,�˵����ؽ�����������
#endif
				return FAIL;//shutdown();
			}
			break;
		case 8:/*PC�����˸��*/
		case BACKSPACE:/*��*/
			if(ShowMessageBox("�������桪��\n������������?", BTN_OK | BTN_CANCEL, -1)==BTN_OK)
			{
#if defined ME15CHX||defined ME15B||defined ME66
				NDK_LedOff(handle);
				NDK_LedClose(handle);
				NDK_PortClose(g_com);
				sleep(1);
#endif	
				NDK_SysReboot();//nlexit(0);
			}

			break;

#if !(defined IM81||ANDIROD_ENABLE)
		case DOT://Ԥ��
			if(ShowMessageBox("�������桪��\n������ػ���?", BTN_OK | BTN_CANCEL, -1)==BTN_OK)
			{
#if defined ME15CHX||defined ME15B||defined ME66
				NDK_LedOff(handle);
				NDK_LedClose(handle);
				sleep(1);
#endif	
				NDK_SysShutDown();
			}
			break;
#endif
	
		case ENTER://zhangnw20061215 add ENTER��Ӧ���͸�����
#if DEBUGLEVEL==COVERAGE //zhangnw20061027 add for �����ʲ���(�̶�ʹ��AUX1)vlgtchk.c��д����PIPE_STD(Ϊ1)
			if(ShowMessageBox("�򿪴��ڹ��߽���\n����������", BTN_OK | BTN_CANCEL, -1)==BTN_OK)
			{
				NDK_PortOpen(PORT_NUM_COM1, "115200,8,N,1");//initaux(AUX1, BPS115200, DB8|STOP1|NP);
				com_end();
			}
#else
			if(ShowMessageBox("�������桪��\nҪ�˳����Գ�����?", BTN_OK | BTN_CANCEL, -1)==BTN_OK)
#if K21_ENABLE
			{
#if defined ME15CHX||defined ME15B||defined ME66
				NDK_LedOff(handle);
				NDK_LedClose(handle);
#endif
				return FAIL;//linwl20130603
			}
#else
			{
				if(ShowMessageBox("�������桪��\nҪ�������Գ�����?", BTN_OK | BTN_CANCEL, -1)==BTN_OK)
					NDK_SysExit(0x52);//20151025Ӧ����������:��������0x52ʵ���˳�Ӧ�ó����Ҳ���ʾ4ͼ���������Ӧ�ó���Ĺ���
				else
					NDK_SysExit(0);
			}
#endif
#endif
			break;
		default ://������չ??
			break;
		}
	}
}


#if 0
#include<stdio.h>

void main(void)
{
	NDK_ScrClrs();
	NDK_ScrDispString(2, 2, "app test 4\r\n", 0);
	NDK_KbGetCode(0, NULL);
	return;
}
#endif



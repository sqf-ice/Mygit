/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 
* Author 			: zhengry
* version			: V1.0
* DATE			: 2016/05/31
* directory 		: 
* description		: ɨ��ͷ/����ͷ�ۺϲ���
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  2016/05/31  zhengry
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define		TESTITEM	"ɨ��ͷ/����ͷ�ۺϲ���"
#define 	MAXTIME		10

/*----------global variables declaration("extern")--------------*/
static int 	nConfInit = 0;	 /*�Ƿ����ù��˲���:0δ���� 1������*/
static int type=0; //0��ʾ��֧��ɨ��ͷ 1��ʾ��֧������ͷ

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
void scan_press(void)
{
	/*private & local definition*/
	int ret = 0, i = 0, succ = 0;

	/*����ǰ��*/
	if(nConfInit == 0)
	{
		cls_show_msg1(g_keeptime, "����δ����");	
		return;
	}
	
	/*process body*/
	if((ret=NDK_ScanInit())!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�豸��ʼ��ʧ��(%d)", __LINE__, ret);
		return;
	}
#if CAMERA_ENABLE
	if(type==1)
	{
		NDK_ScanSet(CAMERA_SETTYPE_IMAGE, g_ShowImage);
		NDK_ScanSet(CAMERA_SETTYPE_SATURATION, g_Saturation);
		NDK_ScanSet(CAMERA_SETTYPE_SHARPNESS, g_Sharpness);
		NDK_ScanSet(CAMERA_SETTYPE_GAMMA, g_Contrast);
	}
#else
	if(type==0)
	{
		NDK_ScanSet(SCAN_SETTYPE_FLOODLIGHT, g_Floodlight);
		NDK_ScanSet(SCAN_SETTYPE_FOCUSLIGHT, g_Focuslight);
		NDK_ScanSet(SCAN_SETTYPE_SENSITIVITY, g_Sensitivity);
	}
#endif
	while(1)
	{		
		if(cls_show_msg1(2,"����%d���ڽ���ɨ��(��ɨ��%d��,�ɹ�%d��),��ESC�˳�", MAXTIME, i+1, succ)==ESC)
			break;
		i++;
		
		if((ret=lib_doscan(g_ScanType, MAXTIME))!=NDK_OK)
		{
			cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
			continue;
		}
		
		succ++;
	}
#if CAMERA_ENABLE
	NDK_ScanClose();
#endif
#if SCAN_SOFT_ENABLE
	NDK_ScanExit();
#endif
	
	cls_show_msg("ɨ��������\n�ܴ���:%d\n�ɹ�:%d��", i, succ);
	return;
}

#if CAMERA_ENABLE
void abnormal(void)
{		
	int ret=0;

	//��������ͷ���ⲿ��U���ǽ���ͬһ��usb��hub����,ͨ��һ��gpio���о�����ʹ������ͷ�����ⲿu��,����ͬʱֻ��һ����·��ͨ��,ֻ����һ��
	//����ǰ��
	NDK_PortClose(PORT_NUM_USB);

	//δ��������ͷɨ��,USB��Ӧ����
	if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:��USBʧ��(%d)", __LINE__, ret);
		return;
	}
	if((ret = NDK_PortClose(PORT_NUM_USB)) != NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:�ر�USBʧ��(%d)", __LINE__, ret);
		return;
	}

	//�ر�USB��,��������ͷӦ����
	if((ret=NDK_ScanInit())!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
		return;
	}
	
	if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_ERR_USB_LINE_UNCONNECT)//USB��Ӧʧ��
	{
		cls_show_msg1(g_keeptime,"line %d:��USBʧ��(%d)", __LINE__, ret);
		return;
	}
	//�˴������ж�
	//USB�򿪳ɹ�����£��������ݲ���
	//USB��ʧ������£��������ݲ���
	NDK_ScanSet(CAMERA_SETTYPE_IMAGE, g_ShowImage);
	NDK_ScanSet(CAMERA_SETTYPE_SATURATION, g_Saturation);
	NDK_ScanSet(CAMERA_SETTYPE_SHARPNESS, g_Sharpness);
	NDK_ScanSet(CAMERA_SETTYPE_GAMMA, g_Contrast);
	if((ret=lib_doscan(g_ScanType, MAXTIME))!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:ɨ��ʧ��(%d)", __LINE__, ret);
		if(g_ScanType==1)
			NDK_ScanClose();
		return;
	}
	if(g_ScanType==1)
		NDK_ScanClose();
	
	//����ͷ�رպ�,USB��Ӧ����
	if((ret = NDK_PortOpen(PORT_NUM_USB, "115200,8,N,1")) != NDK_OK)
	{
		cls_show_msg1(g_keeptime,"line %d:��USBʧ��(%d)", __LINE__, ret);
		return;
	}
	NDK_PortClose(PORT_NUM_USB);
	return;
}
#else
void abnormal(void){cls_show_msg("�ò�Ʒ��֧������ͷ�쳣����");return;}
#endif

void config(void)
{
	/*private & local definition*/
	int ret=0;
	uint unLen=0;
	char CntStr[8]={"0"};
	
	/*process body*/
#if CAMERA_ENABLE
	cls_printf("��ѡ��Ҫ���Ե�Ӳ������(0.ɨ��ͷ 1.����ͷ)Ĭ��Ϊ0:");
	strcpy(CntStr, "0");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen, INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		type = 0;
	else
		type = atoi(CntStr);
	
	if(type!=1 && type!=0) 
	{
		cls_show_msg("Ӳ���������ô���");	
		return;
	}
	if(type==1)
		conf_camera();
	else
		conf_scan();
#else
	cls_printf("ѡ��ɨ��ģʽ:\n0.ͬ��(����)ɨ��\n1.�첽(������)ɨ��:\n");
	strcpy(CntStr, "1");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 30, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		g_ScanType = 1;
	else
		g_ScanType = atoi(CntStr);
	switch(g_ScanType)
	{
		case 0:
#if !SCAN_ENABLE
			cls_show_msg1(5,"��֧��ͬ��ɨ�跽ʽ,������ѡ��");
#endif
			break;
		case 1:
#if !SCANASYCN_ENABLE
			cls_show_msg1(5,"��֧���첽ɨ�跽ʽ,������ѡ��");
#endif
			break;	
	}
#endif
	return;
}

void scan_ability(void)
{
	/*private & local definition*/
	int ret = 0, succ = 0, cnt = 10, bak = 0;
	uint plen = 0;
	uchar pvalue[500] = {0};
	time_t oldtime = 0;
	float ScanTime = 0, ScanTime0 = 0;
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
	uint poutlen = 0;
	uchar poutvalue[500] = {0};
#endif
#if K21_ENABLE
	char str[32] = {0};
#endif
#if defined ME66
	uint a = 0, a1= 0;
#endif

	/*process body*/
	/*����ǰ��*/
	if(nConfInit == 0)
	{
		cls_show_msg1(g_keeptime, "����δ����");	
		return;
	}
		
	if((ret=NDK_ScanInit())!=NDK_OK)
	{
		cls_show_msg1(g_keeptime, "line %d:�豸��ʼ��ʧ��(%d)", __LINE__, ret);
		return;
	}
#if CAMERA_ENABLE
	if(type==1)
	{
		NDK_ScanSet(CAMERA_SETTYPE_IMAGE, g_ShowImage);
		NDK_ScanSet(CAMERA_SETTYPE_SATURATION, g_Saturation);
		NDK_ScanSet(CAMERA_SETTYPE_SHARPNESS, g_Sharpness);
		NDK_ScanSet(CAMERA_SETTYPE_GAMMA, g_Contrast);
	}
#endif
#if SCAN_ENABLE
	if(type==0)
	{
		NDK_ScanSet(SCAN_SETTYPE_FLOODLIGHT, g_Floodlight);
		NDK_ScanSet(SCAN_SETTYPE_FOCUSLIGHT, g_Focuslight);
		NDK_ScanSet(SCAN_SETTYPE_SENSITIVITY, g_Sensitivity);
	}
#endif

	if(g_ScanType!=0 && g_ScanType!=1)
	{
		cls_show_msg1(10,"line %d:д���������(%d)", __LINE__, g_ScanType);
		return;
	}
	cls_show_msg("��׼���ô�ɨ���ͼ��,���������ʼɨ��");
    	bak = cnt;                                                                
	while(cnt--)
	{		
		cls_printf("��%d��ɨ����...", bak-cnt);
		plen=0;
		ScanTime0 = 0;
		memset(pvalue,0,sizeof(pvalue));
#if SCANASYCN_ENABLE	  //������ɨ��
		if(g_ScanType)
		{
#if defined ME66
			NDK_SysReadWatch(&a1);
#else
			lib_StartStopwatch();
#endif
			if((ret=NDK_ScanStart()) != NDK_OK )
			{
				NDK_ScanStop();
				cls_show_msg1_record(FILENAME, FUNCNAME, 1, "line %d:��%d������ͷ����ɨ��ʧ��(%d)", __LINE__, bak-cnt, ret);
				continue;
			}   
			oldtime = time(NULL);
			while(1)
			{	
				if((ret=NDK_ScanState(pvalue, &plen)) == NDK_OK)
				{
#if defined ME66
					NDK_SysReadWatch(&a);
					ScanTime0 = (a - a1)/(1000*1.0);
#else
					ScanTime0 = lib_ReadStopwatch();
#endif
					//ScanTime += lib_ReadStopwatch();  //���ӿ��Կ�ÿһ�ε�ɨ��ʱ��
					ScanTime += ScanTime0;
					succ++;
#if CPU5810X_ENABLE||defined ME50NX||defined ME68		
					memset(poutvalue,0,sizeof(poutvalue));
					if((ret=NDK_Utf8ToGbk(pvalue, plen, poutvalue, &poutlen))!=NDK_OK)
					{
						cls_show_msg1_record(FILENAME, FUNCNAME, 1,"line %d:��%d��UTF8תGBKʧ��(%d)", __LINE__, bak-cnt, ret);
						break;
					}
					memset(str,0,sizeof(str));
					ftos(ScanTime0,str);
					//cls_show_msg1(1,"����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s", poutlen, poutvalue);
					cls_show_msg1_record(FILENAME, FUNCNAME, 1,"��%d�ν���ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ɨ��ʱ��:%ss", bak-cnt, poutlen, poutvalue, str);
#else
					memset(str,0,sizeof(str));
					ftos(ScanTime0,str);
					//cls_show_msg1(1,"����ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s", plen, pvalue);	
					cls_show_msg1_record(FILENAME, FUNCNAME, 1,"��%d�ν���ɹ�!ɨ�赽�ĳ���Ϊ:%d������:%s,ɨ��ʱ��:%ss", bak-cnt, plen, pvalue, str);	
#endif
					break;
				}
				NDK_SysMsDelay(10);
				if((time(NULL) - oldtime)>MAXTIME)
				{
					NDK_ScanStop();
					cls_show_msg1(1,"line %d:ɨ�賬ʱ", __LINE__);
					break;
				}
			}
			if((ret = NDK_ScanStop()) != NDK_OK)
			{
				cls_show_msg1(1,"line %d:�ر�����ͷʧ��(%d)", __LINE__, ret);
				continue;
			}
		}
#endif
#if SCAN_ENABLE
		if(!g_ScanType)
		{
			lib_StartStopwatch();
			if((ret=NDK_ScanDoScan(MAXTIME, pvalue, &plen))!=NDK_OK)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,1,"line %d:��%d������ʽɨ��ʧ��(%d)",  __LINE__, bak-cnt, ret);
				continue;
			}
			ScanTime0 = lib_ReadStopwatch();
			ScanTime += ScanTime0;
			//ScanTime += lib_ReadStopwatch();
			succ++;
#if CPU5810X_ENABLE||defined ME50NX||defined ME68
			memset(poutvalue,0,sizeof(poutvalue));
			if((ret=NDK_Utf8ToGbk(pvalue, plen, poutvalue, &poutlen))!=NDK_OK)
			{
				cls_show_msg1_record(FILENAME,FUNCNAME,1,"line %d:��%d��UTF8תGBKʧ��(%d)", __LINE__, bak-cnt,ret);
				continue;
			}
			memset(str,0,sizeof(str));
			ftos(ScanTime0,str);
			//cls_show_msg1(1,"ɨ�赽�ĳ���Ϊ:%d������:%s", poutlen, poutvalue);
			cls_show_msg1_record(FILENAME, FUNCNAME, 1,"��%d��ɨ�赽�ĳ���Ϊ:%d������:%s,ɨ��ʱ��:%ss", bak-cnt, poutlen, poutvalue, str);
#else
			//cls_show_msg1(1,"ɨ�赽�ĳ���Ϊ:%d������:%s", plen, pvalue);
			cls_show_msg1_record(FILENAME, FUNCNAME, 1,"��%d��ɨ�赽�ĳ���Ϊ:%d������:%s,ɨ��ʱ��:%fs", bak-cnt, plen, pvalue, ScanTime0);
#endif
		}
#endif
	}
	cls_show_msg_record(FILENAME,FUNCNAME,"ͨѶ�ܴ���%d,�ɹ�����%d\n", bak, succ);
	
#if !K21_ENABLE 
	cls_show_msg_record(FILENAME,FUNCNAME,"ɨ������:%fs/��\n", ScanTime/succ);
#else
	memset(str,0,sizeof(str));
	ftos(ScanTime/succ,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "ɨ������:%ss/��\n", str);
#endif
#if CAMERA_ENABLE
	NDK_ScanClose();
#endif	
#if SCAN_SOFT_ENABLE
	NDK_ScanExit();
#endif
	return;
}

void systest33(void)
{
	/*private & local definition*/
	int nKeyin=0;

	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);
	while(1)
	{
		nKeyin=cls_show_msg("1.ѹ��  2.�쳣\n"
							"3.���� 0.Ӳ������\n"
							);
		switch(nKeyin)
		{
			case '1':
				scan_press();
				break;
			case '2':
				abnormal();
				break;	
			case '3':
				scan_ability();
				break;
			case '0':
				config();
				nConfInit=1;
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


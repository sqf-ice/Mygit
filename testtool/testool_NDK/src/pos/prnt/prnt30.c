/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt30.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ��NDK_ttf�ű���ӡ�ӿڲ���
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
#define TESTAPI	"ttf_script_print"	
#define MAXWAITTIME 90
#define DESTFILE "/appfs/ums"
#define SOURCEFILE "/mnt/sdcard/picture/ums"  
#define PNGFORMAT ".png"
#define IMAGEPATH "/mnt/sdcard/picture/"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 		:void prnt30(void)
* functional description 	: 
* input parameter	 		:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author		date		remarks
*			  	  	  	    sull           20180723         created
*****************************************************************/
//������ttf��ӡ�ӿ�ֻ�ж�������ȷ��񣬲��жϴ�ӡ��״ֵ̬,��Ҫ�Լ�ȥ����status�����ж�
static int getStatus(void)
{
	EM_PRN_STATUS emStatus = 0;
	int ret = 0;

	while(1)
	{
		if((ret=NDK_PrnGetStatus(&emStatus))!=NDK_OK )
		{
			send_result("line %d:%s��ȡ��ӡ״̬ʧ��(%d)",__LINE__,TESTAPI,ret);
			return ret;
		}
		switch(emStatus)
		{
			case PRN_STATUS_BUSY:
				continue;
				break;
			case PRN_STATUS_OK:
				return PRN_STATUS_OK;
				break;
			case PRN_STATUS_NOPAPER:
				return PRN_STATUS_NOPAPER;
				break;
			default:
				send_result("line %d:%s��ӡ��״̬�쳣(%d)", __LINE__, TESTAPI, emStatus);
				return emStatus;
		}
	}	
}
void prnt30(void)
{
	//����Ϊ�ֲ�����
#include	"dotfont.h"
#include	"hzfont.h"

	EM_PRN_STATUS emStatus = 0;
	int nBusy = FALSE, nKeyin = 0, i = 0;
	time_t nOldtime = 0, nDiff = 0;
	int nRet = 0, nRet1 = 0;
	char *type[] = {"l", "c", "r", "l"};
	//char *position[] = {"����", "����", "����", "����"};
	char *position[] = {"居左", "居中", "居右", "居左"};
	int p = 0;
	#if 0  //ttf_str��GB2312��ʽ���������ⶨλ
	char *ttf_str[] = {
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪϵͳĬ��Ӣ������\n!font DroidSans.ttf\n!NLFONT 9 12 3\n*text l DroidSansDroidSans\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪϵͳĬ��Ӣ�Ĵ�����\n!font DroidSans-Bold.ttf\n!NLFONT 9 12 3\n*text l DroidSansDroidSans\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪϵͳĬ����Ӣ������\n!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l �´�½֧���������޹�˾newlandpatment\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪDroidaSansMono����\n!font DroidSansMono.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪDancingScript������\n!font DancingScript-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪDancingScript��������\n!font DancingScript-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRoboto���棨Ӣ�ģ�\n!font Roboto-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRoboto���壨Ӣ�ģ�\n!font Roboto-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRoboto��б�壨Ӣ�ģ�\n!font Roboto-BoldItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",						
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRobotoRobotoб�壨Ӣ�ģ�\n!font Roboto-Italic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRobotoϸ���壨Ӣ�ģ�\n!font Roboto-Thin.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRobotoϸб�壨Ӣ�ģ�\n!font Roboto-ThinItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRobotoCondensed���棨Ӣ�ģ�\n!font RobotoCondensed-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRobotoCondensed���壨Ӣ�ģ�\n!font RobotoCondensed-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRobotoCondensed��б�壨Ӣ�ģ�\n!font RobotoCondensed-BoldItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRobotoCondensedϸ�壨Ӣ�ģ�\n!font RobotoCondensed-Light.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪRRobotoCondensedϸб�壨Ӣ�ģ�\n!font RobotoCondensed-LightItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪNotoSerif��б�壨Ӣ�ģ�\n!font NotoSerif-BoldItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪNotoSerif���壨Ӣ�ģ�\n!font NotoSerif-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪNotoSerifб�壨Ӣ�ģ�\n!font NotoSerif-Italic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l ��������ΪNotoSerif���棨Ӣ�ģ�\n!font NotoSerif-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l �ָ�Ĭ������\n*line\n*feedline 1\n",
					};
	#endif
	//TTF��ӡ�ű�����ֻ֧��UTF-8��ʽ����sourceingsightĬ��GB2312��ʽ���ʴ��ļ���������sourceinsight����ʾΪ���룬��λ����ʱ�ɲ鿴��Ӧ��GB2312��
	char *ttf_str[] = {
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为系统默认英文字体\n!font DroidSans.ttf\n!NLFONT 9 12 3\n*text l DroidSansDroidSans\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为系统默认英文粗字体\n!font DroidSans-Bold.ttf\n!NLFONT 9 12 3\n*text l DroidSansDroidSans\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为系统默认中英文字体\n!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 新大陆支付技术有限公司newlandpatment\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为DroidaSansMono字体\n!font DroidSansMono.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为DancingScript粗字体\n!font DancingScript-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为DancingScript常规字体\n!font DancingScript-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为Roboto常规（英文）\n!font Roboto-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为Roboto粗体（英文）\n!font Roboto-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为Roboto粗斜体（英文）\n!font Roboto-BoldItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",						
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为RobotoRoboto斜体（英文）\n!font Roboto-Italic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为Roboto细字体（英文）\n!font Roboto-Thin.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为Roboto细斜体（英文）\n!font Roboto-ThinItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为RobotoCondensed常规（英文）\n!font RobotoCondensed-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为RobotoCondensed粗体（英文）\n!font RobotoCondensed-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为RobotoCondensed粗斜体（英文）\n!font RobotoCondensed-BoldItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为RobotoCondensed细体（英文）\n!font RobotoCondensed-Light.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为RRobotoCondensed细斜体（英文）\n!font RobotoCondensed-LightItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为NotoSerif粗斜体（英文）\n!font NotoSerif-BoldItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为NotoSerif粗体（英文）\n!font NotoSerif-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为NotoSerif斜体（英文）\n!font NotoSerif-Italic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 以下设置为NotoSerif常规（英文）\n!font NotoSerif-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 恢复默认字体\n*line\n*feedline 1\n",
					};
	char *pngbuf[] = {"ysz1", "ysz2", "ysz3", "ysz4", "ysz5", "ysz6", "ysz7", "color1", "color2", "color3", "color4", "color5", "color6",
					"IHDR1", "IHDR2", "IHDR3", "IHDR4", "IHDR5", "IHDR6", "IHDR7", "IHDR8", "IHDR9", "IHDR10", "IHDR11", "IHDR12", "IHDR13",
					"IHDR14", "IHDR15", "other1", "other2", "other3", "other4", "other5", "other6", "other7", "other8", "other9", "other10",
					"other11", "other12", "other13", "other14", "other15", "other16"};
	char *whbuf[] = {"284*52", "268*100", "292*52", "309*72", "211*93", "309*72", "302*112", "384*300", "384*300", "384*300", "384*300", "384*300", "384*300", "280*280", "310*136", "310*136", "315*164", 
					 "384*75", "384*241", "195*384", "384*144", "294*292", "300*191", "100*54", "300*200", "384*192", "315*164", "220*183", "315*164", "384*149", "327*87", "315*164", "220*165", "240*240",
					 "250*250", "99*99", "384*49", "328*112", "310*136", "205*86", "361*361", "361*361", "384*259", "384*49"};  //��pngͼƬ��Ӧ�Ŀ�͸�
	#if 0 //buf_str��GB2312��ʽ���������ⶨλ
	char *buf_str[] = {
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�120����)��ɨ���ӡ���������������Ƿ�Ϊ:0123456789ABC\n!barcode 2 120\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",//�����������Ե�һ�ε��ýű���ӡֱ�Ӵ�ӡ����,�м䲻Ӧ���пհ���
						"!NLFONT 9 12\n*text c ���´�ӡ������(���8�߶�384����)��ɨ���ӡ���������������Ƿ�Ϊ:0123456789ABC\n!barcode 8 384\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",//���������ȳ���384,����ʧ��
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�384����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 384 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",//�������Զ�ά���ȳ���384,����ʧ��
						//ttf����:ʹ��WellnerAdaptiveThreshold��ֵ�㷨 yzΪ0
						"!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ����ֵʹ��WellnerAdaptiveThreshold�㷨(����)��Ч�������ʵ��ͼƬ����ɫͼƬǳ��ɫ����ӡ�ɰ�ɫ������ɫ����ӡ�ɺ�ɫ\n*image l 384*300 path:yz:0;/mnt/sdcard/picture/color1.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ����ֵʹ��WellnerAdaptiveThreshold�㷨(����)��Ч�������ʵ��ͼƬ����ɫͼƬǳ��ɫ����ӡ�ɰ�ɫ������ɫ����ӡ�ɺ�ɫ\n*image c 394*300 path:yz:0;/mnt/sdcard/picture/color2.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ����ֵʹ��WellnerAdaptiveThreshold�㷨(����)��Ч�������ʵ��ͼƬ����ɫͼƬǳ��ɫ����ӡ�ɰ�ɫ������ɫ����ӡ�ɺ�ɫ\n*image r 384*300 path:yz:0;/mnt/sdcard/picture/color3.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ,��ֵ255��ӡ�ڿ�(����)\n*image r 384*240 path:yz:255;/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n",  //������������pngͼƬ����ʹ��png02��png03�滻ԭ����logoͼƬ
						"!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ,��ֵĬ��(����),ע���ӡЧ��\n*image c 384*49 path:/mnt/sdcard/picture/png08.png\n*line\n*feedline 1\n",//������ӡͼƬ
						"!NLFONT 9 12\n*text c ���´�ӡѹ��PNGͼƬ,��ֵ�Զ���(100*200������)�����ع�ע��ӡЧ��\n*image l 100*200 path:/mnt/sdcard/picture/png02.png\n*line\n*feedline 1\n", 
						"!NLFONT 9 12\n*text c ���´�ӡѹ��PNGͼƬ,��ֵ�Զ���(100*200������)�����ع�ע��ӡЧ��\n*image c 100*200 path:/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n", 
						"!NLFONT 9 12\n*text c ���´�ӡѹ��PNGͼƬ,��ֵ�Զ���(100*200������)�����ع�ע��ӡЧ��\n*image r 100*200 path:/mnt/sdcard/picture/png04.png\n*line\n*feedline 1\n", //������ӡѹ����PNGͼƬ
						"!NLFONT 9 12\n*text c ���´�ӡѹ��PNGͼƬ,WellnerAdaptiveThreshold�㷨��ֵ�Զ�,(384*144,����),���ع�ע��ӡЧ��\n*image r 384*144 path:yz:0;/mnt/sdcard/picture/IHDR8.png\n*feedline 1\n",
						//������ά��֧��400����
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�100���У�����400)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼\
						���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼\
						���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼\n!qrcode 100 2\n*qrcode c ���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼\
						���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼���¼�¼\n*line\n*feedline 1\n",
						//��ӡ����:hz���underline����
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС����������ʽ�����������»���\n!hz s\n!asc l\n!gray 5\n!yspace 6\n*underline l aBc34��������aBc34\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ��׼����������ʽ��׼������д��»���\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*underline c aBc1234������������������������������������������������������������aBc1234\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ������������ʽС������Ҵ��»���\n!hz l\n!asc s\n!gray 5\n!yspace 6\n*underline r aBc1234������������������������������������������������������������aBc1234\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ������������ʽС����ƫ��16���»���\n!hz l\n!asc s\n!gray 5\n!yspace 6\n*underline x:16 aBc1234������������������������������������������������������������aBc1234\n*line\n*feedline 1\n",//����ƫ������ӡx:16
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС����,ÿ��24��,����\n!hz s\n!gray 5\n!yspace 6\n*text l ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС����,ÿ��24��,ƫ��120����\n!hz s\n!gray 5\n!yspace 6\n*text x:120 ������������������������������������������������������������\n*line\n*feedline 1\n",//����ƫ������ӡx:120
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ��׼����,ÿ��16��,����\n!hz n\n!gray 5\n!yspace 6\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ������,ÿ��12��,����\n!hz l\n!gray 5\n!yspace 6\n*text r ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС�����ȱ�׼����߶�,����\n!hz sn\n!gray 5\n!yspace 6\n*text l ������������������������������������������������������������\n*line\n*feedline 1\n",		
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС�����ȴ�����߶�,����\n!hz sl\n!gray 5\n!yspace 6\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ��׼�����ȴ�����߶�,����\n!hz nl\n!gray 5\n!yspace 6\n*text r ������������������������������������������������������������\n*line\n*feedline 1\n",
						//��ӡӢ��:asc����
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС����,ÿ��48��,����\n!asc s\n!gray 5\n!yspace 6\n*text l sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ��׼����,ÿ��32��,����\n!asc n\n!gray 5\n!yspace 6\n*text c sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ������,ÿ��24��,����\n!asc l\n!gray 5\n!yspace 6\n*text r sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС�����ȱ�׼����߶�,����\n!asc sn\n!gray 5\n!yspace 6\n*text l sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС�����ȴ�����߶�,����\n!asc sl\n!gray 5\n!yspace 6\n*text c sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ��׼�����ȴ�����߶�,����\n!asc nl\n!gray 5\n!yspace 6\n*text r sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						//�Ҷ�:gray����
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵΪ1\n!hz n\n!gray 1\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵΪ3\n!hz n\n!gray 3\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵΪ7\n!hz n\n!gray 7\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵΪ10\n!hz n\n!gray 10\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵΪ5\n!hz n\n!gray 5\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",//�Ҷ�ֵ5�ź�����Ϊ�˻ָ�Ĭ��ֵ5
						//�м��:yspace����
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ0\n!hz n\n!yspace 0\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ10\n!hz n\n!yspace 10\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ20\n!hz n\n!yspace 20\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ30\n!hz n\n!yspace 30\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ40\n!hz n\n!yspace 40\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ50\n!hz n\n!yspace 50\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ60\n!hz n\n!yspace 60\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ6\n!hz n\n!yspace 6\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",//�ָ���Ĭ��ֵ�м��6
						//������:!barcode�����*barcode����
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�120����)��ɨ���ӡ���������������Ƿ�Ϊ:0123456789ABC\n!barcode 2 120\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���4�߶�128����)��ɨ���ӡ���������������Ƿ�Ϊ:123456\n!barcode 4 128\n*barcode c 123456\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���4�߶�128ƫ��50)��ɨ���ӡ���������������Ƿ�Ϊ:123456\n!barcode 4 128\n*barcode x:50 123456\n*line\n*feedline 1\n",//����ƫ������ӡx:50
						"!NLFONT 9 12\n*text c ���´�ӡ������(���8�߶�320����)��ɨ���ӡ���������������Ƿ�Ϊ:123\n!barcode 8 320\n*barcode l 123\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���1�߶�64����,���ַ�31��),��Ҫ���ܹ�ɨ�����\n!barcode 1 64\n*barcode c AAAAAAAAAABBBBBBBBBBCCCCCCCCCCD\n*line\n*feedline 1\n",//��������ɨ����ַ����ı߽����,���Ϊ1:���ַ�31��,������60,���Ϊ2:���ַ�14��,������28��
						"!NLFONT 9 12\n*text c ���´�ӡ������(���1�߶�64����,������60��),��Ҫ���ܹ�ɨ�����\n!barcode 1 64\n*barcode c 012345678901234567890123456789012345678901234567890123456789\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,������28��)��ɨ���ӡ���������������Ƿ�Ϊ:0123456789012345678901234567\n!barcode 2 64\n*barcode c 0123456789012345678901234567\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,����ĸ14��)��ɨ���ӡ���������������Ƿ�Ϊ:AAAAAAAAAABBBB\n!barcode 2 64\n*barcode c AAAAAAAAAABBBB\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,��ĸ���������)��ɨ���ӡ���������������Ƿ�Ϊ:01AB23BC34C\n!barcode 2 64\n*barcode c 01AB23BC34C\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,��ĸ���������)��ɨ���ӡ���������������Ƿ�Ϊ:A1B2C3D4E5F6G7\n!barcode 2 64\n*barcode c A1B2C3D4E5F6G7\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,��ĸ���������)��ɨ���ӡ���������������Ƿ�Ϊ:ABC123CDE4567\n!barcode 2 64\n*barcode c ABC123CDE4567\n*line\n*feedline 1\n",							
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,������32��(����28,��Ȼ��Զ���Ϊ1)),��Ҫ���ܹ�ɨ�����\n!barcode 2 64\n*barcode c 01234567890123456789012345678912\n*line\n*feedline 1\n",//�ָ�Ĭ�Ͽ��2,�߶�64(����28��,��Ȼ��Զ���Ϊ1)
						//��ά��:!qrcode�����*qrcode����
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�384����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 384 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�10����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 10 0\n*qrcode l ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�50����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 50 1\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�50ƫ��50)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 50 1\n*qrcode x:50 ABC123456789DEFGH\n*line\n*feedline 1\n",//����ƫ������ӡx:50
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�200����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 200 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�100����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 100 2\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�300����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:400�ֽڵ�A\n!qrcode 300 1\n*qrcode c AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�300����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:401�ֽڵ�A\n!qrcode 300 1\n*qrcode c AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n*line\n*feedline 1\n",
						//��ӡͼƬ
						//"!NLFONT 9 12\n*text c ���´�ӡͼƬ(����)\n*image l 32*64 #ums\n*line\n*feedline 1\n",
						//"!NLFONT 9 12\n*text c ���´�ӡͼƬ(����)\n*image c 64*128 #ums\n*line\n*feedline 1\n",
						//"!NLFONT 9 12\n*text c ���´�ӡͼƬ(����)\n*image r 128*64 #ums\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c ���´�ӡͼƬ(����)\n*image r 128*64 #logo\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c ���´�ӡͼƬ(����)\n*image c 128*64 data:base64:ROIGODIhuA\n*line\n*feedline 1\n",
						//��ӡ�ָ���
						"!hz n\n!asc n\n!gray 7\n!yspace 6\n*text c ���´�ӡ�����ָ���\n*line\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c ���´�ӡ�����ָ���\n*line\n*feedline p 1\n", 
						//NLFONT����
						"!NLFONT 9 12 0\n*text l �����ӡ������Ϊ��������24x24,����ΪGulimche����12x24,����������Ŵ�2��\n*line\n*feedline 1\n",
						"!NLFONT 10 13 1\n*text l �����ӡ������Ϊ����24x24,����ΪMSGothic����16*32,����Ŵ�2����������\n*line\n*feedline 1\n",
						"!NLFONT 11 17 2\n*text l �����ӡ������Ϊ����24*24,����Ϊ�������12x16,������������Ŵ�2��\n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text l �����ӡ������Ϊ����12x12,����Ϊ�������16x32,�������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12 0\n*text c ���д�ӡ������Ϊ��������24x24,����ΪGulimche����12x24,����������Ŵ�2��\n*line\n*feedline 1\n",
						"!NLFONT 10 13 1\n*text c ���д�ӡ������Ϊ����24x24,����ΪMSGothic����16*32,����2���Ŵ���������\n*line\n*feedline 1\n",
						"!NLFONT 11 17 2\n*text c ���д�ӡ������Ϊ����24*24,����Ϊ�������12x16,������������2���Ŵ�\n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text c ���д�ӡ������Ϊ����12x12,����Ϊ�������16x32,�������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12 4\n*text r ���Ҵ�ӡ������Ϊ��������24x24,����ΪGulimche����12x24,����3���Ŵ�����3���Ŵ�\n*line\n*feedline 1\n",
						"!NLFONT 10 13 5\n*text r ���Ҵ�ӡ������Ϊ����24x24,����ΪMSGothic����16*32,����3���Ŵ���������\n*line\n*feedline 1\n",
						"!NLFONT 11 17 6\n*text r ���Ҵ�ӡ������Ϊ����24*24,����Ϊ�������12x16,������������3���Ŵ�\n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text r ���Ҵ�ӡ������Ϊ����12x12,����Ϊ�������16x32,�������������\n*line\n*feedline 1\n",
						//��ͣʱ��:pause���NLPRNOVER����						
						"!NLFONT 9 12\n*text l ���´�ӡͼƬ������,����ͣ5s���ӡ\n!NLPRNOVER\n*pause 5\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image c 128*64 #ums\n*text c ��������-ȫ�񸶽���ƾ��\n!hz l\n!asc l\n*text c (������)�ͷ��绰:95534\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text l ��ƫ��256��ӡͼƬ\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:256 128*64 #ums\n*line\n*feedline 1\n",//����ƫ��x:256��ӡ128��ȵ�ͼƬ
						//��ֽ
						"!NLFONT 9 12\n*text l ����ֽ���Ե�һ������\n*feedline 1\n!gray 5\n!yspace 6\n*text c �ڶ�������.��ӡ������û���໥����\n*line\n*feedline 1\n",//*feedline -1\n
						"!gray 5\n!yspace 6\n*text c ��������Ӧ��ȫ��ֽ����\n*feedline 5\n",//!NLPRNOVER\n	
						//������
	                    			//ttf����:!BARCODE�����*BARCODE���֧�ֶ��ֱ��룬ǰ2��������!barcodeһ����������������ʾ�Ƿ���ʾ�·���ʾ��Ϣ�����ĸ�������ʾ�����������
	                    			//0 caodebar; 1 code39; 2 code93; 3 code128; 4 EAN-8/EAN-13/JAN-8/JAN-13; 5 ITF; 6 UPCA; 7 UPCE
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����Codebar�����������Ƿ�Ϊ:0123456789�����������·����С���ʾ������Ϣ\n!BARCODE 6 120 1 0\n*BARCODE c A0123456789C\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����Code39�����������Ƿ�Ϊ:ABCDEFGHIJKLMNOPQRSTUVWXYZ -.$/+%0123456789�����������·����С���ʾ������Ϣ\n!BARCODE 6 120 1 1\n*BARCODE c ABCDEFGHIJKLMNOPQRSTUVWXYZ -.$/+%0123456789\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����Code93�����������Ƿ�Ϊ:CD 45%+67$/�����������·����С���ʾ������Ϣ\n!BARCODE 6 120 1 2\n*BARCODE c CD 45%+67$\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����Code128�����������Ƿ�Ϊ:A0123456C%$#@�����������·����С���ʾ������Ϣ\n!BARCODE 6 120 1 3\n*BARCODE c A0123456C%$#@\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����EAN-8�����������Ƿ�Ϊ:12345670�����������·����ޡ���ʾ������Ϣ\n!BARCODE 6 120 0 4\n*BARCODE c 1234567\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����EAN-13�����������Ƿ�Ϊ:1234567890128�����������·����ޡ���ʾ������Ϣ\n!BARCODE 6 120 0 4\n*BARCODE c 123456789012\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����EAN-8�����������Ƿ�Ϊ:00001236�����������·����С���ʾ������Ϣ\n!BARCODE 6 120 1 4\n*BARCODE c 123\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����EAN-13�����������Ƿ�Ϊ:0000123456784�����������·����С���ʾ������Ϣ\n!BARCODE 6 120 1 4\n*BARCODE c 12345678\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����ITF�����������Ƿ�Ϊ:01234567890128�����������·����ޡ���ʾ������Ϣ\n!BARCODE 6 120 0 5\n*BARCODE c 0123456789012\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����ITF�����������Ƿ�Ϊ:00123456789050�����������·����С���ʾ������Ϣ\n!BARCODE 6 120 1 5\n*BARCODE c 12345678905\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����UPC-A�����������Ƿ�Ϊ:123456789012�����������·����ޡ���ʾ������Ϣ\n!BARCODE 6 120 0 6\n*BARCODE c 12345678901\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����UPC-A�����������Ƿ�Ϊ:000765432108�����������·����С���ʾ������Ϣ\n!BARCODE 6 120 1 6\n*BARCODE c 76543210\n*line\n*feedline 1\n",							
						"!NLFONT 9 12\n*text c ���´�ӡ������(���6�߶�120����)��ɨ���ӡ����UPC-E�����������Ƿ�Ϊ:01234565�����������·����ޡ���ʾ������Ϣ\n!BARCODE 6 120 0 7\n*BARCODE c 0123456\n*line\n*feedline 1\n",
						//��ά��
						//ttf����:!QRCODE�����*QRCODE���֧�ֶ��ֱ��룬ǰ2��������!qrcodeһ�����ڶ�����������QRCODE��Ч��������������ʾ��ά���������
						//0 Data Matrix;  1 Matrixcode; 2 PDF417; 3 QRCODE
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�200����,���뷽ʽData Matrix)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:Aa12%#@*&�����è�����\n!QRCODE 200 0 0\n*QRCODE c Aa12%#@*&�����è�����\n*line\n*feedline 1\n",//�����Խϴ�߶ȵĶ�ά�뿪ʼ�� �ٴ�ӡ�߶�С��
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�200����,���뷽ʽMaxicode)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC~*&%#!56789DEFGH\n!QRCODE 200 0 1\n*QRCODE c ABC~*&%#!56789DEFGH\n*line\n*feedline 1\n",//�����Խϴ�߶ȵĶ�ά�뿪ʼ�� �ٴ�ӡ�߶�С��
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�200����,���뷽ʽPDF417)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:PDF����֧�����Ľϲ�\n!QRCODE 200 0 2\n*QRCODE c PDF����֧�����Ľϲ�\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�200����,���뷽ʽQR Code)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:QRCODE����֧�����ĺ�\n!QRCODE 200 2 3\n*QRCODE c QRCODE����֧�����ĺ�\n*line\n*feedline 1\n",
						//ttf����:*TEXTָ��÷���*textָ��һ����������*text�ỻ�У�*TEXT���ỻ��
						//ttf����:*UNDERLINEָ��÷���*underlineָ��һ����������*underline�ỻ�У�*UNDERLINE���ỻ��
						//"!NLFONT 2 1 3\n*TEXT l ��������Test\n!NLFONT 9 12 3\n*text r ���־��Ҳ��ұ��в�����Test\n",
						"!NLFONT 2 1 3\n*TEXT l ��������Test\n!NLFONT 9 12 3\n*text r ������Test\n"
						"!NLFONT 9 12 3\n*UNDERLINE l �������ִ��»���\n*underline r ���Ҳ�����Test\n",
						"!NLFONT 9 12 3\n*TEXT l ���left\n!NLFONT 2 1 3\n*UNDERLINE c �»���center\n!NLFONT 6 4 3\n*text r �ұ�right\n",
						//Ӣ�ĺ���Ŵ�����Ŵ�֮ǰ����һ��Ӣ����ĸ��ֿ�2�ߵ�����
						"!NLFONT 9 12 0\n*text r ���Ҵ�ӡ����,����Ŵ�����Ŵ�,������������������������������������\n*line\n",
						"!NLFONT 9 12 0\n*text r To print text,transverse magnification,vertical magnification\n*line\n",
					  };
	#endif
	char *buf_str[] = {
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度120居右)请扫描打印出的条形码内容是否为:0123456789ABC\n!barcode 2 120\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",//�����������Ե�һ�ε��ýű���ӡֱ�Ӵ�ӡ����,�м䲻Ӧ���пհ���
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度8高度384居右)请扫描打印出的条形码内容是否为:0123456789ABC\n!barcode 8 384\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度384居右)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 384 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						//ttf����:ʹ��WellnerAdaptiveThreshold��ֵ�㷨 yzΪ0
						"!NLFONT 9 12\n*text c 以下打印PNG图片，阈值使用WellnerAdaptiveThreshold算法(居左)，效果请对照实际图片，彩色图片浅颜色将打印成白色，深颜色将打印成黑色\n*image l 384*300 path:yz:0;/mnt/sdcard/picture/color1.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印PNG图片，阈值使用WellnerAdaptiveThreshold算法(居中)，效果请对照实际图片，彩色图片浅颜色将打印成白色，深颜色将打印成黑色\n*image c 394*300 path:yz:0;/mnt/sdcard/picture/color2.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印PNG图片，阈值使用WellnerAdaptiveThreshold算法(居右)，效果请对照实际图片，彩色图片浅颜色将打印成白色，深颜色将打印成黑色\n*image r 384*300 path:yz:0;/mnt/sdcard/picture/color3.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印PNG图片,阈值255打印黑块(居右)\n*image r 384*240 path:yz:255;/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n",  //新增用例测试png图片功能使用png02和png03替换原来的logo图片
						"!NLFONT 9 12\n*text c 以下打印PNG图片,阈值默认(居中),注意打印效果\n*image c 384*49 path:/mnt/sdcard/picture/png08.png\n*line\n*feedline 1\n",//新增打印图片
						"!NLFONT 9 12\n*text c 以下打印压缩PNG图片,阈值自动，(100*200，居左)，不必关注打印效果\n*image l 100*200 path:/mnt/sdcard/picture/png02.png\n*line\n*feedline 1\n", 
						"!NLFONT 9 12\n*text c 以下打印压缩PNG图片,阈值自动，(100*200，居中)，不必关注打印效果\n*image c 100*200 path:/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n", 
						"!NLFONT 9 12\n*text c 以下打印压缩PNG图片,阈值自动，(100*200，居右)，不必关注打印效果\n*image r 100*200 path:/mnt/sdcard/picture/png04.png\n*line\n*feedline 1\n", //新增打印压缩的PNG图片
						"!NLFONT 9 12\n*text c 以下打印压缩PNG图片,WellnerAdaptiveThreshold算法阈值自动,(384*144,居右),不必关注打印效果\n*image r 384*144 path:yz:0;/mnt/sdcard/picture/IHDR8.png\n*feedline 1\n",
						//������ά��֧��400����
						"!NLFONT 9 12\n*text c 以下打印二维码(高度100居中，内容400)请扫描打印出的二维码内容是否为:更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录\
						更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录\
						更新记录更新记录更新记录更新记录更新记录\n!qrcode 100 2\n*qrcode c 更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录\
						更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录\n*line\n*feedline 1\n",
						//��ӡ����:hz���underline����
						"!NLFONT 9 12\n*text c 以下打印汉字样式小字体西方样式大字体居左带下划线\n!hz s\n!asc l\n!gray 5\n!yspace 6\n*underline l aBc34国国国国aBc34\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式标准字体西方样式标准字体居中带下划线\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*underline c aBc1234国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国aBc1234\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式大字体西方样式小字体居右带下划线\n!hz l\n!asc s\n!gray 5\n!yspace 6\n*underline r aBc1234国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国aBc1234\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式大字体西方样式小字体偏移16带下划线\n!hz l\n!asc s\n!gray 5\n!yspace 6\n*underline x:16 aBc1234国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国aBc1234\n*line\n*feedline 1\n",//ƫ������ӡx:16
						"!NLFONT 9 12\n*text c 以下打印汉字样式小字体,每行24个,居左\n!hz s\n!gray 5\n!yspace 6\n*text l 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式小字体,每行24个,偏移120像素\n!hz s\n!gray 5\n!yspace 6\n*text x:120 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",//ƫ������ӡx:120
						"!NLFONT 9 12\n*text c 以下打印汉字样式标准字体,每行16个,居中\n!hz n\n!gray 5\n!yspace 6\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式大字体,每行12个,居右\n!hz l\n!gray 5\n!yspace 6\n*text r 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式小字体宽度标准字体高度,居左\n!hz sn\n!gray 5\n!yspace 6\n*text l 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",		
						"!NLFONT 9 12\n*text c 以下打印汉字样式小字体宽度大字体高度,居中\n!hz sl\n!gray 5\n!yspace 6\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式标准字体宽度大字体高度,居右\n!hz nl\n!gray 5\n!yspace 6\n*text r 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						//��ӡӢ��:asc����
						"!NLFONT 9 12\n*text c 以下打印西方样式小字体,每行48个,居左\n!asc s\n!gray 5\n!yspace 6\n*text l sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印西方样式标准字体,每行32个,居中\n!asc n\n!gray 5\n!yspace 6\n*text c sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印西方样式大字体,每行24个,居右\n!asc l\n!gray 5\n!yspace 6\n*text r sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印西方样式小字体宽度标准字体高度,居左\n!asc sn\n!gray 5\n!yspace 6\n*text l sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印西方样式小字体宽度大字体高度,居中\n!asc sl\n!gray 5\n!yspace 6\n*text c sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印西方样式标准字体宽度大字体高度,居右\n!asc nl\n!gray 5\n!yspace 6\n*text r sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						//�Ҷ�:gray����
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值为1\n!hz n\n!gray 1\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值为3\n!hz n\n!gray 3\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值为7\n!hz n\n!gray 7\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值为10\n!hz n\n!gray 10\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值为5\n!hz n\n!gray 5\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",//�Ҷ�ֵ5�ź�����Ϊ�˻ָ�Ĭ��ֵ5
						//�м��:yspace����
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为0\n!hz n\n!yspace 0\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为10\n!hz n\n!yspace 10\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为20\n!hz n\n!yspace 20\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为30\n!hz n\n!yspace 30\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为40\n!hz n\n!yspace 40\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为50\n!hz n\n!yspace 50\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为60\n!hz n\n!yspace 60\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为6\n!hz n\n!yspace 6\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",//�ָ���Ĭ��ֵ�м��6
						//������:!barcode�����*barcode����
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度120居右)请扫描打印出的条形码内容是否为:0123456789ABC\n!barcode 2 120\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度128居中)请扫描打印出的条形码内容是否为:123456\n!barcode 4 128\n*barcode c 123456\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度128偏移50)请扫描打印出的条形码内容是否为:123456\n!barcode 4 128\n*barcode x:50 123456\n*line\n*feedline 1\n",//ƫ������ӡx:50
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度8高度320居左)请扫描打印出的条形码内容是否为:123\n!barcode 8 320\n*barcode l 123\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度1高度64居中,纯字符31个),不要求能够扫描出来\n!barcode 1 64\n*barcode c AAAAAAAAAABBBBBBBBBBCCCCCCCCCCD\n*line\n*feedline 1\n",//��������ɨ����ַ����ı߽����,���Ϊ1:���ַ�31��,������60,���Ϊ2:���ַ�14��,������28��
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度1高度64居中,纯数字60个),不要求能够扫描出来\n!barcode 1 64\n*barcode c 012345678901234567890123456789012345678901234567890123456789\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,纯数字28个)请扫描打印出的条形码内容是否为:0123456789012345678901234567\n!barcode 2 64\n*barcode c 0123456789012345678901234567\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,纯字母14个)请扫描打印出的条形码内容是否为:AAAAAAAAAABBBB\n!barcode 2 64\n*barcode c AAAAAAAAAABBBB\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,字母与数字组合)请扫描打印出的条形码内容是否为:01AB23BC34C\n!barcode 2 64\n*barcode c 01AB23BC34C\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,字母与数字组合)请扫描打印出的条形码内容是否为:A1B2C3D4E5F6G7\n!barcode 2 64\n*barcode c A1B2C3D4E5F6G7\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,字母与数字组合)请扫描打印出的条形码内容是否为:ABC123CDE4567\n!barcode 2 64\n*barcode c ABC123CDE4567\n*line\n*feedline 1\n",							
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,纯数字32个(超过28,宽度会自动降为1)),不要求能够扫描出来\n!barcode 2 64\n*barcode c 01234567890123456789012345678912\n*line\n*feedline 1\n",//�ָ�Ĭ�Ͽ��2,�߶�64(����28��,��Ȼ��Զ���Ϊ1)
						//��ά��:!qrcode�����*qrcode����
						"!NLFONT 9 12\n*text c 以下打印二维码(高度384居右)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 384 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度10居左)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 10 0\n*qrcode l ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度50居中)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 50 1\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度50偏移50)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 50 1\n*qrcode x:50 ABC123456789DEFGH\n*line\n*feedline 1\n",//����ƫ������ӡx:50
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居右)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 200 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度100居中)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 100 2\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度300居中)请扫描打印出的二维码内容是否为:400字节的A\n!qrcode 300 1\n*qrcode c AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度300居中)请扫描打印出的二维码内容是否为:401字节的A\n!qrcode 300 1\n*qrcode c AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n*line\n*feedline 1\n",
						//��ӡͼƬ, ����Ҳ��֪������ʲô��ʽ��ͼƬ����ʵ��û���õ�������ͼƬ������
						//"!NLFONT 9 12\n*text c 以下打印图片(居左)\n*image l 32*64 #ums\n*line\n*feedline 1\n",
						//"!NLFONT 9 12\n*text c 以下打印图片(居中)\n*image c 64*128 #ums\n*line\n*feedline 1\n",
						//"!NLFONT 9 12\n*text c 以下打印图片(居右)\n*image r 128*64 #ums\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c 以下打印图片(居右)\n*image r 128*64 #logo\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c 以下打印图片(居中)\n*image c 128*64 data:base64:ROIGODIhuA\n*line\n*feedline 1\n",
						//��ӡ�ָ���
						"!hz n\n!asc n\n!gray 7\n!yspace 6\n*text c 以下打印两条分隔符\n*line\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c 以下打印两条分隔符\n*line\n*feedline p 1\n", 
						//NLFONT����
						"!NLFONT 9 12 0\n*text l 靠左打印的中文为自有宋体24x24,西文为Gulimche字体12x24,横向纵向均放大2倍\n*line\n*feedline 1\n",
						"!NLFONT 10 13 1\n*text l 靠左打印的中文为仿宋24x24,西文为MSGothic粗体16*32,横向放大2倍纵向正常\n*line\n*feedline 1\n",
						"!NLFONT 11 17 2\n*text l 靠左打印的中文为楷体24*24,西文为楷体粗体12x16,横向正常纵向放大2倍\n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text l 靠左打印的中文为宋体12x12,西文为楷体粗体16x32,横向纵向均正常\n*line\n*feedline 1\n",
						"!NLFONT 9 12 0\n*text c 居中打印的中文为自有宋体24x24,西文为Gulimche字体12x24,横向纵向均放大2倍\n*line\n*feedline 1\n",
						"!NLFONT 10 13 1\n*text c 居中打印的中文为仿宋24x24,西文为MSGothic粗体16*32,横向2倍放大纵向正常\n*line\n*feedline 1\n",
						"!NLFONT 11 17 2\n*text c 居中打印的中文为楷体24*24,西文为楷体粗体12x16,横向正常纵向2倍放大\n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text c 居中打印的中文为宋体12x12,西文为楷体粗体16x32,横向纵向均正常\n*line\n*feedline 1\n",
						"!NLFONT 9 12 4\n*text r 靠右打印的中文为自有宋体24x24,西文为Gulimche字体12x24,横向3倍放大纵向3倍放大\n*line\n*feedline 1\n",
						"!NLFONT 10 13 5\n*text r 靠右打印的中文为仿宋24x24,西文为MSGothic粗体16*32,横向3倍放大纵向正常\n*line\n*feedline 1\n",
						"!NLFONT 11 17 6\n*text r 靠右打印的中文为楷体24*24,西文为楷体粗体12x16,横向正常纵向3倍放大\n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text r 靠右打印的中文为宋体12x12,西文为楷体粗体16x32,横向纵向均正常\n*line\n*feedline 1\n",
						//��ͣʱ��:pause���NLPRNOVER����					
						"!NLFONT 9 12\n*text l 以下打印图片及文字,将暂停5s后打印\n!NLPRNOVER\n*pause 5\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image c 128*64 #ums\n*text c 银联商务-全民付交易凭条\n!hz l\n!asc l\n*text c (大字体)客服电话:95534\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text l 以偏移256打印图片\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:256 128*64 #ums\n*line\n*feedline 1\n",//新增偏移x:256打印128款度的图片
						//��ֽ
						"!NLFONT 9 12\n*text l 进退纸测试第一行文字\n*feedline 1\n!gray 5\n!yspace 6\n*text c 第二行文字.打印的文字没有相互覆盖\n*line\n*feedline 1\n",//*feedline -1\n
						"!gray 5\n!yspace 6\n*text c 本行文字应完全在纸槽外\n*feedline 5\n",//!NLPRNOVER\n	
						//������
						//ttf����:!BARCODE�����*BARCODE���֧�ֶ��ֱ��룬ǰ2��������!barcodeһ����������������ʾ�Ƿ���ʾ�·���ʾ��Ϣ�����ĸ�������ʾ�����������
	                    			//0 caodebar; 1 code39; 2 code93; 3 code128; 4 EAN-8/EAN-13/JAN-8/JAN-13; 5 ITF; 6 UPCA; 7 UPCE
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的Codebar条形码内容是否为:0123456789，并且条码下方【有】显示条码信息\n!BARCODE 6 120 1 0\n*BARCODE c A0123456789C\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的Code39条形码内容是否为:ABCDEFGHIJKLMNOPQRSTUVWXYZ -.$/+%0123456789，并且条码下方【有】显示条码信息\n!BARCODE 6 120 1 1\n*BARCODE c ABCDEFGHIJKLMNOPQRSTUVWXYZ -.$/+%0123456789\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的Code93条形码内容是否为:CD 45%+67$/，并且条码下方【有】显示条码信息\n!BARCODE 6 120 1 2\n*BARCODE c CD 45%+67$\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的Code128条形码内容是否为:A0123456C%$#@，并且条码下方【有】显示条码信息\n!BARCODE 6 120 1 3\n*BARCODE c A0123456C%$#@\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的EAN-8条形码内容是否为:12345670，并且条码下方【无】显示条码信息\n!BARCODE 6 120 0 4\n*BARCODE c 1234567\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的EAN-13条形码内容是否为:1234567890128，并且条码下方【无】显示条码信息\n!BARCODE 6 120 0 4\n*BARCODE c 123456789012\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的EAN-8条形码内容是否为:00001236，并且条码下方【有】显示条码信息\n!BARCODE 6 120 1 4\n*BARCODE c 123\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的EAN-13条形码内容是否为:0000123456784，并且条码下方【有】显示条码信息\n!BARCODE 6 120 1 4\n*BARCODE c 12345678\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的ITF条形码内容是否为:01234567890128，并且条码下方【无】显示条码信息\n!BARCODE 6 120 0 5\n*BARCODE c 0123456789012\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的ITF条形码内容是否为:00123456789050，并且条码下方【有】显示条码信息\n!BARCODE 6 120 1 5\n*BARCODE c 12345678905\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的UPC-A条形码内容是否为:123456789012，并且条码下方【无】显示条码信息\n!BARCODE 6 120 0 6\n*BARCODE c 12345678901\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的UPC-A条形码内容是否为:000765432108，并且条码下方【有】显示条码信息\n!BARCODE 6 120 1 6\n*BARCODE c 76543210\n*line\n*feedline 1\n",							
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度6高度120居中)请扫描打印出的UPC-E条形码内容是否为:01234565，并且条码下方【无】显示条码信息\n!BARCODE 6 120 0 7\n*BARCODE c 0123456\n*line\n*feedline 1\n",
						//��ά��
						//ttf����:!QRCODE�����*QRCODE���֧�ֶ��ֱ��룬ǰ2��������!qrcodeһ�����ڶ�����������QRCODE��Ч��������������ʾ��ά���������
						//0 Data Matrix;  1 Matrixcode; 2 PDF417; 3 QRCODE
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居中,编码方式Data Matrix)请扫描打印出的二维码内容是否为:Aa12%#@*&αΒγàúǒ\n!QRCODE 200 0 0\n*QRCODE c Aa12%#@*&αΒγàúǒ\n*line\n*feedline 1\n",//�����Խϴ�߶ȵĶ�ά�뿪ʼ�� �ٴ�ӡ�߶�С��
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居中,编码方式Maxicode)请扫描打印出的二维码内容是否为:ABC~*&%#!56789DEFGH\n!QRCODE 200 0 1\n*QRCODE c ABC~*&%#!56789DEFGH\n*line\n*feedline 1\n",//�����Խϴ�߶ȵĶ�ά�뿪ʼ�� �ٴ�ӡ�߶�С��
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居中,编码方式PDF417)请扫描打印出的二维码内容是否为:PDF测试支持中文较差\n!QRCODE 200 0 2\n*QRCODE c PDF测试支持中文较差\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居中,编码方式QR Code)请扫描打印出的二维码内容是否为:QRCODE测试支持中文好\n!QRCODE 200 2 3\n*QRCODE c QRCODE测试支持中文好\n*line\n*feedline 1\n",
						//ttf����:*TEXTָ��÷���*textָ��һ����������*text�ỻ�У�*TEXT���ỻ��
						//ttf����:*UNDERLINEָ��÷���*underlineָ��һ����������*underline�ỻ�У�*UNDERLINE���ỻ��
						//"!NLFONT 2 1 3\n*TEXT l 本行文字Test\n!NLFONT 9 12 3\n*text r 文字居右并且本行不换行Test\n",
						"!NLFONT 2 1 3\n*TEXT l 本行文字Test\n!NLFONT 9 12 3\n*text r 不换行Test\n",
						"!NLFONT 9 12 3\n*UNDERLINE l 本行文字带下划线\n*underline r 并且不换行Test\n",
						"!NLFONT 9 12 3\n*TEXT l 左边left\n!NLFONT 2 1 3\n*UNDERLINE c 下划线center\n!NLFONT 6 4 3\n*text r 右边right\n",
						//Ӣ�ĺ���Ŵ�����Ŵ�֮ǰ����һ��Ӣ����ĸ��ֿ�2�ߵ�����
						"!NLFONT 9 12 0\n*text r 靠右打印文字,横向放大、纵向放大,国国国国国国国国国国国国国国国国国国\n*line\n",
						"!NLFONT 9 12 0\n*text r To print text,transverse magnification,vertical magnification\n*line\n",
					  };
	#if 0 //err_str��GB2312��ʽ���������ⶨλ
	char *err_str[] = {
						"!NLFONT 9 12\n*text c ���½����쳣����,���Դ�ӡЧ��,���ܷɼ���:��ӡ���ֻҶ�ֵ����0\n!hz n\n!gray 0\n!yspace 10\n*text l ������������������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵ����11\n!hz n\n!gray 11\n!yspace 10\n*text l ������������������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м������70\n!hz n\n!gray 5\n!yspace 70\n*text l ������������������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���0�߶�64����)\n!gray 5\n!yspace 6\n!barcode 0 64\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���9�߶�64����)\n!barcode 9 64\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���4�߶�0����)\n!barcode 4 0\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���4�߶�400����)\n!barcode 4 400\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�128����,���ø߶Ⱥ��пո�)\n!barcode 2 128 \n*barcode c 0123456789ABC\n*line\n*feedline 1\n",//���ø߶�֮���пո�
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(���������Ϊ4)\n!qrcode 100 4\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(���������Ϊ-1)\n!qrcode 100 -1\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n!NLPRNOVER\n",
						//"!NLFONT 9 12\n*text l ��ƫ��257��ӡͼƬ,Ԥ����ͼƬ,��������-1\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:257 128*64 #ums\n*line\n*feedline 1\n",//����ƫ��x:257��ӡ128��ȵ�ͼƬ������384
						//����BARCODE��QRCODE�쳣���
						"!NLFONT 9 12\n*text c ���´�ӡ������(���0�߶�64����)\n!gray 5\n!yspace 6\n!BARCODE 0 64 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���9�߶�64����)\n!BARCODE 9 64 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���4�߶�0����)\n!BARCODE 4 0 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���4�߶�328����)\n!BARCODE 4 328 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�128����,���ñ��뷽ʽ���пո�)\n!BARCODE 2 128 3 \n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",//���ø߶�֮���пո�
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(���������Ϊ4)\n!QRCODE 100 4 3\n*QRCODE c ABC123456789DEFGH\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(���������Ϊ-1)\n!QRCODE 100 -1 3\n*QRCODE c ABC123456789DEFGH\n*line\n*feedline 1\n!NLPRNOVER\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽCODEBAR,���ݸ�ʽ�����ϱ���淶)\n!BARCODE 2 64 0\n*BARCODE c 123456784\n*line\n*feedline 1\n",						
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽCODE39,���ݸ�ʽ�����ϱ���淶)\n!BARCODE 2 64 1\n*BARCODE c !#*123456784\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽCODE93,���ݸ�ʽ�����ϱ���淶)\n!BARCODE 2 64 2\n*BARCODE c !#*123456784\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽEAN-8/JAN-8,����У��λ����)\n!BARCODE 2 64 4\n*BARCODE c 34569837\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽEAN-13/JAN-13,����У��λ����)\n!BARCODE 2 64 4\n*BARCODE c 0123456789019\n*line\n*feedline 1\n",			
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽEAN/JAN,���ݳ��ȴ���)\n!BARCODE 2 64 4\n*BARCODE c 012345678901234567\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽITF,���ݳ��ȴ���)\n!BARCODE 2 64 5\n*BARCODE c 3801234567899123456\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽUPC-A,���ݳ��ȴ���)\n!BARCODE 2 64 6\n*BARCODE c 3801234567899123456\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽUPC-E,���ݳ��ȴ���)\n!BARCODE 2 64 7\n*BARCODE c 01234567890\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽUPC-E,��һλ�������ݴ���)\n!BARCODE 2 64 7\n*BARCODE c 21234569\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽ���ô���)\n!BARCODE 2 64 8\n*BARCODE c 21234569\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�64����,���뷽ʽ���ô���)\n!BARCODE 2 64 -1\n*BARCODE c 21234569\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�200����,���뷽ʽ���ô���)\n!QRCODE 200 3 4\n*QRCODE r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�200����,���뷽ʽ���ô���)\n!QRCODE 200 3 -1\n*QRCODE r ABC123456789DEFGH\n*line\n*feedline 1\n",
						
					  };
	#endif
	char *err_str[] = {
						"!NLFONT 9 12\n*text c 以下进行异常测试,忽略打印效果,不跑飞即可:打印汉字灰度值设置0\n!hz n\n!gray 0\n!yspace 10\n*text l 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值设置11\n!hz n\n!gray 11\n!yspace 10\n*text l 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距设置70\n!hz n\n!gray 5\n!yspace 70\n*text l 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度0高度64居中)\n!gray 5\n!yspace 6\n!barcode 0 64\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度9高度64居中)\n!barcode 9 64\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度0居中)\n!barcode 4 0\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度400居中)\n!barcode 4 400\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度128居中,设置高度后还有空格)\n!barcode 2 128 \n*barcode c 0123456789ABC\n*line\n*feedline 1\n",//���ø߶�֮���пո�
						"!NLFONT 9 12\n*text c 以下打印二维码(纠错级别参数为4)\n!qrcode 100 4\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c 以下打印二维码(纠错级别参数为-1)\n!qrcode 100 -1\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n!NLPRNOVER\n",
						//"!NLFONT 9 12\n*text l 以偏移257打印图片,预期无图片,函数返回-1\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:257 128*64 #ums\n*line\n*feedline 1\n",//新增偏移x:257打印128款度的图片，超过384
						//����BARCODE��QRCODE�쳣���
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度0高度64居中)\n!gray 5\n!yspace 6\n!BARCODE 0 64 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度9高度64居中)\n!BARCODE 9 64 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度0居中)\n!BARCODE 4 0 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度328居中)\n!BARCODE 4 328 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度128居中,设置编码方式后还有空格)\n!BARCODE 2 128 3 \n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",//���ø߶�֮���пո�
						"!NLFONT 9 12\n*text c 以下打印二维码(纠错级别参数为4)\n!QRCODE 100 4 3\n*QRCODE c ABC123456789DEFGH\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c 以下打印二维码(纠错级别参数为-1)\n!QRCODE 100 -1 3\n*QRCODE c ABC123456789DEFGH\n*line\n*feedline 1\n!NLPRNOVER\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式CODEBAR,数据格式不符合编码规范)\n!BARCODE 2 64 0\n*BARCODE c 123456784\n*line\n*feedline 1\n",						
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式CODE39,数据格式不符合编码规范)\n!BARCODE 2 64 1\n*BARCODE c !#*123456784\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式CODE93,数据格式不符合编码规范)\n!BARCODE 2 64 2\n*BARCODE c !#*123456784\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式EAN-8/JAN-8,数据校验位错误)\n!BARCODE 2 64 4\n*BARCODE c 34569837\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式EAN-13/JAN-13,数据校验位错误)\n!BARCODE 2 64 4\n*BARCODE c 0123456789019\n*line\n*feedline 1\n",			
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式EAN/JAN,数据长度错误)\n!BARCODE 2 64 4\n*BARCODE c 012345678901234567\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式ITF,数据长度错误)\n!BARCODE 2 64 5\n*BARCODE c 3801234567899123456\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式UPC-A,数据长度错误)\n!BARCODE 2 64 6\n*BARCODE c 3801234567899123456\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式UPC-E,数据长度错误)\n!BARCODE 2 64 7\n*BARCODE c 01234567890\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式UPC-E,第一位数据内容错误)\n!BARCODE 2 64 7\n*BARCODE c 21234569\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式设置错误)\n!BARCODE 2 64 8\n*BARCODE c 21234569\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,编码方式设置错误)\n!BARCODE 2 64 -1\n*BARCODE c 21234569\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居右,编码方式设置错误)\n!QRCODE 200 3 4\n*QRCODE r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居右,编码方式设置错误)\n!QRCODE 200 3 -1\n*QRCODE r ABC123456789DEFGH\n*line\n*feedline 1\n",
						
					  };
	uchar *rmbbuf=(uchar *)"\xA3\xA4\xA3\xA4";
	int len = 0;
	char tmp[256] = {0};
	char tmp1[200] = {0};
	char tmp2[] = "*line\n*feedline 1\n";

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:3G ����ͼƬҪ����K21��4G ����ͼƬ���ڰ�׿��/mnt/sdcard/picture/(/data/share/ ·���Ѳ�����)���԰�·�����ĳɴ� 
	//���������ռ�����pngͼƬ
	if(cls_show_msg("�Ƿ��ѽ�����pngͼƬ��ums�ļ����ص�pad��/mnt/sdcard/picture/·��,��DroidSansFallback.ttf�ļ�������SD��·����,��[ESC]�˳�,��[����]����")==ESC)
		return;
	//��ͼƬ�ļ�������k21��(4G������ʹ�д˲���Ҳ����Ӱ��)
	if((nRet=NDK_CopyFileToSecMod(SOURCEFILE, DESTFILE))!=NDK_OK)
	{
		send_result("line %d:����ͼƬ�ļ�ʧ��(%d)",__LINE__, nRet);
		return;	
	}	
	//����ums����·��������/data/share/·��,ʹ��ϵͳ�����ums����/data/share/·����
	system("rm -rf  /data/share/ums");
	system("cp /mnt/sdcard/picture/ums  /data/share/ums");
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Ч 
	{
		send_result("line %d:%s����ʧ��",__LINE__,TESTAPI);
		goto prnt_error;	
	}
	//���ؿ�
	JNI_dlload();
	//��ӡ����
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
			nBusy=FALSE;
			//case1:����!font������ڼ���/system/fonts/Ŀ¼�µ�����
			for(i=0;i<SZ_ARRAY(ttf_str);i++)
			{
				if((nRet = ttf_script_print(ttf_str[i]))!= NDK_OK)					
				{
					send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;
				}
				else 
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
			}
			
			//case2:���������������
			for(i=0;i<SZ_ARRAY(buf_str);i++)
			{
				if((nRet = ttf_script_print(buf_str[i]))!= NDK_OK)					
				{
					send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
			}
			
			//case3:��ӡ��������ĺ��ֺ�����ҷ���,ע�ⲻ�����е����嶼֧��,��֧�ֵ�����Ԥ�ڴ�ӡЧ��ͬ���һ����Ч������һ��,֧������
			for(i = 0;i<SZ_ARRAY(hzfont);i++)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "!NLFONT %d 12 3", hzfont[i]);
				len = strlen(tmp);
				memcpy(&tmp[len], "\x0a", 1);
				tmp[len+1] = '\0';
				if((nRet = ttf_script_print(tmp))!= NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
				memset(tmp, 0, sizeof(tmp));
				//sprintf(tmp, "*text c ��%s������д�ӡ����RMB���ţ���", hzfontstr[i]);
				sprintf(tmp, "*text c 以%s字体居中打印两横RMB符号￥￥", hzfontstr[i]);
				len = strlen(tmp);
				memcpy(&tmp[len], "\x0a", 1);
				tmp[len+1] = '\0';
				if((nRet = ttf_script_print(tmp))!= NDK_OK)
				{					
					send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet, i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
				if((nRet = ttf_script_print(tmp2))!= NDK_OK)
				{					
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
			}
			//case4:��ӡʣ����������壬���ڲ����ֿ�(�����������һ��ҷ��Ų���)
			for(i = 0;i<SZ_ARRAY(dotfont);i++)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "!NLFONT 9 %d 3", dotfont[i]);
				len = strlen(tmp);
				memcpy(&tmp[len], "\x0a", 1);
				tmp[len+1] = '\0';
				if((nRet = ttf_script_print(tmp))!= NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet, i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
				memset(tmp, 0, sizeof(tmp));
				//sprintf(tmp, "*text c ������%s�������(��Ԫ) print dollar symbol:$$", dotfontstr[i]);
				sprintf(tmp, "*text c 以西文%s字体居中(美元) print dollar symbol:$$", dotfontstr[i]);
				len = strlen(tmp);
				memcpy(&tmp[len], "\x0a", 1);
				tmp[len+1] = '\0';
				if((nRet = ttf_script_print(tmp))!= NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet, i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
				if((nRet = ttf_script_print(tmp2))!= NDK_OK)
				{
					send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
			}
			//case4.1:����Ŀǰ�ռ�����ȫ��PNGͼƬ����(����colorϵ�ࡢIHDRϵ�ࡢ������ϵ�࣬����ϵ��) 
			for(i=0;i<SZ_ARRAY(pngbuf);i++)
			{
				switch(pngbuf[i][0])
				{
					case 'y':
						p = 0;
						break;
					case 'c':
						p = 1;
						break;
					case 'I':
						p = 2;
						break;
					case 'o':
						p = 3;
						break;
					default:
						break;
				}
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "*image %s %s path:%s%s%s", type[p], whbuf[i], IMAGEPATH, pngbuf[i], PNGFORMAT);
				len = strlen(tmp);
				memcpy(&tmp[len], "\x0a", 1);
				tmp[len+1] = '\0';
				memset(tmp1, 0, sizeof(tmp1));
				//sprintf(tmp1, "*text c ���´�ӡPNGͼƬ,��ֵ�Զ�(%s),Ч�������ʵ��ͼƬ,��ɫͼƬǳ��ɫ����ӡ�ɰ�ɫ������ɫ����ӡ�ɺ�ɫ",position[p]);
				sprintf(tmp1, "*text c 以下打印PNG图片,阈值自动(%s),效果请对照实际图片,彩色图片浅颜色将打印成白色，深颜色将打印成黑色",position[p]);
				len = strlen(tmp1);
				memcpy(&tmp1[len], "\x0a", 1);
				tmp[len+1] = '\0';
				if((nRet = ttf_script_print("!NLFONT 9 12\n")) != NDK_OK)
				{			
					send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;	
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
				if((nRet = ttf_script_print(tmp1)) != NDK_OK)
				{			
					send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;	
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
				if((nRet = ttf_script_print(tmp))!= NDK_OK)
				{	
					send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
				if((nRet = ttf_script_print(tmp2))!= NDK_OK)
				{	
					send_result("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
				}
			}
			
			//case5:��ӡ���������в������ô���ʱӦ�÷���NDK_ERR_PARA
			for(i=0;i<SZ_ARRAY(err_str);i++)
			{
				if((nRet = ttf_script_print(err_str[i]))!= NDK_ERR_PARA)
				{
					cls_show_msg("line %d:%s����ʧ��(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;	
				}
			}
			
			//case6:�쳣��������
			if((nRet = ttf_script_print(NULL))!=NDK_ERR) 
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}

			//case7:ƫ��x:257��ӡ128��ȵ�ͼƬ������384
			//if((nRet = ttf_script_print("!NLFONT 9 12\n*text l ��ƫ��257��ӡͼƬ,Ԥ����ͼƬ,��������-1\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:257 128*64 #ums\n*line\n*feedline 1\n"))!=NDK_ERR)
			if ((nRet = ttf_script_print("!NLFONT 9 12\n*text l 以偏移257打印图片,预期无图片,函数返回-1\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:257 128*64 #ums\n*line\n*feedline 1\n"))!=NDK_ERR)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}

			// case8:�����ͼƬ��PNG���ͣ�����bmp��jpg����Ӧ����ʧ�ܣ�-1��
			//if((nRet = ttf_script_print("!NLFONT 9 12\n*text c ��ӡͼƬ���ͷǷ����ԣ�Ӧ�򲻳�ͼƬ\n*image c 361*361 path:/mnt/sdcard/picture/xdl.jpg\n*line\n*feedline 1\n"))!=NDK_ERR) 
			if((nRet = ttf_script_print("!NLFONT 9 12\n*text c 打印图片类型非法测试，应打不出图片\n*image c 361*361 path:/mnt/sdcard/picture/xdl.jpg\n*line\n*feedline 1\n"))!=NDK_ERR) 
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
		
			//case9:!font����:���벻�����ļ�������-7
			if((nRet = ttf_script_print("!font err.ttf\n"))!=NDK_ERR_PATH) 
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			
			//case10: �쳣����!font���� ����������·��ttf�ļ�
			//if((nRet = ttf_script_print("!font /mnt/sdcard/DroidSansFallback.ttf\n*text l ��ӡ����·����ttf�ļ�����������Ҫ�ɹ����\n*line\n*feedline 1\n"))!=NDK_OK) 
			if((nRet = ttf_script_print("!font /mnt/sdcard/DroidSansFallback.ttf\n*text l 打印其他路径下ttf文件，本行文字要成功打出\n*line\n*feedline 1\n"))!=NDK_OK) 
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			else
			{
				if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
					cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
			}

			//���Խ���
			send_result("%s����ͨ��", TESTAPI);
			goto prnt_error;
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
				goto prnt_error;
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
			goto prnt_error;
			break;
		}
	
	}
	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);
	return;
}



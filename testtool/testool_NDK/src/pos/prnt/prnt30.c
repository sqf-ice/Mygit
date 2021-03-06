/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: 打印模块
* file name		: prnt30.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: 新NDK_ttf脚本打印接口测试
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
* input parameter	 		:无
* output parameter	 	:无
* return value		 	:无
* history 		 		:  author		date		remarks
*			  	  	  	    sull           20180723         created
*****************************************************************/
//新增的ttf打印接口只判断命令正确与否，不判断打印机状态值,需要自己去调用status函数判断
static int getStatus(void)
{
	EM_PRN_STATUS emStatus = 0;
	int ret = 0;

	while(1)
	{
		if((ret=NDK_PrnGetStatus(&emStatus))!=NDK_OK )
		{
			send_result("line %d:%s获取打印状态失败(%d)",__LINE__,TESTAPI,ret);
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
				send_result("line %d:%s打印机状态异常(%d)", __LINE__, TESTAPI, emStatus);
				return emStatus;
		}
	}	
}
void prnt30(void)
{
	//以下为局部变量
#include	"dotfont.h"
#include	"hzfont.h"

	EM_PRN_STATUS emStatus = 0;
	int nBusy = FALSE, nKeyin = 0, i = 0;
	time_t nOldtime = 0, nDiff = 0;
	int nRet = 0, nRet1 = 0;
	char *type[] = {"l", "c", "r", "l"};
	//char *position[] = {"居左", "居中", "居右", "居左"};
	char *position[] = {"灞呭乏", "灞呬腑", "灞呭彸", "灞呭乏"};
	int p = 0;
	#if 0  //ttf_str的GB2312格式，方便问题定位
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
	#endif
	//TTF打印脚本命令只支持UTF-8格式，而sourceingsight默认GB2312格式，故此文件的命令在sourceinsight上显示为乱码，定位问题时可查看对应的GB2312码
	char *ttf_str[] = {
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓虹郴缁熼粯璁よ嫳鏂囧瓧浣揬n!font DroidSans.ttf\n!NLFONT 9 12 3\n*text l DroidSansDroidSans\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓虹郴缁熼粯璁よ嫳鏂囩矖瀛椾綋\n!font DroidSans-Bold.ttf\n!NLFONT 9 12 3\n*text l DroidSansDroidSans\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓虹郴缁熼粯璁や腑鑻辨枃瀛椾綋\n!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 鏂板ぇ闄嗘敮浠樻妧鏈湁闄愬叕鍙竛ewlandpatment\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篋roidaSansMono瀛椾綋\n!font DroidSansMono.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篋ancingScript绮楀瓧浣揬n!font DancingScript-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篋ancingScript甯歌瀛椾綋\n!font DancingScript-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟oboto甯歌锛堣嫳鏂囷級\n!font Roboto-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟oboto绮椾綋锛堣嫳鏂囷級\n!font Roboto-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟oboto绮楁枩浣擄紙鑻辨枃锛塡n!font Roboto-BoldItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",						
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟obotoRoboto鏂滀綋锛堣嫳鏂囷級\n!font Roboto-Italic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟oboto缁嗗瓧浣擄紙鑻辨枃锛塡n!font Roboto-Thin.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟oboto缁嗘枩浣擄紙鑻辨枃锛塡n!font Roboto-ThinItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟obotoCondensed甯歌锛堣嫳鏂囷級\n!font RobotoCondensed-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟obotoCondensed绮椾綋锛堣嫳鏂囷級\n!font RobotoCondensed-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟obotoCondensed绮楁枩浣擄紙鑻辨枃锛塡n!font RobotoCondensed-BoldItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟obotoCondensed缁嗕綋锛堣嫳鏂囷級\n!font RobotoCondensed-Light.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篟RobotoCondensed缁嗘枩浣擄紙鑻辨枃锛塡n!font RobotoCondensed-LightItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篘otoSerif绮楁枩浣擄紙鑻辨枃锛塡n!font NotoSerif-BoldItalic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篘otoSerif绮椾綋锛堣嫳鏂囷級\n!font NotoSerif-Bold.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篘otoSerif鏂滀綋锛堣嫳鏂囷級\n!font NotoSerif-Italic.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 浠ヤ笅璁剧疆涓篘otoSerif甯歌锛堣嫳鏂囷級\n!font NotoSerif-Regular.ttf\n!NLFONT 9 12 3\n*text l newlandpatmentcompany\n*line\n*feedline 1\n",
						"!font DroidSansFallback.ttf\n!NLFONT 9 12 3\n*text l 鎭㈠榛樿瀛椾綋\n*line\n*feedline 1\n",
					};
	char *pngbuf[] = {"ysz1", "ysz2", "ysz3", "ysz4", "ysz5", "ysz6", "ysz7", "color1", "color2", "color3", "color4", "color5", "color6",
					"IHDR1", "IHDR2", "IHDR3", "IHDR4", "IHDR5", "IHDR6", "IHDR7", "IHDR8", "IHDR9", "IHDR10", "IHDR11", "IHDR12", "IHDR13",
					"IHDR14", "IHDR15", "other1", "other2", "other3", "other4", "other5", "other6", "other7", "other8", "other9", "other10",
					"other11", "other12", "other13", "other14", "other15", "other16"};
	char *whbuf[] = {"284*52", "268*100", "292*52", "309*72", "211*93", "309*72", "302*112", "384*300", "384*300", "384*300", "384*300", "384*300", "384*300", "280*280", "310*136", "310*136", "315*164", 
					 "384*75", "384*241", "195*384", "384*144", "294*292", "300*191", "100*54", "300*200", "384*192", "315*164", "220*183", "315*164", "384*149", "327*87", "315*164", "220*165", "240*240",
					 "250*250", "99*99", "384*49", "328*112", "310*136", "205*86", "361*361", "361*361", "384*259", "384*49"};  //各png图片对应的宽和高
	#if 0 //buf_str的GB2312格式，方便问题定位
	char *buf_str[] = {
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度120居右)请扫描打印出的条形码内容是否为:0123456789ABC\n!barcode 2 120\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",//新增用例测试第一次调用脚本打印直接打印条码,中间不应该有空白行
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度8高度384居右)请扫描打印出的条形码内容是否为:0123456789ABC\n!barcode 8 384\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",//测试条码宽度超过384,返回失败
						"!NLFONT 9 12\n*text c 以下打印二维码(高度384居右)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 384 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",//用例测试二维码宽度超过384,返回失败
						//ttf特有:使用WellnerAdaptiveThreshold阈值算法 yz为0
						"!NLFONT 9 12\n*text c 以下打印PNG图片，阈值使用WellnerAdaptiveThreshold算法(居左)，效果请对照实际图片，彩色图片浅颜色将打印成白色，深颜色将打印成黑色\n*image l 384*300 path:yz:0;/mnt/sdcard/picture/color1.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印PNG图片，阈值使用WellnerAdaptiveThreshold算法(居中)，效果请对照实际图片，彩色图片浅颜色将打印成白色，深颜色将打印成黑色\n*image c 394*300 path:yz:0;/mnt/sdcard/picture/color2.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印PNG图片，阈值使用WellnerAdaptiveThreshold算法(居右)，效果请对照实际图片，彩色图片浅颜色将打印成白色，深颜色将打印成黑色\n*image r 384*300 path:yz:0;/mnt/sdcard/picture/color3.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印PNG图片,阈值255打印黑块(居右)\n*image r 384*240 path:yz:255;/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n",  //新增用例测试png图片功能使用png02和png03替换原来的logo图片
						"!NLFONT 9 12\n*text c 以下打印PNG图片,阈值默认(居中),注意打印效果\n*image c 384*49 path:/mnt/sdcard/picture/png08.png\n*line\n*feedline 1\n",//新增打印图片
						"!NLFONT 9 12\n*text c 以下打印压缩PNG图片,阈值自动，(100*200，居左)，不必关注打印效果\n*image l 100*200 path:/mnt/sdcard/picture/png02.png\n*line\n*feedline 1\n", 
						"!NLFONT 9 12\n*text c 以下打印压缩PNG图片,阈值自动，(100*200，居中)，不必关注打印效果\n*image c 100*200 path:/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n", 
						"!NLFONT 9 12\n*text c 以下打印压缩PNG图片,阈值自动，(100*200，居右)，不必关注打印效果\n*image r 100*200 path:/mnt/sdcard/picture/png04.png\n*line\n*feedline 1\n", //新增打印压缩的PNG图片
						"!NLFONT 9 12\n*text c 以下打印压缩PNG图片,WellnerAdaptiveThreshold算法阈值自动,(384*144,居右),不必关注打印效果\n*image r 384*144 path:yz:0;/mnt/sdcard/picture/IHDR8.png\n*feedline 1\n",
						//新增二维码支持400长度
						"!NLFONT 9 12\n*text c 以下打印二维码(高度100居中，内容400)请扫描打印出的二维码内容是否为:更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录\
						更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录\
						更新记录更新记录更新记录更新记录更新记录\n!qrcode 100 2\n*qrcode c 更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录\
						更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录更新记录\n*line\n*feedline 1\n",
						//打印汉字:hz命令、underline命令
						"!NLFONT 9 12\n*text c 以下打印汉字样式小字体西方样式大字体居左带下划线\n!hz s\n!asc l\n!gray 5\n!yspace 6\n*underline l aBc34国国国国aBc34\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式标准字体西方样式标准字体居中带下划线\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*underline c aBc1234国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国aBc1234\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式大字体西方样式小字体居右带下划线\n!hz l\n!asc s\n!gray 5\n!yspace 6\n*underline r aBc1234国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国aBc1234\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式大字体西方样式小字体偏移16带下划线\n!hz l\n!asc s\n!gray 5\n!yspace 6\n*underline x:16 aBc1234国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国aBc1234\n*line\n*feedline 1\n",//新增偏移量打印x:16
						"!NLFONT 9 12\n*text c 以下打印汉字样式小字体,每行24个,居左\n!hz s\n!gray 5\n!yspace 6\n*text l 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式小字体,每行24个,偏移120像素\n!hz s\n!gray 5\n!yspace 6\n*text x:120 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",//新增偏移量打印x:120
						"!NLFONT 9 12\n*text c 以下打印汉字样式标准字体,每行16个,居中\n!hz n\n!gray 5\n!yspace 6\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式大字体,每行12个,居右\n!hz l\n!gray 5\n!yspace 6\n*text r 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式小字体宽度标准字体高度,居左\n!hz sn\n!gray 5\n!yspace 6\n*text l 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",		
						"!NLFONT 9 12\n*text c 以下打印汉字样式小字体宽度大字体高度,居中\n!hz sl\n!gray 5\n!yspace 6\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字样式标准字体宽度大字体高度,居右\n!hz nl\n!gray 5\n!yspace 6\n*text r 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						//打印英文:asc命令
						"!NLFONT 9 12\n*text c 以下打印西方样式小字体,每行48个,居左\n!asc s\n!gray 5\n!yspace 6\n*text l sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印西方样式标准字体,每行32个,居中\n!asc n\n!gray 5\n!yspace 6\n*text c sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印西方样式大字体,每行24个,居右\n!asc l\n!gray 5\n!yspace 6\n*text r sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印西方样式小字体宽度标准字体高度,居左\n!asc sn\n!gray 5\n!yspace 6\n*text l sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印西方样式小字体宽度大字体高度,居中\n!asc sl\n!gray 5\n!yspace 6\n*text c sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印西方样式标准字体宽度大字体高度,居右\n!asc nl\n!gray 5\n!yspace 6\n*text r sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						//灰度:gray命令
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值为1\n!hz n\n!gray 1\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值为3\n!hz n\n!gray 3\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值为7\n!hz n\n!gray 7\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值为10\n!hz n\n!gray 10\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值为5\n!hz n\n!gray 5\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",//灰度值5放后面是为了恢复默认值5
						//行间距:yspace命令
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为0\n!hz n\n!yspace 0\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为10\n!hz n\n!yspace 10\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为20\n!hz n\n!yspace 20\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为30\n!hz n\n!yspace 30\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为40\n!hz n\n!yspace 40\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为50\n!hz n\n!yspace 50\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为60\n!hz n\n!yspace 60\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距为6\n!hz n\n!yspace 6\n*text c 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",//恢复成默认值行间距6
						//条形码:!barcode命令和*barcode命令
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度120居右)请扫描打印出的条形码内容是否为:0123456789ABC\n!barcode 2 120\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度128居中)请扫描打印出的条形码内容是否为:123456\n!barcode 4 128\n*barcode c 123456\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度128偏移50)请扫描打印出的条形码内容是否为:123456\n!barcode 4 128\n*barcode x:50 123456\n*line\n*feedline 1\n",//新增偏移量打印x:50
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度8高度320居左)请扫描打印出的条形码内容是否为:123\n!barcode 8 320\n*barcode l 123\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度1高度64居中,纯字符31个),不要求能够扫描出来\n!barcode 1 64\n*barcode c AAAAAAAAAABBBBBBBBBBCCCCCCCCCCD\n*line\n*feedline 1\n",//新增可以扫描的字符数的边界测试,宽度为1:纯字符31个,纯数字60,宽度为2:纯字符14个,纯数字28个
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度1高度64居中,纯数字60个),不要求能够扫描出来\n!barcode 1 64\n*barcode c 012345678901234567890123456789012345678901234567890123456789\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,纯数字28个)请扫描打印出的条形码内容是否为:0123456789012345678901234567\n!barcode 2 64\n*barcode c 0123456789012345678901234567\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,纯字母14个)请扫描打印出的条形码内容是否为:AAAAAAAAAABBBB\n!barcode 2 64\n*barcode c AAAAAAAAAABBBB\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,字母与数字组合)请扫描打印出的条形码内容是否为:01AB23BC34C\n!barcode 2 64\n*barcode c 01AB23BC34C\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,字母与数字组合)请扫描打印出的条形码内容是否为:A1B2C3D4E5F6G7\n!barcode 2 64\n*barcode c A1B2C3D4E5F6G7\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,字母与数字组合)请扫描打印出的条形码内容是否为:ABC123CDE4567\n!barcode 2 64\n*barcode c ABC123CDE4567\n*line\n*feedline 1\n",							
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度64居中,纯数字32个(超过28,宽度会自动降为1)),不要求能够扫描出来\n!barcode 2 64\n*barcode c 01234567890123456789012345678912\n*line\n*feedline 1\n",//恢复默认宽度2,高度64(超过28个,宽度会自动降为1)
						//二维码:!qrcode命令和*qrcode命令
						"!NLFONT 9 12\n*text c 以下打印二维码(高度384居右)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 384 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度10居左)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 10 0\n*qrcode l ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度50居中)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 50 1\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度50偏移50)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 50 1\n*qrcode x:50 ABC123456789DEFGH\n*line\n*feedline 1\n",//新增偏移量打印x:50
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居右)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 200 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度100居中)请扫描打印出的二维码内容是否为:ABC123456789DEFGH\n!qrcode 100 2\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度300居中)请扫描打印出的二维码内容是否为:400字节的A\n!qrcode 300 1\n*qrcode c AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度300居中)请扫描打印出的二维码内容是否为:401字节的A\n!qrcode 300 1\n*qrcode c AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n*line\n*feedline 1\n",
						//打印图片
						//"!NLFONT 9 12\n*text c 以下打印图片(居左)\n*image l 32*64 #ums\n*line\n*feedline 1\n",
						//"!NLFONT 9 12\n*text c 以下打印图片(居中)\n*image c 64*128 #ums\n*line\n*feedline 1\n",
						//"!NLFONT 9 12\n*text c 以下打印图片(居右)\n*image r 128*64 #ums\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c 以下打印图片(居右)\n*image r 128*64 #logo\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c 以下打印图片(居中)\n*image c 128*64 data:base64:ROIGODIhuA\n*line\n*feedline 1\n",
						//打印分隔符
						"!hz n\n!asc n\n!gray 7\n!yspace 6\n*text c 以下打印两条分隔符\n*line\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c 以下打印两条分隔符\n*line\n*feedline p 1\n", 
						//NLFONT命令
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
						//暂停时间:pause命令、NLPRNOVER命令						
						"!NLFONT 9 12\n*text l 以下打印图片及文字,将暂停5s后打印\n!NLPRNOVER\n*pause 5\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image c 128*64 #ums\n*text c 银联商务-全民付交易凭条\n!hz l\n!asc l\n*text c (大字体)客服电话:95534\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text l 以偏移256打印图片\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:256 128*64 #ums\n*line\n*feedline 1\n",//新增偏移x:256打印128款度的图片
						//退纸
						"!NLFONT 9 12\n*text l 进退纸测试第一行文字\n*feedline 1\n!gray 5\n!yspace 6\n*text c 第二行文字.打印的文字没有相互覆盖\n*line\n*feedline 1\n",//*feedline -1\n
						"!gray 5\n!yspace 6\n*text c 本行文字应完全在纸槽外\n*feedline 5\n",//!NLPRNOVER\n	
						//条形码
	                    			//ttf特有:!BARCODE命令和*BARCODE命令，支持多种编码，前2个参数跟!barcode一样，第三个参数表示是否显示下方提示信息，第四个参数表示条码编码类型
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
						//二维码
						//ttf特有:!QRCODE命令和*QRCODE命令，支持多种编码，前2个参数跟!qrcode一样，第二个参数仅对QRCODE有效，第三个参数表示二维码编码类型
						//0 Data Matrix;  1 Matrixcode; 2 PDF417; 3 QRCODE
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居中,编码方式Data Matrix)请扫描打印出的二维码内容是否为:Aa12%#@*&αΒγàúǒ\n!QRCODE 200 0 0\n*QRCODE c Aa12%#@*&αΒγàúǒ\n*line\n*feedline 1\n",//新增以较大高度的二维码开始打 再打印高度小的
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居中,编码方式Maxicode)请扫描打印出的二维码内容是否为:ABC~*&%#!56789DEFGH\n!QRCODE 200 0 1\n*QRCODE c ABC~*&%#!56789DEFGH\n*line\n*feedline 1\n",//新增以较大高度的二维码开始打 再打印高度小的
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居中,编码方式PDF417)请扫描打印出的二维码内容是否为:PDF测试支持中文较差\n!QRCODE 200 0 2\n*QRCODE c PDF测试支持中文较差\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印二维码(高度200居中,编码方式QR Code)请扫描打印出的二维码内容是否为:QRCODE测试支持中文好\n!QRCODE 200 2 3\n*QRCODE c QRCODE测试支持中文好\n*line\n*feedline 1\n",
						//ttf特有:*TEXT指令，用法与*text指令一样，区别是*text会换行，*TEXT不会换行
						//ttf特有:*UNDERLINE指令，用法与*underline指令一样，区别是*underline会换行，*UNDERLINE不会换行
						//"!NLFONT 2 1 3\n*TEXT l 本行文字Test\n!NLFONT 9 12 3\n*text r 文字居右并且本行不换行Test\n",
						"!NLFONT 2 1 3\n*TEXT l 本行文字Test\n!NLFONT 9 12 3\n*text r 不换行Test\n"
						"!NLFONT 9 12 3\n*UNDERLINE l 本行文字带下划线\n*underline r 并且不换行Test\n",
						"!NLFONT 9 12 3\n*TEXT l 左边left\n!NLFONT 2 1 3\n*UNDERLINE c 下划线center\n!NLFONT 6 4 3\n*text r 右边right\n",
						//英文横向放大纵向放大之前存在一个英文字母会分开2边的问题
						"!NLFONT 9 12 0\n*text r 靠右打印文字,横向放大、纵向放大,国国国国国国国国国国国国国国国国国国\n*line\n",
						"!NLFONT 9 12 0\n*text r To print text,transverse magnification,vertical magnification\n*line\n",
					  };
	#endif
	char *buf_str[] = {
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害120灞呭彸)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:0123456789ABC\n!barcode 2 120\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",//新增用例测试第一次调用脚本打印直接打印条码,中间不应该有空白行
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害8楂樺害384灞呭彸)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:0123456789ABC\n!barcode 8 384\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害384灞呭彸)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:ABC123456789DEFGH\n!qrcode 384 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						//ttf特有:使用WellnerAdaptiveThreshold阈值算法 yz为0
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃PNG鍥剧墖锛岄槇鍊间娇鐢╓ellnerAdaptiveThreshold绠楁硶(灞呭乏)锛屾晥鏋滆瀵圭収瀹為檯鍥剧墖锛屽僵鑹插浘鐗囨祬棰滆壊灏嗘墦鍗版垚鐧借壊锛屾繁棰滆壊灏嗘墦鍗版垚榛戣壊\n*image l 384*300 path:yz:0;/mnt/sdcard/picture/color1.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃PNG鍥剧墖锛岄槇鍊间娇鐢╓ellnerAdaptiveThreshold绠楁硶(灞呬腑)锛屾晥鏋滆瀵圭収瀹為檯鍥剧墖锛屽僵鑹插浘鐗囨祬棰滆壊灏嗘墦鍗版垚鐧借壊锛屾繁棰滆壊灏嗘墦鍗版垚榛戣壊\n*image c 394*300 path:yz:0;/mnt/sdcard/picture/color2.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃PNG鍥剧墖锛岄槇鍊间娇鐢╓ellnerAdaptiveThreshold绠楁硶(灞呭彸)锛屾晥鏋滆瀵圭収瀹為檯鍥剧墖锛屽僵鑹插浘鐗囨祬棰滆壊灏嗘墦鍗版垚鐧借壊锛屾繁棰滆壊灏嗘墦鍗版垚榛戣壊\n*image r 384*300 path:yz:0;/mnt/sdcard/picture/color3.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃PNG鍥剧墖,闃堝��255鎵撳嵃榛戝潡(灞呭彸)\n*image r 384*240 path:yz:255;/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n",  //鏂板鐢ㄤ緥娴嬭瘯png鍥剧墖鍔熻兘浣跨敤png02鍜宲ng03鏇挎崲鍘熸潵鐨刲ogo鍥剧墖
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃PNG鍥剧墖,闃堝�奸粯璁�(灞呬腑),娉ㄦ剰鎵撳嵃鏁堟灉\n*image c 384*49 path:/mnt/sdcard/picture/png08.png\n*line\n*feedline 1\n",//鏂板鎵撳嵃鍥剧墖
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鍘嬬缉PNG鍥剧墖,闃堝�艰嚜鍔紝(100*200锛屽眳宸�)锛屼笉蹇呭叧娉ㄦ墦鍗版晥鏋淺n*image l 100*200 path:/mnt/sdcard/picture/png02.png\n*line\n*feedline 1\n", 
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鍘嬬缉PNG鍥剧墖,闃堝�艰嚜鍔紝(100*200锛屽眳涓�)锛屼笉蹇呭叧娉ㄦ墦鍗版晥鏋淺n*image c 100*200 path:/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n", 
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鍘嬬缉PNG鍥剧墖,闃堝�艰嚜鍔紝(100*200锛屽眳鍙�)锛屼笉蹇呭叧娉ㄦ墦鍗版晥鏋淺n*image r 100*200 path:/mnt/sdcard/picture/png04.png\n*line\n*feedline 1\n", //鏂板鎵撳嵃鍘嬬缉鐨凱NG鍥剧墖
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鍘嬬缉PNG鍥剧墖,WellnerAdaptiveThreshold绠楁硶闃堝�艰嚜鍔�,(384*144,灞呭彸),涓嶅繀鍏虫敞鎵撳嵃鏁堟灉\n*image r 384*144 path:yz:0;/mnt/sdcard/picture/IHDR8.png\n*feedline 1\n",
						//新增二维码支持400长度
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害100灞呬腑锛屽唴瀹�400)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍\
						鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍\
						鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍\n!qrcode 100 2\n*qrcode c 鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍\
						鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍鏇存柊璁板綍\n*line\n*feedline 1\n",
						//打印汉字:hz命令、underline命令
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡灏忓瓧浣撹タ鏂规牱寮忓ぇ瀛椾綋灞呭乏甯︿笅鍒掔嚎\n!hz s\n!asc l\n!gray 5\n!yspace 6\n*underline l aBc34鍥藉浗鍥藉浗aBc34\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡鏍囧噯瀛椾綋瑗挎柟鏍峰紡鏍囧噯瀛椾綋灞呬腑甯︿笅鍒掔嚎\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*underline c aBc1234鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗aBc1234\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡澶у瓧浣撹タ鏂规牱寮忓皬瀛椾綋灞呭彸甯︿笅鍒掔嚎\n!hz l\n!asc s\n!gray 5\n!yspace 6\n*underline r aBc1234鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗aBc1234\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡澶у瓧浣撹タ鏂规牱寮忓皬瀛椾綋鍋忕Щ16甯︿笅鍒掔嚎\n!hz l\n!asc s\n!gray 5\n!yspace 6\n*underline x:16 aBc1234鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗aBc1234\n*line\n*feedline 1\n",//偏移量打印x:16
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡灏忓瓧浣�,姣忚24涓�,灞呭乏\n!hz s\n!gray 5\n!yspace 6\n*text l 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡灏忓瓧浣�,姣忚24涓�,鍋忕Щ120鍍忕礌\n!hz s\n!gray 5\n!yspace 6\n*text x:120 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",//偏移量打印x:120
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡鏍囧噯瀛椾綋,姣忚16涓�,灞呬腑\n!hz n\n!gray 5\n!yspace 6\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡澶у瓧浣�,姣忚12涓�,灞呭彸\n!hz l\n!gray 5\n!yspace 6\n*text r 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡灏忓瓧浣撳搴︽爣鍑嗗瓧浣撻珮搴�,灞呭乏\n!hz sn\n!gray 5\n!yspace 6\n*text l 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",		
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡灏忓瓧浣撳搴﹀ぇ瀛椾綋楂樺害,灞呬腑\n!hz sl\n!gray 5\n!yspace 6\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鏍峰紡鏍囧噯瀛椾綋瀹藉害澶у瓧浣撻珮搴�,灞呭彸\n!hz nl\n!gray 5\n!yspace 6\n*text r 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						//打印英文:asc命令
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃瑗挎柟鏍峰紡灏忓瓧浣�,姣忚48涓�,灞呭乏\n!asc s\n!gray 5\n!yspace 6\n*text l sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃瑗挎柟鏍峰紡鏍囧噯瀛椾綋,姣忚32涓�,灞呬腑\n!asc n\n!gray 5\n!yspace 6\n*text c sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃瑗挎柟鏍峰紡澶у瓧浣�,姣忚24涓�,灞呭彸\n!asc l\n!gray 5\n!yspace 6\n*text r sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃瑗挎柟鏍峰紡灏忓瓧浣撳搴︽爣鍑嗗瓧浣撻珮搴�,灞呭乏\n!asc sn\n!gray 5\n!yspace 6\n*text l sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃瑗挎柟鏍峰紡灏忓瓧浣撳搴﹀ぇ瀛椾綋楂樺害,灞呬腑\n!asc sl\n!gray 5\n!yspace 6\n*text c sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃瑗挎柟鏍峰紡鏍囧噯瀛椾綋瀹藉害澶у瓧浣撻珮搴�,灞呭彸\n!asc nl\n!gray 5\n!yspace 6\n*text r sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						//灰度:gray命令
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鐏板害鍊间负1\n!hz n\n!gray 1\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鐏板害鍊间负3\n!hz n\n!gray 3\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鐏板害鍊间负7\n!hz n\n!gray 7\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鐏板害鍊间负10\n!hz n\n!gray 10\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鐏板害鍊间负5\n!hz n\n!gray 5\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",//灰度值5放后面是为了恢复默认值5
						//行间距:yspace命令
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧琛岄棿璺濅负0\n!hz n\n!yspace 0\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧琛岄棿璺濅负10\n!hz n\n!yspace 10\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧琛岄棿璺濅负20\n!hz n\n!yspace 20\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧琛岄棿璺濅负30\n!hz n\n!yspace 30\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧琛岄棿璺濅负40\n!hz n\n!yspace 40\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧琛岄棿璺濅负50\n!hz n\n!yspace 50\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧琛岄棿璺濅负60\n!hz n\n!yspace 60\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧琛岄棿璺濅负6\n!hz n\n!yspace 6\n*text c 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",//恢复成默认值行间距6
						//条形码:!barcode命令和*barcode命令
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害120灞呭彸)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:0123456789ABC\n!barcode 2 120\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害4楂樺害128灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:123456\n!barcode 4 128\n*barcode c 123456\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害4楂樺害128鍋忕Щ50)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:123456\n!barcode 4 128\n*barcode x:50 123456\n*line\n*feedline 1\n",//偏移量打印x:50
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害8楂樺害320灞呭乏)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:123\n!barcode 8 320\n*barcode l 123\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害1楂樺害64灞呬腑,绾瓧绗�31涓�),涓嶈姹傝兘澶熸壂鎻忓嚭鏉n!barcode 1 64\n*barcode c AAAAAAAAAABBBBBBBBBBCCCCCCCCCCD\n*line\n*feedline 1\n",//新增可以扫描的字符数的边界测试,宽度为1:纯字符31个,纯数字60,宽度为2:纯字符14个,纯数字28个
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害1楂樺害64灞呬腑,绾暟瀛�60涓�),涓嶈姹傝兘澶熸壂鎻忓嚭鏉n!barcode 1 64\n*barcode c 012345678901234567890123456789012345678901234567890123456789\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,绾暟瀛�28涓�)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:0123456789012345678901234567\n!barcode 2 64\n*barcode c 0123456789012345678901234567\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,绾瓧姣�14涓�)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:AAAAAAAAAABBBB\n!barcode 2 64\n*barcode c AAAAAAAAAABBBB\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,瀛楁瘝涓庢暟瀛楃粍鍚�)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:01AB23BC34C\n!barcode 2 64\n*barcode c 01AB23BC34C\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,瀛楁瘝涓庢暟瀛楃粍鍚�)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:A1B2C3D4E5F6G7\n!barcode 2 64\n*barcode c A1B2C3D4E5F6G7\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,瀛楁瘝涓庢暟瀛楃粍鍚�)璇锋壂鎻忔墦鍗板嚭鐨勬潯褰㈢爜鍐呭鏄惁涓�:ABC123CDE4567\n!barcode 2 64\n*barcode c ABC123CDE4567\n*line\n*feedline 1\n",							
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,绾暟瀛�32涓�(瓒呰繃28,瀹藉害浼氳嚜鍔ㄩ檷涓�1)),涓嶈姹傝兘澶熸壂鎻忓嚭鏉n!barcode 2 64\n*barcode c 01234567890123456789012345678912\n*line\n*feedline 1\n",//恢复默认宽度2,高度64(超过28个,宽度会自动降为1)
						//二维码:!qrcode命令和*qrcode命令
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害384灞呭彸)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:ABC123456789DEFGH\n!qrcode 384 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害10灞呭乏)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:ABC123456789DEFGH\n!qrcode 10 0\n*qrcode l ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害50灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:ABC123456789DEFGH\n!qrcode 50 1\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害50鍋忕Щ50)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:ABC123456789DEFGH\n!qrcode 50 1\n*qrcode x:50 ABC123456789DEFGH\n*line\n*feedline 1\n",//新增偏移量打印x:50
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害200灞呭彸)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:ABC123456789DEFGH\n!qrcode 200 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害100灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:ABC123456789DEFGH\n!qrcode 100 2\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害300灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:400瀛楄妭鐨凙\n!qrcode 300 1\n*qrcode c AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害300灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:401瀛楄妭鐨凙\n!qrcode 300 1\n*qrcode c AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n*line\n*feedline 1\n",
						//打印图片, 开发也不知道这是什么格式的图片，且实际没有用到这样的图片，不测
						//"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鍥剧墖(灞呭乏)\n*image l 32*64 #ums\n*line\n*feedline 1\n",
						//"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鍥剧墖(灞呬腑)\n*image c 64*128 #ums\n*line\n*feedline 1\n",
						//"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鍥剧墖(灞呭彸)\n*image r 128*64 #ums\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鍥剧墖(灞呭彸)\n*image r 128*64 #logo\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鍥剧墖(灞呬腑)\n*image c 128*64 data:base64:ROIGODIhuA\n*line\n*feedline 1\n",
						//打印分隔符
						"!hz n\n!asc n\n!gray 7\n!yspace 6\n*text c 浠ヤ笅鎵撳嵃涓ゆ潯鍒嗛殧绗n*line\n*line\n*feedline 1\n",						
						//"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃涓ゆ潯鍒嗛殧绗n*line\n*feedline p 1\n", 
						//NLFONT命令
						"!NLFONT 9 12 0\n*text l 闈犲乏鎵撳嵃鐨勪腑鏂囦负鑷湁瀹嬩綋24x24,瑗挎枃涓篏ulimche瀛椾綋12x24,妯悜绾靛悜鍧囨斁澶�2鍊峔n*line\n*feedline 1\n",
						"!NLFONT 10 13 1\n*text l 闈犲乏鎵撳嵃鐨勪腑鏂囦负浠垮畫24x24,瑗挎枃涓篗SGothic绮椾綋16*32,妯悜鏀惧ぇ2鍊嶇旱鍚戞甯竆n*line\n*feedline 1\n",
						"!NLFONT 11 17 2\n*text l 闈犲乏鎵撳嵃鐨勪腑鏂囦负妤蜂綋24*24,瑗挎枃涓烘シ浣撶矖浣�12x16,妯悜姝ｅ父绾靛悜鏀惧ぇ2鍊峔n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text l 闈犲乏鎵撳嵃鐨勪腑鏂囦负瀹嬩綋12x12,瑗挎枃涓烘シ浣撶矖浣�16x32,妯悜绾靛悜鍧囨甯竆n*line\n*feedline 1\n",
						"!NLFONT 9 12 0\n*text c 灞呬腑鎵撳嵃鐨勪腑鏂囦负鑷湁瀹嬩綋24x24,瑗挎枃涓篏ulimche瀛椾綋12x24,妯悜绾靛悜鍧囨斁澶�2鍊峔n*line\n*feedline 1\n",
						"!NLFONT 10 13 1\n*text c 灞呬腑鎵撳嵃鐨勪腑鏂囦负浠垮畫24x24,瑗挎枃涓篗SGothic绮椾綋16*32,妯悜2鍊嶆斁澶х旱鍚戞甯竆n*line\n*feedline 1\n",
						"!NLFONT 11 17 2\n*text c 灞呬腑鎵撳嵃鐨勪腑鏂囦负妤蜂綋24*24,瑗挎枃涓烘シ浣撶矖浣�12x16,妯悜姝ｅ父绾靛悜2鍊嶆斁澶n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text c 灞呬腑鎵撳嵃鐨勪腑鏂囦负瀹嬩綋12x12,瑗挎枃涓烘シ浣撶矖浣�16x32,妯悜绾靛悜鍧囨甯竆n*line\n*feedline 1\n",
						"!NLFONT 9 12 4\n*text r 闈犲彸鎵撳嵃鐨勪腑鏂囦负鑷湁瀹嬩綋24x24,瑗挎枃涓篏ulimche瀛椾綋12x24,妯悜3鍊嶆斁澶х旱鍚�3鍊嶆斁澶n*line\n*feedline 1\n",
						"!NLFONT 10 13 5\n*text r 闈犲彸鎵撳嵃鐨勪腑鏂囦负浠垮畫24x24,瑗挎枃涓篗SGothic绮椾綋16*32,妯悜3鍊嶆斁澶х旱鍚戞甯竆n*line\n*feedline 1\n",
						"!NLFONT 11 17 6\n*text r 闈犲彸鎵撳嵃鐨勪腑鏂囦负妤蜂綋24*24,瑗挎枃涓烘シ浣撶矖浣�12x16,妯悜姝ｅ父绾靛悜3鍊嶆斁澶n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text r 闈犲彸鎵撳嵃鐨勪腑鏂囦负瀹嬩綋12x12,瑗挎枃涓烘シ浣撶矖浣�16x32,妯悜绾靛悜鍧囨甯竆n*line\n*feedline 1\n",
						//暂停时间:pause命令、NLPRNOVER命令					
						"!NLFONT 9 12\n*text l 浠ヤ笅鎵撳嵃鍥剧墖鍙婃枃瀛�,灏嗘殏鍋�5s鍚庢墦鍗癨n!NLPRNOVER\n*pause 5\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image c 128*64 #ums\n*text c 閾惰仈鍟嗗姟-鍏ㄦ皯浠樹氦鏄撳嚟鏉n!hz l\n!asc l\n*text c (澶у瓧浣�)瀹㈡湇鐢佃瘽:95534\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text l 浠ュ亸绉�256鎵撳嵃鍥剧墖\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:256 128*64 #ums\n*line\n*feedline 1\n",//鏂板鍋忕Щx:256鎵撳嵃128娆惧害鐨勫浘鐗�
						//退纸
						"!NLFONT 9 12\n*text l 杩涢��绾告祴璇曠涓�琛屾枃瀛梊n*feedline 1\n!gray 5\n!yspace 6\n*text c 绗簩琛屾枃瀛�.鎵撳嵃鐨勬枃瀛楁病鏈夌浉浜掕鐩朶n*line\n*feedline 1\n",//*feedline -1\n
						"!gray 5\n!yspace 6\n*text c 鏈鏂囧瓧搴斿畬鍏ㄥ湪绾告Ы澶朶n*feedline 5\n",//!NLPRNOVER\n	
						//条形码
						//ttf特有:!BARCODE命令和*BARCODE命令，支持多种编码，前2个参数跟!barcode一样，第三个参数表示是否显示下方提示信息，第四个参数表示条码编码类型
	                    			//0 caodebar; 1 code39; 2 code93; 3 code128; 4 EAN-8/EAN-13/JAN-8/JAN-13; 5 ITF; 6 UPCA; 7 UPCE
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨凜odebar鏉″舰鐮佸唴瀹规槸鍚︿负:0123456789锛屽苟涓旀潯鐮佷笅鏂广�愭湁銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 1 0\n*BARCODE c A0123456789C\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨凜ode39鏉″舰鐮佸唴瀹规槸鍚︿负:ABCDEFGHIJKLMNOPQRSTUVWXYZ -.$/+%0123456789锛屽苟涓旀潯鐮佷笅鏂广�愭湁銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 1 1\n*BARCODE c ABCDEFGHIJKLMNOPQRSTUVWXYZ -.$/+%0123456789\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨凜ode93鏉″舰鐮佸唴瀹规槸鍚︿负:CD 45%+67$/锛屽苟涓旀潯鐮佷笅鏂广�愭湁銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 1 2\n*BARCODE c CD 45%+67$\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨凜ode128鏉″舰鐮佸唴瀹规槸鍚︿负:A0123456C%$#@锛屽苟涓旀潯鐮佷笅鏂广�愭湁銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 1 3\n*BARCODE c A0123456C%$#@\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨凟AN-8鏉″舰鐮佸唴瀹规槸鍚︿负:12345670锛屽苟涓旀潯鐮佷笅鏂广�愭棤銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 0 4\n*BARCODE c 1234567\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨凟AN-13鏉″舰鐮佸唴瀹规槸鍚︿负:1234567890128锛屽苟涓旀潯鐮佷笅鏂广�愭棤銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 0 4\n*BARCODE c 123456789012\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨凟AN-8鏉″舰鐮佸唴瀹规槸鍚︿负:00001236锛屽苟涓旀潯鐮佷笅鏂广�愭湁銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 1 4\n*BARCODE c 123\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨凟AN-13鏉″舰鐮佸唴瀹规槸鍚︿负:0000123456784锛屽苟涓旀潯鐮佷笅鏂广�愭湁銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 1 4\n*BARCODE c 12345678\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨処TF鏉″舰鐮佸唴瀹规槸鍚︿负:01234567890128锛屽苟涓旀潯鐮佷笅鏂广�愭棤銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 0 5\n*BARCODE c 0123456789012\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨処TF鏉″舰鐮佸唴瀹规槸鍚︿负:00123456789050锛屽苟涓旀潯鐮佷笅鏂广�愭湁銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 1 5\n*BARCODE c 12345678905\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨刄PC-A鏉″舰鐮佸唴瀹规槸鍚︿负:123456789012锛屽苟涓旀潯鐮佷笅鏂广�愭棤銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 0 6\n*BARCODE c 12345678901\n*line\n*feedline 1\n",	 						
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨刄PC-A鏉″舰鐮佸唴瀹规槸鍚︿负:000765432108锛屽苟涓旀潯鐮佷笅鏂广�愭湁銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 1 6\n*BARCODE c 76543210\n*line\n*feedline 1\n",							
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害6楂樺害120灞呬腑)璇锋壂鎻忔墦鍗板嚭鐨刄PC-E鏉″舰鐮佸唴瀹规槸鍚︿负:01234565锛屽苟涓旀潯鐮佷笅鏂广�愭棤銆戞樉绀烘潯鐮佷俊鎭痋n!BARCODE 6 120 0 7\n*BARCODE c 0123456\n*line\n*feedline 1\n",
						//二维码
						//ttf特有:!QRCODE命令和*QRCODE命令，支持多种编码，前2个参数跟!qrcode一样，第二个参数仅对QRCODE有效，第三个参数表示二维码编码类型
						//0 Data Matrix;  1 Matrixcode; 2 PDF417; 3 QRCODE
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害200灞呬腑,缂栫爜鏂瑰紡Data Matrix)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:Aa12%#@*&伪螔纬脿煤菕\n!QRCODE 200 0 0\n*QRCODE c Aa12%#@*&伪螔纬脿煤菕\n*line\n*feedline 1\n",//新增以较大高度的二维码开始打 再打印高度小的
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害200灞呬腑,缂栫爜鏂瑰紡Maxicode)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:ABC~*&%#!56789DEFGH\n!QRCODE 200 0 1\n*QRCODE c ABC~*&%#!56789DEFGH\n*line\n*feedline 1\n",//新增以较大高度的二维码开始打 再打印高度小的
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害200灞呬腑,缂栫爜鏂瑰紡PDF417)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:PDF娴嬭瘯鏀寔涓枃杈冨樊\n!QRCODE 200 0 2\n*QRCODE c PDF娴嬭瘯鏀寔涓枃杈冨樊\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害200灞呬腑,缂栫爜鏂瑰紡QR Code)璇锋壂鎻忔墦鍗板嚭鐨勪簩缁寸爜鍐呭鏄惁涓�:QRCODE娴嬭瘯鏀寔涓枃濂絓n!QRCODE 200 2 3\n*QRCODE c QRCODE娴嬭瘯鏀寔涓枃濂絓n*line\n*feedline 1\n",
						//ttf特有:*TEXT指令，用法与*text指令一样，区别是*text会换行，*TEXT不会换行
						//ttf特有:*UNDERLINE指令，用法与*underline指令一样，区别是*underline会换行，*UNDERLINE不会换行
						//"!NLFONT 2 1 3\n*TEXT l 鏈鏂囧瓧Test\n!NLFONT 9 12 3\n*text r 鏂囧瓧灞呭彸骞朵笖鏈涓嶆崲琛孴est\n",
						"!NLFONT 2 1 3\n*TEXT l 鏈鏂囧瓧Test\n!NLFONT 9 12 3\n*text r 涓嶆崲琛孴est\n",
						"!NLFONT 9 12 3\n*UNDERLINE l 鏈鏂囧瓧甯︿笅鍒掔嚎\n*underline r 骞朵笖涓嶆崲琛孴est\n",
						"!NLFONT 9 12 3\n*TEXT l 宸﹁竟left\n!NLFONT 2 1 3\n*UNDERLINE c 涓嬪垝绾縞enter\n!NLFONT 6 4 3\n*text r 鍙宠竟right\n",
						//英文横向放大纵向放大之前存在一个英文字母会分开2边的问题
						"!NLFONT 9 12 0\n*text r 闈犲彸鎵撳嵃鏂囧瓧,妯悜鏀惧ぇ銆佺旱鍚戞斁澶�,鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n",
						"!NLFONT 9 12 0\n*text r To print text,transverse magnification,vertical magnification\n*line\n",
					  };
	#if 0 //err_str的GB2312格式，方便问题定位
	char *err_str[] = {
						"!NLFONT 9 12\n*text c 以下进行异常测试,忽略打印效果,不跑飞即可:打印汉字灰度值设置0\n!hz n\n!gray 0\n!yspace 10\n*text l 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字灰度值设置11\n!hz n\n!gray 11\n!yspace 10\n*text l 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印汉字行间距设置70\n!hz n\n!gray 5\n!yspace 70\n*text l 国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国国\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度0高度64居中)\n!gray 5\n!yspace 6\n!barcode 0 64\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度9高度64居中)\n!barcode 9 64\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度0居中)\n!barcode 4 0\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度400居中)\n!barcode 4 400\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度128居中,设置高度后还有空格)\n!barcode 2 128 \n*barcode c 0123456789ABC\n*line\n*feedline 1\n",//设置高度之后还有空格
						"!NLFONT 9 12\n*text c 以下打印二维码(纠错级别参数为4)\n!qrcode 100 4\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c 以下打印二维码(纠错级别参数为-1)\n!qrcode 100 -1\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n!NLPRNOVER\n",
						//"!NLFONT 9 12\n*text l 以偏移257打印图片,预期无图片,函数返回-1\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:257 128*64 #ums\n*line\n*feedline 1\n",//新增偏移x:257打印128款度的图片，超过384
						//新增BARCODE和QRCODE异常情况
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度0高度64居中)\n!gray 5\n!yspace 6\n!BARCODE 0 64 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度9高度64居中)\n!BARCODE 9 64 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度0居中)\n!BARCODE 4 0 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度4高度328居中)\n!BARCODE 4 328 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 以下打印条形码(宽度2高度128居中,设置编码方式后还有空格)\n!BARCODE 2 128 3 \n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",//设置高度之后还有空格
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
	#endif
	char *err_str[] = {
						"!NLFONT 9 12\n*text c 浠ヤ笅杩涜寮傚父娴嬭瘯,蹇界暐鎵撳嵃鏁堟灉,涓嶈窇椋炲嵆鍙�:鎵撳嵃姹夊瓧鐏板害鍊艰缃�0\n!hz n\n!gray 0\n!yspace 10\n*text l 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧鐏板害鍊艰缃�11\n!hz n\n!gray 11\n!yspace 10\n*text l 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃姹夊瓧琛岄棿璺濊缃�70\n!hz n\n!gray 5\n!yspace 70\n*text l 鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗鍥藉浗\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害0楂樺害64灞呬腑)\n!gray 5\n!yspace 6\n!barcode 0 64\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害9楂樺害64灞呬腑)\n!barcode 9 64\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害4楂樺害0灞呬腑)\n!barcode 4 0\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害4楂樺害400灞呬腑)\n!barcode 4 400\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害128灞呬腑,璁剧疆楂樺害鍚庤繕鏈夌┖鏍�)\n!barcode 2 128 \n*barcode c 0123456789ABC\n*line\n*feedline 1\n",//设置高度之后还有空格
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(绾犻敊绾у埆鍙傛暟涓�4)\n!qrcode 100 4\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(绾犻敊绾у埆鍙傛暟涓�-1)\n!qrcode 100 -1\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n!NLPRNOVER\n",
						//"!NLFONT 9 12\n*text l 浠ュ亸绉�257鎵撳嵃鍥剧墖,棰勬湡鏃犲浘鐗�,鍑芥暟杩斿洖-1\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:257 128*64 #ums\n*line\n*feedline 1\n",//鏂板鍋忕Щx:257鎵撳嵃128娆惧害鐨勫浘鐗囷紝瓒呰繃384
						//新增BARCODE和QRCODE异常情况
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害0楂樺害64灞呬腑)\n!gray 5\n!yspace 6\n!BARCODE 0 64 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害9楂樺害64灞呬腑)\n!BARCODE 9 64 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害4楂樺害0灞呬腑)\n!BARCODE 4 0 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害4楂樺害328灞呬腑)\n!BARCODE 4 328 3\n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害128灞呬腑,璁剧疆缂栫爜鏂瑰紡鍚庤繕鏈夌┖鏍�)\n!BARCODE 2 128 3 \n*BARCODE c 0123456789ABC\n*line\n*feedline 1\n",//设置高度之后还有空格
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(绾犻敊绾у埆鍙傛暟涓�4)\n!QRCODE 100 4 3\n*QRCODE c ABC123456789DEFGH\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(绾犻敊绾у埆鍙傛暟涓�-1)\n!QRCODE 100 -1 3\n*QRCODE c ABC123456789DEFGH\n*line\n*feedline 1\n!NLPRNOVER\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡CODEBAR,鏁版嵁鏍煎紡涓嶇鍚堢紪鐮佽鑼�)\n!BARCODE 2 64 0\n*BARCODE c 123456784\n*line\n*feedline 1\n",						
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡CODE39,鏁版嵁鏍煎紡涓嶇鍚堢紪鐮佽鑼�)\n!BARCODE 2 64 1\n*BARCODE c !#*123456784\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡CODE93,鏁版嵁鏍煎紡涓嶇鍚堢紪鐮佽鑼�)\n!BARCODE 2 64 2\n*BARCODE c !#*123456784\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡EAN-8/JAN-8,鏁版嵁鏍￠獙浣嶉敊璇�)\n!BARCODE 2 64 4\n*BARCODE c 34569837\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡EAN-13/JAN-13,鏁版嵁鏍￠獙浣嶉敊璇�)\n!BARCODE 2 64 4\n*BARCODE c 0123456789019\n*line\n*feedline 1\n",			
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡EAN/JAN,鏁版嵁闀垮害閿欒)\n!BARCODE 2 64 4\n*BARCODE c 012345678901234567\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡ITF,鏁版嵁闀垮害閿欒)\n!BARCODE 2 64 5\n*BARCODE c 3801234567899123456\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡UPC-A,鏁版嵁闀垮害閿欒)\n!BARCODE 2 64 6\n*BARCODE c 3801234567899123456\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡UPC-E,鏁版嵁闀垮害閿欒)\n!BARCODE 2 64 7\n*BARCODE c 01234567890\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡UPC-E,绗竴浣嶆暟鎹唴瀹归敊璇�)\n!BARCODE 2 64 7\n*BARCODE c 21234569\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡璁剧疆閿欒)\n!BARCODE 2 64 8\n*BARCODE c 21234569\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃鏉″舰鐮�(瀹藉害2楂樺害64灞呬腑,缂栫爜鏂瑰紡璁剧疆閿欒)\n!BARCODE 2 64 -1\n*BARCODE c 21234569\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害200灞呭彸,缂栫爜鏂瑰紡璁剧疆閿欒)\n!QRCODE 200 3 4\n*QRCODE r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c 浠ヤ笅鎵撳嵃浜岀淮鐮�(楂樺害200灞呭彸,缂栫爜鏂瑰紡璁剧疆閿欒)\n!QRCODE 200 3 -1\n*QRCODE r ABC123456789DEFGH\n*line\n*feedline 1\n",
						
					  };
	uchar *rmbbuf=(uchar *)"\xA3\xA4\xA3\xA4";
	int len = 0;
	char tmp[256] = {0};
	char tmp1[200] = {0};
	char tmp2[] = "*line\n*feedline 1\n";

	//以下为函数体
	cls_printf("测试%s...", TESTAPI); 

	//测试前置:3G 机器图片要存在K21端4G 机器图片存在安卓端/mnt/sdcard/picture/(/data/share/ 路径已不能用)所以把路径均改成此 
	//测试所有收集到的png图片
	if(cls_show_msg("是否已将所有png图片和ums文件下载到pad端/mnt/sdcard/picture/路径,将DroidSansFallback.ttf文件放置在SD卡路径下,否[ESC]退出,是[其他]继续")==ESC)
		return;
	//把图片文件拷贝到k21端(4G机器即使有此操作也不会影响)
	if((nRet=NDK_CopyFileToSecMod(SOURCEFILE, DESTFILE))!=NDK_OK)
	{
		send_result("line %d:拷贝图片文件失败(%d)",__LINE__, nRet);
		return;	
	}	
	//由于ums数据路径必须在/data/share/路径,使用系统命令把ums拷到/data/share/路径下
	system("rm -rf  /data/share/ums");
	system("cp /mnt/sdcard/picture/ums  /data/share/ums");
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//初始化使开始设置的配置有效 
	{
		send_result("line %d:%s测试失败",__LINE__,TESTAPI);
		goto prnt_error;	
	}
	//加载库
	JNI_dlload();
	//打印内容
	while(1)
	{
		if((nRet=NDK_PrnGetStatus(&emStatus))!=NDK_OK )
		{
			send_result("line %d:%s测试失败%d",__LINE__,TESTAPI,nRet);
			goto prnt_error;
		}
		if(emStatus != PRN_STATUS_BUSY)
			show_stopwatch(DISABLE, 0);
		
		switch(emStatus)
		{
		case PRN_STATUS_BUSY:
			if(nBusy==FALSE)
			{
				nOldtime = time(NULL);//从第一次收到忙状态开始计时
				nBusy = TRUE;
			}
			if((nDiff=time(NULL)-nOldtime)>MAXWAITTIME)
			{
				send_result("line %d:打印机处于忙状态时间过长", __LINE__);
				goto prnt_error;
			}
			show_stopwatch(ENABLE, nDiff);
			continue;
			break;

		case PRN_STATUS_OK:
			nBusy=FALSE;
			//case1:新增!font命令，用于加载/system/fonts/目录下的字体
			for(i=0;i<SZ_ARRAY(ttf_str);i++)
			{
				if((nRet = ttf_script_print(ttf_str[i]))!= NDK_OK)					
				{
					send_result("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;
				}
				else 
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
			}
			
			//case2:各种输入参数测试
			for(i=0;i<SZ_ARRAY(buf_str);i++)
			{
				if((nRet = ttf_script_print(buf_str[i]))!= NDK_OK)					
				{
					send_result("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
			}
			
			//case3:打印各种字体的汉字和人民币符号,注意不是所有的字体都支持,不支持的字体预期打印效果同最后一次生效的字体一样,支持字体
			for(i = 0;i<SZ_ARRAY(hzfont);i++)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "!NLFONT %d 12 3", hzfont[i]);
				len = strlen(tmp);
				memcpy(&tmp[len], "\x0a", 1);
				tmp[len+1] = '\0';
				if((nRet = ttf_script_print(tmp))!= NDK_OK)
				{
					send_result("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
				memset(tmp, 0, sizeof(tmp));
				//sprintf(tmp, "*text c 以%s字体居中打印两横RMB符号￥￥", hzfontstr[i]);
				sprintf(tmp, "*text c 浠�%s瀛椾綋灞呬腑鎵撳嵃涓ゆíRMB绗﹀彿锟ワ骏", hzfontstr[i]);
				len = strlen(tmp);
				memcpy(&tmp[len], "\x0a", 1);
				tmp[len+1] = '\0';
				if((nRet = ttf_script_print(tmp))!= NDK_OK)
				{					
					send_result("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet, i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
				if((nRet = ttf_script_print(tmp2))!= NDK_OK)
				{					
					send_result("line %d:%s测试失败(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
			}
			//case4:打印剩余的西文字体，用于测试字库(增加其它国家货币符号测试)
			for(i = 0;i<SZ_ARRAY(dotfont);i++)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "!NLFONT 9 %d 3", dotfont[i]);
				len = strlen(tmp);
				memcpy(&tmp[len], "\x0a", 1);
				tmp[len+1] = '\0';
				if((nRet = ttf_script_print(tmp))!= NDK_OK)
				{
					send_result("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet, i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
				memset(tmp, 0, sizeof(tmp));
				//sprintf(tmp, "*text c 以西文%s字体居中(美元) print dollar symbol:$$", dotfontstr[i]);
				sprintf(tmp, "*text c 浠ヨタ鏂�%s瀛椾綋灞呬腑(缇庡厓) print dollar symbol:$$", dotfontstr[i]);
				len = strlen(tmp);
				memcpy(&tmp[len], "\x0a", 1);
				tmp[len+1] = '\0';
				if((nRet = ttf_script_print(tmp))!= NDK_OK)
				{
					send_result("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet, i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
				if((nRet = ttf_script_print(tmp2))!= NDK_OK)
				{
					send_result("line %d:%s测试失败(%d)",__LINE__,TESTAPI,nRet);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
			}
			//case4.1:进行目前收集到的全部PNG图片测试(包括color系类、IHDR系类、艺术字系类，其他系类) 
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
				//sprintf(tmp1, "*text c 以下打印PNG图片,阈值自动(%s),效果请对照实际图片,彩色图片浅颜色将打印成白色，深颜色将打印成黑色",position[p]);
				sprintf(tmp1, "*text c 浠ヤ笅鎵撳嵃PNG鍥剧墖,闃堝�艰嚜鍔�(%s),鏁堟灉璇峰鐓у疄闄呭浘鐗�,褰╄壊鍥剧墖娴呴鑹插皢鎵撳嵃鎴愮櫧鑹诧紝娣遍鑹插皢鎵撳嵃鎴愰粦鑹�",position[p]);
				len = strlen(tmp1);
				memcpy(&tmp1[len], "\x0a", 1);
				tmp[len+1] = '\0';
				if((nRet = ttf_script_print("!NLFONT 9 12\n")) != NDK_OK)
				{			
					send_result("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;	
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
				if((nRet = ttf_script_print(tmp1)) != NDK_OK)
				{			
					send_result("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;	
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
				if((nRet = ttf_script_print(tmp))!= NDK_OK)
				{	
					send_result("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
				if((nRet = ttf_script_print(tmp2))!= NDK_OK)
				{	
					send_result("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;
				}
				else
				{
					if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
						cls_show_msg("打印机缺纸,装纸后任意键继续");
				}
			}
			
			//case5:打印命令设置中参数设置错误时应该返回NDK_ERR_PARA
			for(i=0;i<SZ_ARRAY(err_str);i++)
			{
				if((nRet = ttf_script_print(err_str[i]))!= NDK_ERR_PARA)
				{
					cls_show_msg("line %d:%s测试失败(%d,%d)",__LINE__,TESTAPI,nRet,i);
					goto prnt_error;	
				}
			}
			
			//case6:异常参数测试
			if((nRet = ttf_script_print(NULL))!=NDK_ERR) 
			{
				send_result("line %d:%s测试失败(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}

			//case7:偏移x:257打印128宽度的图片，超过384
			//if((nRet = ttf_script_print("!NLFONT 9 12\n*text l 以偏移257打印图片,预期无图片,函数返回-1\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:257 128*64 #ums\n*line\n*feedline 1\n"))!=NDK_ERR)
			if ((nRet = ttf_script_print("!NLFONT 9 12\n*text l 浠ュ亸绉�257鎵撳嵃鍥剧墖,棰勬湡鏃犲浘鐗�,鍑芥暟杩斿洖-1\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:257 128*64 #ums\n*line\n*feedline 1\n"))!=NDK_ERR)
			{
				send_result("line %d:%s测试失败(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}

			// case8:传入的图片非PNG类型（比如bmp或jpg），应返回失败（-1）
			//if((nRet = ttf_script_print("!NLFONT 9 12\n*text c 打印图片类型非法测试，应打不出图片\n*image c 361*361 path:/mnt/sdcard/picture/xdl.jpg\n*line\n*feedline 1\n"))!=NDK_ERR) 
			if((nRet = ttf_script_print("!NLFONT 9 12\n*text c 鎵撳嵃鍥剧墖绫诲瀷闈炴硶娴嬭瘯锛屽簲鎵撲笉鍑哄浘鐗嘰n*image c 361*361 path:/mnt/sdcard/picture/xdl.jpg\n*line\n*feedline 1\n"))!=NDK_ERR) 
			{
				send_result("line %d:%s测试失败(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
		
			//case9:!font命令:传入不存在文件，返回-7
			if((nRet = ttf_script_print("!font err.ttf\n"))!=NDK_ERR_PATH) 
			{
				send_result("line %d:%s测试失败(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			
			//case10: 异常测试!font命令 ：传入其他路径ttf文件
			//if((nRet = ttf_script_print("!font /mnt/sdcard/DroidSansFallback.ttf\n*text l 打印其他路径下ttf文件，本行文字要成功打出\n*line\n*feedline 1\n"))!=NDK_OK) 
			if((nRet = ttf_script_print("!font /mnt/sdcard/DroidSansFallback.ttf\n*text l 鎵撳嵃鍏朵粬璺緞涓媡tf鏂囦欢锛屾湰琛屾枃瀛楄鎴愬姛鎵撳嚭\n*line\n*feedline 1\n"))!=NDK_OK) 
			{
				send_result("line %d:%s测试失败(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			else
			{
				if((nRet=getStatus()) == PRN_STATUS_NOPAPER)
					cls_show_msg("打印机缺纸,装纸后任意键继续");
			}

			//测试结束
			send_result("%s测试通过", TESTAPI);
			goto prnt_error;
			break;
		case PRN_STATUS_NOPAPER:
			cls_printf("打印机缺纸...装纸后,任意键继续,ESC放弃");
			lib_kbflush(); //清KB BUF
			while ((nKeyin=lib_kbhit())==0)
			{
				NDK_SysBeep();
				//正常装纸
				NDK_PrnFeedByPixel(10);
				sleep(2);
			}
			if (nKeyin==ESC)
			{
				send_result("line %d:打印机缺纸,用户放弃", __LINE__);
				goto prnt_error;
			}
			NDK_PrnStart();
			cls_printf("测试%s...", TESTAPI);
			break;
		case PRN_STATUS_OVERHEAT:
			send_result("line %d: 打印机过热", __LINE__);
			goto prnt_error;
			break;
		case PRN_STATUS_VOLERR:
			send_result("line %d: 打印机电源故障", __LINE__);
			goto prnt_error;
			break;
		case PRN_STATUS_DESTROYED:
			send_result("line %d: 打印机损坏", __LINE__);
			goto prnt_error;
			break;
		default:
			//未知状态，判为错
			send_result("line %d: UNKNOWN STATE(tpstatus=%d)", __LINE__, emStatus);
			goto prnt_error;
			break;
		}
	
	}
	
prnt_error:
	NDK_PrnInit(g_PrnSwitch);
	return;
}



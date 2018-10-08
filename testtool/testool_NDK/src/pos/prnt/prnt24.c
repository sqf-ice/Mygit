/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module		: ��ӡģ��
* file name		: prnt24.c
* Author 		:  
* version		: 
* DATE			:  
* directory 		: 
* description		: ���� NDK_Script_Print�ű���ӡ����
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
#define TESTAPI	"NDK_Script_Print"	
#define MAXWAITTIME 90
#define DESTFILE "/appfs/ums"
#define SOURCEFILE "/mnt/sdcard/picture/ums"  //"/data/share/ums"  ԭ�ȵ�·����Ȩ�� 20171101 modify
#define PNGFORMAT ".png"
#define IMAGEPATH "/mnt/sdcard/picture/"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 		:void prnt24(void)
* functional description 	: 
* input parameter	 		:��
* output parameter	 	:��
* return value		 	:��
* history 		 		:  author		date		remarks
*			  	  	  	jiangym		20160606 created
*****************************************************************/
void prnt24(void)
{
	//����Ϊ�ֲ�����
#include	"dotfont.h"
#include	"hzfont.h"

	EM_PRN_STATUS emStatus = 0;
	int nBusy = FALSE, nKeyin = 0, i = 0;
	time_t nOldtime = 0, nDiff = 0;
	int nRet = 0, nRet1 = 0;
	char *type[] = {"l", "c", "r", "l"};
	char *position[] = {"����", "����", "����", "����"};
	int p = 0;
	char *pngbuf[] = {"ysz1", "ysz2", "ysz3", "ysz4", "ysz5", "ysz6", "ysz7", "color1", "color2", "color3", "color4", "color5", "color6",
					"IHDR1", "IHDR2", "IHDR3", "IHDR4", "IHDR5", "IHDR6", "IHDR7", "IHDR8", "IHDR9", "IHDR10", "IHDR11", "IHDR12", "IHDR13",
					"IHDR14", "IHDR15", "other1", "other2", "other3", "other4", "other5", "other6", "other7", "other8", "other9", "other10",
					"other11", "other12", "other13", "other14", "other15", "other16"};
	char *whbuf[] = {"284*52", "268*100", "292*52", "309*72", "211*93", "309*72", "302*112", "384*300", "384*300", "384*300", "384*300", "384*300", "384*300", "280*280", "310*136", "310*136", "315*164", 
					 "384*75", "384*241", "195*384", "384*144", "294*292", "300*191", "100*54", "300*200", "384*192", "315*164", "220*183", "315*164", "384*149", "327*87", "315*164", "220*165", "240*240",
					 "250*250", "99*99", "384*49", "328*112", "310*136", "205*86", "361*361", "361*361", "384*259", "384*49"};  //��pngͼƬ��Ӧ�Ŀ�͸�
	char *buf_str[] = {
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�120����)��ɨ���ӡ���������������Ƿ�Ϊ:0123456789ABC\n!barcode 2 120\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",//�����������Ե�һ�ε��ýű���ӡֱ�Ӵ�ӡ����,�м䲻Ӧ���пհ���
						"!NLFONT 9 12\n*text c ���´�ӡ������(���8�߶�400����)��ɨ���ӡ���������������Ƿ�Ϊ:0123456789ABC\n!barcode 8 400\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",//�����������������ȳ���384,Ԥ���ܹ���ӡ������ɨ��,���߷���ʧ��
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�400����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 400 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",//�����������Զ�ά���ȳ���384,Ԥ���ܹ���ӡ����ȷɨ��
						//"!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ,��ֵĬ��(����),ע���ӡЧ��\n*image l 240*240 path:/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n", //ԭ�ȵ�·����Ȩ��,���޸ĵ�/mnt/sdcard/ 20171101 modify ���ϵ����е�pngͼƬ��ӡ�� 20180307
						//"!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ,��ֵĬ��(����),ע���ӡЧ��\n*image c 220*165 path:/mnt/sdcard/picture/png02.png\n*line\n*feedline 1\n", 				
						//"!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ,��ֵĬ��(����),ע���ӡЧ��\n*image r 220*165 path:/mnt/sdcard/picture/png04.png\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ,��ֵ255��ӡ�ڿ�(����)\n*image r 384*240 path:yz:255;/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n",  //������������pngͼƬ����ʹ��png02��png03�滻ԭ����logoͼƬ
						"!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ,��ֵĬ��(����),ע���ӡЧ��\n*image c 384*49 path:/mnt/sdcard/picture/png08.png\n*line\n*feedline 1\n",//������ӡͼƬ
						"!NLFONT 9 12\n*text c ���´�ӡѹ��PNGͼƬ,��ֵ�Զ���(100*200������)�����ع�ע��ӡЧ��\n*image l 100*200 path:/mnt/sdcard/picture/png02.png\n*line\n*feedline 1\n", 
						"!NLFONT 9 12\n*text c ���´�ӡѹ��PNGͼƬ,��ֵ�Զ���(100*200������)�����ع�ע��ӡЧ��\n*image c 100*200 path:/mnt/sdcard/picture/png03.png\n*line\n*feedline 1\n", 
						"!NLFONT 9 12\n*text c ���´�ӡѹ��PNGͼƬ,��ֵ�Զ���(100*200������)�����ع�ע��ӡЧ��\n*image r 100*200 path:/mnt/sdcard/picture/png04.png\n*line\n*feedline 1\n", //������ӡѹ����PNGͼƬ
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
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС����,ÿ��48��,����\n!asc s\n!gray 5\n!yspace 6\n*text l sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ��׼����,ÿ��32��,����\n!asc n\n!gray 5\n!yspace 6\n*text c sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ������,ÿ��24��,����\n!asc l\n!gray 5\n!yspace 6\n*text r sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС�����ȱ�׼����߶�,����\n!asc sn\n!gray 5\n!yspace 6\n*text l sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽС�����ȴ�����߶�,����\n!asc sl\n!gray 5\n!yspace 6\n*text c sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������ʽ��׼�����ȴ�����߶�,����\n!asc nl\n!gray 5\n!yspace 6\n*text r sssssssssssssssssssssssssssssssssssssssssssssssssssssss\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵΪ1\n!hz n\n!gray 1\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵΪ3\n!hz n\n!gray 3\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵΪ7\n!hz n\n!gray 7\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵΪ10\n!hz n\n!gray 10\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵΪ5\n!hz n\n!gray 5\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",//�Ҷ�ֵ5�ź�����Ϊ�˻ָ�Ĭ��ֵ5
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ0\n!hz n\n!yspace 0\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ10\n!hz n\n!yspace 10\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ20\n!hz n\n!yspace 20\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ30\n!hz n\n!yspace 30\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ40\n!hz n\n!yspace 40\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ50\n!hz n\n!yspace 50\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ60\n!hz n\n!yspace 60\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м��Ϊ6\n!hz n\n!yspace 6\n*text c ������������������������������������������������������������\n*line\n*feedline 1\n",//�ָ���Ĭ��ֵ�м��6
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�16����)��ɨ���ӡ���������������Ƿ�Ϊ:0123456789ABC\n!barcode 2 16\n*barcode r 0123456789ABC\n*line\n*feedline 1\n",
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
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�400����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 400 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",//�����Խϴ�߶ȵĶ�ά�뿪ʼ�� �ٴ�ӡ�߶�С��
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�10����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 10 0\n*qrcode l ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�50����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 50 1\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�50ƫ��50)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 50 1\n*qrcode x:50 ABC123456789DEFGH\n*line\n*feedline 1\n",//����ƫ������ӡx:50
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�200����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 200 3\n*qrcode r ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�100����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:ABC123456789DEFGH\n!qrcode 100 2\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�300����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:400�ֽڵ�A\n!qrcode 300 1\n*qrcode c AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(�߶�300����)��ɨ���ӡ���Ķ�ά�������Ƿ�Ϊ:401�ֽڵ�A\n!qrcode 300 1\n*qrcode c AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡͼƬ(����)\n*image l 32*64 #ums\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡͼƬ(����)\n*image c 64*128 #ums\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡͼƬ(����)\n*image r 128*64 #ums\n*line\n*feedline 1\n",
						"!hz n\n!asc n!gray 7\n!yspace 6\n*text c ���´�ӡ�����ָ���\n*line\n*line\n*feedline 1\n",
						
						"!NLFONT 9 12 0\n*text l �����ӡ������Ϊ��������24x24,����ΪGulimche����12x24,����������Ŵ�\n*line\n*feedline 1\n",
						"!NLFONT 10 13 1\n*text l �����ӡ������Ϊ����24x24,����ΪMSGothic����16*32,����Ŵ���������\n*line\n*feedline 1\n",
						"!NLFONT 11 17 2\n*text l �����ӡ������Ϊ����24*24,����Ϊ�������12x16,������������Ŵ�\n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text l �����ӡ������Ϊ����12x12,����Ϊ�������16x32,�������������\n*line\n*feedline 1\n",
						//����Ϊ20160920��������
						"!NLFONT 9 12 0\n*text c ���д�ӡ������Ϊ��������24x24,����ΪGulimche����12x24,����������Ŵ�\n*line\n*feedline 1\n",
						"!NLFONT 10 13 1\n*text c ���д�ӡ������Ϊ����24x24,����ΪMSGothic����16*32,����Ŵ���������\n*line\n*feedline 1\n",
						"!NLFONT 11 17 2\n*text c ���д�ӡ������Ϊ����24*24,����Ϊ�������12x16,������������Ŵ�\n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text c ���д�ӡ������Ϊ����12x12,����Ϊ�������16x32,�������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12 0\n*text r ���Ҵ�ӡ������Ϊ��������24x24,����ΪGulimche����12x24,����������Ŵ�\n*line\n*feedline 1\n",
						"!NLFONT 10 13 1\n*text r ���Ҵ�ӡ������Ϊ����24x24,����ΪMSGothic����16*32,����Ŵ���������\n*line\n*feedline 1\n",
						"!NLFONT 11 17 2\n*text r ���Ҵ�ӡ������Ϊ����24*24,����Ϊ�������12x16,������������Ŵ�\n*line\n*feedline 1\n",
						"!NLFONT 13 19 3\n*text r ���Ҵ�ӡ������Ϊ����12x12,����Ϊ�������16x32,�������������\n*line\n*feedline 1\n",
						"!NLFONT 0 0 3\n*text r ���Ҵ�ӡ������Ϊ����12x12,����Ϊ�������16x32,�������������\n*line\n*feedline 1\n",
						
						"!NLFONT 9 12\n*text l ���´�ӡͼƬ������,����ͣ5s���ӡ\n!NLPRNOVER\n*pause 5\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image c 128*64 #ums\n*text c ��������-ȫ�񸶽���ƾ��\n!hz l\n!asc l\n*text c (������)�ͷ��绰:95534\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text l ��ƫ��256��ӡͼƬ\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:256 128*64 #ums\n*line\n*feedline 1\n",//����ƫ��x:256��ӡ128��ȵ�ͼƬ
						"!NLFONT 9 12\n*text l ����ֽ���Ե�һ������\n*feedline 1\n!gray 5\n!yspace 6\n*text c �ڶ�������.��ӡ������û���໥����\n*line\n*feedline 1\n",//*feedline -1\n
						"!gray 5\n!yspace 6\n*text c ��������Ӧ��ȫ��ֽ����\n*feedline 5\n",//!NLPRNOVER\n	
						// ������ӡĬ������ 20180419  
						//������ʽΪ0ΪĬ��ֵ������Ϊ0Ĭ�ϣ�����δ�ӡ������Ϊ��һ�δ�ӡ�����壬��Ϊ��һ�δ�ӡ����0ΪĬ�����壬����Ĭ������Ϊ24*24��Ӣ��Ĭ������Ϊ12*12
						//!hz s\n!+!NLFONT 0�ᵼ�º��ִ�ӡ�쳣����Ĭ�ϳ������ʹ��Ĭ���Ի��Ǵ�ģ����Ǵ�һ�������ķ�Ĭ�ϵ�
						"!hz s\n!asc s\n!NLFONT 0 0 3\n*text c ���½��к�����Ӣ��Ĭ���������,���д�ӡ���ִ�СΪС����,Ӣ�Ĵ�СΪС����,������ʽΪ����Ĭ��(24*24)Ӣ��Ĭ��(12*12)��ӡģʽΪ����������������,English\n*line\n",
						"!hz s\n!NLFONT 0 1 3\n*text c ���½��к���Ĭ���������,���д�ӡ���ִ�СΪС����,������ʽΪ����Ĭ��(24*24)Ӣ��(8*16)��ӡģʽΪ����������������,English\n*line\n",
						"!asc s\n!NLFONT 1 0 3\n*text c ���½���Ӣ��Ĭ���������,���д�ӡӢ�Ĵ�СΪС����,������ʽΪ����(24*24)Ӣ��Ĭ��(12*12)��ӡģʽΪ����������������,English\n*line\n",
						"!hz s\n!asc s\n!NLFONT 1 1 3\n*text c ���½����������,���д�ӡ���ִ�СΪС����,������ʽΪ����(24*24)Ӣ��(8*16)��ӡģʽΪ����������������,English\n*line\n",
						"!hz s\n!asc s\n!NLFONT 0 0 3\n*text c ���½��к�����Ӣ��Ĭ���������,���д�ӡӢ�Ĵ�СΪС����,������ʽΪ����Ĭ��(24*24)Ӣ��Ĭ��(12*12)��ӡģʽΪ����������������,English\n*line\n",
						"!hz s\n!asc s\n!NLFONT 1 1 3\n*text c ���½����������,���д�ӡ���ִ�СΪС����,������ʽΪ���֣�24*24��Ӣ�ģ�8*16����ӡģʽΪ����������������,English\n*line\n",
					  };
	char *err_str[] = {
						"!NLFONT 9 12\n*text c ���½����쳣����,���Դ�ӡЧ��,���ܷɼ���:��ӡ���ֻҶ�ֵ����0\n!hz n\n!gray 0\n!yspace 10\n*text l ������������������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ���ֻҶ�ֵ����11\n!hz n\n!gray 11\n!yspace 10\n*text l ������������������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ�����м������70\n!hz n\n!gray 5\n!yspace 70\n*text l ������������������������������������������������������������������������\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���0�߶�64����)\n!gray 5\n!yspace 6\n!barcode 0 64\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���9�߶�64����)\n!barcode 9 64\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���4�߶�0����)\n!barcode 4 0\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���4�߶�328����)\n!barcode 4 328\n*barcode c 0123456789ABC\n*line\n*feedline 1\n",
						"!NLFONT 9 12\n*text c ���´�ӡ������(���2�߶�128����,���ø߶Ⱥ��пո�)\n!barcode 2 128 \n*barcode c 0123456789ABC\n*line\n*feedline 1\n",//���ø߶�֮���пո�
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(���������Ϊ4)\n!qrcode 100 4\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n",					
						"!NLFONT 9 12\n*text c ���´�ӡ��ά��(���������Ϊ-1)\n!qrcode 100 -1\n*qrcode c ABC123456789DEFGH\n*line\n*feedline 1\n!NLPRNOVER\n",
						"!NLFONT 9 12\n*text l ��ƫ��257��ӡͼƬ,Ԥ����ͼƬ,��������-1\n!NLPRNOVER\n!hz n\n!asc n\n!gray 5\n!yspace 6\n*image x:257 128*64 #ums\n*line\n*feedline 1\n",//����ƫ��x:257��ӡ128��ȵ�ͼƬ������384
					  };
	uchar *rmbbuf=(uchar *)"\xA3\xA4\xA3\xA4";
	int len = 0;
	char tmp[256] = {0};
	char tmp1[200] = {0};
	char tmp2[] = "*line\n*feedline 1\n";

	//����Ϊ������
	cls_printf("����%s...", TESTAPI); 

	//����ǰ��:3G ����ͼƬҪ����K21��4G ����ͼƬ���ڰ�׿��/mnt/sdcard/picture/(/data/share/ ·���Ѳ�����20171101 modify)���԰�·�����ĳɴ� 
	//�������������ռ�����pngͼƬ 20180307 sull add
	if(cls_show_msg("�Ƿ��ѽ�����pngͼƬ��ums�ļ����ص�pad��/mnt/sdcard/picture/·��,��[ESC]�˳�,��[����]����")==ESC)
		return;
	//��ͼƬ�ļ�������k21��(4G������ʹ�д˲���Ҳ����Ӱ��)
	if((nRet=NDK_CopyFileToSecMod(SOURCEFILE, DESTFILE))!=NDK_OK)
	{
		send_result("line %d:����ͼƬ�ļ�ʧ��(%d)",__LINE__, nRet);
		return;	
	}	
	//����ums����·��������/data/share/·��,ʹ��ϵͳ�����ums����/data/share/·����20171101 modify
	system("rm -rf  /data/share/ums");
	system("cp /mnt/sdcard/picture/ums  /data/share/ums");
	if(lib_initprn(g_PrnSwitch) != NDK_OK)//��ʼ��ʹ��ʼ���õ�������Ч 
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
			nBusy=FALSE;
			//case1:���������������
			for(i=0;i<SZ_ARRAY(buf_str);i++)
			{
				if((nRet = NDK_Script_Print(buf_str[i],strlen(buf_str[i])))!= NDK_OK)
				{
					if(nRet == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
					else
					{
						send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
						goto prnt_error;
					}
				}
			}
			//case4:������ӡ��������ĺ��ֺ�����ҷ���,ע�ⲻ�����е����嶼֧��,��֧�ֵ�����Ԥ�ڴ�ӡЧ��ͬ���һ����Ч������һ��,֧������
			for(i = 0;i<SZ_ARRAY(hzfont);i++)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp,"!NLFONT %d 12 3\n*text c ��%s������д�ӡ����RMB����%s\n*line\n*feedline 1\n", hzfont[i], hzfontstr[i], rmbbuf);
				if((nRet = NDK_Script_Print(tmp,strlen(tmp)))!= NDK_OK)
				{
					if(nRet == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
					else
					{
						send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
						goto prnt_error;
					}
				}
			}
			//case4.1:����Ŀǰ�ռ�����ȫ��PNGͼƬ����(����colorϵ�ࡢIHDRϵ�ࡢ������ϵ�࣬����ϵ��) sull 20180319 add
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
				sprintf(tmp1, "!NLFONT 9 12\n*text c ���´�ӡPNGͼƬ,��ֵ�Զ�(%s),Ч�������ʵ��ͼƬ,��ɫͼƬǳ��ɫ����ӡ�ɰ�ɫ������ɫ����ӡ�ɺ�ɫ\n",position[p]);
				if((nRet = NDK_Script_Print(tmp1,strlen(tmp1))) != NDK_OK)
				{			
					if(nRet == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
					else
					{
						send_result("line %d:%s����ʧ��(%d,%s)",__LINE__,TESTAPI,nRet,tmp);
						goto prnt_error;
					}	
				}
				if((nRet = NDK_Script_Print(tmp,strlen(tmp)))!= NDK_OK)
				{	
					if(nRet == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
					else
					{
						send_result("line %d:%s����ʧ��(%d,%s)",__LINE__,TESTAPI,nRet,tmp);
						goto prnt_error;
					}	
				}
				if((nRet = NDK_Script_Print(tmp2,strlen(tmp2)))!= NDK_OK)
				{	
					if(nRet == PRN_STATUS_NOPAPER)
						cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
					else
					{
						send_result("line %d:%s����ʧ��(%d,%s)",__LINE__,TESTAPI,nRet,tmp);
						goto prnt_error;
					}	
				}
			}
			
			//case2:��ӡ���������в������ô���ʱӦ�ò�������ܷ����
			for(i=0;i<SZ_ARRAY(err_str);i++)
			{
				if((nRet = NDK_Script_Print(err_str[i],strlen(err_str[i])))!= NDK_OK)
				{
					if(i==10)//����Ԥ��ʧ�ܵ��쳣���Խ��д���
					{
						if(nRet != NDK_ERR)
						{
							send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
							goto prnt_error;
						}
					}
					else
					{
						if(nRet == PRN_STATUS_NOPAPER)
							cls_show_msg("��ӡ��ȱֽ,װֽ�����������");
						else
						{
							send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
							goto prnt_error;
						}
					}
				}
			}
			if (cls_show_msg("���Ͼ�������ӡ�����ܷ�,��[ȷ��],��[����]")!=ENTER)
			{
				send_result("line %d:%s����ʧ��", __LINE__, TESTAPI);
				goto prnt_error;
			}
			
			//case3:�쳣��������
			if((nRet = NDK_Script_Print(NULL,10))!=NDK_ERR_PARA) 
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet);
				goto prnt_error;
			}
			if((nRet1 = NDK_Script_Print(buf_str[0],0))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet1);
				goto prnt_error;
			}
			if((nRet1 = NDK_Script_Print(buf_str[0],-1))!=NDK_ERR_PARA)
			{
				send_result("line %d:%s����ʧ��(%d)",__LINE__,TESTAPI,nRet1);
				goto prnt_error;
			}	
			
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


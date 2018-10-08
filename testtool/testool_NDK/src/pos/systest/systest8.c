/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 8.c
* Author 			: chenfm
* version			: V1.0
* DATE			: 20130117
* directory 		: 
* description		: �ļ�ϵͳ�ۺϲ���
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  20130117 chenfm
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"fsys����,ѹ��"
#if !K21_ENABLE
#define	FILETESTG8 "FTG8"
#define	BUFFERSIZE	(1024*20)
#else
#define	FILETESTG8 "/appfs/FTG8"
#define	BUFFERSIZE	(1024)
#endif
#define	BUFLEN		(1024)

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*					chenfm				20130117	created
*****************************************************************/
//���˵��:��ע��һ��ǿ�����Ƿ�����������
static void filesys_press_wr(void)
{
	/*private & local definition*/
	char writebuf2[BUFLEN] = {0}, readbuf2[BUFLEN] = {0};
	int fp = 0, writelen = 0, readlen = 0,succ = 0;
	uint unLen = 0;
#define	DEFAULT_CNT_STR	"3000"	//ѹ��������С����!һ�����������������ӦС�ڸ���ֵ,������ܲ��Բ�������
#define	DEFAULT_CNT_VLE	atoi(DEFAULT_CNT_STR)	//(1000)
	int ret = -1, cnt = DEFAULT_CNT_VLE, bak = 0;
	char CntStr[8] = DEFAULT_CNT_STR;
	
	/*process body*/
	//����,��д,�ر�,ɾ��ѹ��
	NDK_FsDel(FILETESTG8);//��������

	cls_printf("Ĭ�ϲ��Դ���:\n");
	if((ret=lib_kbgetinput(CntStr, 0, sizeof(CntStr)-1, &unLen,INPUTDISP_OTHER, 5, INPUT_CONTRL_LIMIT))==NDK_ERR||ret==NDK_ERR_TIMEOUT)
		bak = cnt = DEFAULT_CNT_VLE;
	else
		bak = cnt = atoi(CntStr);
	
	memset(writebuf2, rand()/256/*0x44*/, sizeof(writebuf2));
	
	while(cnt)
	{
		if (ESC==cls_show_msg1(2, "���ļ���дѹ����...\n��ʣ%d��,ESC�жϲ���", cnt))
			break;
		cnt--;
			
		if ((fp=NDK_FsOpen(FILETESTG8, "w"))<0)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:%s��%d��:�����ļ�ʧ��(%d)", __LINE__,TESTITEM, bak-cnt, fp);
			continue;//return; 20171106�޸�:ִ����ԱҪ��ͳ�Ʋ��Գɹ���
			
		}
		if ((writelen= NDK_FsWrite(fp,writebuf2, BUFLEN))!=BUFLEN)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:д�ļ�ʧ��(%d)", __LINE__, bak-cnt,writelen);
			NDK_FsClose(fp);
			NDK_FsDel(FILETESTG8);
			continue;//return;
		}
		if ((ret=NDK_FsClose(fp))!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:�ر��ļ�ʧ��(%d)", __LINE__, bak-cnt, ret);
			NDK_FsDel(FILETESTG8);
			continue;//return;
		}
		if ((fp=NDK_FsOpen(FILETESTG8, "r"))<0)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:���ļ�ʧ��(%d)", __LINE__, bak-cnt, fp);
			NDK_FsDel(FILETESTG8);
			continue;//return;
		}
		memset(readbuf2, 0, sizeof(readbuf2));
		if ((readlen=NDK_FsRead(fp,readbuf2, BUFLEN))!=BUFLEN)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:���ļ�ʧ��(%d)", __LINE__, bak-cnt, readlen);
			NDK_FsClose(fp);
			NDK_FsDel(FILETESTG8);
			continue;//return;
		}
		if (memcmp(writebuf2, readbuf2, sizeof(writebuf2)))
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:У���", __LINE__, bak-cnt);
			NDK_FsClose(fp);
			NDK_FsDel(FILETESTG8);
			continue;//return;
		}		
		if ((ret=NDK_FsClose(fp))!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:�ر��ļ�ʧ��(%d)", __LINE__, bak-cnt, ret);
			NDK_FsDel(FILETESTG8);
			continue;//return;
		}
		if ((ret=NDK_FsDel(FILETESTG8))!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:ɾ���ļ�ʧ��(%d)", __LINE__, bak-cnt, ret);
			NDK_FsDel(FILETESTG8);
			continue;//return;
		}
		succ++;
	}
	
	cls_show_msg_record(FILENAME,FUNCNAME,"���ļ���дѹ���������,��ִ�д���Ϊ%d,�ɹ�%d��", bak-cnt, succ);
	return;
}

static void filesys_press_bigfile(void)
{
	/*private & local definition*/
	int fp = 0, writelen = 0, readlen = 0, cnt = 0, cnt2 = 0, loop = 0,  filelen1 = 0;
	time_t oldtime = 0;
	float fTimeElapsed = 0;
	char writebuf[BUFFERSIZE] = {0}, readbuf[BUFFERSIZE] = {0};
	ulong discspace1 = 0, discspace2 = 0;
	char str[32] = {0};
	uint filelen = 0;

	//���嵥��������С����λK
#if defined SP610
#define BLOCK_SIZE 8	
#else
#define BLOCK_SIZE 4
#endif

	//����ÿ����Ʒ�ĵ����ļ�����С(�ж˲�Ʒû�����Ƶ����ļ���С,�Ͷ˲�Ʒ�ļ���С�ɵ����ļ����������64*����������С4����8������,������256����512,��׿ƽ̨δ֪)
#if defined ME31
#define MAX_FLIE_SIZE 360
#elif defined ME30||defined ME30THM||defined ME32THM||defined ME30MH||defined ME30GX||defined ME50||defined ME50N||defined ME50C||defined ME50H||defined ME62||defined ME50NX||defined ME68||defined ME11||defined ME15||defined ME15C||defined ME20
#define MAX_FLIE_SIZE BLOCK_SIZE*64
#elif ANDIROD_ENABLE //defined IM81||defined N900||
#define MAX_FLIE_SIZE 240
#elif defined ME15C
#define MAX_FLIE_SIZE 66
#elif defined ME30S
#define MAX_FLIE_SIZE 88
#else
#define MAX_FLIE_SIZE 65535	//�����ļ���Сû�����Ƶ����SP610���ж˲�Ʒ 
#endif

	/*process body*/
	// step	(1) ȡ������ʣ��ռ�
#if !(ANDIROD_ENABLE||defined ME15C|| defined ME30S)//Ŀǰ��ME15C,�߶˲�Ʒ��K21������������Ʒ��֧��NDK_FsGetDiskSpace����defined IM81||defined N900||
#if !K21_ENABLE
	sync();sleep(3);
#endif
	NDK_FsGetDiskSpace(1,&discspace1);
#if !K21_ENABLE
	cnt2 = discspace1/sizeof(writebuf)+1;//���1��Ŀ����������һ����д��������,�Դﵽ�߽���Ե�Ч��	
#else
	cnt2 = (discspace1-discspace1/sizeof(writebuf)/BLOCK_SIZE*7)/sizeof(writebuf)+1;//�Ͷ˲�Ʒ��ȡ����ʣ��ռ��С���ļ���д����������Ϊ������λ��������С������4����8��ÿ������Ҫʹ��4+2+1�ֽ�����¼�ļ�ϵͳ��Ϣ,������ʣ��������*7���ֽڵĿռ��޷���ʹ��
#endif
	cnt = (cnt2<MAX_FLIE_SIZE)?cnt2:MAX_FLIE_SIZE;//��ʣ��ռ������ļ���С���бȽϣ���С��ֵ��Ϊ�����ļ���ʵ�ʴ�С���в���
#else
	cnt = MAX_FLIE_SIZE;
#endif
	filelen1 = cnt*sizeof(writebuf);//����������ļ���Ԥ�Ƴ���
/*
#if !K21_ENABLE||defined SP610||defined ME20
	sync();sleep(3);
	NDK_FsGetDiskSpace(1,&discspace1);
	cnt = discspace1/sizeof(writebuf)+1;//���1��Ŀ����������һ����д��������,�Դﵽ�߽���Ե�Ч��
#elif defined ME15C	//ME15C ME20 ���ֻ֧��67KB 
	cnt = 66; 
#elif defined ME11||defined ME15 //ME11 ���֧��128KB
	cnt =118; 
#elif defined ME30||defined ME30MH||defined ME50||defined ME50N//||defined SP610	//ME30 ���֧��512KB
	cnt = 511; 
#else
	cnt = 240;	//IM81,ME31���֧��256KB����֧��NDK_FsGetDiskSpace,����д����Ч����243K+597�ֽڹʴ˴��ĳ�240��,ÿ��1K
#endif
*/
	cls_printf("�����ļ�������...����д��Լ%dK�ĳ����ļ�,�����ĵȴ�", cnt);
	NDK_FsDel(FILETESTG8);
	
	// step	(2) ��"w"��ʽ��һ�ض��ļ�
	if ((fp= NDK_FsOpen(FILETESTG8, "w"))<0)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:���������ļ�ʧ��", __LINE__);
		goto ERR;
	}

	// step	(3) д��20KB����(�����)�����ļ���
	// step	(4) ѭ��������2,���������ļ�
	memset(writebuf, rand()/256/*0x24*/, sizeof(writebuf));
	oldtime = time(NULL);
	lib_StartStopwatch();
	for (loop=0; loop<cnt; loop++)
	{
		if ((writelen=NDK_FsWrite(fp,writebuf, sizeof(writebuf)))!=sizeof(writebuf))
		{
			if((loop+1)!=cnt)
			{
				cls_show_msg_record(FILENAME,FUNCNAME, "line %d:Ԥ��Ӧ����дԼ%dK���ݣ�ʵ��д��%dKʱ����(%d)", __LINE__, cnt, loop+1, writelen);
				goto ERR;
			}
			else
			{
				cls_show_msg1(2, "��������д��(%d)", writelen);//д��ʱ����-1,ͬʱʵ���ϻ�д�������ݽ�ȥ(д��)
				filelen1 = sizeof(writebuf)*loop;//���������ļ���Ԥ�Ƴ���
				cnt = loop;//����cnt,��ʱcnt=ʵ�ʵ�XXK����
			}
		}
		show_stopwatch(ENABLE, time(NULL)-oldtime);//zhangnw20110715add
	}
	fTimeElapsed = lib_ReadStopwatch();
	show_stopwatch(DISABLE, 0);

	// step	(5) �رո��ļ�
	if (NDK_FsClose(fp)!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME, "line %d:�ر��ļ�����", __LINE__);
		goto ERR;
	}

	// step	(6) ��"r"��ʽ�򿪸��ļ�
	if ((fp= NDK_FsOpen(FILETESTG8, "r"))<0)
	{
		cls_show_msg_record(FILENAME,FUNCNAME, "line %d:�򿪳����ļ�ʧ��", __LINE__);
		goto ERR;
	}

	// step	(7) ȡ���ļ�����
	NDK_FsFileSize(FILETESTG8,&filelen);
	if (filelen<filelen1)
	{
		cls_show_msg_record(FILENAME,FUNCNAME, "line %d:�ļ������쳣(%d,%d)", __LINE__, filelen, filelen1);
		goto ERR;
	}

	// step	(8) �����ļ����ݲ�����У��
	for (loop=0; loop<cnt; loop++)
	{
		memset(readbuf, 0, sizeof(readbuf));
		if ((readlen=NDK_FsRead(fp,readbuf, sizeof(readbuf)))!=sizeof(readbuf))
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d�ζ��ļ�����(%d)", __LINE__, loop+1, readlen);
			goto ERR;
		}
		if (memcmp(writebuf, readbuf, sizeof(writebuf)))
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��У���", __LINE__, loop+1);
			goto ERR;
		}
	}
	if(filelen>filelen1)//if(writelen!=sizeof(writebuf))//��β�������Ĳ���sizeof(writebuf)�������ݽ���У��
	{
		memset(readbuf, 0, sizeof(readbuf));
		if ((readlen=NDK_FsRead(fp,readbuf, filelen-filelen1))!=(filelen-filelen1))
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:���ļ�����(%d,%d)", __LINE__, readlen, filelen-filelen1);
			goto ERR;
		}
		if (memcmp(writebuf, readbuf, (filelen-filelen1)))
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:У���", __LINE__);
			goto ERR;
		}
	}

	// step	(9) �رղ�ɾ�����ļ�
	NDK_FsClose(fp);
	NDK_FsDel(FILETESTG8);

	// step	(10)�ٴ�ȡ������ʣ��ռ䣬���Ƿ�Ϊ����1ȡ�ÿռ估һ��
	//Ŀǰ�ж˲�Ʒ���ļ�ϵͳ�ǻ���Linux��,�ռ䲢���������ͷ�,ֻ����85�Ļ��ߵͶ˲�Ʒ���Ҫ���������20170809 �������jym�޸�
#if  K21_ENABLE && !defined ME30S//!(defined IM81||defined N900||ANDIROD_ENABLE||ME15C)//30S��֧��NDK_FsGetDiskSpace 20180509
#if 0//!K21_ENABLE
	sync();sleep(3);
#endif 
	NDK_FsGetDiskSpace(1,&discspace2);
	if (discspace1!=discspace2)
	{
		cls_show_msg_record(FILENAME,FUNCNAME, "line %d:�����̿ռ�δ��ȫ�ͷ�(%ld,%ld)", __LINE__, discspace1, discspace2);
		return;
	}
#endif

#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME, "�����ļ�����ͨ��(������СΪ%dKB�ļ���ʱ%fs)", (cnt*sizeof(writebuf)+writelen)/1024, fTimeElapsed);
#else
	ftos(fTimeElapsed,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "�����ļ�����ͨ��(������СΪ%dKB�ļ���ʱ%ss)", (cnt*sizeof(writebuf)+writelen)/1024, str);
#endif
	return;
ERR:
	NDK_FsClose(fp);
	NDK_FsDel(FILETESTG8);
	return;
}

static void filesys_press_multifiles(void)
{
	/*private & local definition*/
	int fp[5] ={-1, -1, -1, -1, -1}, ret = -1;
	int countnum = 0, i = 0, j = 0, cnt = 0/*, filelen[5] = {0}*/;
	int  writelen = 0, readlen = 0;
	char writebuf[BUFLEN] = {0}, readbuf[BUFLEN] = {0}, tempbuf[BUFLEN] = {0};
#if !K21_ENABLE
	char filename[5][10] ={"FTG8A", "FTG8B", "FTG8C", "FTG8D", "FTG8E"};
#else
	char filename[5][20] ={"/appfs/FTG8A", "/appfs/FTG8B", "/appfs/FTG8C", "/appfs/FTG8D", "/appfs/FTG8E"};
#endif
	ulong discspace1 = 0;

	/*process body*/
	cls_printf("���ļ�������������...");

	//NDK_FsGetDiskSpace(1,&discspace1);
	//cnt = discspace1/1024/5/8*8-7;//ÿ���ļ�����8K��������-7���ֽ�,�����Ͷ˲�Ʒÿ���ļ���ռ��8K�ռ�,��ʵ���ļ���С��8K-7
#if defined ME20||defined ME30THM||defined ME32THM||defined ME32||defined ME65 //ME20�ļ�ϵͳ��С��68K,ÿ���ļ�����4K��������-7���ֽڣ�һ���ļ�д11K����ռ��12K�Ĵ�С
	cnt = 11;
#elif defined ME32GX
	cnt = 7;
#else
	cnt = 60;//Ϊ���Ч�ʽ�cnt��С��60
#endif
	
	// step (1)
	for (countnum = 0; countnum<5; countnum++)
		NDK_FsDel(filename[countnum]);
	
	// step (2) ��"w"��ʽ������5���ļ�:FT21A,FT21B,FT21C,FT21D,FT21E,�ֱ�д��5K�ֽڵ�����(����ļ���д���������ݸ�����ͬ)
	for (countnum = 0; countnum<5; countnum++)
	{
		if ((fp[countnum] =  NDK_FsOpen(filename[countnum], "w"))<0) // �����ļ�
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:�����ļ�(%d)ʧ��", __LINE__, countnum);
			for (i = countnum; i >= 0; i--)
			{
				NDK_FsClose(fp[i]);
				NDK_FsDel(filename[i]);
			}
			return;
		}

		memset(writebuf, countnum, sizeof(writebuf));
		for (j = 0; j < cnt; j++)
		{
			if ((writelen =  NDK_FsWrite(fp[countnum],writebuf, BUFLEN))!=BUFLEN) // д�����ݳ��ȳ���
			{
				cls_show_msg_record(FILENAME,FUNCNAME, "line %d:д��%d���ļ��ĵ�%dK���ݳ���(%d)", __LINE__, countnum+1, j+1, writelen);
				for (i = countnum; i >= 0; i--)
				{
					NDK_FsClose(fp[i]);
					NDK_FsDel(filename[i]);
				}
				return;
			}
		}
	}
	//ME 32 ME32GX��Ҫ�ر�֮���ٽ��м��
#if K21_ENABLE  //defined ME32|| defined ME32GX || defined ME32THM|| defined ME30S||defined ME65||CPU5810X_ENABLE//���� ��Ʒ NDK_FsExistʵ����ͨ��fsopen���ж�,���ھ��������,�ᵼ��ʧ��,����Ҫ�ȹرվ��20180412
	for (i = 0; i<5; i++)
		NDK_FsClose(fp[i]);
#endif
	//��Ը߶˲�Ʒ�����bug�����˼���ļ��Ƿ����20171113(ȫ��ƽ̨ͨ��)
	for (countnum = 0; countnum<5; countnum++)
	{
		if((ret=NDK_FsExist(filename[countnum])) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:����%d���ļ��Ƿ���ڳ���(%d)", __LINE__, countnum+1, ret);
			for (i = 0; i<5; i++)
			{
				NDK_FsClose(fp[i]);
				NDK_FsDel(filename[i]);
			}
			return;
		}
	}

	// step (3) �ر�������ļ�
	for (countnum = 0; countnum<5; countnum++)
		NDK_FsClose(fp[countnum]);

	// step (4) ��"r"��ʽ��������ļ����ֱ��������У��
	for (countnum = 0; countnum<5; countnum++)
	{
		if ((fp[countnum] = NDK_FsOpen(filename[countnum], "r"))<0) // ���ļ�ʧ��
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:���ļ�(%d)ʧ��", __LINE__, countnum);
			for (i = countnum; i >= 0; i--)
			{
				NDK_FsClose(fp[i]);
				NDK_FsDel(filename[i]);
			}
			return;
		}

		for (j = 0; j < cnt; j++)
		{
			memset(readbuf, 0, sizeof(readbuf));
			if ((readlen =  NDK_FsRead(fp[countnum],readbuf, BUFLEN))!=BUFLEN) // �������ݳ��ȳ��� 
			{
				cls_show_msg_record(FILENAME,FUNCNAME, "line %d:����%d���ļ��ĵ�%dK���ݳ���(%d)", __LINE__, countnum+1, j+1, readlen);
				for (i = countnum; i >= 0; i--)
				{
					NDK_FsClose(fp[i]);
					NDK_FsDel(filename[i]);
				}
				return;
			}

			memset(tempbuf, countnum, sizeof(tempbuf));
			if (memcmp(tempbuf, readbuf, readlen)) // У������
			{
				cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d���ļ�У�����", __LINE__, countnum+1);
				for (i = 0; i<5; i--)
				{
					NDK_FsClose(fp[i]);
					NDK_FsDel(filename[i]);
				}
				return;
			}
		}
		
		//ɾ��ĳ���ļ���,��ȡʣ���ļ��������ݱȽ�,����Ӧ�ò��ᱻ�ı�
		if(countnum%2==1)//��FTG8B��FTG8D�ļ�ɾ�����ȡʣ���ļ����бȽ�
		{
			NDK_FsClose(fp[countnum]);
			NDK_FsDel(filename[countnum]);
		}
	}

	for (i = 0; i<5; i++)
	{
		NDK_FsClose(fp[i]);
		NDK_FsDel(filename[i]);
	}

	cls_show_msg_record(FILENAME,FUNCNAME, "���ļ�����������ͨ��"); // ����ͨ��
	return;
}

static void filesys_press(void)
{
	/*private & local definition*/
	int keyin = '0';

	/*process body*/
	while (1)
	{
		if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
		{
			 //��Ӧ����Ĳ˵���+1���˵����Ӵ˴�ҲҪ��Ӧ������
			if((++keyin == '4')||(cls_show_msg1(2, "��������%s����ѹ������,ESC�˳�",TESTITEM)==ESC))
				return ;
		}
		else
		{
			keyin = cls_show_msg("_�ļ�ϵͳѹ��_\n"
								"1.�����ļ���д\n" 
								"2.���������ļ�\n"
								"3.���ļ�������\n");
		}
		
		switch (keyin)
		{
		case '1':
			filesys_press_wr();
			break;
		case '2':
			filesys_press_bigfile();
			break;			
		case '3':
			filesys_press_multifiles();
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

//���˵��:����ע���ܡ�ѹ��(������һ��ǿ�����Ƿ�ɹ�)��,����ע����
//һ����˵,Ҳ���ù�עability����,����ֵ�ں������й����и���
static void filesys_ability(void)
{
	/*private & local definition*/
	char writebuf2[BUFLEN] = {0}, readbuf2[BUFLEN] = {0};
	int ret = -1,fp = 0, writelen = 0, readlen = 0, j = 0;
#if !K21_ENABLE
	ulong space = 0;
#else
	char str[32] = {0};
#endif

#if !K21_ENABLE
	sync();sleep(3);
	NDK_FsGetDiskSpace(1,&space);
	int times = (space/BUFLEN>10240)?10240:(space/BUFLEN);//д�ļ���С��ԭ����1M�޸ĳ�10M 20140219 �������Ҫ���޸�
#elif defined ME15C||defined ME20||defined ME32THM	//ME15C ���ֻ֧��67KB 
	int times = 66; 
#elif defined ME11||defined ME15 //ME11 ���ֻ֧��128KB (121+597)
	int times = 118; 
#elif defined ME30||defined ME30THM||defined ME30MH||defined ME32THM||defined ME30GX||CPU5810X_ENABLE||defined ME50NX||defined ME68	//ME30 ���ֻ֧��512KB
	int times = 511; 
#elif defined ME30S
    int times =255; 
#elif defined ME32GX||defined ME32||defined ME65
	int times = 51;
#else
	int times = 255;  //ME31���ֻ�ܴ�256KB,,ʵ�ʿ��ܻ�����256,ͬʱ�������֮ǰ���и�ʽ��
#endif
	/*process body*/
	if(times==0)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:���̿ռ䲻��,�����������!", __LINE__);
		return;
	}
	
	//��д1M���ݺ�ʱ����
	NDK_FsDel(FILETESTG8);//��������
	memset(writebuf2, rand()/256/*0x44*/, sizeof(writebuf2));
	fp =  NDK_FsOpen(FILETESTG8, "w");

	cls_printf("д�ļ���...  ");
	lib_StartStopwatch();
	//for(j=0; j<1024; j++)//ѭ��д1K����1K*10240��
	for(j=0; j<times; j++)
	{
		if ((writelen= NDK_FsWrite(fp,writebuf2, BUFLEN))!=BUFLEN)//�����ѹ��������ͬ,�������ж�һ�·���
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:д�ļ�ʧ��(%d,%d)", __LINE__,j, writelen);
			NDK_FsClose(fp);
			NDK_FsDel(FILETESTG8);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME, "д�ļ��ٶ�Ϊ%fkb/s", times/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(times/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME, "д�ļ��ٶ�Ϊ%skb/s", str);
#endif
	if ((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:�ر��ļ�ʧ��(%d)", __LINE__, ret);
		NDK_FsDel(FILETESTG8);
		return;
	}
	if ((fp=NDK_FsOpen(FILETESTG8, "r"))<0)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:���ļ�ʧ��(%d)", __LINE__, fp);
		NDK_FsDel(FILETESTG8);
		return;
	}

	cls_printf("���ļ���...  ");
	memset(readbuf2, 0, sizeof(readbuf2));
	lib_StartStopwatch();
	for(j=0; j<times; j++)//��10240*1024=10M������
	{
		if ((readlen= NDK_FsRead(fp,readbuf2, BUFLEN))!=BUFLEN)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:���ļ�ʧ��(%d)", __LINE__, readlen);
			NDK_FsClose(fp);
			NDK_FsDel(FILETESTG8);
			return;
		}
	}
#if !K21_ENABLE
	cls_show_msg_record(FILENAME,FUNCNAME, "���ļ��ٶ�Ϊ%fkb/s", times/lib_ReadStopwatch());
#else
	memset(str,0,sizeof(str));
	ftos(times/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME, "���ļ��ٶ�Ϊ%skb/s", str);
#endif
	if ((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:�ر��ļ�ʧ��(%d)", __LINE__, ret);
		NDK_FsDel(FILETESTG8);
		return;
	}
	if ((ret=NDK_FsDel(FILETESTG8))!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:ɾ���ļ�ʧ��(%d)", __LINE__, ret);
		NDK_FsDel(FILETESTG8);
		return;
	}
	
	return;
}

static void fwrite_shutdown(void)
{
	/*private & local definition*/
	char wbuf[BUFFERSIZE] = {0};
	int fp = 0, wlen = 0, i = 0;
	time_t oldtime = 0;
	int cnt = 10;
	/*process body*/
	if ((fp=NDK_FsOpen(FILETESTG8, "w"))<0)
	{
		cls_show_msg("line %d:�����ļ�ʧ��", __LINE__);
		return;
	}
	memset(wbuf, rand()/256/*0x44*/, sizeof(wbuf));
	cls_printf("д�ļ�������...");
#if defined ME15C||defined ME20 
	cnt = 8;
#else
	cnt = 120;//�Ͷ��ļ�ϵͳ������Ϊ��д��λ��ÿ������4K��8K
#endif
	oldtime = time(NULL);
	for(; i<cnt; i++)//ȡ120,�Խӽ�2M���ҵĴ�С
	{
		if ((wlen=NDK_FsWrite(fp,wbuf, BUFFERSIZE))!=BUFFERSIZE)
			break;
		show_stopwatch(ENABLE, time(NULL)-oldtime);	
	}
	show_stopwatch(DISABLE, 0);

	cls_printf("��д��%d bytes����!��ֱ�Ӷϵ�!", BUFFERSIZE*i);
	while(1);//��ѭ��,��ǿ���û��ػ�
}

//���Բ���:1(����ǰ��)->2(��¼�����ļ�ǰ�Ĵ�С)->3(ֱ�ӹص�Դ����)->2(��¼�����ļ���Ĵ�С,�仯�������ļ���С)->1(�����ֳ�)->2(�ռ��СӦ�ָ�����ǰ)
//˵��:ִ��3ʱ,����ʹ��"ֱ�ӹص�Դ"�����ǰ���Դ��,������Դ��,��һ���д��������;"ֱ�ӹص�Դ"��һ����д��(�ļ�Ҳ��ⲻ��,�ռ�Ҳ����)
//��֮�󿴵��ռ��С�򲻱䶼�п���.
//���ռ��С�ڶϵ�ǰ�󲻱仯,��Ҫע��ռ�ʵ������뷵�صĿռ��Сһ��(���걾�������ʹ�����ܼ������ļ���������֤�Ƿ����㹻�ռ�)
static void filesys_shutdown(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	ulong pulSpace = 0;
	
	/*process body*/
	while (1)
	{
		nKeyin = cls_show_msg("ѡ����ʵĶ���:\n"
							"1.ɾ�������ļ�\n"
              					"2.ȡʣ��ռ�\n"
							"3.fwrite��ϵ�\n");
		
		switch (nKeyin)
		{
		case '1':
			if( NDK_FsExist(FILETESTG8)==NDK_OK)
			{
				cls_show_msg1(2, "��⵽�����ļ�,ɾ����...");
				NDK_FsDel(FILETESTG8);
			}
			else
				cls_show_msg1(2, "δ��⵽�����ļ�");
			break;
		case '2':
#if !K21_ENABLE
			sync();sleep(3);
#endif
			NDK_FsGetDiskSpace(1,&pulSpace);
			cls_show_msg("��ǰ������ʣ��ռ�:%ld,���¼!", pulSpace);
			break;	
		case '3':
			fwrite_shutdown();
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

//������ͼ:��fwrite�����а��ػ���,��ʱ��Ӧ��ʲô�쳣,��,�޷��ػ�(����дȫ��ȫ���п���)
static void fwrite_shutdown1(int slct)
{
	/*private & local definition*/
	char wbuf[BUFFERSIZE] = {0};
	int fp = 0, wlen = 0, i = 0;
	//time_t oldtime = 0;
	int cnt = 10;
	/*process body*/
	NDK_FsDel(FILETESTG8);
	
	if ((fp= NDK_FsOpen(FILETESTG8, "w"))<0)
	{
		cls_show_msg("line %d:�����ļ�ʧ��", __LINE__);
		return;
	}
	memset(wbuf, rand()/256/*0x44*/, sizeof(wbuf));
	cls_show_msg("�Ȱ������,��2s��%s", slct==1?"���ػ������йػ�":"ֱ�ӹرյ�Դ");
	//oldtime = time(NULL);
#if defined ME15C||defined ME20 
		cnt = 64;  //ֻ��68K���ļ�ϵͳ  д��64�ܹ���ʱ����йػ�
#else
		cnt = 1024;
#endif	
	for(; i<cnt/*102*/; i++)//ȡ102,�Խӽ�2M���ҵĴ�С   20140714 �޸�102��1024����Թػ��Ƚ������豸����д����ʱ�����д���˻�û����ɹػ�����
	{
		if ((wlen=NDK_FsWrite(fp,wbuf, BUFFERSIZE))!=BUFFERSIZE)
			break;
		//show_stopwatch(ENABLE, time(NULL)-oldtime);	
	}
	show_stopwatch(DISABLE, 0);

	cls_show_msg("��д��%d bytes����!�ػ�ʧ��!", BUFFERSIZE*i);
	while(1);//�������ػ�,��ǿ����ػ�!
}

//fwrite�������µ�����,��ʱ��Ӧ��ʲô�쳣,��,�޷��ػ�(����дȫ��ȫ���п���)
static void filesys_shutdown1(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	
	/*process body*/
	while (1)
	{
		nKeyin = cls_show_msg("1.fwrite�йػ�\n"
              					"2.fwrite�е���\n");
		
		switch (nKeyin)
		{
		case '1':
		case '2':
			fwrite_shutdown1(nKeyin-'0');
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

#if 0
//������ͼ:д��������,����ӦӰ��ϵͳ������������(��,�����޷�������),��Ҫ����������ʹ��"�ļ��ۺ�"���˵��е�"����"����
static void filesys_bigfile(void)
{
	/*private & local definition*/
	int fp = 0, writelen = 0, loop = 0/*MIN_SIGNINT*/;
	char writebuf[BUFFERSIZE] = {0};//ͳһʹ��40K�ͺ���,����ջ���
	time_t oldtime = 0;

	/*process body*/
	NDK_FsDel(FILETESTG8);

	// step	(2) ��"w"��ʽ��һ�ض��ļ�
	if ((fp=NDK_FsOpen(FILETESTG8, "w"))<0)
	{
		cls_show_msg("line %d:���������ļ�ʧ��", __LINE__);
		goto ERR;
	}

	// step	(3) д��20KB����(�����)�����ļ���
	// step	(4) ѭ��������2,���������ļ�
	cls_printf("д����������...");
	memset(writebuf, rand()/256/*0x24*/, sizeof(writebuf));
	oldtime = time(NULL);
	for (; loop<MAX_SIGNINT; loop++)
	{
		if ((writelen= NDK_FsWrite(fp,writebuf, sizeof(writebuf)))!=sizeof(writebuf))
		{
			cls_printf("line %d:��������д��,����������!(%d,%d)", __LINE__, loop, writelen);
			 NDK_FsClose(fp);
			while(1);//��ѭ��,��ǿ���û��ػ�
		}
		show_stopwatch(ENABLE, time(NULL)-oldtime);//zhangnw20110715add
	}
	show_stopwatch(DISABLE, 0);

	cls_show_msg("line %d:������δ��д��,����ʧ��!", __LINE__);
ERR:	
	NDK_FsClose(fp);
	NDK_FsDel(FILETESTG8);
	return;
}
#endif

#if !(ANDIROD_ENABLE||ME15C)//��֧��ȡ�����̿ռ亯���Ĳ�Ʒ�����쳣����defined IM81||defined N900||
//ע��:�ļ��쳣���Ժ󽫿��ܻ���������ļ���ϵͳ��,�ͻ�Ӱ���ļ����Ȳ�����Ŀ��.���鵽ʱ�ٽ���һ�θ�ʽ��
static void filesys_abnormity(void)
{
	/*private & local definition*/
	int nKeyin = 0;
	
	/*process body*/
	while (1)
	{
		nKeyin = cls_show_msg("_�ļ��쳣����_\n"
			              		"1.fwrite��ϵ�\n"  //20140827 1.12��׼��NA �� ��Ӧ�Ĵ�������
			              		"2.fwrite�жϵ�\n"
			              		//"3.д��������"
			              		);
		
		switch (nKeyin)
		{
		case '1':
			filesys_shutdown();
			break;
		case '2':
			filesys_shutdown1();
			break;
		//case '3':
			//filesys_bigfile();
			//break;
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
#endif

void systest8(void)
{
	/*private & local definition*/
	int keyin = 0;
	ulong pulSpace = 0;
	
	/*process body*/
	cls_show_msg1(2, "%s������...", TESTITEM);

	if(auto_flag==1)//autotest_flag
	{
		//�Զ�����������
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s�������Կ�ʼ",TESTITEM);
		
		//ִ��ѹ������
		filesys_press();

		//ִ�����ܲ���
		filesys_ability();
		
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s�������Խ���",TESTITEM);
		return;
	}
	else
	{
	while(1)
       {
              keyin = cls_show_msg("1.ѹ������\n"
							"2.���ܲ���\n"
              					"3.�쳣����\n"
							"4.������\n");
		switch(keyin) 
		{
		case '1':
			cls_show_msg1(1, "�ļ�ϵͳѹ��������...\n");// 1.һ��ǿ�ȵ�ѹ������;
			filesys_press();
			break;
		case '2':
			cls_show_msg1(1, "�ļ�ϵͳ���ܲ�����...\n");// 2.���ܲ���
			filesys_ability();
			break;
		case '3':
#if !(ANDIROD_ENABLE||ME15C)//��֧��ȡ�����̿ռ亯���Ĳ�Ʒ�����쳣����defined IM81||defined N900||
			cls_show_msg1(1, "�ļ�ϵͳ�쳣������...\n");
			filesys_abnormity();
#else
			cls_show_msg1(1, "�޻�ȡ�����̿ռ�ĺ������ʲ����쳣����\n");
#endif
			break;
		case '4':
#if !(ANDIROD_ENABLE||ME15C)//��֧��ȡ�����̿ռ亯���Ĳ�Ʒ�����쳣����defined IM81||defined N900||		
			if(NDK_FsExist(FILETESTG8)==NDK_OK)
			{	
#if !K21_ENABLE			
				sync();sleep(3);
#endif
				NDK_FsGetDiskSpace(1,&pulSpace);
				cls_show_msg("��ǰ������ʣ��ռ�:%ld", pulSpace);	
				cls_show_msg1(2, "���Ի�������%s!",  NDK_FsDel(FILETESTG8)==NDK_OK?"�ɹ�":"ʧ��");
			}
			else
				cls_show_msg1(2, "δ��⵽�����ļ�");
#if !K21_ENABLE			
			sync();sleep(3);
#endif
			NDK_FsGetDiskSpace(1,&pulSpace);
			cls_show_msg("��ǰ������ʣ��ռ�:%ld", pulSpace);
#elif ANDIROD_ENABLE//!defined ME15C	//��֧��NDK_FsClearAppFile�����ĵͶ˲�Ʒ��Ҫ�򿪱�����ʹ��NDK_FsFormat����������
			cls_show_msg("���������ʼ��ʽ��");	
			NDK_FsFormat();//�߶˲�Ʒ��k21����ֱ���ø�ʽ������������
#else //�����Ͷ˲�Ʒ��������и�ʽ��
			cls_show_msg1(2, "���Ի�������%s!",  NDK_FsClearAppFile()==NDK_OK?"�ɹ�":"ʧ��");//�����л����ܵĵͶ˲�Ʒ��֧��NDK_FsFormat
#endif
			break;
		case ESC:
			return;
			break;
		default:
			continue;
			break;
		}
	}
	}
	return;
}


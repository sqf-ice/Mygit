/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����
* file name		: 96.c
* Author 			: linying
* version			: V1.0
* DATE			: 20180828
* directory 		: 
* description		: �߶��ļ�ϵͳ�ۺϲ���
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
#define	TESTITEM	"K21_fs����,ѹ��"
#define	FILETESTG8 "/appfs/FTG8"
#define	BUFFERSIZE	(1024)
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
*					       linying			20180828   	created
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
	NDK_SP_FsDel(FILETESTG8);//��������

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
			
		if ((fp=NDK_SP_FsOpen(FILETESTG8, "w"))<0)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:%s��%d��:�����ļ�ʧ��(%d)", __LINE__,TESTITEM, bak-cnt, fp);
			continue;//return; 20171106�޸�:ִ����ԱҪ��ͳ�Ʋ��Գɹ���
			
		}
		if ((writelen= NDK_SP_FsWrite(fp,writebuf2, BUFLEN))!=BUFLEN)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:д�ļ�ʧ��(%d)", __LINE__, bak-cnt,writelen);
			NDK_SP_FsClose(fp);
			NDK_SP_FsDel(FILETESTG8);
			continue;//return;
		}
		if ((ret=NDK_SP_FsClose(fp))!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:�ر��ļ�ʧ��(%d)", __LINE__, bak-cnt, ret);
			NDK_SP_FsDel(FILETESTG8);
			continue;//return;
		}
		if ((fp=NDK_SP_FsOpen(FILETESTG8, "r"))<0)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:���ļ�ʧ��(%d)", __LINE__, bak-cnt, fp);
			NDK_SP_FsDel(FILETESTG8);
			continue;//return;
		}
		memset(readbuf2, 0, sizeof(readbuf2));
		if ((readlen=NDK_SP_FsRead(fp,readbuf2, BUFLEN))!=BUFLEN)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:���ļ�ʧ��(%d)", __LINE__, bak-cnt, readlen);
			NDK_SP_FsClose(fp);
			NDK_SP_FsDel(FILETESTG8);
			continue;//return;
		}
		if (memcmp(writebuf2, readbuf2, sizeof(writebuf2)))
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:У���", __LINE__, bak-cnt);
			NDK_SP_FsClose(fp);
			NDK_SP_FsDel(FILETESTG8);
			continue;//return;
		}		
		if ((ret=NDK_SP_FsClose(fp))!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:�ر��ļ�ʧ��(%d)", __LINE__, bak-cnt, ret);
			NDK_SP_FsDel(FILETESTG8);
			continue;//return;
		}
		if ((ret=NDK_SP_FsDel(FILETESTG8))!=NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d��:ɾ���ļ�ʧ��(%d)", __LINE__, bak-cnt, ret);
			NDK_SP_FsDel(FILETESTG8);
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
#define BLOCK_SIZE 4

	//����ÿ����Ʒ�ĵ����ļ�����С(��׿ƽ̨δ֪)
#define MAX_FLIE_SIZE 240


	/*process body*/
	//  step	(1) �߶˲�Ʒ��֧��ȡ�ļ��ռ�
	cnt = MAX_FLIE_SIZE;
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
	NDK_SP_FsDel(FILETESTG8);
	
	// step	(2) ��"w"��ʽ��һ�ض��ļ�
	if ((fp= NDK_SP_FsOpen(FILETESTG8, "w"))<0)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:���������ļ�ʧ��", __LINE__);
		goto ERR;
	}

	// step	(3) д��240KB����(�����)�����ļ���
	// step	(4) ѭ��������2,���������ļ�
	memset(writebuf, rand()/256/*0x24*/, sizeof(writebuf));
	oldtime = time(NULL);
	lib_StartStopwatch();
	for (loop=0; loop<cnt; loop++)
	{
		if ((writelen=NDK_SP_FsWrite(fp,writebuf, sizeof(writebuf)))!=sizeof(writebuf))
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
	if (NDK_SP_FsClose(fp)!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME, "line %d:�ر��ļ�����", __LINE__);
		goto ERR;
	}

	// step	(6) ��"r"��ʽ�򿪸��ļ�
	if ((fp= NDK_SP_FsOpen(FILETESTG8, "r"))<0)
	{
		cls_show_msg_record(FILENAME,FUNCNAME, "line %d:�򿪳����ļ�ʧ��", __LINE__);
		goto ERR;
	}

	// step	(7) ȡ���ļ�����
	NDK_SP_FsFileSize(FILETESTG8,&filelen);
	if (filelen<filelen1)
	{
		cls_show_msg_record(FILENAME,FUNCNAME, "line %d:�ļ������쳣(%d,%d)", __LINE__, filelen, filelen1);
		goto ERR;
	}

	// step	(8) �����ļ����ݲ�����У��
	for (loop=0; loop<cnt; loop++)
	{
		memset(readbuf, 0, sizeof(readbuf));
		if ((readlen=NDK_SP_FsRead(fp,readbuf, sizeof(readbuf)))!=sizeof(readbuf))
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
		if ((readlen=NDK_SP_FsRead(fp,readbuf, filelen-filelen1))!=(filelen-filelen1))
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
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(FILETESTG8);

	// step	(10)�߶˲�Ʒ��֧��ȡ�ļ�ʣ��ռ�
	ftos(fTimeElapsed,str);
	cls_show_msg_record(FILENAME,FUNCNAME, "�����ļ�����ͨ��(������СΪ%dKB�ļ���ʱ%ss)", (cnt*sizeof(writebuf)+writelen)/1024, str);
	return;
ERR:
	NDK_SP_FsClose(fp);
	NDK_SP_FsDel(FILETESTG8);
	return;
}

static void filesys_press_multifiles(void)
{
	/*private & local definition*/
	int fp[5] ={-1, -1, -1, -1, -1}, ret = -1;
	int countnum = 0, i = 0, j = 0, cnt = 0/*, filelen[5] = {0}*/;
	int  writelen = 0, readlen = 0;
	char writebuf[BUFLEN] = {0}, readbuf[BUFLEN] = {0}, tempbuf[BUFLEN] = {0};
	char filename[5][20] ={"/appfs/FTG8A", "/appfs/FTG8B", "/appfs/FTG8C", "/appfs/FTG8D", "/appfs/FTG8E"};
	ulong discspace1 = 0;

	/*process body*/
	cls_printf("���ļ�������������...");
	cnt = 60;//Ϊ���Ч�ʽ�cnt��С��60
	
	// step (1)
	for (countnum = 0; countnum<5; countnum++)
		NDK_SP_FsDel(filename[countnum]);
	
	// step (2) ��"w"��ʽ������5���ļ�:FT21A,FT21B,FT21C,FT21D,FT21E,�ֱ�д��5K�ֽڵ�����(����ļ���д���������ݸ�����ͬ)
	for (countnum = 0; countnum<5; countnum++)
	{
		if ((fp[countnum] =  NDK_SP_FsOpen(filename[countnum], "w"))<0) // �����ļ�
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:�����ļ�(%d)ʧ��", __LINE__, countnum);
			for (i = countnum; i >= 0; i--)
			{
				NDK_SP_FsClose(fp[i]);
				NDK_SP_FsDel(filename[i]);
			}
			return;
		}

		memset(writebuf, countnum, sizeof(writebuf));
		for (j = 0; j < cnt; j++)
		{
			if ((writelen =  NDK_SP_FsWrite(fp[countnum],writebuf, BUFLEN))!=BUFLEN) // д�����ݳ��ȳ���
			{
				cls_show_msg_record(FILENAME,FUNCNAME, "line %d:д��%d���ļ��ĵ�%dK���ݳ���(%d)", __LINE__, countnum+1, j+1, writelen);
				for (i = countnum; i >= 0; i--)
				{
					NDK_SP_FsClose(fp[i]);
					NDK_SP_FsDel(filename[i]);
				}
				return;
			}
		}
	}
	//��Ը߶˲�Ʒ�����bug�����˼���ļ��Ƿ����20171113(ȫ��ƽ̨ͨ��)
	for (countnum = 0; countnum<5; countnum++)
	{
		if((ret=NDK_SP_FsExist(filename[countnum])) != NDK_OK)
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:����%d���ļ��Ƿ���ڳ���(%d)", __LINE__, countnum+1, ret);
			for (i = 0; i<5; i++)
			{
				NDK_SP_FsClose(fp[i]);
				NDK_SP_FsDel(filename[i]);
			}
			return;
		}
	}

	// step (3) �ر�������ļ�
	for (countnum = 0; countnum<5; countnum++)
		NDK_SP_FsClose(fp[countnum]);

	// step (4) ��"r"��ʽ��������ļ����ֱ��������У��
	for (countnum = 0; countnum<5; countnum++)
	{
		if ((fp[countnum] = NDK_SP_FsOpen(filename[countnum], "r"))<0) // ���ļ�ʧ��
		{
			cls_show_msg_record(FILENAME,FUNCNAME, "line %d:���ļ�(%d)ʧ��", __LINE__, countnum);
			for (i = countnum; i >= 0; i--)
			{
				NDK_SP_FsClose(fp[i]);
				NDK_SP_FsDel(filename[i]);
			}
			return;
		}

		for (j = 0; j < cnt; j++)
		{
			memset(readbuf, 0, sizeof(readbuf));
			if ((readlen =  NDK_SP_FsRead(fp[countnum],readbuf, BUFLEN))!=BUFLEN) // �������ݳ��ȳ��� 
			{
				cls_show_msg_record(FILENAME,FUNCNAME, "line %d:����%d���ļ��ĵ�%dK���ݳ���(%d)", __LINE__, countnum+1, j+1, readlen);
				for (i = countnum; i >= 0; i--)
				{
					NDK_SP_FsClose(fp[i]);
					NDK_SP_FsDel(filename[i]);
				}
				return;
			}

			memset(tempbuf, countnum, sizeof(tempbuf));
			if (memcmp(tempbuf, readbuf, readlen)) // У������
			{
				cls_show_msg_record(FILENAME,FUNCNAME, "line %d:��%d���ļ�У�����", __LINE__, countnum+1);
				for (i = 0; i<5; i--)
				{
					NDK_SP_FsClose(fp[i]);
					NDK_SP_FsDel(filename[i]);
				}
				return;
			}
		}
		
		//ɾ��ĳ���ļ���,��ȡʣ���ļ��������ݱȽ�,����Ӧ�ò��ᱻ�ı�
		if(countnum%2==1)//��FTG8B��FTG8D�ļ�ɾ�����ȡʣ���ļ����бȽ�
		{
			NDK_SP_FsClose(fp[countnum]);
			NDK_SP_FsDel(filename[countnum]);
		}
	}

	for (i = 0; i<5; i++)
	{
		NDK_SP_FsClose(fp[i]);
		NDK_SP_FsDel(filename[i]);
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
	char str[32] = {0};
	int times = 255;  
	
	/*process body*/
	
	//��д1M���ݺ�ʱ����
	NDK_SP_FsDel(FILETESTG8);//��������
	memset(writebuf2, rand()/256/*0x44*/, sizeof(writebuf2));
	fp =  NDK_SP_FsOpen(FILETESTG8, "w");

	cls_printf("д�ļ���...  ");
	lib_StartStopwatch();
	//for(j=0; j<1024; j++)//ѭ��д1K����1K*10240��
	for(j=0; j<times; j++)
	{
		if ((writelen= NDK_SP_FsWrite(fp,writebuf2, BUFLEN))!=BUFLEN)//�����ѹ��������ͬ,�������ж�һ�·���
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:д�ļ�ʧ��(%d,%d)", __LINE__,j, writelen);
			NDK_SP_FsClose(fp);
			NDK_SP_FsDel(FILETESTG8);
			return;
		}
	}
	memset(str,0,sizeof(str));
	ftos(times/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME, "д�ļ��ٶ�Ϊ%skb/s", str);
	if ((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:�ر��ļ�ʧ��(%d)", __LINE__, ret);
		NDK_SP_FsDel(FILETESTG8);
		return;
	}
	if ((fp=NDK_SP_FsOpen(FILETESTG8, "r"))<0)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:���ļ�ʧ��(%d)", __LINE__, fp);
		NDK_SP_FsDel(FILETESTG8);
		return;
	}

	cls_printf("���ļ���...  ");
	memset(readbuf2, 0, sizeof(readbuf2));
	lib_StartStopwatch();
	for(j=0; j<times; j++)//��255K������
	{
		if ((readlen= NDK_SP_FsRead(fp,readbuf2, BUFLEN))!=BUFLEN)
		{
			cls_show_msg_record(FILENAME,FUNCNAME,"line %d:���ļ�ʧ��(%d)", __LINE__, readlen);
			NDK_SP_FsClose(fp);
			NDK_SP_FsDel(FILETESTG8);
			return;
		}
	}
	memset(str,0,sizeof(str));
	ftos(times/lib_ReadStopwatch(),str);
	cls_show_msg_record(FILENAME,FUNCNAME, "���ļ��ٶ�Ϊ%skb/s", str);
	if ((ret=NDK_SP_FsClose(fp))!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:�ر��ļ�ʧ��(%d)", __LINE__, ret);
		NDK_SP_FsDel(FILETESTG8);
		return;
	}
	if ((ret=NDK_SP_FsDel(FILETESTG8))!=NDK_OK)
	{
		cls_show_msg_record(FILENAME,FUNCNAME,"line %d:ɾ���ļ�ʧ��(%d)", __LINE__, ret);
		NDK_SP_FsDel(FILETESTG8);
		return;
	}
	
	return;
}

void systest96(void)
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
							"2.���ܲ���\n");
              					//"3.�쳣����\n"�߶˲�Ʒ��֧��ȡʣ��ռ䣬�����쳣����
							//"4.������\n");
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


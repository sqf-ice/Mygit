/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: systest
* file name			: systest67.c
* Author 			: 
* version			: 
* DATE				: 
* directory 			: 
* description			: ���߳���
* related document	: NDK.chm
*
************************************************************************
* log			: 
* Revision 
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"
#include <pthread.h>
#include "libconfig.h"

#define  MAXWAITTIME  200
#if defined SP630PG || defined SP830PG||defined SP930P
#define  FILEPATH "/app/data"
#else
#define  FILEPATH "/appfs/etc/sys.conf"
#endif
/*---------------constants/macro definition---------------------*/
#define	TESTITEM		"���߳���"
typedef struct __my_thread
{
	char *thread_name;
	int thread_flag;
	int thread_exit;
}MYPTHREAD;

#define PTHREAD_NUM 6
int g_thread_exit = 0;
/*------------global variables definition-----------------------*/

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
*			  			jiangym 			20140619     created
*****************************************************************/
//kbhit�߳� ��ȡ��ֵ,����Ϊ1�� 9,��ֵӦ��Ϊ0x31��0x39,������������߱���
static void *pthread_hit(void *arg)
{
	int nKeyin = 0;
	//g_thread_exit ��1ʱ �߳��˳�
	while(g_thread_exit == 0)
	{	
		NDK_KbHit(&nKeyin);
		if(nKeyin!=0)
		{
			//Ϊ��ȷ��ȷʵ���߳�����
			fprintf(stderr,"line:[%d]thread_hit is runing\n",__LINE__);
			if(nKeyin!=0x31 && nKeyin!=0x39)
			{
				//����̨��������Ϣ
				fprintf(stderr,"line:%d ��ȡ��ֵʧ��(%02x)\n",__LINE__,nKeyin);
				//pos����ʾ������Ϣ
			 	cls_show_msg1(2,"line:%d ��ȡ��ֵʧ��(%02x)",__LINE__,nKeyin);
				sleep(3);
				g_thread_exit=1;
			}	
		}
	}
	//�˳��߳�
	return (void *)0;
}
//NDK_KbGetCode��NDK_KbGetPad���Ѹ��ֳ�
//�߳� ��ȡ��ֵ,����Ϊ1�� 9,��ֵӦ��Ϊ0x31��0x39,������������߱���
static void *pthread_getcode(void *arg)
{
	int nKeyin = 0;
	//g_thread_exit ��1ʱ �߳��˳�
	while(g_thread_exit == 0)
	{	
		NDK_KbGetCode(1, &nKeyin);
		if(nKeyin!=0)
		{
			fprintf(stderr,"line:[%d]pthread_getcode is runing\n",__LINE__);
			if(nKeyin!=0x31 && nKeyin!=0x39)
			{
				fprintf(stderr,"line:%d ��ȡ��ֵʧ��(%02x)\n",__LINE__,nKeyin);
			 	cls_show_msg1(2,"line:%d ��ȡ��ֵʧ��(%02x)",__LINE__,nKeyin);
				sleep(3);
				g_thread_exit=1;
			}	
		}
	}
	//�˳��߳�
	return (void *)0;
}
//�߳� ��ȡ��ֵ,����Ϊ1�� 9,��ֵӦ��Ϊ0x31��0x39,������������߱���
static void *pthread_getpad(void *arg)
{
	ST_PADDATA pad_data;
	
	//g_thread_exit ��1ʱ �߳��˳�
	while(g_thread_exit == 0)
	{	
		NDK_KbGetPad(&pad_data,1);
		if(pad_data.emPadState== PADSTATE_KEY)
		{
			if(pad_data.unKeycode!=0x31 && pad_data.unKeycode!=0x39)
			{
				fprintf(stderr,"line:%d ��ȡ��ֵʧ��(%02x)\n",__LINE__,pad_data.unKeycode);
				cls_show_msg1(2,"line:%d ��ȡ��ֵʧ��(%02x)",__LINE__,pad_data.unKeycode);
				sleep(3);
				g_thread_exit=1;
			}
		}
	}
	//�˳��߳�
	return (void *)0;
}

//����6��kbhit�߳�,pos����1��9,�߳��յ���᷵�ؼ�ֵ,��ֵ����Ԥ��ֵʱ����
//g_thread_exitΪȫ�ֱ���,����1ʱȫ���߳��˳�,ͬʱ������Ҳ�˳�
static void kbhit_pthread(void)
{
	/*private & local definition*/	
	pthread_t tidp[PTHREAD_NUM];
	int error=-1, i=0;
	time_t oldtime = 0;
	g_thread_exit = 0;
	
	/*process body*/
	cls_show_msg("���Լ���NDK_KbHit���߳�,�������������1��9,���������...");
	//Ҫʹ��pthread_join���ȴ����߳̽���,��Ҫ�ڷǷ���������,pthread_create�ڶ�������ΪNULLʱ��Ĭ������,Ĭ�������а����Ƿ�������
	//����6�����߳�
	for(i=0;i<PTHREAD_NUM;i++)
	{
		if((error=NDK_PthreadCreate(&tidp[i],NULL,(void *)pthread_hit,NULL))!=0) 
		{
			cls_show_msg1(g_keeptime,"line %d:NDK_KbHit NDK_PthreadCreate fail(%d)\n", __LINE__, error);
			return;
		}
	}
	oldtime=time(NULL);
	while(1)
	{
		if(g_thread_exit==1)	
			break;
		//��ʱʱ�䵽,g_thread_exit��1,�߳�ȫ������,������Ҳ����
		if((time(NULL)-oldtime)>MAXWAITTIME)
		{
			g_thread_exit=1;
			cls_show_msg("����ͨ��,���������");
			break;
		}
	}
	//�ȴ����߳̽���
	for(i=0;i<PTHREAD_NUM;i++)
	{
		 NDK_PthreadJoin(tidp[i],NULL);
	}
	return;
}

//NDK_KbGetCode��NDK_KbGetPad���Ѹ��ֳ�
//����6��kbgetcode�߳�,pos������1��9,�߳��յ��󷵻ؼ�ֵ
//g_thread_exitΪȫ�ֱ���,����1ʱȫ���߳��˳�,ͬʱ������Ҳ�˳�
static void kbgetcode_pthread(void)
{
	/*private & local definition*/
	pthread_t tidp[PTHREAD_NUM];
	int error=-1, i=0;
	time_t oldtime = 0;
	g_thread_exit = 0;
	
	/*process body*/
	cls_show_msg("���Լ���NDK_KbGetCode���߳�,�������������1��9,���������...");
	//Ҫʹ��pthread_join���ȴ����߳̽���,��Ҫ�ڷǷ���������,pthread_create�ڶ�������ΪNULLʱ��Ĭ������,Ĭ�������а����Ƿ�������	
	//����6�����߳�
	for(i=0;i<PTHREAD_NUM;i++)
	{
		if((error=NDK_PthreadCreate(&tidp[i],NULL,(void *)pthread_getcode,NULL))!=0) 
		{
			cls_show_msg1(g_keeptime,"line %d:NDK_KbGetCode NDK_PthreadCreate fail(%d)\n", __LINE__, error);
			return;
		}
	}	
	oldtime=time(NULL);
	while(1)
	{
		if(g_thread_exit==1)	
			break;
		//��ʱʱ�䵽,g_thread_exit��1,�߳�ȫ������,������Ҳ����
		if((time(NULL)-oldtime)>MAXWAITTIME)
		{
			g_thread_exit=1;
			cls_show_msg("����ͨ��,���������");
			break;	
		}
	}
	//�ȴ����߳̽���
	for(i=0;i<PTHREAD_NUM;i++)
	{
		 NDK_PthreadJoin(tidp[i],NULL);
	}
	return;
}
//����6��kbgetpad�߳�,pos������1��9,�߳��յ��󷵻ؼ�ֵ
//g_thread_exitΪȫ�ֱ���,����1ʱȫ���߳��˳�,ͬʱ������Ҳ�˳�
static void kbgetpad_pthread(void)
{
	/*private & local definition*/
	pthread_t tidp[PTHREAD_NUM];
	int error=-1, i=0;
	time_t oldtime = 0;
	g_thread_exit = 0;
	
	/*process body*/
	cls_show_msg("���Լ���NDK_KbGetPad���߳�,�������������1��9,���������...");
	//Ҫʹ��pthread_join���ȴ����߳̽���,��Ҫ�ڷǷ���������,pthread_create�ڶ�������ΪNULLʱ��Ĭ������,Ĭ�������а����Ƿ�������
	//����6�����߳�
	for(i=0;i<PTHREAD_NUM;i++)
	{
		if((error=NDK_PthreadCreate(&tidp[i],NULL,(void *)pthread_getpad,NULL))!=0) 
		{
			cls_show_msg1(g_keeptime,"line %d:NDK_KbGetPad NDK_PthreadCreate fail(%d)\n", __LINE__, error);
			return;
		}
	}	
	oldtime=time(NULL);
	while(1)
	{
		if(g_thread_exit==1)
			break;
		//��ʱʱ�䵽,g_thread_exit��1,�߳�ȫ������,������Ҳ����	
		if((time(NULL)-oldtime)>MAXWAITTIME)
		{
			g_thread_exit=1;
			cls_show_msg("����ͨ��,���������");
			break;	
		}
	}
	//�ȴ����߳̽���
	for(i=0;i<PTHREAD_NUM;i++)
	{
		 NDK_PthreadJoin(tidp[i],NULL);
	}
	return;
}


//���õײ㺯�����������ļ��Ķ�����
void *pthread_configread(void *arg)
{
	int tmp_language = -1;
	int i= 0,j=0,s = 0;
	MYPTHREAD *tmpphread = NULL;
	tmpphread=(MYPTHREAD *)arg;
	int old_value=0;
	config_t tmp_cfg;
	config_setting_t *dev_class;	
	config_setting_t *tmp_root_setting;
	
	//thread_exit��1ʱ,�߳��˳�	
	while(tmpphread->thread_exit==0)
	{
		tmp_language = -1;
		config_init(&tmp_cfg);
		if (config_read_file(&tmp_cfg, FILEPATH) == CONFIG_FALSE)
		{
			fprintf(stderr,"[%d] %s errs=[%s]\n",__LINE__,tmpphread->thread_name,config_error_text(&tmp_cfg));
			tmpphread->thread_flag=1;
			break;
		} 
		tmp_root_setting = config_root_setting(&tmp_cfg);
		if (tmp_root_setting == NULL)
		{
			fprintf(stderr,"[%d] %s errs=[%s]\n",__LINE__,tmpphread->thread_name,config_error_text(&tmp_cfg));
			tmpphread->thread_flag=1;
    		break;
		}
		//��ѯ�����ļ�,�鵽����ΪNULLʱ����
		dev_class = config_lookup(&tmp_cfg, "sys");	   		
		if(dev_class==NULL){
			s++;
			cls_show_msg("line %d:%s�ļ�����ΪNULL,���������",__LINE__,tmpphread->thread_name);
			tmpphread->thread_flag=1;
			break;   			
		}
		if (config_setting_lookup_int(dev_class, "language", (int *)&tmp_language) != CONFIG_TRUE)
		{
			fprintf(stderr,"[%d] %s errs=[%s]\n",__LINE__,tmpphread->thread_name,config_error_text(&tmp_cfg));
			tmpphread->thread_flag=1;
			break;
		}
		old_value=tmp_language;
		NDK_SysMsDelay(10);
		i++;
		if(i%100==0)
			fprintf(stderr,"thread_name=[%s]time:[%ds]\n",tmpphread->thread_name,(j++));
		if(old_value!=tmp_language)
		{
	    	fprintf(stderr,"thread_name=[%s]time:[%ds]value=[%d]\n",tmpphread->thread_name,(j),tmp_language);
	    	old_value=tmp_language;
		}
		config_destroy(&tmp_cfg);
	}
	config_destroy(&tmp_cfg);
	//�˳��߳�
	return (void *)0;
}
//���õײ㺯�����������ļ���д����
void *pthread_configwrite(void *arg)
{
	int language_int = 0;
	MYPTHREAD *tmpphread = NULL;
	tmpphread=(MYPTHREAD *)arg;
	config_t tmp_cfg;
	config_setting_t *dev_class;
	config_setting_t *tmp_root_setting;
	
	//thread_exit��1ʱ,�߳��˳�
	while(tmpphread->thread_exit==0)
	{
		config_init(&tmp_cfg);
		if (config_read_file(&tmp_cfg, FILEPATH) == CONFIG_FALSE) {
			fprintf(stderr,"[%d] %s errs=[%s]\n",__LINE__,tmpphread->thread_name,config_error_text(&tmp_cfg));
			tmpphread->thread_flag=1;
			break;
		}
		tmp_root_setting = config_root_setting(&tmp_cfg);
	    if (tmp_root_setting == NULL) {
			fprintf(stderr,"[%d] %s errs=[%s]\n",__LINE__,tmpphread->thread_name,config_error_text(&tmp_cfg));
			tmpphread->thread_flag=1;
			break;
	    }
	    dev_class = config_lookup(&tmp_cfg, "sys");
	    if(dev_class==NULL){
			cls_show_msg("line %d:%s�ļ�����ΪNULL,���������",__LINE__,tmpphread->thread_name);
			tmpphread->thread_flag=1;
			break;
	    }
	    if(dev_class) {
	        dev_class = config_setting_get_member(dev_class, "language");
	        if (dev_class){
	        	if (config_setting_set_int(dev_class, language_int) != CONFIG_TRUE) {
	    			fprintf(stderr,"[%d] %s errs=[%s]\n",__LINE__,tmpphread->thread_name,config_error_text(&tmp_cfg));
	    			tmpphread->thread_flag=1;
	    			break;
	             }
	        }
	        else{
				fprintf(stderr,"[%d] %s errs=[%s]\n",__LINE__,tmpphread->thread_name,config_error_text(&tmp_cfg));
				tmpphread->thread_flag=1;
				break;
	        }
	    }
	    if (! config_write_file(&tmp_cfg, FILEPATH)) {
			fprintf(stderr,"[%d] %s errs=[%s]\n",__LINE__,tmpphread->thread_name,config_error_text(&tmp_cfg));
			tmpphread->thread_flag=1;
			break;
	    }
		fprintf(stderr,"[%d] %s write value=%d\n",__LINE__,tmpphread->thread_name,language_int);
	    config_destroy(&tmp_cfg);
		NDK_SysMsDelay(10);
	}
	config_destroy(&tmp_cfg);
	//�˳��߳�
	return (void *)0;
}
//����NDK����ʵ�������ļ���д����
/*void *pthcreate_wr(void *arg)
{
	int ret = -1, i = 0;
	MYPTHREAD *tmpphread = NULL;
	tmpphread=(MYPTHREAD *)arg;

	while(1)
	{
		if(tmpphread->thread_flag)
		{
			NDK_ScrSetContrast(32);
			break;	
		}
		if((ret=NDK_ScrSetContrast((i++)%63))!=NDK_OK)
		{
			cls_show_msg("line %d:���öԱȶ�ʧ��(%d,%d)", __LINE__, ret, i);
			break;
		}		
		NDK_SysMsDelay(50);
	}
	
	//�˳��߳�	 
	pthread_exit(NULL);	
	
}*/
//��д�߳�ͬʱ����
void libconfigrw_test()
{	
	/*private & local definition*/
	pthread_t tidp,tidp1;
    int error=-1, error1=-1;
	pthread_attr_t attr;
	MYPTHREAD *myphreadone,*myphreadtwo;
	time_t oldtime;
	
	/*process body*/
	cls_printf("���������ļ����߳�ͬʱ��д�Ƿ��ͻ");	
	//���䶯̬�ڴ�
    myphreadone=(MYPTHREAD *)malloc(sizeof(MYPTHREAD));
	myphreadtwo=(MYPTHREAD *)malloc(sizeof(MYPTHREAD));
	//Ԥ���ڴ�й©
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	//��ʼ���̵߳���������������߳�
	myphreadone->thread_name="Pthread read";
	myphreadone->thread_flag=0;
	myphreadone->thread_exit=0;
	if((error=NDK_PthreadCreate(&tidp,&attr,pthread_configread,(void *)myphreadone))!=0)
	{
		cls_show_msg1(g_keeptime,"line %d:����%s�߳�ʧ��(%d)", __LINE__,myphreadone->thread_name, error);
		return;
	}
	//��ʼ���̵߳�������� ����д�߳�
	myphreadtwo->thread_name="Pthread write";
	myphreadtwo->thread_flag=0;
	myphreadtwo->thread_exit=0;
	if((error1=NDK_PthreadCreate(&tidp1,&attr,pthread_configwrite,(void *)myphreadtwo))!=0)
	{
		cls_show_msg1(g_keeptime,"line %d:����%s�߳�ʧ��(%d)", __LINE__, myphreadtwo->thread_name, error1);
		return;
	}
	oldtime=time(NULL);
	//��ĳ���߳��е�thread_flag��1ʱ,��һ���̵߳�thread_exit��1 , �˳��߳�,ͬʱ���߳�Ҳ�˳�
	while(1)
	{
		if(myphreadone->thread_flag==1||myphreadtwo->thread_flag==1)
		{
			//�˴������ж����ĸ��̳߳���,Ȼ�����һ���̵߳��˳���־λ��1,ͬʱ�ȵ���һ���߳̽����������̽���
			if(myphreadone->thread_flag==1)
			{
				myphreadtwo->thread_exit=1;
				cls_show_msg1(g_keeptime,"line %d:%sʧ��,���������",__LINE__,myphreadone->thread_name);
				NDK_PthreadJoin(tidp1,NULL);//�ȴ���һ���߳̽���
				break;
			}
			else if(myphreadtwo->thread_flag==1)
			{
				myphreadone->thread_exit=1;
				cls_show_msg1(g_keeptime,"line %d:%sʧ��,���������",__LINE__,myphreadtwo->thread_name);
				NDK_PthreadJoin(tidp,NULL);
				break;
			}
		}
		//��ʱ���߳��˳���־λΪ1,ͬʱ������Ҳ�˳�
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			myphreadone->thread_exit=1;
			myphreadtwo->thread_exit=1;
			break;
		}
	}
	//�ͷ��ڴ�
	cls_show_msg_record(FILENAME ,FUNCNAME ,"��д�߳�ͬʱ���в���ͨ��");
	free(myphreadone);
	free(myphreadtwo);
	return;
}

//дд�߳�ͬʱ����
void libconfigww_test()
{
	pthread_t tidp,tidp1;
	int error=-1,error1=-1;
	pthread_attr_t attr;
	MYPTHREAD *myphreadone,*myphreadtwo;
	time_t oldtime;
	
	cls_printf("���������ļ����߳�ͬʱдд�Ƿ��ͻ");
	//��̬�����ڴ�
	myphreadone=(MYPTHREAD *)malloc(sizeof(MYPTHREAD));
	myphreadtwo=(MYPTHREAD *)malloc(sizeof(MYPTHREAD));
	//��ֹ�ڴ�й©
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	//��ʼ���̵߳�������� ������һ��д�߳�
	myphreadone->thread_name="Pthread write 1";
	myphreadone->thread_flag=0;
	myphreadone->thread_exit=0;
	if((error=NDK_PthreadCreate(&tidp,&attr,pthread_configwrite,(void *)myphreadone))!=0)
	{
		cls_show_msg1(g_keeptime,"line %d:�����߳�ʧ��(%d)", __LINE__, error);
		return;
	}
	//��ʼ���̵߳��������,�����ڶ���д�߳�
	myphreadtwo->thread_name="Pthread write 2";
	myphreadtwo->thread_flag=0;
	myphreadtwo->thread_exit=0;
	if((error1=NDK_PthreadCreate(&tidp1,&attr,pthread_configwrite,(void *)myphreadtwo))!=0)
	{
		cls_show_msg1(g_keeptime,"line %d:�����߳�ʧ��(%d)", __LINE__, error1);
		return;
	}
	oldtime=time(NULL);
	//��ĳ���߳��е�thread_flag��1ʱ,��һ���̵߳�thread_exit��1 , �˳��߳�,ͬʱ���߳�Ҳ�˳�
	while(1)
	{
		if(myphreadone->thread_flag==1||myphreadtwo->thread_flag==1)
		{
			//�˴������ж����ĸ��̳߳���,Ȼ�����һ���̵߳��˳���־λ��1,ͬʱ�ȵ���һ���߳̽����������̽���			
			if(myphreadone->thread_flag==1)
			{
				myphreadtwo->thread_exit=1;
				cls_show_msg1(g_keeptime,"line %d:%sʧ��,���������",__LINE__,myphreadone->thread_name);				
				NDK_PthreadJoin(tidp1,NULL);//�ȴ���һ���߳̽���
				break;
			}
			else if(myphreadtwo->thread_flag==1)
			{
				myphreadone->thread_exit=1;
				cls_show_msg1(g_keeptime,"line %d:%sʧ��,���������",__LINE__,myphreadtwo->thread_name);
				NDK_PthreadJoin(tidp,NULL);
				break;
			}
		}
		//��ʱ���߳��˳���־λΪ1,ͬʱ������Ҳ�˳�
		if(time(NULL)-oldtime>MAXWAITTIME)
		{
			myphreadone->thread_exit=1;
			myphreadtwo->thread_exit=1;
			break;
		}
	}
	//�ͷ��ڴ�
	cls_show_msg_record(FILENAME ,FUNCNAME ,"дд�߳�ͬʱ���в���ͨ��");
	free(myphreadone);
	free(myphreadtwo);
	return;
}

//�����ຯ�������������Ķ��̲߳��Ե�����ѡ��
void kb_pthread(void)
{
	int keyin = 0;
	while (1)
	{	
		keyin = cls_show_msg("1.KbHit���߳�\n"
							 "2.KbGetCode���߳�\n"
							 "3.KbGetPad���߳�");
		switch(keyin)
		{
		case '1':
			kbhit_pthread();
			break;
		case '2':
			kbgetcode_pthread();
			break;
		case '3':
			kbgetpad_pthread();
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
//�����ļ����̵߳�ѡ��,�ļ���ͬʱ��д��ͬʱдд
void configfile_pthread(void)
{
	int keyin = '0';
	while (1)
	{	
		if(g_SequencePressFlag)  //�ж�ͳһģ�����Ƿ���������ѹ������
		{
			//��Ӧ����Ĳ˵���+1���˵����Ӵ˴�ҲҪ��Ӧ������
			if((++keyin == '3')||(cls_show_msg1(2, "�������������ļ����̲߳���,ESC�˳�")==ESC))
				return ;
		}
		else
		{
			keyin = cls_show_msg("1.�ļ�ͬʱ��д\n"
								 "2.�ļ�ͬʱдд");
		}
		switch(keyin)
		{
		case '1':
			libconfigrw_test();
			break;
		case '2':
			libconfigww_test();
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
void systest66(void)
{
	/*private & local definition*/
	int keyin = 0;
	
	/*process body*/
	cls_show_msg1(1, "%s������...", TESTITEM);
	
	if(auto_flag==1)
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Կ�ʼ",TESTITEM);
		//ִ�������ļ����߳�
		configfile_pthread();

		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s���Խ���\n",TESTITEM);
		return;
	}
	while (1)
	{	
		keyin = cls_show_msg("1.���̺������߳�\n"
							 "2.�����ļ����߳�");

		switch(keyin)
		{
		case '1':
			kb_pthread();
			break;
		case '2':
			configfile_pthread();
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


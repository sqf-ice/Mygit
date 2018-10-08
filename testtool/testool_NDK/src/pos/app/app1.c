/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App1.c
* Author 			: dait
* version			: 
* DATE			: 20130228
* directory 		: 
* description		: ����NDK_AppLoad,NDK_AppGetReboot�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if K21_ENABLE&&!CPU5810X_ENABLE&&!defined ME50NX&&!defined ME68
#define  FILETEST1 "/appfs/test1.NLP"   //��ĸ����
#define  FILETEST2 "/appfs/�Լ�1.NLP"    //��������
#define  FILETEST3 "/appfs/k#~a.NLP"   //�����ַ� ֮ǰ��&���Ų�֧��
#define  FILETEST4 "/appfs/App_NULL.NLP" //�����ڵ�Ӧ��,������
#define  FILETEST5 "/appfs/ErrDg.NLP" 	//ǩ�������Ӧ��
#define  FILETEST6 "/appfs/shutdow.NLP" //���ڲ��ԵͶ˲�Ʒ��װӦ�ù����жϵ磬������Ҫ���Զ�������װ������ //Ŀǰ��31֧�֣��ұ������ò��Գ����Զ����иù��ܲ���Ч//�ĳ�11�ֽ�+������12�ֽ�20180810modify
#elif CPU5810X_ENABLE||defined ME50NX||defined ME68//defined SP610||defined ME50||defined ME50N||defined ME62
#define  FILETEST1 "test1.NLP"   //��ĸ����
#define  FILETEST2 "�Լ�1.NLP"    //��������
#define  FILETEST3 "k#~a.NLP"   //�����ַ� ֮ǰ��&���Ų�֧��
#define  FILETEST4 "App_NULL.NLP" //�����ڵ�Ӧ��,������
#define  FILETEST5 "ErrDg.NLP" 	//ǩ�������Ӧ��
#define  FILETEST6 "shutdow.NLP" //���ڲ��ԵͶ˲�Ʒ��װӦ�ù����жϵ磬������Ҫ���Զ�������װ������ //Ŀǰ��31֧�֣��ұ������ò��Գ����Զ����иù��ܲ���Ч//68������Ϊ12�ֽڻ����,��5810��Ʒ�ڳ�����������Ҳ��֧��12�ֽ�,�ʸĳ�11�ֽ�20180612 modify
#define  FILETEST7 "overboot.NLP"	//���ʱ.xml�ļ��е�<address>��ַС��44KB�Ĳ���nlp�ļ�,Ԥ������Ӧ��ʧ��
#if defined SP610
#define  FILETEST8 "ApppkS4.NLP"   //�����ַ0x0314C000(APPPK��ַ)��Ӧ��С��4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST9 "ApppkS8.NLP"  //�����ַ0x0314A000(DEBUGAPPPK��ַ),Ӧ�ô���4k��С��8k��Ԥ��Ӧװ��ʧ��
#define  FILETEST10 "FSigS4.NLP"  //�����ַ0x0314D000(Font24*24-SIG��ַ),Ӧ��С��4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST11 "SigB4.NLP"   //�����ַ0x0314B000(APPPK-SIG)��ַ��Ӧ�ô���4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST12 "SigS4.NLP"  //�����ַ0x0314B000(APPPK-SIG)��ַ��Ӧ��С��4k��Ԥ��Ӧ��װ��ʧ��
#endif
#elif OVERSEAS_ENABLE
//�����Ʒ7.0��ʽ�汾����֧��rootȨ�ޣ���֧�ֿ�����̨����֧�ֿ����棬Ϊ�˷��㣬���ڵ�ǰ·���£��Ҳ���rootȨ���������  20180420 sull modify
#define  FILETEST1 "typeAunMaster.NLD"  //������������ͨӦ��
#define  FILETEST2 "typeAMaster.NLD" 	//����������ͨӦ��
#define  FILETEST5 "typeI.NLD" 		//Ӧ��Ŀ¼�µĹ����ļ�����
#define  FILETEST6 "typeS_A.NLD" //A��������Ӧ�ò�����
#define  FILETEST9 "App_NULL.NLD" //�����ڵİ� 
#define  FILETEST10 "fw_patch_5.2.0.0.NLD" //�汾����ȷ�Ĺ̼������� ��Ҫ���д��,ǩ��Ҫ��ȷ
#define  FILETEST11 "fw_patch_5.3.1.0.NLD" //�汾��ȷ�� ��Ҫ���д��,ǩ��Ҫ��ȷ,�Ҳ���һ�κ���Ϊ�̼��汾������,������ʧЧ��Ҫ���´�������ٲ���
#define  FILETEST12 "test01234567890123456789012345678.NLD"
#define  FILESYS  "/appfs/fsys"   //����汾��Ҫʹ�þ���·��(�����ر�ɾ�����·��������)
#define  FILESYS1 "/appfs/fsys1"
#if !OVERSEAS_OS70_ENABLE
#define  FILETEST3 "typeRunMaster.NLD" 	//����������ϵͳӦ��
#define  FILETEST4 "typeRMaster.NLD" 	//��������ϵͳӦ��
#define  FILETEST7 "typeS_R_RMaster.NLD" //R��������Ӧ�ò�����
#define  FILETEST8 "typeS_R_RunMaster.NLD" //R���ͷ�����Ӧ�ò�����
#endif
#elif defined ME50S || defined ME50NS
#define  FILETEST1 "/appfs/apps/download/App_U_NonMaster.NLD"  //���ļ�����/appfs/download/tempĿ¼��
#define  FILETEST2 "/appfs/apps/download/App_U_Master.NLD"  //���ļ�����/appfs/download/tempĿ¼��
#define  FILETEST3 "/appfs/apps/download/App_A_NonMaster.NLD" 
#define  FILETEST4 "/appfs/apps/download/App_A_Master.NLD" 
#define  FILETEST5 "/appfs/apps/download/App_P.NLD" 
#define  FILETEST6 "/appfs/apps/download/App_L.NLD" 
#define  FILETEST7 "/appfs/apps/download/App_A_NonMaster_pri.NLD" //��Ӧ�ô򲹶���
#define  FILETEST8 "/appfs/apps/download/fw_patch_1.2.3.4_2013.NLD" //���̼��򲹶������汾����ȷ��
#define  FILETEST9 "/appfs/apps/download/App_ErrorDigist.NLD"
#define  FILETEST10 "/appfs/apps/download/App_NULL.NLD"
#define  FILETEST11 "/appfs/apps/download/NDK_AppEXECV.NLD"
#define  FILETEST12 "/appfs/apps/download/fw_patch_1.2.3.5_2013.NLD" //���̼��򲹶������������Ա���д��һ����ȷ�Ĺ̼�������������֤���̼��汾���ƺ�POS�ϵİ汾ǰ3λҪһ�£�
#define  FILETEST13 "/appfs/apps/download/a` $&();'a.NLD"//���ļ��е��ַ� Ŀǰ�޷�֧�֣������Ƿ���Ҫ
#define  FILETEST14 "/appfs/apps/download/a~!@#%^_=+,.[{]}��.NLD"
#define  FILETEST15 "/appfs/apps/download/Upt_CERT_TEST.NLD"
#define  FILETEST16 "/appfs/apps/download/test01234567890123456789012345678.NLD"
#define  FILESYS  "fsys"
#define  FILESYS1 "fsys1"
#else
#define  FILETEST1 "/appfs/download/App_U_NonMaster.NLD"  //���ļ�����/appfs/download/tempĿ¼��
#define  FILETEST2 "/appfs/download/App_U_Master.NLD"  //���ļ�����/appfs/download/tempĿ¼��
#define  FILETEST3 "/appfs/download/App_A_NonMaster.NLD" 
#define  FILETEST4 "/appfs/download/App_A_Master.NLD" 
#define  FILETEST5 "/appfs/download/App_P.NLD" 
#define  FILETEST6 "/appfs/download/App_L.NLD" 
#define  FILETEST7 "/appfs/download/App_A_NonMaster_pri.NLD" //��Ӧ�ô򲹶���
#define  FILETEST8 "/appfs/download/fw_patch_1.2.3.4_2013.NLD" //���̼��򲹶������汾����ȷ��
#define  FILETEST9 "/appfs/download/App_ErrorDigist.NLD"
#define  FILETEST10 "/appfs/download/App_NULL.NLD"
#define  FILETEST11 "/appfs/download/NDK_AppEXECV.NLD"
#define  FILETEST12 "/appfs/download/fw_patch_1.2.3.5_2013.NLD" //���̼��򲹶������������Ա���д��һ����ȷ�Ĺ̼�������������֤���̼��汾���ƺ�POS�ϵİ汾ǰ3λҪһ�£�
#define  FILETEST13 "/appfs/download/a` $&();'a.NLD"//���ļ��е��ַ� Ŀǰ�޷�֧�֣������Ƿ���Ҫ
#define  FILETEST14 "/appfs/download/a~!@#%^_=+,.[{]}��.NLD"
#define  FILETEST15 "/appfs/download/Upt_CERT_TEST.NLD"
#define  FILETEST16 "/appfs/download/test01234567890123456789012345678.NLD"
#define  FILESYS  "fsys"
#define  FILESYS1 "fsys1"
#endif
#if K21_ENABLE	
#define 	TESTAPI	  "NDK_AppLoad"
#else
#define 	TESTAPI	  "NDK_AppLoad,NDK_AppGetReboot"
#endif
/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : app1
* functional description : Ӧ�ù���ģ���1������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*
*****************************************************************/
#if K21_ENABLE
void app1(void)
{
	/*private & local definition*/
	int  ret = 0;
#if defined ME68 || defined ME50NX	
	int  fd = 0;
#endif
	/*uchar *appname[] = {
							(uchar *)"test1",
							(uchar *)"�Լ�1",
							(uchar *)"k#~a"
						};*/ //��ʱ����Ҫ�õ�,����������Ϣ

	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	if(cls_show_msg1(30, "����%s...��ȷ�������ļ�����,ESC�˳�", TESTAPI)==ESC)
		return;//��Ļ��ʾ��ǰ��������ģ��������

	//case5:����������֤�����԰�װ�����жϵ磬���������Զ���װǰ��ϵ��Ӧ�ð�
	if(cls_show_msg1(30, "�������Զ����к�,��ȷ�ϼ����밲װ�����жϵ���ԣ���������������", TESTAPI)==ENTER)
	{
		cls_show_msg("���������ʼ��װӦ��,���ڿ�ʼ��װ��2���ڶϵ磬�ϵ������Ӧ���ܹ��Զ����Ӧ�ó���İ�װ");
		//����ƽ̨�Ĺ��߲�֧�����ļ���ʽ����NLP�ļ�����Ϊ���߻�ʶ���ļ�ͷ����ͷΪScorpiop��ᱻ����ʶ�𣬹ʲ��ù��⽫ͷ�Ĵ����ô��뽫ͷ��Ϊ��ȷ����ʽ����NLP�ļ�
#if defined ME68 || defined ME50NX  
		if((fd = NDK_FsOpen(FILETEST6, "w")) <0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fd);
			RETURN;
		}
		if((ret = NDK_FsWrite(fd, "Scorpiop", 8)) != 8)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
		if((ret = NDK_FsClose(fd)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			RETURN;
		}
#endif
		cls_printf("��ϵ�");
		if(( ret = NDK_AppLoad((char *)FILETEST6,0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
			RETURN;
		}
	}
	
	//case1:����װ��
#if defined ME68 || defined ME50NX
	if((fd = NDK_FsOpen(FILETEST1, "w")) <0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fd);
		RETURN;
	}
	if((ret = NDK_FsWrite(fd, "Scorpiop", 8)) != 8)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_FsClose(fd)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif
	if(( ret = NDK_AppLoad((char *)FILETEST1,0))!=NDK_OK)//uchar�ĳ�char ��Ϊ���������� 
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
		RETURN;
	}
	
#if defined ME68 || defined ME50NX
	if((fd = NDK_FsOpen(FILETEST2, "w")) <0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fd);
		RETURN;
	}
	if((ret = NDK_FsWrite(fd, "Scorpiop", 8)) != 8)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_FsClose(fd)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif	
	if(( ret = NDK_AppLoad((char *)FILETEST2,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

#if defined ME68 || defined ME50NX
	if((fd = NDK_FsOpen(FILETEST3, "w")) <0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fd);
		RETURN;
	}
	if((ret = NDK_FsWrite(fd, "Scorpiop", 8)) != 8)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_FsClose(fd)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif
	if(( ret = NDK_AppLoad((char *)FILETEST3,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case2:�ļ�����������²���,����ʧ��
	if(( ret = NDK_AppLoad((char *)FILETEST4,0))!=NDK_ERR_OPEN_DEV)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	//case3:�ļ�����������²���,��װʧ��
	if(( ret = NDK_AppLoad(NULL,0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case4:�ļ�ǩ������ȷ,��װʧ�� 20141104 linwl added
#if defined ME68 || defined ME50NX
	if((fd = NDK_FsOpen(FILETEST5, "w")) <0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fd);
		RETURN;
	}
	if((ret = NDK_FsWrite(fd, "Scorpiop", 8)) != 8)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_FsClose(fd)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif
	if(( ret = NDK_AppLoad((char *)FILETEST5,0))!=NDK_ERR_APP_SIGN_DECRYPT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	//case5:�����ַ���ԣ�Ӧװ��ʧ��20180604 sull added
#if defined SP610
	if(( ret = NDK_AppLoad((char *)FILETEST8,0))!=NDK_ERR_APP_ADDRESS_NOT_LEGAL/*NDK_ERR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(( ret = NDK_AppLoad((char *)FILETEST9,0))!=NDK_ERR_APP_ADDRESS_NOT_LEGAL/*NDK_ERR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(( ret = NDK_AppLoad((char *)FILETEST10,0))!=NDK_ERR_APP_ADDRESS_NOT_LEGAL/*NDK_ERR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(( ret = NDK_AppLoad((char *)FILETEST11,0))!=NDK_ERR_APP_ADDRESS_NOT_LEGAL/*NDK_ERR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if(( ret = NDK_AppLoad((char *)FILETEST12,0))!=NDK_ERR_APP_ADDRESS_NOT_LEGAL/*NDK_ERR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

#if (CPU5810X_ENABLE&&!defined SP610) //defined ME50NX||defined ME68||defined ME50||defined ME50N||defined ME62  ����ƽ̨�ݲ��Ե�ַ������
	//case4:��ַС��44K��Ӧ�ð�,Ӧ�ð�װʧ��
	if(( ret = NDK_AppLoad((char *)FILETEST7,0))!=NDK_ERR)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

	//����ͨ��
	send_result("%s����ͨ��,����˳���������������������ز��Գ���", TESTAPI);//����FILETEST6 �ǰ�װ�ڲ��Գ���λ�ã����Բ��Գ���ᱻ�滻�������°�װ
/*ERR:
	for(i = 0;i<SZ_ARRAY(appname);i++)
		NDK_AppDel((char *)appname[i]);*/  //��֧��NDK_AppDel
	return;
}
#elif OVERSEAS_ENABLE
void app1(void)
{
	/*private & local definition*/
	int  ret = 0, i = 0;
	int nRebootFlag=-1;
	int fp = -1, fp2 = -1;
	uchar *appname[] = {
							(uchar *)"typeAunMaster",
							(uchar *)"typeAMaster",
							(uchar *)"typeI",
							(uchar *)"typeS_A",
#if !OVERSEAS_OS70_ENABLE  //7.0�汾��ʼ��֧��rootȨ�� Ӧ�ð� 20180420 modify
							(uchar *)"typeRunMaster", 
							(uchar *)"typeRMaster",
							(uchar *)"typeS_R_RMaster",
							(uchar *)"typeS_R_RunMaster",
#endif
						};
	
	struct tm stSetRtcTime;//����汾ʹ��rtcӲʱ���ж�֤�� 20180202
	struct tm stOldRtcTime;
	
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	if(cls_show_msg1(30, "����%s...��ȷ�������ļ�����,�ҵ�ǰ�û�����ΪrootȨ��,ESC�˳�", TESTAPI)==ESC)
		return;//��Ļ��ʾ��ǰ��������ģ��������

	//����ǰ��:ȡ��ǰʱ��Ϊ�����޸Ĳ���ʱ����׼��
	NDK_FsDel(FILESYS);
	NDK_FsDel(FILESYS1);
	if((ret=NDK_SysGetRtcTime(&stOldRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

#if !OVERSEAS_OS70_ENABLE //7.0�汾��ʼ��֧��rootȨ��  20180420 modify
	//case1:����װ��R���ͷ�����Ӧ��,Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST3,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case3:װ��R���ͷ�����Ӧ�õĲ�����Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST8,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//Ϊ��������Ӧ�ã���������Ӧ��Ҳ��������˲������Ƶ�����	
#endif
	
	//case5:װ��A��������Ӧ�õĲ�����Ӧ��ʧ�ܷ���NDK_ERR_APP_NOT_EXIST
	if(( ret = NDK_AppLoad((uchar *)FILETEST6,0))!=NDK_ERR_APP_NOT_EXIST)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case6:����װ��A���ͷ�����Ӧ��,Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST1,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

  	//Ϊ��������Ӧ�ã���������Ӧ��Ҳ��������˲������Ƶ�����

	//case9:����װ��I����Ӧ��,Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST5,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case10:�ļ�����������²���,��װʧ��
	if(( ret = NDK_AppLoad((uchar *)FILETEST9,0))!=NDK_ERR_APP_FILE_STAT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case11:�ļ�����������²���,��װʧ��
	if(( ret = NDK_AppLoad(NULL,0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case12:��֤�ļ�������(����32�ֽ�)�����Ӧ��Ҫ����NDK_ERR_APP_FILE_NAME_TOO_LONG
	if(( ret = NDK_AppLoad((uchar *)FILETEST12,0))!=NDK_ERR_APP_FILE_NAME_TOO_LONG)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
		GOTOERR;
	}

	//case13:nRebootFlag����ΪNULL,Ӧ�÷��ز�������NDK_ERR_PARA
	if((ret = NDK_AppGetReboot(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case14:�ǹ̼�������������Ҫ������������־λӦ��Ϊ0
	if((ret = NDK_AppGetReboot(&nRebootFlag))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(nRebootFlag!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRebootFlag);
		GOTOERR;
	}
	
	//case15:�汾����ȷ�̼�����������,��װӦ�÷���NDK_ERR_APP_FIRM_PATCH_VERSION
	if(( ret = NDK_AppLoad((uchar *)FILETEST10,0))!=NDK_ERR_APP_FIRM_PATCH_VERSION)//���Կ�����ʱ�汾����ȷ�Ĺ̼�����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);//-821
		GOTOERR;
	}

	//case16:�汾��ȷ�Ĺ̼�������Ӧ���ܹ��ɹ����أ������д���޸�,ע��:����һ�κ�汾����������һ�λᱨ��,��Ҫ���´��
	if(( ret = NDK_AppLoad((uchar *)FILETEST11,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case17:װ����ȷ�Ĺ̼���������Ӧ��Ҫ��������ȡ����������־Ӧ����1
	if((ret = NDK_AppGetReboot(&nRebootFlag))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(nRebootFlag!=1)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRebootFlag);
		GOTOERR;
	}
		
	//case18:��֤NDK_AppLoad���ú��Ƿ�Ѿ���رգ�����ر������ǰ�����δ��ļ��ľ��Ӧ�������������������˵��NDK_AppLoadδ�رվ��
	if ((fp=NDK_FsOpen(FILESYS, "w"))<0) // ��д���ļ��򿪸ո�ɾ�����ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if(( ret = NDK_AppLoad((uchar *)FILETEST1,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
		GOTOERR;
	}
	if ((fp2=NDK_FsOpen(FILESYS1, "w"))<0) // ��д���ļ��򿪸ո�ɾ�����ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp2);
		GOTOERR;
	}
	if((fp2-fp)!=1)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, fp2, fp);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp2))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case19:֤����δ��Ч ��Ҫ����֤�����Чʱ����ȷ��,Ŀǰ��2013-2023��Ϊ�ο�
	//����POSʱ��2012.10.10 10:10:10   ����2000����ǰʱ��,��ʽδ��Ч
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2012 - 1900;
	stSetRtcTime.tm_mon = 10 - 1;
	stSetRtcTime.tm_mday = 10;
	stSetRtcTime.tm_hour = 10;
	stSetRtcTime.tm_min = 10;
	stSetRtcTime.tm_sec = 10;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if(( ret = NDK_AppLoad((uchar *)FILETEST1,0))!=NDK_ERR_APP_CERT_NOT_YET_VALID)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	
	//case20:֤���Ѿ�ʧЧ
	//����POSʱ��2024.10.10 10:10:10 ����2025���Ժ�֤����ʧЧ
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2024 - 1900;
	stSetRtcTime.tm_mon = 10 - 1;
	stSetRtcTime.tm_mday = 10;
	stSetRtcTime.tm_hour = 10;
	stSetRtcTime.tm_min = 10;
	stSetRtcTime.tm_sec = 10;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if(( ret = NDK_AppLoad((uchar *)FILETEST1,0))!=NDK_ERR_APP_CERT_HAS_EXPIRED)//����汾�ж�֤��ʱ��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}

	//Ϊ��������Ӧ���ܳɹ�װ�أ�����Ϊ֤����Чʱ��
	memset(&stSetRtcTime, 0, sizeof(stSetRtcTime));
	stSetRtcTime.tm_year = 2018 - 1900;
	stSetRtcTime.tm_mon = 10 - 1;
	stSetRtcTime.tm_mday = 10;
	stSetRtcTime.tm_hour = 10;
	stSetRtcTime.tm_min = 10;
	stSetRtcTime.tm_sec = 10;
	if((ret=NDK_SysSetRtcTime(stSetRtcTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}

	//case7:����װ��A��������Ӧ��,Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST2,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR2;
	}

#if 0  //������Ӧ���Ѿ�����������ļ������ڣ��޷���֤
	//case8:װ��A��������Ӧ�õĲ�����Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST6,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2:����װ��R��������Ӧ��,Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST4,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:װ��R��������Ӧ�õĲ�����Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST7,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//����ͨ��
	send_result("%s����ͨ��,���˳�����������������ز��Գ���,����Ӱ��֮����������" , TESTAPI);
ERR2:
	NDK_SysSetRtcTime(stOldRtcTime);
	goto ERR;
ERR:
	for(i = 0;i<SZ_ARRAY(appname);i++)
		NDK_AppDel((char *)appname[i]);
	NDK_FsClose(fp);
	NDK_FsClose(fp2);
	NDK_FsDel(FILESYS);
	NDK_FsDel(FILESYS1);
	return;
}
#else
void app1(void)
{
	/*private & local definition*/
	int  ret = 0, i = 0;
	int nRebootFlag=-1;
	int fp = -1, fp2 = -1;
	uchar *appname[] = {
											(uchar *)"App_U_NonMaster",
											(uchar *)"App_U_Master",
											(uchar *)"App_A_NonMaster",
											(uchar *)"App_A_Master",
											(uchar *)"App_P",
											(uchar *)"App_L",
											(uchar *)"App_A_NonMaster_pri",
											//(uchar *)"fw_patch_1.2.3.4_2013",
											(uchar *)"NDK_AppEXECV",
											(uchar *)"a` $&();'a",
											(uchar *)"a~!@#%^_=+,.[{]}��"
										};

	struct tm stSetPosTime;
	struct tm stOldPosTime;
	
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	if(cls_show_msg1(30, "����%s...��ȷ�������ļ�����,�ҵ�ǰ�û�����ΪrootȨ��,ESC�˳�", TESTAPI)==ESC)
		return;//��Ļ��ʾ��ǰ��������ģ��������
	
	//����ǰ��:ȡ��ǰʱ��Ϊ�����޸Ĳ���ʱ����׼��
	NDK_FsDel(FILESYS);
	NDK_FsDel(FILESYS1);
	if((ret=NDK_SysGetPosTime(&stOldPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//case21:��֤�ļ�������(����32�ֽ�)�����Ӧ��Ҫ����NDK_ERR_APP_FILE_NAME_TOO_LONG
	if(( ret = NDK_AppLoad((uchar *)FILETEST16,0))!=NDK_ERR_APP_FILE_NAME_TOO_LONG)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
		GOTOERR;
	}
	
	//case1:����װ��U���ͷ�����Ӧ�ã�Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST1,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
		GOTOERR;
	}
	
	//case2::����װ��U��������Ӧ�ã�Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST2,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case3::����װ��A���ͷ�����Ӧ�ã�Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST3,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case4:����װ��A��������Ӧ�ã�Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST4,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case5:����װ��P���Ͳ����ļ���Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST5,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case6:����װ��L���Ϳ��ļ���Ӧ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST6,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case7:װ�ش������ϢժҪ.��װʧ��
	if(( ret = NDK_AppLoad((uchar *)FILETEST9,0))!=NDK_ERR_APP_SIGN_CHECK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case8:��ͨӦ�ð�װ�ɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST11,0))!=NDK_OK)//������֤NDK_AppEXECV���԰�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case9:�ļ�����������²���,��װʧ��
	if(( ret = NDK_AppLoad((uchar *)FILETEST10,0))!=NDK_ERR_APP_FILE_STAT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case10:�ļ�����������²���,��װʧ��
	if(( ret = NDK_AppLoad(NULL,0))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case11:Ӧ�ò���������,��װӦ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST7,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case14:nRebootFlag����ΪNULL,Ӧ�÷��ز�������NDK_ERR_PARA
	if((ret = NDK_AppGetReboot(NULL))!=NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case15:�ǹ̼�������������Ҫ������������־λӦ��Ϊ0
	if((ret = NDK_AppGetReboot(&nRebootFlag))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(nRebootFlag!=0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRebootFlag);
		GOTOERR;
	}
	
#if 1
	//case12:�汾����ȷ�̼�����������,��װӦ�÷���NDK_ERR_APP_FIRM_PATCH_VERSION//daitao 20130711 ���ع̼������������µ�NDKҪ�����жϰ汾�Ƿ�ƥ�䣬���ƥ�����������أ������ֹ
	if(( ret = NDK_AppLoad((uchar *)FILETEST8,0))!=NDK_ERR_APP_FIRM_PATCH_VERSION)//�汾����ȷ�Ĺ̼�����
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//case13:�汾��ȷ�Ĺ̼�������Ӧ���ܹ��ɹ����أ������д���޸�
	if(( ret = NDK_AppLoad((uchar *)FILETEST12,0))!=NDK_OK)//�汾��ȷ�Ĺ̼������������д���޸�
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case16:װ����ȷ�Ĺ̼���������Ӧ��Ҫ��������ȡ����������־Ӧ����1
	if((ret = NDK_AppGetReboot(&nRebootFlag))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(nRebootFlag!=1)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, nRebootFlag);
		GOTOERR;
	}
#endif

	//20140603 added by huangjianb
	//case17:��֧�ֵ������ַ�����,��װӦ��ʧ��
	if(( ret = NDK_AppLoad((uchar *)FILETEST13,0))!=NDK_ERR/*NDK_ERR_APP_FILE_STAT*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}

	//case18:֧�ֵ������ַ�����,��װӦ�óɹ�
	if(( ret = NDK_AppLoad((uchar *)FILETEST14,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//End add

	//case21:��֤NDK_AppLoad���ú��Ƿ�Ѿ���رգ�����ر������ǰ�����δ��ļ��ľ��Ӧ�������������������˵��NDK_AppLoadδ�رվ��
	if ((fp=NDK_FsOpen(FILESYS, "w"))<0) // ��д���ļ��򿪸ո�ɾ�����ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		GOTOERR;
	}
	if(( ret = NDK_AppLoad((uchar *)FILETEST1,0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
		GOTOERR;
	}
	if ((fp2=NDK_FsOpen(FILESYS1, "w"))<0) // ��д���ļ��򿪸ո�ɾ�����ļ�
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, fp2);
		GOTOERR;
	}
	if((fp2-fp)!=1)
	{
		send_result("line %d: %s����ʧ��(%d,%d)", __LINE__, TESTAPI, fp2, fp);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_FsClose(fp2))!=NDK_OK)
	{
		send_result("line %d: %s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case19:֤����δ��Ч ��Ҫ����֤�����Чʱ����ȷ��,Ŀǰ��2013-2023��Ϊ�ο�
	//����POSʱ��2012.10.10 10:10:10   ����2000����ǰʱ��,��ʽδ��Ч
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2012 - 1900;
	stSetPosTime.tm_mon = 10 - 1;
	stSetPosTime.tm_mday = 10;
	stSetPosTime.tm_hour = 10;
	stSetPosTime.tm_min = 10;
	stSetPosTime.tm_sec = 10;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if(( ret = NDK_AppLoad((uchar *)FILETEST15,0))!=NDK_OK/*NDK_ERR_APP_CERT_NOT_YET_VALID*/)//20140901 NDK 5.0.0�汾�в���У��֤��ʱ�䣬����װδ��Ч������ڵ�Ӧ��
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	
	//case20:֤���Ѿ�ʧЧ
	//����POSʱ��2024.10.10 10:10:10 ����2025���Ժ�֤����ʧЧ
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 2024 - 1900;
	stSetPosTime.tm_mon = 10 - 1;
	stSetPosTime.tm_mday = 10;
	stSetPosTime.tm_hour = 10;
	stSetPosTime.tm_min = 10;
	stSetPosTime.tm_sec = 10;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
#if 0//OVERSEAS_ENABLE
	if(( ret = NDK_AppLoad((uchar *)FILETEST15,0))!=NDK_ERR_APP_CERT_HAS_EXPIRED)//����汾��Ȼ���ж�֤��ʱ��
#else		
	if(( ret = NDK_AppLoad((uchar *)FILETEST15,0))!=NDK_OK/*NDK_ERR_APP_CERT_HAS_EXPIRED*/)//20140901 NDK 5.0.0�汾�в���У��֤��ʱ�䣬����װδ��Ч������ڵ�Ӧ��
#endif	
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR2;
	}
#if 0
	//case21:��Կ�Ѿ�ʧЧ
	//����POSʱ��2099.10.10 10:10:10 ����2098���Ժ�Կ�Ѿ�ʧЧNDK_ERR_APP_PUBKEY_EXPIRED
	memset(&stSetPosTime, 0, sizeof(stSetPosTime));
	stSetPosTime.tm_year = 1970 - 1900;
	stSetPosTime.tm_mon = 10 - 1;
	stSetPosTime.tm_mday = 10;
	stSetPosTime.tm_hour = 10;
	stSetPosTime.tm_min = 10;
	stSetPosTime.tm_sec = 10;
	if((ret=NDK_SysSetPosTime(stSetPosTime)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
	if(( ret = NDK_AppLoad((uchar *)FILETEST15,0))!=NDK_ERR_APP_PUBKEY_EXPIRED)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR2;
	}
#endif
	//����ͨ��
	send_result("%s����ͨ��,����˳���������������������ز��Գ���", TESTAPI);
ERR2:
	NDK_SysSetPosTime(stOldPosTime);
	goto ERR;
ERR:
	for(i = 0;i<SZ_ARRAY(appname);i++)
		NDK_AppDel((char *)appname[i]);
	NDK_FsClose(fp);
	NDK_FsClose(fp2);
	NDK_FsDel(FILESYS);
	NDK_FsDel(FILESYS1);
	return;
}

#endif

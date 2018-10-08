/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App11.c
* Author 			:
* version			: 
* DATE			:
* directory 		: 
* description		: ����NDK_AppDataSave,NDK_AppDataVerify,NDK_AppUpdate���ݴ��䡢����У�顢app����
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  TESTAPI		"NDK_AppDataSave,NDK_AppDataVerify,NDK_AppUpdate"
#if defined SP610
#define  APPTEST11		"right.NLP"
#define  ERRSIGNFILE	"wrong.NLP"//�ļ�ǩ������
#define  APPTEST12     "master.NLP"
#define  APPTEST3      "ApppkS4.NLP"   //�����ַ0x0314C000(APPPK��ַ)��Ӧ��С��4k��Ԥ��Ӧ�ø���ʧ��
#define  APPTEST4      "DApppkS8.NLP"  //�����ַ0x0314A000(DEBUGAPPPK��ַ),Ӧ�ô���4k��С��8k��Ԥ��Ӧװ��ʧ��
#define  APPTEST5     "FSigS4.NLP"  //�����ַ0x0314D000(Font24*24-SIG��ַ),Ӧ��С��4k��Ԥ��Ӧ��װ��ʧ��
#define  APPTEST6     "SigB4.NLP"   //�����ַ0x0314B000(APPPK-SIG)��ַ��Ӧ�ô���4k��Ԥ��Ӧ��װ��ʧ��
#define  APPTEST7     "SigS4.NLP"  //�����ַ0x0314B000(APPPK-SIG)��ַ��Ӧ��С��4k��Ԥ��Ӧ��װ��ʧ��
#else
#define  APPTEST11		"/appfs/right.NLP"
#define  ERRSIGNFILE	"/appfs/wrong.NLP"//�ļ�ǩ������
#define  APPTEST12      "/appfs/master.NLP"
#define  FWTESTFILE    "/appfs/fwupdat.img"
#endif

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : 
* functional description : �ļ�ϵͳģ���11������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*
*****************************************************************/
//pszName:Ӧ�ó����ļ�·����i
//NLP��ʽ:16�ֽڵ�ͷ + 256�ֽڵ�ǩ�� + 4���ֽڵĹ̼����� + ʵ������
//16�ֽ�ͷ���󷵻�NDK_ERR_APP_NLD_HEAD_LEN
//256�ֽ�ǩ������NDK_ERR_APP_SIGN_DECRYPT
//4�ֽڳ��Ⱥ�ʵ�����ݴ���NDK_ERR_APP_SIGN_CHECK
int app_transdata(const char *pszName, int offseterrflag, int buferrflag)
{
	int  ret = 0, offset=0, fp=0, tmp = 0;
	uint len = 0;
	char buf[1024]={0};
	if((fp=NDK_FsOpen(pszName, "r"))<0)
	{
		cls_show_msg1(5, "line %d:%s�ļ���ʧ��(%d)", __LINE__, pszName, fp);
		return NDK_ERR;
	}
	while(1)
	{
		if((len=NDK_FsRead(fp, (char *)buf, 1024))<0)
		{
			cls_show_msg1(5, "line %d:%s�ļ���ȡ����ʧ��(%d)", __LINE__, pszName, len);
			NDK_FsClose(fp);
			return NDK_ERR;
		}
		if(buferrflag==1)
		{
			tmp = rand()%(len-272)+272;
			if(buf[tmp]==0)
				buf[tmp]=0xff;
			else
				buf[tmp]=0;//�����޸ĵ�һ�ζ��������е�����һ���ֽ�Ϊ0,�����������ݴ���
			buferrflag = 0;
		}
#if defined ME68||defined ME50NX  //����ƽ̨����app:offset = 256*1024+offset�����¹̼�:offset = 0+ offset
		if((ret=NDK_AppDataSave(256*1024+offset,buf,len))!=NDK_OK)
#else
		if((ret=NDK_AppDataSave(offset, buf, len))!=NDK_OK)
#endif
		{
			cls_show_msg1(5, "line %d:%s�ļ�App���ݴ���ʧ��(%d)", __LINE__, pszName, ret);
			NDK_FsClose(fp);
			return NDK_ERR;
		}
		if(offseterrflag!=0)
		{
			offset+=offseterrflag;//���׵�ַ�޸ĳ�offseterrflag,ʹ��Ե�ַ����ȷ�������ݱ������
			offseterrflag=0;
		}
		offset+=len;
		
		if(len<1024)
			break;
	}
	NDK_FsClose(fp);
	return NDK_OK;
}
void app11(void)
{
	/*private & local definition*/
	int  ret = 0, fp=0, nRebootFlag = 0,fd = 0;
	uint offset=0, len=0, size = 0, i = 0;
	char buf[1024]={0};
	
	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
#if !(defined ME15C||defined ME15CHX||defined ME15B)
	if(cls_show_msg1(30, "����%s...��ȷ��%s,%s��%s�����ļ�����,ESC�˳�", TESTAPI, APPTEST11, ERRSIGNFILE, APPTEST12)==ESC)
#elif defined SP610
	if(cls_show_msg1(30, "����%s...��ȷ�����еĲ����ļ�����,ESC�˳�", TESTAPI)==ESC)
#else
	if(cls_show_msg1(30, "����%s...��ȷ��%s�����ļ�����,ESC�˳�", TESTAPI, APPTEST11)==ESC)
#endif
		return;//��Ļ��ʾ��ǰ��������ģ��������
		
#if defined ME68 || defined ME50NX
	if((fp = NDK_FsOpen(APPTEST11, "w")) <0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		RETURN;
	}
	if((ret = NDK_FsWrite(fp, "Scorpiop", 8)) != 8)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_FsClose(fp)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

	if(cls_show_msg("�Ƿ����1�����������ԣ��ǰ�ȷ�ϼ�������������")==ENTER)
	{
		//case1:�������ԣ��ļ���������У��ϵ�󣬿��������ܼ������䣬������־Ϊ1
		if(cls_show_msg1(30, "�Ƿ�Ҫ����֮ǰ��������ݲ���(������Ҫ������)?����[ENTER],������[����]��")!=ENTER)
		{
			if((fp=NDK_FsOpen(APPTEST11, "r"))<0)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
				return;
			}
			if((len=NDK_FsRead(fp, (char *)buf, 1024))<0)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, len);
				goto ERR;
			}
#if defined ME68||defined ME50NX
			if((ret=NDK_AppDataSave(256*1024+offset,buf,len))!=NDK_OK)
#else
			if((ret=NDK_AppDataSave(offset, buf, len))!=NDK_OK)
#endif
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
				goto ERR;
			}
		}
#if defined ME50N || defined ME50 ||defined ME50C||defined ME50H||defined ME62 ||defined ME50NX||defined ME68//ME50N���ӵ�ȡ����Ϊ��Դ��
		if(cls_show_msg("��ȷ�ϼ�����POS��(��֮ǰ�ѽ��д˲�����������������)")==ENTER)
			NDK_SysReboot();
#else
		cls_show_msg("�밴��Դ����POS��(��֮ǰ�ѽ��д˲������ɰ����������)");
#endif
		if((fp=NDK_FsOpen(APPTEST11, "r"))<0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
			return;
		}
		if((len=NDK_FsRead(fp, (char *)buf, 1024))<0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, len);
			goto ERR;
		}
		offset+=len;
		while(1)
		{
			if((len=NDK_FsRead(fp, (char *)buf, 1024))<0)
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, len);
				goto ERR;
			}
#if defined ME68||defined ME50NX  //����ƽ̨����app:offset = 256*1024+offset�����¹̼�:offset = 0+ offset
			if((ret=NDK_AppDataSave(256*1024+offset,buf,len))!=NDK_OK)
#else
			if((ret=NDK_AppDataSave(offset, buf, len))!=NDK_OK)
#endif
			{
				send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
				goto ERR;
			}
			offset+=len;
			if(len<1024)
				break;
		}
		NDK_FsClose(fp);
#if !(defined ME15C||defined ME15CHX||defined ME15B)	//ME15C��֧��NDK_AppDataVerify������NDK_AppUpdate(0)
		if((ret=NDK_AppDataVerify())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
#endif	
		cls_show_msg1(3,"3S������...������۲�Ӧ���Ƿ����Ϊright�ĳ���,���ɹ�����,��������Ϊͨ����");
		if((ret=NDK_AppUpdate(1))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
	}

	if(cls_show_msg("�Ƿ����2�����������ԣ��ǰ�ȷ�ϼ�������������")==ENTER)
	{
		//case2:�ļ�ǩ����ȷ�������ļ���ȷ�����ݼ�������ɹ���app���³ɹ���������־Ϊ0
		if((ret=app_transdata(APPTEST11, 0, 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
#if !(defined ME15C||defined ME15CHX||defined ME15B)	//ME15C��֧��NDK_AppDataVerify������NDK_AppUpdate(0)
		if((ret=NDK_AppDataVerify())!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
#if defined ME68 || defined ME50NX //����ƽ̨ԭ��Ӧ�ñ����ǵ�����ܷɣ�����Ϊ����
		cls_show_msg1(3,"3��󽫽���Ӧ�ø��£�Ӧ�ø��º������(����������Ϊ��������)");
#else
		cls_show_msg1(3,"3S�󽫽���Ӧ�ø��£����º�Ӧ�ò�����");
#endif
		nRebootFlag = 0;
#else
		cls_show_msg1(3,"3S������...������۲�Ӧ���Ƿ����Ϊright�ĳ���,���ɹ�����,��������Ϊͨ����");
		nRebootFlag = 1;
		if((ret=NDK_AppUpdate(0))!=NDK_ERR_NOT_SUPPORT)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
#endif
		if((ret=NDK_AppUpdate(nRebootFlag))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			return;
		}
	}

#if !(defined ME15C||defined ME15CHX||defined ME15B)	//ME15C��֧��NDK_AppDataVerify����,����APP�����޷�֪���Ƿ�������.  zhengry  20160419  modified
	//case3:�쳣���ԣ��ļ�ǩ�����󣬴����ļ��ɹ�������У������ݸ��¶�Ӧ����ǩ��У�����NDK_ERR_APP_SIGN_DECRYPT,
#if defined ME68 || defined ME50NX
	if((fp = NDK_FsOpen(ERRSIGNFILE, "w")) <0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		RETURN;
	}
	if((ret = NDK_FsWrite(fp, "Scorpiop", 8)) != 8)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret = NDK_FsClose(fp)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif
	if((ret=app_transdata(ERRSIGNFILE, 0, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#if defined ME68||defined ME50NX
	if((ret=NDK_AppDataVerify())!=NDK_ERR_APP_SIGN_CHECK)
#else
	if((ret=NDK_AppDataVerify())!=NDK_ERR_APP_SIGN_DECRYPT)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#if defined ME68||defined ME50NX
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_SIGN_CHECK)
#else
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_SIGN_DECRYPT)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}

	//case4:�쳣���ԣ��ļ������У�offset��Ե�ַ���ݴ�������app����ɣ�����У������ݸ���Ӧ����sha256У��ʧ��NDK_ERR_APP_SIGN_CHECK
	if((ret=app_transdata(APPTEST11, 1, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_AppDataVerify())!=NDK_ERR_APP_SIGN_CHECK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_SIGN_CHECK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	
	//case5:�쳣���ԣ��ļ������У�buf����һ�����ݷ�����������У������ݸ���Ӧ����sha256У��ʧ��NDK_ERR_APP_SIGN_CHECK
	if((ret=app_transdata(APPTEST11, 0, 1))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_AppDataVerify())!=NDK_ERR_APP_SIGN_CHECK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_SIGN_CHECK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#endif

#if defined SP610
	if((ret=app_transdata(APPTEST3, 0, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AppDataVerify())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_ADDRESS_NOT_LEGAL/*NDK_ERR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	if((ret=app_transdata(APPTEST4, 0, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AppDataVerify())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_ADDRESS_NOT_LEGAL/*NDK_ERR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	
	if((ret=app_transdata(APPTEST5, 0, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AppDataVerify())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_ADDRESS_NOT_LEGAL/*NDK_ERR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if((ret=app_transdata(APPTEST6, 0, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AppDataVerify())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_ADDRESS_NOT_LEGAL/*NDK_ERR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}

	if((ret=app_transdata(APPTEST7, 0, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AppDataVerify())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_ADDRESS_NOT_LEGAL/*NDK_ERR*/)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif	

	//case6:�쳣���ԣ��ļ�ǩ����ȷ�������ļ���ȷ�����ݼ�������ɹ��������ݴ������һ�����ݣ����³���ӦУ��ʧ�ܡ�
	if((ret=app_transdata(APPTEST11, 0, 0))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#if !(defined ME15C||defined ME15CHX||defined ME15B)	//ME15C��֧��NDK_AppDataVerify����
	if((ret=NDK_AppDataVerify())!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#endif	
#if defined ME50NX||defined ME68
	if((ret=NDK_AppDataSave(256*1024+3, (uchar *)'1', 1))!=NDK_OK)
#else
	if((ret=NDK_AppDataSave(0, (uchar *)'1', 1))!=NDK_OK)  //�����ݴ������һ������
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI,  ret);
		return;
	}
#if !(defined ME15C||defined ME15CHX||defined ME15B)
#if defined ME68||defined ME50NX  //����ƽ̨����ʧ�ܶ��Ƿ���-821  
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_SIGN_CHECK)
#else
	if((ret=NDK_AppUpdate(0))!=NDK_ERR_APP_NLD_HEAD_LEN)
#endif
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#else
	//ME15C NDK_AppUpdate�޷��жϸ����Ƿ�ɹ�    zhengry 20160421 modified
	cls_show_msg1(3,"3S������...�Զ��������������,�ٴ�������۲쵽Ӧ�ò�û�и��³ɹ�,����ԭ�ȵ�Ӧ�á�");
	if((ret=NDK_AppUpdate(1))!=NDK_ERR_APP_NLD_HEAD_LEN)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		return;
	}
#endif

	//case7:��������master�������۲��Ƿ������� 20171120 sull
#if CPU5810X_ENABLE //ֻ��5810ƽ̨֧��������ܣ�Ŀǰֻ��ME50N֧�� 20171122
	//ע��:���boot������Ҫ�ȸ���boot�ٽ��д���������
	cls_printf("�����̼�������...");
	offset = 0;
	if((fd = NDK_FsOpen(APPTEST12,"r"))<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fd);
		goto ERR1;
	}
	NDK_FsFileSize(APPTEST12,&size);
	len = size /sizeof(buf);
	for(i=0;i<len;i++)
	{
		memset(buf,0x00,sizeof(buf));
		if((ret = NDK_FsRead(fd,buf,sizeof(buf)))<0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR1;
		}
		if((ret = NDK_AppDataSave(offset,buf,sizeof(buf)))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR1;
		}
		offset += sizeof(buf);
	}
	memset(buf,0x00,sizeof(buf));
	if((ret = NDK_FsRead(fd,buf,size%sizeof(buf)))<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR1;
	}
	if((ret = NDK_AppDataSave(offset,buf,size%sizeof(buf)))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR1;
	}
	if((ret = NDK_FsClose(fd))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR1;
	}
	if((ret = NDK_FsDel(APPTEST12))!=NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR1;
	}
	if(cls_show_msg("��ȷ�ϼ�����POS,����������Լ�鿴�汾�Ƿ����,ע�ⲻͬ��Ʒ��master��ͬ,���ҿ����ṩ,�Ҳ������Ҫ����ˢ��ԭ����master")==ENTER)
		NDK_SysReboot();
#endif

#if defined ME68||defined ME50NX  //����ƽ̨���������̼�����:���¹̼�:offset = 0+ offset
	cls_printf("�����̼�������...");
	offset = 0;
	if((fp=NDK_FsOpen(FWTESTFILE, "r"))<0)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, fp);
		goto ERR;
	}
	while(1)
	{
		if((len=NDK_FsRead(fp, (char *)buf, 1024))<0)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, len);
			goto ERR;
		}
		if((ret=NDK_AppDataSave(offset, buf, len))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
			goto ERR;
		}
		offset+=len;
		if(len<1024)
			break;
	}
	if((ret = NDK_FsClose(fp)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	
	//	�̼����� Ҫ�� data save ��֮���һ��NDK_AppDataSave(0xffffffff, NULL, 0),ac35����
	if((ret = NDK_AppDataSave(0xffffffff, NULL, 0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		goto ERR;
	}	
  	//ac35�Ĺ̼���������У�飬�̼�У����ac35�װ����
	if(cls_show_msg("��ȷ�ϼ�����POS,������۲�̼��Ƿ�����,���������Ҫ��Ϊԭ���Ĺ̼�")==ENTER)
		NDK_SysReboot();
#endif
		
	//����ͨ��
	send_result("%s����ͨ��", TESTAPI);	
ERR:
	NDK_FsClose(fp);
	return;
ERR1:
	NDK_FsClose(fd);
	NDK_FsDel(APPTEST12);
	return;
}



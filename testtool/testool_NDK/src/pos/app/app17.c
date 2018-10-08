/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: appģ��
* file name		: App17.c
* Author 			:
* version			: 
* DATE			: 
* directory 		: 
* description		: ���Է�������NDK_AppLoad�ӿ�
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#if 0 
//���SP610�����ķ������ƣ�������Ӧ��������
#define  FILETEST13 "factor.NLP" //����Ϊfactory��flash��Ϣ��ȷ�������ַ0x030D6000��Ӧ��8K��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST14 "selftes.NLP"  //����Ϊselftest��flash��Ϣ��ȷ�������ַ0x030F0000��Ӧ��8k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST15 "klaa.NLP"  //����Ϊkla��flash��Ϣ��ȷ�������ַ0x032FB000��Ӧ��8k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST16 "mapp.NLP" //����Ϊmapp��flash��Ϣ��ȷ�������ַ0x0330A000��Ӧ��8k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST17 "mErr1.NLP" //����Ϊmapp��flash��Ϣ��ȷ�������ַ0x033FF000��Ӧ�ô���4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST18 "sErr1.NLP" //����Ϊselftest��flash��Ϣ��ȷ�������ַ0x030F9000��Ӧ�ô���4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST19 "mErr2.NLP" //����Ϊmapp��flash��Ϣ���󣬴����ַ0x0330A000��Ӧ��8k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST20 "fErr3.NLP" //����Ϊfactory��flash��Ϣ��ȷ�������ַ����0x030EF000��Ӧ��С��4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST21 "sErr3.NLP" //����Ϊselftest��flash��Ϣ��ȷ�������ַ����0x030FB000��Ӧ��С��4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST22 "kErr3.NLP" //����Ϊkla��flash��Ϣ��ȷ�������ַ����0x03305000��Ӧ��С��4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST23 "mErr3.NLP" //����Ϊmapp��flash��Ϣ��ȷ�������ַ����0x03308000��Ӧ��С��4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST24 "debug.NLP" //����Ϊdebugapppk��flash��Ϣ��ȷ�������ַ0x0314B000��Ӧ��С��4k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST25 "apppk.NLP" //����Ϊapppk��flash��Ϣ��ȷ�������ַ0x0314E000��Ӧ��С��4k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST26 "font24.NLP" //����Ϊfont24_h��flash��Ϣ��ȷ�������ַ0x0314E000��Ӧ��С��4k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST27 "sec.NLP"  //����Ϊsec_config��flash��Ϣ��ȷ�������ַ0x0340000��Ӧ��С��4k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST28 "font32.NLP" //����Ϊfont32_h��flash��Ϣ��ȷ�������ַ0x0340000��Ӧ��С��4k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST29 "font.NLP" //����Ϊfont-h��flash��Ϣ��ȷ�������ַ0x003B9000��Ӧ��С��4k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST30 "xuii.NLP" //����Ϊxui��flash��Ϣ��ȷ�������ַ0x003E7000��Ӧ��С��4k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST31 "status.NLP" //����Ϊstatusicon��flash��Ϣ��ȷ�������ַ0x003F2000��Ӧ��С��4k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST32 "debug1.NLP" //����Ϊdebugapppk��flash��Ϣ���󣬴����ַ0x0314E000��Ӧ��С��4k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST33 "xui1.NLP" //����Ϊxui��flash��Ϣ���󣬴����ַ0x003E7000��Ӧ��С��4k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST34 "dErr.NLP" //����Ϊdebugapppk��flash��Ϣ��ȷ�������ַ0x0314A000��Ӧ�ô���4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST35 "boot1.NLP"  //����Ϊboot(name����ΪƷ��)��flash��Ϣ��ȷ�������ַ0x0040000��Ӧ��С��52k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST36 "boot2.NLP"  //����Ϊboot(name����ΪƷ��)��flash��Ϣ���󣬴����ַ0x0040000��Ӧ��С��52k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST37 "bErr.NLP" //����Ϊboot(name����ΪƷ��)��flash��Ϣ��ȷ�������ַ0x00400000��Ӧ�ô���52k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST38 "other1.NLP"  //����Ϊmapp��flash��Ϣ��ȷ�������ַ0x0336D000��Ӧ��8k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST39 "boot3.NLP" //name��Ϊboot��flash��Ϣ��ȷ�������ַ0x03372000��Ӧ��8k��Ԥ��Ӧ��װ�سɹ�
#define  FILETEST40 "other2.NLP" //����Ϊmapp��flash��Ϣ���󣬴����ַ0x03309000��Ӧ��8k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST41 "other3.NLP" //����Ϊmapp��flash��Ϣ��ȷ�������ַ0x033FF000��Ӧ�ô���4k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST42 "other4.NLP" //����Ϊmapp��flash��Ϣ��ȷ�������ַ����0x0340A000��Ӧ��8k��Ԥ��Ӧ��װ��ʧ��
#define  FILETEST43 "boot4.NLP" //name��Ϊboot��flash��Ϣ��ȷ�������ַ����0x0340A000��Ӧ��8k��Ԥ��Ӧ��װ��ʧ��
#endif

#define 	TESTAPI	  "��������NDK_AppLoad"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 	 : app17
* functional description : Ӧ�ù���ģ���17������������
* input parameter	  	 :
* output parameter	 	 : 
* return value		 	 :
* history 		 		 : author			date	 	  remarks
*                                       sull                 20180806          created
*****************************************************************/
void app17(void)
{
	/*private & local definition*/
	int  ret = 0;
	int i = 0;
	char *package[] = {"factor.NLP", "selftes.NLP", "klaa.NLP", "mapp.NLP", "mErr1.NLP", "sErr1.NLP", 
						"mErr2.NLP", "fErr3.NLP", "sErr3.NLP", "kErr3.NLP", "mErr3.NLP","apppk.NLP", 
						"debug.NLP", "font24.NLP", "sec.NLP", "font32.NLP", "font.NLP", "xuii.NLP", 
						"status.NLP", "debug1.NLP", "xui1.NLP", "dErr.NLP", "boot1.NLP", "boot2.NLP",
						"bErr.NLP", "other1.NLP", "boot3.NLP","other2.NLP", "other3.NLP", "other4.NLP", "boot4.NLP"};
	char *pName[] = {"factor", "selftes", "klaa", "mapp", "mErr1", "sErr1", "mErr2", "fErr3", "sErr3", "kErr3",
						"mErr3", "debug", "apppk", "font24", "sec", "font32", "font", "xuii", "status", "debug1", 
						"xui1", "dErr", "boot1", "boot2", "bErr", "other1", "boot","other2", "other3", "other4", "boot4"};

	/*process body*/
	if(auto_flag==1)
	{
		send_result("%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTAPI);
		return;
	}
	if(cls_show_msg1(30, "����%s...��ȷ�������ļ�����,ESC�˳�", TESTAPI)==ESC)
		return;//��Ļ��ʾ��ǰ��������ģ��������

	//���SP610�����ķ������ƣ��������²��� 
	//case1:���ҵõ���Ӧ�ķ�����Ϣ��NLP��Ϊ��ִ�г���NLP����ַ�ڶ�Ӧ�ķ�����Ϣ�����ĵ�ַ��Χ�ڣ�Ӧ�ð���С+�����ַ<=������ַ��Χ��Ӧ�ɹ�����
	for(i=0;i<=2;i++)
	{
		if((ret = NDK_AppLoad(package[i], 0))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[i]);
			GOTOERR;
		}
	}
	if((cls_show_msg1(30, "��ȷ�ϼ�����������1����(3�����Զ�����������������Ӧ����Ļ��ʾmapp���������²����������������)����������������")) == ENTER)
	{
		if((ret = NDK_AppLoad(package[3], 1))!=NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[3]);
			GOTOERR;
		}
	}

	//case2:���ҵõ���Ӧ�ķ�����Ϣ��NLP��Ϊ��ִ�г���NLP����ַ�ڶ�Ӧ�ķ�����Ϣ�����ĵ�ַ��Χ�ڣ�flash��Ϣ����Ӧ����ʧ��
	if((ret = NDK_AppLoad(package[4], 0)) != NDK_ERR_APP_ADDRESS_NOT_LEGAL)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[4]);
		GOTOERR;
	}

	//case3:���ҵõ���Ӧ�ķ�����Ϣ��NLP��Ϊ��ִ�г���NLP����ַ�ڶ�Ӧ�ķ�����Ϣ�����ĵ�ַ��Χ�ڣ�Ӧ�ð���С+�����ַ>������ַ��Χ��Ӧ����ʧ��
	if((ret = NDK_AppLoad(package[5], 0)) != NDK_ERR_APP_ADDRESS_NOT_LEGAL)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[5]);
		GOTOERR;
	}
	if((ret = NDK_AppLoad(package[6], 0)) != NDK_ERR_APP_ADDRESS_NOT_LEGAL)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[6]);
		GOTOERR;
	}

	//case4:���ҵõ���Ӧ�ķ�����Ϣ��NLP��Ϊ��ִ�г���NLP����ַ���ڶ�Ӧ�ķ��� ��Ϣ�����ĵ�ַ��Χ�ڣ�Ӧ����ʧ��
	for(i= 7; i<=10; i++)
	{
		if((ret = NDK_AppLoad(package[i], 0)) != NDK_ERR_APP_ADDRESS_NOT_LEGAL)
		{
			send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[i]);
			GOTOERR;
		}
	}

	//case5:���ҵõ���Ӧ�ķ�����Ϣ��NLP��Ϊ����ִ�г���NLP����Ӧ�ķ�����ַ+Ӧ�ð���С�ڷ�����ַ��Χ�ڣ�����flash��Ϣ�Ƿ���ȷ����Ӧ�ɹ�����
	//���ڽӿڲ����ظ����ع�Կ���Ҳ���ʱ�����ع�Կ���ʴ˴�Ӧ�ᱨ��
	if((ret = NDK_AppLoad(package[11], 0)) != NDK_ERR_APP_SIGN_DECRYPT)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[11]);
		GOTOERR;
	}
	for(i=12; i<=20; i++)
	{
		if((ret = NDK_AppLoad(package[i], 0)) != NDK_OK)
		{
			send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[i]);
			GOTOERR;
		}
	}

	//case6:���ҵõ���Ӧ�� ������Ϣ��NLP��Ϊ����ִ�г���NLP����Ӧ�ķ�����ַ+Ӧ�ð���С���ڷ�����ַ��Χ�ڣ�Ӧ����ʧ��
	if((ret = NDK_AppLoad(package[21], 0)) != NDK_ERR_APP_ADDRESS_NOT_LEGAL)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[21]);
		GOTOERR;
	}

	//case7:���Ҳ�����Ӧ�ķ�����Ϣ��NLP���ڰ������У�boot������ַ+boot��������С��boot������ַ��Χ�ڣ�����flash��Ϣ������ȷ��Ӧ�ɹ�����
	if((ret = NDK_AppLoad(package[22], 0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[22]);
		GOTOERR;
	}
	if((ret = NDK_AppLoad(package[23], 0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[23]);
		GOTOERR;
	}

	//case8:���Ҳ�����Ӧ�ķ�����Ϣ��NLP���ڰ������У�boot������ַ+boot��������С����boot������ַ��Χ�ڣ�Ӧ����ʧ��
	if((ret = NDK_AppLoad(package[24], 0)) != NDK_ERR_APP_ADDRESS_NOT_LEGAL)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[24]);
		GOTOERR;
	}

	//case9:���Ҳ�����Ӧ�ķ�����Ϣ��NLP�����ڰ������У�NLP����ַ��mapp������Ϣ�������ĵ�ַ��Χ�ڣ��Ҵ����ַ+Ӧ�ô�С��mapp������ַ��Χ�ڣ�Ӧ�ɹ�����
	if((ret = NDK_AppLoad(package[25], 0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[25]);
		GOTOERR;
	}
	cls_show_msg1(2,"��Ļ����ʾother1����ʾother1����������в���");
	if((ret = NDK_AppRun(pName[25])) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[25]);
		GOTOERR;
	}
	if((ret = NDK_AppLoad(package[26], 0)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[26]);
		GOTOERR;
	}
	cls_show_msg1(2,"��Ļ����ʾboot����ʾboot����������в���");
	if((ret = NDK_AppRun(pName[26])) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[26]);
		GOTOERR;
	}

	//case10:���Ҳ�����Ӧ�ķ�����Ϣ��NLP�����ڰ������У�NLP����ַ��mapp������Ϣ�������ĵ�ַ��Χ�ڣ�flash��Ϣ���󣬴����ַ+Ӧ�ô�С��mapp������ַ��Χ�ڣ�Ӧ����ʧ��
	if((ret = NDK_AppLoad(package[27], 0)) != NDK_ERR_APP_ADDRESS_NOT_LEGAL)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[27]);
		GOTOERR;
	}

	//case11:���Ҳ�����Ӧ�ķ�����Ϣ��NLP�����ڰ������У�NLP����ַ��mapp������Ϣ�������ĵ�ַ��Χ�ڣ��Ҵ����ַ+Ӧ�ô�С����mapp������ַ��Χ�ڣ�Ӧ����ʧ��
	if((ret = NDK_AppLoad(package[28], 0)) != NDK_ERR_APP_ADDRESS_NOT_LEGAL)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[28]);
		GOTOERR;
	}

	//case12:���Ҳ�����Ӧ�ķ�����Ϣ��NLP�����ڰ������У�NLP����ַ����mapp������Ϣ�������ĵ�ַ��Χ�ڣ�Ӧ����ʧ��
	if((ret = NDK_AppLoad(package[29], 0)) != NDK_ERR_APP_ADDRESS_NOT_LEGAL)
	{
		send_result("line %d:%s����ʧ��(%d,%s)", __LINE__, TESTAPI, ret,package[29]);
		GOTOERR;
	}
	if((ret = NDK_AppLoad(package[30], 0)) != NDK_ERR_APP_ADDRESS_NOT_LEGAL)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret,package[30]);
		goto ERR;
	}

	//����ͨ��
	send_result("%s����ͨ��,�Լ��ѱ��滻�����������Լ����", TESTAPI);
ERR:
	for(i=0; i<=30; i++)
		NDK_AppDel(pName);
	return;
}


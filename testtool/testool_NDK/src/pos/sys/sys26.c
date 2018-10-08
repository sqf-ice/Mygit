/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: sysģ��
* file name			: sys26.c
* Author 			: linwl
* version			: 
* DATE				: 20150509
* directory 		: 
* description		: ��������NDK_SysSetPosInfo����posӲ����Ϣ�ӿڹ���(��K21֧��)
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define TESTAPI	"NDK_SysSetPosInfo"	

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/

/****************************************************************
* function name 	 	: 
* functional description: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author		date		remarks
*			  	  	        linwl         20150509	created
*****************************************************************/
void sys26(void)
{
	if(auto_flag==2)
		return;
	//����Ϊ�ֲ�����
	int ret = -1;
	unsigned int unLen=0;
	char usnbuf[128] = {0}, psnbuf[128] = {0}, boardver[128] = {0}, boardnum[128] = {0}, sBuf[128] = {0};
#if ANDIROD_ENABLE  //�߶˲�Ʒ�������úͻ�ȡ�ǽ�оƬ����  20180503 sull add
	char rfidtype[128] = {0};
#endif

	//����Ϊ������
	cls_printf("����%s...", TESTAPI);

	//����ǰ��:��ȡ����ǰ���ã��������ָ�
	memset(usnbuf,0,sizeof(usnbuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_USN, &unLen, usnbuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	memset(psnbuf,0,sizeof(psnbuf));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_PSN, &unLen, psnbuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	memset(boardver,0,sizeof(boardver));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_BOARD_VER, &unLen, boardver)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
	memset(boardnum,0,sizeof(boardnum));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_BOARD_NUMBER, &unLen, boardnum)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#if ANDIROD_ENABLE //�߶˲�Ʒ�������úͻ�ȡ�ǽ�оƬ����  20180503 sull add
	memset(rfidtype,0,sizeof(rfidtype));
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_RFID_CHIP_TYPE, &unLen, rfidtype)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		RETURN;
	}
#endif

	//case1: �쳣���ԣ����������psBufΪ��ָ��
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_BOARD_NUMBER, NULL)) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	
	//case2: �쳣���ԣ����������emFlagΪ��֧�ֵ�����Ӧ�÷���ʧ��
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_BIOS_VER, "300")) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_KEY_COUNT, "300")) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_POS_TYPE, "ME30")) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_HARDWARE_INFO, "300")) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_CREDITCARD_COUNT, "300")) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_PRN_LEN, "300")) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_POS_RUNTIME, "300")) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_CPU_TYPE, "300")) != NDK_ERR_PARA)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	//�߶˲�Ʒ�������úͻ�ȡ�ǽ�оƬ����,֧�ִ����������AS3911��MH1608��NORFID  20180503 sull add
#if ANDIROD_ENABLE
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_RFID_CHIP_TYPE, "300")) != NDK_ERR_PARA)  
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if !CPU5810X_ENABLE	//20170111 SP610�°汾���ڰ�ȫ���ǹر�����PN/SN�Ĺ��ܣ����ز�֧�֣����������Ͷ˲�ƷҪ½������������ linwl linshengxin
	//case3:����SYS_HWINFO_GET_POS_USN�����ú��ȡӦ�ú����õ�һ��  //ME50NX��ME68ֻ֧������һ�Σ�����֮�������û᷵��-18 20180416 sull
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_POS_USN, "Us2n!@����")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_USN, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(sBuf, "Us2n!@����"))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, sBuf);
		GOTOERR;
	}
#else
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_POS_USN, "Us2n!@����")) != NDK_ERR_NOT_SUPPORT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

#if !CPU5810X_ENABLE
	//case4:����SYS_HWINFO_GET_POS_PSN   //ME50NX��ME68ֻ֧������һ�Σ�����֮�������û᷵��-18 20180416 sull
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_POS_PSN, "ps2n!@�й�")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_POS_PSN, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(sBuf, "ps2n!@�й�"))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, sBuf);
		GOTOERR;
	}
#else
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_POS_PSN, "ps2n!@�й�")) != NDK_ERR_NOT_SUPPORT)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
#endif

	//case5:����SYS_HWINFO_GET_BOARD_VER
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_BOARD_VER, "ps2n!@�����")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_BOARD_VER, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(sBuf, "ps2n!@�����"))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, sBuf);
		GOTOERR;
	}

	//case6:����SYS_HWINFO_GET_BOARD_NUMBER
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_BOARD_NUMBER, "ps2n!@������")) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_BOARD_NUMBER, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(sBuf, "ps2n!@������"))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, sBuf);
		goto ERR;
	}

	//case7:�߶˲�Ʒ�������úͻ�ȡ�ǽ�оƬ����  20180503 sull add 
#if ANDIROD_ENABLE
	memset(sBuf,0,sizeof(sBuf));
	if((ret=NDK_SysSetPosInfo(SYS_HWINFO_GET_RFID_CHIP_TYPE, rfidtype)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if((ret=NDK_SysGetPosInfo(SYS_HWINFO_GET_RFID_CHIP_TYPE, &unLen, sBuf)) != NDK_OK)
	{
		send_result("line %d:%s����ʧ��(%d)", __LINE__, TESTAPI, ret);
		GOTOERR;
	}
	if(strcmp(sBuf, rfidtype))
	{
		send_result("line %d:%s����ʧ��(%s)", __LINE__, TESTAPI, sBuf);
		goto ERR;
	}
#endif

	//���Խ���
	send_result("%s����ͨ��", TESTAPI);
ERR:
	NDK_SysSetPosInfo(SYS_HWINFO_GET_POS_USN, usnbuf);
	NDK_SysSetPosInfo(SYS_HWINFO_GET_POS_PSN, psnbuf);
	NDK_SysSetPosInfo(SYS_HWINFO_GET_BOARD_VER, boardver);
	NDK_SysSetPosInfo(SYS_HWINFO_GET_BOARD_NUMBER, boardnum);
#if ANDIROD_ENABLE
	NDK_SysSetPosInfo(SYS_HWINFO_GET_RFID_CHIP_TYPE, rfidtype);
#endif
	return;
}


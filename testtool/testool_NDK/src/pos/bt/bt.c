/************************************************************************
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: ����ģ��
* file name			: 
* Author 			: jiangym
* version			: 
* DATE				: 20140217
* directory 			: 
* description			: ɨ��ģ�������,��ģ��Ĺ��������빫������Ҳ���Է��ڴ˴�
* related document	:  NDK.chm
*
************************************************************************/

/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define	TESTMODULE	"����ģ��"

/*------------global variables definition-----------------------*/

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/

/*---------------functions definition---------------------------*/
/****************************************************************
* function name 	 		: bt
* functional description 	: ����ģ�����������,�ɸ�����Ҫ��չ
* input parameter	 		:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  	  	       jiangym			20140217		created
*****************************************************************/
void bt(PF* list, unsigned char id[])
{
	/*private & local definition*/		
	int ret = -1;
	g_UCID = 0;
	/*process body*/
	cls_show_msg1(1, "%s����...", TESTMODULE);	

#if !(defined ME15C||defined ME15CHX||defined ME20||defined ME15B)	//ME15C,ME20��֧��NDK_BTSetType����
#if K21_ENABLE
	char ph_type[3]={0x0c,0x02,0x5a};
	
	if(!g_at_enable)//BM77��֧�������������������� 20150415 jiangym
	{
		if(cls_show_msg1(5,"�Ƿ��ǿ�������ִ�б�����,��[Enter],��[����]")==ENTER)
		{
			if((ret=NDK_BTSetType(ph_type))==NDK_OK/*NDK_ERR_IOCTL*/)//ME30MH����-1,ԭ�ȵķ���-5,�ſ���ֵ�ж� 20170523 modify
			{
				send_result("line %d:NDK_BTSetType����ʧ��(%d)", __LINE__, ret);
				return;
			}
			//����δ��patchʱδopenִ�л�ʧ��
			if((ret=NDK_BTSetDiscoverableStatus(1))==NDK_OK/*NDK_ERR_IOCTL*/)//ME30MH����-1,ԭ�ȵķ���-5,�ſ���ֵ�ж� 20170523 modify 
			{
				send_result("line %d:NDK_BTSetDiscoverableStatus����ʧ��(%d)", __LINE__, ret);
				return;
			}	
#if BTMFI_ENABLE
			//��������δ�� ���������Ƿ�֧��ble���ӷ���ʧ��
			if((ret=NDK_BTSetBleStatus(0))!=NDK_ERR_IOCTL)
			{
				send_result("line %d:����ʧ��(%d)", __LINE__, ret);
				return;
			}
			//��������δ�� ������¼���ֻ���Ϣ��Ŀ����ʧ��
			if((ret=NDK_BTSetMaxBondedDevices(0))!=NDK_ERR_IOCTL)
			{
				send_result("line %d:����ʧ��(%d)", __LINE__, ret);
				return;
			}
#endif
		}
	}

#endif
#endif
	if(conf_conn_BT()==FAIL)
		return;
	
#if BTMFI_ENABLE
	ret=NDK_MFI_CHECK();
	switch(ret)
	{
		case NDK_OK: 
			NDK_BTSetBleStatus(0);//��MFIЭ���£��ر�BLEģʽ�������ֻ����������ʶ���MFI�豸�����Ҹú�������OPEN֮����ã������ƶ�������֮��
			break;
		case NDK_ERR_IOCTL:
			cls_show_msg("Ӳ������,����Ӳ������ʦȷ��");
			return;
			break;
		case NDK_ERR_OPEN_DEV:
			cls_show_msg("����֧��CPоƬ,�������������");
			break;
		case NDK_ERR_NOT_SUPPORT:
			cls_show_msg("BM77������֧�ִ˺���,�������������");
			break;
		default:
			cls_show_msg("δ֪�ķ���ֵ:%d",ret);
			break;
	}
#endif

	//ִ�в�������
#if STABAR_ENABLE
	if((ret=NDK_ScrStatusbar(STATUSBAR_DISP_TIME|STATUSBAR_DISP_BLUETOOTH))!=NDK_OK)
	{
		send_result("״̬����ʾʧ��!(%d)", ret);
		return;
	}
#endif
	process(list, id);
#if STABAR_ENABLE	
	NDK_ScrStatusbar(STATUSBAR_DISP_CLOSE);
#endif
	
	return;
}


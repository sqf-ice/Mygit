/************************************************************************
*
* copyright(c)2012-20xx Newland Co. Ltd. All rights reserved
* module			: RTC�ۺϲ���
* file name			: 30.c
* Author 			: linwei
* version			: 
* DATE				: 20130121
* directory 		: 
* description		: RTC�ۺϲ���
* related document	: NDK.chm
*
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"
#include "systest.h"

/*---------------constants/macro definition---------------------*/
#define	TESTITEM	"RTC�ۺ�"

#define	BASE_YEAR		(1900)
#define	BASE_MON		(1)

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
* history 		 		: author			date			remarks
*			  		      linwei	        20130121	 	created
*****************************************************************/
void systest10(void)
{
	if(auto_flag==1)//��֧���Զ�����
	{
		cls_show_msg_record(FILENAME ,FUNCNAME ,"%s��֧���Զ����ԣ���ʹ���ֶ�������֤", TESTITEM);
		return;
	}

	//����Ϊ�ֲ�����
	int ret = -1, keyin = 0;
	struct tm stOldPosTime;
	struct tm stExpect;
	struct tm stNew_time;

	//����Ϊ������
	cls_show_msg1(1, "%s������...", TESTITEM);
	if(cls_show_msg("��������Ҫ��rootȨ���½���,[Enter]����,��������" )!=ENTER)
	{
		cls_show_msg("line %d:%s�����ж�", __LINE__, TESTITEM);
		return;
	}
	memset(&stOldPosTime, 0, sizeof(stOldPosTime));
	memset(&stExpect, 0, sizeof(stExpect));
	memset(&stNew_time, 0, sizeof(stNew_time));

	//���ú���ʱ��(��BCD����,int��)
	//E90֧�ֵ����:2010-2099; E90G:2000-2030
	//�ۺϸ�����,ѡ��2025��12��31����Ϊ����ֵ
	stNew_time.tm_year = stExpect.tm_year = 2025 - BASE_YEAR;
	stNew_time.tm_mon = stExpect.tm_mon = 12 - BASE_MON;
	stNew_time.tm_mday = stExpect.tm_mday = 31;
	stNew_time.tm_wday = stExpect.tm_wday = 3;
	stNew_time.tm_hour = stExpect.tm_hour = 0x17;
	stNew_time.tm_min = stExpect.tm_min = 0x37;
	stNew_time.tm_sec = stExpect.tm_sec = 0x03;
	if((ret=NDK_SysGetPosTime(&stOldPosTime)) != NDK_OK)
	{
		cls_show_msg("line %d:��ȡʱ��ʧ��", __LINE__);
		return;
	}
	if (stOldPosTime.tm_year == stNew_time.tm_year)
	{
		cls_show_msg("���Ƚ�ϵͳʱ���Ϊ��%04d��,������˳�...", stNew_time.tm_year+BASE_YEAR);
		return;
	}
	if((ret=NDK_SysSetPosTime(stNew_time)) != NDK_OK)
	{
		cls_show_msg("line %d:����ʱ��ʧ��", __LINE__);
		return;
	}
	cls_show_msg("ʱ��Ӧ�ѵ���Ϊ%04d��%02d��%02d��%02d:%02d,���������...", stExpect.tm_year+BASE_YEAR, stExpect.tm_mon+BASE_MON, stExpect.tm_mday, stExpect.tm_hour, stExpect.tm_min);
	
	while(1)
	{
		keyin = cls_show_msg("��ѡ����:\n"
							"1.�˳�����\n"
							"2.�ػ�������������\n"
							"3.��������\n");

		switch(keyin) 
		{
		case '1':
			cls_show_msg1(3, "�ֶ��˳������,���˹�У������ʱ��!\n");
			return;
			break;
		case '2':
#if !(ANDIROD_ENABLE)
			cls_show_msg("��������ػ�������������,���˹�У������ʱ��!\n");
			NDK_SysShutDown();
#else
			cls_printf("����Ʒ��֧���Զ��ػ�����,���ֶ��ػ�����������,���˹�У������ʱ��!\n");
			while(1);
#endif	
			break;
		case '3':
#if defined ME30||defined ME30S||defined ME30THM||defined ME30MH||defined ME32||defined ME65||defined ME30GX||defined ME32GX||defined ME32THM||defined ME11||defined ME15||defined ME20||defined ME15C||defined ME15CHX||defined ME15B||defined ME66||CPU5810X_ENABLE||defined ME50NX||defined ME68||defined ME50S||defined ME50NS //ME30��Ҫ���ŵ�Դ�߲�����������������ʾ
			cls_show_msg("����ϵ�Դ�ߺ󣬰����������\n");
#endif
			cls_show_msg1(3, "����������,���˹�У������ʱ��!\n");
			NDK_SysReboot();
			break;
		default:
			continue;
			break;
		}
	}
	
	return;
}


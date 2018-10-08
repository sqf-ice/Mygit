/************************************************************************
*
* copyright(c)2005-2008 Newland Co. Ltd. All rights reserved
* module			: PPPͨѶ
* file name		: 1.c
* Author 			: jiangwx
* version			: V1.0
* DATE			: 20061026
* directory 		: .\SRC\net\
* description		: ����pppOpen�ܷ񿪱�һ���µ�PPPͨ��
* related document	: ����Ա�ο��ֲ�
*
************************************************************************
* log			: 
* Revision 1.0  2006.10.26 jiangwx
* no message(created for ARM250 platform)
************************************************************************/
/*-----------------------includes-------------------------------*/
#include "mode.h"

/*---------------constants/macro definition---------------------*/
#define  UCID  1
#define	TESTAPI		"pppOpen"
#define	MAXTIMEOUT	60

/*------------global variables definition-----------------------*/
static float fTimeElapsed = 0;
static int (*ppp_subcase)(void) = NULL;

/*----------global variables declaration("extern")--------------*/

/*---------------structure definition---------------------------*/

/*---------------functions declaration--------------------------*/
extern int IsPPPTimeout(int ph, int state);

static int subcase_1(void);
static int subcase_2(void);
static int subcase_3(void);
static int subcase_4(void);

/*---------------functions definition---------------------------*/
//��1��subcase:���Դ�����PPPͨ��-1	       
static int subcase_1(void)
{
	/*private & local definition*/
	int p = 0;
	
	/*process body*/
	//����PPP����ǰ����
	//ע��:pppClose�����������·Ҳ�Ͽ�,ÿ�ε���pppClose��,�ٵ���pppOpen��Ҫ����LinkUp
	if(LinkUp()!=SUCC)
	{
		send_result("line %d:������·ʧ��", __LINE__);
		return FAIL;
	}

	if ((p=pppOpen(-1))<0)                                                                                           	                                                                                                                                                        	
	{
		send_result("line %d:%s����ʧ��(err=%d)", __LINE__, TESTAPI, getPPPerrorcode(p));
		LinkDown();
		return FAIL;                                                                                                  	                                                                                                                                                        	
	}
	ppp_subcase = subcase_2;//������һ�ڵ�
	return SUCC;
}

//���Դ�����PPPͨ��0	          
static int subcase_2(void)
{
	/*private & local definition*/
	int p = 0;
	fTimeElapsed = 0;
	
	/*process body*/
	//����PPP����ǰ����
	//ע��:pppClose�����������·Ҳ�Ͽ�,ÿ�ε���pppClose��,�ٵ���pppOpen��Ҫ����LinkUp
	if(LinkUp()!=SUCC)
	{
		send_result("line %d:������·ʧ��", __LINE__);
		return FAIL;
	}

	StartStopwatch();
	if ((p=pppOpen(0))<0)                                                                                            	                                                                                                                                                        	
	{
		send_result("line %d:%s����ʧ��(err=%d)", __LINE__, TESTAPI, getPPPerrorcode(p));
		LinkDown();
		return FAIL;                                                                                          	                                                                                                                                                        	
	}
	fTimeElapsed = ReadStopwatch();
	ppp_subcase = subcase_3;//������һ�ڵ�
	return SUCC;
}

//���Դ�����PPPͨ��1024	      	          
static int subcase_3(void)
{
	/*private & local definition*/
	int p = 0;
	
	/*process body*/
	//����PPP����ǰ����
	//ע��:pppClose�����������·Ҳ�Ͽ�,ÿ�ε���pppClose��,�ٵ���pppOpen��Ҫ����LinkUp
	if(LinkUp()!=SUCC)
	{
		send_result("line %d:������·ʧ��", __LINE__);
		return FAIL;
	}
			
	if ((p=pppOpen(1024))<0)                                                                                         	                                                                                                                                                        	
	{
		send_result("line %d:%s����ʧ��(err=%d)", __LINE__, TESTAPI, getPPPerrorcode(p));
		LinkDown(); 
		return FAIL;                                                                                                  	                                                                                                                                                        	
	}	                                         
	ppp_subcase = subcase_4;//������һ�ڵ�
	return SUCC;
}

//�������ͨ����,ĿǰΪ1           	          
static int subcase_4(void)
{
	/*private & local definition*/
	int p = 0, p1 = 0;//Ŀǰֻ֧��һ��PPPͨ��,ϵͳ���һ��Ϊ0
	
	/*process body*/
	//����PPP����ǰ����
	//ע��:pppClose�����������·Ҳ�Ͽ�,ÿ�ε���pppClose��,�ٵ���pppOpen��Ҫ����LinkUp
	if(LinkUp()!=SUCC)
	{
		send_result("line %d:������·ʧ��", __LINE__);
		return FAIL;
	}
				
	if ((p=pppOpen(1))<0)                                                                                            	                                                                                                                                                        	
	{
		send_result("line %d:%s����ʧ��(err=%d)", __LINE__, TESTAPI, getPPPerrorcode(p));
		LinkDown();
		return FAIL;                                                                                                  	                                                                                                                                                        	
	}	                                                                                                         	                                                                                                                                                        	
	if ((p1=pppOpen(1024))>=0)                                                                                        	                                                                                                                                                        	
	{                                                                         	                                                                                                                                                        	
		send_result("line %d:%s����ʧ��(err=%d, hd=%d)", __LINE__, TESTAPI, getPPPerrorcode(p1), p1);
		LinkDown(); 
		return FAIL;                                                                                                	                                                                                                                                                        	
	}	                                            
	ppp_subcase = NULL;//������һ�ڵ�
	return SUCC;
}

static int resume(void)
{
	/*private & local definition*/
	
	/*process body*/
	LinkDown();    
	send_result("%s����ͨ��(��ʱ%fs)", TESTAPI, fTimeElapsed);                                                               	                                                                                                                                                        	                                                                                                  	                                                                                                                                                        	
	return SUCC;	
}

static int handle_subcase(void)
{
	/*private & local definition*/

	/*process body*/
	if(ppp_subcase!=NULL)
		return ppp_subcase();
	else	//test over
	{
		resume();
		return (-2);
	}
}

/****************************************************************
* function name 	 	: 
* functional description 	: 
* input parameter	 	:
* output parameter	 	: 
* return value		 	:
* history 		 		: author			date			remarks
*			  			  jiangwx		   2006.9.26	 	created
*						lwl		         20080922		recreated
*****************************************************************/
void ppp1(void)
{
	/*private & local definition*/
	int subcase = 0, p = 0, ret = 0;
	int tpstatus = PHASE_DEAD;
	ppp_subcase = subcase_1;//����ǰ�ָ��׽ڵ�
	
	/*process body*/
	cls_printf("����%s...", TESTAPI);

	while (1)
	{
		delay(10);//����ʱ��Ϊ�˽���������״̬��оƬ��ѹ��
		tpstatus = GetPPPphase(p);

		switch (tpstatus)
		{
		case PHASE_INITIALIZE:  //PPP���ڳ�ʼ��                                                                                                  	
			cls_show_msg1(2, "PPP��ʼ����...\n");
		case PHASE_DEAD://ppp�ر�                                                                                                                	
			cls_show_msg1(2, "PPP���ڳ�ʼ̬(%d#������)...\n", ++subcase);			                                                                                         	                                                                                                                  	

			ret = handle_subcase();
			if(ret==FAIL)
				return;
			else if(ret==SUCC)
				;//break;
			else	//-2
				return;
			
			break;                                                                                                                           	                                                                                                                       	
		case PHASE_ESTABLISH: //PPP��������                                                                                                      	
			cls_show_msg1(2, "PPP�Ѵ���,��ȡ�������Ϣ��...\n");
			if(IsPPPTimeout(p, PHASE_ESTABLISH))
			{
				send_result("line %d:״̬������,һֱ���ڴ���̬(subcase:%d,  err=%d)", __LINE__, subcase, getPPPerrorcode(p));
				pppClose(p);//�򵥴�����һ��,�������Ƿ�ɹ�,��������������                                                             	
				LinkDown();
				return;     				
			}
			break;                                                                                                                           	
		case PHASE_AUTHENTICATE://PPP������֤�û���������                                                                                        	
			cls_show_msg1(2, "PPP��֤�û�����������...\n");
			if(IsPPPTimeout(p, PHASE_AUTHENTICATE))
			{
				send_result("line %d:״̬������,һֱ������֤̬(subcase:%d,  err=%d)", __LINE__, subcase, getPPPerrorcode(p));
				pppClose(p);//�򵥴�����һ��,�������Ƿ�ɹ�,��������������                                                             	
				LinkDown();
				return;     				
			}
			break;                                                                                                                           	
		case PHASE_CALLBACK: //PPP�ظ�����                                                                                                       	
			cls_show_msg1(2, "PPP�ظ�������...\n");
			break;                                                                                                                           	
		case PHASE_NETWORK://PPP��������IP��ַ                                                                                                   	
			cls_show_msg1(2, "PPP�ѽ�������,��ȡIP��...\n");
			if(IsPPPTimeout(p, PHASE_NETWORK))
			{
				send_result("line %d:���ػ�ȡ��̬IPʧ��(subcase:%d, err=%d)", __LINE__, subcase, getPPPerrorcode(p));
				pppClose(p);//�򵥴�����һ��,�������Ƿ�ɹ�,��������������                                                             	
				LinkDown();
				return;                                                                                                                  	
			}
			//�ɹ��Ļ�,�ر�PPP,��ʼ��һ�ֵĲ���                                                                                          	
                     //����close�ĳɹ�����Ӱ����һ�ε�open,������һ��close�Ƿ�ɹ�                                                          	
			if ((ret=pppClose(p))<0)                                                                                               	
			{
				send_result("line %d:pppCloseʧ��(0x%, subcase:%d,  err=%d)", __LINE__, ret, subcase, getPPPerrorcode(p));
				LinkDown();
				return;
			}

			//PPPCLOSE��,�����Ѵ���LINKDOWN״̬,��Լ40s,�Զ˲Ż���ȫ�Ҷ�
			//��������LinkDownһ��,״̬������Զ�ͬ��,��Ӱ����һ�ֲ���
			LinkDown();
			break;	                                                                                                                         	
		case PHASE_TERMINATE://ppp���ڶϿ�                                                                                                       	
			cls_show_msg1(2, "ppp���ڶϿ�...");                                                                                                        	                                                                                                   	
			break;                                                                                                                           	
		default://δ֪״̬����Ϊ��	
			send_result("line %d: UNKNOWN STATE(tpstatus=%d, err=%d)", __LINE__, tpstatus, getPPPerrorcode(p));
			pppClose(0);//Ϊ�˷�ֹopen�ɹ���,����δ֪״̬,������return,����open���ľ��(Ŀǰֻ�ܴ�һ����ֵΪ0)                      	
			LinkDown();
			return;                
			break;
		}
	}
}


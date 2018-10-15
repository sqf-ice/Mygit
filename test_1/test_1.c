#include  "NDK.h"
#include "kb.h"

#include "libdevmgr.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h> 
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <linux/input.h>

#define DEBUG
#undef DEBUG
#ifdef DEBUG
#define ic_printf(fmt, arg...)  printf(fmt, ##arg)
#else
#define ic_printf(fmt, arg...)    
#endif

#define LOOPBACK_VER	"EMV L1 2.1.0"

typedef struct			//	Define ATR Structural parameters,just use "T" now.
{
/*	unsigned long WWT;
	unsigned long CWT;
	unsigned long BWT;
	unsigned int F;
	unsigned char D;
	unsigned char IFSC;
	unsigned char NAD;
	unsigned char GTR;
	unsigned char wtx;
*/	unsigned char T;
/*	unsigned char cwi;

	unsigned char IFD_Seq;
	unsigned char ICC_Seq;
	unsigned char IFSD_Negotiated;
	unsigned char ATROK;
	unsigned char Vcc;
*/
}atr_t;
extern atr_t g_atr;

typedef struct 
{
	unsigned char data[512];	//  261  'CLA INS P1 P2 [Lc + Lc data] [Le]' for command or data
	int length;					// '[Le data] sw1 sw2' for response (max = 256+2)
}T_APDU;
T_APDU C_Apdu, R_Apdu;
#define LEN 0x08    // define the data length receive from the IC card
#define TypeAPDU	R_Apdu.data[3]
#define LC			R_Apdu.data[4]
#define LE			R_Apdu.data[5]	
const uint8_t GetChallenge[]={0x00, 0x84, 0x00, 0x00, LEN};
const uint8_t GetChallenge_err[]={0x00, 0x84, 0x00, 0x00, 0x06, 0x04};		//define error C-APDU command header for test.
//const uint8_t card_type;
/*
const uint8_t SelectPse[] = {0x00, 0xA4, 0x04, 0x00, 0x0E,
		'1','P','A','Y','.','S','Y','S','.','D','D','F','0','1',
		0x00};
*/

/**********************************************************************************
 *@Function £∫uint8_t ComputerLrc(uint16_t len,uint8_t *p)
 *@Input :    uint16_t len,uint8_t *p
 *@Output :   lrc
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
 **********************************************************************************/
uint8_t ComputerLrc(uint16_t len,uint8_t *p)
{
	uint8_t lrc = 0;
	uint16_t i = 0;
	while(i <len)
		lrc ^= p[i++];
	return lrc;
}

/**********************************************************************************
 *@Function £∫InitGetChallenge(T_APDU *apdu)
 *@Input :    T_APDU *apdu
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
***********************************************************************************/
void InitGetChallenge(T_APDU *apdu)
{
	memcpy(apdu->data,GetChallenge,5);
	apdu->length = 5;
}


/**********************************************************************************
 *@Function £∫void InitGetChallenge_err(T_APDU *apdu)
 *@Input :    T_APDU *apdu
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
***********************************************************************************/
void InitGetChallenge_err(T_APDU *apdu)
{
	memcpy(apdu->data,GetChallenge_err,6);
	apdu->length = 6;
}
/**********************************************************************************
 *@Function £∫int get_quit_test(void)
 *@Input :    no
 *@Output :   0 or 1
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
***********************************************************************************/
/*when return 1 then quit test,other return 0*/
int get_quit_test(void)
{
	int key = 0;
	//NDK_KbGetCode(0, &key);
	NDK_KbHit(&key);
	if(key == ESC) return 1;
	else return 0;
}

/**********************************************************************************
 *@Function £∫void emv_wait_continue(void)
 *@Input :    no
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
**********************************************************************************/
void emv_wait_continue(void)
{
	int i = 0;
	int key = 0;
	int ret = 0;
	int time = 0;
	int rece_len = 0;
	uint8_t rbuf[40]={0};
	uint8_t run_num = 0;
//	uint8_t card_type
	ic_printf("start wait_continue test \n");
//	NDK_ScrDispString(0, 12, "input delay s 00-99 ", 0);
	EM_ICTYPE card_type = ICTYPE_M_1;
	card_type = card_type_choose();
	NDK_SysMsDelay(50);
	while(1){
		if((ret = card_to_powerup())<0) break;
		/********waiting change******/
		switch (g_atr.T){
			case 0:
				ret = rw_wait_rw(card_type);
				NDK_ScrPrintf(" ºÃ–¯≤‚ ‘T1ø®£∫\n");
				break;
			case 1:
				ret = rw_wait_rw(card_type);
				break;
			default:
				break;
			}
		if(ret < 0){
			ret = NDK_IccPowerDown(card_type);
			NDK_ScrDispString(0, 24, "power down  ", 0);
			break;
		}
	}
}
/******************************************************************************
 *@Function £∫rw_wait_rw(card_type)
 *@Input :    card_type
 *@Output :   read and write results
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
******************************************************************************/
int rw_wait_rw(card_type)
{
	int ret=0;
	ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data);
		if(ret < 0){
			ic_printf("apdu err %d \r\n",ret);
			NDK_ScrDispString(0,12," apdu err  ",0);
			return ret;
		}
		else{
			NDK_SysMsDelay(200);
			ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data);
			if(ret < 0){
				NDK_ScrPrintf(" ≤‚ ‘ ß∞‹\n");
				return ret;
			}
			NDK_ScrPrintf(" ≤‚ ‘Õ®π˝\n");
//			ic_printf("icc power up succ \r\n");
		}
	return ret;
}

/***************************************************************************
 *@Function £∫int read_write_T1(card_type)
 *@Input :    card_type
 *@Output :   read and write results
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
***************************************************************************/

int read_write_T1(card_type)
{
	int ret = 0;
	ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data);
	return ret;
}


/*************************************************************************
 *@Function £∫read_write_T0(card_type)
 *@Input :    card_type
 *@Output :   read and write results
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
************************************************************************/

int read_write_T0(card_type)
{
	int ret = 0;
	ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data);
	return ret;
}

/************************************************************************
 *@Function £∫void mov_card_dur_pow(card_type)
 *@Input :    card_type
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
************************************************************************/
void mov_card_dur_pow(card_type)
{
	int i = 0;
	int ret = 0;
	int rece_len = 0;
	uint8_t rbuf[40] = {0,};
	while(1){
		if((ret = NDK_IccPowerUp(card_type,rbuf, &rece_len)) < 0){
			ic_printf("icc power up err %d\r\n",ret);
			NDK_ScrDispString(0,24," pow up err ",0);
			break;
		}
		NDK_ScrDispString(0, 12, "move the card ! ", 0);
		for (i = 0; i <= 100; i++){
			//provide the time to move the card during doing something.
			if((ret = NDK_IccPowerUp(card_type,rbuf, &rece_len)) < 0){
				NDK_ScrPrintf(" mov car pow up err \n");
				NDK_ScrPrintf(" ≤‚ ‘Õ®π˝\n");
//				NDK_ScrDispString(0,24," mov car pow up err ",0);
				break;
			}
		}
		if(ret > 0){
			NDK_ScrPrintf(" pow up succ \n");
			NDK_ScrPrintf(" ≤‚ ‘ ß∞‹\n");
		}
		break;
	}

}

/*************************************************************************
 *@Function £∫mov_card_aft_pow(card_type)
 *@Input :    card_type
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
*************************************************************************/
void mov_card_aft_pow(card_type)
{
	int ret = 0;
	int rece_len = 0;
	uint8_t rbuf[40] = {0,};
	while(1){
		if((ret = NDK_IccPowerUp(card_type,rbuf, &rece_len)) < 0){
			ic_printf("icc power up err %d\r\n",ret);
			NDK_ScrDispString(0,24," pow up err ",0);
			break;
		}
		NDK_SysMsDelay(20);
		NDK_ScrDispString(0,24," mov the car ",0);
		NDK_SysMsDelay(50);
		ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data);
		if(ret < 0){
			NDK_ScrPrintf(" rw error \n");
			NDK_ScrPrintf(" ≤‚ ‘≥…π¶\n");
			break;
		}
		NDK_ScrPrintf(" rw succ \n");
		NDK_ScrPrintf(" ≤‚ ‘ ß∞‹\n");
		break;
	}
}


/*************************************************************************
 *@Function £∫void mov_card_dur_rw(card_type)
 *@Input :    card_type
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
*************************************************************************/
void mov_card_dur_rw(card_type)
{
	int i = 0;
	int ret = 0;
	int rece_len = 0;
	uint8_t rbuf[40] = {0,};
	while(1){
		if((ret = NDK_IccPowerUp(card_type,rbuf, &rece_len)) < 0){
			ic_printf("icc power up err %d\r\n",ret);
			NDK_ScrDispString(0,24," pow up err ",0);
			break;
		}
		NDK_SysMsDelay(20);
		ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data);
		if(ret < 0){
			NDK_ScrDispString(0,24," rw error ",0);
			break;
		}
		NDK_ScrDispString(0, 12, "move the card ! ", 0);
		for(i = 0; i <= 100; i++){
			if((ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data)) < 0){
				NDK_ScrPrintf(" mov car rw err \n");
				NDK_ScrPrintf(" ≤‚ ‘Õ®π˝\n");
				break;
			}
		}
		if(ret > 0){
			NDK_ScrPrintf(" mov car rw succ \n");
			NDK_ScrPrintf(" ≤‚ ‘ ß∞‹\n");
		}
		break;
	}
}

/*************************************************************************
 *@Function £∫void rw_aft_mov_in(card_type)
 *@Input :    card_type
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
************************************************************************/
void rw_aft_mov_in(card_type)
{
	int ret = 0;
	int rece_len = 0;
	uint8_t rbuf[40] = {0,};
	while(1){
		if((ret = NDK_IccPowerUp(card_type,rbuf, &rece_len)) < 0){
			ic_printf("icc power up err %d\r\n",ret);
			NDK_ScrDispString(0,24," pow up err ",0);
			break;
		}
		NDK_ScrDispString(0,24," pow up succ ",0);
		NDK_SysMsDelay(50);
		NDK_ScrDispString(0,24," mov and insert card ",0);
		NDK_SysMsDelay(100);  //waiting for mov and insert the card
		if((ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data)) < 0){
			NDK_ScrPrintf(" rw aft mov in err \n");
			NDK_ScrPrintf(" ≤‚ ‘Õ®π˝\n");
			break;
		}
		NDK_ScrPrintf(" rw aft mov in succ \n");
		NDK_ScrPrintf(" ≤‚ ‘ ß∞‹\n");
		break;
	}
}

/*************************************************************************
 *@Function £∫int card_to_powerup(card_type)
 *@Input :    card_type
 *@Output :   power up result
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
*************************************************************************/
int card_to_powerup(card_type)
{
	int ret = 0;
	int rece_len = 0;
	int expectedCharacters = 0;
	g_atr.T = 0;	//	Initialize the protocol code
	uint8_t rbuf[40] = {0,};
	NDK_IccDetect(&ret);
	if((ret & 0x01) == 0x00){
		ic_printf("please insert iccard \r\n");
		NDK_ScrDispString(0,24," no card ",0);
		NDK_SysMsDelay(1500);
		return -1;
	}
	NDK_SysMsDelay(100);
	if((ret = NDK_IccPowerUp(card_type,rbuf, &rece_len)) < 0){
		ic_printf("icc power up err %d\r\n",ret);
		NDK_ScrDispString(0,24," pow up err ",0);
		return -1;
	}
	else{
		ic_printf("icc power up succ \r\n");
		NDK_ScrDispString(0,24," pow up succ",0);
		expectedCharacters = rbuf(1) & 0xF0;         /* Figure out upcoming protocol conf bytes */
		if (expectedCharacters & 0x80){
			/* TD exist or not  */
			g_atr.T = 1;
		}
		return ret;
}

/*************************************************************************
 *@Function £∫int card_type_choose(void)
 *@Input :    no
 *@Output :   card_type
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
*************************************************************************/
int card_type_choose(void)
{
	int i = 0;
	int key = 0;
//	ic_printf("start wait_continue test \n");
//	NDK_ScrDispString(0, 12, "input delay s 00-99 ", 0);
	EM_ICTYPE card_type = ICTYPE_M_1;
	ic_printf("choose card type \n");
	while(1){
		NDK_ScrClrs();
		NDK_ScrPrintf(" «Î—°‘Ò±æ¥Œ≤‚ ‘µƒø®£∫\n");
		NDK_ScrPrintf(" 1°¢IC\n");
		NDK_ScrPrintf(" 2°¢SAM1\n");
		NDK_ScrPrintf(" 3°¢SAM2\n");
		NDK_ScrPrintf(" 4°¢SAM3\n");
		NDK_KbGetCode(0, &key);
		switch(key){
			case ONE: 	card_type = ICTYPE_IC; break;
			case TWO: 	card_type = ICTYPE_SAM1; break;
			case THREE: card_type = ICTYPE_SAM2; break;
			case FOUR: 	card_type = ICTYPE_SAM3; break;
			default:	card_type = ICTYPE_M_1;	break;
		}
		if(card_type != ICTYPE_M_1) break;
	}
	return card_type;
}

/************************************************************************
 *@Function £∫void T1_to_T0_test(void)
 *@Input :    no
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
************************************************************************/
void T1_to_T0_test(void)
{
	int key = 0;
	int ret = 0;
	ic_printf("start wait_continue test \n");
//	NDK_ScrDispString(0, 12, "input delay s 00-99 ", 0);
	EM_ICTYPE card_type = ICTYPE_M_1;
	card_type_choose();
	NDK_SysMsDelay(50);
	while(1){
		if((ret = card_to_powerup())<0) break;
		NDK_SysMsDelay(100);
		if((ret = read_write_T1()) < 0){
			NDK_ScrDispString(0,24," rw T1 error ",0);
			break;
		}
		NDK_SysMsDelay(20000);  //after rw t1 card , waiting 20s .....
		if((ret = read_write_T0()) < 0){
			NDK_ScrDispString(0,24," ≤‚ ‘ ß∞‹ ",0);
			break;
		}
		NDK_ScrDispString(0,24," ≤‚ ‘Õ®π˝ ",0);
		break;
	}
}

/***********************************************************************
 *@Function £∫void pow_mov_rw(void)
 *@Input :    no
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
***********************************************************************/
void pow_mov_rw(void)
{
	int i = 0;
	int ret = 0;
	int key = 0;
	ic_printf("start wait_continue test \n");
	EM_ICTYPE card_type = ICTYPE_M_1;
	card_type = card_type_choose();
	NDK_SysMsDelay(50);
	while(1){
		NDK_ScrClrs();
		NDK_ScrPrintf(" «Î—°‘Ò≤‚ ‘—˘¿˝£∫\n");
		NDK_ScrPrintf(" 1°¢mov card dur pow\n");
		NDK_ScrPrintf(" 2°¢mov card aft pow\n");
		NDK_ScrPrintf(" 3°¢mov card dur rw\n");
		NDK_ScrPrintf(" 4°¢rw aft mov_in card\n");
		NDK_KbGetCode(0, &key);
		switch(key){
			case ONE:
				mov_card_dur_pow();
				break;
			case TWO:
				mov_card_aft_pow();
				break;
			case THREE:
				mov_card_dur_rw();
				break;
			case FOUR:
				rw_aft_mov_in();
				break;
//			default:card_type = ICTYPE_M_1;	break;
		}
		break;
	}
	NDK_ScrDispString(0, 12, "move the card ! ", 0);
	NDK_SysMsDelay(50);
	while(1){
		ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data);
		if(ret < 0){
			NDK_ScrDispString(0,24," ≤‚ ‘≥…π¶ ",0);
			break;
		}
		NDK_ScrDispString(0,24," ≤‚ ‘ ß∞‹ ",0);
	}
}

/************************************************************************
 *@Function £∫void rec_sen_rate(void)
 *@Input :    no
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
************************************************************************/
void rec_sen_rate(void)
{
	int ret = 0;
	int key = 0;
	int pstTime = 0;
	int start_time = 0;
	int end_time = 0;
	int i = 0;
	int time_length = 20; // Set measurement time
	int r_w_efficiency = 0;
	int num = 0;
	int ulTime = 0;
	ic_printf("start rev sen rate test \n");
	EM_ICTYPE card_type = ICTYPE_M_1;
	card_type = card_type_choose();
	NDK_SysMsDelay(50);
	InitGetChallenge(&C_Apdu);
//	start_time = NDK_SysGetPosTime(pstTime);
	while(1){
		if((ret = NDK_IccPowerUp(card_type,rbuf, &rece_len)) < 0){
			ic_printf("icc power up err %d\r\n",ret);
			NDK_ScrDispString(0,24," pow up err ",0);
			break;
		}
	}
	start_time = NDK_SysTime(ulTime);
	while(1){
		i++;
		ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data);
		if(ret < 0){
			NDK_ScrDispString(0,24," apdu err ",0);
			break;
		}
		end_time = NDK_SysTime(ulTime);
//		end_time = NDK_SysGetPosTime(pstTime);
		if(((ret = end_time - start_time)) >= time_length) break;
	}
    num = i * (LEN+2);
    r_w_efficiency = num / ret;
    ic_printf("r_w_efficiency = %d \r\n", r_w_efficiency);
    NDK_ScrPrintf(" r_w_efficiency = %s\n", r_w_efficiency);
	/******** waiting *******/
}

/************************************************************************
 *@Function £∫void standard_C_APDU(void)
 *@Input :    no
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
************************************************************************/
void standard_C_APDU(void)
{
	int ret = 0;
	InitGetChallenge_err(&C_Apdu);
	while(1){
		ret = NDK_Iccrw(card_type,C_Apdu.length, C_Apdu.data, &R_Apdu.length, R_Apdu.data);
		if(ret < 0){
			NDK_ScrPrintf(" C-APDU or R-APDU err \n");
			NDK_ScrPrintf(" ≤‚ ‘Õ®π˝ \n");
			break;
		}
/***********************waiting for change***************/
		NDK_ScrPrintf(" APDU succ \n");
		NDK_ScrPrintf(" ≤‚ ‘ ß∞‹\n");
		break;
	}
}

/************************************************************************
 *@Function £∫void emv_test(void)
 *@Input :    no
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
************************************************************************/
void emv_test(void)
{
	int key=0;
	char ver[30];
	while(1){
		NDK_ScrClrs();
    	NDK_ScrPrintf(" 1.waiting_test\n");
    	NDK_ScrPrintf(" 2.T1_to_T0\n");
    	NDK_ScrPrintf(" 3.card_move\n");
    	NDK_ScrPrintf(" 4.rate_test\n");
    	NDK_ScrPrintf(" 5.standard_C_APDU\n");
		NDK_KbGetCode(0,&key);
		switch(key){
			case ONE :
				NDK_ScrClrs();
				NDK_ScrPrintf("wait_continuing....\n");
				emv_wait_continue();
			break;
			case TWO :
				NDK_ScrClrs();
	    		NDK_ScrPrintf(" T1_T0_test......\n");
				ic_printf(" T1_T0_test......\n");
				T1_to_T1_test();
			break;
			case THREE:
				NDK_ScrClrs();
				NDK_ScrPrintf(" card moving test.....\n");
				pow_mov_rw();
			break;
			case FOUR:
				NDK_ScrClrs();
				NDK_ScrPrintf(" rate_test......\n");
				rec_sen_rate();
			break;
			case FIVE:
				NDK_ScrClrs();
				NDK_ScrPrintf(" standard C-APDU_test......\n");
				standard_C_APDU();
			break;
			case ESC :
				NDK_ScrClrs();
				NDK_ScrPrintf(" quit test! \n");
				ic_printf("quit test \n");
				return ;
			break;
			default:	
			break;
		}
	}
}

/****************************************************************************
 *@Function £∫int main(int arg_t,char **arg_v)
 *@Input :    int arg_t,char **arg_v
 *@Output :   no
 *@Brief £∫
 *@Author £∫
 *@Date £∫
 *@Version £∫
****************************************************************************/
int main(int arg_t,char **arg_v)
{
	int Oldauto = 0;
	 /*≥ı ºªØ”¶”√ΩÁ√Ê*/
    if (NDK_ScrInitGui() != NDK_OK){
        return -1;
    }
	NDK_ScrAutoUpdate(1, &Oldauto);
	NDK_ScrClrs();
	NDK_ScrPrintf("IC EMV Level-1≤‚ ‘...");
	NDK_SysMsDelay(5);
	emv_test();
	return 0;
}

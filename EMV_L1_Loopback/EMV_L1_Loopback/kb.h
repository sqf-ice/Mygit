/*
* �´�½��˾ ��Ȩ����(c) 2011-2015
*
* ͳһƽ̨NDK API
* ��    �ߣ�	��Ʒ������
* ��    �ڣ�	2012-08-17
* ��	����	V1.00
* ����޸��ˣ�
* ����޸����ڣ�
*/
#ifndef __KB_H__
#define	__KB_H__


//���ܼ�
#define  F1			0x01
#define  F2			0x02
#define  F3			0x03
#define  F4			0x04
#define  F5			0x05
#define  F6			0x06
#define	 F7			0x07
#define	 F8			0x08
#define	 F9			0x09
#define  LEFT		F1
#define  RIGHT		F2
#define  UP         0x11
#define  DOWN       0x12
#define  MENU       F3
//ר�ü�
#define  BASP		0x0a    //backspace
#define	 BACKSPACE	BASP
#define  CR			0x0D	//ȷ�ϼ�
#define  ENTER		CR
#define  ESC		0x1B
#define	 ZMK		0x1C	//��ĸ��
#define  DOT		0x2E	//.
//���ּ�
#define ZERO		0x30
#define ONE			0x31
#define TWO			0x32
#define THREE		0x33
#define FOUR		0x34
#define FIVE		0x35
#define SIX			0x36
#define SEVEN		0x37
#define EIGHT		0x38
#define NINE		0x39
//����ATM��
#define	 ATM1		((unsigned char)0xa1)
#define	 ATM2		((unsigned char)0xa2)
#define	 ATM3		((unsigned char)0xa3)
#define	 ATM4		((unsigned char)0xa4)
#define  ATM5		((unsigned char)0xa5)
#define	 ATM6		((unsigned char)0xa6)
#define  ATM7		((unsigned char)0xa7)
#define  ATM8		((unsigned char)0xa8)
//������ֽ��ֽ��
#define	 JZK		0xb1		//��ֽ��
#define	 TZK		0xb2		//��ֽ��

struct event {
    unsigned char type;		/* EVENT_* */
    union {
        struct {
            unsigned char code;	/* key code for KBD event */
            unsigned char value;/* char value */
            unsigned char mode;	/*  bit0: 1, printable key; 0, other key
                                 *  bit7: 1, should replace the former char
                                 */
        } kb;
    } data;
    void * priv;
};

//Ϊ�˱������������߼���IME_BIHUA
#define  ENGLISH IME_ENGLISH	//Ӣ��
#define  PINYIN	IME_NUMPY// ȫƴ
#define  QUWEI	IME_QUWEI	// ��λ
#define  NUMPY	IME_NUMPY	//��ƴ
#define  BIHUA	IME_BIHUA	//�ʻ�
#define  GBK_ENCODE  IME_GBK //����

#define IME_LAST_ONE -1		//��ǰ��һ��

/* key value */
#define BASP_VALUE		BASP
#define ENTER_VALUE		ENTER
#define ESC_VALUE		ESC
#define ZMK_VALUE		ZMK
#define F1_VALUE		F1
#define F2_VALUE		F2
#define F3_VALUE		F3
#define F4_VALUE		F4
#define F5_VALUE		F5
#define F6_VALUE		F6
#define F7_VALUE		F7
#define F8_VALUE		F8
#define F9_VALUE		F9
#define ATM1_VALUE		ATM1
#define ATM2_VALUE		ATM2
#define ATM3_VALUE		ATM3
#define ATM4_VALUE		ATM4
#define ATM5_VALUE		ATM5
#define ATM6_VALUE		ATM6
#define ATM7_VALUE		ATM7
#define ATM8_VALUE		ATM8
#define FORWARD_VALUE		JZK
#define BACKWARD_VALUE		TZK

#define ISCHAR(kb)	(kb.mode & 1)
#define ALTCHAR(kb)	(kb.mode & 0x80)

#define EVENT_KBD	1
#define EVENT_TIMER     2
#define EVENT_MAGCARD	3

#endif


#include "bcm5892_define.h"
#include "bcm5892sc_define.h"
#include "sam.h"
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/kernel.h>

#include <mach/pmb.h>
/*clk���üĴ���ֵ*/
ic_clk_type g_sc_fd_clk[3] =
{
	{0x01,0x0B,0x1F,0x06},
	{0x01,0x05,0x1F,0x06},
	{0x01,0x02,0x1F,0x06},
};

uint8_t reg_read8(uint32_t reg)
{
 	return __raw_readb(reg);
}
void reg_write8(uint32_t reg, uint8_t data)
{
	__raw_writeb(data,reg);
}
uint32_t reg_read32(uint32_t reg)
{
	return __raw_readl(reg);
}
void reg_write32(uint32_t reg,uint32_t data)
{
	__raw_writel(data,reg);
}
/******************************************************************
�������� void ctrl_ic_vcc(unsigned char vcc)
���룺vcc: 1��0
����wno
��������������tda8035��vcccmdֵ��1=�µ磬0=�ϵ�
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
void ctrl_ic_vcc(uint8_t vcc)
{
	uint8_t value = 0;
	value = reg_read8(SMARTCARD_BASE + SC_P_IF_CMD_1);
	value = vcc ? (value |SC_IF_CMD_1_vcc_MASK):(value & (~SC_IF_CMD_1_vcc_MASK));
	reg_write8(SMARTCARD_BASE + SC_P_IF_CMD_1,value);
}
/******************************************************************
�������� void ctrl_ic_io(unsigned char io)
���룺io: 1��0
����wno
��������������tda8035��ioֵ��1=high��0=low
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
void ctrl_ic_io(uint8_t io)
{
	uint8_t value = 0;
	value = reg_read8(SMARTCARD_BASE + SC_P_IF_CMD_1);
	if(io == 0){
		value |= SC_IF_CMD_1_io_MASK;
		reg_write8(SMARTCARD_BASE + SC_P_IF_CMD_1,value);
	}
	else if(io == 1){
		value &= ~SC_IF_CMD_1_io_MASK;
		reg_write8(SMARTCARD_BASE + SC_P_IF_CMD_1,value);
		value = reg_read8(SMARTCARD_BASE + SC_P_UART_CMD_1);
		value |= SC_UART_CMD_1_io_en_MASK;
		reg_write8(SMARTCARD_BASE + SC_P_UART_CMD_1,value);
	}
}
/******************************************************************
�������� void ctrl_sc_rst(unsigned char rst)
���룺rst: 1��0
����wno
��������������tda8035��rstֵ��1=high��0=low
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
void ctrl_ic_rst(uint8_t rst)
{
	uint8_t value =0;	
	value = reg_read8(SMARTCARD_BASE + SC_P_IF_CMD_1);
	value = rst ? (value |SC_IF_CMD_1_rst_MASK) : (value &(~SC_IF_CMD_1_rst_MASK));
	reg_write8(SMARTCARD_BASE+SC_P_IF_CMD_1, value);
}
 /******************************************************************
 �������� void ctrl_ic_pres_pol(unsigned char pres_pol)
 ���룺pres_pol: 1��0
 ����wno
 ��������������tda8035�ı�ʾ���ڵ�presֵ��1=high��ʾ���ڣ�
 0=low��ʾ����
 ���ߣ�guoc
 ���ڣ�2013-02-01
 �桡����1.00
 ******************************************************************/
void ctrl_ic_pres_pol(uint8_t pres_pol)
{
	uint8_t value = 0;
	value = reg_read8(SMARTCARD_BASE + SC_P_IF_CMD_1);
	value = pres_pol ? (value | SC_IF_CMD_1_pres_pol_MASK) : (value & (~SC_IF_CMD_1_pres_pol_MASK));
	reg_write8(SMARTCARD_BASE + SC_P_IF_CMD_1,value);
}
 /******************************************************************
 ��������  int check_iccard_pres(void)
 ���룺no
 ����wint 1=���ڿ���0=�����ڿ���
 �����������жϿ��Ƿ��ڿ�����
 ���ߣ�guoc
 ���ڣ�2013-02-01
 �桡����1.00
 ******************************************************************/
int check_iccard_pres(void)
{
	uint8_t value =0;
	value = reg_read8(SMARTCARD_BASE + SC_P_STATUS_1);
	if((value & SC_STATUS_1_card_pres_MASK) == SC_STATUS_1_card_pres_MASK) return 1;
	else return 0;
}
 /******************************************************************
 ��������  int check_iccard_vccon(void)
 ���룺no
 ����wint 1=���ϵ�0=��û�ϵ�
 �����������жϿ��Ƿ��ϵ�
 ���ߣ�guoc
 ���ڣ�2013-02-01
 �桡����1.00
 ******************************************************************/
int check_iccard_vccon(void)
{
	uint8_t value =0;
	value = reg_read8(SMARTCARD_BASE + SC_P_IF_CMD_1);
	if((value & SC_IF_CMD_1_vcc_MASK) == SC_IF_CMD_1_vcc_MASK) return 0;
	else return 1;
}
/*f=372 d =0\1\2\4*/
/******************************************************************
��������  int set_sc_clk(int d)
���룺int d dΪ���ò�����ֵ������emv2000
����wint -1=�����������0=�ɹ�
��������������ģ���clk��sc_clk��band
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
int ctrl_ic_clk(uint8_t d)
{
	uint8_t value =0;
	if((d != 0) && (d != 1) && (d != 2) && (d != 4)){
		printk("set clk input d is not 0 1 2 4 \n");
		return -1;
	}
	d >>= 1; /* /2 then d = 0 0 1 2 g_sc_fd_clk[d]*/
	reg_write8(SMARTCARD_BASE + SC_P_CLK_CMD,0); /*set clk_en =0*/
	reg_write8(SMARTCARD_BASE + SC_P_PRESCALE,g_sc_fd_clk[d].prescale);
	value |= ((g_sc_fd_clk[d].bauddiv== 31)? 0 : 1);
	value |= (g_sc_fd_clk[d].etudiv -1) << 1;
	value |= (g_sc_fd_clk[d].clkdiv - 1) <<4 ;
	value |= SC_CLK_CMD_clk_en_MASK;
	reg_write8(SMARTCARD_BASE + SC_P_CLK_CMD,value);
	return 0;	
}
/******************************************************************
��������  void close_ic_clk(void)
���룺
����w
�����������ر�clk�����
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
void close_ic_clk(void)
{
	reg_write8(SMARTCARD_BASE + SC_P_CLK_CMD,0); /*set clk_en =0*/
}
/******************************************************************
��������  void rst_ic_module(void)
���룺
����w
������������λ5892��iso7816ģ��
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
void rst_ic_module(void)
{
	uint8_t value = 0;
	/*���Է��ָ�λģ��vcc��io�����һ�������壬����������*/
	reg_gpio_iotr_set_pin_type(GPIO_AUX_SCI0,GPIO_PIN_TYPE_INPUT);
	reg_write8(SMARTCARD_BASE + SC_P_MODE_REGISTER,1);
	//0225
	reg_write8(SMARTCARD_BASE + SC_P_TIMER_CMD,0);
	reg_write8(SMARTCARD_BASE + SC_P_INTR_EN_1,0);
	reg_write8(SMARTCARD_BASE + SC_P_INTR_EN_2,0);
	reg_write8(SMARTCARD_BASE + SC_P_UART_CMD_1,0);
	reg_write8(SMARTCARD_BASE + SC_P_UART_CMD_2,0);
	ctrl_ic_clk(1);/*default d = 1*/
	reg_write8(SMARTCARD_BASE + SC_P_UART_CMD_1,SC_UART_CMD_1_uart_rst_MASK);
	wait_ic_etu_cnt(4);
	value = reg_read8(SMARTCARD_BASE + SC_P_UART_CMD_1);
	if((value & SC_UART_CMD_1_uart_rst_MASK) == SC_UART_CMD_1_uart_rst_MASK){
		reg_write8(SMARTCARD_BASE + SC_P_UART_CMD_1,SC_UART_CMD_1_uart_rst_MASK);
	}
	else{
		wait_ic_etu_cnt(10);
	}
	reg_write8(SMARTCARD_BASE + SC_P_PROTO_CMD,(SC_PROTO_CMD_rbuf_rst_MASK | SC_PROTO_CMD_tbuf_rst_MASK));
	reg_gpio_iotr_set_pin_type( GPIO_AUX_SCI0, GPIO_PIN_TYPE_ALTERNATIVE_FUNC0 );

//add by qiut in 20017.03.13 to get atr successfully from the IC card of Bank of Cang Zhou
	reg_gpio_iotr_set_pin_type(IC_TDA8035_AUXIO1, GPIO_PIN_TYPE_OUTPUT);
	reg_gpio_iotr_set_pin_type(IC_TDA8035_AUXIO2, GPIO_PIN_TYPE_OUTPUT);
	gpio_direction_output(IC_TDA8035_AUXIO1, 0);
	gpio_direction_output(IC_TDA8035_AUXIO2, 0);
	value = reg_read8(SMARTCARD_BASE + SC_P_UART_CMD_1);
	value &= ~SC_UART_CMD_1_get_atr_MASK;
	value |= SC_UART_CMD_1_t_r_MASK;
	reg_write8(SMARTCARD_BASE + SC_P_UART_CMD_1, value);

	ctrl_ic_pres_pol(1);/*pres_pol high indicate ic card pressent*/
	reg_write8(SMARTCARD_BASE + SC_P_INTR_EN_1,SC_INTR_STAT_1_pres_intr_MASK);
}
/******************************************************************
�������� void powdown_ic_module(void)
���룺
����w
�����������ر�5892��iso7816ģ��
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
void powdown_ic_module(void)
{
	reg_write8(SMARTCARD_BASE + SC_P_MODE_REGISTER,0);
}
/******************************************************************
��������  int check_ic_module_work(void)
���룺
�����= 1 ��ʾģ�鿪����= 0 ��ʾģ��رգw
�������������5892��iso7816ģ���Ƿ���
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
int check_ic_module_work(void)
{
	uint8_t value =0 ;
	value = reg_read8(SMARTCARD_BASE + SC_P_MODE_REGISTER);
	if((value & 0x01) == 0x01) return 1;
	else return 0;
}
/******************************************************************
�������� void ctrl_sc_timer(TIMER_CTRL *timer_ctrl)
���룺TIMER_CTRL *timer_ctrl mode = ����Դ mode=������ʼ��ʽ
			onoff=ʹ�ܡ���ʹ��ģ��
����wno
��������������ģ��Ķ�ʱ��
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
void ctrl_ic_timer(timer_ctrl_type *timer_ctrl)
{
	uint8_t value = 0;
	value = reg_read8(SMARTCARD_BASE + SC_P_TIMER_CMD);
	value &= ~SC_TIMER_CMD_timer_en_MASK;
	reg_write8(SMARTCARD_BASE + SC_P_TIMER_CMD,value);
	reg_write8(SMARTCARD_BASE + SC_P_TIMER_CMP_1,timer_ctrl->cnt & 0xff);
	reg_write8(SMARTCARD_BASE + SC_P_TIMER_CMP_2,(timer_ctrl->cnt & 0xff00) >> 8);//02-26
	value = timer_ctrl->onoff ? (value |SC_TIMER_CMD_timer_en_MASK):(value & (~SC_TIMER_CMD_timer_en_MASK));
	value = timer_ctrl->mode ? (value |SC_TIMER_CMD_timer_mode_MASK):(value &(~SC_TIMER_CMD_timer_mode_MASK));
	value = timer_ctrl->src ?(value | SC_TIMER_CMD_timer_src_MASK):(value &(~SC_TIMER_CMD_timer_src_MASK));
	reg_write8(SMARTCARD_BASE + SC_P_TIMER_CMD,value);
}
/******************************************************************
��������int check_timer_over(void)
���� :no
��� : int 1=��ʱʱ�䵽�0=��ʱΪ�ﵽ
����������������ʱ������ʼ��ʱ
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
int check_ic_timer_over(void)
{
	uint8_t value = 0;
	value = reg_read8(SMARTCARD_BASE + SC_P_INTR_STAT_1);
	if((value & SC_INTR_STAT_1_timer_intr_MASK) == SC_INTR_STAT_1_timer_intr_MASK)
		return 1;
	else
		return 0 ;
}
/******************************************************************
��������void wait_clk_cnt(unsigned short int cnt)
���� :int cnt ��ʱclkֵ
��� : no
���������������ʱcnt��clkʱ��
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/

void wait_ic_clk_cnt(uint16_t cnt)
{
	timer_ctrl_type timer_ctrl;
	uint8_t value = 0;
	timer_ctrl.cnt = cnt;
	timer_ctrl.mode = start_immediate ;
	timer_ctrl.src = clk;
	timer_ctrl.onoff = enable ;
	ctrl_ic_timer(&timer_ctrl);
	do{
		value =check_ic_timer_over();
	}while(value == 0);/*wait for timer over*/
	timer_ctrl.onoff = disable ;
	ctrl_ic_timer(&timer_ctrl);
}
/******************************************************************
��������void wait_ic_etu_cnt(unsigned short int cnt)
���� :int cnt ��ʱetuֵ
��� : no
���������������ʱcnt��etuʱ��
���ߣ�guoc
���ڣ�2013-02-01
�桡����1.00
******************************************************************/
void wait_ic_etu_cnt(uint16_t cnt)
{
	timer_ctrl_type timer_ctrl;
	uint8_t value = 0;
	if(cnt >0){
		timer_ctrl.cnt = cnt;
		timer_ctrl.mode = start_immediate ;
		timer_ctrl.src = etu;
		timer_ctrl.onoff = enable ;
		ctrl_ic_timer(&timer_ctrl);
		do{
			value =check_ic_timer_over();
		}while(value == 0);/*wait for timer over*/
		timer_ctrl.onoff = disable ;
		ctrl_ic_timer(&timer_ctrl);
	}
}
void sam_ctrl_clk(uint8_t onoff)
{
	uint32_t reg = 0x1024048;	
	uint32_t  val = 0;	 
	if(onoff == 1)
		val = 0x1C0;//3m
	if(onoff == 0)
		val = 1;
	call_secure_api(WR_SEC_REG_ID, 2, reg, val);
}
void sam_timer_init(void)
{
	/*clear timer int flag */
	uint32_t value = 0;
	reg_write32(TIMER45_REG_BASE + REG_TIMER1_INTCLR,0x00);
	value = (1 << 0) |(1 << 1);/*one-shot,32bit 6M disable*/
	reg_write32(TIMER45_REG_BASE + REG_TIMER1_CONTROL,value);
	reg_write32(TIMER45_REG_BASE + REG_TIMER1_LOAD,0xffffff);/*load data */
}
void sam_start_timer_cnt(uint32_t cnt)
{
	uint32_t value = 0;
	reg_write32(TIMER45_REG_BASE + REG_TIMER1_INTCLR,0x00);
	value = (1 << 0) |(1 << 1);/*one-shot,32bit 6M disable*/
	reg_write32(TIMER45_REG_BASE + REG_TIMER1_CONTROL,value);
	reg_write32(TIMER45_REG_BASE + REG_TIMER1_LOAD,cnt);
	value = (1 << 0) |(1 << 1)|(1 << 7);/*one-shot,32bit 6M enable*/
	reg_write32(TIMER45_REG_BASE + REG_TIMER1_CONTROL,value);
	
}
uint32_t sam_read_timer_value(void)
{
	uint32_t retval = 0;
	retval = reg_read32(TIMER45_REG_BASE + REG_TIMER1_VALUE);
	return retval;
}
int sam_check_timer_over(void)
{
	uint32_t value = 0;
	value = reg_read32(TIMER45_REG_BASE + REG_TIMER1_VALUE);
	if(value == 0)
		return 1;
	else
		return 0;
}
void sam_stop_timer(void)
{
	reg_write32(TIMER45_REG_BASE + REG_TIMER1_INTCLR,0x00);
	reg_write32(TIMER45_REG_BASE + REG_TIMER1_CONTROL,0x00);
}
void sam_wait_clk_cnt(uint32_t clk)
{
	uint32_t start_cnt = 0 ;
	if(clk > 0){
		start_cnt = sam_read_timer_value();
		clk *= ctrl->timer_factor;
		while(start_cnt - sam_read_timer_value() < clk);
	}
}
void sam_wait_etu_cnt(uint32_t etu)
{
	uint32_t start_cnt = 0 ;
	if(etu > 0){
		start_cnt = sam_read_timer_value();
		etu *= ctrl->etu_count;
		while(start_cnt - sam_read_timer_value() < etu);
	}
}
void bcm5892_gpio_set_pin_type(uint32_t pin,io_type_t type)
{
	(type == input) ? reg_gpio_iotr_set_pin_type(pin,GPIO_PIN_TYPE_INPUT) :\
					  reg_gpio_iotr_set_pin_type(pin,GPIO_PIN_TYPE_OUTPUT);	
}

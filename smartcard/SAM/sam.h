#ifndef _SAM_H_
typedef struct {
        uint8_t bit_reverse;    /* bit logic convention */
        uint8_t t1;             /* T1 or T0 */
        uint8_t rec_byte;       /* received byte */
        uint8_t parity_count;   /* event parity counter */
        uint8_t IFSC;
        uint8_t NAD;
        uint8_t wtx;
        uint8_t IFD_Seq;
        uint8_t ICC_Seq;
        uint8_t IFSD_Negotiated;
        uint32_t etu_count;     /* ETU */
        uint32_t F;             /* F factor */
        uint32_t WWT;           /* work waiting time 960 * D * WI */
        uint32_t CWT;           /* character waiting time 2^CWI + 11 */
        uint32_t BWT;           /* block waiting time 2^BWI * 960 + 11 */
        uint32_t BGT;
        uint32_t GTR;
}icc_ctrl_t;
extern sam_stat_t sam_stat[5];
extern int sam_powerup(uint8_t *atrstr,uint8_t len,uint8_t samnum,icc_standard_t standard);
extern int sam_send_apdu(uint8_t *buffer, uint32_t length, uint8_t *rbuffer,uint8_t samnum);
#endif

#ifndef _SLE4442_H_
#define _SLE4442_H_
//����Ը��洢���Ĳ�������   


 #define  RD_MAIN_RAM         0X30       //�����洢�� ic��ģʽ:outgoing data   
 #define WR_MAIN_RAM          0X38       //д���洢�� ic��ģʽ:processing   
 #define RD_P_RAM             0X34      //�������洢�� ic��ģʽ:outgoing data   
 #define WR_P_RAM             0X3C      //д�����洢�� ic��ģʽ:processing   
 #define RD_PSC_RAM           0X31        //����ȫ�洢�� ic��ģʽ:outgoing data   
 #define WR_PSC_RAM           0X39        //д��ȫ�洢�� ic��ģʽ:processing   
 #define COMP_PSC_RAM         0X33      //�Ƚϰ�ȫ�洢�� ic��ģʽ:processing 
 
 extern int sle4442_poweron(unsigned char *atrbuf);
 extern int sle4442_pro_main(const unsigned char *sendbuf,unsigned char *rcvlen,unsigned char *recebuf);

#endif



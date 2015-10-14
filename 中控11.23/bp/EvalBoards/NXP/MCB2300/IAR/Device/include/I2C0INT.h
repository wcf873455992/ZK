/*
*********************************************************************************************************
** �ļ����� �� I2CINT.h
** ����˵�� �� LPC2000ϵ��оƬӲ��I2C�������
** ʹ��˵�� �� ������Ҫ���ú�I2C���߽ӿ�(I2C���Ź��ܺ�I2C�жϣ�����ʹ��I2C��ģʽ)
*********************************************************************************************************
*/

#ifndef  I2CINT_H
#define  I2CINT_H

#define	AT24C512	0XA0

#define	ONE_BYTE_SUBA	1
#define TWO_BYTE_SUBA	2
#define X_ADD_8_SUBA	3

uint8 IRcvByte0(uint8 sla, uint8 *dat);
uint8 ISendByte0(uint8 sla, uint8 dat);
uint8 I2C0_ReadNByte (uint8 sla, uint32 suba_type, uint32 suba, uint8 *s, uint32 num);
uint8 I2C0_WriteNByte(uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num);
void   I2C0_Exception(void);
void I2c0Init(uint32 Fi2c);
//////////////////////////
uint8 I2C0_WriteNByte1(uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num);
////////////////////////


#endif



/*
*********************************************************************************************************
** �ļ����� �� I2C1INT.h
** ����˵�� �� LPC2000ϵ��оƬӲ��I2C1�������
** ʹ��˵�� �� ������Ҫ���ú�I2C���߽ӿ�(I2C���Ź��ܺ�I2C�жϣ�����ʹ��I2C��ģʽ)
*********************************************************************************************************
*/

#ifndef  I2C1INT_H
#define  I2C1INT_H


uint8 IRcvByte1(uint8 sla, uint8 *dat);
uint8 ISendByte1(uint8 sla, uint8 dat);
uint8 I2C1_ReadNByte (uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num);
uint8 I2C1_WriteNByte(uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num);
void   I2C1_Exception(void);
void I2c1Init(uint32 Fi2c);
BYTE AT24C16_Read(BYTE *buf, uint16 address, uint16 lg);
BYTE AT24C16_Write( BYTE *buf, uint16 address, uint16 lg);
#endif



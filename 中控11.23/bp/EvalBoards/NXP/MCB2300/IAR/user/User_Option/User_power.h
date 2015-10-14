#ifndef __USER_POWER_H
#define  __USER_POWER_H

#define POWER_CHANLE_VER  		0
#define POWER_CHANLE_CONTRL 	1
#define POWER_CHANLE_SET	 	2
#define POWER_ACTIVE			0x80000000
//�豸���Ӷ���-----��Դ������
#define DEV_POWER_NONE				0x00 //��Դ���޶���
#define DEV_POWER_DEVICE			0x01//��Դ���豸
#define DEV_POWER_LMTPC				0x02//��Դ����ý��
#define DEV_POWER_SHOW				0x03//��Դ��չ̨
#define DEV_POWER_CRT				0x04//��Դ����ʾ��
#define DEV_POWER_PC				0x05//��Դ������
#define DEV_POWER_TYJ				0x06//��Դ��ͶӰ��
#define DEV_POWER_LIGHT				0x07//��Դ��ƹ�
#define DEV_POWER_GF				0x08//��Դ�书��
#define DEV_POWER_SXJ				0x09//��Դ�������
#define DEV_POWER_TYM				0x0a//��Դ��ͶӰľ
#define DEV_POWER_CL				0x0b//��Դ�䴰��
#define DEV_POWER_LCD				0x0c//��Դ��Һ��
#define DEV_POWER_TYJUP_DOWN		0x0d//��Դ�����

//�����豸�ػ���ʱ
#define DEV_TIME_PC					60
#define DEV_TIME_LMTPC				60
#define DEV_TIME_GF					0
#define DEV_TIME_LIGHT				0
#define DEV_TIME_TYJ				10
#define DEV_TIME_TYM				20
#define DEV_TIME_TYJDJ			20
#define DEV_TIME_LCD				20
//����״̬
#define DEV_POWER_BUSY			0x80
#define DEV_POWER_RUN			0x40
#define DEV_POWER_MASK			0x0f

#define POWER_FLAG_PC			1
#define POWER_FLAG_LMT			(1<<1)
#define POWER_FLAG_LIGHT		(1<<2)
#define POWER_FLAG_TYJA			(1<<3)
#define POWER_FLAG_GF			(1<<4)
#define POWER_FLAG_TYM			(1<<5)
#define POWER_FLAG_LCD			(1<<6)
#define POWER_FLAG_TYJDJ		(1<<7)
#define POWER_FLAG_CL			(1<<8)
#define POWER_FLAG_TYJB			(1<<9)
//�����豸�ػ��ظ�����
#define DEV_REPEAT_TIMES_PC			3
#define DEV_REPEAT_TIMES_LMTPC	        2

void POWER_init(void);
void POWER_Set(uint8 contrl_io, uint8 option);
void POWER_msg_rev(uint8 *pdata, uint8 sub);
uint32 ON_POWER_option(void *pdata);
uint32 On_MSG_OPTION_POWERCARD_ONOFF(void *pdata);
void LED_SendFLASH(void);
void Power_Beep(uint16 DelayTime);
uint8 LMT_Power_contrl(uint8 Device, uint8 option);
extern uint32 POWER_FLAG_STATE;

#endif

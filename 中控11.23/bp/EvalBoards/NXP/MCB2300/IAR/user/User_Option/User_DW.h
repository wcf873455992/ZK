#ifndef __USER_DW_H
#define __USER_DW_H

#define BJC_SUB                           0x0b

#define  BJC_CMD_Ver   		0  //���汾��
#define  BJC_CMD_CSContrl 		2//��������λ������ͨ��
#define  BJC_CMD_DATA		3//���ݲ���ͨ��
#define  BJC_CMD_CSReturn		4//�����������ź���ֵ
#define  BJC_CMD_JDATA		5//���У׼����
#define  BJC_CMD_WDVal		6//���¶�ֵ
#define  BJC_CMD_WXval		7//��/д����MIC������
#define  BJC_CMD_WXStutas		8//����MIC״̬��ROֻ������״̬�ı�ʱ���ͺ��İ�
#define  BJC_CMD_WXSet	        9//����ң�������ã�����Ѹ�ݸ��������MIC���ջ���
#define  BJC_CMD_WXREVStatus 10//����ң����״̬
#define  BJC_CMD_Vol			11//����/��ȡMIC����ֵ
#define  BJC_CMD_WXPower		12//��ȡ����MIC�ز�ǿ��
#define  BJC_CMD_CoreIP		13//���İ��д�װ�IP
#define  BJC_CMD_CardNo		14//У԰һ��ͨ����
#define  XCOM_CMD_PRINT          85 //���ڴ�ӡ��Ϣ
#define XCOM_CMD_TEST	        170 //���ڲ���ָ��


#define RF_CARD_BIGEN    			1
#define RF_CARD_TIME_OVER	    (1<<1)
/////////////RF�������֤////////////////////
#define RF_ATT_ERR		0  	//RF ��֤����û���û�
#define RF_ATT_REQ		1	//RFһ�����֤����
#define RF_ATT_OK		2	//RF��֤ͨ��

#define K103_Head		0xbb
#define K103_sHead		0xFF

struct YXJ_TYPE
{
    uint8 type;
    uint8 time;
};

struct RF_CARD_TYPE
{
    uint16 state;
    uint16 CMD_Time;
    uint8 Card_No[22];
};

struct UART_TR_TYPE
{
    uint8 Type;  //
    uint8 port;
    uint8 lg;
    uint8 bak;
    uint8 *pdata;


};

struct RFIC_DATA_TYPE
{
    uint8 buf[32];  		//�����ݻ�����
    uint8 Card_No[32];	//���ţ��ַ�
    uint8 lg;			//����������
    uint8 RFIC_TYPE;	//������
    //uint8 State;			//������״̬
    //uint8 Power_count;    //��Դ��λ����
    //uint16 Power_Reset_time;//��Դ��λʱ��
    //uint16 LED_Flash_time;  //LED��˸ʱ������0.5s��δ����0.2s
    //uint16 Check_Time;	//����������ʱ����
    //OS_EVENT  *SendFlag;
    //uint32 Count;
};

void DW_init(void);
//void DWQ_Contrl(uint8 val);
//void DW_Read_WD(void);
void DW_CMD_Contrl(uint8 *pdata);
void RF_CARD_Check(void);


static uint8 _K103_sum_check(uint8 *pdata, uint8 lg);
static uint8  _ic_find_syn(uint8 ic_type, uint8 *pdata, uint8 lg);
int RFIC_K103_Mode(uint8 *pdata);











#endif

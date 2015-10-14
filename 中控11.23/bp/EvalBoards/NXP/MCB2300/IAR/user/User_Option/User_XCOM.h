#ifndef __USER_XCOM_H
#define __USER_XCOM_H

#define XCOM_QMSG_SIZE			11
#define XCOM_CMD_GET_INFO  		0x00
#define XCOM_CMD_UPDATA			0x01
#define XCOM_CMD_GET_DATA 			0x02
#define XCOM_CMD_SET_DATA 			0x03
#define XCOM_CMD_RECEIVE_DATA 	0x04
#define XCOM_CMD_RECEIVE_INFO 	0x05
#define XCOM_CMD_SEND_DATA		0x06

#define DEV_XCOM_RPTIME	3

#define DEV_PROTOCOL_NONE       0   //����Э���޷��ͺ��޿���
#define DEV_PROTOCOL_ZYCOM     1 //׿Խ���ڿ���Э��
#define DEV_PROTOCOL_ZYUDP      2//׿ԽUDP����Э��
#define DEV_PROTOCOL_PD             3 //���������Э��PELCOD
#define DEV_PROTOCOL_VISCA          5 //���������Э��VISCA

#define ZY_XCOM_FLAG_START  	0xf1
#define ZY_XCOM_FLAG_ACK      	0xf2
#define ZY_XCOM_PROTOCOL_SIZE    3


#define UART_SEND_WAIT_TIME       50
struct ZY_XCOM
{
    uint8 flag;
    uint8  frame_CMD;
    uint8  type_lg;
    // uint8  data[17];
};
struct ZY_XCOM_DATA
{
    uint8 flag;
    uint8  frame_CMD;
    uint8  type_lg;
    uint8  data[17];
};
#define XCOM_READ_FLAG_WAIT_ACK    (1<<0)
#define XCOM_READ_FLAG_WAIT_DATA    (1<<1)

struct COMM_XCOM
{
    uint8 	 	PortNum;             //���ں�
    uint8  	 	address;		       //�ӵ�ַ1
    uint16 	 	PROTOCOL;		//����Э��2
    uint8 	 	frame_count;	//���������
    uint8 	 	linecontrl;            //������żУ��
    uint32 	 	BPS;		      //������2
    OS_EVENT	*flag;                 //�������ݻص���־
    OS_EVENT	*Q_send;                 //�������ݶ���
    OS_EVENT      *UART_send_end;
    uint8  		Rflag;                  //�ȴ������������
    uint8             RCMDNo;            //����ȴ�����������׿Խ��ʽͨ����
    uint16  	Rev_lg;		      //���ڽ��ջ���������
    uint8  		Rev_buf[32];      //���ڽ��ջ�����
    uint8             *pbuf;               //�ȴ��������ݷ���ָ��
    uint8             Rdata_lg;          //�ȴ��������ݷ��س���


};
struct  XCOM_DATA
{
    struct XCOM_DATA  *next;
    char *data;
    uint32 lg;
    uint8  PortNum;    //���ں�
    uint8 Rev_frame;
    uint8 Repeat_time;

};
struct XCOM_REV_TYPE
{
    uint16 XcomPort;
    uint16 XcomLengh;
};
/*
struct XCOM_DEV
{
	uint8 comm_no;					//�豸���Ӷ���----ͨѶ
	uint8  status;						//ͨѶ״̬
	uint8 frame_count;				//���������
	uint8  count;						//�ط����ݰ�
	uint16 type;
	struct COMM_XCOM  Xcom_Config;	       //ͨѶ����
	// struct XCOM_DATA *send_data;
        HANDLER  flag;                                   //��־
};*/
#define XCOM_BUFF_SZIE    32
struct XCOM_BUFF_TYPE
{
    uint8 lg;
    uint8 buf[XCOM_BUFF_SZIE];
};
void Xcom_set(uint8 PortNum, uint32 Baudrate, uint8 poctl, uint8 LineContrl);
void *Get_XCOM_Port(uint8 no);
void XCOM_Q_Send(uint8 PortNum, uint8 *pdata, uint16 lg);
uint32 On_MSG_OPTION_COMM(void *pdata);//ͨѶ��ʱִ��
void XCOM_Data_send(uint8	PortNum, uint8 *pdata, uint8 lg, uint8 CMD, uint8 sub, uint8 repeat); //
void XCOM_init(void);
void TASK_UART(void *pdata);
void TASK_UART0Send(void *pdata);
void TASK_UART1Send(void *pdata);
void TASK_UART2Send(void *pdata);
void TASK_UART3Send(void *pdata);
void TASK_UART4Send(void *pdata);
void TASK_UART5Send(void *pdata);

void TASK_UART8Send(void *pdata); //�¼�
void TASK_UART9Send(void *pdata);//�¼�

uint8 UART_Write(uint8 Port, uint8 *pdata, uint8 lg);
uint8 XCOM_Data_Read(uint8 PortNum, uint8 sub, uint8 cmd, uint8 *buf, uint8 *plg, uint8 WaitType);
uint8 XCOM_Wait_Ack(uint8 PortNum, uint16 WaitTime);
void XCOM_Send_ACK(uint8 *p);

extern uint8 IsImageRecognition(const uint8 *pbuf);
#endif

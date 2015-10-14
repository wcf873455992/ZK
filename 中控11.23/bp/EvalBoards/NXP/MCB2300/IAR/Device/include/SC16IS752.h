#ifndef _SC16IS752
#define  _SC16IS752
//-------------------------
//General register set
//-------------------------
#define Fxcom		1843200       //  1.8432MHZ

#define RHR 0x00			//���ձ���Ĵ���
#define THR 0x00			//���ͱ���Ĵ���

#define IER 0x01			//�ж�ʹ�ܼĴ���

#define FCR 0x02			//FIFO���ƼĴ���
#define IIR 0x02			//�ж�ʶ��Ĵ���

#define LCR 0x03			//�߿��ƼĴ���
#define MCR 0x04			//Modem���ƼĴ���
#define LSR 0x05			//��״̬�Ĵ���

#define MSR 0x06			//Modem״̬�Ĵ���
#define SPR 0x07			//�ݴ�Ĵ���
#define TCR 0x06			//���Ϳ��ƼĴ���
#define TLR 0x07			//������Ĵ���

#define TXLVL 0x08		//����FIFO��ƽ�Ĵ���
#define RXLVL 0x09		//����FIFO��ƽ�Ĵ���

#define IODir 0x0A		//I/O����Ĵ���
#define IOState 0x0B	//I/O״̬�Ĵ���
#define IOIntEna 0x0C //I/O�ж�ʹ�ܼĴ���
#define IOControl 0x0E //I/O�ſ��ƼĴ���

#define EFCR 0x0F			//�������ԼĴ���

//-------------------------
//Special register set
//-------------------------
#define DLL 0x00			//�����Ĵ���LSB
#define DLH 0x01			//�����Ĵ���MSB

//-------------------------
//Enhanced register set
//-------------------------
#define EFR 0x02			//��ǿ�����ԼĴ���
#define Xon1 0x04			//Xon1��
#define Xon2 0x05			//Xon2��
#define Xoff1 0x06		//Xoff1��
#define Xoff2 0x07		//Xoff1��

//-------------------------
//Function:
//-------------------------
#define SC16IS752 0x9A	//SC16IS752��ַ����

#define CHA 0x00		//����0
#define CHB 0x02		//����1

typedef struct
{
    uint8 BAND_H;
    uint8 BAND_L;
} UART_BAN;
#define	COM0	1
#define	COM1	2
#define	COM2	3
#define	COM3	4
#define	COM4	5
#define	COM5	6
#define	COM6	7
#define	COM7	8

void  SPI_Init(void);
uint8    SPI_send(uint8 data);
uint8	SPI_read(uint8	reg, uint8	ncom);
void	SPI_write(uint8	add, uint8	ncom, uint8	date);
void	SPI_writeData(uint8	*Value, uint8	num, uint8	ncom);
uint8 ReadXuartBuff(uint8 *buf, uint8 ncom);
void set_IS752_bps(uint8 no, uint32 Baudrate, uint8 LineContrl);
void init_SC16IS752(void);
//void IS752_Scan(uint8 i);
void IS752_Scan(void);
//////////////////////////////
uint16    SPI_send1(uint16 data);
#endif

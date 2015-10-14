#include  "includes.h"
//UART_BAN  uart_config[4];
extern struct COMM_XCOM XCOM_PORT[DEV_XCOM_MAX_SIZE];
/********************************************************************************************************
** �������ƣ�MSPI_Init()
** �������ܣ���ʼ��SPI�ӿڣ�����Ϊ������
** ��ڲ�������
** ���ڲ�������
*********************************************************************************************************/

void  SPI_Init(void)
{
    S0SPCCR = 0x52;		            // ����SPIʱ�ӷ�Ƶ
    S0SPCR  = 	 (0 << 2) |
                 (0 << 3) |				// CPHA = 0, ������SCK �ĵ�һ��ʱ���ز���
                 (0 << 4) |				// CPOL = 1, SCK Ϊ����Ч
                 (1 << 5) |				// MSTR = 1, SPI ������ģʽ
                 (0 << 6) |				// LSBF = 0, SPI ���ݴ���MSB (λ7)����
                 (0 << 7);				// SPIE = 0, SPI �жϱ���ֹ
}
/********************************************************************************************************
** �������ƣ�MSPI_SendData()
** �������ܣ���SPI���߷������ݡ�
** ��ڲ�����data   �����͵�����
** ���ڲ���������ֵΪ��ȡ������
********************************************************************************************************/
uint8  SPI_send(uint8 data)
{



    S0SPDR = data;
    while( 0 == (S0SPSR & 0x80));		// �ȴ�SPIF��λ�����ȴ����ݷ������

    return (S0SPDR);
}
/********************************************************************************************************
** �������ƣ�MSPI_SendData()
** �������ܣ���SPI���߷������ݡ�
** ��ڲ�����data   �����͵�����
** ���ڲ���������ֵΪ��ȡ������
********************************************************************************************************/
uint16  SPI_send1(uint16 data)
{



    S0SPDR = data;
    while( 0 == (S0SPSR & 0x80));		// �ȴ�SPIF��λ�����ȴ����ݷ������

    return(S0SPDR);
}

//---------------------------------------
//	�������ƣ�SPI_read
//	�������ܣ���752�Ĵ�����ֵ
//	��ڲ�����uint8	reg��752�Ĵ�����ַ��NCOM
//	���ڲ�����RegValue�����ؼĴ�����ֵ
//---------------------------------------
uint8	SPI_read(uint8	reg, uint8	ncom)
{
    uint8 k;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif

    OS_ENTER_CRITICAL();
    if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_5) || (ncom == DEV_XCOM_8) || (ncom == DEV_XCOM_9))
    {
        if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_5))
        {
            IO1CLR |= COM2_3_CS;    //
        }
        else
            //if((ncom==DEV_XCOM_SCAMERA3)||(ncom==DEV_XCOM_SCAMERA4))
        {
            IO1CLR |= COM7_8_CS;    //
        }
    }
    else
    {
        OS_EXIT_CRITICAL();
        return 0;
    }
    if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_5) || (ncom == DEV_XCOM_8) || (ncom == DEV_XCOM_9))
    {
        if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_8))
            k = CHA;
        else
            k = CHB;
    }
    SPI_send(((reg << 3) + k) | 0x80);
    reg = SPI_send(0);
    //reg=SPI_send(0xff);
    if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_5))
        IO1SET |= COM2_3_CS;    //
    else  IO1SET |= COM7_8_CS;

    OS_EXIT_CRITICAL();
    return	reg;
}

//---------------------------------------
//	�������ƣ�SPI_write��ok
//	�������ܣ�mcu��һ���ֽ�dateͨ��SPI���߷��͵�752�������͵��ⲿ�豸��
//	��ڲ�����uint8	add���Ĵ�����ַ�ֽ�
//	���ڲ�������
//---------------------------------------
void	SPI_write(uint8	add, uint8	ncom, uint8	date)
{
    uint8 k, reg1, reg2, add1;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif
    if ((ncom != DEV_XCOM_DYX) && (ncom != DEV_XCOM_5) && (ncom != DEV_XCOM_8) && (ncom != DEV_XCOM_9)) return;
    OS_ENTER_CRITICAL();
    if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_5) || (ncom == DEV_XCOM_8) || (ncom == DEV_XCOM_9))
    {
        if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_5))
            IO1CLR |= COM2_3_CS;
        else//((ncom==DEV_XCOM_SCAMERA3)||(ncom==DEV_XCOM_SCAMERA4))
            IO1CLR |= COM7_8_CS;
        if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_8))
            k = CHA;
        else //(ncom==DEV_XCOM_SCAMERA||(ncom=DEV_XCOM_SCAMERA4))
            k = CHB;
    }
    //add1=add;
    reg1 = SPI_send((add << 3) + k);
    reg2 = SPI_send(date);
    //IP_printf("add1=%x,date=%x,SO_reg1=%x,reg2=%x",add1,date,reg1,reg2);

    if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_5))
        IO1SET |= COM2_3_CS;
    else//((ncom==DEV_XCOM_SCAMERA3||(ncom=DEV_XCOM_SCAMERA4))
        IO1SET |= COM7_8_CS;
    OS_EXIT_CRITICAL();
}


//---------------------------------------
//	�������ƣ�SPI_writeData��ok
//	�������ܣ�mcu������ֽ�ͨ��SPI���߷��͵�752�������͵��ⲿ�豸��
//	��ڲ�����
//							uint8	channel�����ںţ���1-8
//							unsigned int	*Value����Ҫ��������ָ���׵�ַ
//							uint8	num����Ҫ���������ֽ���
//	���ڲ�������
//---------------------------------------
void	SPI_writeData(uint8	*Value, uint8	num, uint8	ncom)
{
    uint8 k;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif

    OS_ENTER_CRITICAL();

    if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_5))
    {
        IO1CLR |= COM2_3_CS;
    }
    else
        IO1CLR |= COM7_8_CS;
    if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_8))
        k = CHA;
    else
        k = CHB;

    SPI_send((THR << 3) + k);
    while(num > 0)
    {
        SPI_send(*Value);
        Value++;
        num--;
    }
    if((ncom == DEV_XCOM_DYX) || (ncom == DEV_XCOM_5))
        IO1SET |= COM2_3_CS;
    else 	IO1SET |= COM7_8_CS;
    OS_EXIT_CRITICAL();
}
//---------------------------------------
//	�������ƣ�
//	�������ܣ������ڻ���������,����С��32
//	��ڲ���: buf������ָ��
//			  ncom���ں�1-9
//	���ڲ��������ݳ���
//---------------------------------------

uint8 ReadXuartBuff(uint8 *buf, uint8 ncom)
{
    uint8	tmp, k, len;
    tmp = SPI_read(IIR, ncom) & 0x0f;
    if( tmp == 0x0C) 	// check interrupt status register
    {
        len = SPI_read(RXLVL, ncom); //��FIFO�������ֽڳ���
        if (len > 64) len = 64;
        for(k = 0; k < len; k++)
        {
            buf[k] = SPI_read(RHR, ncom);	//����FIFO�����ݴ���a[]��
        }
        return len;
    }
    else
        return 0;
}
//	�������ƣ�set_UART_bps(uint8 no)
//	�������ܣ����ô��ڵĲ�����
//      ��ڲ���:no=4/5/8/9
//	���ڲ�������
//---------------------------------------
//	�������ƣ�init_SC16IS752
//	�������ܣ�752�Ĵ�����ʼ��
//	��ڲ�������
//	���ڲ�������
//---------------------------------------
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
/*
void IS752_Scan(uint8 i)
{
  if(i==2)
  {
      uint8 tmp;
          tmp=SPI_read(MCR,DEV_XCOM_DYX);
      if (tmp!=0x02) goto rest1;
          tmp=SPI_read(EFCR,DEV_XCOM_DYX);
      if (tmp!=0x31) goto rest1;
          tmp=SPI_read(IER,DEV_XCOM_DYX);
      if (tmp!=0x01) goto rest1;
///////////////////////////////////
          tmp=SPI_read(MCR,DEV_XCOM_SCAMERA);
      if (tmp!=0x02) goto rest2;
          tmp=SPI_read(EFCR,DEV_XCOM_SCAMERA);
      if (tmp!=0x31) goto rest2;
          tmp=SPI_read(IER,DEV_XCOM_SCAMERA);
      if (tmp!=0x01) goto rest2;
        return;
  rest1:
	 Xcom_set(DEV_XCOM_DYX,9600,DEV_PROTOCOL_ZYCOM,0x03);
	 return;
  rest2:
	 Xcom_set(DEV_XCOM_SCAMERA,XCOM_PORT[DEV_XCOM_SCAMERA].BPS,DEV_PROTOCOL_NONE,0x03);
	 return;
  }
  else
  {
     uint8 tmp;
          tmp=SPI_read(MCR,DEV_XCOM_SCAMERA3);
      if (tmp!=0x02) goto rest3;
          tmp=SPI_read(EFCR,DEV_XCOM_SCAMERA3);
      if (tmp!=0x31) goto rest3;
          tmp=SPI_read(IER,DEV_XCOM_SCAMERA3);
      if (tmp!=0x01) goto rest3;
///////////////////////////////////
          tmp=SPI_read(MCR,DEV_XCOM_SCAMERA4);
      if (tmp!=0x02) goto rest4;
          tmp=SPI_read(EFCR,DEV_XCOM_SCAMERA4);
      if (tmp!=0x31) goto rest4;
          tmp=SPI_read(IER,DEV_XCOM_SCAMERA4);
      if (tmp!=0x01) goto rest4;
        return;
  rest3:
	 Xcom_set(DEV_XCOM_SCAMERA3,XCOM_PORT[DEV_XCOM_SCAMERA3].BPS,DEV_PROTOCOL_NONE,0x03);
	 return;
  rest4:
	 Xcom_set(DEV_XCOM_SCAMERA4,XCOM_PORT[DEV_XCOM_SCAMERA4].BPS,DEV_PROTOCOL_NONE,0x03);
	 return;
  }
}
*/
// *
void IS752_Scan(void)
{
    uint8 tmp;
    tmp = SPI_read(MCR, DEV_XCOM_DYX);
    //IP_printf("MCR=%x",tmp);
    if (tmp != 0x02) goto rest1;
    tmp = SPI_read(EFCR, DEV_XCOM_DYX);
    //IP_printf("EFCR=%x",tmp);
    if (tmp != 0x31) goto rest1;
    tmp = SPI_read(IER, DEV_XCOM_DYX);
    //IP_printf("IER=%x",tmp);
    if (tmp != 0x01) goto rest1;
    ///////////////////////////////////
    tmp = SPI_read(MCR, DEV_XCOM_5);
    //IP_printf("MCR2=%x",tmp);
    if (tmp != 0x02) goto rest2;
    tmp = SPI_read(EFCR, DEV_XCOM_5);
    //IP_printf("EFCR2=%x",tmp);
    if (tmp != 0x20) goto rest2;
    tmp = SPI_read(IER, DEV_XCOM_5);
    //IP_printf("IER2=%x",tmp);
    if (tmp != 0x01) goto rest2;
    return;
rest1:
    Xcom_set(DEV_XCOM_DYX, 9600, DEV_PROTOCOL_ZYCOM, 0x03);
    return;
rest2:
    Xcom_set(DEV_XCOM_5, XCOM_PORT[DEV_XCOM_5].BPS, DEV_PROTOCOL_NONE, 0x03);
    return;
}//*/


void IS752_Scan2(void)
{
    uint8 tmp;
    tmp = SPI_read(MCR, DEV_XCOM_8);
    //IP_printf("MCR=%x",tmp);
    if (tmp != 0x02) goto rest1;
    tmp = SPI_read(EFCR, DEV_XCOM_8);
    //IP_printf("EFCR=%x",tmp);
    if (tmp != 0x20) goto rest1;
    tmp = SPI_read(IER, DEV_XCOM_8);
    //IP_printf("IER=%x",tmp);
    if (tmp != 0x01) goto rest1;
    ///////////////////////////////////
    tmp = SPI_read(MCR, DEV_XCOM_9);
    //IP_printf("MCR2=%x",tmp);
    if (tmp != 0x02) goto rest2;
    tmp = SPI_read(EFCR, DEV_XCOM_9);
    //IP_printf("EFCR2=%x",tmp);
    if (tmp != 0x20) goto rest2;
    tmp = SPI_read(IER, DEV_XCOM_9);
    //IP_printf("IER2=%x",tmp);
    if (tmp != 0x01) goto rest2;
    return;
rest1:
    Xcom_set(DEV_XCOM_8, XCOM_PORT[DEV_XCOM_8].BPS, DEV_PROTOCOL_NONE, 0x03);
    return;
rest2:
    Xcom_set(DEV_XCOM_9, XCOM_PORT[DEV_XCOM_9].BPS, DEV_PROTOCOL_NONE, 0x03);
    return;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void set_IS752_bps(uint8 no, uint32 Baudrate, uint8 LineContrl)
{
    uint16 tmp;
    tmp = Fxcom / Baudrate / 16;

    if((no == DEV_XCOM_DYX) || (no == DEV_XCOM_5) || (no == DEV_XCOM_8) || (no == DEV_XCOM_9))
    {
        SPI_write(LCR, no, 0X80);		//�����ʵĳ�������ʹ��
        SPI_write(DLL, no, tmp % 256);
        SPI_write(DLH, no, tmp / 256);		//DLH�ĳ�ֵΪ0x0
        SPI_write(LCR, no, 0XBF);		//ʹ�ܴ�����ǿ����
        SPI_write(EFR, no, 0X10);		//ʹ����ǿ�͹���IER[7:4], FCR[5:4]��MCR[7:5]ʹ��ɱ��޸�
        SPI_write(LCR, no, LineContrl & 0x7f); //0X03);		//8 data bit,1 stop bit, no У��
        if(no == DEV_XCOM_DYX)
        {
            SPI_write(MCR, no, 0X02);		//��������ʶ����
            SPI_write(EFCR, no, 0X31);		//����485����
        }
        if((no == DEV_XCOM_5) || (no == DEV_XCOM_8) || (no == DEV_XCOM_9))
        {
            SPI_write(MCR, no, 0X02);		//��������ʶ����
            SPI_write(EFCR, no, 0X20);		//����232����
        }
        SPI_write(FCR, no, 0X87);		//��գ���λ���շ��͵�FIFO,FIFO����56���ַ���ʹ��FIFO
        SPI_write(FCR, no, 0X07);
        SPI_write(IER, no, 0X01);		//ʹIRQ�����ж��ź�
    }
}
//---------------------------------------
//	�������ƣ�init_SC16IS752
//	�������ܣ�752�Ĵ�����ʼ��
//	��ڲ�������
//	���ڲ�������
//---------------------------------------
void init_SC16IS752(void)
{

}



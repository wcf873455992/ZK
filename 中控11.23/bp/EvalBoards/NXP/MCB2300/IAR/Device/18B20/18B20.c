#include "includes.h"


uchar dat; //��д���ݱ���
uchar a = 0;
uchar b = 0;
float tep = 0; //��һ���¶�ʱ���¶�ת���м��
//uchar data tempbuf[4]=0;//�¶�������ʾ�м����
uchar tempbuf[4] = {0, 0, 0, 0}; //�¶�������ʾ�м����
#define IO_OUT 1
#define IO_IN  0
////////////////////////////////////////
//�����������ƣ�delay(uint num)
//��������ֵ����
//����������uint32 num 10us��ʱ����ֵ
//�������ã���ʱ
//////////////////////////////////////////
void delay(uint8 num)
{
    uint8 i, n;

    for(i = 0; i < num; i++)
        for(n = 0; n < 74; n++);


}


uchar SET_IO(uchar IO, uchar value)
{
    if (IO == IO_OUT)
    {
        if((IO1DIR & DS18B20_IO) >> 26);
        else IO1DIR |= DS18B20_IO; //���
        switch(value)
        {
        case 1:
            IO1SET |= DS18B20_IO;
            break;
        case 0:
            IO1CLR |= DS18B20_IO;
            break;
        }

    }
    else
    {
        //IO1DIR&=~(DS18B20_IO);//����
        //value=IO1PIN&(DS18B20_IO)>>26;

    }
    return value;
}

uchar Read_IO()
{
    uchar value = 2;
    IO1DIR &= ~(DS18B20_IO); //����
    delay(1);
    value = (IO1PIN & (DS18B20_IO)) >> 26;
    delay(5);
    //IP_printf("value=%x",value);
    return value;
}

//******************************************************
//�����������ƣ�void Init_DS18B20(void)
//��������ֵ����
//������������
//�������ã���ʼ��18B20
//******************************************************
void Init_DS18B20(void)
{
    //uchar x=0;
    uchar x = 3;

    SET_IO(IO_OUT, 1);
    delay(1);
    SET_IO(IO_OUT, 0);
    delay(65);
    IO1DIR &= ~(DS18B20_IO); //����
    delay(10);
    x = Read_IO();
    delay(10);
}

/******************************************************
�����������ƣ�uchar ReadOneChar(void)
��������ֵ��uchar dat
���������� ��
�������ã���ȡ1820һ���ֽ�
*******************************************************/
uchar ReadOneChar(void)
{
    uchar i, dat = 0;

    for(i = 0; i < 8; i++)
    {
        SET_IO(IO_OUT, 0);
        dat >>= 1;
        SET_IO(IO_OUT, 1);
        if(Read_IO())
        {
            dat |= 0x80;   //��1 /// ��0���ƴ���
        }
        delay(1);
    }
    //IP_printf("dat=%x",dat);
    return dat;
}
/******************************************************
�����������ƣ�void WriteOneChar(uchar dat)
��������ֵ����
���������� uchar dat
�������ã���1820дһ���ֽ�
*******************************************************/
/////****дDS18B20***///д0 60us���꣬д1 30us �ڶ���
void WriteOneChar(uchar dat)
{
    uchar i = 0;
    //for(;;)
    for(i = 0; i < 8; i++)
    {
        SET_IO(IO_OUT, 0);
        SET_IO(IO_OUT, dat & 0x01); //д�����������λ
        delay(10);
        SET_IO(IO_OUT, 1);
        dat >>= 1;
    }
    delay(40);

}
// ******************************************************
//�����������ƣ�int ReadOneTemperature(void)
//��������ֵ��int t
//���������� ��
//�������ã����¶�ֵ
//��******************************************************
//int ReadOneTemperature(void) ////////***��ȡ�¶�ֵ***********///// ÿ�ζ�д��Ҫ�ȸ�λ

int ReadOneTemperature(void)
{
    int t;
    //for(;;)

    Init_DS18B20();
    WriteOneChar(0xcc);//������ROM����
    WriteOneChar(0x44);//������ʼת������
    Init_DS18B20();
    WriteOneChar(0xcc);//������ROM����
    WriteOneChar(0xbe);//���Ĵ����������ֽڣ�ǰ���ֽ�Ϊת��ֵ
    a = ReadOneChar(); //a����ֽ�
    b = ReadOneChar(); //b����ֽ�
    t = b;
    t <<= 8; //���ֽ�ת��Ϊ10����
    t = t | a;
    tep = t * 0.0625; //ת������Ϊ0.0625/LSB
    t = tep * 10 + 0.5; //����1λС������������****�����10��ԭ��ȷ�¶�ֵ��
    /* */
    return t;
}


/******************************************************
�����������ƣ�uint Temperaturepro(void)
��������ֵ��void
���������� void
�������ã��¶ȴ���
*******************************************************/
void Temperaturepro(void)
{
    int temp;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
    OS_ENTER_CRITICAL();
    temp = ReadOneTemperature();
    OS_EXIT_CRITICAL();
    tempbuf[3] = temp / 1000; //��λ
    tempbuf[2] = temp / 100 % 10; //ʮλ
    tempbuf[1] = temp % 100 / 10; //��λ
    tempbuf[0] = temp % 10; //С��
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("�¶� %d%d%d.%d ��", tempbuf[3], tempbuf[2], tempbuf[1], tempbuf[0]);
}

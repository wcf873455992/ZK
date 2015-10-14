#include "includes.h"


uchar dat; //读写数据变量
uchar a = 0;
uchar b = 0;
float tep = 0; //读一个温度时的温度转换中间间
//uchar data tempbuf[4]=0;//温度字型显示中间变量
uchar tempbuf[4] = {0, 0, 0, 0}; //温度字型显示中间变量
#define IO_OUT 1
#define IO_IN  0
////////////////////////////////////////
//　　函数名称：delay(uint num)
//　　返回值：无
//　　参数：uint32 num 10us延时数据值
//　　作用：延时
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
        else IO1DIR |= DS18B20_IO; //输出
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
        //IO1DIR&=~(DS18B20_IO);//输入
        //value=IO1PIN&(DS18B20_IO)>>26;

    }
    return value;
}

uchar Read_IO()
{
    uchar value = 2;
    IO1DIR &= ~(DS18B20_IO); //输入
    delay(1);
    value = (IO1PIN & (DS18B20_IO)) >> 26;
    delay(5);
    //IP_printf("value=%x",value);
    return value;
}

//******************************************************
//　　函数名称：void Init_DS18B20(void)
//　　返回值：无
//　　参数：无
//　　作用：初始化18B20
//******************************************************
void Init_DS18B20(void)
{
    //uchar x=0;
    uchar x = 3;

    SET_IO(IO_OUT, 1);
    delay(1);
    SET_IO(IO_OUT, 0);
    delay(65);
    IO1DIR &= ~(DS18B20_IO); //输入
    delay(10);
    x = Read_IO();
    delay(10);
}

/******************************************************
　　函数名称：uchar ReadOneChar(void)
　　返回值：uchar dat
　　参数： 无
　　作用：读取1820一个字节
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
            dat |= 0x80;   //读1 /// 读0右移处理
        }
        delay(1);
    }
    //IP_printf("dat=%x",dat);
    return dat;
}
/******************************************************
　　函数名称：void WriteOneChar(uchar dat)
　　返回值：无
　　参数： uchar dat
　　作用：向1820写一个字节
*******************************************************/
/////****写DS18B20***///写0 60us读完，写1 30us 内读完
void WriteOneChar(uchar dat)
{
    uchar i = 0;
    //for(;;)
    for(i = 0; i < 8; i++)
    {
        SET_IO(IO_OUT, 0);
        SET_IO(IO_OUT, dat & 0x01); //写所给数据最低位
        delay(10);
        SET_IO(IO_OUT, 1);
        dat >>= 1;
    }
    delay(40);

}
// ******************************************************
//　　函数名称：int ReadOneTemperature(void)
//　　返回值：int t
//　　参数： 无
//　　作用：读温度值
//　******************************************************
//int ReadOneTemperature(void) ////////***读取温度值***********///// 每次读写均要先复位

int ReadOneTemperature(void)
{
    int t;
    //for(;;)

    Init_DS18B20();
    WriteOneChar(0xcc);//发跳过ROM命令
    WriteOneChar(0x44);//发读开始转换命令
    Init_DS18B20();
    WriteOneChar(0xcc);//发跳过ROM命令
    WriteOneChar(0xbe);//读寄存器，共九字节，前两字节为转换值
    a = ReadOneChar(); //a存低字节
    b = ReadOneChar(); //b存高字节
    t = b;
    t <<= 8; //高字节转换为10进制
    t = t | a;
    tep = t * 0.0625; //转换精度为0.0625/LSB
    t = tep * 10 + 0.5; //保留1位小数并四舍五入****后面除10还原正确温度值）
    /* */
    return t;
}


/******************************************************
　　函数名称：uint Temperaturepro(void)
　　返回值：void
　　参数： void
　　作用：温度处理
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
    tempbuf[3] = temp / 1000; //百位
    tempbuf[2] = temp / 100 % 10; //十位
    tempbuf[1] = temp % 100 / 10; //个位
    tempbuf[0] = temp % 10; //小数
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("温度 %d%d%d.%d ℃", tempbuf[3], tempbuf[2], tempbuf[1], tempbuf[0]);
}

#include  "includes.h"
#include "string.h"
#include "Uart.h"
//串口初始化，PortNum为0或者1,2,3
extern struct COMM_XCOM XCOM_PORT[DEV_XCOM_MAX_SIZE];
extern	uint8	gTestUartFlag;
//////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void UART_set_bps(uint8	PortNum, uint32	Baudrate, uint8  LineContrl)
{
    switch(PortNum)
    {
    case DEV_XCOM_MB://面板串口
        U0LCR = 0x83;// DLAB = 1，可设置波特率
        U0DLL = ((Fpclk / 16) / Baudrate) % 256;
        U0DLM = ((Fpclk / 16) / Baudrate) / 256;

        U0LCR = LineContrl & 0x7f; //0x03;
        U0FCR = 0xc7; // 使能FIFO，并复位R,T FIFO，R触发8 Byte
        U0IER = 0x01;  // 允许RBR中断和超时接收中断
        break;
    case DEV_XCOM_TYJ ://投影机串口
        //PINSEL0=(PINSEL0&(~(0X0F<<16)))|(0X05<<16);//设置P0.8,P0.9为串口1
        U1LCR = 0x83;// DLAB = 1，可设置波特率
        U1DLL = ((Fpclk / 16) / Baudrate) % 256;
        U1DLM = ((Fpclk / 16) / Baudrate) / 256;
        U1LCR = LineContrl & 0x7f; //0x03;
        U1FCR = 0xc7; // 使能FIFO，并复位R,T FIFO，R触发8 Byte
        U1IER = 0x01;  // 允许RBR中断和超时接收中断
        break;
    case DEV_XCOM_SOUND://刷卡器
        U2LCR = 0x83;// DLAB = 1，可设置波特率
        U2DLL = ((Fpclk / 16) / Baudrate) % 256;
        U2DLM = ((Fpclk / 16) / Baudrate) / 256;
        U2LCR = LineContrl & 0x7f; //0x03;
        U2FCR = 0xc7; // 使能FIFO，并复位R,T FIFO，R触发8 Byte
        U2IER = 0x01;  // 允许RBR中断和超时接收中断
        break;
    case DEV_XCOM_3://球机1串口3
        U3LCR = 0x83;// DLAB = 1，可设置波特率
        U3DLL = ((Fpclk / 16) / Baudrate) % 256;
        U3DLM = ((Fpclk / 16) / Baudrate) / 256;
        U3LCR = LineContrl & 0x7f; // 0x03;
        U3FCR = 0xc7; // 使能FIFO，并复位R,T FIFO，R触发8 Byte
        U3IER = 0x01;  // 允许RBR中断和超时接收中断
        break;
    case DEV_XCOM_DYX://电源箱串口
        set_IS752_bps(DEV_XCOM_DYX, Baudrate, LineContrl & 0x7f);
        break;
    case DEV_XCOM_5://球机控制串口
        set_IS752_bps( DEV_XCOM_5, Baudrate, LineContrl & 0x7f);
        break;
    case DEV_XCOM_8://球机控制串口
        set_IS752_bps( DEV_XCOM_8, Baudrate, LineContrl & 0x7f);
        break;
    case DEV_XCOM_9://球机控制串口
        set_IS752_bps( DEV_XCOM_9, Baudrate, LineContrl & 0x7f);
        break;




    default:
        break;

    };



}
//////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
//发送一个字节
void UART_PutChar ( uint8	PortNum, uint8 data)
{
    if(PortNum == DEV_XCOM_MB)
    {
        U0THR = data;			    // 发送数据
        while( (U0LSR & 0x40) == 0 );	 // 等待数据发送完毕
    }
    else if(PortNum == DEV_XCOM_TYJ)
    {
        U1THR = data;			    // 发送数据
        while( (U1LSR & 0x40) == 0 );	 // 等待数据发送完毕
    }
    else	if(PortNum == DEV_XCOM_SOUND)
    {
        U2THR = data;			    // 发送数据
        while( (U2LSR & 0x40) == 0 );	 // 等待数据发送完毕
    }
    else if(PortNum == DEV_XCOM_3)
    {
        U3THR = data;			    // 发送数据
        while( (U3LSR & 0x40) == 0 );	 // 等待数据发送完毕
    }
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：发送字符串
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////

void  UART_PutString(uint8	PortNum, uint8 *str)
{
    while(*str != '\0')
    {
        UART_PutChar(PortNum, *str++);
    }
}
void COM_IP_printf(uint8 PortNum, uint8 *pdata, uint8 len)
{
    uint8 date[13], i;

    for(i = 0; i < len; i++)
    {
        date[i] = *(pdata + i);
    }
    //IP_printf("串口%d发送%d:%x",PortNum,len,date[0]);
    if(PortNum != 4)return;

    switch(len)
    {
    case 0:
        IP_printf("串口%d发送%d:", PortNum, len);
        break;
    case 1:
        IP_printf("串口%d发送%d:%x ", PortNum, len, date[0]);
        break;
    case 2:
        IP_printf("串口%d发送%d:%x %x", PortNum, len, date[0], date[1]);
        break;
    case 3:
        IP_printf("串口%d发送%d:%x %x %x", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2));
        break;
    case 4:
        IP_printf("串口%d发送%d:%x %x %x %x", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3));
        break;
    case 5:
        IP_printf("串口%d发送%d:%x %x %x %x %x ", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3), *(pdata + 4));
        break;
    case 6:
        IP_printf("串口%d发送%d:%x %x %x %x %x %x ", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3), *(pdata + 4), *(pdata + 5));
        break;
    // case 7:IP_printf("串口%d发送%d:%x %x %x %x %x %x %x ",PortNum,len,*pdata,*(pdata+1),*(pdata+2),*(pdata+3),*(pdata+4),*(pdata+5),*(pdata+6));break;
    case 8:
        IP_printf("串口%d发送%d:%x %x %x %x %x %x %x %x", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3), *(pdata + 4), *(pdata + 5), *(pdata + 6), *(pdata + 7));
        break;
    case 9:
        IP_printf("串口%d发送%d:%x %x %x %x %x %x %x %x %x", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3), *(pdata + 4), *(pdata + 5), *(pdata + 6), *(pdata + 7), *(pdata + 8));
        break;
    case 10:
        IP_printf("串口%d发送%d:%x %x %x %x %x %x %x %x %x", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3), *(pdata + 4), *(pdata + 5), *(pdata + 6), *(pdata + 7), *(pdata + 8), *(pdata + 9));
        break;
    case 11:
        IP_printf("串口%d发送%d:%x %x %x %x %x %x %x %x %x %x", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3), *(pdata + 4), *(pdata + 5), *(pdata + 6), *(pdata + 7), *(pdata + 8), *(pdata + 9), *(pdata + 10));
        break;
    case 12:
        IP_printf("串口%d发送%d:%x %x %x %x %x %x %x %x %x %x %x", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3), *(pdata + 4), *(pdata + 5), *(pdata + 6), *(pdata + 7), *(pdata + 8), *(pdata + 9), *(pdata + 10), *(pdata + 11));
        break;
    case 13:
        IP_printf("串口%d发送%d:%x %x %x %x %x %x %x %x %x %x %x %x", PortNum, len, date[0], date[1], date[2], date[3], date[4], date[5], date[6], date[7], date[8], date[9], date[10], date[11], date[12]);
        break;
    default:
        break;
    }/**/

}
//---------------------------------------
//	函数名称：void UART_Send_Data(uint8	PortNum,uint8 *pdata,uint8 len)
//	函数功能：发送数据
//	入口参数：				uint8 	PortNum：串口号，＝1-8
//							unsigned int	*pdata：需要发送数据指针首地址
//							uint8 	len：需要发送数据字节数
//	出口参数：无
////////////////////////////////////////////////////////////////////
void UART_Send_Data(uint8	PortNum, uint8 *pdata, uint8 len)
{
    uint8 i = 0, a, b, j, err;

    //COM_IP_printf( PortNum, pdata, len);
    a = len / 16;
    b = len % 16;
    if (Get_Debug(DEV_DEBUG_COMDATE))
    {
        if(PortNum != 0)
            IP_printf("串口%d发送%d:%x %x %x %x %x %x ", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3), *(pdata + 4), *(pdata + 5));
    }
    switch(PortNum)
    {
    case DEV_XCOM_MB:
        //串口0发送数据
        for (i = 0; i < a; i++)
        {
            for (j = 0; j < 16; j++)
            {
                U0THR = pdata[j + i * 16];
            }
            U0IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_MB].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_MB].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_MB, XCOM_PORT[DEV_XCOM_MB].BPS,
                             XCOM_PORT[DEV_XCOM_MB].PROTOCOL, XCOM_PORT[DEV_XCOM_MB].linecontrl);
            }

        }
        if (b > 0)
        {

            for (j = 0; j < b; j++)
                U0THR = pdata[j + a * 16];
            U0IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_MB].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_MB].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_MB, XCOM_PORT[DEV_XCOM_MB].BPS, XCOM_PORT[DEV_XCOM_MB].PROTOCOL, XCOM_PORT[DEV_XCOM_MB].linecontrl);

            }
        }
        break;
    case DEV_XCOM_TYJ:
        //串口1发送数据
        for (i = 0; i < a; i++)
        {
            for (j = 0; j < 16; j++)
                U1THR = pdata[j + i * 16];
            U1IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_TYJ].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_TYJ].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_TYJ, XCOM_PORT[DEV_XCOM_TYJ].BPS, XCOM_PORT[DEV_XCOM_TYJ].PROTOCOL, XCOM_PORT[DEV_XCOM_TYJ].linecontrl);
            }

        }

        if (b > 0)
        {

            for (j = 0; j < b; j++)
                U1THR = pdata[j + a * 16];
            U1IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_TYJ].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_TYJ].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_TYJ, XCOM_PORT[DEV_XCOM_TYJ].BPS, XCOM_PORT[DEV_XCOM_TYJ].PROTOCOL, XCOM_PORT[DEV_XCOM_TYJ].linecontrl);
            }
        }

        break;

    case DEV_XCOM_SOUND:
        //串口2发送数据
        for (i = 0; i < a; i++)
        {

            for (j = 0; j < 16; j++)
                U2THR = pdata[j + i * 16];
            U2IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_SOUND].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_SOUND].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_SOUND, XCOM_PORT[DEV_XCOM_SOUND].BPS, XCOM_PORT[DEV_XCOM_SOUND].PROTOCOL, XCOM_PORT[DEV_XCOM_SOUND].linecontrl);

            }

        }

        if (b > 0)
        {

            for (j = 0; j < b; j++)
                U2THR = pdata[j + a * 16];
            U2IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_SOUND].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_SOUND].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_SOUND, XCOM_PORT[DEV_XCOM_SOUND].BPS, XCOM_PORT[DEV_XCOM_SOUND].PROTOCOL, XCOM_PORT[DEV_XCOM_SOUND].linecontrl);

            }

        }
        break;

    case DEV_XCOM_3:
        //串口3发送数据
        for (i = 0; i < a; i++)
        {
            for (j = 0; j < 16; j++)
            {
                U3THR = pdata[j + i * 16];
                while( (U3LSR & 0x40) == 0 );	 // 等待数据发送完毕
            }
            U3IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_3].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_3].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_3, XCOM_PORT[DEV_XCOM_3].BPS,
                             XCOM_PORT[DEV_XCOM_3].PROTOCOL, XCOM_PORT[DEV_XCOM_3].linecontrl);

            }

        }
        if (b > 0)
        {

            for (j = 0; j < b; j++)
            {
                U3THR = pdata[j + a * 16];
                while( (U3LSR & 0x40) == 0 );	 // 等待数据发送完毕

            }
            U3IER |= 0x02;
            if (XCOM_PORT[DEV_XCOM_3].UART_send_end != NULL)
            {
                OSSemPend(XCOM_PORT[DEV_XCOM_3].UART_send_end, UART_SEND_WAIT_TIME, &err);
                if (err != OS_NO_ERR)
                    Xcom_set(DEV_XCOM_3, XCOM_PORT[DEV_XCOM_3].BPS, XCOM_PORT[DEV_XCOM_3].PROTOCOL, XCOM_PORT[DEV_XCOM_3].linecontrl);
            }
        }
        break;
    case DEV_XCOM_DYX: //串口4
        if (Get_Debug(DEV_DEBUG_POWER))
            IP_printf("串口%d发送%d:%x %x %x %x %x %x ", PortNum, len, *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3), *(pdata + 4), *(pdata + 5));
        if (len > 64) len = 64;
        SPI_writeData(pdata, len, PortNum);
        SPI_write(IER, DEV_XCOM_DYX, 0X03);		//关闭发送中断
        if (XCOM_PORT[DEV_XCOM_DYX].UART_send_end != NULL)
        {
            OSSemPend(XCOM_PORT[DEV_XCOM_DYX].UART_send_end, UART_SEND_WAIT_TIME, &err);
            if (err != OS_NO_ERR)
                Xcom_set(DEV_XCOM_DYX, XCOM_PORT[DEV_XCOM_DYX].BPS,
                         XCOM_PORT[DEV_XCOM_DYX].PROTOCOL, XCOM_PORT[DEV_XCOM_DYX].linecontrl);


        }
        break;
    case DEV_XCOM_5:	//串口5
        if (len > 64) len = 64;
        SPI_writeData(pdata, len, PortNum);
        OSTimeDly(len);
        SPI_write(IER, DEV_XCOM_5, 0X03); //关闭发送中断
        if (XCOM_PORT[DEV_XCOM_5].UART_send_end != NULL)
        {
            OSSemPend(XCOM_PORT[DEV_XCOM_5].UART_send_end, 500, &err);
            if (err != OS_NO_ERR)
                Xcom_set(DEV_XCOM_5, XCOM_PORT[DEV_XCOM_5].BPS,
                         XCOM_PORT[DEV_XCOM_5].PROTOCOL, XCOM_PORT[DEV_XCOM_5].linecontrl);

        }
        break;
    case DEV_XCOM_8://串口8	新加
        if (len > 64) len = 64;
        SPI_writeData(pdata, len, PortNum);
        OSTimeDly(len);
        SPI_write(IER, DEV_XCOM_8, 0X03); //关闭发送中断
        if (XCOM_PORT[DEV_XCOM_8].UART_send_end != NULL)
        {
            OSSemPend(XCOM_PORT[DEV_XCOM_8].UART_send_end, 500, &err);
            if (err != OS_NO_ERR)
                Xcom_set(DEV_XCOM_8, XCOM_PORT[DEV_XCOM_8].BPS,
                         XCOM_PORT[DEV_XCOM_8].PROTOCOL, XCOM_PORT[DEV_XCOM_8].linecontrl);

        }
        break;
    case DEV_XCOM_9:	//串口9 新加
        if (len > 64) len = 64;
        SPI_writeData(pdata, len, PortNum);
        OSTimeDly(len);
        SPI_write(IER, DEV_XCOM_9, 0X03); //关闭发送中断
        if (XCOM_PORT[DEV_XCOM_9].UART_send_end != NULL)
        {
            OSSemPend(XCOM_PORT[DEV_XCOM_9].UART_send_end, 500, &err);
            if (err != OS_NO_ERR)
                Xcom_set(DEV_XCOM_9, XCOM_PORT[DEV_XCOM_9].BPS,
                         XCOM_PORT[DEV_XCOM_9].PROTOCOL, XCOM_PORT[DEV_XCOM_9].linecontrl);

        }
        break;
    }


}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：剩下的数据长度
////////////////////////////////////////////////////////////////////
uint8 Able_CMD_TB(uint8 *pdata, int lg)
{
    uint8 i, tmp_lg, tmp = 0;
    if (lg == 0) return lg;
    tmp_lg = lg;
    /////////寻找同步头
    for (;;)
    {
        if ((*pdata == ZY_XCOM_FLAG_START) || (*pdata == ZY_XCOM_FLAG_ACK)) break;

        for (i = 0; i < tmp_lg; i++)
            *(pdata + i) = *(pdata + i + 1);
        tmp_lg--;
        tmp++;
        if (tmp_lg == 0) break;

    }
    return lg - tmp;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 XCOM_DEV_Read(uint8 XCOM_DEV_Chanle, uint8 *pdata, uint8 lg)
{
    uint8 i, tmp;
    i = lg;
    switch(XCOM_DEV_Chanle)
    {
    //CPU_UART0
    case DEV_XCOM_MB:
        if ((U0LSR & 1) == 0)
            tmp = U0RBR;
        for (i = lg; i < 32 && (U0LSR & 1); i++)
            *(pdata + i) = U0RBR;
        break;
    //CPU_UART1
    case DEV_XCOM_TYJ:

        if ((U1LSR & 1) == 0)
            tmp = U1RBR;
        for (i = lg; i < 32 && (U1LSR & 1); i++)
            *(pdata + i) = U1RBR;
        break;
    //CPU_UART2
    case DEV_XCOM_SOUND:

        if ((U2LSR & 1) == 0)
            tmp = U2RBR;
        for (i = lg; i < 32 && (U2LSR & 1); i++)
            *(pdata + i) = U2RBR;
        break;
    //CPU_UART3
    case DEV_XCOM_3:

        if ((U3LSR & 1) == 0)
            tmp = U3RBR;
        for (i = lg; i < 32 && (U3LSR & 1); i++)
            *(pdata + i) = U3RBR;
        break;
    //
    case DEV_XCOM_DYX:
        tmp = SPI_read(RXLVL, DEV_XCOM_DYX); //读FIFO中数据字节长度
        if (tmp != 0)
        {
            for (i = lg; i < 32 && (tmp != 0); i++)
            {
                *(pdata + i) = SPI_read(RHR, DEV_XCOM_DYX);	//读出FIFO中数据存入a[]中
                tmp--;
            }
        }
        break;
    case DEV_XCOM_5:
        tmp = SPI_read(RXLVL, DEV_XCOM_5); //读FIFO中数据字节长度
        if (tmp != 0)
        {
            for (i = lg; i < 32 && (tmp != 0); i++)
            {
                *(pdata + i) = SPI_read(RHR, DEV_XCOM_5);	//读出FIFO中数据存入a[]中
                tmp--;

            }
        }
        break;
    case DEV_XCOM_8:
        tmp = SPI_read(RXLVL, DEV_XCOM_8); //读FIFO中数据字节长度
        if (tmp != 0)
        {
            for (i = lg; i < 32 && (tmp != 0); i++)
            {
                *(pdata + i) = SPI_read(RHR, DEV_XCOM_8);	//读出FIFO中数据存入a[]中
                tmp--;

            }
        }
        break;
    case DEV_XCOM_9:
        tmp = SPI_read(RXLVL, DEV_XCOM_9); //读FIFO中数据字节长度
        if (tmp != 0)
        {
            for (i = lg; i < 32 && (tmp != 0); i++)
            {
                *(pdata + i) = SPI_read(RHR, DEV_XCOM_9);	//读出FIFO中数据存入a[]中
                tmp--;

            }
        }
        break;
    default:
        return i;
    }
    return i;
}

uint8 sum_check(uint8 *pdata, uint8 lg)
{
    uint8 sum = 0, i;
    for (i = 0; i < lg; i++)
        sum += *(pdata + i);
    return sum;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
int  UART_REV(uint8 Xcom)
{
    uint8 sum = 0, i, tmp, *prev;
    struct ZY_XCOM *pable;
    struct COMM_XCOM *pxcom;
    struct DEVICE_STATUS_TYPE *ps;

    if (Xcom > DEV_XCOM_9)  return 0; //串口号错误

    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    prev = (uint8 *)&XCOM_PORT[Xcom].Rev_buf[0];
    pxcom = (struct COMM_XCOM *)&XCOM_PORT[Xcom];

    if (Get_Debug(DEV_DEBUG_XCOM))
    {
        pxcom->Rev_lg = XCOM_DEV_Read(Xcom, prev, pxcom->Rev_lg);
        UART_Write(Xcom, (uint8 *)&pxcom->Rev_buf[0], pxcom->Rev_lg);
        pxcom->Rev_lg = 0;
        return  0;
    }
    if (Xcom == DEV_XCOM_3 || Xcom == DEV_XCOM_5 || Xcom == DEV_XCOM_8 || Xcom == DEV_XCOM_9) goto clp1;
    if (pxcom->PROTOCOL == DEV_PROTOCOL_ZYCOM)
    {
xcom_loop01:
        pxcom->Rev_lg = XCOM_DEV_Read(Xcom, prev, pxcom->Rev_lg);
        pxcom->Rev_lg = Able_CMD_TB((uint8 *)&pxcom->Rev_buf[0], pxcom->Rev_lg);
        if (pxcom->Rev_lg < 4) return 0; //没有接收到最小帧
        pable = (struct ZY_XCOM *)&pxcom->Rev_buf[0];
        tmp = (pable->type_lg & 0x0f) + 4;
        if (tmp > pxcom->Rev_lg)
        {
            return 0;
        };//没有接收完整帧
        //纠正面板音量发送错误不检验校验和
        if (Xcom == DEV_XCOM_MB)
            if ((ps->KB_Type == 1) && (pable->flag == ZY_XCOM_FLAG_START) && (pxcom->Rev_buf[3] == 3))
            {
                tmp--;
                goto err1;
            }
        if (sum_check(&pxcom->Rev_buf[0], tmp))
        {
            pxcom->Rev_buf[0] = 0; //
            pxcom->Rev_lg = Able_CMD_TB((uint8 *)&pxcom->Rev_buf[0], pxcom->Rev_lg);
            goto xcom_loop01;
        }
err1://发送数据到接收队列
        XCOM_Q_Send(Xcom, (uint8 *)&pxcom->Rev_buf[0], tmp);

        //通知等待函数
        if ((pxcom->flag != NULL) && (pable->flag == ZY_XCOM_FLAG_ACK)
                && (pxcom->Rflag & XCOM_READ_FLAG_WAIT_ACK))
            OSSemPost(pxcom->flag);
        if ((pxcom->Rflag & XCOM_READ_FLAG_WAIT_DATA)
                && (pxcom->pbuf != NULL)
                && (pable->flag == ZY_XCOM_FLAG_START)
                && (pxcom->flag != NULL)
                && (pxcom->RCMDNo == *(prev + sizeof(struct ZY_XCOM))))
        {
            pxcom->Rdata_lg = (pable->type_lg & 0xf) + 4;
            Mem_Copy(pxcom->pbuf, prev, pxcom->Rdata_lg);
            OSSemPost(pxcom->flag);
        }
        //调整接收缓冲区
        sum = pxcom->Rev_lg - tmp;
        for (i = 0; i < sum; i++)
            pxcom->Rev_buf[i] = pxcom->Rev_buf[i + tmp];
        pxcom->Rev_lg = sum;
        if (sum > 4)
            goto xcom_loop01;

    }
    else
    {
clp1:
        pxcom->Rev_lg = XCOM_DEV_Read(Xcom, prev, pxcom->Rev_lg);
        XCOM_Q_Send(Xcom, (uint8 *)&pxcom->Rev_buf[0], pxcom->Rev_lg);
        if (pxcom->flag != NULL)
            OSSemPost(pxcom->flag);
        pxcom->Rev_lg = 0;

    }
    return pxcom->Rev_lg;
}
void UART_REV1(uint8 Xcom)
{

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
//uint8	UartSendPOWERFlag=0;
//uint8	UartSendQIUFlag=0;

void   EINT1_Exception(void)//u21,
{
    uint32 UxIIR;
    UxIIR = SPI_read(IIR, DEV_XCOM_DYX) & 0x0f;
Uxint1:
    //if((UxIIR==0X04)||(UxIIR==0x0c))
    if(UxIIR == 0x0c)
        UART_REV(DEV_XCOM_DYX);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_DYX].UART_send_end != NULL))
    {
        SPI_write(IER, DEV_XCOM_DYX, 0X01);		//关闭发送中断
        OSSemPost(XCOM_PORT[DEV_XCOM_DYX].UART_send_end);
    }
    UxIIR = SPI_read(IIR, DEV_XCOM_DYX) & 0x0f;
    if((UxIIR == 0X04) || (UxIIR == 0x0c) || (UxIIR == 0x02))
        goto Uxint1;
    ////////////////////////////////////////////////
    UxIIR = SPI_read(IIR, DEV_XCOM_5) & 0x0f;
Uxint2:
    //if((UxIIR==0X04)||(UxIIR==0x0c))
    if(UxIIR == 0x0c)
        UART_REV(DEV_XCOM_5);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_5].UART_send_end != NULL))
    {
        SPI_write(IER, DEV_XCOM_5, 0X01);		//关闭发送中断
        OSSemPost(XCOM_PORT[DEV_XCOM_5].UART_send_end);
    }
    UxIIR = SPI_read(IIR, DEV_XCOM_5) & 0x0f;

    if((UxIIR == 0X04) || (UxIIR == 0x0c) || (UxIIR == 0x02))
        goto Uxint2;
    EXTINT = 0x02;				/* 清除EINT1中断标志		*/
    if ((EXTINT & 0x02) != 0)    IS752_Scan();//IS752_Scan(2);

}

void  EINT2_Exception(void)
{
    uint32 UxIIR;
    UxIIR = SPI_read(IIR, DEV_XCOM_8) & 0x0f;
Uxint1:
    //if((UxIIR==0X04)||(UxIIR==0x0c))
    if(UxIIR == 0x0c)
        UART_REV(DEV_XCOM_8);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_8].UART_send_end != NULL))
    {
        SPI_write(IER, DEV_XCOM_8, 0X01);		//关闭发送中断
        OSSemPost(XCOM_PORT[DEV_XCOM_8].UART_send_end);
    }
    UxIIR = SPI_read(IIR, DEV_XCOM_8) & 0x0f;
    if((UxIIR == 0X04) || (UxIIR == 0x0c) || (UxIIR == 0x02))
        goto Uxint1;
    ////////////////////////////////////////////////
    UxIIR = SPI_read(IIR, DEV_XCOM_9) & 0x0f;
Uxint2:
    //if((UxIIR==0X04)||(UxIIR==0x0c))
    if(UxIIR == 0x0c)
        UART_REV(DEV_XCOM_9);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_9].UART_send_end != NULL))
    {
        SPI_write(IER, DEV_XCOM_9, 0X01);		//关闭发送中断
        OSSemPost(XCOM_PORT[DEV_XCOM_9].UART_send_end);
    }
    UxIIR = SPI_read(IIR, DEV_XCOM_9) & 0x0f;
    if((UxIIR == 0X04) || (UxIIR == 0x0c) || (UxIIR == 0x02))
        goto Uxint2;
    EXTINT = 0x04;				/* 清除EINT2中断标志		*/
    if ((EXTINT & 0x02) != 0)    IS752_Scan2();//IS752_Scan(1);
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void   UART0_Exception(void)
{
    uint32 UxIIR;
    UxIIR = U0IIR & 0x0f;
    //IP_printf("uart 0");
    if((UxIIR == 0X04) || (UxIIR == 0x0c))
        UART_REV(DEV_XCOM_MB);   //改为面板
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_MB].UART_send_end != NULL))
        OSSemPost(XCOM_PORT[DEV_XCOM_MB].UART_send_end); //改为面板
    else
        UxIIR = U0RBR;

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////

void   UART1_Exception(void)
{
    uint32 UxIIR;
    UxIIR = U1IIR & 0x0f;
    //IP_printf("uart 1");
    if((UxIIR == 0X04) || (UxIIR == 0x0c))
        UART_REV(DEV_XCOM_TYJ);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_TYJ].UART_send_end != NULL))
        OSSemPost(XCOM_PORT[DEV_XCOM_TYJ].UART_send_end);
    else
        UxIIR = U1RBR;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void   UART2_Exception(void)
{
    uint32 UxIIR;
    UxIIR = U2IIR & 0x0f;
    //IP_printf("uart 2");
    if((UxIIR == 0X04) || (UxIIR == 0x0c))
        UART_REV(DEV_XCOM_SOUND);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_SOUND].UART_send_end != NULL))
        OSSemPost(XCOM_PORT[DEV_XCOM_SOUND].UART_send_end);
    else
        UxIIR = U2RBR;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：com3 摄像机
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void   UART3_Exception(void)
{

    uint32 UxIIR;
    UxIIR = U3IIR & 0x0f;
    //IP_printf("uart 3");
    if((UxIIR == 0X04) || (UxIIR == 0x0c))
        UART_REV(DEV_XCOM_3);
    else if  ((UxIIR == 0X02) && (XCOM_PORT[DEV_XCOM_3].UART_send_end != NULL))
        OSSemPost(XCOM_PORT[DEV_XCOM_3].UART_send_end);
    else
        UxIIR = U3RBR;
}




////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////

void UART_init(void)
{
    EXTMODE = 0X00; //设置4个外部中断为电平触发
    EXTPOLAR = 0X00; //设置4个外部中断为低电平触发
    EXTINT    = 0x0f;			/* 清除EINT中断标志				*/
    SetVICIRQ(VIC_CHANLE_UART0, 3, (uint32)UART0_Exception);
    SetVICIRQ(VIC_CHANLE_UART1, 4, (uint32)UART1_Exception);
    SetVICIRQ(VIC_CHANLE_UART2, 5, (uint32)UART2_Exception);
    SetVICIRQ(VIC_CHANLE_UART3, 6, (uint32)UART3_Exception);
    SetVICIRQ(VIC_CHANLE_EINT1, 7, (uint32)EINT1_Exception); 	//Uart4,5
    SetVICIRQ(VIC_CHANLE_EINT2, 11, (uint32)EINT2_Exception); //Uart8,9
}



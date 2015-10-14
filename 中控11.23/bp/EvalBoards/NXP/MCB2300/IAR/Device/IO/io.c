#include  "includes.h"
#define	TYJCOUNTER		10
uint8 tyjcounterON = 0;
uint8 tyjcounterOFF = 0;
uint8 tyjcounterFlagON = 0;
uint8 tyjcounterFlagOFF = 0;
struct REALTIME_TYPE time;
uint8 buf[3];
struct MSG_REV_TYPE  Rev;


////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void PINInit(void)
{
    PINSEL10 = 0; //��ֹETM
    PINSEL0 = 0xc055555f; //��������
    PINSEL1 = 0x0145403c; //p0.26 GPIO
    //PINSEL1=0x0155403c;
    PINSEL2 = 0x50150105;
    //PINSEL3=0x00fc0005;//p1.28 GPIO
    PINSEL3 = 0x00c00005; //p1.28 26 GPIO
    PINSEL4 = 0x0140000a;
    PINSEL7 = 0x0; //p3.26 GPIO
    PINSEL9 = 0x0f000000;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void Io_Init(void)
{
#if DBG_TEST   >  0
    PCONP = 0x6fc8139e;
#else
    PCONP = 0x2fc8139e;
#endif
    //��Щ��������λ���
    //SCS=0X01;//����Ϊ����GPIO
    IO0DIR |= (POWER_DOG | KUOZHAN_RST | IC_RST | ICREADER_POWER);
    IO1DIR |= (CTRL2_OUT | COM7_8_CS | COM2_3_CS | BJ_CTRL1);
    FIO2DIR |= (PC1POWER_OUT | LED_OUT | CTRL1_OUT);
    //FIO3DIR|=(CTRL3_OUT|CTRL4_OUT|MAX9675_UPDATA);
    //IO1DIR|=DS18B20_IO;//���
    //��Щ��������Ϊ����
    IO1DIR &= ~(LOAD_DEFAULT | UPDATE);
    FIO2DIR &= ~(PC1POWER_IN);
    //��ʼ��IO��ƽ
    //��Щ��������Ϊ0
    IO0CLR |= (ICREADER_POWER);
    IO1CLR |= (CTRL2_OUT | BJ_CTRL1); /////////////////////////////////
    FIO2CLR |= (PC1POWER_OUT | LED_OUT | CTRL1_OUT);
    //FIO3CLR|=(CTRL3_OUT|CTRL4_OUT);
    //��Щ��������Ϊ1
    IO0SET |= (POWER_DOG | KUOZHAN_RST | IC_RST);
    IO1SET |= (COM2_3_CS | COM7_8_CS);
    //FIO3SET|=MAX9675_UPDATA;
    ///////////////////start////////////////
    //2012.3.15����ָ��ʶ�����
    IO2IntEnF |= ALARM6_IN;
    SetVICIRQ(VIC_CHANLE_EINT3, 12, (uint32)EINT3_Exception);
    //VICVectAddr17 = (uint32)VIC_CHANLE_EINT3;	    // �����жϷ�������ַ
    //VICIntEnable  = 1 << 17;			// ʹ��EINT3�ж�
    Timer3Init();
    Rev.Type = DEV_XCOM_FINGERPRINT;
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_CLASS_BEGIN;

}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void Timer3Init(void)
{
    T3TCR = 0;
    T3CTCR = 0; //��ʱģʽ
    T3PR = HW_PR_TIME10us; //Ԥ��Ƶ10us TC+1����
    T3TC = 0;
    T3MR0 = HW_TIME10MS / 10; //  10ms��ʱ
    T3MCR = 3; //MAT0 �ж�
    T3IR = 0xff;
    SetVICIRQ(VIC_CHANLE_TIME3, 13, (uint32)Timer3_Exception);
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void Timer3_Exception(void)
{
    //OS_ENTER_CRITICAL();
    if(T3IR & 1)
    {
        IO2IntEnR &= ~ALARM6_IN;
        T3TCR = 0;
        //T3TCR=2;
        //T3TCR=2;
        T3IR = 0xff;
    }
    //OS_EXIT_CRITICAL();
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void EINT3_Exception(void)
{
    //OS_ENTER_CRITICAL();
    OS_CPU_SR  cpu_sr;
    if(IO2IntStatF & 0x00000020)
    {
        IO2IntEnR |= ALARM6_IN;
        T3TCR = 1;
        IO2IntClr = 0xFFFFFFFF;
        return;
    }
    else if(IO2IntStatR & 0x00000020)
    {
        IO2IntEnR &= ~ALARM6_IN;
        IO2IntClr = 0xFFFFFFFF;
        /*uint32 TxIR,Treg;
        TxIR=T3IR;
        Treg=T3TC;*/
        if(T3TCR & 1)
        {
            //kaijichuli
            OS_ENTER_CRITICAL();
            T3IR = 0xff;
            T3TCR = 0;
            //T3TCR=2;
            if(Check_FunctionMode(Device_FunctionMode_CARD) == 3)
            {
                Msg_send(buf, 2, (uint8 *)&Rev);
            }
            OS_EXIT_CRITICAL();


            return;
        }

    }
    //if(IO2IntStatR&0x00000040)

    //else if(IO2IntStatF&0x00000080)


    //EXTINT 		= 1 << 3;						// ��EINT1�жϱ�־
    IO2IntClr 	= 0xFFFFFFFF;				// ����P0.6789�ж�
    //OS_EXIT_CRITICAL();
}

/////////////////////end///////////////////

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void IO_Out(uint32  out, uint8 type)
{
    uint32 port, x;
    port = out & 0xff00;
    x = out & 0xff;
    if (type)
    {
        switch(port)
        {
        case IO_PORT0:
            IO0SET |= (1 << x);
            break;
        case IO_PORT1:
            IO1SET |= (1 << x);
            break;
        case IO_PORT2:
            FIO2SET |= (1 << x);
            break;
        case IO_PORT3:
            FIO3SET |= (1 << x);
            break;

        };
    }
    else
    {
        switch(port)
        {
        case IO_PORT0:
            IO0CLR |= (1 << x);
            break;
        case IO_PORT1:
            IO1CLR |= (1 << x);
            break;
        case IO_PORT2:
            FIO2CLR |= (1 << x);
            break;
        case IO_PORT3:
            FIO3CLR |= (1 << x);
            break;

        };
    }
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint8 IO_in(uint32 in)
{
    uint32 port, x, tmp;
    port = in & 0xff00;
    x = in & 0xff;
    switch(port)
    {
    case IO_PORT0:
        if (IO0PIN & (1 << x))
            tmp = TRUE;
        else
            tmp = FALSE;
        break;
    case IO_PORT1:
        if (IO1PIN & (1 << x))
            tmp = TRUE;
        else
            tmp = FALSE;
        break;
    case IO_PORT2:
        if (FIO2PIN & (1 << x))
            tmp = TRUE;
        else
            tmp = FALSE;
        break;
    }
    return tmp;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void IO_FlagLingt_Flash(void)
{
    uint8 tmp;
    tmp = IO_in(OUT_LED);
    if (tmp)
    {
        IO_Out(OUT_LED, FALSE);
        IO_Out(OUT_POWER_DOG, FALSE);
    }
    else
    {
        IO_Out(OUT_LED, TRUE);
        IO_Out(OUT_POWER_DOG, TRUE);
    }
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
BYTE IO_Contrl(BYTE Device, BYTE Option)
{
    uint32 io;
    if (Device == DEV_IO_OUT_NONE)
        return FALSE;

    switch(Device)
    {
    case DEV_IO_OUT_ALARM1:
        io = OUT_ALARM1;
        break;
    case DEV_IO_OUT_ALARM2:
        io = OUT_ALARM2;
        break;
    case DEV_IO_OUT_ALARM3:
        io = OUT_ALARM3;
        break;
    case DEV_IO_OUT_PC1POWER:
        io = OUT_PC1POWER;
        break;
    case DEV_IO_OUT_PC2POWER:
        io = OUT_PC2POWER;
        break;
    case DEV_IO_OUT_DCS:
        io = OUT_DCS;
        break;

    case DEV_IO_OUT_M4:
        io = OUT_M4;
        break;
    default:
        return FALSE;
    }
    IO_Out(io, Option);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 IO_Scan(void)
{
    uint32 tmp_link;
    uint32 tmp_alarm;
    struct DEVICE_STATUS_TYPE *ps;

    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    tmp_alarm = ps->BJ_IN;
    tmp_link = ps->DEV_Link_State;
    //2012.6.7�򿪵����������ʾ
    //if (Device_DCS_Check_in(1))
    //{
    if (gDeviceTable.Alarm_Type & 1)
    {
        /*
        if (IO_in(IN_ALARM1))
        	ps->BJ_IN|=1;
        else
          	ps->BJ_IN&=~1;*/
    }
    else
    {
        /*
        	if (IO_in(IN_ALARM1))
        		ps->BJ_IN&=~1;
        	else
        	  	ps->BJ_IN|=1;*/
    }
    //}
    //if (Device_DCS_Check_in(2))
    //{
    if (gDeviceTable.Alarm_Type & 2)
    {
        /*if (IO_in(IN_ALARM2))
        	ps->BJ_IN|=2;
        else
          	ps->BJ_IN&=~2;*/
    }
    else
    {
        /*if (IO_in(IN_ALARM2))
        	ps->BJ_IN&=~2;
        else
          	ps->BJ_IN|=2;*/
    }
    //}

    //if (Device_DCS_Check_in(3))
    //{
    if (gDeviceTable.Alarm_Type & 4)
    {
        /*if (IO_in(IN_ALARM3))
        	ps->BJ_IN|=4;
        else
          	ps->BJ_IN&=~4;*/
    }
    else
    {
        /*if (IO_in(IN_ALARM3))
        	ps->BJ_IN&=~4;
        else
        	ps->BJ_IN|=4;*/
    }
    //}

    //if (Device_DCS_Check_in(4))
    //{
    if (gDeviceTable.Alarm_Type & 8)
    {
        /*if (IO_in(IN_ALARM4))
        	ps->BJ_IN|=8;
        else
          	ps->BJ_IN&=~8;*/
    }
    else
    {
        /*if (IO_in(IN_ALARM4))
        	ps->BJ_IN&=~8;
        else
          	ps->BJ_IN|=8;*/
    }
    //}

    //if (Device_DCS_Check_in(5))
    //{
    if (gDeviceTable.Alarm_Type & 0x10)
    {
        /*if (IO_in(IN_ALARM5))
        	ps->BJ_IN|=0x10;
        else
          	ps->BJ_IN&=~0x10;*/
    }
    else
    {
        /*if (IO_in(IN_ALARM5))
        	ps->BJ_IN&=~0x10;
        else
          	ps->BJ_IN|=0x10;*/
    }
    //}
    /////////////////////////2012.3.16 ALARM6��������ָ��ʶ��//////////////
    /*if (Device_DCS_Check_in(6))
    {
    		if (gDeviceTable.Alarm_Type&0x20){
    			if (IO_in(IN_ALARM6))
    				ps->BJ_IN|=0x20;
    			else
    			  	ps->BJ_IN&=~0x20;
    		}
    		else{
    			if (IO_in(IN_ALARM6))
    				ps->BJ_IN&=~0x20;
    			else
    			  	ps->BJ_IN|=0x20;
    		}
    }*/
    ///////////////////////////////////////////////////////////////////////
    /*
    	if (IO_in(IN_TYJVGA))
    	ps->BJ_IN&=~0x40;
    else
    	ps->BJ_IN|=0x40;
    */
    //û������

    if( ((ps->BJ_IN ) & 0x40) == 0x40 )
    {
        //if(IO_in(IN_TYJVGA)==0)
        //{
        ps->BJ_IN |= 0x40;
        tyjcounterFlagOFF = 0;
        tyjcounterOFF = 0;
        //}
        /*else
        	if(IO_in(IN_TYJVGA))
        	{
        		tyjcounterFlagOFF=1;
        		if(tyjcounterOFF==TYJCOUNTER)
        			if(IO_in(IN_TYJVGA))
        				{
        					ps->BJ_IN &= ~0x40;
        					tyjcounterFlagOFF=0;
        					tyjcounterOFF=0;
        				}
        	}*/
    }
    //��������
    else
    {
        /*if(IO_in(IN_TYJVGA))
        	{
        		ps->BJ_IN &= ~0x40;
        		tyjcounterFlagON=0;
        		tyjcounterON=0;
        	}
        else
        	if(IO_in(IN_TYJVGA)==0)
        		{*/
        tyjcounterFlagON = 1;
        if(tyjcounterON == TYJCOUNTER)
        {
            ps->BJ_IN |= 0x40;
            tyjcounterFlagON = 0;
            tyjcounterON = 0;
        }
        //}

    }

    //zhw add 2009-09-11
    if  (POWER_FLAG_STATE & 0x01)
    {
    }
    else
    {
        if  (IO_in(IN_PC1POWER)) //�ػ�״̬
        {
            if (ps->DEV_Link_State & 0x01)
            {
                ps->DEV_Link_State &= ~1;
                Key_TB_state(KEY_REP_PC_POWER, FALSE);
                LED_Set(LED_PC_POWER, LED_DISPLAY_OFF);
                LED_SendFLASH();

            }
        }
        else
        {
            if (0x00 == (ps->DEV_Link_State & 0x01))
            {
                ps->DEV_Link_State |= 1;
                Key_TB_state(KEY_REP_PC_POWER, TRUE);

                LED_Set(LED_PC_POWER, LED_DISPLAY_ON);
                LED_SendFLASH();

            }
        }
    }

    if (IO_in(IN_PC2POWER))
        ps->DEV_Link_State &= ~0x10;
    else
        ps->DEV_Link_State |= 0x10;

    IO_FlagLingt_Flash();
    if ((tmp_link != ps->DEV_Link_State) || (tmp_alarm != ps->BJ_IN))
        return STATUS_CHANGE;

    return 0;

}


/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void Device_IC_Reset(void)
{
    uint32 tmp;
    IO_Out(OUT_IC_RST, FALSE);
    for (tmp = 0; tmp < 1000; tmp++);
    IO_Out(OUT_IC_RST, TRUE);
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void Device_KZ_Reset(void)
{
    uint32 tmp;
    IO_Out(OUT_KUOZHAN_RST, FALSE);
    for (tmp = 0; tmp < 1000; tmp++);
    IO_Out(OUT_KUOZHAN_RST, TRUE);
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void Device_DCS_open(void)
{
    IO_Out(OUT_DCS, TRUE);
    OSTimeDly(150);
    IO_Out(OUT_DCS, FALSE);
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint8 BJ_IC_Read(void)
{
    uint8 tmp;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif

    OS_ENTER_CRITICAL();
    BJ_Select(TRUE);

    tmp = SPI_send(0);
    BJ_Select(FALSE);

    OS_EXIT_CRITICAL();
    return tmp;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void ICReader_Power(uint8 type)
{
    if(ICREADER_POWER_ON == type)
        IO0SET |= ICREADER_POWER;
    else if(ICREADER_POWER_OFF == type)
        IO0CLR |= ICREADER_POWER;
}




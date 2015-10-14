#include "includes.h"

extern	uint8 tyjcounterON;
extern	uint8 tyjcounterOFF;
extern	uint8 tyjcounterFlagON;
extern	uint8 tyjcounterFlagOFF;

struct CLASS_TABLE_TYPE   gClassT;
struct REALTIME_TYPE RTime, baktime;
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
struct REALTIME_TYPE *RealTime_Get(void)
{
    uint32 tmp;

    tmp = CTIME0;
    RTime.sec =    tmp & 0x0000003f;
    RTime.min =  (tmp & 0x00003f00) >> 8;
    RTime.hour = (tmp & 0x001f0000) >> 16;
    RTime.week = (tmp & 0x07000000) >> 24;
    tmp = CTIME1;
    RTime.day = tmp & 0x1f;
    RTime.mon = (tmp & 0xf00) >> 8;
    RTime.year = (tmp & 0xffff0000) >> 16;
    return &RTime;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 RealTime_Get_Minute(void)
{
    uint32 tmp;
    RealTime_Get();
    tmp = RTime.hour * 60 + RTime.min;
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
BYTE  Rtime_IRQFlag(void)
{
    struct REALTIME_TYPE *timep;
    BYTE flag = 1;
    timep = RealTime_Get();
    if (timep->min != baktime.min) flag |= 2;
    if (timep->hour != baktime.hour) flag |= 4;
    if (timep->day != baktime.day) flag |= 8;
    if (timep->week != baktime.week) flag |= 0x10;
    memcpy((BYTE *)&baktime, (BYTE *)timep, sizeof(struct REALTIME_TYPE));
    return flag;

}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void RealTime_IRQ(void)
{
    uint8 buf[10], i = 0;
    struct MSG_REV_TYPE  Rev;

    Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
    Rev.Type = DEV_XCOM_TIME;

    buf[0] = MSG_DEV_MAINBARD;

    if(tyjcounterFlagON)
    {
        tyjcounterON++;
        if(tyjcounterON >= 0xff)	tyjcounterON = 0;
    }
    if(tyjcounterFlagOFF)
    {
        tyjcounterOFF++;
        if(tyjcounterOFF >= 0xff)	tyjcounterOFF = 0;
    }

    if (ILR & 0x01)
    {
        buf[1] = MSG_OPTION_TIMEACTIVE;
        buf[2] = Rtime_IRQFlag();
    }
    else if (ILR & 0x02)
    {
        buf[1] = MSG_OPTION_ALM_TIME;
        buf[2] = ALMIN;
        buf[3] = ALHOUR;
    }

    Msg_send(buf, 4, (uint8 *)&Rev);
    Run_Send_Flag();
    ILR |= 3;
    //VICVectAddr=0;

    i++;
    if(i == 3)
    {
        //Temperaturepro();
        i = 0;
    }


}

///////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�RTC �ж�����   1s
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void RealTime_init(void)
{
    struct REALTIME_TYPE *ptime;

    if ((CCR & 0x10) == 0) CCR |= 0x10; //ѡ���ⲿRTCX1��RTCX2����
    CISS = 0;                         //����������ѡ�����μĴ�������Լÿ��488΢��Ͳ���һ���ж�
    CIIR = 0x1;                       //�����������жϼĴ���(CIIR)��ÿ����һ�����һ���ж�
    ILR = 0x03;                       //�ж�λ�üĴ�������������������жϣ���������ж�
    CCR &= ~0x02;                     //ʱ�ӿ��ƼĴ���,�����λ
    CCR = 0x11;	                //ʱ�������ʹ��,CTCʱ��Դѡ���ⲿRTCX1��RTCX2����
    SetVICIRQ(VIC_CHANLE_RTC, 2, (uint32)RealTime_IRQ); //һ�����һ���ж�
    ptime = RealTime_Get();
    memcpy((BYTE *)&baktime, (BYTE *)ptime, sizeof(struct REALTIME_TYPE));
    //memset((BYTE *)&gClassT,0,sizeof(struct CLASS_TABLE_TYPE));//zjj add6
    gClassT.week = RTime.week;
}


/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void ClassTableWeekInit()
{
    struct REALTIME_TYPE *ptime;
    ptime = RealTime_Get();
    gClassT.week = RTime.week;
    ptime = ptime;
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void RealTime_Set(BYTE *time_val)
{

    struct REALTIME_TYPE *ptime;
    ptime = (struct REALTIME_TYPE *)time_val;
    CCR &= ~1;
    SEC = ptime->sec;
    MIN = ptime->min;
    HOUR = ptime->hour;
    DOW = ptime->week;
    DOM = ptime->day;
    MONTH = ptime->mon;
    YEAR = ptime->year;
    CCR |= 1;

}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����1 big
//			 2 small  0 equ
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint8 RealTime_Compare(BYTE *pdata)
{
    struct BASE_TIME *pt;
    uint32 tmp1, tmp2, tmp;
    pt = (struct BASE_TIME *)pdata;
    tmp1 = pt->hour << 8 + pt->min;
    tmp = CTIME0;
    tmp2 = ((tmp & 0x001f0000) >> 8) + ((tmp & 0x003f00) >> 8);
    if (tmp1 > tmp2) return 1;
    if (tmp1 < tmp2) return 2;
    return 0;

}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����option=0 close
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void RealTime_ALM_Set(BYTE cmd, uint8 *time_data)
{
    struct REALTIME_TYPE *ptime;
    ptime = (struct REALTIME_TYPE *)time_data;
    ALSEC = ptime->sec;
    ALMIN = ptime->min;
    ALHOUR = ptime->hour;
    ALDOM = ptime->day;
    ALDOW = ptime->week;
    ALDOY = ptime->year;
    AMR = cmd;
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����option=0 close
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void RealTime_Msg_Set(BYTE cmd, uint8 option)
{
    if (option)
        CIIR |= cmd;
    else
        CIIR &= ~cmd;
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void On_MSG_OPTION_CLASSTABLE_OFF(void)
{
}
BYTE RealTime_Check(BYTE *pdata)
{
    if(( (*pdata == 0) &&
            (*(pdata + 1) == 0)) || (*pdata == 0xff) || (*(pdata + 1) == 0xff)) return FALSE;
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
void On_MSG_OPTION_CLASSTABLE_ON(void)
{
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void ON_MSG_OPTION_ALM_TIME(BYTE *pdata)
{}

#include "includes.h"
struct KEY_TYPE KEY_DATA;
struct LED_TYPE LED_DATA;
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void *Get_KeyData(void )
{
    return &KEY_DATA;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void KEY_Set_Vol(uint8 Vol)
{
    uint8 buf[3];
    buf[0] = KEY_SOUND_VOL;
    buf[1] = Vol;
    XCOM_Data_send(DEV_XCOM_MB, &buf[0], 2, XCOM_CMD_SET_DATA, DEVICE_SUB_MB, TRUE);
    XCOM_Wait_Ack(DEV_XCOM_MB, 10);

}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void LED_Contrl(uint16 Contrl, uint8 type)
{
    uint8 buf[16], lg, err, i, tmp;
    //struct DEVICE_STATUS_TYPE *ps;

    if (type)
        LED_DATA.Contrl |= Contrl;
    else
        LED_DATA.Contrl &= ~Contrl;

    //ps=(struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    //if (ps->DEV_Link_State&DEV_LINK_KBLK)
    tmp = 3;
    //else
    //	tmp=1;
    for (i = 0; i < tmp; i++)
    {
        IO_FlagLingt_Flash();
        buf[0] = KEY_CONTRL;
        buf[1] = LED_DATA.Contrl & 0xff;
        buf[2] = (LED_DATA.Contrl >> 8) & 0xff;
        lg = 3;
        err = XCOM_Data_Read(DEV_XCOM_MB, DEVICE_SUB_MB, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
        if (err != OS_NO_ERR)
        {
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("���Data ����ʧ��");
            else
            {
                IO_FlagLingt_Flash();
                OSTimeDly(50);
            };
        }
        else
            break;
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
void LED_Set(uint32 val, uint8 type)
{
    uint8 num, i;

    switch(type)
    {
    case LED_DISPLAY_ON:
        num = val >> 16;
        LED_DATA.LED_data[num] &= ~(val & 0xffff);
        LED_DATA.LED_type[num] &= ~(val & 0xffff);
        break;
    case LED_DISPLAY_OFF:
        num = val >> 16;
        LED_DATA.LED_data[num] |= val & 0xffff;
        LED_DATA.LED_type[num] &= ~(val & 0xffff);
        break;
    case LED_DISPLAY_FLASH:
        num = val >> 16;
        LED_DATA.LED_data[num] &= ~(val & 0xffff);
        LED_DATA.LED_type[num] |= (val & 0xffff);
        break;
    case LED_ALL_CLR:
        for (i = 0; i < 4; i++)
            LED_DATA.LED_data[i] = 0xffff;
        for (i = 0; i < 4; i++)
            LED_DATA.LED_type[i] = 0;
        break;
    case LED_ALL_SET:
        for (i = 0; i < 4; i++)
            LED_DATA.LED_data[i] = 0;
        for (i = 0; i < 4; i++)
            LED_DATA.LED_type[i] = 0;
        break;
    case LED_ALL_FLASH:
        for (i = 0; i < 4; i++)
            LED_DATA.LED_data[i] = 0;
        for (i = 0; i < 4; i++)
            LED_DATA.LED_type[i] = 0xffff;
        break;
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
void LED_Time_Flash(void)
{
    BYTE buf[16], lg, err;
    struct REALTIME_TYPE *stime;

    stime = RealTime_Get();
    LED_DATA.LED_time[0] = stime->hour;
    LED_DATA.LED_time[1] = stime->min;
    LED_DATA.LED_time[2] = stime->sec;
    LED_DATA.LED_time[3] = (stime->year) / 256;
    LED_DATA.LED_time[4] = (stime->year) % 256;
    LED_DATA.LED_time[5] = stime->mon;
    LED_DATA.LED_time[6] = stime->day;
    LED_DATA.LED_time[7] = stime->week;
    buf[0] = KEY_TIME;
    memcpy(&buf[1], LED_DATA.LED_time, 8);
    lg = 9;
    err = XCOM_Data_Read(DEV_XCOM_MB, DEVICE_SUB_MB, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
    err = err;


}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void LED_SendFLASH(void)
{
    uint8 buf[16], lg, err, i, tmp;
    //struct DEVICE_STATUS_TYPE *ps;
    //ps=(struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    //if (ps->DEV_Link_State&DEV_LINK_KBLK)
    tmp = 3;
    //else
    //	tmp=1;
    for (i = 0; i < tmp; i++)
    {
        IO_FlagLingt_Flash();
        buf[0] = LED_CHANLE_TYPE;
        memcpy(buf + 1, (uint8 *)LED_DATA.LED_type, 8);
        lg = 9;
        err = XCOM_Data_Read(DEV_XCOM_MB, DEVICE_SUB_MB, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
        if (err != OS_NO_ERR)
        {
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("���Type����ʧ��");
            else
            {
                IO_FlagLingt_Flash();
                OSTimeDly(50);
            };
        }
        else
            break;
    }
    for (i = 0; i < tmp; i++)
    {
        IO_FlagLingt_Flash();
        buf[0] = LED_CHANLE_DATA;
        memcpy(buf + 1, (uint8 *)LED_DATA.LED_data, 8);
        lg = 9;
        err = XCOM_Data_Read(DEV_XCOM_MB, DEVICE_SUB_MB, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
        if (err != OS_NO_ERR)
        {
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("���Data ����ʧ��");
            else
            {
                IO_FlagLingt_Flash();
                OSTimeDly(50);
            };
        }
        else
            break;
    }



}

////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void key_init(void)
{
    uint8 i, lg;
    uint8 buf[16], err;
    struct DEVICE_STATUS_TYPE *ps;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    KEY_DATA.CURR_DEVICE = MSG_DEV_PC;
    LED_Set(LED_PC, LED_DISPLAY_ON);
    buf[0] = MSG_DEV_PC;
    buf[1] = 0;
    buf[3] = DEV_XCOM_MB;
    Msg_send(buf, 2, &buf[3]);

    KEY_DATA.METRIC = MSG_OPTION_METRIC1;
    LED_Set(LED_METRIC_XGA, LED_DISPLAY_ON);
    buf[0] = MSG_DEV_LMT;
    buf[1] = MSG_OPTION_METRIC1;
    buf[3] = DEV_XCOM_MB;
    Msg_send(buf, 2, &buf[3]);

    KEY_DATA.CODE1_SPEED = MSG_OPTION_CODE1_SPEED2;
    LED_Set(LED_SPEED_1M, LED_DISPLAY_ON);
    buf[0] = MSG_DEV_LMT;
    buf[1] = MSG_OPTION_CODE1_SPEED2;
    buf[3] = DEV_XCOM_MB;
    Msg_send(buf, 2, &buf[3]);

    KEY_DATA.FRAME = MSG_OPTION_FRAME1;
    LED_Set(LED_FRAME_CG, LED_DISPLAY_ON);
    buf[0] = MSG_DEV_LMT;
    buf[1] = MSG_OPTION_FRAME1;
    buf[3] = DEV_XCOM_MB;
    Msg_send(buf, 2, &buf[3]);

    KEY_DATA.PERSONAL_TALL = MSG_OPTION_PERSONAL_TALL2;
    LED_Set(LED_TAIL_170, LED_DISPLAY_ON);
    buf[0] = MSG_DEV_TRACK;
    buf[1] = MSG_OPTION_PERSONAL_TALL2;
    buf[3] = DEV_XCOM_MB;
    Msg_send(buf, 2, &buf[3]);


    KEY_DATA.POWER_DEV = MSG_DEV_TYM;
    LED_Set(LED_TYM, LED_DISPLAY_ON);
    gDeviceStatus.Curr_Contrl_Device = MSG_DEV_TYM;

    KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA1;
    LED_Set(LED_SCAMERA1, LED_DISPLAY_ON);
    buf[0] = MSG_DEV_TRACK;
    buf[1] = MSG_OPTION_SELECT_SCAMERA1;
    buf[3] = DEV_XCOM_MB;
    Msg_send(buf, 2, &buf[3]);

    KEY_DATA.MODE = MSG_OPTION_AUTO_ON;
    LED_Set(LED_MODE_ZD, LED_DISPLAY_ON);
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_AUTO_ON;
    buf[3] = DEV_XCOM_MB;
    Msg_send(buf, 2, &buf[3]);

    gDeviceStatus.CLASS_state &= 0x0f;
    gDeviceStatus.CLASS_state |= DEV_CLASS_RECSTOP;
    KEY_DATA.key_rep &= ~KEY_REP_REC_PUSH;
    LED_Set(LED_REC, LED_DISPLAY_OFF);

    LED_SendFLASH();
    LED_Time_Flash();
    KEY_DATA.RF_lg = 0;
    KEY_DATA.CONTRL = 0;
    //�����İ汾��
    for (i = 0; i < 10; i++)
    {
        buf[0] = 0;
        lg = 1;
        IO_FlagLingt_Flash();
        err = XCOM_Data_Read(DEV_XCOM_MB, DEVICE_SUB_MB, XCOM_CMD_GET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_DATA);
        if (err  !=  OS_NO_ERR)
        {
            //UART_printf("User init Key--Error!\n");
            IP_printf("�����Լ����\n");
        }
        else
        {
            //UART_printf("User init Key--OK  Ver=%2x%2x",buf[4],buf[5]);
            IP_printf("�����Լ�ͨ���汾%d,%d", buf[4], buf[5]);
            ps->DEV_Link_State |= DEV_LINK_KBLK;
            if(buf[4] == 0x01)
            {
                ps->KB_Type = KB_TYPE_ZHONGKONG;
                IP_printf("�пؼ���\n");
            }
            else if (buf[4] >= 0x80)
            {
                ps->KB_Type = KB_TYPE_KUREI;
                IP_printf("�����\n");
            }
            else
            {
                IP_printf("��ҫ����\n");
                ps->KB_Type = KB_TYPE_RONGYAO;
            }
            return;
        }
        IO_FlagLingt_Flash();
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
void LED_device_set(struct BASE_MSG *pmsg, uint8 type)
{
    uint32 tmp_led = 0;
    switch(pmsg->device)
    {
    case MSG_DEV_PC:
        if (pmsg->option == 0)
            tmp_led = LED_PC;
        else
            tmp_led = LED_PC_POWER;
        break;
    case MSG_DEV_LIGHT:
        tmp_led = LED_LIGHT_POWER;
        break;
    case MSG_DEV_GF:
        if ((pmsg->option == MSG_OPTION_POWEROPEN) || (pmsg->option == MSG_OPTION_POWERCLOSE))
            tmp_led = LED_GF_POWER;
        else
            tmp_led = LED_MUTE;
        break;
    case MSG_DEV_TYJA:
        tmp_led = LED_TYJ_POWER;
        break;
    case MSG_DEV_MAINBARD:
        if ((pmsg->option == MSG_OPTION_HELP_OFF) || (pmsg->option == MSG_OPTION_HELP_ON))
            tmp_led = LED_HELP;
        else if (pmsg->option == MSG_OPTION_AUTO_ON) //�Զ�
        {
            LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
            tmp_led = LED_MODE_ZD;
        }
        else if (pmsg->option == MSG_OPTION_AUTO_OFF) //�ֶ�
        {
            LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
            tmp_led = LED_MODE_SD;
        }
        else if ((pmsg->option == MSG_OPTION_DEV_REMOTE_ON) || (pmsg->option == MSG_OPTION_REC_REMOTE_ON)
                 || (pmsg->option == MSG_OPTION_REC_REMOTE_OFF) || (pmsg->option == MSG_OPTION_DEV_REMOTE_OFF))
            tmp_led = LED_MODE_YC;
        break;
    case MSG_DEV_TYM:
        tmp_led = LED_TYM;
        break;
    case MSG_DEV_LCD:
        tmp_led = LED_LCD_UP;
        break;
    case MSG_DEV_TYJUP:
        tmp_led = LED_TYDJ;
        break;
    case MSG_DEV_PELMET:
        tmp_led = LED_PELMET_POWER;
        break;
    case MSG_DEV_NPC:
        tmp_led = LED_NPC;
        break;
    case MSG_DEV_KZA:
        tmp_led = LED_KZA;
        break;
    case MSG_DEV_KZB	:
        tmp_led = LED_KZB;
        break;
    case MSG_DEV_DVD:
        tmp_led = LED_DVD;
        break;
    case MSG_DEV_SHOW:
        tmp_led = LED_SHOW;
        break;
    case MSG_DEV_LYJ:
        tmp_led = LED_RECODER;
        break;
    case MSG_DEV_LMT:
        if ((pmsg->option == MSG_OPTION_RECORD_PAUSE) || (pmsg->option == MSG_OPTION_RECORD_START)
                || (pmsg->option == MSG_OPTION_RECORD_STOP))
            tmp_led = LED_REC;
        else if(pmsg->option == MSG_OPTION_CODE1_SPEED1)
            tmp_led = LED_SPEED_400;
        else if(pmsg->option == MSG_OPTION_CODE1_SPEED2)
            tmp_led = LED_SPEED_1M;
        else if(pmsg->option == MSG_OPTION_CODE1_SPEED3)
            tmp_led = LED_SPEED_ZDY;
        else if(pmsg->option == MSG_OPTION_METRIC1)
            tmp_led = LED_METRIC_XGA;
        else if(pmsg->option == MSG_OPTION_METRIC2)
            tmp_led = LED_METRIC_D1;
        else if(pmsg->option == MSG_OPTION_METRIC3)
            tmp_led = LED_METRIC_ZDY;
        else if(pmsg->option == MSG_OPTION_FRAME1)
            tmp_led = LED_FRAME_CG;
        else if(pmsg->option == MSG_OPTION_FRAME2)
            tmp_led = LED_FRAME_HZH;
        else if(pmsg->option == MSG_OPTION_FRAME3)
            tmp_led = LED_FRAME_ZDY;
        break;
    case MSG_DEV_TRACK:
        if(pmsg->option == MSG_OPTION_PERSONAL_TALL1)
            tmp_led = LED_TAIL_160;
        else if(pmsg->option == MSG_OPTION_PERSONAL_TALL2)
            tmp_led = LED_TAIL_170;
        else if(pmsg->option == MSG_OPTION_PERSONAL_TALL3)
            tmp_led = LED_TAIL_180;
        else if(pmsg->option == MSG_OPTION_SELECT_SCAMERA1)
            tmp_led = LED_SCAMERA1;
        else if(pmsg->option == MSG_OPTION_SELECT_SCAMERA2)
            tmp_led = LED_SCAMERA2;
        else if(pmsg->option == MSG_OPTION_SELECT_SCAMERA3)
            tmp_led = LED_SCAMERA3;
        else if(pmsg->option == MSG_OPTION_SELECT_SCAMERA4)
            tmp_led = LED_SCAMERA4;
        else if(pmsg->option == MSG_OPTION_SELECT_SCAMERA5)
            tmp_led = LED_SCAMERA5;
        else if(pmsg->option == MSG_OPTION_SELECT_SCAMERA6)
            tmp_led = LED_SCAMERA6;
        break;
    default:
        break;
    }

    if (tmp_led != 0)
        LED_Set(tmp_led, type);
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void Key_TB_state(uint16 flag, uint16 type)
{
    if (type)
    {
        KEY_DATA.key_rep |= flag;
    }
    else
    {
        KEY_DATA.key_rep &= ~flag;
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
void KEY_CMD(uint8 *pkey)
{


    uint8 led_type, buf[15], *ip; //,*p;
    uint16 key;
    struct BASE_MSG keymsg;
    struct DEVICE_STATUS_TYPE *ps;
    struct MSG_REV_TYPE  ReV;

    Mem_Clr(buf, 15);
    Mem_Clr((uint8 *)&ReV, sizeof(struct MSG_REV_TYPE));
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    memset(buf, 0, sizeof(buf));
    keymsg.device = 0;
    keymsg.option = 0;
    ReV.Type = DEV_XCOM_MB;
    led_type = LED_DISPLAY_ON;
    key = *(pkey + 1) + *(pkey + 2) * 256;
    if (key == KEY_HELP)
        goto key_loop;
    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_RUN)
        return;
    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
    {
        if ((key == KEY_CLASS_BEGIN) && (Check_FunctionMode(Device_FunctionMode_CARD) == 0))
        {
            //�Ͽ�
            keymsg.option = MSG_OPTION_CLASS_BEGIN;
            keymsg.device = MSG_DEV_MAINBARD;
            KEY_DATA.key_rep &= ~KEY_REP_CLASS;
            buf[0] = MSG_DEV_MAINBARD;
            buf[1] = MSG_OPTION_CLASS_BEGIN;
            buf[2] = 0x01;
            Msg_send(buf, 3, (uint8 *)&ReV);
            //LED_Set(LED_POWER,LED_DISPLAY_ON);
            /*
            led_type = LED_DISPLAY_FLASH;
            LED_device_set(&keymsg,led_type);
            LED_SendFLASH();
            */

            if (Get_Debug(DEV_DEBUG_MSG)) IP_printf("�Ͽμ�1");
        }
        return;
    }

key_loop:
    switch(key)
    {
    case KEY_CLASS_BEGIN://�Ͽμ�
        //IP_printf("�Ͽμ�2");
        if(Check_FunctionMode(Device_FunctionMode_CARD) == 0 || Check_FunctionMode(Device_FunctionMode_CARD) == 3)
        {
            keymsg.device = MSG_DEV_MAINBARD;
            if ((KEY_DATA.key_rep & KEY_REP_CLASS) == KEY_REP_CLASS)
            {
                keymsg.option = MSG_OPTION_CLASS_BEGIN;
                KEY_DATA.key_rep &= ~KEY_REP_CLASS;
            }
            else
            {
                keymsg.option = MSG_OPTION_CLASS_OVER;
                KEY_DATA.key_rep |= KEY_REP_CLASS;
            }
        }
        break;
    //*
    case KEY_MIC_MUTE://mic������
        IP_printf("mic������");
        keymsg.device = MSG_DEV_TMIC;
        if ((KEY_DATA.key_rep & KEY_REP_MIC_MUTE) == KEY_REP_MIC_MUTE)
        {
            KEY_DATA.key_rep &= ~KEY_REP_MIC_MUTE;
            keymsg.option = MSG_OPTION_MUTECLOSE;
        }
        else
        {
            KEY_DATA.key_rep |= KEY_REP_MIC_MUTE;
            keymsg.option = MSG_OPTION_MUTEOPEN;
        }
        break;
    case KEY_PC_POWER://�μ����Ե�Դ
        IP_printf("�μ����Լ�");
        keymsg.device = MSG_DEV_PC;
        if ((KEY_DATA.key_rep & KEY_REP_PC_POWER) != KEY_REP_PC_POWER)
        {
            KEY_DATA.key_rep |= KEY_REP_PC_POWER;
            keymsg.option = MSG_OPTION_POWEROPEN;
            led_type = LED_DISPLAY_FLASH;
        }
        else
        {
            KEY_DATA.key_rep &= ~KEY_REP_PC_POWER;
            keymsg.option = MSG_OPTION_POWERCLOSE;
            led_type = LED_DISPLAY_FLASH;
        }
        break;
    case KEY_LIGHT_POWER://���ڵƹ�
        IP_printf("���ڵƹ��");
        keymsg.device = MSG_DEV_LIGHT;
        if ((KEY_DATA.key_rep & KEY_REP_LIGHT_POWER) != KEY_REP_LIGHT_POWER)
        {
            KEY_DATA.key_rep |= KEY_REP_LIGHT_POWER;
            keymsg.option = MSG_OPTION_POWEROPEN;
            led_type = LED_DISPLAY_FLASH;
        }
        else
        {
            KEY_DATA.key_rep &= ~KEY_REP_LIGHT_POWER;
            keymsg.option = MSG_OPTION_POWERCLOSE;
            led_type = LED_DISPLAY_OFF;
        }
        break;
    case KEY_GF_POWER://���ŵ�Դ
        IP_printf("���ż�");
        keymsg.device = MSG_DEV_GF;
        if ((KEY_DATA.key_rep & KEY_REP_GF_POWER) != KEY_REP_GF_POWER)
        {
            KEY_DATA.key_rep |= KEY_REP_GF_POWER;
            keymsg.option = MSG_OPTION_POWEROPEN;
            led_type = LED_DISPLAY_FLASH;
        }
        else
        {
            KEY_DATA.key_rep &= ~KEY_REP_GF_POWER;
            keymsg.option = MSG_OPTION_POWERCLOSE;
            led_type = LED_DISPLAY_OFF;
        }
        break;
    //*/
    case KEY_TYJ_POWER://ͶӰ����Դ
        IP_printf("ͶӰ����");
        //ͶӰ���Ƚ�����(��Ϊ����ʱ��Ƚϳ�)�����ع����ж������ٿ��͹�zjj add6
        keymsg.device = MSG_DEV_TYJA;
        if ((KEY_DATA.key_rep & KEY_REP_TYJ_POWER) != KEY_REP_TYJ_POWER)
        {
            if ((gDeviceStatus.DEV_Link_State & DEV_LINK_TYJON) == 0)
            {
                KEY_DATA.key_rep |= KEY_REP_TYJ_POWER;
                keymsg.option = MSG_OPTION_POWEROPEN;
                led_type = LED_DISPLAY_FLASH;
            }
            else
            {
                //keymsg.device = keymsg.option = 0;
                return;
            }
        }
        else
        {
            if ((gDeviceStatus.DEV_Link_State & DEV_LINK_TYJON) != 0)
            {
                KEY_DATA.key_rep &= ~KEY_REP_TYJ_POWER;
                keymsg.option = MSG_OPTION_POWERCLOSE;
                led_type = LED_DISPLAY_OFF;
            }
            else
            {
                //keymsg.device = keymsg.option = 0;
                return;
            }
        }
        break;
    case KEY_DEVICE_UP://����
        IP_printf("������");
        keymsg.device = KEY_DATA.POWER_DEV;
        led_type = LED_DISPLAY_FLASH;
        keymsg.option = MSG_OPTION_POWEROPEN;
        if (KEY_DATA.Curr_option != MSG_OPTION_POWEROPEN)
        {
            KEY_DATA.Curr_option = MSG_OPTION_POWEROPEN;
            if (keymsg.device == MSG_DEV_TYM)
                buf[10] = DEV_POWER_TYM;
            else if (keymsg.device == MSG_DEV_PELMET)
                buf[10] = DEV_POWER_CL;
            else if (keymsg.device == MSG_DEV_TYJUP)
                buf[10] = DEV_POWER_TYJUP_DOWN;
            else
                buf[10] = DEV_POWER_LCD;
            POWER_Set(buf[10], MSG_OPTION_POWERCLOSE);
        }
        buf[0] = keymsg.device;
        Task_Rec_Del(buf, 1);
        buf[1] = keymsg.option;
        buf[2] = DEV_XCOM_SELF;
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 1, buf, 2, 0, &buf[2]);

        //keymsg.device =0;
        break;
    case KEY_DEVICE_STOP://ֹͣ
        IP_printf("ֹͣ��");
        keymsg.device = KEY_DATA.POWER_DEV;
        KEY_DATA.Curr_option = MSG_OPTION_POWERCLOSE;
        led_type = LED_DISPLAY_ON;
        keymsg.option = MSG_OPTION_POWERCLOSE;
        buf[0] = keymsg.device;
        Task_Rec_Del(buf, 1);
        break;
    case KEY_DEVICE_DOWN://�½�
        IP_printf("�½���");
        keymsg.device = KEY_DATA.POWER_DEV;
        led_type = LED_DISPLAY_FLASH;
        keymsg.option = MSG_OPTION_POWERDOWN;
        if (KEY_DATA.Curr_option != MSG_OPTION_POWERDOWN)
        {
            KEY_DATA.Curr_option = MSG_OPTION_POWERDOWN;
            if (keymsg.device == MSG_DEV_TYM)
                buf[10] = DEV_POWER_TYM;
            else if (keymsg.device == MSG_DEV_PELMET)
                buf[10] = DEV_POWER_CL;
            else if (keymsg.device == MSG_DEV_TYJUP)
                buf[10] = DEV_POWER_TYJUP_DOWN;
            else
                buf[10] = DEV_POWER_LCD;
            POWER_Set(buf[10], MSG_OPTION_POWERCLOSE);
        }
        buf[0] = keymsg.device;
        Task_Rec_Del(buf, 1);
        buf[1] = keymsg.option;
        buf[2] = DEV_XCOM_SELF;
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 1, buf, 2, 0, &buf[2]);
        //keymsg.device =0;
        break;
    //*
    case KEY_DEVICE_SELECT://�豸ѡ��
        IP_printf("�豸ѡ���");
        LED_Set(LED_TYM, LED_DISPLAY_OFF);
        LED_Set(LED_LCD_UP, LED_DISPLAY_OFF);
        LED_Set(LED_TYDJ, LED_DISPLAY_OFF);
        LED_Set(LED_PELMET_POWER, LED_DISPLAY_OFF);
        if (KEY_DATA.POWER_DEV == MSG_DEV_TYM)
        {
            KEY_DATA.POWER_DEV = MSG_DEV_LCD;
            keymsg.device = MSG_DEV_LCD;
        }
        else if (KEY_DATA.POWER_DEV == MSG_DEV_LCD)
        {
            KEY_DATA.POWER_DEV = MSG_DEV_TYJUP;
            keymsg.device = MSG_DEV_TYJUP;
        }
        else if (KEY_DATA.POWER_DEV == MSG_DEV_TYJUP)
        {
            KEY_DATA.POWER_DEV = MSG_DEV_PELMET;
            keymsg.device = MSG_DEV_PELMET;
        }
        else
        {
            KEY_DATA.POWER_DEV = MSG_DEV_TYM;
            keymsg.device = MSG_DEV_TYM;
        }
        keymsg.option = 0xff;
        gDeviceStatus.Curr_Contrl_Device = KEY_DATA.POWER_DEV;
        break;
    case KEY_HELP:
        IP_printf("������");
        keymsg.device = MSG_DEV_MAINBARD;
        if ((KEY_DATA.key_rep & KEY_REP_HELP) == KEY_REP_HELP)
        {
            LED_Set(LED_HELP, LED_DISPLAY_OFF);
            KEY_DATA.key_rep &= ~KEY_REP_HELP;
            keymsg.option = MSG_OPTION_HELP_OFF;
            led_type = LED_DISPLAY_OFF;
        }
        else
        {
            KEY_DATA.key_rep |= KEY_REP_HELP;
            keymsg.option = MSG_OPTION_HELP_ON;
            led_type = LED_DISPLAY_FLASH;
        }
        break;
    //*/
    case KEY_DEVICE_PC:	//�μ������
        IP_printf("�μ������ѡ���");
        Temperaturepro();///����
        if(Check_FunctionMode(Device_FunctionMode_LB) == FALSE) //add 2014.12.30
        {
            IP_printf("8168δ����,�����л�VGA");
            break;
        }
        keymsg.device = KEY_DATA.CURR_DEVICE;
        LED_device_set(&keymsg, LED_DISPLAY_OFF);
        keymsg.device = KEY_DATA.CURR_DEVICE = MSG_DEV_PC;
        //zjj add6 ȡ������
        led_type = LED_DISPLAY_ON;
        Key_TB_state(KEY_REP_DEVICE_MUTE, FALSE);
        LED_Set(LED_MUTE, LED_DISPLAY_OFF);
        gDeviceStatus.MK_Vol &= 0x7FFF;
        KEY_DATA.key_rep |= KEY_REP_PLAY_PUSH;
        break;
    case KEY_DEVICE_NPC://13//�ʼǱ�
        IP_printf("�ʼǱ�ѡ���");
        if(Check_FunctionMode(Device_FunctionMode_LB) == FALSE) //add 2014.12.30
        {
            IP_printf("8168δ����,�����л�VGA");
            break;
        }
        keymsg.device = KEY_DATA.CURR_DEVICE;
        LED_device_set(&keymsg, LED_DISPLAY_OFF);
        keymsg.device = KEY_DATA.CURR_DEVICE = MSG_DEV_NPC;
        //zjj add6 ȡ������
        led_type = LED_DISPLAY_ON;
        Key_TB_state(KEY_REP_DEVICE_MUTE, FALSE);
        LED_Set(LED_MUTE, LED_DISPLAY_OFF);
        gDeviceStatus.MK_Vol &= 0x7FFF;
        KEY_DATA.key_rep |= KEY_REP_PLAY_PUSH;
        break;
    case KEY_DEVICE_SHOW://17//��Ƶչ̨
        IP_printf("��Ƶչ̨ѡ���");
        if(Check_FunctionMode(Device_FunctionMode_LB) == FALSE) //add 2014.12.30
        {
            IP_printf("8168δ����,�����л�VGA");
            break;
        }
        keymsg.device = KEY_DATA.CURR_DEVICE;
        LED_device_set(&keymsg, LED_DISPLAY_OFF);
        keymsg.device = KEY_DATA.CURR_DEVICE = MSG_DEV_SHOW;
        //zjj add6 ȡ������
        led_type = LED_DISPLAY_ON;
        //Key_TB_state(KEY_REP_DEVICE_MUTE, FALSE);
        LED_Set(LED_MUTE, LED_DISPLAY_OFF);
        //gDeviceStatus.MK_Vol &=0x7FFF;
        KEY_DATA.key_rep |= KEY_REP_PLAY_PUSH;
        break;
    //*
    case KEY_DEVICE_DVD:
        IP_printf("dvdѡ���");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        LED_device_set(&keymsg, LED_DISPLAY_OFF);
        keymsg.device = KEY_DATA.CURR_DEVICE = MSG_DEV_DVD;
        //zjj add6 ȡ������
        led_type = LED_DISPLAY_ON;
        Key_TB_state(KEY_REP_DEVICE_MUTE, FALSE);
        LED_Set(LED_MUTE, LED_DISPLAY_OFF);
        gDeviceStatus.MK_Vol &= 0x7FFF;
        KEY_DATA.key_rep |= KEY_REP_PLAY_PUSH;
        break;
    case KEY_DEVICE_RECODER://19//¼����
        IP_printf("¼����ѡ���");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        LED_device_set(&keymsg, LED_DISPLAY_OFF);
        keymsg.device = KEY_DATA.CURR_DEVICE = MSG_DEV_LYJ;
        //zjj add6 ȡ������
        led_type = LED_DISPLAY_ON;
        Key_TB_state( KEY_REP_DEVICE_MUTE, FALSE );
        LED_Set(LED_MUTE, LED_DISPLAY_OFF);
        gDeviceStatus.MK_Vol &= 0x7FFF;
        KEY_DATA.key_rep |= KEY_REP_PLAY_PUSH;
        break;
    case KEY_DEVICE_KZA:
        if (Check_FunctionMode(Device_FunctionMode_YY) == FALSE)
        {
            KEY_DATA.key_rep &= ~KEY_REP_JXZX;
            IP_printf("��չAѡ���");
            keymsg.device = KEY_DATA.CURR_DEVICE;
            LED_device_set(&keymsg, LED_DISPLAY_OFF);
            keymsg.device = KEY_DATA.CURR_DEVICE = MSG_DEV_KZA;
            //zjj add6 ȡ������
            led_type = LED_DISPLAY_ON;
            Key_TB_state( KEY_REP_DEVICE_MUTE, FALSE );
            LED_Set(LED_MUTE, LED_DISPLAY_OFF);
            gDeviceStatus.MK_Vol &= 0x7FFF;
            KEY_DATA.key_rep |= KEY_REP_PLAY_PUSH;
        }
        else
        {
            //����ģ��򿪣���չAת��ɽ�ѧ/��ϰ
            if (KEY_DATA.key_rep & KEY_REP_JXZX)
            {
                IP_printf("��ϰģʽ");
                KEY_DATA.key_rep &= ~KEY_REP_JXZX;
                LED_Set(LED_KZA, LED_DISPLAY_OFF);
                keymsg.device = MSG_DEV_MAINBARD;
                keymsg.option = MSG_OPTION_YY_NoTeather;
            }
            else
            {
                IP_printf("��ѧģʽ");
                KEY_DATA.key_rep |= KEY_REP_JXZX;
                LED_Set(LED_KZA, LED_DISPLAY_ON);
                keymsg.device = MSG_DEV_MAINBARD;
                keymsg.option = MSG_OPTION_YY_Teather;
            }
        }

        break;
    case KEY_DEVICE_KZB:
        if (Check_FunctionMode(Device_FunctionMode_LB) == FALSE)
        {
            //¼�����ܹرգ�һ�������չ�豸B
            IP_printf("��չBѡ���");
            keymsg.device = KEY_DATA.CURR_DEVICE;
            LED_device_set(&keymsg, LED_DISPLAY_OFF);
            keymsg.device = KEY_DATA.CURR_DEVICE = MSG_DEV_KZB;
            //zjj add6 ȡ������
            led_type = LED_DISPLAY_ON;
            Key_TB_state( KEY_REP_DEVICE_MUTE, FALSE );
            LED_Set(LED_MUTE, LED_DISPLAY_OFF);
            gDeviceStatus.MK_Vol &= 0x7FFF;
            KEY_DATA.key_rep |= KEY_REP_PLAY_PUSH;
        }
        else
        {
            //¼�����ܴ򿪣�ת��ΪԤ¼��
            if ((gDeviceStatus.CLASS_state & DEV_CLASS_RECSTART) ||
                    (gDeviceStatus.CLASS_state & DEV_CLASS_RECPUSH))
            {
                IP_printf(" ¼�ƿ�ʼ��Ԥ¼�ƿ�ʼ����Ч");
                return;
            }
            if (KEY_DATA.key_rep & KEY_REP_YREC)
            {
                KEY_DATA.key_rep &= ~KEY_REP_YREC;
                LED_Set(LED_KZB, LED_DISPLAY_OFF);
                keymsg.device = MSG_DEV_LMT;
                keymsg.option = MSG_OPTION_YREC_Stop;
                IP_printf("Ԥ¼��ֹͣ");
            }
            else
            {
                KEY_DATA.key_rep |= KEY_REP_YREC;
                LED_Set(LED_KZB, LED_DISPLAY_ON);
                keymsg.device = MSG_DEV_LMT;
                keymsg.option = MSG_OPTION_YREC_Begin;
                IP_printf("Ԥ¼�ƿ�ʼ");
            }
        }
        break;
    //*/
    case KEY_REC_PUSH://15//¼��/��ͣ
        if (Check_FunctionMode(Device_FunctionMode_LB) && (KEY_DATA.key_rep & KEY_REP_YREC)) break;

        if(Check_FunctionMode(Device_FunctionMode_LB) == FALSE)
        {
            IP_printf("8168δ����,����¼��");
            break;
        }/**/
        keymsg.device = MSG_DEV_LMT;
        LED_Set(LED_REC_STOP, LED_DISPLAY_OFF);
        if ((KEY_DATA.key_rep & KEY_REP_REC_PUSH) != KEY_REP_REC_PUSH)
        {
            IP_printf("¼�ƿ�ʼ��");
            KEY_DATA.key_rep |= KEY_REP_REC_PUSH;
            keymsg.option = MSG_OPTION_RECORD_START;
            led_type = LED_DISPLAY_ON;
        }
        else
        {
            KEY_DATA.key_rep &= ~KEY_REP_REC_PUSH;
            keymsg.option = MSG_OPTION_RECORD_PAUSE;
            led_type = LED_DISPLAY_FLASH;
            IP_printf("¼����ͣ��");
        }
        break;
    case KEY_REC_STOP:
        IP_printf("¼��ֹͣ��");
        if(Check_FunctionMode(Device_FunctionMode_LB) == FALSE) //add 2014.12.30
        {
            IP_printf("8168δ����,����ֹͣ");
            break;
        }
        KEY_DATA.key_rep &= ~KEY_REP_REC_PUSH;
        keymsg.device = MSG_DEV_LMT;
        keymsg.option = MSG_OPTION_RECORD_STOP;
        led_type = LED_DISPLAY_OFF;
        LED_Set(LED_REC_STOP, LED_DISPLAY_ON);
        if (Check_FunctionMode(Device_FunctionMode_LB) == TRUE)
        {
            if (KEY_DATA.key_rep & KEY_REP_YREC)
            {
                KEY_DATA.key_rep &= ~KEY_REP_YREC;
                LED_Set(LED_KZB, LED_DISPLAY_OFF);
                keymsg.device = MSG_DEV_MAINBARD;
                keymsg.option = MSG_OPTION_YREC_Stop;
                IP_printf("Ԥ¼��ֹͣ");
            }
        }
        break;//*
    case KEY_DEVICE_MUTE	://21//�豸����
        IP_printf("�豸����");
        keymsg.device = MSG_DEV_GF;
        if ((KEY_DATA.key_rep & KEY_REP_DEVICE_MUTE) == KEY_REP_DEVICE_MUTE)
        {
            KEY_DATA.key_rep &= ~KEY_REP_DEVICE_MUTE;
            keymsg.option = MSG_OPTION_MUTECLOSE;
            led_type = LED_DISPLAY_OFF;
        }
        else
        {
            KEY_DATA.key_rep |= KEY_REP_DEVICE_MUTE;
            keymsg.option = MSG_OPTION_MUTEOPEN;
            led_type = LED_DISPLAY_FLASH;
        }
        break;
    case KEY_MODE_SELECT://22 //ģʽѡ��
        if ((gDeviceStatus.CLASS_state & DEV_CLASS_RECSTART) ||
                (gDeviceStatus.CLASS_state & DEV_CLASS_RECPUSH))
        {
            IP_printf(" ¼�ƿ�ʼ��ģʽѡ�����Ч");
            return;
        }
        IP_printf("ģʽѡ���");
        keymsg.device = MSG_DEV_MAINBARD;
        if (KEY_DATA.MODE == MSG_OPTION_AUTO_ON)
        {
            KEY_DATA.MODE = MSG_OPTION_AUTO_OFF;
            keymsg.option = MSG_OPTION_AUTO_OFF;
        }
        else
        {
            KEY_DATA.MODE = MSG_OPTION_AUTO_ON;
            keymsg.option = MSG_OPTION_AUTO_ON;
        }
        break;
    case KEY_METERIC_SELECT://0x0217//23//¼�Ʒֱ���
        if ((gDeviceStatus.CLASS_state & DEV_CLASS_RECSTART) ||
                (gDeviceStatus.CLASS_state & DEV_CLASS_RECPUSH))
        {
            IP_printf(" ¼�ƿ�ʼ��¼�Ʒֱ��ʼ���Ч");
            return;
        }
        IP_printf("¼�Ʒֱ��ʼ�");
        keymsg.device = MSG_DEV_LMT;
        if (KEY_DATA.METRIC == MSG_OPTION_METRIC1)
        {
            LED_Set(LED_METRIC_XGA, LED_DISPLAY_OFF);
            KEY_DATA.METRIC = MSG_OPTION_METRIC2;
            keymsg.option = MSG_OPTION_METRIC2;
        }
        else if (KEY_DATA.METRIC == MSG_OPTION_METRIC2)
        {
            LED_Set(LED_METRIC_D1, LED_DISPLAY_OFF);
            KEY_DATA.METRIC = MSG_OPTION_METRIC3;
            keymsg.option = MSG_OPTION_METRIC3;
        }
        else
        {
            LED_Set(LED_METRIC_ZDY, LED_DISPLAY_OFF);
            KEY_DATA.METRIC = MSG_OPTION_METRIC1;
            keymsg.option = MSG_OPTION_METRIC1;
        }
        break;
    case KEY_FRAME_SELECT://0x0218//24//������Ч

        if ((gDeviceStatus.CLASS_state & DEV_CLASS_RECSTART) ||
                (gDeviceStatus.CLASS_state & DEV_CLASS_RECPUSH))
        {
            IP_printf(" ¼�ƿ�ʼ��������Ч����Ч");
            return;
        }
        IP_printf("������Ч��");
        keymsg.device = MSG_DEV_LMT;
        if (KEY_DATA.FRAME == MSG_OPTION_FRAME1)
        {
            LED_Set(LED_FRAME_CG, LED_DISPLAY_OFF);
            KEY_DATA.FRAME = keymsg.option = MSG_OPTION_FRAME2;
        }
        else if (KEY_DATA.FRAME == MSG_OPTION_FRAME2)
        {
            LED_Set(LED_FRAME_HZH, LED_DISPLAY_OFF);
            KEY_DATA.FRAME = keymsg.option = MSG_OPTION_FRAME1;
        }
        else
        {
            LED_Set(LED_FRAME_ZDY, LED_DISPLAY_OFF);
            KEY_DATA.FRAME = keymsg.option = MSG_OPTION_FRAME1;
        }
        break;
    case KEY_SPEED_SELECT://0x0219//25//¼������

        if ((gDeviceStatus.CLASS_state & DEV_CLASS_RECSTART) ||
                (gDeviceStatus.CLASS_state & DEV_CLASS_RECPUSH))
        {
            IP_printf(" ¼�ƿ�ʼ��¼�����ʼ���Ч");
            return;
        }
        IP_printf("¼�����ʼ�");
        keymsg.device = MSG_DEV_LMT;
        if (KEY_DATA.CODE1_SPEED == MSG_OPTION_CODE1_SPEED1)
        {
            LED_Set(LED_SPEED_400, LED_DISPLAY_OFF);
            keymsg.option = MSG_OPTION_CODE1_SPEED2;
            KEY_DATA.CODE1_SPEED = MSG_OPTION_CODE1_SPEED2;
        }
        else if (KEY_DATA.CODE1_SPEED == MSG_OPTION_CODE1_SPEED2)
        {
            LED_Set(LED_SPEED_1M, LED_DISPLAY_OFF);
            keymsg.option = MSG_OPTION_CODE1_SPEED3;
            KEY_DATA.CODE1_SPEED = MSG_OPTION_CODE1_SPEED3;
        }
        else
        {
            LED_Set(LED_SPEED_ZDY, LED_DISPLAY_OFF);
            keymsg.option = MSG_OPTION_CODE1_SPEED1;
            KEY_DATA.CODE1_SPEED = MSG_OPTION_CODE1_SPEED1;
        }
        break;
    case  KEY_TAIL_SELECT://0x021a//26//����趨
        IP_printf("����趨��");
        keymsg.device = MSG_DEV_TRACK;
        if (KEY_DATA.PERSONAL_TALL == MSG_OPTION_PERSONAL_TALL1)
        {
            LED_Set(LED_TAIL_160, LED_DISPLAY_OFF);
            keymsg.option = KEY_DATA.PERSONAL_TALL = MSG_OPTION_PERSONAL_TALL2;
        }
        else if (KEY_DATA.PERSONAL_TALL == MSG_OPTION_PERSONAL_TALL2)
        {
            LED_Set(LED_TAIL_170, LED_DISPLAY_OFF);
            keymsg.option = KEY_DATA.PERSONAL_TALL = MSG_OPTION_PERSONAL_TALL3;
        }
        else
        {
            LED_Set(LED_TAIL_180, LED_DISPLAY_OFF);
            keymsg.option = KEY_DATA.PERSONAL_TALL = MSG_OPTION_PERSONAL_TALL1;
        }
        break;//*/
    case KEY_SCAMERA_SELECT://0x021b//27 //������趨
        IP_printf("������趨��");
        keymsg.device = MSG_DEV_TRACK;
        LED_Set(LED_SCAMERA1, LED_DISPLAY_OFF);
        LED_Set(LED_SCAMERA2, LED_DISPLAY_OFF);
        LED_Set(LED_SCAMERA3, LED_DISPLAY_OFF);
        LED_Set(LED_SCAMERA4, LED_DISPLAY_OFF);
        LED_Set(LED_SCAMERA5, LED_DISPLAY_OFF);
        LED_Set(LED_SCAMERA6, LED_DISPLAY_OFF);
        if (KEY_DATA.CURR_SCAMERA == MSG_OPTION_SELECT_SCAMERA1)
        {
            keymsg.option = KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA2;
        }
        else if (KEY_DATA.CURR_SCAMERA == MSG_OPTION_SELECT_SCAMERA2)
        {
            keymsg.option = KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA3;
        }
        else if (KEY_DATA.CURR_SCAMERA == MSG_OPTION_SELECT_SCAMERA3)
        {
            keymsg.option = KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA4;
        }
        else if (KEY_DATA.CURR_SCAMERA == MSG_OPTION_SELECT_SCAMERA4)
        {
            keymsg.option = KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA5;
        }
        else if (KEY_DATA.CURR_SCAMERA == MSG_OPTION_SELECT_SCAMERA5)
        {
            keymsg.option = KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA6;
        }
        else
        {
            keymsg.option = KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA1;
        }
        break;
    case KEY_SCAMERA_ZOOM1://0x021c//28//Zoom+
        IP_printf("���������-Zoom+��");
        keymsg.device = MSG_DEV_TRACK;
        keymsg.option = MSG_OPTION_SCAMERA_ZOOMADD;
        break;
    case KEY_SCAMERA_UP://0x021d//29//������
        IP_printf("���������-�ϼ�");
        keymsg.device = MSG_DEV_TRACK;
        keymsg.option = MSG_OPTION_SCAMERA_UP;
        break;
    case KEY_SCAMERA_ZOOM2://0x021e//30//ZOOM-
        IP_printf("���������-Zoom��");
        keymsg.device = MSG_DEV_TRACK;
        keymsg.option = MSG_OPTION_SCAMERA_ZOOMSUB;
        break;
    case KEY_SCAMERA_LEFT://0x021f//31//������
        IP_printf("���������-���");
        keymsg.device = MSG_DEV_TRACK;
        keymsg.option = MSG_OPTION_SCAMERA_LIFT;
        break;
    case KEY_SCAMERA_DOWN://0x0220//32//������
        IP_printf("���������-�¼�");
        keymsg.device = MSG_DEV_TRACK;
        keymsg.option = MSG_OPTION_SCAMERA_DOWN;
        break;
    case KEY_SCAMERA_RIGHT://0x0221//33//������
        IP_printf("���������-�Ҽ�");
        keymsg.device = MSG_DEV_TRACK;
        keymsg.option = MSG_OPTION_SCAMERA_RIGHT;
        break;
    case KEY_SCAMERA_STOP://0x0221//33//������
        keymsg.device = MSG_DEV_TRACK;
        keymsg.option = MSG_OPTION_SCAMERA_STOP;
        break;
    //*
    case KEY_CONTRL_BACK://0x0122//34//����
        IP_printf("���ſ��Ʒ��ؼ�");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_RETURN;
        break;
    case KEY_CONTRL_UP://0x0123//35//������
        IP_printf("���ſ��ƿ����ϼ�");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_UP;
        break;
    case KEY_CONTRL_ENTER://0x0124//36//ȷ��
        IP_printf("���ſ���ȷ�ϼ�");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_ENTER;
        break;
    case KEY_CONTRL_LEFT://0x0125//37//������
        IP_printf("���ſ��ƿ������");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_LIFT;
        break;
    case KEY_CONTRL_DOWN://0x0126//38//������
        IP_printf("���ſ��ƿ����¼�");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_DOWN;
        break;
    case KEY_CONTRL_RIGHT://0x0127//39//������
        IP_printf("���ſ��ƿ����Ҽ�");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_RIGHT;
        break;
    case KEY_CONTRL_SOUND://0x0128//40//����
        IP_printf("���ſ���������");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_TRACK;
        break;
    case KEY_CONTRL_TITLE://0x0129//41//��Ļ
        IP_printf("���ſ�����Ļ��");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_TITLE;
        break;
    case KEY_CONTRL_YY://0x012a//42//����
        IP_printf("���ſ������Լ�");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_LANGUAGE;
        break;
    case KEY_CONTRL_PREV://0x012b//43//��һ��
        IP_printf("���ſ�����һ����");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_PRE;
        break;
    case KEY_CONTRL_QBACK://0x012c//44//����
        IP_printf("���ſ��ƿ��˼�");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_BACK;
        break;
    case KEY_CONTRL_PLAY://0x012d//45//����/��ͣ
        IP_printf("���ſ��Ʋ���/��ͣ��");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        if ((KEY_DATA.key_rep & KEY_REP_PLAY_PUSH) == KEY_REP_PLAY_PUSH)
        {
            KEY_DATA.key_rep &= ~KEY_REP_PLAY_PUSH;
            keymsg.option = MSG_OPTION_PLAY;
        }
        else
        {
            KEY_DATA.key_rep |= KEY_REP_PLAY_PUSH;
            keymsg.option = MSG_OPTION_PUSE;
        }
        break;
    case KEY_CONTRL_QUIT://	0x012e//46//���
        IP_printf("���ſ��ƿ����");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_GO;
        break;
    case KEY_CONTRL_NEXT://	0x012f//47//��һ��
        IP_printf("���ſ�����һ����");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_NEXT;
        break;

    //��������̶���ļ�ֵ
    //*/
    case KEY_SELECT_TYM:
        IP_printf("ͶӰĻѡ��");
        LED_Set(LED_TYM, LED_DISPLAY_ON);
        LED_Set(LED_LCD_UP, LED_DISPLAY_OFF);
        LED_Set(LED_TYDJ, LED_DISPLAY_OFF);
        LED_Set(LED_PELMET_POWER, LED_DISPLAY_OFF);
        KEY_DATA.POWER_DEV = MSG_DEV_TYM;
        keymsg.device = MSG_DEV_TYM;
        keymsg.option = 0xff;
        gDeviceStatus.Curr_Contrl_Device = KEY_DATA.POWER_DEV;
        break;

    case KEY_SELECT_TYJUP:
        IP_printf("ͶӰ������ѡ��");
        LED_Set(LED_TYM, LED_DISPLAY_OFF);
        LED_Set(LED_LCD_UP, LED_DISPLAY_OFF);
        LED_Set(LED_TYDJ, LED_DISPLAY_ON);
        LED_Set(LED_PELMET_POWER, LED_DISPLAY_OFF);
        KEY_DATA.POWER_DEV = MSG_DEV_TYJUP;
        keymsg.device = MSG_DEV_TYJUP;
        keymsg.option = 0xff;
        gDeviceStatus.Curr_Contrl_Device = KEY_DATA.POWER_DEV;
        break;
    case KEY_SELECT_LCDUP:
        IP_printf("Һ��������ѡ��");
        LED_Set(LED_TYM, LED_DISPLAY_OFF);
        LED_Set(LED_LCD_UP, LED_DISPLAY_ON);
        LED_Set(LED_TYDJ, LED_DISPLAY_OFF);
        LED_Set(LED_PELMET_POWER, LED_DISPLAY_OFF);
        KEY_DATA.POWER_DEV = MSG_DEV_LCD;
        keymsg.device = MSG_DEV_LCD;
        keymsg.option = 0xff;
        gDeviceStatus.Curr_Contrl_Device = KEY_DATA.POWER_DEV;
        break;
    case KEY_SELECT_PELMET:
        IP_printf("�綯����ѡ��");
        LED_Set(LED_TYM, LED_DISPLAY_OFF);
        LED_Set(LED_LCD_UP, LED_DISPLAY_OFF);
        LED_Set(LED_TYDJ, LED_DISPLAY_OFF);
        LED_Set(LED_PELMET_POWER, LED_DISPLAY_ON);
        KEY_DATA.POWER_DEV = MSG_DEV_PELMET;
        keymsg.device = MSG_DEV_PELMET;
        keymsg.option = 0xff;
        gDeviceStatus.Curr_Contrl_Device = KEY_DATA.POWER_DEV;
        break;
    //*
    //�����嶨��Ĳ��ſ��Ƽ�
    case KEY_CONTRL_STOP:
        IP_printf("����ֹͣ��");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_STOP;
        break;
    case KEY_CONTRL_EJC:
        IP_printf("���п��Ƽ�");
        keymsg.device = KEY_DATA.CURR_DEVICE;
        keymsg.option = MSG_OPTION_OPEN;
        break;
    //�����嶨��ı��ü�
    case KEY_REC_BAK:
        IP_printf("¼�Ʊ��ü�");
        break;
    case KEY_DEV_BAK:
        IP_printf("�豸���ü�");
        break;//*/
    default:
        IP_printf("δ����˰���");
    }

    if (keymsg.device != 0)
    {
        LED_device_set(&keymsg, led_type);
        LED_SendFLASH();
    }
    memset(buf, 0, 5);
    memcpy(buf, (uint8 *)&keymsg, 2);
    if ( (keymsg.device == MSG_DEV_TYM) || (keymsg.device == MSG_DEV_PELMET) ||
            (keymsg.device == MSG_DEV_TYJUP) || (keymsg.device == MSG_DEV_LCD) )
        if 	(keymsg.option != MSG_OPTION_POWERCLOSE)
        {
            Send_State();
            goto kl1;
        }

    if ((keymsg.device == MSG_DEV_MAINBARD) && (keymsg.option == MSG_OPTION_CLASS_BEGIN))
    {
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        buf[2] = 1;
    }
    if ((keymsg.device == MSG_DEV_MAINBARD) && (keymsg.option == MSG_OPTION_CLASS_OVER))
    {
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_OVER;
        buf[2] = 0;
    }
    Msg_send(buf, 3, (uint8 *)&ReV);
    IP_printf("����:%x�豸��:%d������:%x\n\n\n", key, keymsg.device, keymsg.option);

kl1:
    if (ps->DEV_Link_State & DEV_LINK_BJCardLK)
    {
        //ip = Get_DesIp(DEVICE_SUB_SER); //add 
        ip = Get_DesIp(DEVICE_SUB_PC); //add 
        IP_printf("PC_IP:%d.%d.%d.%d",*ip,*(ip+1),*(ip+2),*(ip+3));
        if (ARP_Rec_Find(ip) == NULL)
        {
            Arp_Request(ip);
            return ;
        }
        K8_UDP_Send_CMD(buf , 2, ip);

        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("��༭��������Ϣ���豸��:%2x������:%2x", keymsg.device, keymsg.option);
        ip = Get_DesIp(DEVICE_SUB_BJCard);
        if (ARP_Rec_Find(ip) == NULL)
        {
            Arp_Request(ip);
            return ;
        }
        UDP_Send_CMD(buf, 2, ip);

    }
    return;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ����̴����������
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void key_msg_rev(uint8 *pdata)
{
    uint8 buf[5];
    struct DEVICE_STATUS_TYPE *ps = NULL;

    //IP_printf("%x %x %x %x %x \n ",*pdata,*(pdata+1),*(pdata+2),*(pdata+3),*(pdata+4),*(pdata+5));
    switch(*pdata)
    {
    case LED_CHANLE_VER:
        ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
        ps->KB_ver = *(pdata + 1) * 256 + *(pdata + 2);
        if (*(pdata + 2) >= 0x80)
            ps->KB_Type = KB_TYPE_KUREI;
        else
            ps->KB_Type = KB_TYPE_RONGYAO;
        break;
    case LED_CHANLE_DATA:
    case LED_CHANLE_TYPE:
        LED_SendFLASH();
        break;
    case KEY_SOUND_VOL:
        buf[0] = MSG_DEV_GF;
        buf[1] = MSG_OPTION_SOUND_VOL;
        buf[2] = *(pdata + 1);
        buf[3] = DEV_XCOM_MB;
        Msg_send(buf, 3, &buf[3]);
        break;
    case KEY_VAL:
        KEY_CMD(pdata);
        break;
    case KEY_CONTRL:
        break;
    }
}

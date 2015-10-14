#include "includes.h"

const char power_ostr[][10] =
{
    "���½�",
    "����",
    "�ر�ֹͣ",
};
const char power_str[][10] =
{
    "�豸",
    "��ý��",
    "չ̨",
    "��ʾ��",
    "�����",
    "ͶӰ��",
    "�ƹ�",
    "����",
    "�����",
    "ͶӰĻ",
    "����",
    "Һ��",
    "����",
};
extern struct KEY_TYPE KEY_DATA;
uint32 POWER_FLAG, POWER_FLAG_STATE;
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void POWER_init(void)
{
    uint8 buf[10], err, lg, time;
    IO_Out(OUT_POWER_M3, 0);
    IO_Out(OUT_POWER_M4, 0);
    for (time = 0; time < 10; time++)
    {
        buf[0] = 0;
        lg = 1;
        IO_FlagLingt_Flash();
        err = XCOM_Data_Read(DEV_XCOM_DYX, 0, XCOM_CMD_GET_INFO, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
        if (err != OS_NO_ERR)
        {
            //UART_printf("User Power Error  Times:%d\n",time);
            IP_printf("��Դ���Լ����");
        }
        else
        {
            //UART_printf("User Power OK Ver=%d.%d\n",buf[4],buf[5]);
            IP_printf("��Դ���Լ�ͨ�����汾:%d.%d", buf[4], buf[5]);
            Power_Beep(200);
            //			OSTimeDlyHMSM(0,0, 1,0);
            //			Power_Beep(200);
            break;
        }
        IO_FlagLingt_Flash();
    }
    POWER_FLAG = 0;
    POWER_FLAG_STATE = 0;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�������ת��ȫ��(Ԥ��λ1)
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void CameraInit( BYTE id)
{
    BYTE buf[18] = {0};

    BYTE devid, *pBuf;
    pBuf = (uint8 *) Get_ClassInfo_Table(0);
    devid = *(pBuf + 166);//��������豸ID

    if (id != devid)
        return;

    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_SCAMERA_PREPOSITION;
    buf[2] = 1;
    //buf[3]=option;
    buf[6] = buf[8] = 0xa6;
    buf[7] = buf[9] = 0xa5;
    buf[17] = DEV_XCOM_SELF;
    Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 30, &buf[0], 10, 0, &buf[17]);
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�POWER_Set
//	�������ܣ�����һ����߼���Դ��
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void POWER_Set(uint8 contrl_io, uint8 option)
{
    struct DEVICE_STATUS_TYPE *ps;
    uint8 buf[5], sub, port, err, lg;
    uint16 time;

    if (contrl_io == 0)
        return;
    //if(contrl_io==DEV_POWER_SXJ) contrl_io=DEV_POWER_TYM;//�п��������Դ��ΪͶӰĻ��Դ�����
    ps = ( struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    sub = contrl_io / 16;
    port = contrl_io % 16;

    if (ps->Power_Type == 2)
        time = DEV_XCOM_RPTIME;
    else
        time = 1;
    do
    {
        buf[0] = POWER_CHANLE_CONTRL;
        buf[1] = port;
        buf[2] = option;
        lg = 3;
        err = XCOM_Data_Read(DEV_XCOM_DYX, sub, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
        if (err != OS_NO_ERR)
        {
            //UART_printf("Power Error\n");
            OSTimeDlyHMSM(0, 0, 0, 1000);
        }
        else
            break;
    }
    while(time--);

    ps->power_state[sub] &= ~(3 << port);
    ps->power_state[sub] |= option << port;

    Send_State();
    if (Get_Debug(DEV_DEBUG_POWER))
        IP_printf("��Դ�����:%s�豸:%s���:%d", &power_ostr[option - 1][0], &power_str[contrl_io - 1][0], contrl_io - 1);

}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void SetPowerDeltimemsg(BYTE device, BYTE option, BYTE time, uint8 repeat)
{
    BYTE buf[4], rev_buf[10];
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_POWERCARD_ONOFF;
    buf[2] = device;
    buf[3] = option;
    rev_buf[0] = DEV_XCOM_SELF;
    Task_Run_Delay(REALTIME_CIIR_SEC * 256 + time, buf, 4, repeat, rev_buf);
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void DelPowerDeltimemsg(BYTE device, BYTE option, BYTE lg)
{
    BYTE buf[4];
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_POWERCARD_ONOFF;
    buf[2] = device;
    buf[3] = option;
    Task_Rec_Del(buf, lg);
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void IO_PC_Power(BYTE device, BYTE option)
{
    //BYTE buf[8];
    if (device == MSG_DEV_PC)
    {
        IO_Out(OUT_PC1POWER, 1);
        OSTimeDly(150);
        if  (Get_Debug(DEV_DEBUG_POWER))
            IP_printf("�μ����������");
        IO_Out(OUT_PC1POWER, 0);
    }
    else if (device == MSG_DEV_LMT)
    {
        IO_Out(OUT_PC2POWER, 1);
        OSTimeDly(150);
        if  (Get_Debug(DEV_DEBUG_POWER))
            IP_printf("��ý����������");
        IO_Out(OUT_PC2POWER, 0);
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
uint32 ON_POWER_option(void *pdata)
{
    uint8 *p, time, buf[10], rev_buf[12];
    struct DEVICE_INFO *pdev;
    struct BASE_MSG msg, *pmsg;
    struct DEVICE_STATUS_TYPE *ps;
    struct KEY_TYPE *pkey;

    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    msg.device = *p;
    msg.option = 0;
    time = *(p + 2);
    pmsg = (struct BASE_MSG *)p;
    pkey = (struct KEY_TYPE *)Get_KeyData();
    rev_buf[0] = DEV_XCOM_SELF;
    pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)&msg, DEV_FIND_TYPE);
    if (Get_Debug(DEV_DEBUG_POWER)) IP_printf("��Դ����;�豸:%d,����:%x", pmsg->device, pmsg->option);
    switch(pmsg->device)
    {
    case MSG_DEV_PC://�μ�PC
        if (POWER_FLAG_STATE & POWER_FLAG_PC)
            break;
        //zhw add 2009-09-07
        if  (pmsg->option == MSG_OPTION_POWEROPEN)
        {
            if  (!IO_in(IN_PC1POWER)) //����״̬
            {
                LED_Set(LED_PC_POWER, LED_DISPLAY_ON); //����led
                LED_SendFLASH();
                Key_TB_state(KEY_REP_PC_POWER, TRUE);
                ps->DEV_Link_State |= DEV_LINK_PCON;
                //�����ɹ�����ʼͳ��ʱ��
                Device_Stat_Option(MSG_DEV_PC, MSG_OPTION_STAT_START);
                return STATUS_CHANGE;
            }
        }
        else
        {
            if  (IO_in(IN_PC1POWER)) //�ػ�״̬
            {
                LED_Set(LED_PC_POWER, LED_DISPLAY_OFF); //����led
                LED_SendFLASH();

                Key_TB_state(KEY_REP_PC_POWER, FALSE);
                ps->DEV_Link_State &= ~DEV_LINK_PCON;
                //�ػ��ɹ�����ʼͳ��ʹ��ʱ��
                Device_Stat_Option(MSG_DEV_PC, MSG_OPTION_STAT_END);
                return STATUS_CHANGE;
            }
        }
        //zhw add end
        POWER_FLAG_STATE |= POWER_FLAG_PC;
        LED_Set(LED_PC_POWER, LED_DISPLAY_FLASH);
        if (pmsg->option == MSG_OPTION_POWEROPEN)
        {
            POWER_Set(DEV_POWER_PC, pmsg->option);
            POWER_Set(DEV_POWER_CRT, pmsg->option);
            time = 10;
        }
        else
        {
            time = DEV_TIME_PC;
        }
        IO_PC_Power(pmsg->device, pmsg->option);
        SetPowerDeltimemsg(pmsg->device, pmsg->option, time, DEV_REPEAT_TIMES_PC);
        break;
    case MSG_DEV_LMT://��ý��PC
        POWER_FLAG_STATE |= POWER_FLAG_LMT;
        LMT_Power_contrl(MSG_DEV_LMT, pmsg->option);
        time = 1;
        SetPowerDeltimemsg(pmsg->device, pmsg->option, time, DEV_REPEAT_TIMES_LMTPC);
        break;
    case MSG_DEV_TYJA://ͶӰ��A
        if (POWER_FLAG_STATE & POWER_FLAG_TYJA)
            break;
        POWER_FLAG_STATE |= POWER_FLAG_TYJA;
        goto loop2;
    case MSG_DEV_TYJB://ͶӰ��B
        if (POWER_FLAG_STATE & POWER_FLAG_TYJB)
            break;
        POWER_FLAG_STATE |= POWER_FLAG_TYJB;
        break;
loop2:
        LED_Set(LED_TYJ_POWER, LED_DISPLAY_FLASH);
        buf[0] = MSG_DEV_TYM;
        buf[1] = 0;
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            //����ͶӰ���ػ�
            if (Get_Debug(DEV_DEBUG_POWER))
                IP_printf("device:%d(25 A,52 B),dev_id:%x,ͶӰ���ػ���ʱ %d S", pmsg->device, pdev->dev_id, pdev->OFF_TIME);
            time = pdev->OFF_TIME;
            if(time <= 10) time = 10;
            SetPowerDeltimemsg(pmsg->device, pmsg->option, time, 0);
            pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)buf, DEV_FIND_TYPE);
            if (pdev != NULL)
            {
                if (pdev->prev_val)
                {
                    buf[0] = MSG_DEV_TYM;
                    buf[1] = MSG_OPTION_POWEROPEN;
                    buf[2] = 0;
                    Msg_send(buf, 2, rev_buf);
                }
            }
        }
        else
        {
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
            else
                POWER_Set(DEV_POWER_TYJ, pmsg->option);
            SetPowerDeltimemsg(pmsg->device, pmsg->option, 10, 0);
            pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)buf, DEV_FIND_TYPE);
            if (pdev != NULL)
            {
                if (pdev->prev_val)
                {
                    buf[0] = MSG_DEV_TYM;
                    buf[1] = MSG_OPTION_POWERDOWN;
                    Msg_send(buf, 2, rev_buf);
                }
            }
        }
        break;
    case MSG_DEV_GF://����
        if (pmsg->option == MSG_OPTION_POWEROPEN)
        {
            LED_Set(LED_GF_POWER, LED_DISPLAY_ON);
            ps->DEV_Link_State |= DEV_LINK_GFON;
            KEY_DATA.key_rep |= KEY_REP_GF_POWER;
            Key_TB_state(KEY_REP_GF_POWER, TRUE);
        }
        else
        {
            LED_Set(LED_GF_POWER, LED_DISPLAY_OFF);
            ps->DEV_Link_State &= ~DEV_LINK_GFON;
        }
        //�رչ�������
        if (pdev != NULL)
            POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        else
            POWER_Set(DEV_POWER_GF, pmsg->option);

        Device_Stat_Option(MSG_DEV_GF, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_GF, MSG_OPTION_STAT_END);
        break;
    case MSG_DEV_LIGHT://���ڵƹ�
        if (pmsg->option == MSG_OPTION_POWEROPEN)
        {
            LED_Set(LED_LIGHT_POWER, LED_DISPLAY_ON);
            ps->DEV_Link_State |= DEV_LINK_LIGHT;

            //zjj add6
            KEY_DATA.key_rep |= KEY_REP_LIGHT_POWER;
            Key_TB_state(KEY_REP_LIGHT_POWER, TRUE);
        }
        else
        {
            LED_Set(LED_LIGHT_POWER, LED_DISPLAY_OFF);
            ps->DEV_Link_State &= ~DEV_LINK_LIGHT;
        }
        if (pdev != NULL)
            POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        else
            POWER_Set(DEV_POWER_LIGHT, pmsg->option);

        Device_Stat_Option(MSG_DEV_LIGHT, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_LIGHT, MSG_OPTION_STAT_END);
        break;
    case MSG_DEV_TYM://ͶӰĻ
        DelPowerDeltimemsg(MSG_DEV_TYM, 0, 3);
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            if (pkey->POWER_DEV == MSG_DEV_TYM)
                LED_Set(LED_TYM, LED_DISPLAY_ON);
            else
                LED_Set(LED_TYM, LED_DISPLAY_OFF);
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
            else
                POWER_Set(DEV_POWER_TYM, pmsg->option);
            break;
        }
        LED_Set(LED_TYM, LED_DISPLAY_FLASH);
        if (pdev != NULL)
        {
            if ((pdev->OUT.out_no != DEV_IO_NONE) && (pdev->POWER.power_no == DEV_POWER_NONE))
            {
                if (pmsg->option == MSG_OPTION_POWERDOWN)
                    IO_Contrl(pdev->OUT.out_no, TRUE);
                else
                    IO_Contrl(pdev->OUT.out_no, FALSE);
            }
            else
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        }
        else
            POWER_Set(DEV_POWER_TYM, pmsg->option);
        SetPowerDeltimemsg(pmsg->device, pmsg->option, 10, 0);
        break;
    case MSG_DEV_PELMET://����
        DelPowerDeltimemsg(MSG_DEV_PELMET, 0, 3);
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            if (pkey->POWER_DEV == MSG_DEV_PELMET)
                LED_Set(LED_PELMET_POWER, LED_DISPLAY_ON);
            else
                LED_Set(LED_PELMET_POWER, LED_DISPLAY_OFF);
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, MSG_OPTION_POWERCLOSE);
            else
                POWER_Set(DEV_POWER_CL, MSG_OPTION_POWERCLOSE);
            break;
        }
        LED_Set(LED_PELMET_POWER, LED_DISPLAY_FLASH);
        if (pdev != NULL)
        {
            if ((pdev->OUT.out_no != DEV_IO_NONE) && (pdev->POWER.power_no == DEV_POWER_NONE))
            {
                if (pmsg->option == MSG_OPTION_POWERDOWN)
                    IO_Contrl(pdev->OUT.out_no, TRUE);
                else
                    IO_Contrl(pdev->OUT.out_no, FALSE);
            }
            else
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        }
        else
            POWER_Set(DEV_POWER_CL, pmsg->option);
        SetPowerDeltimemsg(pmsg->device, pmsg->option, 10, 0);
        break;
    case MSG_DEV_LCD://Һ������
        DelPowerDeltimemsg(MSG_DEV_LCD, 0, 3);
        LED_Set(LED_LCD_UP, LED_DISPLAY_FLASH);
        if (pdev != NULL)
        {
            if ((pdev->OUT.out_no != DEV_IO_NONE) && (pdev->POWER.power_no == DEV_POWER_NONE))
            {
                if (pmsg->option == MSG_OPTION_POWERDOWN)
                    IO_Contrl(pdev->OUT.out_no, TRUE);
                else
                    IO_Contrl(pdev->OUT.out_no, FALSE);
            }
            else
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        }
        else
            POWER_Set(DEV_POWER_LCD, pmsg->option);
        SetPowerDeltimemsg(pmsg->device, pmsg->option, 10, 0);
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            if (pkey->POWER_DEV == MSG_DEV_LCD)
                LED_Set(LED_LCD_UP, LED_DISPLAY_ON);
            else
                LED_Set(LED_LCD_UP, LED_DISPLAY_OFF);

            if (pdev != NULL)
            {
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, MSG_OPTION_POWERCLOSE);
            }
            else
                POWER_Set(DEV_POWER_LCD, MSG_OPTION_POWERCLOSE);
            break;
        }
        break;
    case MSG_DEV_TYJUP://ͶӰ������
        DelPowerDeltimemsg(MSG_DEV_TYJUP, 0, 3);
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            LED_Set(LED_TYDJ, LED_DISPLAY_ON);
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, MSG_OPTION_POWERCLOSE);
            else
                POWER_Set(DEV_POWER_TYJUP_DOWN, MSG_OPTION_POWERCLOSE);
            break;
        }
        LED_Set(LED_TYDJ, LED_DISPLAY_FLASH);
        if (pdev != NULL)
        {
            if ((pdev->OUT.out_no != DEV_IO_NONE) && (pdev->POWER.power_no == DEV_POWER_NONE))
            {
                if (pmsg->option == MSG_OPTION_POWERDOWN)
                    IO_Contrl(pdev->OUT.out_no, TRUE);
                else
                    IO_Contrl(pdev->OUT.out_no, FALSE);
            }
            else
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        }
        else
            POWER_Set(DEV_POWER_TYJUP_DOWN, pmsg->option);
        SetPowerDeltimemsg(pmsg->device, pmsg->option, 10, 0);
        break;
    case MSG_DEV_SCAMERA1://���������1  �������Դ��ʹ��ͶӰ��Ļ���½���
    case MSG_DEV_SCAMERA2://���������2
    case MSG_DEV_SCAMERA3://���������3
    case MSG_DEV_SQCAMERA://ѧ�����
    case MSG_DEV_TQCAMERA://��ʦ���
    case MSG_DEV_BQCAMERA://�������
        if (pmsg->option == MSG_OPTION_POWEROPEN)
            Device_Stat_Option(pmsg->device, MSG_OPTION_STAT_START);
        else
            Device_Stat_Option(pmsg->device, MSG_OPTION_STAT_END);
        CameraInit(pmsg->device);
        if (pdev != NULL)
            POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        else
            POWER_Set(DEV_POWER_SXJ, pmsg->option);
        break;

    case MSG_DEV_DCS://�����
        Device_DCS_open();
        break;
    default:
        if (pmsg->option == MSG_OPTION_POWEROPEN)
            Device_Stat_Option(pmsg->device, MSG_OPTION_STAT_START);
        else
            Device_Stat_Option(pmsg->device, MSG_OPTION_STAT_END);

        if (pdev != NULL)
            POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
        else
            POWER_Set(DEV_POWER_DEVICE, pmsg->option);
        break;
    }
    LED_SendFLASH();
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
uint32 On_MSG_OPTION_POWERCARD_ONOFF(void *pdata)
{
    uint8 tmp, buf[20];
    struct DEVICE_STATUS_TYPE *ps;
    struct BASE_MSG *pmsg;
    struct DEVICE_INFO *pdev;
    struct KEY_TYPE *pkey;

    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    pkey = (struct KEY_TYPE *)Get_KeyData();
    pmsg = (struct BASE_MSG *)((BYTE *)pdata + sizeof(struct BASE_MSG) + sizeof(struct MSG_REV_TYPE));
    pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg, DEV_FIND_TYPE);
    switch(pmsg->device)
    {
    case MSG_DEV_PC://�μ�PC
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            Device_Stat_Option(MSG_DEV_PC, MSG_OPTION_STAT_END);
            if (!IO_in(IN_PC1POWER))
            {
                //�ر�û�����,�ظ��ػ�
                IO_PC_Power(pmsg->device, pmsg->option);
                if (0xff != Task_Rec_Find((BYTE *)pdata + sizeof(struct MSG_REV_TYPE), 4))return FALSE;
            }
            LED_Set(LED_PC_POWER, LED_DISPLAY_OFF); //����led
            pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg, DEV_FIND_TYPE);
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
            else
                POWER_Set(DEV_POWER_PC, pmsg->option);

            POWER_Set(DEV_POWER_CRT, pmsg->option);
            Key_TB_state(KEY_REP_PC_POWER, FALSE);
            ps->DEV_Link_State &= ~DEV_LINK_PCON;
            Device_Stat_Option(MSG_DEV_PC, MSG_OPTION_STAT_END);
        }
        else
        {
            if (IO_in(IN_PC1POWER))
            {
                //����û����ɣ��ظ�����
                IO_PC_Power(pmsg->device, pmsg->option);
                tmp = Task_Rec_Find((BYTE *)pdata + sizeof(struct MSG_REV_TYPE), 4);
                if (tmp != 0xff)
                {
                    if (Get_Debug(DEV_DEBUG_POWER))
                        IP_printf("�μ����������ʧ��!����:%d��", tmp);

                    return FALSE;
                }
                LED_Set(LED_PC_POWER, LED_DISPLAY_OFF); //����led
                Key_TB_state(KEY_REP_PC_POWER, FALSE);
                ps->DEV_Link_State &= ~DEV_LINK_PCON;
            }
            else
            {
                LED_Set(LED_PC_POWER, LED_DISPLAY_ON); //����led
                Key_TB_state(KEY_REP_PC_POWER, TRUE);
                ps->DEV_Link_State |= DEV_LINK_PCON;
                //�����ɹ�����ʼͳ��ʱ��
                Device_Stat_Option(MSG_DEV_PC, MSG_OPTION_STAT_START);
            }
        }
        DelPowerDeltimemsg(MSG_DEV_PC, 0, 3);
        POWER_FLAG_STATE &= ~POWER_FLAG_PC;
        break;
    case MSG_DEV_LMT://��ý��PC
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {

            /*
            pdev=(struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg,DEV_FIND_TYPE);
            if (pdev != NULL)
            	POWER_Set(pdev->POWER.dev_id*16+pdev->POWER.power_no, pmsg->option);
            else
            	POWER_Set(DEV_POWER_LMTPC,pmsg->option);
            if (!IO_in(IN_PC2POWER)){
            	IO_PC_Power(pmsg->device, pmsg->option);
            	if (0xff != Task_Rec_Find((BYTE *)pdata+sizeof(struct MSG_REV_TYPE),4))
            		return FALSE;
            }
            */
            ps->DEV_Link_State &= ~DEV_LINK_LMTON;
            Device_Stat_Option(MSG_DEV_LMT, MSG_OPTION_STAT_END);
        }
        else
        {
            /*
            	pdev=(struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg,DEV_FIND_TYPE);
            	if (pdev != NULL)
            		POWER_Set(pdev->POWER.dev_id*16+pdev->POWER.power_no, pmsg->option);
            	else
            		POWER_Set(DEV_POWER_LMTPC,pmsg->option);
            	LMT_Power_contrl(MSG_DEV_LMT,MSG_OPTION_POWEROPEN);
            	*/

            /*
            if (IO_in(IN_PC2POWER)){
            	tmp = Task_Rec_Find((BYTE *)pdata+sizeof(struct MSG_REV_TYPE),4);
            	if (tmp != 0xff){
            		if (Get_Debug(DEV_DEBUG_POWER))
            			IP_printf("��ý����������ʧ��!����:%d��",tmp);
            	 	IO_PC_Power(pmsg->device,pmsg->option);
            		return 0;;
            	}
            	else
            		ps->DEV_Link_State &= ~DEV_LINK_LMTON;
            }
            else{
            	ps->DEV_Link_State |= DEV_LINK_LMTON;
             	Device_Stat_Option(MSG_DEV_LMT, MSG_OPTION_STAT_START);
            }
            */
        }
        DelPowerDeltimemsg(MSG_DEV_LMT, 0, 3);
        POWER_FLAG_STATE &= ~POWER_FLAG_LMT;
        break;
    case MSG_DEV_TYJA://ͶӰ��A
        POWER_FLAG_STATE &= ~POWER_FLAG_TYJA;
        goto loop1;
    case MSG_DEV_TYJB://ͶӰ��B
        POWER_FLAG_STATE &= ~POWER_FLAG_TYJB;
loop1:
        if (pmsg->option == MSG_OPTION_POWERCLOSE)
        {
            LED_Set(LED_TYJ_POWER, LED_DISPLAY_OFF);//����led
            Key_TB_state(KEY_REP_TYJ_POWER, FALSE);
            pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg, DEV_FIND_TYPE);
            if (pdev != NULL)
                POWER_Set(pdev->POWER.dev_id * 16 + pdev->POWER.power_no, pmsg->option);
            else
                POWER_Set(DEV_POWER_TYJ, pmsg->option);
            ps->DEV_Link_State &= ~DEV_LINK_TYJON;
            Device_Stat_Option(MSG_DEV_TYJA, MSG_OPTION_STAT_END);
        }
        else
        {
            LED_Set(LED_TYJ_POWER, LED_DISPLAY_ON);//����led
            Device_Stat_Option(MSG_DEV_TYJA, MSG_OPTION_STAT_START);
            Key_TB_state(KEY_REP_TYJ_POWER, TRUE);
            if (0 == (ps->DEV_Link_State & DEV_LINK_TYJON))
            {
                //����ͶӰ��������
                ps->DEV_Link_State |= DEV_LINK_TYJON;
                tmp = sizeof(struct MSG_REV_TYPE);
                buf[tmp] = MSG_DEV_TYJA;
                buf[tmp + 1] = MSG_OPTION_POWEROPEN;
                //On_MSG_Power_OPTION((void *)buf);
                On_MSG_RepeatPower_OPTION((void *)buf);
                //buf[tmp]=MSG_DEV_TYJB;
                //buf[tmp+1]=MSG_OPTION_POWEROPEN;
                //On_MSG_Power_OPTION((void *)buf);
                //On_MSG_RepeatPower_OPTION((void *)buf);
            }
        }
        break;
    case MSG_DEV_TYM://ͶӰĻ
        if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
            LED_Set(LED_TYM, LED_DISPLAY_OFF); //����led
        else if (pkey->POWER_DEV == MSG_DEV_TYM)
            LED_Set(LED_TYM, LED_DISPLAY_ON); //����led
        else
            LED_Set(LED_TYM, LED_DISPLAY_OFF); //����led
        Device_Stat_Option(MSG_DEV_TYM, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_TYM, MSG_OPTION_STAT_END);
        break;
    case MSG_DEV_PELMET://����
        if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
            LED_Set(LED_PELMET_POWER, LED_DISPLAY_OFF); //����led
        else if (pkey->POWER_DEV == MSG_DEV_PELMET)
            LED_Set(LED_PELMET_POWER, LED_DISPLAY_ON); //����led
        else
            LED_Set(LED_PELMET_POWER, LED_DISPLAY_OFF); //����led
        Device_Stat_Option(MSG_DEV_PELMET, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_PELMET, MSG_OPTION_STAT_END);
        break;
    case MSG_DEV_TYJUP://ͶӰ������
        if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
            LED_Set(LED_TYDJ, LED_DISPLAY_OFF); //����led
        else if (pkey->POWER_DEV == MSG_DEV_TYJUP)
            LED_Set(LED_TYDJ, LED_DISPLAY_ON); //����led
        else
            LED_Set(LED_TYDJ, LED_DISPLAY_OFF); //����led
        Device_Stat_Option(MSG_DEV_TYJUP, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_TYJUP, MSG_OPTION_STAT_END);
        break;
    case MSG_DEV_LCD: //Һ������
        if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
            LED_Set(LED_LCD_UP, LED_DISPLAY_OFF); //����led
        else if (pkey->POWER_DEV == MSG_DEV_LCD)
            LED_Set(LED_LCD_UP, LED_DISPLAY_ON); //����led
        else
            LED_Set(LED_LCD_UP, LED_DISPLAY_OFF); //����led
        Device_Stat_Option(MSG_DEV_LCD, MSG_OPTION_STAT_START);
        Device_Stat_Option(MSG_DEV_LCD, MSG_OPTION_STAT_END);
        break;
    };
    LED_SendFLASH();
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
void POWER_msg_rev(uint8 *pdata, uint8 sub)
{
    uint8 chanle;
    struct DEVICE_STATUS_TYPE *ps;
    chanle = *pdata;

    if (chanle == POWER_CHANLE_VER)
    {
        ps = ( struct DEVICE_STATUS_TYPE *)Get_Device_Status();
        ps->power_state[sub] |= 0x80000000;
    }
    else if (chanle == POWER_CHANLE_CONTRL)
    {
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
void Power_Beep(uint16 DelayTime)
{

    uint8 buf[10], err, lg;
    buf[0] = POWER_CHANLE_SET;
    buf[1] = 0x03;
    lg = 2;
    err = XCOM_Data_Read(DEV_XCOM_DYX, 0, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
    if (err != OS_NO_ERR)
        UART_printf("Power  Contr Beep Error !\n");
    OSTimeDlyHMSM(0, 0, 0, DelayTime);
    buf[0] = POWER_CHANLE_SET;
    buf[1] = 0x1;
    err = XCOM_Data_Read(DEV_XCOM_DYX, 0, XCOM_CMD_SET_DATA, buf, &lg, XCOM_READ_FLAG_WAIT_ACK);
    if (err != OS_NO_ERR)
        UART_printf("Power  Contr Beep Error !\n");

}

#include "includes.h"
#include "PelcoD.h"
#include "User_DW.h"
extern struct LMT_CONTRL_TYPE  gLMT_Power;
extern struct LINK_REC_TABLE_TYPE DEVICE_LINK;
extern struct COMM_XCOM XCOM_PORT[DEV_XCOM_MAX_SIZE];
const uint8 Encode_TAB[8] =
{6, 5, 4, 3, 2, 1, 16, 15};
static uint8 ViscaCount = 0;
extern struct YXJ_TYPE gYXJ;
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����flag=1 Mute on
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 ON_Sound_Mute(void *pdata)
{
    struct BASE_MSG *pmsg;
    uint16 tmp;
    uint32 flag = 0;
    struct MSG_REV_TYPE *prev;

    prev = (struct MSG_REV_TYPE *)pdata;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    tmp = Device_Get_MIX();
    if (pmsg->option == MSG_OPTION_MUTEOPEN)
    {
        switch(pmsg->device)
        {
        case MSG_DEV_TMIC:
            Device_InVol( 0, DEV_SOUND_MIC1);
            LED_Contrl(LED_CONTRL_MIC_MUTE, TRUE);
            break;
        case MSG_DEV_SMIC:
            Device_Set_MIX(tmp | SMIX_CONTRLMIC2);
            break;
        case MSG_DEV_JMIC:
            Device_Set_MIX(tmp | SMIX_CONTRLMIC3);
            break;
        case MSG_DEV_GF:
            SoundVolumeControl(VOL_CH_GF, 0);
            gDeviceStatus.MK_Vol |= 0x8000;
            flag = 1;
            break;
        case MSG_DEV_DVD:
        case MSG_DEV_LYJ:
        case MSG_DEV_PC:
        case MSG_DEV_NPC:
        case MSG_DEV_KZA:
        case MSG_DEV_KZB:
            Device_Set_MIX(tmp | SMIX_DEVICE);
            break;
        }
    }
    else
    {
        switch(pmsg->device)
        {
        case MSG_DEV_TMIC:
            LED_Contrl(LED_CONTRL_MIC_MUTE, FALSE);
            Device_InVol( 98, DEV_SOUND_MIC1);
            break;
        case MSG_DEV_SMIC:
            Device_Set_MIX(tmp & (~SMIX_CONTRLMIC2));
            break;
        case MSG_DEV_JMIC:
            Device_Set_MIX(tmp & (~SMIX_CONTRLMIC3));
            break;
        case MSG_DEV_GF:
            SoundVolumeControl(VOL_CH_GF, DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol);
            gDeviceStatus.MK_Vol &= 0x7FFF;
            flag = 1;
            break;
        case MSG_DEV_DVD:
        case MSG_DEV_LYJ:
        case MSG_DEV_PC:
        case MSG_DEV_NPC:
        case MSG_DEV_KZA:
        case MSG_DEV_KZB:
            Device_Set_MIX(tmp & (~SMIX_DEVICE));
            break;
        }
    }
    prev = prev;
    return flag;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�void On_MSG_OPTION_MSG_RUN(void *pdata)
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_MSG_RUN(void *pdata)
{
    struct CLASS_OPTION_REC classmsg;
    uint8 *p, buf[20];
    struct MSG_REV_TYPE Rev;
    Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
    Rev.Type = DEV_XCOM_TABLE;
    if ((gDeviceTable.Class.Flag & CLASS_TASK_FLAG_RUN) != CLASS_TASK_FLAG_RUN)
        return 0;
    p = (BYTE *)Get_ClassInfo_Table(gDeviceTable.Class.Curr_id);
    p += sizeof(struct CLASS_OPEN_INFO) + gDeviceTable.Class.ClassCurrNUm * sizeof(struct CLASS_OPTION_REC);
    memcpy((BYTE *)&classmsg, p, sizeof(struct CLASS_OPTION_REC));

    if (gDeviceTable.Class.ClassCurrNUm < gDeviceTable.Class.ClassTaskNum)
    {
        if ((classmsg.MsgRec.option >= 16) || (classmsg.MsgRec.option <= 100))
        {
            buf[0] = classmsg.MsgRec.device;
            buf[1] = classmsg.MsgRec.option;
            if (classmsg.MsgRec.option == 0) classmsg.MsgRec.option = 1;
            buf[2] = classmsg.OptionTime & 0xff;
            //Task_Run_Delay(REALTIME_CIIR_SEC*256+buf[2], buf,2, 1,rev_buf);
            Msg_send(buf, 3, (uint8 *)&Rev);

            LED_Contrl(LED_CONTRL_TIME_OFF, gDeviceTable.Class.TIME_LED_STATE);
            if (gDeviceTable.Class.TIME_LED_STATE == FALSE)
                gDeviceTable.Class.TIME_LED_STATE = TRUE;
            else
                gDeviceTable.Class.TIME_LED_STATE = FALSE;
        }
        gDeviceTable.Class.ClassCurrNUm++;

    }
    else
    {
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_POWERCARD_ONOFF;
        if (Task_Rec_Find(buf, 2) != 0xff)
        {
            buf[0] = MSG_DEV_MAINBARD;
            buf[1] = MSG_OPTION_MSG_RUN;
            if (Task_Rec_Find(buf, 2) == 0xff)
                Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 1, buf, 2, 1, (uint8 *)&Rev);
            LED_Contrl(LED_CONTRL_TIME_OFF, gDeviceTable.Class.TIME_LED_STATE);
            if (gDeviceTable.Class.TIME_LED_STATE == FALSE)
                gDeviceTable.Class.TIME_LED_STATE = TRUE;
            else
                gDeviceTable.Class.TIME_LED_STATE = FALSE;
            return 0;
        }

        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_MSG_RUN;
        Task_Rec_Del(buf, 2);
        if (gDeviceTable.Class.Curr_id == 1)
        {
            //����
            gDeviceTable.Class.Flag &= ~CLASS_TASK_FLAG_OVER;
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("��������ִ����");
            LED_Set(LED_POWER, LED_DISPLAY_ON); //���������ʾ
            LED_SendFLASH();
            LED_Contrl(LED_CONTRL_TIME_OFF, FALSE);
            buf[0] = MSG_DEV_PC;
            buf[1] = MSG_OPTION_NONE;
            buf[2] = 0;
            Msg_send(buf, 2, (uint8 *)&Rev);
        }
        else if (gDeviceTable.Class.Curr_id == 2)
        {
            gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_OVER;
            LED_Set(0, LED_ALL_CLR);
            LED_SendFLASH();
            LED_Contrl(LED_CONTRL_TIME_OFF, TRUE);
            Key_TB_state(KEY_REP_CLASS, TRUE);
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("�ػ�����ִ����");
        }
        gDeviceTable.Class.Flag &= ~CLASS_TASK_FLAG_RUN;
    }
    return 1;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_POWER(void *pdata)
{
    uint8 port, option, *p;
    p = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE);
    port = *(p + sizeof(struct BASE_MSG));
    option = *(p + sizeof(struct BASE_MSG) + 1);
    POWER_Set(port,  option);

    return 0;
}
void On_MSG_Send_Delay_chk(BYTE time)
{
    BYTE buf[4], rev_buf[10]; //�ӳ�2s���ping����״̬
    buf[0] = MSG_DEV_MAINBARD;
    //buf[1]=MSG_OPTION_Chk_DeviceState;
    rev_buf[0] = DEV_XCOM_SELF;
    Task_Run_Delay(REALTIME_CIIR_SEC * 256 + time, buf, 4, 0, rev_buf);
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
extern struct gDevice_ADD Device_3730;
extern struct gDevice_ADD Device_8168;
extern BYTE Delay_Ping;
uint32 On_MSG_OPTION_CLASS_BEGIN(void *pdata)
{
    uint8 flag, buf[10], *ip; //,tmp;
    struct DEVICE_STATUS_TYPE *ps;
    struct MSG_REV_TYPE *prev;
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    prev = (struct MSG_REV_TYPE *)pdata;

    IO_Out(OUT_BJ_CTRL1, TRUE); //IP_printf("�򿪷���");
    if ((gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER) == 0)
        return 0;
    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_RUN)
        return 0;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    gDeviceTable.Class.Flag &= ~CLASS_TASK_FLAG_OVER;
    gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_RUN;
    LED_Contrl(LED_CONTRL_TIME_OFF, FALSE);
    LED_Contrl(LED_CONTRL_POWER_ON, TRUE);
    LED_Set(LED_POWER, LED_DISPLAY_ON); //
    gDeviceTable.Class.TIME_LED_STATE = FALSE;
    //LED_Set(LED_PC_POWER,LED_DISPLAY_FLASH);//��忪�ػ���
    flag = Class_Task_Start(1); //ִ���꿪������
    key_init();
    if (flag == CLASS_TASK_OK)
        IP_printf("ִ�п�������");
    //SetDeltimePingMsg(Device_3730.IP,Delay_Ping,0);
    //SetDeltimePingMsg(Device_8168.IP,Delay_Ping,0);
    //On_MSG_Send_Delay_chk(Delay_Ping+2);

    //�޸�״̬��
    switch(prev->Type)
    {
    case DEV_XCOM_MB://��Ϣ�������
        gDeviceStatus.DEV_state &= 0xf0;
        gDeviceStatus.DEV_state += CLASS_BEGIN_KEY;
        break;
    case DEV_XCOM_RF://�Ͽ���Ϣ������RF��
        gDeviceStatus.DEV_state &= 0xf0;
        gDeviceStatus.DEV_state += CLASS_BEGIN_RF;
        break;
    case DEV_XCOM_TIME:	//�Ͽ���Ϣ���Զ�ʱ�α�
        gDeviceStatus.DEV_state &= 0xf0;
        gDeviceStatus.DEV_state += CLASS_BEGIN_TIME;
        break;
    case DEV_XCOM_UDP://���Ժ�̨������
        gDeviceStatus.DEV_state &= 0xf0;
        gDeviceStatus.DEV_state += CLASS_BEGIN_UDPCMD;
        break;
    case DEV_XCOM_FINGERPRINT://ָ��ʶ���������
        gDeviceStatus.DEV_state &= 0xf0;
        gDeviceStatus.DEV_state += CLASS_BEGIN_FINGERPRINT;
        break;
    };
    //gDeviceStatus.CLASS_state|=1;
    gDeviceStatus.CLASS_state |= DEV_CLASS_START;
    ViscaCount = 0;
    //֪ͨ��ý��
    Key_TB_state(KEY_REP_CLASS, FALSE);
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_CLASS_BEGIN;
    if (ps->DEV_Link_State & DEV_LINK_BJCardLK)
    {
        ip = Get_DesIp(DEVICE_SUB_BJCard);
        if (ARP_Rec_Find(ip) == NULL)
        {
            Arp_Request(ip);
            goto nclass;
            //return 1;
        }
        else
            UDP_Send_CMD(buf, 2, ip);
        buf[0] = MSG_DEV_PC;
        buf[1] = MSG_OPTION_NONE;
        UDP_Send_CMD(buf, 2, ip);
    }
nclass:
    //֪ͨ����
    ip = Get_DesIp(DEVICE_SUB_SER);
    if (ARP_Rec_Find(ip) == NULL)
    {
        Arp_Request(ip);
        return 1;
    }
    else
        UDP_Send_CMD(buf, 2, ip);
    return 1;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_CLASS_OVER(void *pdata)
{
    uint8 flag, buf[10] , *ip;
    struct DEVICE_STATUS_TYPE *ps;
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    //prev=(struct MSG_REV_TYPE * )pdata;

    if ((gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER) == CLASS_TASK_FLAG_OVER)
        return 0;
    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_RUN)
        return 0;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();

    IO_Out(OUT_BJ_CTRL1, FALSE); //IP_printf("�رշ���");
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("ִ�йػ�����");
    //DWQ_Contrl(0);//�򿪳�������λ��
    gDeviceTable.Class.TIME_LED_TYPE = 1;
    LED_Contrl(LED_CONTRL_TIME_OFF, TRUE);
    gDeviceTable.Class.TIME_LED_STATE = TRUE;
    gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_OVER;
    gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_RUN;
    flag = Class_Task_Start(2);
    //IP_printf("flag=%x()",flag);
    switch(flag)
    {
    case CLASS_TASK_OK:
        break;
    case CLASS_TASK_ERR_ID:
        break;
    case CLASS_TASK_EMPTY:
        LED_Set(0, LED_ALL_CLR);
        gDeviceTable.Class.Flag |= CLASS_TASK_FLAG_OVER;
        //�ر�ͳ��
        Device_Stat_Save();
        //�رռ�����ʾ
        LED_Set(0, LED_ALL_CLR);
        LED_SendFLASH();
        LED_Contrl(LED_CONTRL_TIME_OFF, TRUE);
        Key_TB_state(KEY_REP_CLASS, TRUE);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�ػ�����ִ�����");
        //DWQ_Contrl(0);//�򿪳�������λ��
        break;
    }
    //�޸�״̬��
    gDeviceStatus.DEV_state &= 0xf0;
    gDeviceStatus.CLASS_state &= 0xfe;

    //�رն�λ��
    gYXJ.type = 1;
    //if (ps->DEV_Link_State&DEV_LINK_BJCardLK)
    // DWQ_Contrl(0);//�򿪳�������λ��
    //֪ͨ��ý��
    ViscaCount = 0;
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_CLASS_OVER;
    if (ps->DEV_Link_State & DEV_LINK_BJCardLK)
    {
        ip = Get_DesIp(DEVICE_SUB_BJCard);
        if (ARP_Rec_Find(ip) == NULL)
        {
            Arp_Request(ip);
            goto pclass;
            //return 1;
        }
        else
            UDP_Send_CMD(buf, 2, ip);
    }
pclass:
    //֪ͨ����
    ip = Get_DesIp(DEVICE_SUB_SER);
    if (ARP_Rec_Find(ip) == NULL)
    {
        Arp_Request(ip);
        return 1;
    }
    else
        UDP_Send_CMD(buf, 2, ip);

    return 1;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_RF_ACTIVE(void *pdata)
{
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
uint32 On_MSG_OPTION_HELP_ON(void *pdata)
{

    uint8 buf[20];
    uint16 tmp;
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif


    memset(buf, 0, 20);
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_HELP_LINK;
    memcpy(buf + 2, Get_DesIp(DEVICE_SUB_CLASS), 4);
    UDP_Send_CMD(buf, 20, Get_DesIp(DEVICE_SUB_SER));
    ///����
    buf[0] = 192;
    buf[1] = 168;
    buf[2] = 0;
    buf[3] = 230;
    //Sound_Open(buf,AUDIO_PORT,0,SOUND_CONTRL_SEND);
    tmp = Device_Get_MIX();
    tmp |= 1 << 12;
    Device_Set_MIX(tmp);



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
uint32 On_MSG_OPTION_HELP_OFF(void *pdata)
{
    uint8 buf[20];

#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif


    memset(buf, 0, 20);
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_HELP_BREAK;
    memcpy(buf + 2, Get_DesIp(DEVICE_SUB_CLASS), 4);
    UDP_Send_CMD(buf, 20, Get_DesIp(DEVICE_SUB_SER));
    Sound_Close(NULL, SOUND_OPTION_ALL);
    Key_TB_state(KEY_REP_HELP, FALSE);
    LED_Set(LED_HELP, LED_DISPLAY_OFF);
    LED_SendFLASH();
    return 0;
}
/////////////////////////////////////////////////////////////////////
//  �������ƣ�
//  �������ܣ�
//  ��ڲ�����
//
//
//  ���ڲ�������
////////////////////////////////////////////////////////////////////
uint32  On_MSG_OPTION_IP_DPlay(void *pdata)
{
    uint8 *des_ip;
    uint16 tmp;
    //	 struct MSG_REV_TYPE *prev;
    des_ip = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    Sound_Close(NULL, SOUND_OPTION_ALL);
    Sound_Open(des_ip, AUDIO_PORT, 0, SOUND_CONTRL_ECHO | SOUND_CONTRL_SEND | SOUND_IP_ASK);
    IP_printf("�յ����������Խ�ģʽָ��");
    Key_TB_state(KEY_REP_HELP, TRUE);
    LED_Set(LED_HELP, LED_DISPLAY_ON);
    LED_SendFLASH();
    tmp = Device_Get_MIX();
    tmp &= ~(3 << 12);
    tmp |= 1 << 12;
    tmp &= ~SMIX_LAN_OUT_SELEC;
    Device_Set_MIX(tmp);

    return 0;


}
/////////////////////////////////////////////////////////////////////
//  �������ƣ�
//  �������ܣ�
//  ��ڲ�����
//
//
//  ���ڲ�������
////////////////////////////////////////////////////////////////////

uint32 on_MSG_OPTION_IP_MICVol(void *pdata)
{
    uint8 *micvol;
    micvol = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    TMS320AIC23_Set_Vol(TMS320AIC23_MIC_VOL, *micvol);
    IP_printf("����mic����(0-100)%d", *micvol);
    return 0;


}
/////////////////////////////////////////////////////////////////////
//  �������ƣ�
//  �������ܣ�
//  ��ڲ�����
//
//
//  ���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 on_MSG_OPTION_IP_HPVol(void *pdata)
{
    uint8 *micvol;
    micvol = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    TMS320AIC23_Set_Vol(TMS320AIC23_HP_VOL, *micvol);
    IP_printf("����mic����(0-100)%d", *micvol);
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
uint32 On_MSG_OPTION_HELP_TEST(void *pdata)
{
    uint8 *des_ip;

#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    des_ip = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    Sound_Open(des_ip, AUDIO_PORT, 0, SOUND_CONTRL_ECHO | SOUND_CONTRL_SEND | SOUND_IP_hASK);
    /*Key_TB_state(KEY_REP_HELP,TRUE);
    LED_Set(LED_HELP,LED_DISPLAY_ON);
    LED_SendFLASH();
    tmp=Device_Get_MIX();
    tmp|=1<<12;
    Device_Set_MIX(tmp);*/
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

uint32 On_MSG_OPTION_IPPlay(void *pdata)
{


    uint8 *des_ip, cmd;
    uint32 tmp;

#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    cmd = *((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
    if (cmd == 1)
    {
        des_ip = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + 1;
        Sound_Close( des_ip, SOUND_OPTION_IP );
        Sound_Open(des_ip, AUDIO_PORT, 0, SOUND_CONTRL_SEND | SOUND_IP_mASK);
        tmp = Device_Get_MIX();
        tmp &= ~(3 << 12);
        tmp |= 3 << 12;
        tmp |= SMIX_LAN_OUT_SELEC;
        Device_Set_MIX(tmp);
        IP_printf("�յ�������ѧϵͳ��ͨ������");
    }
    else
    {
        Sound_Close(NULL, SOUND_OPTION_ALL);
        IP_printf("�յ�������ѧϵͳ�ر�ͨ������");
    }

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
uint32 On_MSG_OPTION_HELP_LINK(void *pdata)
{
    uint8 *des_ip;
    uint32 tmp;
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif


    des_ip = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    Sound_Close( des_ip, SOUND_OPTION_ALL);
    Sound_Open(des_ip, AUDIO_PORT, 0, SOUND_CONTRL_SEND);
    Key_TB_state(KEY_REP_HELP, TRUE);
    LED_Set(LED_HELP, LED_DISPLAY_ON);
    LED_SendFLASH();
    tmp = Device_Get_MIX();
    tmp &= ~(3 << 12);
    tmp |= 1 << 12;
    tmp &= ~SMIX_LAN_OUT_SELEC;

    Device_Set_MIX(tmp);

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
uint32 On_MSG_OPTION_HELP_BREAK(void *pdata)
{
    uint8 buf[20];



    memset(buf, 0, 20);
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_HELP_BREAK;
    memcpy(buf, Get_DesIp(DEVICE_SUB_CLASS), 4);
    UDP_Send_CMD(buf, 20, Get_DesIp(DEVICE_SUB_SER));
    Sound_Close(NULL, SOUND_OPTION_ALL);
    Key_TB_state(KEY_REP_HELP, FALSE);
    LED_Set(LED_HELP, LED_DISPLAY_OFF);
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
uint32 On_MSG_OPTION_AUTO_ON(void *pdata)
{
    //zjj add
    if (Get_Class_Stauts() == CLASS_OVER) return 0;
    gDeviceStatus.DEV_state = gDeviceStatus.DEV_state & 0xf | DEV_STATE_OPTIONAT;

    LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
    LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
    LED_Set(LED_MODE_ZD, LED_DISPLAY_ON);
    LED_SendFLASH();
    KEY_DATA.MODE = MSG_OPTION_AUTO_ON;
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_AUTO_OFF(void *pdata)
{
    if (Get_Class_Stauts() == CLASS_OVER) return 0;
    gDeviceStatus.DEV_state &= 0xf ;
    gDeviceStatus.DEV_state |= DEV_STATE_OPTIONSD;
    LED_Set(LED_MODE_SD, LED_DISPLAY_ON);
    LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
    LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
    LED_SendFLASH();
    KEY_DATA.MODE = MSG_OPTION_AUTO_OFF;
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_DEV_REMOTE_ON(void *pdata)
{
    BYTE bState = gDeviceStatus.DEV_state & 0xf0 ;
    bState = bState >> 4;
    if ( bState  &  8 )
    {
        gDeviceStatus.DEV_state = gDeviceStatus.DEV_state & 0xf | DEV_STATE_OPTIONJG;//�ӹ��豸
    }
    else
    {
        gDeviceStatus.DEV_state = gDeviceStatus.DEV_state & 0xf | DEV_STATE_OPTIONJGDEV;//�ӹ��豸
    }
    if (Get_Class_Stauts() == CLASS_OVER) return  STATUS_CHANGE;
    LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
    LED_Set(LED_MODE_YC, LED_DISPLAY_ON);
    LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_REC_REMOTE_ON(void *pdata)
{
    BYTE bState = gDeviceStatus.DEV_state & 0xf0 ;
    bState = bState >> 4;
    if ( bState  &  4 )
    {
        gDeviceStatus.DEV_state = gDeviceStatus.DEV_state & 0xf | DEV_STATE_OPTIONJG;//�ӹ�¼��
    }
    else
    {
        gDeviceStatus.DEV_state = gDeviceStatus.DEV_state & 0xf | DEV_STATE_OPTIONJGREC;//�ӹ�¼��
    }
    if (Get_Class_Stauts() == CLASS_OVER) return  STATUS_CHANGE;
    LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
    LED_Set(LED_MODE_YC, LED_DISPLAY_ON);
    LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_DEV_REMOTE_OFF(void *pdata)
{
    gDeviceStatus.DEV_state &= (~(DEV_STATE_OPTIONJGDEV)) ;
    if (Get_Class_Stauts() == CLASS_OVER) return  STATUS_CHANGE;
    if (KEY_DATA.MODE == MSG_OPTION_AUTO_ON)
    {
        LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_ZD, LED_DISPLAY_ON);
    }
    else
    {

        LED_Set(LED_MODE_SD, LED_DISPLAY_ON);
        LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_REC_REMOTE_OFF(void *pdata)
{
    gDeviceStatus.DEV_state &= (~(DEV_STATE_OPTIONJGREC)) ;
    if (Get_Class_Stauts() == CLASS_OVER) return  STATUS_CHANGE;
    if (KEY_DATA.MODE == MSG_OPTION_AUTO_ON)
    {
        LED_Set(LED_MODE_SD, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_ZD, LED_DISPLAY_ON);
    }
    else
    {

        LED_Set(LED_MODE_SD, LED_DISPLAY_ON);
        LED_Set(LED_MODE_YC, LED_DISPLAY_OFF);
        LED_Set(LED_MODE_ZD, LED_DISPLAY_OFF);
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_AUTOTRACK_ON(void *pdata)
{

    gDeviceStatus.CLASS_state |= (1 << 1);
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_AUTOTRACK_OFF(void *pdata)
{
    gDeviceStatus.CLASS_state &= (~(1 << 1));
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_STRATEGY_ON(void *pdata)
{
    gDeviceStatus.CLASS_state |= (1 << 2);
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_STRATEGY_OFF(void *pdata)
{
    gDeviceStatus.CLASS_state &= (~(1 << 2));
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HD_ON(void *pdata)
{
    gDeviceStatus.CLASS_state |= (1 << 3);
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HD_OFF(void *pdata)
{
    gDeviceStatus.CLASS_state &= (~(1 << 3));
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HD_VAL(void *pdata)
{
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
uint32 On_MSG_OPTION_SCAMERA_CONTRL(void *pdata)
{
    uint8 LMT_Cmd, *p, buf[16], ID, tmp;
    uint16 hSpeed, vSpeed;
    uint8 dwPos = 0;
    uint32 bps;
    uint8 portNum;
    struct DEVICE_INFO *pdev;
    struct COMM_XCOM  *pxcom;
#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    p = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    LMT_Cmd = *p;
    ID = *(p + 1);
    hSpeed = *(p + 2);
    vSpeed = *(p + 3);
    dwPos = *(p + 3);
    pdev = (struct DEVICE_INFO *)Get_ClassInfo_Table(ID);
    if (pdev->dev_id != ID)
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("��������ƴ�����豸��:%d", ID);
        return 0;
    }

    if ((pdev->dev_type != MSG_DEV_TQCAMERA) &&
            (pdev->dev_type != MSG_DEV_SQCAMERA) &&
            (pdev->dev_type != MSG_DEV_BQCAMERA) &&
            (pdev->dev_type != MSG_DEV_SCAMERA1) &&
            (pdev->dev_type != MSG_DEV_SCAMERA2) &&
            (pdev->dev_type != MSG_DEV_SCAMERA3))
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("��������ƴ�����豸����:%d", pdev->dev_type);
        return 0;

    }  /*
         if(pdev->COMM.val.xcom.PortNum == 6)
         {
           portNum=0xff;

            portNum = DEV_XCOM_3;
         }
         else
         {
            portNum = DEV_XCOM_5;
         }
         */
    portNum = pdev->COMM.val.xcom.PortNum;
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("ID:%d��������ƴ��ں�:%d", ID, portNum);
    buf[0] = 0xff;
    buf[1] = pdev->COMM.val.xcom.address;
    SetAddress(pdev->COMM.val.xcom.address, pdev->COMM.val.xcom.PROTOCOL);
    pxcom = Get_XCOM_Port(portNum);
    bps = pdev->COMM.val.xcom.BPS + pdev->COMM.val.xcom.Hbps * 0x10000;
    if (Get_Debug(DEV_DEBUG_MSG))
    {
        IP_printf("����������豸��:%d", ID);
        IP_printf("���������BPS:%d", bps);
        IP_printf("����������˿�:%d��ַ:%d", pdev->COMM.val.xcom.PortNum, pdev->COMM.val.xcom.address);
    }

    if ((bps == 2400 ) || (bps == 4800) || (bps == 9600) || (bps == 19200) || (bps == 38400))
        if (bps != pxcom->BPS)
            Xcom_set(portNum, bps, 0, pdev->COMM.val.xcom.linecontrl);
    if(ViscaCount == 0)
    {
        buf[0] = 0x88;
        buf[1] = 0x30;
        buf[2] = 0x01;
        buf[3] = 0xff;
        UART_Write(portNum, buf, 4);
        ViscaCount++;
    }
    switch(LMT_Cmd)
    {
    case 1://up�������ת��(��)
        TurnUpCmd( buf, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������ת��(��)", LMT_Cmd);
        break;
    case 2://�������ת��(��)
        TurnDownCmd( buf, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������ת��(��)", LMT_Cmd);
        break;
    case 3://�������ת��(��)
        TurnLeftCmd( buf, hSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������ת��(��)", LMT_Cmd);
        break;
    case 4://�������ת��(��)
        TurnRightCmd( buf, hSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������ת��(��)", LMT_Cmd);
        break;
    case 5://�������ת��(����)
        TurnLeftUpCmd( buf, hSpeed, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������ת��(����)", LMT_Cmd);
        break;
    case 6://�������ת��(����)
        TurnLeftDownCmd( buf, hSpeed, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������ת��(����)", LMT_Cmd);
        break;
    case 7://�������ת��(����)
        TurnRightUpCmd( buf, hSpeed, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������ת��(����)", LMT_Cmd);
        break;
    case 8:	//�������ת��(����)
        TurnRightDownCmd( buf, hSpeed, vSpeed , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������ת��(����)", LMT_Cmd);
        break;
    case 9://ֹͣת��
        StopCmd(portNum, buf , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������ֹͣת��", LMT_Cmd);
        if(pdev->COMM.val.xcom.PROTOCOL == DEV_PROTOCOL_VISCA)
            return 0;
        break;
    case 10://���Zoom +
        ZoomInCmd( buf, hSpeed, pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������Zoom +", LMT_Cmd);
        break;
    case 11://���Zoom -
        ZoomOutCmd( buf, hSpeed, pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������Zoom -", LMT_Cmd);
        break;
    case 12://���Focus +
        FocusNearCmd(  buf , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������Focus +", LMT_Cmd);
        break;
    case 13://���Focus -
        FocusFarCmd(  buf  , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������Focus -", LMT_Cmd);
        break;
    case 14://�������Ԥ��λ
        GotoPrepositionCmd( buf, (BYTE)dwPos , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d��������������Ԥ��λ:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 15://��ѯ�����ǰ�Ƕ�
        buf[2] = 0x00;
        buf[3] = 0x51;
        buf[4] = 0;
        buf[5] = 0;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d��ѯ�����ǰ�Ƕ�", LMT_Cmd);
        break;
    //16
    case 17://�������Ԥ��λ
        SetPrepositionCmd(  buf, (BYTE)dwPos , pdev->COMM.val.xcom.PROTOCOL);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�������Ԥ��λ:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 18://ɾ�����Ԥ��λ
        buf[2] = 0x00;
        buf[3] = 0x05;
        buf[4] = hSpeed;
        buf[5] = vSpeed;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%dɾ�����Ԥ��λ:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 19://ˮƽ����ת���ԽǶ�
        buf[2] = 0x00;
        buf[3] = 0x4b;
        buf[4] = hSpeed;
        buf[5] = vSpeed;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%dˮƽ����ת���ԽǶ�:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 20://	��ֱ����ת���ԽǶ�
        buf[2] = 0x00;
        buf[3] = 0x4d;
        buf[4] = hSpeed;
        buf[5] = vSpeed;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d��ֱ����ת���ԽǶ�:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 21://��ȡ�������ά����
        if(DEV_PROTOCOL_PD == pdev->COMM.val.xcom.PROTOCOL )
        {
            buf[0] = 0x55;
            buf[1] = 0x01;
            buf[2] = pdev->COMM.val.xcom.address;
            for (tmp = 3; tmp < 10; tmp++)
                buf[tmp] = 0;
            buf[10] = 0;
            for (tmp = 1; tmp < 10; tmp++)
                buf[10] += buf[tmp];
            UART_Write(portNum, buf, 11);
        }
        else if(DEV_PROTOCOL_VISCA == pdev->COMM.val.xcom.PROTOCOL)
        {
            buf[0] = 0x80 + pdev->COMM.val.xcom.address;
            buf[1] = 0x09;
            buf[2] = 0x06;
            buf[3] = 0x12;
            buf[4] = 0xff;
            UART_Write(portNum, buf, 5);
            OSTimeDly(10);
            buf[1] = 0x09;
            buf[2] = 0x04;
            buf[3] = 0x47;
            buf[4] = 0xff;
            UART_Write(portNum, buf, 5);
        }

        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d��ȡ�������ά����", LMT_Cmd);
        return  0;
    // case 22://
    case 23://�����������ά����
        if(DEV_PROTOCOL_PD == pdev->COMM.val.xcom.PROTOCOL)
        {
            buf[0] = 0x55;
            buf[1] = 0xfc;
            buf[2] = pdev->COMM.val.xcom.address;
            for (tmp = 3; tmp < 10; tmp++)
                buf[tmp] = *(p + tmp - 1);
            buf[10] = 0;
            for (tmp = 1; tmp < 10; tmp++)
                buf[10] += buf[tmp];
            UART_Write(portNum, buf, 11);
        }
        else if(DEV_PROTOCOL_VISCA == pdev->COMM.val.xcom.PROTOCOL)
        {
            buf[0] = 0x80 + pdev->COMM.val.xcom.address;
            buf[1] = 0x01;
            buf[2] = 0x06;
            buf[3] = 0x02;
            buf[4] = hSpeed;
            buf[5] = vSpeed;
            for(tmp = 6; tmp < 14; tmp++)
                buf[tmp] = *(p + tmp - 2);
            buf[14] = 0xff;
            UART_Write(portNum, buf, 15);
            buf[1] = 0x01;
            buf[2] = 0x04;
            buf[3] = 0x47;
            for(tmp = 4; tmp < 8; tmp++)
                buf[tmp] = *(p + tmp + 8);
            buf[8] = 0xff;
            UART_Write(portNum, buf, 9);
        }
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d�����������ά����", LMT_Cmd);
        return  0;
    case 24://��ѯ�����ǰ��ֱ�Ƕ�
        buf[2] = 0x00;
        buf[3] = 0x53;
        buf[4] = 0;
        buf[5] = 0;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d��ѯ�����ǰ��ֱ�Ƕ�", LMT_Cmd);
        break;
    case 25://����zoomֵ
        if(DEV_PROTOCOL_PD == pdev->COMM.val.xcom.PROTOCOL)
        {
            buf[2] = 0x00;
            buf[3] = 0x4f;
            buf[4] = hSpeed;
            buf[5] = vSpeed;
        }
        else if(DEV_PROTOCOL_VISCA == pdev->COMM.val.xcom.PROTOCOL)
        {
            buf[1] = 0x01;
            buf[2] = 0x04;
            buf[3] = 0x47;
            buf[8] = 0xff;
            //UART_Write(portNum,buf,9);
            return 0;
        }
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d����zoomֵ:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 26://����zoom�ٶ�
        buf[2] = *(p + 4);
        buf[3] = 0x79;
        buf[4] = hSpeed;
        buf[5] = vSpeed;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d����zoom�ٶ�:%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    case 27://����ˮƽ�ٶ�
        buf[2] = 0x00;
        buf[3] = 0x57;
        buf[4] = hSpeed;
        buf[5] = vSpeed;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%d����ˮƽ�ٶ�%d,%d", LMT_Cmd, hSpeed, vSpeed);
        break;
    default:
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("�������������:%dδ����������������", LMT_Cmd);
        return 0;
    }
    if(DEV_PROTOCOL_PD == pdev->COMM.val.xcom.PROTOCOL )
    {
        buf[6] = 0;
        for (tmp = 1; tmp < 6; tmp++)
            buf[6] += buf[tmp];
        UART_Write(portNum, buf, 7);
    }
    else if(DEV_PROTOCOL_VISCA == pdev->COMM.val.xcom.PROTOCOL)
    {
        UART_Write(portNum, buf, 9);
    }
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
uint32 On_MSG_OPTION_XCOM_SEND(void *pdata)
{
    tagCommand *p;
    // struct XCOM_REV_TYPE xcomrev;
    uint8 *p1;

    p1 = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + 2;
    p = (tagCommand *)p1;
    //IP_printf("p:%x %x %x %x %x %x %x %x\n",p->sender,p->receiver,p->command,p->flag,p->extended[0],p->extended[1],p->extended[2],p->extended[3]);

    switch(p->command)
    {
    case EMCOMMANDSETBAUDRATE:
        Xcom_set(DEV_XCOM_8, p->extended[0], DEV_PROTOCOL_NONE, 0x03); //����1
        Xcom_set(DEV_XCOM_5, p->extended[0], DEV_PROTOCOL_NONE, 0x03); //����2
        Xcom_set(DEV_XCOM_3, p->extended[0], DEV_PROTOCOL_NONE, 0x03); //����3
        Xcom_set(DEV_XCOM_9, p->extended[0], DEV_PROTOCOL_NONE, 0x03); //����4
        break;
    case EMCOMMANDCOM1:
        UART_Write(DEV_XCOM_8, (uint8 *)p + 4, p->flag);
        break;
    case EMCOMMANDCOM2:
        UART_Write(DEV_XCOM_5, (uint8 *)p + 4, p->flag);
        break;
    case EMCOMMANDCOM3:
        UART_Write(DEV_XCOM_3, (uint8 *)p + 4, p->flag);
        break;
    case EMCOMMANDCOM4:
        UART_Write(DEV_XCOM_9, (uint8 *)p + 4, p->flag);
        break;
    default:
        break;
    }/**/

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
uint32 On_MSG_OPTION_SCAMERA_VAL(void *pdata)
{
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
uint32 On_MSG_OPTION_DEBUG(void *pdata)
{
    struct BASE_MSG *pmsg;
    NetP *pnet;
    uint8 tmp;
    pnet = (NetP *)Get_NetPort();
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));

    if (pmsg->device == MSG_DEV_MAINBARD)
    {
        gDeviceTable.Debug = *((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
        if (Get_Debug(DEV_DEBUG_SYSTEM))
        {
            gDeviceTable.Debug &= ~DEV_DEBUG_SYSTEM;
            tmp = *((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + 1);
            Config_printf(tmp);
        }
        pnet->Test_TTL = 9;
    }
    IP_printf("����״̬:%2x", gDeviceTable.Debug);
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
uint32 On_MSG_OPTION_DEV_RESET(void *pdata)
{
    Device_KZ_Reset();
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ� MSG_OPTION_FILE_REQ://			0xb2//�ļ�����
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_FILE_REQ(void *pdata)
{
    struct SFILE_TRANSLATE_TYPE *pfile;
    struct MSG_REV_TYPE *prev;
    uint8 *v;

    prev = (struct MSG_REV_TYPE *)pdata;
    pfile = (struct SFILE_TRANSLATE_TYPE *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
    //v = (uint8 *)((uint8 *)pdata+sizeof(struct MSG_REV_TYPE));
    //IP_printf("rev:%x %x %x %x %x %x %x %x %x %x",*(v+0),*(v+1),*(v+2),*(v+3),*(v+4),*(v+5),*(v+6),*(v+7),*(v+8),*(v+9));
    switch(pfile->FileType)
    {
    case FILE_TYPE_TABLE:

        //IP_printf("page:%d", pfile->FilePage);
        File_Table_Read(pfile->FilePage, prev->des_ip);
        break;
    case FILE_TYPE_CLASS_TABLE:
        File_ClassTable_Read(pfile->FilePage, prev->des_ip);
        break;
    case FILE_TYPE_USER_PRORAM:
        break;
    case FILE_TYPE_KB_PROGRAM:
        break;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	��������:MSG_OPTION_FILE_SEND://			0xb3//�ļ�����
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_FILE_SEND(void *pdata)
{
    struct SFILE_TRANSLATE_TYPE *pfile;
    struct MSG_REV_TYPE *prev;

    prev = (struct MSG_REV_TYPE *)pdata;
    pfile = (struct SFILE_TRANSLATE_TYPE *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
    switch(pfile->FileType)
    {
    case FILE_TYPE_TABLE:
        File_Table_Write(pfile->FilePage, ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE ), prev->des_ip);
        break;
    case FILE_TYPE_USER_PRORAM:
        File_UserProgram_Write(pfile->FilePage, ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE ), prev->des_ip);
        break;
    case FILE_TYPE_KB_PROGRAM:
        break;
    case FILE_TYPE_CLASS_TABLE:
        File_ClassTable_Write(pfile->FilePage, ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE ), prev->des_ip);
        break;

    }
    return 0;
}

uint32 On_MSG_OPTION_SERVER_FILE_SEND(void *pdata)
{
    struct SFILE_TRANSLATE_TYPE *pfile;
    struct MSG_REV_TYPE *prev;
    uint8 *p, *pBuf;
    uint32 resault = 0xffff;

#if OS_CRITICAL_METHOD == 3              /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;                   /* Prevent compiler warning                      */
#endif
    prev = (struct MSG_REV_TYPE *)pdata;
    pfile = (struct SFILE_TRANSLATE_TYPE *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
    switch(pfile->FileType)
    {
    case FILE_TYPE_TABLE_BASEINFO:
        p = (uint8 *)User_mem_allco(500);
        if (NULL != p)
        {
            memcpy( p, (uint8 *)Get_ClassInfo_Table(0), 256 );
            pBuf = ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE );
            /*memcpy( p+2,pBuf+2,32 );
            memcpy( p+ 2+32,pBuf+2+32,32 );
            memcpy( p + 2 + 32 +32,pBuf+2+32+32,32 );
            memcpy( p + 132,pBuf+132,4 );*/
            memset( p + 2, '3', 20 );
            memset( p + 2 + 32, '5', 20 );
            OS_ENTER_CRITICAL();
            InitIAP();
            // resault=SelSector(ARM_TABLE_SECTOR_START,ARM_TABLE_SECTOR_END);
            // resault=EraseSector(ARM_TABLE_SECTOR_START,ARM_TABLE_SECTOR_END);
            // resault=BlankCHK(ARM_TABLE_SECTOR_START,ARM_TABLE_SECTOR_END);
            resault = SelSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
            resault = RamToFlash(ARM_TABLE_ADDRESS, (uint32)p, 256);
            OS_EXIT_CRITICAL();

            pBuf = (uint8 *) Get_ClassInfo_Table(0);
            memcpy( p + 6, p, 256 );
            *p = MSG_DEV_MAINBARD;
            *(p + 1) = MSG_OPTION_SERVERWRITE_ACK;
            memcpy( p + 2, (uint8 *)pfile, 4 );
            UDP_Send_CMD(p, 256 + sizeof(struct SFILE_TRANSLATE_TYPE ) + 2, prev->des_ip);
            //File_Table_Write(0,( BYTE *)p,prev->des_ip);
            User_mem_free(p);
        }
        break;
    case FILE_TYPE_CLASS_TABLE_ONEDAY:
        File_ClassTable_Write_OneDay(pfile->FilePage, ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE ), prev->des_ip);
        p = (uint8 *)User_mem_allco(500);
        if (p != NULL)
        {
            memcpy( p + 6, ( BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE ), 256 );
            *p = MSG_DEV_MAINBARD;
            *(p + 1) = MSG_OPTION_SERVERWRITE_ACK;
            memcpy( p + 2, (uint8 *)pfile, 4 );
            UDP_Send_CMD(p, 256 + sizeof(struct SFILE_TRANSLATE_TYPE ) + 2, prev->des_ip);
            User_mem_free(p);
        }
        break;
    }
    resault = resault;
    pBuf = pBuf;
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�����������On_MSG_OPTION_SVRLINK_REQ()����
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////

uint32 On_MSG_OPTION_SEVER_ACK(void *pdata)
{
    struct STIME_LINK_TYPE *pstime;
    struct REALTIME_TYPE realtime;

    pstime = (struct STIME_LINK_TYPE *)((BYTE *)pdata + sizeof(struct BASE_MSG) + sizeof( struct MSG_REV_TYPE));
    realtime.year = pstime->yearH + pstime->yearL * 256;
    realtime.mon = pstime->mon;
    realtime.day = pstime->day;
    realtime.week = pstime->week;
    realtime.hour = pstime->hour;
    realtime.min = pstime->min;
    realtime.sec = pstime->sec;
    RealTime_Set((BYTE *)&realtime);
    LED_Time_Flash();
    if ((realtime.week - 1) != gClassT.week)
        File_ClassTable_Read_CurDay();
    gDeviceTable.flag_sver |= DEV_FLAG_SVER_LINK;

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
void  On_MSG_OPTION_STATUS_CHANGE(BYTE *pdata)
{
    BYTE buf[80], tmp, *ip;
    struct MSG_REV_TYPE Rev;
    Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
    Rev.Type = DEV_XCOM_SELF;
    pdata = pdata;
    memset(buf, 0, 16);
    ip = Get_DesIp(DEVICE_SUB_SER);
    if (ARP_Rec_Find(ip) == NULL)
    {
        Arp_Request(ip);
    }
    else if (IP_Compare(ip, buf) == FALSE)
    {
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_STATUS_CHANGE;
        memcpy(buf + sizeof(struct BASE_MSG), (BYTE *)Get_Device_Status(), sizeof(struct DEVICE_STATUS_TYPE));
        UDP_Send_CMD(buf, sizeof(struct BASE_MSG) + sizeof(struct DEVICE_STATUS_TYPE), ip);
    }

    if (gDeviceStatus.DEV_Link_State & DEV_LINK_PCON)
    {
        memset(buf, 0, 16);
        ip = Get_DesIp(DEVICE_SUB_PC);
        if (ARP_Rec_Find(ip) == NULL)
        {
            Arp_Request(ip);
        }
        else if (IP_Compare(ip, buf) == FALSE)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_PCLK;
            buf[0] = MSG_DEV_MAINBARD;
            buf[1] = MSG_OPTION_DEV_LINK;
            UDP_Send_CMD(buf, sizeof(struct BASE_MSG), ip);
        }
    }
    if ((gDeviceTable.flag_sver & DEV_FLAG_SVER_LINK) == 0)
        tmp = 10;
    else
        tmp = 180;
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_STATUS_CHANGE;
    if (Task_Rec_Find(buf, 2) == 0xff)
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + tmp, buf, 2, 0, (uint8 *)&Rev);
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�void On_MSG_OPTION_SEVER_LINK(void *pdata)
//	�������ܣ�����������0xb1
//	��ڲ�����pdata=��Ϣ+STIME_LINK_TYPE����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_SVRLINK_REQ(void *pdata)
{
    BYTE buf[80];
    NetP *pnet;
    struct MSG_REV_TYPE *prev;
    prev = (struct MSG_REV_TYPE *)pdata;
    pnet = (NetP *)Get_NetPort();

    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_SVRLINK_REQ;
    memcpy(buf + sizeof(struct BASE_MSG), (BYTE *)Get_Device_Status(), sizeof(struct DEVICE_STATUS_TYPE));
    UDP_Send_CMD(buf, sizeof(struct BASE_MSG) + sizeof(struct DEVICE_STATUS_TYPE), prev->des_ip);
    pnet = pnet;
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//			MSG_OPTION_HW_SEND
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_HW_SEND(BYTE *pdata)
{
    return 0;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�1s
//	��ڲ�����
//			MSG_OPTION_TIMEACTIVE
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_TIMEACTIVE(BYTE *pdata)
{
    uint8 tmp, tciir, rev_buf[10];
    struct REALTIME_TYPE *ptime;
    struct TASK_REC_TYPE *ptask;
#if OS_CRITICAL_METHOD == 3                /* Allocate storage for CPU status register        */
    OS_CPU_SR  cpu_sr;                    /* Prevent compiler warning                        */
#endif
    tciir = *(pdata + sizeof(struct BASE_MSG) + sizeof(struct MSG_REV_TYPE));
    rev_buf[0] = DEV_XCOM_SELF;
    //���¿γ̱�
    if (tciir & REALTIME_CIIR_WEEK)
    {
        ptime = RealTime_Get();
        if ((ptime->week != gClassT.week) && (ptime->week < 7))
        {
            gClassT.week = ptime->week;
            File_ClassTable_Read_CurDay();
        }
    }

    if  (tciir & REALTIME_CIIR_MIN)
    {
        //if (DEV_STATE_OPTIONSD != gDeviceStatus.DEV_state & 0xf) //�豸���ó��ֶ�
        //{
        // if (Check_FunctionMode(Device_FunctionMode_CARD)==0) //�豸û������RF����
        //Class_Table_Check(); //change 10.21
        //}
    }

    if (TASK_TABLE.RecCount == 0)
        return  0;
    //IP_printf("1��,TASK_TABLE.RecCount:%x",TASK_TABLE.RecCount);
    OS_ENTER_CRITICAL();
    for (tmp = 0; tmp < TASK_DELAY_MAX_SIZE; tmp++)
    {
        ptask = (struct TASK_REC_TYPE *)&TASK_TABLE.DelayRec[tmp];
        if ((ptask->TimeType & tciir) && (ptask->pmsg != NULL))
        {
            if (ptask->DelayTime != 0)
            {
                ptask->DelayTime--;
                if (ptask->DelayTime == 0)
                    ptask->TimeType = REALTIME_CIIR_SEC;
            }
            else
            {
                memcpy(rev_buf, ptask->pmsg, sizeof(struct MSG_REV_TYPE));
                Msg_send((uint8 *)ptask->pmsg + sizeof(struct MSG_REV_TYPE), ptask->msg_lg, rev_buf);
                if (ptask->repeat == 0)
                {
                    User_mem_free(ptask->pmsg);
                    if (TASK_TABLE.RecCount != 0)
                        TASK_TABLE.RecCount--;
                    ptask->DelayTime = 0;
                    ptask->pmsg = NULL;
                    ptask->TimeType = 0;
                }
                else
                {
                    ptask->repeat--;
                    ptask->DelayTime = ptask->bak_time;
                }
            }
        }
    }
    OS_EXIT_CRITICAL();
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
uint32 On_MSG_RepeatPower_OPTION(void *pdata)
{
    uint8 *p, id, *pbuf, buf[5] = {0};
    struct DEVICE_INFO *pdev;
    struct MESSAGE_TYPE *poption;

    pbuf = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE);
    //zjj add
    if (MSG_OPTION_REPEATPOWERCLOSE == pbuf[1])
    {
        pbuf[1] = pbuf[2];
    }
    //zjj add
    pdev = (struct DEVICE_INFO *)Device_Rec_Find(pbuf, DEV_FIND_TYPE);
    if (pdev == NULL)
        return FALSE;//û�и��豸�Ǽ�

    id = Msg_Option_Find(pbuf);
    if (id == 0)
        return FALSE; //û�иò������¼
    poption = (struct MESSAGE_TYPE *)Get_ClassInfo_Table(id);
    p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
    switch (pdev->COMM.comm_type)
    {
    case DEVICE_COMMTYPE_NONE:
        break;
    case DEVICE_COMMTYPE_HW:
        HW_send(poption->dev_id);
        break;
    case DEVICE_COMMTYPE_XCOM:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        //		UART_Send_Data(pdev->COMM.val.xcom.PortNum,p, poption->data_lg);
        UART_Write(pdev->COMM.val.xcom.PortNum, p, poption->data_lg);
        break;
    case DEVICE_COMMTYPE_UDP:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        break;
    }

    if (MSG_DEV_TYJA == pdev->dev_type)
    {
        buf[0] = MSG_DEV_TYJB;
        buf[1] = pbuf[1];
        //buf[2] = MSG_OPTION_POWERCLOSE;
        buf[2] = DEV_XCOM_SELF;
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 5, &buf[0], 2, 0, &buf[2]);
    }

    //memcpy( &id,(uint8 *)pdata + 3,1 );
    id = pbuf[3];
    if ((MSG_DEV_TYJA == pdev->dev_type || MSG_DEV_TYJB == pdev->dev_type) && id > 1  && MSG_OPTION_POWERCLOSE == pbuf[1])
    {
        //memcpy( &buf[0],pbuf,2 );
        buf[0] = pdev->dev_type;
        buf[1] = MSG_OPTION_REPEATPOWERCLOSE;
        buf[2] = pbuf[2];
        buf[3] = id - 1;
        id = (pdev->prev_val >> 3);
        buf[4] = DEV_XCOM_SELF;
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + id, &buf[0], 4, 3, &buf[4]);
    }
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
uint32 On_MSG_Power_OPTION(void *pdata)
{
    uint8 *p, id, *pbuf, buf[5] = {0};
    struct DEVICE_INFO *pdev;
    struct MESSAGE_TYPE *poption;

    pbuf = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE);
    pdev = (struct DEVICE_INFO *)Device_Rec_Find(pbuf, DEV_FIND_TYPE);
    if (pdev == NULL)
        return FALSE;//û�и��豸�Ǽ�

    id = Msg_Option_Find(pbuf);
    if (id == 0)
        return FALSE; //û�иò������¼
    poption = (struct MESSAGE_TYPE *)Get_ClassInfo_Table(id);
    p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
    switch (pdev->COMM.comm_type)
    {
    case DEVICE_COMMTYPE_NONE:
        break;
    case DEVICE_COMMTYPE_HW:
        HW_send(poption->dev_id);
        break;
    case DEVICE_COMMTYPE_XCOM:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        //		UART_Send_Data(pdev->COMM.val.xcom.PortNum,p, poption->data_lg);
        if(pdev->COMM.val.xcom.PortNum == DEV_XCOM_TYJ)
            Xcom_set(DEV_XCOM_TYJ, pdev->COMM.val.xcom.BPS + (pdev->COMM.val.xcom.Hbps * 0x10000), DEV_PROTOCOL_NONE, pdev->COMM.val.xcom.linecontrl);
        UART_Write(pdev->COMM.val.xcom.PortNum, p, poption->data_lg);
        break;
    case DEVICE_COMMTYPE_UDP:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        break;
    }

    if (MSG_DEV_TYJA == pdev->dev_type)
    {
        buf[0] = MSG_DEV_TYJB;
        buf[1] = pbuf[1];
        buf[2] = DEV_XCOM_SELF;
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 5, &buf[0], 2, 0, &buf[2]);
    }

    id = pdev->prev_val & 0x7;
    if ((MSG_DEV_TYJA == pdev->dev_type || MSG_DEV_TYJB == pdev->dev_type) && id > 1 && MSG_OPTION_POWERCLOSE == pbuf[1])
    {
        buf[0] = pdev->dev_type;
        buf[1] = MSG_OPTION_REPEATPOWERCLOSE;
        buf[2] = MSG_OPTION_POWERCLOSE;
        buf[3] = id - 1;

        id = (pdev->prev_val >> 3);
        buf[4] = DEV_XCOM_SELF;
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + id, &buf[0], 4, 0, &buf[4]);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION(void *pdata)
{
    uint8 *p, id, *pbuf;
    struct DEVICE_INFO *pdev;
    struct MESSAGE_TYPE *poption;

    pbuf = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE);
    pdev = (struct DEVICE_INFO *)Device_Rec_Find(pbuf, DEV_FIND_SELECT);
    if (pdev == NULL)
        return FALSE;//û�и��豸�Ǽ�
    id = Msg_Option_Find(pbuf);
    if (id == 0)
        return FALSE; //û�иò������¼
    poption = (struct MESSAGE_TYPE *)Get_ClassInfo_Table(id);
    p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
    switch (pdev->COMM.comm_type)
    {
    case DEVICE_COMMTYPE_NONE:
        break;
    case DEVICE_COMMTYPE_HW:
        HW_send(poption->dev_id);
        break;
    case DEVICE_COMMTYPE_XCOM:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        //		UART_Send_Data(pdev->COMM.val.xcom.PortNum,p, poption->data_lg);
        UART_Write(pdev->COMM.val.xcom.PortNum, p, poption->data_lg);
        break;
    case DEVICE_COMMTYPE_UDP:
        p = ((BYTE *)poption + sizeof(struct MESSAGE_TYPE ));
        break;
    }
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
uint32 ON_Device_select(void *pdata)
{
    uint8 tmp, buf[16];
    struct DEVICE_INFO *pdev;
    struct BASE_MSG bmsg, *pmsg;
    struct DEVICE_STATUS_TYPE *ps;

    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    //�ǵ�ǰ�豸
    if ((ps->Current_Device != pmsg->device) && (pmsg->device != MSG_DEV_PC))
    {
        if (ps->Current_Device != MSG_DEV_PC)
            Device_Stat_Option(ps->Current_Device, MSG_OPTION_STAT_END);
        Device_Stat_Option(pmsg->device, MSG_OPTION_STAT_START);
    }
    ps->Current_Device = pmsg->device;
    pdev = (struct DEVICE_INFO *)Device_Rec_Find((void *)pmsg, DEV_FIND_SELECT);
    //�����豸�л���Ϣ--���豸���ж���
    if (pdev != NULL)
    {
        if (pdev->VGA.VGAIn != DEV_VGA_NONE)  //��VGA����
        {
            //���ͶӰ��A  VGA�Ƿ�������
            if (gDeviceTable.TYJ[0].vgain	!=	DEV_VGA_NONE)
            {
                if (gDeviceTable.TYJ[0].curr_Type == 0)
                {
                    gDeviceTable.TYJ[0].curr_Type = 1; //������ʾVGA
                    bmsg.device = MSG_DEV_TYJA;
                    bmsg.option = MSG_OPTION_TYJ_DIS_VGA;
                    Mem_Copy(buf + sizeof(struct MSG_REV_TYPE), (uint8 *)&bmsg, sizeof(struct BASE_MSG));
                    On_MSG_OPTION(&buf);
                };
                if (pdev->VGA.VGALink	!=	DEV_VGA_OUT_NONE)
                {
                    Device_VGA_Select(pdev->VGA.VGAIn, pdev->VGA.VGALink);
                    if (pdev->VGA.VGALink  == DEV_VGA_OUT_TB1)
                        TB_SelectVGA(TB_InputVGA_OUT_TB1, TB_OutputVGA_TYJ);
                }
                else
                    Device_VGA_Select(pdev->VGA.VGAIn, DEV_VGA_OUT_TYJ);
            };
            //���ͶӰ��B  VGA�Ƿ�������
            if (gDeviceTable.TYJ[1].vgain != DEV_VGA_NONE)
            {
                if (gDeviceTable.TYJ[1].curr_Type == 0)
                {
                    gDeviceTable.TYJ[1].curr_Type = 1; //������ʾVGA
                    bmsg.device = MSG_DEV_TYJB;
                    bmsg.option = MSG_OPTION_TYJ_DIS_VGA;
                    Mem_Copy(buf + sizeof(struct MSG_REV_TYPE), (uint8 *)&bmsg, sizeof(struct BASE_MSG));
                    On_MSG_OPTION(&buf);

                }
            }

        }
        //���ͬ�����
        if (pdev->VIDEO.VideoIn	!= DEV_VIDEO_NONE)
        {
            if (gDeviceTable.TYJ[0].videoin != DEV_VIDEO_OUT_NONE)
            {
                if (gDeviceTable.TYJ[0].curr_Type == 1)
                {
                    gDeviceTable.TYJ[0].curr_Type = 0;
                    bmsg.device = MSG_DEV_TYJA;
                    bmsg.option = MSG_OPTION_TYJ_DIS_VIDEO;
                    Mem_Copy(buf + sizeof(struct MSG_REV_TYPE), (uint8 *)&bmsg, sizeof(struct BASE_MSG));
                    On_MSG_OPTION(&buf);

                }
                Device_Video_Select(pdev->VIDEO.VideoIn, DEV_VIDEO_OUT_TYJA);
            }
            if (gDeviceTable.TYJ[1].videoin != DEV_VIDEO_OUT_NONE)
            {
                if (gDeviceTable.TYJ[1].curr_Type == 1)
                {
                    gDeviceTable.TYJ[1].curr_Type = 0;
                    bmsg.device = MSG_DEV_TYJB;
                    bmsg.option = MSG_OPTION_TYJ_DIS_VIDEO;
                    Mem_Copy(buf + sizeof(struct MSG_REV_TYPE), (uint8 *)&bmsg, sizeof(struct BASE_MSG));
                    On_MSG_OPTION(&buf);

                }
                Device_Video_Select(pdev->VIDEO.VideoIn, DEV_VIDEO_OUT_TYJB);
            }
        }
        //��Ƶ�ź�
        if (pdev->SOUND.SoundIn != DEV_SOUND_NONE)
        {
            Device_Sound_Contrl(TRUE);
            Device_Sound_Select(pdev->SOUND.SoundIn);
            Device_InVol(pdev->SOUND.SoundVol, DEV_SOUND_DEV);
            //zjj add6
            //SOUND_DEVICE_Mute(VOL_CH_MIC1,0);
            //SOUND_DEVICE_Mute(VOL_CH_MIC2,0);
            //SOUND_DEVICE_Mute(VOL_CH_MIC3,0);
            //SOUND_DEVICE_Mute(VOL_CH_MIC4,0);
            SoundVolumeControl(VOL_CH_GF, DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol);
            KEY_Set_Vol(DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_GF].OUT_Vol);
        }
        else
            Device_Sound_Contrl(FALSE);
    }
    else
    {
        //û���豸��
        pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
        switch(pmsg->device)
        {
        case MSG_DEV_PC:
            Device_VGA_Select(DEV_VGA_PC, DEV_VGA_OUT_TYJ);
            tmp = Device_Get_Vol(DEV_SOUND_OUT_GF, 4);
            Device_Sound_Select(DEV_SOUND_PC);
            Device_Sound_Contrl(TRUE);
            break;
        case MSG_DEV_DVD:
            Device_VGA_Select(DEV_VGA_DVD, DEV_VGA_OUT_TYJ);
            Device_Video_Select(DEV_VIDEO_DVD, DEV_VIDEO_OUT_TYJA);
            Device_Video_Select(DEV_VIDEO_DVD, DEV_VIDEO_OUT_TYJB);

            Device_Sound_Contrl(TRUE);
            Device_Sound_Select(DEV_SOUND_DVD);
            break;
        case MSG_DEV_NPC:
            Device_VGA_Select(DEV_VGA_NPC, DEV_VGA_OUT_TYJ);
            Device_Sound_Select(DEV_SOUND_NPC);

            break;
        case MSG_DEV_SHOW:
            Device_VGA_Select(DEV_VGA_SHOW, DEV_VGA_OUT_TYJ);
            Device_Video_Select(DEV_VIDEO_SHOW, DEV_VIDEO_OUT_TYJA);
            Device_Video_Select(DEV_VIDEO_SHOW, DEV_VIDEO_OUT_TYJB);
            Device_Sound_Contrl(FALSE);
            break;
        case MSG_DEV_LYJ:
            Device_Sound_Select(DEV_SOUND_LYJ);
            Device_Video_Select(DEV_VIDEO_LXJ, DEV_VIDEO_OUT_TYJA);
            Device_Video_Select(DEV_VIDEO_LXJ, DEV_VIDEO_OUT_TYJB);
            Device_Sound_Contrl(TRUE);
            break;
        case MSG_DEV_KZA:
        case MSG_DEV_KZB:
            Device_Sound_Contrl(TRUE);
            break;
        }
        Device_InVol(95, DEV_SOUND_DEV);
    }
    tmp = tmp;
    return STATUS_CHANGE;

}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 Send_State(void)
{
    BYTE *desip;
    BYTE buf[80] = {0};
    buf[0] = MSG_DEV_MAINBARD;
    buf[1] = MSG_OPTION_STATUS_CHANGE;
    desip = Get_DesIp(DEVICE_SUB_SER);
    memcpy(buf + sizeof(struct BASE_MSG), (BYTE *)Get_Device_Status(), sizeof(struct DEVICE_STATUS_TYPE));
    UDP_Send_CMD(buf, sizeof(struct BASE_MSG) + sizeof(struct DEVICE_STATUS_TYPE), desip);
    //if (Get_Debug(DEV_DEBUG_MSG))
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("��IP:%d.%d.%d.%d����״̬", *desip, *(desip + 1), *(desip + 2), *(desip + 3));
    IO_FlagLingt_Flash();
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
uint32 ON_Change_State(struct BASE_MSG *pmsg)
{
    uint8 flag = FALSE;
    if (pmsg->device == MSG_DEV_PC)
    {
        switch(pmsg->option)
        {
        case MSG_OPTION_OPEN_OVER:
            gDeviceStatus.DEV_Link_State |= DEV_LINK_PCON;
            flag = TRUE;
            break;
        case MSG_OPTION_POWERCLOSE:
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_PCON;
            flag = TRUE;
            break;
        case MSG_OPTION_DEV_LINK:
            gDeviceStatus.DEV_Link_State |= DEV_LINK_PCOK;
            flag = TRUE;
            break;
        case MSG_OPTION_DEV_BREAK:
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_PCLK;
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_PCOK;
            flag = TRUE;
            break;
        }
    }
    else if (pmsg->device == MSG_DEV_LMT)
    {
        switch(pmsg->option)
        {
        case MSG_OPTION_OPEN_OVER:
            gDeviceStatus.DEV_Link_State |= DEV_LINK_LMTON;
            flag = TRUE;
            break;
        case MSG_OPTION_POWERCLOSE:
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_LMTON;
            flag = TRUE;
            break;
        case MSG_OPTION_DEV_LINK:
            gDeviceStatus.DEV_Link_State |= DEV_LINK_LMTLK;
            flag = TRUE;
            break;
        case MSG_OPTION_DEV_BREAK:
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_LMTLK;
            flag = TRUE;
            break;
        }
    }
    else if (pmsg->device == MSG_DEV_KB)
    {
        if (pmsg->option == MSG_OPTION_DEV_LINK)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_KBLK;
            flag = TRUE;
        }
        else if (pmsg->option == MSG_OPTION_DEV_BREAK)
        {
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_KBLK;
            flag = TRUE;
        }
    }
    else if (pmsg->device == MSG_DEV_TRACK)
    {
        if (pmsg->option == MSG_OPTION_DEV_LINK)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_BJCardLK;
            flag = TRUE;
        }
        else if (pmsg->option == MSG_OPTION_DEV_BREAK)
        {
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_BJCardLK;
            flag = TRUE;
        }
    }
    else if (pmsg->device == MSG_DEV_TB)
    {
        if (pmsg->option == MSG_OPTION_DEV_LINK)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_TBLK;
            flag = TRUE;
        }
        else if (pmsg->option == MSG_OPTION_DEV_BREAK)
        {
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_TBLK;
            flag = TRUE;
        }
    }
    else if (pmsg->device == MSG_DEV_VIDEOCARD)
    {
        if (pmsg->option == MSG_OPTION_DEV_LINK)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_BM1LK;
            flag = TRUE;
        }
        else if (pmsg->option == MSG_OPTION_DEV_BREAK)
        {
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_BM1LK;
            flag = TRUE;
        }
    }
    else if (pmsg->device == MSG_DEV_KZYY)
    {
        if (pmsg->option == MSG_OPTION_DEV_LINK)
        {
            gDeviceStatus.DEV_Link_State |= DEV_LINK_YYLK;
            flag = TRUE;
        }
        else if (pmsg->option == MSG_OPTION_DEV_BREAK)
        {
            gDeviceStatus.DEV_Link_State &= ~DEV_LINK_YYLK;
            flag = TRUE;
        }
    }
    flag = flag;
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_RECORD(void *pdata)
{
    //uint8 buf[10];
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
        return 0;
    switch(pmsg->option)
    {
    //¼�Ʋ���
    case MSG_OPTION_RECORD_START:	//0x40  //¼��ʼ
        gDeviceStatus.CLASS_state &= 0x0f;
        gDeviceStatus.CLASS_state |= DEV_CLASS_RECSTART;
        KEY_DATA.key_rep |= KEY_REP_REC_PUSH;
        LED_Set(LED_REC, LED_DISPLAY_ON);
        LED_Set(LED_REC_STOP, LED_DISPLAY_OFF); //2014.7.24
        if (Check_FunctionMode(Device_FunctionMode_LB) == TRUE)
            LED_Set(LED_KZB, LED_DISPLAY_OFF);
        //DWQ_Contrl(1);//�򿪳�������λ��
        /////////////////////////2011.3.2 test by liyang/////////////////
        /*BYTE ip[4] = {0,0,0,0};
        BYTE *des_ip = ip;
        Sound_Close(NULL,SOUND_OPTION_ALL);
        Sound_Open(des_ip,AUDIO_PORT,1,SOUND_CONTRL_SEND);
                ////////////////////////////////
        BYTE pcontrol = 1;
        BYTE blank_buf[512],new_buf[512];
        memset(blank_buf, 0xb, 512);
        memset(new_buf, 0, 512);
        BYTE *buff1 = blank_buf;
        BYTE *buff2 = new_buf;
        for(int i=0;i<10000;i++)
        {
          Sound_Save_PlayData(buff1, &pcontrol, 1);
          Sound_Save_PlayData(buff2, &pcontrol, 1);
        }
        Sound_Close(NULL,SOUND_OPTION_ALL); */
        ////////////////////////////////////////////////
        ////////////////////////////////////////////////////
        break;
    case MSG_OPTION_RECORD_PAUSE:	//0x41 //¼����ͣ
        gDeviceStatus.CLASS_state &= 0x0f;
        gDeviceStatus.CLASS_state |= DEV_CLASS_RECPUSH;
        KEY_DATA.key_rep &= ~KEY_REP_REC_PUSH;
        LED_Set(LED_REC, LED_DISPLAY_FLASH);
        LED_Set(LED_REC_STOP, LED_DISPLAY_OFF); //2014.7.24
        if (Check_FunctionMode(Device_FunctionMode_LB) == TRUE)
        {
            LED_Set(LED_KZB, LED_DISPLAY_OFF);
        }
        break;
    case MSG_OPTION_RECORD_STOP:	//0x42 //¼��ֹͣ
        gDeviceStatus.CLASS_state &= 0x0f;
        gDeviceStatus.CLASS_state |= DEV_CLASS_RECSTOP;
        KEY_DATA.key_rep &= ~KEY_REP_REC_PUSH;
        LED_Set(LED_REC, LED_DISPLAY_OFF);
        LED_Set(LED_REC_STOP, LED_DISPLAY_ON); //2014.7.24
        if (Check_FunctionMode(Device_FunctionMode_LB) == TRUE)
        {
            LED_Set(LED_KZB, LED_DISPLAY_OFF);
        }
        //DWQ_Contrl(0);//�򿪳�������λ��
        break;
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_METRIC(void *pdata)
{
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));

    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
        return 0;
    if ((pmsg->option < MSG_OPTION_METRIC1) ||
            (pmsg->option > MSG_OPTION_METRIC3)) return 0;
    LED_Set(LED_METRIC_XGA, LED_DISPLAY_OFF);
    LED_Set(LED_METRIC_D1, LED_DISPLAY_OFF);
    LED_Set(LED_METRIC_ZDY, LED_DISPLAY_OFF);

    switch(pmsg->option)
    {
    case MSG_OPTION_METRIC1:	//0x46 //�ֱ���XGA
        gDeviceStatus.Current_REC_set &= 0xff0f;
        gDeviceStatus.Current_REC_set |= DEV_REC_LZVGA;
        LED_Set(LED_METRIC_XGA, LED_DISPLAY_ON);
        KEY_DATA.METRIC = MSG_OPTION_METRIC1;
        break;
    case MSG_OPTION_METRIC2:	//0x47 //�ֱ���D1
        gDeviceStatus.Current_REC_set &= 0xff0f;
        gDeviceStatus.Current_REC_set |= DEV_REC_LZD1;
        LED_Set(LED_METRIC_D1, LED_DISPLAY_ON);
        KEY_DATA.METRIC = MSG_OPTION_METRIC2;
        break;
    case MSG_OPTION_METRIC3:	//0x48 ////�ֱ����Զ���
        gDeviceStatus.Current_REC_set &= 0xff0f;
        gDeviceStatus.Current_REC_set |= DEV_REC_LZZDY;
        LED_Set(LED_METRIC_ZDY, LED_DISPLAY_ON);
        KEY_DATA.METRIC = MSG_OPTION_METRIC3;
        break;
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_FRAME(void *pdata)
{
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));

    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
        return 0;
    if ((pmsg->option < MSG_OPTION_FRAME1) ||
            (pmsg->option > MSG_OPTION_FRAME3)) return 0;
    LED_Set(LED_FRAME_CG, LED_DISPLAY_OFF);
    LED_Set(LED_FRAME_HZH, LED_DISPLAY_OFF);
    LED_Set(LED_FRAME_ZDY, LED_DISPLAY_OFF);
    switch(pmsg->option)
    {
    case MSG_OPTION_FRAME1:	//0x49//����
        gDeviceStatus.Current_REC_set &= 0xf0ff;
        gDeviceStatus.Current_REC_set |= DEV_REC_TXCG;
        KEY_DATA.FRAME = MSG_OPTION_FRAME1;
        LED_Set(LED_FRAME_CG, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_FRAME2:	//0x4a//���л�
        gDeviceStatus.Current_REC_set &= 0xf0ff;
        gDeviceStatus.Current_REC_set |= DEV_REC_TXHZH;
        KEY_DATA.FRAME = MSG_OPTION_FRAME2;
        LED_Set(LED_FRAME_HZH, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_FRAME3:	//0x4b //�Զ���
        gDeviceStatus.Current_REC_set &= 0xf0ff;
        gDeviceStatus.Current_REC_set |= DEV_REC_TXZDY;
        KEY_DATA.FRAME = MSG_OPTION_FRAME3;
        LED_Set(LED_FRAME_ZDY, LED_DISPLAY_ON);
        break;
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_CODE1_SPEED(void *pdata)
{
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));

    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
        return 0;
    if ((pmsg->option < MSG_OPTION_CODE1_SPEED1) ||
            (pmsg->option > MSG_OPTION_CODE1_SPEED3)) return 0;
    LED_Set(LED_SPEED_400, LED_DISPLAY_OFF);
    LED_Set(LED_SPEED_1M, LED_DISPLAY_OFF);
    LED_Set(LED_SPEED_ZDY, LED_DISPLAY_OFF);
    switch(pmsg->option)
    {
    case MSG_OPTION_CODE1_SPEED1:	//0x43 // ����1 500k
        gDeviceStatus.Current_REC_set &= 0xfff0;
        gDeviceStatus.Current_REC_set |= DEV_REC_ML500;
        KEY_DATA.CODE1_SPEED = MSG_OPTION_CODE1_SPEED1;
        LED_Set(LED_SPEED_400, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_CODE1_SPEED2:	//0x44// ����2 1M
        gDeviceStatus.Current_REC_set &= 0xfff0;
        gDeviceStatus.Current_REC_set |= DEV_REC_ML1M;
        KEY_DATA.CODE1_SPEED = MSG_OPTION_CODE1_SPEED2;
        LED_Set(LED_SPEED_1M, LED_DISPLAY_ON);

        break;
    case MSG_OPTION_CODE1_SPEED3:	//0x45 // ����1 �Զ���
        gDeviceStatus.Current_REC_set &= 0xfff0;
        gDeviceStatus.Current_REC_set |= DEV_REC_MLZDY;
        KEY_DATA.CODE1_SPEED = MSG_OPTION_CODE1_SPEED3;
        LED_Set(LED_SPEED_ZDY, LED_DISPLAY_ON);
        break;
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_PERSONAL_TALL(void *pdata)
{
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    if (gDeviceTable.Class.Flag & CLASS_TASK_FLAG_OVER)
        return 0;
    switch(pmsg->option)
    {
    //�������
    case MSG_OPTION_PERSONAL_TALL1:	//0x30  //����趨160
        gDeviceStatus.Current_REC_set &= 0x0fff;
        gDeviceStatus.Current_REC_set |= DEV_REC_SG160;
        KEY_DATA.PERSONAL_TALL = MSG_OPTION_PERSONAL_TALL1;
        LED_Set(LED_TAIL_170, LED_DISPLAY_OFF);
        LED_Set(LED_TAIL_180, LED_DISPLAY_OFF);
        LED_Set(LED_TAIL_160, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_PERSONAL_TALL2:	//0x31 //����趨170
        gDeviceStatus.Current_REC_set &= 0x0fff;
        gDeviceStatus.Current_REC_set |= DEV_REC_SG170;
        KEY_DATA.PERSONAL_TALL = MSG_OPTION_PERSONAL_TALL2;
        LED_Set(LED_TAIL_170, LED_DISPLAY_ON);
        LED_Set(LED_TAIL_180, LED_DISPLAY_OFF);
        LED_Set(LED_TAIL_160, LED_DISPLAY_OFF);
        break;
    case MSG_OPTION_PERSONAL_TALL3:	//0x32 //����趨180
        gDeviceStatus.Current_REC_set &= 0x0fff;
        gDeviceStatus.Current_REC_set |= DEV_REC_SG180;
        KEY_DATA.PERSONAL_TALL = MSG_OPTION_PERSONAL_TALL3;
        LED_Set(LED_TAIL_170, LED_DISPLAY_OFF);
        LED_Set(LED_TAIL_180, LED_DISPLAY_ON);
        LED_Set(LED_TAIL_160, LED_DISPLAY_OFF);

        break;
    }
    LED_SendFLASH();
    return STATUS_CHANGE;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_SELECT_SCAMERA(void *pdata)
{
    struct BASE_MSG *pmsg;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    if ((pmsg->option < MSG_OPTION_SELECT_SCAMERA1) ||
            (pmsg->option > MSG_OPTION_SELECT_SCAMERA6)) return 0;
    LED_Set(LED_SCAMERA1, LED_DISPLAY_OFF);
    LED_Set(LED_SCAMERA2, LED_DISPLAY_OFF);
    LED_Set(LED_SCAMERA3, LED_DISPLAY_OFF);
    LED_Set(LED_SCAMERA4, LED_DISPLAY_OFF);
    LED_Set(LED_SCAMERA5, LED_DISPLAY_OFF);
    LED_Set(LED_SCAMERA6, LED_DISPLAY_OFF);
    switch(pmsg->option)
    {
    case MSG_OPTION_SELECT_SCAMERA1:	//0x33 //�����ѡ��1
        gDeviceStatus.Curr_SCAME = 1;
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA1;
        LED_Set(LED_SCAMERA1, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_SELECT_SCAMERA2:	//0x34 //�����ѡ��2
        gDeviceStatus.Curr_SCAME = 2;
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA2;
        LED_Set(LED_SCAMERA2, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_SELECT_SCAMERA3:	//0x35//�����ѡ��3
        gDeviceStatus.Curr_SCAME = 3;
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA3;
        LED_Set(LED_SCAMERA3, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_SELECT_SCAMERA4:	//0x36//�����ѡ��4
        gDeviceStatus.Curr_SCAME = 4;
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA4;
        LED_Set(LED_SCAMERA4, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_SELECT_SCAMERA5:	//0x37//�����ѡ��5
        gDeviceStatus.Curr_SCAME = 5;
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA5;
        LED_Set(LED_SCAMERA5, LED_DISPLAY_ON);
        break;
    case MSG_OPTION_SELECT_SCAMERA6:	//0x38//�����ѡ��6
        KEY_DATA.CURR_SCAMERA = MSG_OPTION_SELECT_SCAMERA6;
        gDeviceStatus.Curr_SCAME = 6;
        LED_Set(LED_SCAMERA6, LED_DISPLAY_ON);
        break;
    }
    if (Get_Class_Stauts() != CLASS_OVER)
        LED_SendFLASH();
    return STATUS_CHANGE;
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_REC_ReqStatus(void *pdata)
{


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
uint32 On_MSG_OPTION_REC_Contrl(void *pdata)
{
    uint8 *p, tmpVGA, tmpAV, tmp;
    struct TBCard_RECNo_State_Type *pRecState;
    struct TB_RECSingle_ContrlType   RecCtrl;
    struct DEVICE_INFO *pdev;

    pRecState = Get_TB_REC_State();
    p = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    Mem_Copy((uint8 *)&RecCtrl, p, sizeof( struct TB_RECSingle_ContrlType));
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("¼�ƿ�������:%d,���:%d,ID:%d", RecCtrl.SingleType, RecCtrl.SingleNo, RecCtrl.SingleID);
    p += sizeof( struct TB_RECSingle_ContrlType);
    Mem_Copy((uint8 *)pRecState, p, sizeof( struct TBCard_RECNo_State_Type));
    pdev = (struct DEVICE_INFO *)Get_ClassInfo_Table(RecCtrl.SingleID);
    if ((pdev->dev_id != RecCtrl.SingleID) || (RecCtrl.SingleID < 16))
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("¼��ͨ������-�����ID%d", RecCtrl.SingleID);
        return 0;
    }
    if ((pdev->dev_type == 0xfe) || (pdev->dev_type < 16))
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("¼��ͨ������-������豸����%d", pdev->dev_type);
        return 0;
    }
    tmpVGA = pdev->VGA.VGAIn;
    tmpAV = pdev->VIDEO.VideoIn;
    if (RecCtrl.SingleType == TBCard_Option_VGA)
    {
        if (tmpVGA == DEV_VGA_OUT_NONE) return 0;
        TB_SelectVGA(TB_InputVGA_OUT_TB0, TB_OutputVGA_LMT);
        Device_VGA_Select(tmpVGA, DEV_VGA_OUT_TB0);
        TB_REC_UnFreez(TBCard_Option_VGA, RecCtrl.SingleNo, RecCtrl.SingleID);

    }
    else
    {
        if (tmpAV == DEV_VIDEO_NONE)	 return 0;
        TB_SelectAV(RecCtrl.SingleNo, RecCtrl.SingleNo);
        tmp = TB_SingleChage(TBCard_Option_AV, RecCtrl.SingleNo);
        Device_Video_Select(tmpAV, tmp);
        TB_REC_UnFreez(TBCard_Option_AV, RecCtrl.SingleNo, RecCtrl.SingleID);
    }
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
uint32 On_MSG_OPTION_LMT_ReqStatus(void *pdata)
{
    //struct BASE_MSG *pmsg;
    struct DEVICE_STATUS_TYPE *ps;

    uint8 *p, tmp;
    uint32 *pint;
    // pmsg = (struct BASE_MSG *)((uint8 *)pdata+sizeof(struct MSG_REV_TYPE));

    if((gDeviceStatus.CLASS_state & DEV_CLASS_START) != DEV_CLASS_START) return 0;
    p = (uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    tmp = *p;
    pint = (uint32 *)(p + 2);
    gDeviceStatus.Disk_size = *pint;
    gDeviceStatus.Current_REC = tmp;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    ps->Current_REC = tmp;
    LMT_Delay_Power();
    switch(tmp)
    {
    case LMT_STATUS_NONE:
        break;
    case LMT_STATUS_REC_Push:
    case LMT_STATUS_Pushing:
        Key_TB_state(KEY_REP_REC_PUSH, FALSE);
        LED_Set(LED_REC, LED_DISPLAY_FLASH);
        LED_Set(LED_REC_STOP, LED_DISPLAY_OFF); //add
        //DWQ_Contrl(1);//�򿪳�������λ��
        break;
    case LMT_STATUS_BackPlay:
        LED_Set(LED_KZB, LED_DISPLAY_OFF);
        Key_TB_state(KEY_REP_YREC, FALSE);
        break;
    case LMT_STATUS_RECing:
        LED_Set(LED_REC, LED_DISPLAY_ON);
        LED_Set(LED_REC_STOP, LED_DISPLAY_OFF); //add
        Key_TB_state(KEY_REP_REC_PUSH, TRUE);
        //DWQ_Contrl(1);//�򿪳�������λ��
        break;
    case LMT_STATUS_Stoping:
        Key_TB_state(KEY_REP_REC_PUSH, FALSE);
        LED_Set(LED_REC, LED_DISPLAY_FLASH);
        LED_Set(LED_REC_STOP, LED_DISPLAY_OFF); //add
        //DWQ_Contrl(1);//�򿪳�������λ��
        break;
    case LMT_STATUS_REC_Stop:
        Key_TB_state(KEY_REP_REC_PUSH, FALSE);
        LED_Set(LED_REC, LED_DISPLAY_OFF);
        LED_Set(LED_REC_STOP, LED_DISPLAY_ON); //add
        //DWQ_Contrl(0);//�򿪳�������λ��
        break;
    case LMT_STATUS_Exit:
        Key_TB_state(KEY_REP_REC_PUSH, FALSE);
        LED_Set(LED_REC, LED_DISPLAY_OFF);
        LED_Set(LED_REC_STOP, LED_DISPLAY_ON); //add
        //DWQ_Contrl(0);//�򿪳�������λ��
        break;
    case LMT_STATUS_Exiting:
        //DWQ_Contrl(0);//�򿪳�������λ��
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
uint32 On_MSG_OPTION_SCAMERA(void *pdata)
{

    BYTE *pBuf;
    BYTE data[10] = {0};
    BYTE hspeed, vspeed;
    struct BASE_MSG *pmsg;
    //struct DEVICE_INFO *pdev;
    //struct MSG_REV_TYPE *prev;

    uint8 com;//4/6 485 232��Ϊ���ں�
    uint8 portNum;
    uint16 baudRate = 2400;
    BYTE addr = 1;//����ӵ�ַ
    uint16 protocol = 3;//����Э��
    BYTE devid = 0;
    DWORD dwPos = 0;
    DWORD dwNetData = 0;
#if  Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    pBuf = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    memcpy(&dwNetData, pBuf + sizeof( struct BASE_MSG ) + 4, 4);
    memcpy(&dwPos, pBuf + sizeof( struct BASE_MSG ), 4);
    memcpy(&hspeed, pBuf + sizeof( struct BASE_MSG ) + 2, 1);
    memcpy(&vspeed, pBuf + sizeof( struct BASE_MSG ) + 3, 1);
    //IP_printf("���������");
    if (hspeed <= 0)
    {
        hspeed = 32;
    }
    if (hspeed > 64)
    {
        hspeed = 64;
    }
    if (vspeed <= 0)
    {
        vspeed = 32;
    }
    if (vspeed > 64)
    {
        vspeed = 64;
    }
    //gDeviceTable.Debug |= DEV_DEBUG_MSG;
#if 0   //2011��6��15���޸�
    if (0xa5a6a5a6 == dwNetData)//�Ǽ��ط�����������
    {
        pBuf = Get_ClassInfo_Table(0);
        devid = *(pBuf + 140 );//�������
        if (devid > 7 ) return 0;
        devid = Encode_TAB[devid];
        for (id = MSG_DEV_MAINBARD + 1; id < 254; id++)
        {
            pdev = (struct DEVICE_INFO *)Get_ClassInfo_Table(id);
            if (pdev->dev_type == MSG_DEV_VIDEOCARD)
            {
                if (pdev->VIDEO.VideoOut == devid) break;
            }
        }
        if (id >= 254)  return 0;
        devid = pdev->VIDEO.VideoLink;
        if ((devid < DEV_VIDEO_OUT_BM1_1) || (devid > DEV_VIDEO_OUT_BM2_4)) return 0;
        pdev = (struct DEVICE_INFO *)Device_Rec_Find(&devid, DEV_FIND_VIDEOIN);
        if (pdev->dev_type == 0xfe) return 0;
        pBuf = (uint8 *)pdev;
        if (NULL == pBuf)
            return 0;
    }
    else
    {
        hspeed = vspeed = 32;//�������緢�����ã����Ǽ��̷�����Ϣ���ٶȹ̶���32
        //��ԭ�����豸���Ͳ��Ҹ�Ϊ�����Ӷ˿����ò���
        devid = DEV_VIDEO_QSCAMERA1 + gDeviceStatus.Curr_SCAME - 1;
        pBuf = Device_Rec_Find(&devid, DEV_FIND_VIDEOIN);
        if (NULL == pBuf)
            return 0;
    }
#endif
    if (0xa5a6a5a6 == dwNetData)//�Ǽ��ط�����������
    {
        devid = DEV_VIDEO_QSCAMERA1 + gDeviceStatus.Curr_SCAME - 1;
        pBuf = Device_Rec_Find(&devid, DEV_FIND_VIDEOIN);
        if (NULL == pBuf)
            return 0;
    }
    else
    {
        hspeed = vspeed = 32;//�������緢�����ã����Ǽ��̷�����Ϣ���ٶȹ̶���32
        //��ԭ�����豸���Ͳ��Ҹ�Ϊ�����Ӷ˿����ò���
        devid = DEV_VIDEO_QSCAMERA1 + gDeviceStatus.Curr_SCAME - 1;
        pBuf = Device_Rec_Find(&devid, DEV_FIND_VIDEOIN);
        if (NULL == pBuf)
            return 0;
        memcpy(&protocol, pBuf + 94, 2);
        if(protocol == DEV_PROTOCOL_VISCA)
        {
            if(pmsg->option == MSG_OPTION_SCAMERA_ZOOMADD || pmsg->option == MSG_OPTION_SCAMERA_ZOOMSUB)
                hspeed = vspeed = 4;
            else
                hspeed = vspeed = 10;
        }
    }
    memcpy(&com, pBuf + 90, 1); ///
    portNum = com; //�¼�
    //IP_printf("devid=%d,com=%x",devid,com);
    memcpy(&addr, pBuf + 91, 1);
    memcpy(&baudRate, pBuf + 92, 2);
    memcpy(&protocol, pBuf + 94, 2);
    if ((baudRate != 2400) && (baudRate != 4800) && (baudRate != 9600) && (baudRate != 19200) && (baudRate != 38400)) return 0;
    //baudRate+=0x10000*(*(pBuf+97));//��λ������
    if (XCOM_PORT[portNum].BPS != baudRate)
    {
        Xcom_set(portNum, baudRate, DEV_PROTOCOL_NONE, 0x03);
    }
    SetAddress( addr , protocol);
    if(ViscaCount == 0)
    {
        data[0] = 0x88;
        data[1] = 0x30;
        data[2] = 0x01;
        data[3] = 0xff;
        UART_Write(portNum, (uint8 *)&data[0], 4);
        ViscaCount++;
    }
    switch(pmsg->option)
    {
    case MSG_OPTION_SCAMERA_ZOOMADD:	//0x39//�����ZOOM+
        ZoomInCmd( &data[0], vspeed, protocol);
        break;
    case MSG_OPTION_SCAMERA_ZOOMSUB:	//0x3a//�����ZOOM-
        ZoomOutCmd( &data[0], vspeed, protocol);
        break;
    case MSG_OPTION_SCAMERA_UP:			//0x3b//�������
        TurnUpCmd( &data[0], vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_DOWN:		//0x3c//�������
        TurnDownCmd( &data[0], vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_LIFT:		//0x3d//�������
        TurnLeftCmd( &data[0], hspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_RIGHT:		//0x3e//�������
        TurnRightCmd( &data[0], hspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_STOP:
        StopCmd(portNum, &data[0] , protocol);
        if(protocol == DEV_PROTOCOL_VISCA)
            return 0;
        break;
    case MSG_OPTION_SCAMERA_LEFTUP:
        TurnLeftUpCmd( &data[0], hspeed, vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_LEFTDOWN:
        TurnLeftDownCmd( &data[0], hspeed, vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_RIGHTUP:
        TurnRightUpCmd( &data[0], hspeed, vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_RIGHTDOWN:
        TurnRightDownCmd( &data[0], hspeed, vspeed , protocol);
        break;
    case MSG_OPTION_SCAMERA_FOCUSNEAR:
        FocusNearCmd(  &data[0] , protocol);
        break;
    case MSG_OPTION_SCAMERA_FOCUSFAR:
        FocusFarCmd(  &data[0]  , protocol);
        break;
    case MSG_OPTION_SCAMERA_PREPOSITION:
        GotoPrepositionCmd( &data[0], (BYTE)dwPos , protocol);
        break;
    case MSG_OPTION_SCAMERA_SETPREPOSITION:
        SetPrepositionCmd(  &data[0], (BYTE)dwPos , protocol);
        break;
    }
    if(protocol == DEV_PROTOCOL_PD)
    {
        UART_Write(portNum, (uint8 *)&data[0], PELCOD_LENGTH );
    }
    else
    {
        UART_Write(portNum, (uint8 *)&data[0], VISCA_LENGTH );
    }
    if (Get_Debug(DEV_DEBUG_COMDATE))
        IP_printf("COM%d�ڷ���Э��%x(3,P-D/5,VISCA),%x %x %x %x %x %x %x %x %x", portNum, protocol, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8]);
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
uint32 On_MSG_OPTION_SOUND_SELECT(void *pdata)
{

    BYTE *p;
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);

    Device_Sound_Select(*(p + 2));

    return 0;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ������豸����
//	�������ܣ������豸����
//	��ڲ�����*(pdata)=(struct MSG_REV_TYPE)����·��8�ֽ�
//			             *(pdata+8)=(struct BASE_MSG)�豸��+������(2�ֽ�)
//			             *(pdata+8+2)=Vol����
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_SOUND_VOL(void *pdata)
{
    struct BASE_MSG *pmsg;
    BYTE *p, Vol;
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    Vol = *(p + 2);
    switch(pmsg->device)
    {
    case MSG_DEV_GF:
        Device_OutVol(Vol, DEV_SOUND_OUT_GF);
        //Vol=DEVICE_LINK.Sound_Out[DEV_SOUND_OUT_LMT].OUT_Vol;
        //Device_OutVol(Vol, DEV_SOUND_OUT_LMT);

        break;
    case MSG_DEV_LMT:
        Device_OutVol(Vol, DEV_SOUND_OUT_LMT);
        break;
    }
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
uint32 On_MSG_OPTION_VIDEO_SELECT(void *pdata)
{
    struct BASE_MSG *pmsg;
    BYTE *p;
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    if (pmsg->device == MSG_DEV_TB)
        TB_SelectAV(*(p + 2), *(p + 3));
    else
    {
        if (*(p + 4) == 0x55)
        {
            Device_Video_Off(*(p + 3));
            IP_printf("Video�źŹر�");
        }
        else
            Device_Video_Select(*(p + 2), *(p + 3));
    }
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
uint32 On_MSG_OPTION_VGA_SELECT(void *pdata)
{
    BYTE *p;
    struct BASE_MSG *pmsg;
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct MSG_REV_TYPE));
    if (pmsg->device == MSG_DEV_TB)
        TB_SelectVGA(*(p + 2), *(p + 3));
    else
    {
        if (*(p + 4) == 0x55)
        {
            Device_VGA_Off(*(p + 3));
            IP_printf("VGA�źŹر�");
        }
        else
            Device_VGA_Select(*(p + 2), *(p + 3));
    }
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
uint32 On_MSG_OPTION_ARLM(void *pdata)
{
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
uint32 On_MSG_OPTION_IO_OUT(void *pdata)
{
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
uint32 On_MSG_OPTION_JT(void *pdata)
{
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
uint32 On_MSG_OPTION_TEST_IO(void *pdata)
{
    /*
    // BYTE *p;
    // struct BASE_MSG *pmsg;
    // p=(BYTE *)pdata+sizeof(struct MSG_REV_TYPE);
    // pmsg=(struct BASE_MSG *)((uint8 *)pdata+sizeof(struct MSG_REV_TYPE));
    // if(pmsg->device==MSG_DEV_ALM) //36
    // {
    if (IO_in(IN_ALARM1)){
    	IO_Out(OUT_ALARM1,FALSE);
    }
    else{
    	IO_Out(OUT_ALARM1,TRUE);
    	if (Get_Debug(DEV_DEBUG_MSG))
    	 	IP_printf("��������1,���1:��Ч");
    }

    if (IO_in(IN_ALARM2)){
    	if (Get_Debug(DEV_DEBUG_MSG))
    	 	IO_Out(OUT_ALARM2,FALSE);
     	//IP_printf("��������1,���1:��Ч");
    }
    else{
     	IO_Out(OUT_ALARM2,TRUE);
    	if (Get_Debug(DEV_DEBUG_MSG))
    	 	IP_printf("��������2,���2:��Ч");
    }

    if (IO_in(IN_ALARM3)){
     	IO_Out(OUT_ALARM3,FALSE);
     	//IP_printf("��������1,���1:��Ч");
    }
    else{
     	IO_Out(OUT_ALARM3,TRUE);
    	if (Get_Debug(DEV_DEBUG_MSG))
    	 	IP_printf("��������3,���3:��Ч");
    }

    if (IO_in(IN_ALARM4)){
     	IO_Out(OUT_DCS,FALSE);
     	//IP_printf("��������1,���1:��Ч");
    }
    else{
     	IO_Out(OUT_DCS,TRUE);
    	if (Get_Debug(DEV_DEBUG_MSG))
    	 	IP_printf("��������4,��������:��Ч");
    }

    if (IO_in(IN_ALARM5)){
     	//IO_Out(OUT_DCS,FALSE);
     	//IP_printf("��������1,���1:��Ч");
    }
    else{
     	IO_Out(OUT_ALARM1,TRUE);
     	//IO_Out(OUT_ALARM2,TRUE);
     	if (Get_Debug(DEV_DEBUG_MSG))
    	 	IP_printf("��������5,���1:��Ч");
    }

    if (IO_in(IN_ALARM6)){
     	//IO_Out(OUT_DCS,FALSE);
     	//IP_printf("��������1,���1:��Ч");
    }
    else{
     	//IO_Out(OUT_ALARM3,TRUE);
     	IO_Out(OUT_ALARM2,TRUE);
    	if (Get_Debug(DEV_DEBUG_MSG))
    	 	IP_printf("��������6,���2:��Ч");
    }

    if (IO_in(IN_PC1POWER)){
     	IO_Out(OUT_PC1POWER,FALSE);
     	//IP_printf("��������1,���1:��Ч");
    }
     else{
     	IO_Out(OUT_PC1POWER,TRUE);
    	if (Get_Debug(DEV_DEBUG_MSG))
    	 	IP_printf("�����1���,������Ч");
    }

    if (IO_in(IN_PC2POWER)){
     	IO_Out(OUT_PC2POWER,FALSE);
     	//IP_printf("��������1,���1:��Ч");
    }
    else{
     	IO_Out(OUT_PC2POWER,TRUE);
    	if (Get_Debug(DEV_DEBUG_MSG))
    	 	IP_printf("�����2���,������Ч");
    }
    */
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
uint32 On_MSG_OPTION_IO_TEST(void *pdata)
{
    //uint8 tyjflag=0,tmp;
    //uint8 gTestIOFlag=0;
    /*
    	if (IO_in(IN_ALARM1))gTestIOFlag|=TEST_ALARM1;
    	else	gTestIOFlag&=~TEST_ALARM1;
    	if (IO_in(IN_ALARM2))gTestIOFlag|=TEST_ALARM2;
    	else	gTestIOFlag&=~TEST_ALARM2;
    	if (IO_in(IN_ALARM3))gTestIOFlag|=TEST_ALARM3;
    	else	gTestIOFlag&=~TEST_ALARM3;
    	if (IO_in(IN_ALARM4))gTestIOFlag|=TEST_ALARM4;
    	else	gTestIOFlag&=~TEST_ALARM4;
    	if (IO_in(IN_ALARM5))gTestIOFlag|=TEST_ALARM5;
    	else	gTestIOFlag&=~TEST_ALARM5;
    	if (IO_in(IN_ALARM6))gTestIOFlag|=TEST_ALARM6;
    	else	gTestIOFlag&=~TEST_ALARM6;
    	if (IO_in(IN_PC1POWER))gTestIOFlag|=TEST_PC1POWER;
    	else	gTestIOFlag&=TEST_PC1POWER;
    	if (IO_in(IN_PC2POWER))gTestIOFlag|=TEST_PC2POWER;
    	else	gTestIOFlag&=TEST_PC2POWER;

    	if (IO_in(IN_TYJVGA))tyjflag=0;
    	else	tyjflag=1;

    	IP_printf("��������ڲ���:0x%2x,ͶӰ������%d",gTestIOFlag,tyjflag);
    	PINSEL3=0x00f00005;
    	for(tmp=0;tmp<5;tmp++)
    	{

    	IO_Out(OUT_HW,FALSE);// �����
    	IO_Out(OUT_ALARM1,TRUE);
    	IO_Out(OUT_ALARM2,TRUE);
    	IO_Out(OUT_ALARM3,TRUE);
    	IO_Out(OUT_DCS,TRUE);
    	IO_Out(OUT_PC1POWER,TRUE);
    	IO_Out(OUT_PC2POWER,TRUE);

    	OSTimeDly(50);
    	IO_Out(OUT_HW,TRUE);// �����
    	IO_Out(OUT_ALARM1,FALSE);
    	IO_Out(OUT_ALARM2,FALSE);
    	IO_Out(OUT_ALARM3,FALSE);
    	IO_Out(OUT_DCS,FALSE);
    	IO_Out(OUT_PC1POWER,FALSE);
    	IO_Out(OUT_PC2POWER,FALSE);
    	OSTimeDly(50);

    	}
    	PINSEL3=0x00fc0005;

    */
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

uint32 On_MSG_OPTION_LEDContrl(void *pdata)
{
    BYTE *p;
    uint32	leddata = 0;
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    leddata = (*(p + 2) << 24) + (*(p + 3) << 16) + (*(p + 4) << 8) + (*(p + 5));
    LED_Set(leddata, *(p + 6));
    LED_SendFLASH();
    return 0;
}

void Erase_Table(void)
{
    //uint8 *p;
    uint32 resault;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif

    OS_ENTER_CRITICAL();
    InitIAP();
    resault = SelSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
    resault = EraseSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
    resault = BlankCHK(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
    OS_EXIT_CRITICAL();
    UART_printf("Erase_Table\n\t");

}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ� ����ʹ��
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
extern struct LED_TYPE LED_DATA;
extern NetP NetPort;
uint32 On_MSG_OPTION_Debug(void *pdata)
{
    uint8 p;
    uint8	buf[16] = 0;

    p = *((uint8 *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG));
    IP_printf("10 b5 p:%x \n", p);
    IP_printf("p=\n (0x01: ARP) \n (0x02: Erase config) \n (0x03: bootload) \n (0x04: LED) \n");

    switch( p )
    {
    case 0x01://��ӡARP�б�
        ARP_List();
        break;
    case 0x02://����������Ϣ
        ClassInfo_RESET();
        break;
    case 0x03:
        IP_printf("�ָ�bootload");
        AT24C16_Write(buf, PROGRAM_FLAG_ADDRESS, 16);
        Reset(pdata);
        //Erase_Table();
        break;
    case 0x04:
        IP_printf("LED_date:%x %x %x %x", LED_DATA.LED_data[0], LED_DATA.LED_data[1], LED_DATA.LED_data[2], LED_DATA.LED_data[3]);
        IP_printf("LED_type:%x %x %x %x", LED_DATA.LED_type[0], LED_DATA.LED_type[1], LED_DATA.LED_type[2], LED_DATA.LED_type[3]);
        break;
    case 0x05:
      IP_printf("PC_IP:%d.%d.%d.%d",NetPort.PC_Ip[0],NetPort.PC_Ip[1],NetPort.PC_Ip[2],NetPort.PC_Ip[3]);
      IP_printf("Ser_Ip:%d.%d.%d.%d",NetPort.Ser_Ip[0],NetPort.Ser_Ip[1],NetPort.Ser_Ip[2],NetPort.Ser_Ip[3]);
    default:
        break;
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ��¼�8168������ɣ�����״̬���пأ��ı�ԭ�༭��״̬
//                ���Խ���¼�Ʋ���
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint8  State_8168 = FALSE; //TRUE;
uint32 On_MSG_8168_STATUS(void)
{
    //struct DEVICE_STATUS_TYPE *ps;

    //ps=(struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    //ps->DEV_Link_State|=DEV_LINK_BJCardLK;
    gDeviceStatus.DEV_Link_State |= DEV_LINK_BJCardLK;
    if(State_8168 == FALSE)
    {
        if((gDeviceStatus.CLASS_state & DEV_CLASS_START) == DEV_CLASS_START)
        {
            switch(gDeviceStatus.CLASS_state & 0xf0)
            {
            case DEV_CLASS_RECSTART:
                LED_Set(LED_REC, LED_DISPLAY_ON);
                LED_Set(LED_REC_STOP, LED_DISPLAY_OFF); //add
                break;
            case DEV_CLASS_RECPUSH:
                LED_Set(LED_REC, LED_DISPLAY_FLASH);
                LED_Set(LED_REC_STOP, LED_DISPLAY_OFF); //add
                break;
            case DEV_CLASS_RECSTOP:
                LED_Set(LED_REC, LED_DISPLAY_OFF);
                LED_Set(LED_REC_STOP, LED_DISPLAY_ON); //add
                break;
            default:
                break;
            }
            LED_SendFLASH();
            State_8168 = TRUE;
            IP_printf("8168�Ѿ�����,�ָ����״̬");
            return STATUS_CHANGE;
        }
    }
}

void Change_State_BJCardLK(void)
{
    if((gDeviceStatus.CLASS_state & DEV_CLASS_START) == DEV_CLASS_START)
    {

        LED_Set(LED_REC, LED_DISPLAY_FLASH); //8168δ���ӣ�¼�Ƶ���˸������
        LED_SendFLASH();
        IP_printf("8168δ����,����");
    }
    gDeviceStatus.DEV_Link_State &= ~DEV_LINK_BJCardLK;
    State_8168 = FALSE;

}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_YY_Teather(void *pdata)
{
    struct BASE_MSG *pmsg;
    uint8 *p;
    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE);
    pmsg = (struct BASE_MSG *)p;
    if (pmsg->option == MSG_OPTION_YY_Teather)
    {
        LED_Set(LED_KZA, LED_DISPLAY_ON);
        Key_TB_state(KEY_REP_JXZX, TRUE);
        LED_SendFLASH();
    }
    else
    {
        LED_Set(LED_KZA, LED_DISPLAY_OFF);
        Key_TB_state(KEY_REP_JXZX, FALSE);
        LED_SendFLASH();
    }
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
uint32 On_MSG_OPTION_TYJ_DIS_VGA(void *pdata)
{

    On_MSG_OPTION(pdata);
    return FALSE;

}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////

void On_MSG_OPTION_Ping(void  *pdata)
{
    struct gDevice_ADD *pDevice;
    uint8 *p;

    p = (BYTE *)pdata + sizeof(struct MSG_REV_TYPE) + sizeof(struct BASE_MSG);
    pDevice = (struct gDevice_ADD *)p;
    Ping_Precess(pDevice->IP);
    OSTimeDlyHMSM(0, 0, 0, 100);
    Ping_Precess(pDevice->IP);
    OSTimeDlyHMSM(0, 0, 0, 100);
    Ping_Precess(pDevice->IP);

}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void On_MSG_OPTION_Chk_DeviceState(void *pdata)
{

    extern struct gDevice_ADD Device_3730;
    extern struct gDevice_ADD Device_8168;

    if(Device_3730.State == FALSE || Device_8168.State == FALSE)
    {
        IP_printf("3730��8168����δͨ���豸�������ϵ�");
        POWER_Set(DEV_POWER_DEVICE, MSG_OPTION_POWERCLOSE);
        OSTimeDlyHMSM(0, 0, 2, 0);
        POWER_Set(DEV_POWER_DEVICE, MSG_OPTION_POWEROPEN);

    }
    Device_8168.State = FALSE;
    Device_3730.State = FALSE;

    //return FALSE;

}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint32 On_MSG_OPTION_CardContrl(void *pdata)
{
    uint8 buf[32], *p, tmp, *ip;
    struct BASE_MSG *pmsg;
    struct MSG_REV_TYPE Rev;

#if Debug_MSG_Log  >0
    Print_MSG_info(pdata);
#endif

    Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
    Rev.Type = DEV_XCOM_RF;
    //û������RF����
    if ( Check_FunctionMode(Device_FunctionMode_CARD) != 1) return     0;

    if (Get_Class_Stauts() != CLASS_OVER)
    {
        //�Ѿ��ǿ���״̬
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        Task_Rec_Del(buf, 2);
        return  0;
    }

    p = ((uint8 *)pdata + sizeof(struct MSG_REV_TYPE ));
    pmsg = (struct BASE_MSG *)p;
    switch(*(p + 2))
    {
    case RF_ATT_ERR://��֤δͨ��
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        Task_Rec_Del(buf, 2);
        //�Ѿ��¿Σ���ػ�
        if (Get_Class_Stauts() == CLASS_OVER) break;
        //������ʽ�������򲻹ر�
        if (Get_Class_Stauts() != CLASS_BEGIN_RF) break;
        //û������RF����
        if ( Check_FunctionMode(Device_FunctionMode_CARD) != 1) break;
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_OVER;
        Msg_send(buf, 3, (uint8 *)&Rev);
        IP_printf("RF��֤δͨ�� ������Ϣ������:%s", p + 3);
        break;
    case RF_ATT_REQ://��֤����
        //�Ѿ��Ͽ���ȡ����֤����
        if (Get_Class_Stauts() != CLASS_OVER) break;
        Mem_Set(buf, 0, 32);
        buf[0] = pmsg->device;
        buf[1] = pmsg->option;
        buf[2] = RF_ATT_REQ;
        for (tmp = 0; tmp < 28; tmp++)
        {
            buf[3 + tmp] = *(p + 3 + tmp);
            if (*(p + 2 + tmp) == 0) break;
        }
        ip = Get_DesIp(DEVICE_SUB_SER);
        if (ARP_Rec_Find(ip) == NULL)
        {
            Arp_Request(ip);
        }
        UDP_Send_CMD(buf, 32, ip);
        Mem_Clr(buf, 12);
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        buf[2] = 2;
        //�����м��뿪�������5��û���յ���֤�����Զ�����
        Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 5, buf, 3, 0, (uint8 *)&Rev);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("RF ������Ϣ������:%s", p + 3);
        break;
    case RF_ATT_OK: 		//��֤ͨ��
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        Task_Rec_Del(buf, 2);	//������ɾ����������
        //�Ѿ��Ͽ���ȡ��2�ο���
        if (Get_Class_Stauts() != CLASS_OVER) break;
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_BEGIN;
        buf[2] = 2;
        Msg_send(buf, 3, (uint8 *)&Rev);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("RF��֤ͨ�� ������Ϣ������:%s", p + 3);
        break;
    }
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
uint32 on_MSG_OPTION_TB_ReqStatus(void *pdata)
{

    TB_Read_Csingle();
    TB_Read_state();
    return 0;

}



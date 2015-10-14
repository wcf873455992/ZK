#include "includes.h"
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：一体机命令发送
//	入口参数：pdata:数据  lg=数据长度 CMD_TYPE=CMD_SENDER_ACK 响应
//			Device_Sub:发送设备的IP
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
const uint8 Lan_TestFlag[4] = {0x55, 0x5a, 0xaa, 0xa5};
void LAN_Test_Ack(uint8 *pdata, uint16 lg)
{
    struct LAN_TEST_DATA *ptest;
    struct _pkst ptu;

    ptest = (struct LAN_TEST_DATA *)pdata;

    if (IP_Compare((BYTE *)Lan_TestFlag, ptest->flag))
    {
        ptu.STPTR = NULL;
        ptu.length = lg;
        ptu.DAPTR = pdata;
        Udp_SendPacked(&ptu, ptest->sip, UDP_LAN_TEST_PORT);
        //Udp_SendPacked(&ptu,Get_DesIp(DEVICE_SUB_TEST),UDP_LAN_TEST_PORT);

    }
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：一体机命令发送
//	入口参数：pdata:数据  lg=数据长度 CMD_TYPE=CMD_SENDER_ACK 响应
//			Device_Sub:发送设备的IP
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void UDP_Send_CMD(BYTE *pdata, uint16 lg, uint8 *des_ip)
{
    struct  _pkst udata, udatah, *pst;
    struct COMM_XCOM  *pxcom;
    NetP *pnet;
    IC2CMD_Header hcmd;
    struct MSG_OPTION_COMM_TYPE *pc;
    struct MSG_REV_TYPE *prev;
    BYTE *p, *p2, rev_buf[20];
    //封装命令头
    pxcom = (struct COMM_XCOM *)Get_XCOM_Port(DEV_XCOM_UDP);
    p = Get_DesIp(DEVICE_SUB_CLASS); //一体机IP
    hcmd.byteFrameIndex = pxcom->frame_count++;
    hcmd.dwFromIP = IPByte_TO_Dword(p);
    hcmd.byteProtocolType = CMD_SENDER_AD; //cmd or ack
    hcmd.dwSyncHead = CMD_ABLE_ICII;
    hcmd.dwToIP = IPByte_TO_Dword(des_ip);
    hcmd.wDataLen = lg + 28;
    hcmd.Cmd.dwCommand = CMD_AD_SENDMSG;
    memset(hcmd.Cmd.cGUID, 0, 10);
    memcpy(hcmd.Cmd.cGUID, Get_ClassID(), 8);
    //封装数据链
    udatah.DAPTR = (BYTE *)&hcmd;
    udatah.length = sizeof(IC2CMD_Header);
    if (lg != 0)
    {
        udata.STPTR = NULL;
        udata.length = lg;
        udata.DAPTR = pdata,
              udatah.STPTR = &udata;
    }
    else
        udatah.STPTR = NULL;
    Udp_SendPacked(&udatah, des_ip, IC2_COMMAND_PORT);

    //发送协议数据
    pnet = (NetP *)Get_NetPort();
    if (IP_Compare(des_ip, pnet->CMD_IP) && (pnet->CMD_IP_TTL == 0)) return;
    if (IP_Compare(des_ip, pnet->Ser_Ip)) return;
    p = (uint8 *)User_mem_allco(sizeof(struct MSG_OPTION_COMM_TYPE) + lg + sizeof(IC2CMD_Header));
    if (p == NULL) return;

    pc = (struct MSG_OPTION_COMM_TYPE *)p;
    pc->msg.device = MSG_DEV_MAINBARD;
    pc->msg.option = MSG_OPTION_COMM;
    pc->Frame = hcmd.byteFrameIndex;
    pc->PortNum = DEV_XCOM_UDP;
    pc->lg = lg + sizeof(IC2CMD_Header);
    prev = (struct MSG_REV_TYPE *)rev_buf;
    prev->Type = DEV_XCOM_UDP;
    memcpy(prev->des_ip, des_ip, 4);
    p2 = p + sizeof(struct MSG_OPTION_COMM_TYPE);
    for (pst = &udatah; pst != NULL; pst = pst->STPTR)
    {
        memcpy(p2, pst->DAPTR, pst->length);
        p2 += pst->length;
    }
    Task_Run_Delay(REALTIME_CIIR_SEC * 256 + 1, p, pc->lg + sizeof(struct MSG_OPTION_COMM_TYPE), 2, rev_buf);
    User_mem_free(p);



    ///OSAPIBlockExit(pxcom->flag);

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void UDP_Send_ACK(uint8 *pdata)
{
    struct  _pkst udatah;
    //struct COMM_XCOM  *pxcom;
    IC2CMD_Header hcmd;
    BYTE *p, buf[4];
    //pxcom=(struct COMM_XCOM  *)Get_XCOM_Port(DEV_XCOM_UDP);
    //OSAPIBlockEnter(pxcom->flag,0);
    memcpy((BYTE *)&hcmd, pdata + sizeof( struct XCOM_REV_TYPE), sizeof(IC2CMD_Header));
    p = Get_DesIp(DEVICE_SUB_CLASS); //一体机IP
    hcmd.byteProtocolType = CMD_SENDER_ACK | CMD_SENDER_AD; //cmd or ack
    hcmd.dwToIP = hcmd.dwFromIP;
    hcmd.dwFromIP = IPByte_TO_Dword(p);
    IPDword_TO_Byte(buf, hcmd.dwToIP);
    hcmd.wDataLen = 28;
    hcmd.Cmd.dwCommand = CMD_AD_RECVMSG;
    memset(hcmd.Cmd.cGUID, 0, 10);
    memcpy(hcmd.Cmd.cGUID, Get_ClassID(), 8);
    udatah.DAPTR = (BYTE *)&hcmd;
    udatah.length = sizeof(IC2CMD_Header);
    udatah.STPTR = NULL;
    Udp_SendPacked(&udatah, buf, IC2_COMMAND_PORT);
    //OSAPIBlockExit(pxcom->flag);
}
////////////////////////////////////////////////////////////////////
//	函数名称：void UDP_msg_rev(uint8 *p)
//	函数功能：网络命令解析
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void UDP_msg_rev(uint8 *pdata)
{
    struct tagIC2CMD_Header cmd;
    NetP *pnet;
    BYTE buf[8], rev_buf[12];
    struct BASE_MSG *pmsg;
    struct MSG_REV_TYPE *prev;
    memcpy((BYTE *)&cmd, pdata + sizeof( struct XCOM_REV_TYPE), sizeof(struct tagIC2CMD_Header));
    pnet = (NetP *)Get_NetPort();
    if (cmd.dwSyncHead != CMD_ABLE_ICII) return;
    IPDword_TO_Byte(buf, cmd.dwToIP);
    if ((cmd.byteProtocolType & CMD_SENDER_ACK) == CMD_SENDER_ACK)
    {

        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_COMM;
        buf[2] = cmd.byteFrameIndex;
        buf[3] = DEV_XCOM_UDP;
        Task_Rec_Del(buf, 4); //删除消息列表中的重发消息
    }
    else
    {
        if (buf[3] == 0xff)
        {
            pmsg = (struct BASE_MSG *)(pdata + sizeof(struct XCOM_REV_TYPE) + sizeof(struct tagIC2CMD_Header));
            if (pmsg->option == MSG_OPTION_FILE_REQ) return;
        }
        UDP_Send_ACK(pdata);
        prev = (struct MSG_REV_TYPE *)rev_buf;
        prev->Type = DEV_XCOM_UDP;
        IPDword_TO_Byte(buf, cmd.dwFromIP);
        memcpy(prev->des_ip, buf, 4);
        /*
        pmsg=(struct BASE_MSG)pdata+sizeof(struct XCOM_REV_TYPE)+sizeof(struct tagIC2CMD_Header);
           p=(uint8 *)pmsg+2;
           if ((pmsg->device==MSG_DEV_MAINBARD)&&(pmsg->option==MSG_OPTION_CLASS_BEGIN))
           	{*p=4;
           	}
           if ((pmsg->device==MSG_DEV_MAINBARD)&&(pmsg->option==MSG_OPTION_CLASS_OVER))
           	{*p=0;
           	}
                      */
        Msg_send((uint8 *)pdata + sizeof(struct XCOM_REV_TYPE) + sizeof(struct tagIC2CMD_Header), cmd.wDataLen, rev_buf);
    }
    pnet = pnet;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 Check_UDP_Paket(void *pdata, uint16 lg)
{
    uint8 *p;
    struct COMM_XCOM *pxcom;
    struct tagIC2CMD_Header cmd;
    struct BASE_MSG *pmsg;
    p = (uint8 *)pdata;
    pxcom = (struct COMM_XCOM *)Get_XCOM_Port(DEV_XCOM_UDP);
    Mem_Copy((uint8 *)&cmd, p, sizeof( struct tagIC2CMD_Header));
    if (cmd.dwSyncHead != CMD_ABLE_ICII) return 0;
    if ((cmd.byteProtocolType & CMD_SENDER_ACK) == CMD_SENDER_ACK)
    {
        if ((pxcom->Rflag & XCOM_READ_FLAG_WAIT_ACK) && (pxcom->flag != NULL))
            OSSemPost(pxcom->flag);
    }
    else
    {
        if ((pxcom->Rflag & XCOM_READ_FLAG_WAIT_DATA) != XCOM_READ_FLAG_WAIT_DATA) return 0;
        if (pxcom->flag == NULL) return 0;
        pmsg = (struct BASE_MSG *)((uint8 *)pdata + sizeof(struct XCOM_REV_TYPE) + sizeof(struct tagIC2CMD_Header));
        if (pmsg->option != pxcom->RCMDNo) return 0;
        pxcom->Rdata_lg = lg - sizeof(struct tagIC2CMD_Header);
        Mem_Copy(pxcom->pbuf, p + sizeof(struct tagIC2CMD_Header), pxcom->Rdata_lg);
        OSSemPost(pxcom->flag);
        UDP_Send_ACK(pdata);
        return TRUE;
    }
    return FALSE;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 UDP_Read(BYTE *pdata, uint16 lg, uint8 *des_ip, uint8 WaitType, uint8 KeyWord)
{
    struct COMM_XCOM *pxcom;
    uint8 err, tmp;
    pxcom = ( struct COMM_XCOM *)Get_XCOM_Port(DEV_XCOM_UDP);
    if (WaitType == XCOM_READ_FLAG_WAIT_DATA)
    {
        if (pxcom->flag != NULL)  return 0;
        pxcom->RCMDNo = KeyWord;
        pxcom->pbuf = pdata;
        pxcom->Rflag = WaitType;
        UDP_Send_CMD(pdata, lg, des_ip);
        OSSemPend(pxcom->flag, 100, &err);
        if (err != OS_NO_ERR) return 0;
        pxcom->RCMDNo = 0;
        pxcom->pbuf = NULL;
        pxcom->Rflag = 0;
        tmp = pxcom->Rdata_lg;
        pxcom->Rdata_lg = 0;
        return tmp;
    }
    else if (WaitType == XCOM_READ_FLAG_WAIT_ACK)
    {
        UDP_Send_CMD(pdata, lg, des_ip);
        if (pxcom->flag == NULL) return FALSE;
        OSSemPend(pxcom->flag, 100, &err);
        pxcom->Rflag = 0;
        return err;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void K8_UDP_Send_ACK(const tagCommand *pcommand, eip *prec)
{
    tagCommand command;
    unsigned char tmp;
    struct _pkst SendData;

    memcpy(&command, pcommand, sizeof(tagCommand));
    tmp = command.receiver;
    command.receiver = command.sender;
    command.sender = tmp;
    command.extended[0] = EMACK;

    SendData.length = sizeof(tagCommand);
    SendData.STPTR = NULL;
    SendData.DAPTR = (uint8 *)&command;
    //IP_printf("ack S:%d R:%d C:%d F:%d E0:%d E1:%d",command.sender,command.receiver,command.command,command.flag,command.extended[0]);
    Udp_SendPacked(&SendData, prec->SourceIp, IC2_COMMAND_PORT_K8_PLUS);

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：处理
//	入口参数：void *pdata
//
//			MSG_OPTION_CLASS_BEGIN
//	出口参数：无
////////////////////////////////////////////////////////////////////
//void K8_UDP_Rec_Process( tagCommand *pcommand, eip *prec)
void K8_UDP_Rec_Process( void *pdata, eip *prec)
{
    uint8 buf[300] = {0}, rev_buf[80], n, *p;
    uint16 i;
    struct MSG_REV_TYPE  rec_type;
    struct SFILE_TRANSLATE_TYPE File_type;
    struct BASE_MSG Base_msg;
    tagCommand pcommand;
    
    p = (uint8 *)pdata;
    Mem_Copy((uint8 *)&pcommand, p, sizeof( tagCommand));   

    rec_type.Type = DEV_XCOM_UDP;
    rec_type.des_ip[0] = prec->SourceIp[0];
    rec_type.des_ip[1] = prec->SourceIp[1];
    rec_type.des_ip[2] = prec->SourceIp[2];
    rec_type.des_ip[3] = prec->SourceIp[3];

    memcpy(&rev_buf, &rec_type, sizeof(struct MSG_REV_TYPE));
    
    //memcpy(&buf[4], p + sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE), sizeof(tagCommand));
    

    Base_msg.device = MSG_DEV_LMT;

    //IP_printf("recive S:%x R:%x C:%x F:%x\n",pcommand->sender,pcommand->receiver,pcommand->command,pcommand->flag);
    //buf[0]=MSG_DEV_LMT;
    switch(pcommand.command)
    {
    case EMCOMMANDREMOTECONTROLLER:
        Base_msg.device = MSG_DEV_LAN_KB;
        Base_msg.option = MSG_OPTION_LAN_KEY;
        buf[2] = KEY_VAL;
        switch(pcommand.flag)
        {
        case EMRCACTIONPOWERON:
            buf[3] = KEY_CLASS_BEGIN;
            buf[4] = KEY_CLASS_BEGIN >> 8;
            break;
        case EMRCACTIONPOWEROFF:
            buf[3] = KEY_CLASS_BEGIN;
            buf[4] = KEY_CLASS_BEGIN >> 8;
            break;
        case EMRCACTIONRECORDSTART:
            buf[3] = KEY_REC_PUSH ;
            buf[4] = KEY_REC_PUSH >> 8;
            break;
        case EMRCACTIONRECORDPAUSE:
            buf[3] = KEY_REC_PUSH;
            buf[4] = KEY_REC_PUSH >> 8;
            break;
        case EMRCACTIONRECORDSTOP:
            buf[3] = KEY_REC_STOP;
            buf[4] = KEY_REC_STOP >> 8;
            break;
        case EMRCACTIONDEVPC:
            buf[3] = KEY_DEVICE_PC;
            buf[4] = KEY_DEVICE_PC >> 8;
            break;
        case EMRCACTIONDEVNPC:
            buf[3] = KEY_DEVICE_NPC;
            buf[4] = KEY_DEVICE_NPC >> 8;
            break;
        case EMRCACTIONDEVSHOW:
            buf[3] = KEY_DEVICE_SHOW;
            buf[4] = KEY_DEVICE_SHOW >> 8;
            break;
        }
        break;
    case EMCOMMANDHEARTBEAT:
        Base_msg.option = MSG_OPTION_REC_K8;
        break;
    case EMCOMMANDCOM1:
    case EMCOMMANDCOM2:
    case EMCOMMANDCOM3:
    case EMCOMMANDCOM4:
    case EMCOMMANDSETBAUDRATE:
        Base_msg.option = MSG_OPTION_SCAMERA_CONTRL;
        break;
    case EMCOMMANDRESET:
        Base_msg.option = MSG_OPTION_RESET;
        break;
    case EMCOMMANDFILEREQ:
        Base_msg.option = MSG_OPTION_FILE_REQ;

        File_type.FileType = FILE_TYPE_TABLE;
        File_type.FilePage = pcommand.flag;
        memcpy(&buf[2], &File_type, sizeof(struct SFILE_TRANSLATE_TYPE));
        break;
    case EMCOMMANDFILESEND:
        Base_msg.option = MSG_OPTION_FILE_SEND;
        File_type.FileType = FILE_TYPE_TABLE;
        File_type.FilePage = pcommand.flag;
        memcpy(&buf[2], &File_type, sizeof(struct SFILE_TRANSLATE_TYPE));
        break;
    default:
        Base_msg.option = MSG_OPTION_NONE;
        break;
    }
    //Msg_send(buf,sizeof(tagCommand)+4,rev_buf);
    memcpy(&buf[0], &Base_msg, sizeof(struct BASE_MSG));
    if(Base_msg.option == MSG_OPTION_FILE_SEND)
    {   
        n = sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE);
        memcpy(&buf[n], p + sizeof(tagCommand), 256);
        //IP_printf_256(&buf[n]);
        Msg_send(buf, sizeof(struct BASE_MSG) + sizeof(struct SFILE_TRANSLATE_TYPE) +256, rev_buf);
       
    }
    else
    {
        Msg_send(buf, 30, rev_buf);
    }

    //IP_printf("buf:%x %x %x %x %x %x %x %x %x %x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9]);
    

}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：处理
//	入口参数：
//
//			MSG_OPTION_CLASS_BEGIN
//	出口参数：无
////////////////////////////////////////////////////////////////////
void K8_UDP_Send_CMD(BYTE *pdata, uint16 lg, uint8 *des_ip)
{
    tagCommand  pcommand;
    struct BASE_MSG *keymsg;
    struct _pkst SendData;


    keymsg = (struct BASE_MSG *)pdata;
    
    
    switch(keymsg->option)
    {
    case MSG_OPTION_CLASS_BEGIN:
        pcommand.flag = EMRCACTIONPOWERON;
        break;
    case MSG_OPTION_CLASS_OVER:
        pcommand.flag = EMRCACTIONPOWEROFF;
        break;
    case MSG_OPTION_RECORD_START:
        pcommand.flag = EMRCACTIONRECORDSTART;
        break;
    case MSG_OPTION_RECORD_PAUSE:
        pcommand.flag = EMRCACTIONRECORDPAUSE;
        break;
    case MSG_OPTION_RECORD_STOP:
        pcommand.flag = EMRCACTIONRECORDSTOP;
        break;     
    default:
        pcommand.flag = EMRCACTION;
        break;
    }
    if(pcommand.flag == EMRCACTION)
    {
        switch(keymsg->device)
        {
        case MSG_DEV_PC:
            pcommand.flag = EMRCACTIONDEVPC;
            break;
        case MSG_DEV_NPC:
            pcommand.flag = EMRCACTIONDEVNPC;
            break;
        case MSG_DEV_SHOW:
            pcommand.flag = EMRCACTIONDEVSHOW;
            break;
        default:
            pcommand.flag = EMRCACTION;
            break;
        }
    }
    pcommand.sender = EMPARTICIPATORCONTROLBOARD;
    pcommand.receiver = EMPARTICIPATORDEVCONTROLLER;
    pcommand.command = EMCOMMANDREMOTECONTROLLER;
    
    if(keymsg->option == MSG_OPTION_FILE_REQ)
    {        
        pcommand.command = EMCOMMANDFILEREQ;
        pcommand.flag = *(pdata + 4);//请求页码
    
    }
    SendData.length = sizeof(tagCommand);
    SendData.STPTR = NULL;
    SendData.DAPTR = (uint8 *)&pcommand;
    Udp_SendPacked(&SendData, des_ip, IC2_COMMAND_PORT_K8_PLUS);
    //IP_printf("K8 Send C:%d F：%d,IP:%d,%d,%d,%d \n",pcommand.command,pcommand.flag,*(des_ip+0),*(des_ip+1),*(des_ip+2),*(des_ip+3));

}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：处理
//	入口参数：
//
//			MSG_OPTION_CLASS_BEGIN
//	出口参数：无
////////////////////////////////////////////////////////////////////
void K8_UDP_Send_File(BYTE *pdata, uint16 lg, uint8 *des_ip)
{
    tagCommand  pcommand;
    struct BASE_MSG *pmsg;
    struct _pkst SendData;
    uint8 buf[300]={0},i;
    
    pmsg = (struct BASE_MSG *)pdata;
    memset(&pcommand, 0, sizeof(tagCommand));
    pcommand.sender = EMPARTICIPATORCONTROLBOARD;
    pcommand.receiver = EMPARTICIPATORDEVCONTROLLER;
    
    
    
    switch(pmsg->option)
    {
    case MSG_OPTION_FILE_SEND:
        pcommand.command = EMCOMMANDFILESEND;
        pcommand.flag = *(pdata + 4);
        
        memcpy(&buf[0], &pcommand, sizeof(tagCommand));
        memcpy(&buf[16], (uint8 *)pdata + 6, 256);       
        SendData.length = sizeof(tagCommand) + lg - 6;
        SendData.STPTR = NULL;
        SendData.DAPTR = &buf[0];
        Udp_SendPacked(&SendData, des_ip, IC2_COMMAND_PORT_K8_PLUS);
        break;

    default:
        break;
    }


    //IP_printf("K8 Send C:%d F：%d,IP:%d,%d,%d,%d \n",pcommand.command,pcommand.flag,*(des_ip+0),*(des_ip+1),*(des_ip+2),*(des_ip+3));

}

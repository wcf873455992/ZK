#include "includes.h"
struct LMT_CONTRL_TYPE  gLMT_Power, gPC_power;
OS_EVENT *FRun;			//消息队列
/////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Config_printf(uint8 page_id)
{
    uint32 tmp;//,i;
    uint8 buf[40], *pdata, err;
    //struct BASE_MSG *pmsg;
    struct CLASS_INFO *pclass;
    //struct CLASS_OPION_TYPE *pCLASS_OPtion;
    //struct CLASS_TABLE_TYPE *pclass_table;
    //struct CLASS_TABLE_REC *pclass_Rec;
    //struct DEVICE_INFO *pdev;

    if (page_id == 0)
    {
        pclass = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
        IP_printf("学校名:%s", &pclass->school_name[0]);
        IP_printf("教学楼:%s", &pclass->house_name[0]);
        IP_printf("教室名:%s", &pclass->class_name[0]);
        IP_printf("教室类型:%d", pclass->class_type);
        tmp = Get_ver();
        IP_printf("一体机软件版本:%d.%d", tmp / 256, tmp & 0xff);
        Mem_Clr(buf, 10);
        Mem_Copy(buf, pclass->class_ID, 8);
        IP_printf("教室ID:%s", buf);
        IP_printf("本机IP=%d.%d.%d.%d", pclass->class_ip.IP[0], pclass->class_ip.IP[1], pclass->class_ip.IP[2], pclass->class_ip.IP[3]);
        IP_printf("本机IP MASK=%d.%d.%d.%d", pclass->class_ip.Mask[0], pclass->class_ip.Mask[1], pclass->class_ip.Mask[2], pclass->class_ip.Mask[3]);
        IP_printf("网关IP=%d.%d.%d.%d", pclass->Gateway_ip.IP[0], pclass->Gateway_ip.IP[1], pclass->Gateway_ip.IP[2], pclass->Gateway_ip.IP[3]);
        IP_printf("网关IP MASK=%d.%d.%d.%d", pclass->Gateway_ip.Mask[0], pclass->Gateway_ip.Mask[1], pclass->Gateway_ip.Mask[2], pclass->Gateway_ip.Mask[3]);
        IP_printf("集控IP=%d.%d.%d.%d", pclass->Ser_ip.IP[0], pclass->Ser_ip.IP[1], pclass->Ser_ip.IP[2], pclass->Ser_ip.IP[3]);
        IP_printf("集控IP MASK=%d.%d.%d.%d", pclass->Ser_ip.Mask[0], pclass->Ser_ip.Mask[1], pclass->Ser_ip.Mask[2], pclass->Ser_ip.Mask[3]);
        IP_printf("报警布防时间:%d时%d分", pclass->alm_on.hour, pclass->alm_on.min);
        IP_printf("报警撤防时间:%d时%d分", pclass->alm_off.hour, pclass->alm_off.min);
        IP_printf("当前监控录像机ID:%d", pclass->CurrsScameraPage);
        IP_printf("一体机开机类型:%d型(0手动1课表2定时)", pclass->OpenType);
        IP_printf("编辑卡IP=%d.%d.%d.%d", pclass->BJCardIP[0], pclass->BJCardIP[1], pclass->BJCardIP[2], pclass->BJCardIP[3]);
        IP_printf("编码卡1_ IP=%d.%d.%d.%d", pclass->BMCard1IP[0], pclass->BMCard1IP[1], pclass->BMCard1IP[2], pclass->BMCard1IP[3]);
        IP_printf("编码卡2_ IP=%d.%d.%d.%d", pclass->BMCard2IP[0], pclass->BMCard2IP[1], pclass->BMCard2IP[2], pclass->BMCard2IP[3]);
        IP_printf(" 流媒体 IP=%d.%d.%d.%d", pclass->LMT_IP[0], pclass->LMT_IP[1], pclass->LMT_IP[2], pclass->LMT_IP[3]);
        IP_printf(" 教师PC IP=%d.%d.%d.%d", pclass->PC_IP[0], pclass->PC_IP[1], pclass->PC_IP[2], pclass->PC_IP[3]);
        IP_printf("校园一卡通控制:%d =0无效=1 有效", pclass->Card_Contrl);
        IP_printf("语音卡控制:%d =0无效=1 有效", pclass->YuYinCtrl);

    }
    /*
    else
    	if (page_id==1)
    	{pdata=(uint8 *)Get_ClassInfo_Table(1);
    	 pmsg=(struct BASE_MSG *)(pdata+2);
    	 IP_printf("开关机序列");
    	 IP_printf("开关机序列-触发消息1:%d,%d",pmsg->device,pmsg->option);
    	 pmsg=(struct BASE_MSG *)(pdata+4);
    	 IP_printf("开关机序列-触发消息2:%d,%d",pmsg->device,pmsg->option);
    	 pmsg=(struct BASE_MSG *)(pdata+6);
    	 IP_printf("开关机序列-触发消息3:%d,%d",pmsg->device,pmsg->option);
    	 for (tmp=0;tmp<100;tmp++)
    	 	{ pCLASS_OPtion=(struct CLASS_OPION_TYPE *)(pdata+8+tmp*4);
    	         if ((pmsg->device==0)||(pmsg->option==0)) break;
    	          IP_printf("开关机序列-开关机消息%d:%d,%d延时:%d",tmp,pCLASS_OPtion->msg.device,pCLASS_OPtion->msg.option,pCLASS_OPtion->del_time);
    	 	}
    	}
    else
    	if ((page_id>=2)&&(page_id<=9))
    		{IP_printf("课表控制星期:%d",page_id);
    	         pclass_table=(struct CLASS_TABLE_TYPE *)User_mem_allco(255);
    		 if (pclass_table==NULL)
    		 	{IP_printf("申请内存失败!");
    		        return ;
    		 	}
    		do{
    		err = AT24C16_Read(pclass_table->Class_Table_Rec, CLASS_TABLE_ADDRESS+page_id*256, 256);
    		if (i++ > 5)
    			{User_mem_free(pclass_table);
    			IP_printf("读课表AT24C512内存失败!");
    			return ;
    		        }
    		}while(err == FALSE);
    		for (i=0;i<8;i++)
    			{
    			pclass_Rec=(struct CLASS_TABLE_REC *)(pclass_table->Class_Table_Rec+3+i*sizeof(struct CLASS_TABLE_REC));
    			if ((pclass_Rec->class_begin.hour==0)||(pclass_Rec->class_begin.hour>24)) break;
    			IP_printf("记录:%d 开机时间:%d:%d  关机时间:%d:%d",i,
    				pclass_Rec->class_begin.hour,pclass_Rec->class_begin.min,
    				pclass_Rec->class_over.hour,pclass_Rec->class_over.min);
    			Mem_Clr(buf,20);
    			Mem_Copy(buf,pclass_Rec->class_name,16);
    			IP_printf("课程名:%s",buf);
    			Mem_Clr(buf,20);
    			Mem_Copy(buf,pclass_Rec->teacher_name,10);
    			IP_printf("教师名:%s",buf);
    			}
    		}
    else
    	if(page_id==16)
    		{
    		IP_printf("打印设备配置信息");
    		for (i=MSG_DEV_MAINBARD; i<255; i++)
    		{
    			pdev = (struct DEVICE_INFO*)Get_ClassInfo_Table(i);
    			if (pdev==NULL) break;

    			IP_printf("设备配置信息 设备ID:%d设备类型:%d",pdev->dev_id,pdev->dev_type);
    			Mem_Clr(buf,40);
    			Mem_Copy(buf,pdev->dev_name,32);
    			IP_printf("设备名称:%s",buf);
    			Mem_Clr(buf,40);
    			Mem_Copy(buf,pdev->dev_type_name,32);
    			IP_printf("设备类型名称:%s",buf);
    			IP_printf("VGA配置IN:%d,OUT:%dLINK:%d",
    				pdev->VGA.VGAIn,pdev->VGA.VGAOut,pdev->VGA.VGALink);
    			IP_printf("Video配置IN:%d,OUT:%dLINK:%d",
    				pdev->VIDEO.VideoIn,pdev->VIDEO.VideoOut,pdev->VIDEO.VideoLink);
    			IP_printf("音频配置IN:%d,OUT:%dLINK:%d音量:%d",
    				pdev->SOUND.SoundIn,pdev->SOUND.SoundOut,pdev->SOUND.SoundLink,pdev->SOUND.SoundVol);

    			IP_printf("IO输入配置:输入号:%d,属性:%d音频报警号%d",
    				pdev->IN.in_no,pdev->IN.in_type,pdev->IN.in_sound);
    			IP_printf("IO输出配置:输出号:%d,属性:%d音频报警号%d",
    				pdev->OUT.out_no,pdev->OUT.out_type,pdev->OUT.io_sound);
    			IP_printf("电源配置--电源箱，%d端口号:%d",pdev->POWER.dev_id,pdev->POWER.power_no);
    			IP_printf("通讯配置--%d",pdev->COMM.comm_type);

    		}
    		}*/
}

////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void PC_init(uint16 Power_no)
{
    Mem_Set((uint8 *)&gPC_power, 0, sizeof(struct LMT_CONTRL_TYPE));
    gPC_power.status |= LMT_CONTRL_Set;
    gPC_power.power_no = Power_no;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 PC_Power_contrl(uint8 Device, uint8 option)
{

    if ((option != MSG_OPTION_POWEROPEN) &&
            (option != MSG_OPTION_POWERCLOSE)) return FALSE;
    if (Device != MSG_DEV_LMT) return FALSE;
    gPC_power.Device = Device;
    gPC_power.option = option;
    gLMT_Power.reptimes = 0;
    return TRUE;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void PC_Power_Check(void)
{
    //struct BASE_MSG msg;
    //uint8 *ip;
    if ((gPC_power.status & LMT_CONTRL_Set) == 0) return;

    ///////流媒体开机/////////////////////////////////////
    if ((gPC_power.option == MSG_OPTION_POWEROPEN) &&
            ((gPC_power.status & LMT_CONTRL_PWon) == 0))
    {
        gPC_power.status &= ~LMT_CONTRL_PWoff;
        if (IO_in(IN_PC1POWER) == FALSE) //流媒体已经开机
        {
            gPC_power.status |= LMT_CONTRL_PWon;
            gPC_power.status &= ~LMT_CONTRL_EXIT;
            return;
        }
        else
        {
            if (gPC_power.option_time++ < 10) return;
            gPC_power.option_time = 0;
            POWER_Set(gPC_power.power_no, gPC_power.option);
            IO_PC_Power(MSG_DEV_PC, gPC_power.option);
        }
    }
    ///////流媒体关机////////////////////////////////////
    if ((gPC_power.option == MSG_OPTION_POWERCLOSE) &&
            ((gPC_power.status & LMT_CONTRL_PWoff) == 0))
    {
        gPC_power.status &= ~LMT_CONTRL_PWon;

        if (IO_in(IN_PC1POWER))
        {
            gPC_power.status |= LMT_CONTRL_PWoff;
            POWER_Set(gPC_power.power_no, gPC_power.option);
            gPC_power.status &= ~LMT_CONTRL_EXIT;
            return;
        }
        else
        {
            if (gPC_power.option_time++ < 60) return;
            gPC_power.option_time = 0;
            IO_PC_Power(MSG_DEV_PC, gPC_power.option);
        }
    }

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void LMT_init(uint16 Power_no)
{
    Mem_Set((uint8 *)&gLMT_Power, 0, sizeof(struct LMT_CONTRL_TYPE));
    gLMT_Power.status |= LMT_CONTRL_Set;
    gLMT_Power.power_no = Power_no;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void LMT_Delay_Power(void)
{
    if (gLMT_Power.option != MSG_OPTION_POWERCLOSE) return;
    gLMT_Power.reptimes = 0;

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint8 LMT_Power_contrl(uint8 Device, uint8 option)
{

    if ((option != MSG_OPTION_POWEROPEN) &&
            (option != MSG_OPTION_POWERCLOSE)) return FALSE;
    if (Device != MSG_DEV_LMT) return FALSE;
    gLMT_Power.Device = Device;
    gLMT_Power.option = option;
    gLMT_Power.reptimes = 0;
    return TRUE;
}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void LMT_Power_Check(void)
{
    struct BASE_MSG msg;
    struct DEVICE_STATUS_TYPE *ps;
    //struct DEVICE_INFO *pdev;
    uint8 *ip;
    if (Check_FunctionMode(Device_FunctionMode_LB) == FALSE)  return;
    if ((gLMT_Power.status & LMT_CONTRL_Set) == 0) return;

    ///////流媒体开机/////////////////////////////////////
    if ((gLMT_Power.option == MSG_OPTION_POWEROPEN) &&
            ((gLMT_Power.status & LMT_CONTRL_PWon) == 0))
    {
        gLMT_Power.status &= ~LMT_CONTRL_PWoff;
        if (IO_in(IN_PC2POWER) == FALSE) //流媒体已经开机
        {
            gLMT_Power.status |= LMT_CONTRL_PWon;
            gLMT_Power.status &= ~LMT_CONTRL_EXIT;
            return;
        }
        else
        {
            if (gLMT_Power.option_time++ < 10) return;
            gLMT_Power.option_time = 0;
            POWER_Set(gLMT_Power.power_no, gLMT_Power.option);
            IO_PC_Power(MSG_DEV_LMT, gLMT_Power.option);
        }
    }
    ///////流媒体关机////////////////////////////////////
    if ((gLMT_Power.option == MSG_OPTION_POWERCLOSE) &&
            ((gLMT_Power.status & LMT_CONTRL_PWoff) == 0))
    {
        gLMT_Power.status &= ~LMT_CONTRL_PWon;
        ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
        if((ps->Current_REC == LMT_STATUS_Exit) || (ps->Current_REC == LMT_STATUS_NONE))
            gLMT_Power.status |= LMT_CONTRL_EXIT;

        if (IO_in(IN_PC2POWER))
        {
            gLMT_Power.status |= LMT_CONTRL_PWoff;
            POWER_Set(gLMT_Power.power_no, gLMT_Power.option);
            gLMT_Power.status &= ~LMT_CONTRL_EXIT;
            return;
        }
        else
        {
            if (gLMT_Power.option_time++ < 60) return;
            gLMT_Power.option_time = 0;
            if (gLMT_Power.status & LMT_CONTRL_EXIT)
            {
                IO_PC_Power(MSG_DEV_LMT, gLMT_Power.option);
            }
            else
            {
                if (gLMT_Power.reptimes++ < 2)
                {
                    msg.device = MSG_DEV_LMT;
                    msg.option = MSG_OPTION_LMT_ReqStatus;
                    ip = Get_DesIp(DEVICE_SUB_BJCard);
                    if (ARP_Rec_Find(ip) == NULL)
                    {
                        Arp_Request(ip);
                        return ;
                    }
                    UDP_Send_CMD((uint8 *)&msg, 2, ip);
                }
                else
                {
                    //超时60S,强行关机
                    IP_printf("超时60S,强行关机");
                    gLMT_Power.status |= LMT_CONTRL_EXIT;
                    gLMT_Power.reptimes = 0;

                }
            }
        }
    }

}
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void Run_Send_Flag(void)
{
    OSSemPost(FRun);

}
////////////////////////////////////////////////////////////////////
//	函数名称：ReadTemperature
//	函数功能：读取温度
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
uint16 ReadTemperature(void)
{

}
void IP_printf_256(uint8 *p)
{
    uint16 i;
    for(i = 0; i < 256; i = i + 8)
    {
        IP_printf("%x %x %x %x %x %x %x %x\n",
                  *(p + i), *(p + 1 + i), *(p + 2 + i), *(p + 3 + i), *(p + 4 + i), *(p + 5 + i), *(p + 6 + i), *(p + 7 + i));
    }
}

void TEST_AT24C16(void)
{
    uint8 pw[256], p1[256];
    uint8 *p, err1, err2;
    uint16 i, page;
    uint32 m;

    p = (BYTE *)User_mem_allco(500);

    for(i = 0; i < 256; i++)
    {
        pw[i] = i; //+0xf0;
    }
    for(page = 0; page < 8; page++)
    {
        err1 = AT24C16_Write(pw, page * 256, 256); //CLASS_TABLE_ADDRESS
        for(m = 0; m < 5000; m++);
        err2 = AT24C16_Read(p, page * 256, 256);
        IP_printf("page=%d,err1=%x,err2=%x", page, err1, err2);
        //IP_printf_256(p);
    }/*
    err1=AT24C16_Write(pw,CLASS_TABLE_ADDRESS,256);
    for(m=0;m<5000;m++);
    err2=AT24C16_Read(p,CLASS_TABLE_ADDRESS, 256);
    IP_printf("page=%d,err1=%x,err2=%x",page,err1,err2);
    IP_printf_256(p);
    */
    User_mem_free(p);
    //AT24C16_Write(pw,CLASS_TABLE_ADDRESS+page*256,256)==FALSE
    //AT24C16_Read(p,CLASS_TABLE_ADDRESS+page*256, 256)==FALSE
}

void readflash(void)
{
    uint8 *p, err1, err2;
    uint16 page;

    p = (BYTE *)User_mem_allco(500);
    for(page = 17; page < 23; page++)
    {
        memcpy(p, Get_ClassInfo_Table(page & 0xff), 256);
        //IP_printf_256(p);
    }
    User_mem_free(p);
}
void UART_TEST(void )
{
    uint8 uart1[4] = {0x01, 0x01, 0x01, 0x01};
    uint8 uart3[4] = {0x03, 0x03, 0x03, 0x03};
    uint8 uart5[4] = {0x05, 0x05, 0x05, 0x05};
    uint8 uart8[4] = {0x08, 0x08, 0x08, 0x08};
    uint8 uart9[4] = {0x09, 0x09, 0x09, 0x09};

    UART_Send_Data(DEV_XCOM_TYJ, uart1, 4);
    OSTimeDlyHMSM(0, 0, 1, 0);
    UART_Send_Data(DEV_XCOM_3, uart3, 4);
    OSTimeDlyHMSM(0, 0, 1, 0);
    UART_Send_Data(DEV_XCOM_5, uart5, 4);
    OSTimeDlyHMSM(0, 0, 1, 0);
    UART_Send_Data(DEV_XCOM_8, uart8, 4);
    OSTimeDlyHMSM(0, 0, 1, 0);
    UART_Send_Data(DEV_XCOM_9, uart9, 4);
    OSTimeDlyHMSM(0, 0, 1, 0);
}

void ClassInfo_RESET(void)
{
    struct CLASS_INFO *pinfo;
    struct DEVICE_STATUS_TYPE *ps;
    NetP *pnet;
    BYTE buf[16];
    uint32 resault;
    unsigned char def_classname[] = "卓越中控";
    unsigned char def_schoolname[] = "卓越电子有限公司";
    unsigned char def_housename[] = "设备记录空";
    uint8 def_ip[4] = {192, 168, 7, 101};
    uint8 def_mark[4] = {255, 255, 0, 0};
    uint8 def_gateip[4] = {192, 168, 0, 190};
#define LSZH_VER		0xb00// 版本号

#if OS_CRITICAL_METHOD == 3                  // * Allocate storage for CPU status register     * /
    OS_CPU_SR  cpu_sr;                       // * Prevent compiler warning                     * /
#endif
    pinfo = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
    pnet = (NetP *)Get_NetPort();
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    memset(buf, 0, 16);
    pnet->Class_ID_flag = User_ID_Read(buf, 0);
    memcpy(ps->DEV_ID, buf, 8);
    //if ((pinfo->dev_id!=0)||(pinfo->dev_type!=MSG_DEV_CLASSINFO))
    //{
    pinfo = (struct CLASS_INFO *)User_mem_allco(256);
    if (pinfo == NULL)
        return;
    pinfo->dev_id = 0;
    pinfo->dev_type = 0;
    strcpy((char *)pinfo->class_name, (char *)def_classname);
    strcpy((char *)pinfo->school_name, (char *)def_schoolname);
    strcpy((char *)pinfo->house_name, (char *)def_housename);
    pinfo->alm_off.hour = 20;
    pinfo->alm_off.min = 0;
    pinfo->alm_on.hour = 8;
    pinfo->alm_on.min = 0;
    memcpy((BYTE *)pinfo->class_ID, &buf[8], 8);
    memcpy((BYTE *)pinfo->class_ip.IP, (BYTE *)def_ip, 4);
    memcpy((BYTE *)pinfo->class_ip.Mask, (BYTE *)def_mark, 4);
    memcpy((BYTE *)pinfo->Gateway_ip.IP, (BYTE *)def_gateip, 4);
    OS_ENTER_CRITICAL();
    InitIAP();
    resault = SelSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
    resault = EraseSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
    resault = BlankCHK(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
    resault = SelSector(ARM_TABLE_SECTOR_START, ARM_TABLE_SECTOR_END);
    resault = RamToFlash(ARM_TABLE_ADDRESS, (uint32)pinfo, 256);
    OS_EXIT_CRITICAL();
    IP_printf("配置表被擦除,重新初始化,重启!");
    User_mem_free((void *)pinfo);
    pinfo = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
    //};

    memcpy((BYTE *)pnet->My_Ip, (BYTE *)pinfo->class_ip.IP, 4);
    memcpy((BYTE *)pnet->My_Ip_Mark, (BYTE *)pinfo->class_ip.Mask, 4);
    memcpy((BYTE *)pnet->My_Gateway, (BYTE *)pinfo->Gateway_ip.IP, 4);
    memcpy( (BYTE *)pnet->Ser_Ip, (BYTE *)pinfo->Ser_ip.IP, 4);
    memcpy( (BYTE *)pnet->BJ_CardIP, (BYTE *)pinfo->BJCardIP, 4);
    memcpy((BYTE *)pnet->Class_ID, (BYTE *)ps->DEV_ID, 8);
    memcpy(ps->configtime, (BYTE *)pinfo->configtime, 4);
    pnet->Class_ID_flag = TRUE;
    pnet->Gateway_IP_TTL = 0;
    pnet->My_Mac[0] = 0xa2; //0xa1
    pnet->My_Mac[1] = 0x52; //0x52
    pnet->My_Mac[2] = pnet->My_Ip[0];
    pnet->My_Mac[3] = pnet->My_Ip[1];
    pnet->My_Mac[4] = pnet->My_Ip[2];
    pnet->My_Mac[5] = pnet->My_Ip[3];
    //ps->ver=LSZH_VER;
    resault = resault;
    /**/
}/**/
////////////////////////////////////////////////////////////////////
//	函数名称：
//	函数功能：
//	入口参数：
//
//
//	出口参数：无
////////////////////////////////////////////////////////////////////
void TASK_RUN(void *pdata)
{
    uint8 err;//,*p;
    uint8 Scan_io_Time, ReadWD;
    uint32 flag = 0, tmp;
    uint16 Temperature;

    FRun = OSSemCreate(0);
    Mem_Set((uint8 *)&gLMT_Power, 0, sizeof(struct LMT_CONTRL_TYPE));
    Scan_io_Time = 0;
    ReadWD = 0;
    IO_FlagLingt_Flash();
    Device_Rec_Scan();
    IP_printf("TASK_RUN start");
    for (;;)
    {
        OSSemPend(FRun, 0, &err);
        if (err !=  OS_NO_ERR) continue;
        flag = 0;
        IS752_Scan();
        IS752_Scan2();
        LMT_Power_Check();
        //Device_DCS_Contrl();
        RF_CARD_Check();
        if(IO_in(UPDATE) == 0)
        {
            OSTimeDlyHMSM(0, 0, 1, 0);
            if(IO_in(UPDATE) == 0)
            {
                IP_printf("SW2");
                ClassInfo_RESET();
                //UART_Send_Data(DEV_XCOM_DYX,testuart,4);
                //POWER_Set(DEV_POWER_PC,MSG_OPTION_POWEROPEN);
                //TEST_AT24C16();
            }
        }
        else
        {
        }
        if(IO_in(LOAD_DEFAULT) == 0)
        {
            OSTimeDlyHMSM(0, 0, 1, 0);
            if(IO_in(LOAD_DEFAULT) == 0)
            {
                IP_printf("SW3");
                //ARP_List();
                UART_TEST();
                ARP_List();
                //LED_Set(LED_REC_STOP,LED_DISPLAY_ON);
                //TEST_AT24C16();
                //readflash();
            }

        }
        else
        {
        }

        Temperature = ReadTemperature();

        if (ReadWD++ > 60)
        {
            ReadWD = 0;
            //DW_Read_WD();
            //UDP_Send_CMD(buf, 16,ip);
            //TB_Read_Csingle(); 测试通道信号有效
            //TB_Read_state();通道信号连接
        }
        if (Scan_io_Time++ > 3)
        {
            // Device_Link_Scan();
            //Device_Vol_Scan();
            if (0 == IO_Scan()) //IO扫描
            {
                flag |= 0x00;
            }
            else
            {
                flag |= 0x01;
                Alarm_Link(); //报警联动
            }
            //状态改变
            if  (flag & STATUS_CHANGE)
                Send_State();
        }
    }
}




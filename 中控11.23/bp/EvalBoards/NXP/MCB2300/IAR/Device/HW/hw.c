#include  "includes.h"
struct DEV_HW_TYPE  HW_DEVICE;
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void On_MSG_OPTION_HW_END(void *pdata)
{

}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�void HW_XM_start(struct BASE_MSG *keymsg)
//	�������ܣ�����ѧ��
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void HW_XM_start(void *pdata)
{
    struct BASE_MSG *keymsg, *pmsg;
    struct MSG_REV_TYPE *prev;
    BYTE *p, *pcap, flag;
    uint16 tmp;
    uint32 Treg;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif
    prev = (struct MSG_REV_TYPE *)pdata;
    keymsg = (struct BASE_MSG *)pdata + sizeof(struct MSG_REV_TYPE);
    pmsg = (struct BASE_MSG *)((BYTE *)pdata + 2 + sizeof(struct MSG_REV_TYPE));
    HW_DEVICE.XM.cap0_time = 0;
    HW_DEVICE.XM.buf = (BYTE *)User_mem_allco(600);
    if (HW_DEVICE.XM.buf == NULL)
    {
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("����ѧ���ڴ�����ʧ��!");
        HW_DEVICE.flag = 0;
        return;
    }
    HW_DEVICE.XM.lg = 7;
    p = (BYTE *)HW_DEVICE.XM.buf;
    *p = MSG_DEV_MAINBARD;
    *(p + 1) = MSG_OPTION_HW_END;
    *(p + 2) = pmsg->device;
    *(p + 3) = pmsg->option;
    *(p + 4) = 0;
    *(p + 5) = 0;
    *(p + 6) = 0;
    T0TCR = 0;
    T0CTCR = 0; //��ʱģʽ
    T0PR = HW_PR_TIME10us; //Ԥ��Ƶ10us TC+1����
    T0TC = 0;
    T0MR0 = HW_TIME1MIN / 4; //  1/2���Ӷ�ʱ
    T0MCR = 1; //MAT0 �ж�
    T0CCR = 0x38; //��������ʱ�ж�
    T0TCR = 2; //��ʱ��0��λ
    T0IR = 0xff;
    T0TCR = 1; //��ʱ��0��ʱ
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("����ѧ���������밴ң����\n\n\n");

    OS_ENTER_CRITICAL();
    //����һ��
    do
    {
        flag = T0IR;
        T0IR = flag;
        Treg = T0TC;
        if ((flag & 0x01) || (Treg >= T0MR0))
        {
            //
            T0TCR = 0;
            *(p + 4) = 0x01;
            *(p + 5) = 0;
            *(p + 6) = 0;
            UDP_Send_CMD(p, HW_DEVICE.XM.lg, prev->des_ip);
            HW_DEVICE.flag = 0;
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("����ѧ�볬ʱ\n\n\n");
            User_mem_free(p);
            HW_DEVICE.XM.buf = NULL;
            HW_DEVICE.XM.cap0_time = 0;
            OS_EXIT_CRITICAL();
            return;
        }
        else if (flag & 0x20)
        {
            T0MR0 = HW_TIME10MS * 2 + T0TC; //
            //T0CCR=0x07;//�����½�
            HW_DEVICE.flag |= DEV_HW_FLAG_XMDWON;
            HW_DEVICE.XM.cap0_time = T0CR1; //�½���ʱ��
            pcap = HW_DEVICE.XM.buf;
            *(pcap + HW_DEVICE.XM.lg) = 0x7c; //38k
            HW_DEVICE.XM.lg++;
            *(pcap + HW_DEVICE.XM.lg) = 1;
            HW_DEVICE.XM.lg++;
            break;
        }
        IO_FlagLingt_Flash();
    }
    while(1);
    do
    {
        flag = T0IR;
        T0IR = flag;
        if (flag & 0x20)
        {
            T0MR0 = HW_TIME10MS * 10 + T0TC; //
            tmp = (uint16)(T0CR1 - HW_DEVICE.XM.cap0_time);
            HW_DEVICE.XM.cap0_time = T0CR1;
            *(pcap + HW_DEVICE.XM.lg) = tmp & 0xff;
            HW_DEVICE.XM.lg++;
            *(pcap + HW_DEVICE.XM.lg) = tmp >> 8;
            HW_DEVICE.XM.lg++;
            if (HW_DEVICE.XM.lg > HW_DATA_SIZE * 4)
            {
                break;
            };//ѧ��128λ��

        }
        else
        {
            Treg = T0TC;
            if ((flag & 0x01) || (Treg >= T0MR0))	 break;
        }
        IO_FlagLingt_Flash();
    }
    while(1);
    OS_EXIT_CRITICAL();
    *(p + 4) = 0;
    *(p + 5) = HW_DEVICE.XM.lg & 0xff;
    *(p + 6) = HW_DEVICE.XM.lg >> 8;
    UDP_Send_CMD(p, HW_DEVICE.XM.lg, prev->des_ip);
    HW_DEVICE.flag = 0;
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("����ѧ�����-����:%d\n\n\n", HW_DEVICE.XM.lg);
    User_mem_free(p);
    HW_DEVICE.XM.buf = NULL;
    HW_DEVICE.XM.cap0_time = 0;
    T0TCR = 0;
    T0CCR = 0;
    T0MCR = 0;
    keymsg = keymsg;
    return;

}

/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
BYTE  HW_send(uint8 id)
{
    uint16 tmp, flag;
    uint8 *p, *pend;
    struct MESSAGE_TYPE *phw;
    uint32 Treg;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                           */
#endif

    HW_DEVICE.send.Curr_id = id;
    phw = (struct MESSAGE_TYPE *)(ARM_TABLE_ADDRESS + 256 * HW_DEVICE.send.Curr_id);
    HW_DEVICE.send.Currlength = phw->data_lg;
    HW_DEVICE.send.CurrSendLg = 0;
    p = (BYTE *)(ARM_TABLE_ADDRESS + 256 * HW_DEVICE.send.Curr_id + sizeof(struct MESSAGE_TYPE ));
    pend = p +	phw->data_lg;
    tmp = *(p + 1) * 256 + *p; //�ز�Ƶ��38K
    if (tmp == 0)  return 0;
    p += 2;

    T1CTCR = 0;
    T1PR = 0; //Fpclk����
    T1MR0 = Fpclk / (tmp * 200) - 1; //76k
    T1MCR = 2; //ƥ�����������
    T1EMR = 0xc2; //MT0��ת
    T1TCR = 2;


    T0TCR = 0;
    T0CTCR = 0; //��ʱģʽ
    T0PR = HW_PR_TIME10us; //Ԥ��Ƶ10us TC+1����
    T0TC = 0; //������
    T0TCR = 2; //��λ������
    T0MCR = 3; //ƥ������жϣ�����������Tc=0;
    T0CCR = 0;
    tmp = *(p + 1) * 256 + *p;
    p += 2;
    if (tmp == 0) return 0;
    OS_ENTER_CRITICAL();
    T0MR0 = tmp;
    HW_DEVICE.flag |= DEV_HW_FLAG_SEND;
    T0IR = 0xff;
    T0TCR = 1;
    T1TCR = 1;
    for (;;)
    {
        flag = T0IR;
        Treg = T0TC;
        if ((flag & 0x01) || (Treg >= T0MR0))
        {
            T0IR = 0x01;
            T0TC = 0;
            if (HW_DEVICE.flag & DEV_HW_FLAG_SEND)
            {
                HW_DEVICE.flag &= ~DEV_HW_FLAG_SEND;
                T1TCR = 0;
            }
            else
            {
                HW_DEVICE.flag |= DEV_HW_FLAG_SEND;
                T1TCR = 1;
            }
            tmp = *(p + 1) * 256 + *p;
            p += 2;
            if (tmp == 0) break;
            T0MR0 = tmp;	//T0TC=0;
            if (p >= pend)
            {
                T0TCR = 0;
                T1EMR = 0x81;
                T1TCR = 1;
                for (tmp = 0; tmp < 500; tmp++);
                T1TCR = 0;
                break;
            }

        }
        IO_FlagLingt_Flash();
    }
    flag = flag;

    OS_EXIT_CRITICAL();
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
void Time1_Exception(void)
{
    //VICVectAddr=0x00;
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void  Time0_Exception(void)
{
#if 0
    uint8 buf[10], *p;
    uint32 pdata;
    uint32 flag;

fiq1:
    flag = T0IR;
    T0IR = T0IR;
    if (flag & 0x10)
    {
        if  (( HW_DEVICE.flag & DEV_HW_FLAG_XM) == DEV_HW_FLAG_XM)
        {
            T0MR0 = HW_TIME10MS * 2 + T0TC; //
            pdata = (T0CR0 - HW_DEVICE.XM.cap0_time) | HW_DATA_SEND;
            HW_DEVICE.XM.cap0_time = T0CR0;
            p = (BYTE *)HW_DEVICE.XM.buf;
            *(p + HW_DEVICE.XM.lg) = pdata & 0xff;
            HW_DEVICE.XM.lg++;
            *(p + HW_DEVICE.XM.lg) = pdata >> 8;
            HW_DEVICE.XM.lg++;
            if (HW_DEVICE.XM.lg > HW_DATA_SIZE * 4)
            {
                //ѧ��128λ��
                HW_DEVICE.flag |= DEV_HW_FLAG_XM_OK;
            }
        }
    }
    else if (flag & 0x20) //�����ж�
    {
        if  (( HW_DEVICE.flag & DEV_HW_FLAG_XM) == DEV_HW_FLAG_XM)
        {
            //ѧ��
            if ( (HW_DEVICE.flag & DEV_HW_FLAG_XMFIRST) == DEV_HW_FLAG_XMFIRST)
            {
                //����ѧ������
                T0MR0 = HW_TIME10MS * 2 + T0TC; //
                //T0CCR=0x07;//�����½�
                HW_DEVICE.flag &= ~DEV_HW_FLAG_XMFIRST;
                HW_DEVICE.flag |= DEV_HW_FLAG_XMDWON;
                HW_DEVICE.XM.cap0_time = T0CR1; //�½���ʱ��

            }
            else
            {
                //
                T0MR0 = HW_TIME10MS * 2 + T0TC; //
                pdata = (T0CR1 - HW_DEVICE.XM.cap0_time);
                HW_DEVICE.XM.cap0_time = T0CR1;
                p = (BYTE *)HW_DEVICE.XM.buf;
                *(p + HW_DEVICE.XM.lg) = pdata & 0xff;
                HW_DEVICE.XM.lg++;
                *(p + HW_DEVICE.XM.lg) = pdata >> 8;
                HW_DEVICE.XM.lg++;
                if (HW_DEVICE.XM.lg > HW_DATA_SIZE * 4)
                {
                    //ѧ��128λ��
                    HW_DEVICE.flag |= DEV_HW_FLAG_XM_OK;
                }

            }
        }
    }
    else if (flag & 01) //ƥ���ж�
    {
        if  (( HW_DEVICE.flag & DEV_HW_FLAG_XM) == DEV_HW_FLAG_XM)
        {
            //ѧ��
            if ( (HW_DEVICE.flag & DEV_HW_FLAG_XMFIRST) == DEV_HW_FLAG_XMFIRST)
            {
                //һ���ӳ�ʱ�ж�
                HW_DEVICE.flag |= DEV_HW_FLAG_XM_OVER;
                T0CCR = 0x36; //��������ʱ�ж�
            }
            else
            {
                //20ms�ж�ѧ�����
                HW_DEVICE.flag |= DEV_HW_FLAG_XM_OK;
                T0CCR = 0x36; //��������ʱ�ж�
            }

        }
        else if (HW_DEVICE.flag & DEV_HW_FLAG_SEND)
        {
            //����
            T1TCR = 2;
            pdata = HW_Get_data();
            if (pdata == 0)
            {
                T0TCR = 0;
            }
            else
            {
                if ((pdata & (~HW_DATA_SEND)) > T0TC)
                    T0MR0 = (pdata & (~HW_DATA_SEND));
                else
                    T0MR0 = (pdata & (~HW_DATA_SEND)) + T0TC;

                if (pdata & HW_DATA_SEND)
                    T1TCR = 1; //������ʱ��TIME2
                else
                    T1TCR = 0; //
            }
        }

    }
    //T0IR=0xff;
    if (T0IR != 0) goto fiq1;
    //V/ICVectAddr=0x00;
    return ;
#endif
    //VICVectAddr=0x00;
}
//////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void HW_init(void)
{
    memset((BYTE *)&HW_DEVICE, 0, sizeof (struct DEV_HW_TYPE));
    T0TCR = 0;
    T1TCR = 0;

}

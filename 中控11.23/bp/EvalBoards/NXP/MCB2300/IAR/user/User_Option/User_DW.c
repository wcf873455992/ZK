#include "includes.h"
struct RF_CARD_TYPE gRF;
extern struct DEVICE_REC_TYPE gDeviceTable;
static int    RF_card_times = 0;
struct YXJ_TYPE gYXJ = {0, 0}; //���ػ����ȼ�
struct RFIC_DATA_TYPE 	gRFIC;//����������


/////////////////////////////////////////////////////////////////////
// Description : K103������У����յ�������
// date        : 2010/04/18
// Return      : ���ؼ����
/////////////////////////////////////////////////////////////////////
static uint8 _K103_sum_check(uint8 *pdata, uint8 lg)
{
    uint8 sum_xor, i;
    sum_xor = 0;
    for (i = 0; i < lg; i++)
    {
        sum_xor ^= *(pdata + i);
    }
    return sum_xor;
}

/////////////////////////////////////////////////////////////////////
// Description : ��pdataָ���������Ѱ��ͬ��ͷ(ABLE_FLAG_START �� ABLE_FLAG_ACK)
// date        : 2010/04/18
// Return      : ����ֵ�ĺ���: �ٶ�
//               1. lg = 5,ͬ��ͷ��λ��0��,�򷵻�5;
//               2. lg = 5,ͬ��ͷ��λ��1��,�򷵻�4; ͬʱλ��0�����ݱ�λ��1�������滻... �Դ�����
// NOTE        : ��pdataָ���������Ѱ��ͬ��ͷ
/////////////////////////////////////////////////////////////////////
static uint8  _ic_find_syn(uint8 ic_type, uint8 *pdata, uint8 lg)
{
    uint8 i, tmp_lg, tmp;

    if (lg == 0)
    {
        return lg;
    }

    tmp_lg = lg;
    tmp = 0;
    /* Ѱ��ͬ��ͷ */
    for (;;)
    {
        if ((*pdata == K103_Head) && (*(pdata + 1) == K103_sHead)) break;
        for (i = 0; i < tmp_lg; i++)
        {
            *(pdata + i) = *(pdata + i + 1);
        };
        tmp_lg--;
        tmp++;
        if (tmp_lg == 0) 	break;
    }
    return (lg - tmp);

}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�RF�����ݴ���
//	�������ܣ�
//	��ڲ�����
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
int RFIC_K103_Mode(uint8 *pdata)
{
    uint8 buf[32], *p, tmp;
    uint32 card_k_id;

    struct MSG_REV_TYPE Rev;
    if ( Check_FunctionMode(Device_FunctionMode_CARD) != 1) return 0;
    Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
    Rev.Type = DEV_XCOM_RF;

    p = (uint8 *)pdata + sizeof(struct XCOM_REV_TYPE);
    struct XCOM_REV_TYPE *prev;
    prev = (struct XCOM_REV_TYPE *)pdata;
    if (Get_Debug(DEV_DEBUG_MSG))
        IP_printf("У԰һ��ͨ���������ռ� 32 ��ռ�� %d �������� %d", gRFIC.lg, prev->XcomLengh);
    //if(gRFIC.lg>=24)
    //gRFIC.lg=0;
    Mem_Copy((uint8 *)&gRFIC.buf[gRFIC.lg], (uint8 *)p, prev->XcomLengh);
    gRFIC.lg += prev->XcomLengh;
    //ͬ������֡
    gRFIC.lg = _ic_find_syn('k', (uint8 *)&gRFIC.buf[0], gRFIC.lg);
    //
    if (gRFIC.lg < 8)  return -1;
    if (_K103_sum_check((uint8 *)&gRFIC.buf[0], 7) != gRFIC.buf[7])
    {
        //ͬ������֡
        gRFIC.buf[0] = 0;
        gRFIC.buf[1] = 0;
        gRFIC.lg = _ic_find_syn('k', (uint8 *)&gRFIC.buf[0], gRFIC.lg);
        return -2;
    }
    /* ������ʵΪ4���ֽ�, ��gRFIC.buf[2]��gRFIC.buf[6],�Ӹߵ�������. ��gRFIC.buf[2]Ϊ��,Ӧ�ú��� */
    /* �ѿ���ת��10���Ƶ��� */
    card_k_id = (uint32)(gRFIC.buf[3] << 0)			 /* 10.11 ����,��ǰΪ: [1] << 24 ...*/
                | (uint32)(gRFIC.buf[4] << 8)
                | (uint32)(gRFIC.buf[5] << 16)
                | (uint32)(gRFIC.buf[6] << 24);
    /* �ӵ�10λ����1λ, ÿ1λ��ת����ascll�� */
    /*
     2. /10
     3. /100, 2: /10 , %10  1: %100
     10. /10^9, %10^0, 9: /10^8, %10^1   8: /10^7, %10^1  7: /10^6, %10^1
     */
    Mem_Clr((uint8 *)&gRFIC.Card_No[0], 32);
    gRFIC.Card_No[0] = card_k_id / 1000000000UL + '0';
    gRFIC.Card_No[1] = (card_k_id / 100000000UL) % 10 + '0';
    gRFIC.Card_No[2] = (card_k_id / 10000000UL) % 10 + '0';
    gRFIC.Card_No[3] = (card_k_id / 1000000UL)  % 10 + '0';
    gRFIC.Card_No[4] = (card_k_id / 100000UL)   % 10 + '0';
    gRFIC.Card_No[5] = (card_k_id / 10000UL)    % 10 + '0';
    gRFIC.Card_No[6] = (card_k_id / 1000UL)	   % 10 + '0';
    gRFIC.Card_No[7] = (card_k_id / 100UL)	   % 10 + '0';
    gRFIC.Card_No[8] = (card_k_id / 10UL)	   % 10 + '0';
    gRFIC.Card_No[9] = (card_k_id / 1UL)		   % 10 + '0';
    gRFIC.Card_No[10] = 0;		 /* �ַ��������� */
    if (card_k_id != 0)
    {
        gRF.CMD_Time = 0;
        //gYXJ.time = 0;
        RF_card_times++;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("У԰һ��ͨ��֤����%d", RF_card_times);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("У԰һ��ͨ��֤����%s", gRFIC.Card_No);

        if (gYXJ.type == 1 )
        {
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("У԰һ��ͨ�ػ���ʱʱ��δ�����ѳ���%d s", gYXJ.time);
            gRFIC.buf[0] = 0;
            gRFIC.buf[1] = 0;
            gRFIC.lg = _ic_find_syn('k', (uint8 *)&gRFIC.buf[0], gRFIC.lg);
            gYXJ.time = 0;
            return 0;
        }
        gYXJ.time = 0;


        if (Get_Class_Stauts() == CLASS_OVER)
        {
            gRF.state = 0;
            gRF.state = RF_CARD_BIGEN;
            Mem_Clr(buf, 32);
            buf[0] = MSG_DEV_MAINBARD;
            buf[1] = MSG_OPTION_CardContrl;
            buf[2] = RF_ATT_REQ;
            for (tmp = 0; tmp < 22; tmp++)
            {
                buf[3 + tmp] = gRFIC.Card_No[tmp];
                if (gRFIC.Card_No[tmp] == 0) break;
            }
            Msg_send(buf, 31, (uint8 *)&Rev);
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("RF������");
        }
        //gRFIC.State|=RFIC_State_Active;
    }
    else
    {
        //gRFIC.State&=~RFIC_State_Active;
        //gRFIC.State&=~RFIC_State_LINK_OK;
        //����������λ
        //gRFIC.Power_count=RFIC_POWER_MAX_COUNT;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("У԰һ��ͨˢ��ʧ�ܣ���ǰ����״̬%d", Get_Class_Stauts());

        gRFIC.buf[0] = 0;
        gRFIC.buf[1] = 0;
        gRFIC.lg = _ic_find_syn('k', (uint8 *)&gRFIC.buf[0], gRFIC.lg);
        return 0;

    }
    //ͬ������֡
    gRFIC.buf[0] = 0;
    gRFIC.buf[1] = 0;
    gRFIC.lg = _ic_find_syn('k', (uint8 *)&gRFIC.buf[0], gRFIC.lg);
    //RFIC_State_Clr();
    return 0;
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//			val=0 �ر�
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void RF_CARD_Check(void)
{
    //�¿�ֱ�ӷ���
    uint8 buf[10];
    struct DEVICE_STATUS_TYPE *ps;
    struct MSG_REV_TYPE  Rev;

    Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
    Mem_Clr(buf, 10);
    Rev.Type = DEV_XCOM_RF;

    //û������У԰һ��ͨ�������ػ�
    if ( Check_FunctionMode(Device_FunctionMode_CARD) != 1) return;
    //�¿β����ػ�
    if (Get_Class_Stauts() == CLASS_OVER)
    {
        if(gYXJ.time++ > 20)
        {
            gYXJ.type = 0;
            gYXJ.time = 0;
        }
        return ;
    }
    //����RF�����������ػ�
    if (Get_Class_Stauts() != CLASS_BEGIN_RF) return ;

    if (gRF.CMD_Time++ > 15)
    {
        gRF.CMD_Time = 0;
        RF_card_times = 0;


        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = MSG_OPTION_CLASS_OVER;
        Msg_send(buf, 2, (uint8 *)&Rev);
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("RF���ػ�");
    }
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//			val=0 �ر�
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
/*
void DW_Wite_CoreIP(void)
{
	uint8 *ip,buf[10];
	buf[0]=BJC_CMD_CoreIP;
	ip=Get_DesIp(DEVICE_SUB_CLASS);
	Mem_Copy(&buf[1], ip,4);
	XCOM_Data_send(DEV_XCOM_DW, buf,5,XCOM_CMD_SET_DATA, BJC_SUB,0);

}*/
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//			val=0 �ر�
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
/*
void DW_Read_WD(void)
{
	uint8 buf[10];
	buf[0]=BJC_CMD_WDVal;
	XCOM_Data_send(DEV_XCOM_DW, buf,1,XCOM_CMD_SET_DATA, BJC_SUB,0);
}*/
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////

void DW_init(void)
{
    uint8 tmp, buf[16], lg, err;
    struct DEVICE_STATUS_TYPE *ps;
    struct CLASS_INFO *pclass;
    pclass = (struct CLASS_INFO *)Get_ClassInfo_Table(0);

    Mem_Set((uint8 *)&gRF, 0, sizeof(struct RF_CARD_TYPE));

    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    //ps->DEV_Link_State|=DEV_LINK_BJCardLK;
    ps->DEV_Link_State &= ~DEV_LINK_BJCardLK;
    /*
    for (tmp=0;tmp<5;tmp++)
    {
      buf[0]=BJC_CMD_Ver;
      lg=1;
      ps->DEV_Link_State&=~DEV_LINK_BJCardLK;
      //err=XCOM_Data_Read(DEV_XCOM_DW,BJC_SUB,XCOM_CMD_GET_INFO,buf,&lg,XCOM_READ_FLAG_WAIT_DATA);
      if (err == OS_NO_ERR)
    	{ps->DEV_Link_State|=DEV_LINK_BJCardLK;
    	 ps->BJ_CardVer=buf[4]*256+buf[5];
    	 //if (Get_Debug(DEV_DEBUG_MSG))
    		 IP_printf("�༭���汾:%d.%d",buf[4],buf[5]);
    	 //DW_Wite_CoreIP();
    	 return ;
    	}
      else
      	{//if (Get_Debug(DEV_DEBUG_MSG))
    		IP_printf("�༭��û�а�װ!");
    	}
    	IO_FlagLingt_Flash();
    }
           */
    //ps->DEV_Link_State|=DEV_LINK_BJCardLK;

}/**/
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//			val=0 �ر�
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
/*
void DWQ_Contrl(uint8 val)
{
uint8 tmp,i,buf[10],err,lg;
struct DEVICE_STATUS_TYPE *ps;
	ps=(struct DEVICE_STATUS_TYPE *)Get_Device_Status();
	if (ps->DEV_Link_State&DEV_LINK_BJCardLK)
		tmp=3;
	else
		tmp=1;
	for (i=0;i<tmp;i++)
		{buf[0]=BJC_CMD_CSContrl;
	         buf[1]=val;
		 buf[2]=val;
		lg=3;
		err=XCOM_Data_Read(DEV_XCOM_DW,BJC_SUB,XCOM_CMD_SET_DATA,buf,&lg,XCOM_READ_FLAG_WAIT_ACK);
		if (err==OS_NO_ERR) break;
		IO_FlagLingt_Flash();
		}
}
*/
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
/**/
void DW_CMD_Contrl(uint8 *pdata)
{
    uint8 buf[32], cmd, tmp;
    uint16 lg;
    struct DEVICE_STATUS_TYPE *ps;
    struct MSG_REV_TYPE Rev;


    Mem_Clr((uint8 *)&Rev, sizeof(struct MSG_REV_TYPE));
    Rev.Type = DEV_XCOM_RF;
    ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
    cmd = *pdata;
    switch(cmd)
    {

    case  BJC_CMD_Ver://   		0  //���汾��
        ps->BJ_CardVer = 256 * (*(pdata + 1)) + *(pdata + 2);
        ps->DEV_Link_State |= DEV_LINK_BJCardLK;
        buf[0] = MSG_DEV_MAINBARD;
        buf[1] = 0xff; //״̬�ı�
        Msg_send(buf, 2, &buf[2]);
        break;
    case   BJC_CMD_CSContrl:// 		2//��������λ������ͨ��
        if (ps->CLASS_state & DEV_CLASS_START)
            buf[0] = 1;
        else
            buf[0] = 0;
        DW_CMD_Contrl(buf);
        break;
    case  BJC_CMD_DATA://		3//���ݲ���ͨ��
        break;
    case   BJC_CMD_CSReturn://		4//�����������ź���ֵ
        break;
    case   BJC_CMD_JDATA://		5//���У׼����
        break;
    case   BJC_CMD_WDVal://		6//���¶�ֵ
        ps->WD_Val = 256 * (*(pdata + 1)) + *(pdata + 2);
        break;
    case   BJC_CMD_WXval://		7//��/д����MIC������
        break;
    case   BJC_CMD_WXStutas://		8//����MIC״̬��ROֻ������״̬�ı�ʱ���ͺ��İ�
        break;
    case   BJC_CMD_WXSet://	        9//����ң�������ã�����Ѹ�ݸ��������MIC���ջ���
        break;
    case  BJC_CMD_WXREVStatus:// 10//����ң����״̬
        break;
    case   BJC_CMD_Vol://			11//����/��ȡMIC����ֵ
        break;
    case   BJC_CMD_WXPower://		12//��ȡ����MIC�ز�ǿ��
        break;
    case  BJC_CMD_CoreIP://		13//���İ��д�װ�IP
        DW_Wite_CoreIP();
        break;
    case   BJC_CMD_CardNo://		14//У԰һ��ͨ����
        gRF.CMD_Time = 0;
        gYXJ.time = 0;
        RF_card_times++;
        if (Get_Debug(DEV_DEBUG_MSG))
            IP_printf("У԰һ��ͨ��֤����%d", RF_card_times);
        if ( Check_FunctionMode(Device_FunctionMode_CARD) != 1) break;
        if (gYXJ.type == 1 ) break;

        if (Get_Class_Stauts() == CLASS_OVER)
        {
            gRF.state = 0;
            gRF.state = RF_CARD_BIGEN;
            Mem_Clr(buf, 32);
            buf[0] = MSG_DEV_MAINBARD;
            buf[1] = MSG_OPTION_CardContrl;
            buf[2] = RF_ATT_REQ;
            for (tmp = 0; tmp < 22; tmp++)
            {
                buf[3 + tmp] = *(pdata + 1 + tmp);
                if (*(pdata + 1 + tmp) == 0) break;
            }
            Msg_send(buf, 31, (uint8 *)&Rev);
            if (Get_Debug(DEV_DEBUG_MSG))
                IP_printf("RF������");
        }
        break;
    case  XCOM_CMD_PRINT://          85 //���ڴ�ӡ��Ϣ
        // DBG_XCOM_Printf(DEV_XCOM_DW,pdata+1);
        break;
    case  XCOM_CMD_TEST://	        170 //���ڲ���ָ��
        break;

    }


}

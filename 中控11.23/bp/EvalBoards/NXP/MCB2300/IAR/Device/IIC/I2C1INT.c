
/*
---------------------------------------------------------------------------------------------------------
*********************************************************************************************************
** �ļ����� ��	I2C1INT.c
** ����˵�� ��	LPC2000Ӳ��I2C1�жϷ�ʽ�������
** ʹ��˵�� ��  ������Ҫ���ú�I2C1���߽ӿ�(I2C1���Ź��ܺ�I2C1�жϣ�����ʹ��I2C1��ģʽ)
*********************************************************************************************************
---------------------------------------------------------------------------------------------------------
*/
#include  "includes.h"

/* �������ں�I2C�жϴ�����Ϣ��ȫ�ֱ��� */
uint8 	I2C1_sla;			/* I2C1�����ӵ�ַ 					*/
uint32	I2C1_suba;			/* I2C1�����ڲ��ӵ�ַ 				*/
uint8 	I2C1_suba_num;		/* I2C1�ӵ�ַ�ֽ���					*/
uint8 	*I2C1_buf;        	/* ���ݻ�����ָ��  					*/
uint32 I2C1_num;			/* Ҫ��ȡ/д������ݸ��� 			*/
uint8 	I2C1_end;			/* I2C1���߽�����־��������������1 	*/
uint8 	I2C1_suba_en;		/* 	�ӵ�ַ���ơ�
										0--�ӵ�ַ�Ѿ�������߲���Ҫ�ӵ�ַ
										1--��ȡ����
										2--д����
									*/

/*
**********************************************************************************************************
** �������ƣ�ISendByte()
** �������ܣ������ӵ�ַ��������1�ֽ����ݡ�
** ��ڲ�����sla		������ַ
**           dat		Ҫ���͵�����
** ���ڲ���������ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** ˵����	ʹ��ǰҪ��ʼ����I2C1���Ź��ܺ�I2C1�жϣ�����ʹ��I2C1��ģʽ
*********************************************************************************************************
*/
uint8  ISendByte1(uint8 sla, uint8 dat)
{
    /* �������� */
    uint32 i = 0;
    I2C1_sla     = sla;		// д������������ַ
    I2C1_buf     = &dat;		// �����͵�����
    I2C1_num     = 1;			// ����1�ֽ�����
    I2C1_suba_en = 0;		 	// ���ӵ�ַ
    I2C1_end     = 0;

    I21CONCLR = 0x2C;
    I21CONSET = 0x60;             // ����Ϊ����������������

    while(0 == I2C1_end)
    {
        i++;
        if (i > 100000) break;
    };

    if(1 == I2C1_end) return(1);
    else return(0);
}

/*
*********************************************************************************************************
** �������ƣ�IRcvByte()
** �������ܣ������ӵ�ַ������ȡ1�ֽ����ݡ�
** ��ڲ�����sla		������ַ
**           dat		�������ݵı���ָ��
** ���ڲ���������ֵΪ0ʱ��ʾ��������Ϊ1ʱ��ʾ������ȷ��
** ˵����ʹ��ǰҪ��ʼ����I2C1���Ź��ܺ�I2C1�жϣ�����ʹ��I2C1��ģʽ
*********************************************************************************************************
*/
uint8 IRcvByte1(uint8 sla, uint8 *dat)
{
    /* �������� */
    uint32 i = 0;
    I2C1_sla     = sla + 1;		// ��������������ַ
    I2C1_buf     = dat;
    I2C1_num     = 1;
    I2C1_suba_en = 0;			// ���ӵ�ַ
    I2C1_end     = 0;

    I21CONCLR = 0x2C;
    I21CONSET = 0x60;         // ����Ϊ����������������

    while(0 == I2C1_end)
    {
        i++;
        if (i > 100000) break;
    };
    return I2C1_end;
}

/*
*********************************************************************************************************
** �������� ��I2C_ReadNByte()
** �������� �������ӵ�ַ���������ַ��ʼ��ȡN�ֽ�����
** ��ڲ��� ��	sla			�����ӵ�ַ
**				suba_type	�ӵ�ַ�ṹ	1�����ֽڵ�ַ	2��8+X�ṹ	2��˫�ֽڵ�ַ
**				suba		�����ӵ�ַ
**				s			���ݽ��ջ�����ָ��
**				num			��ȡ�ĸ���
** ���ڲ��� ��	TRUE		�����ɹ�
**		FALSE		����ʧ��
*********************************************************************************************************
*/
uint8 I2C1_ReadNByte (uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num)
{
    uint32 i = 0;
    if (num > 0)	/* �ж�num�����ĺϷ��� */
    {
        /* �������� */
        if (suba_type == 1)/////////////��
        {
            /* �ӵ�ַΪ���ֽ� */
            I2C1_sla     	= sla + 1;							/* �������Ĵӵ�ַ��R=1 	*/
            I2C1_suba    	= suba;								/* �����ӵ�ַ 			*/
            I2C1_suba_num	= 1;								/* �����ӵ�ַΪ1�ֽ� 	*/
        }
        if (suba_type == 2)
        {
            /* �ӵ�ַΪ2�ֽ� */
            I2C1_sla     	= sla + 1;							/* �������Ĵӵ�ַ��R=1 	*/
            I2C1_suba   	= suba;							/* �����ӵ�ַ 			*/
            I2C1_suba_num	= 2;								/* �����ӵ�ַΪ2�ֽ� 	*/
        }
        if (suba_type == 3)
        {
            /* �ӵ�ַ�ṹΪ8+X*/
            I2C1_sla	= sla + ((suba >> 7 ) & 0x0e) + 1; /* �������Ĵӵ�ַ��R=1	*/
            I2C1_suba	= suba & 0x0ff;						/* �����ӵ�ַ	 		*/
            I2C1_suba_num	= 1;							/* �����ӵ�ַΪ8+x	 	*/
        }
        I2C1_buf     = s;								/* ���ݽ��ջ�����ָ�� 	*/
        I2C1_num     = num;								/* Ҫ��ȡ�ĸ��� 		*/
        I2C1_suba_en = 1;								/* ���ӵ�ַ�� 			*/
        I2C1_end     = 0;

        /* ���STA,SI,AA��־λ */
        I21CONCLR = (1 << 2) |	/* AA 		*/
                    (1 << 3) |	/* SI 		*/
                    (1 << 5);	/* STA 		*/

        /* ��λSTA,����I2C1���� */
        I21CONSET = (1 << 5) |	/* STA 		*/
                    (1 << 6);	/* I2CEN 	*/

        /* �ȴ�I2C������� */
        while (I2C1_end == 0)
        {
            //if (i++ > 10000*num)
            //	return FALSE;
            if (i++ > 10000 * num)    //2013.2.28��������޷���������
                return FALSE;
            //IO_FlagLingt_Flash();   //2013.2.28��������޷���������
        }
        return I2C1_end ;
    }
    return (FALSE);
}

/*
*********************************************************************************************************
** �������� ��I2C_WriteNByte()
** �������� �������ӵ�ַ����д��N�ֽ�����
** ��ڲ��� ��	sla			�����ӵ�ַ
**				suba_type	�ӵ�ַ�ṹ	1�����ֽڵ�ַ	3��8+X�ṹ	2��˫�ֽڵ�ַ
**			  	suba		�����ڲ������ַ
**			  	*s			��Ҫд������ݵ�ָ��
**			  	num			��Ҫд������ݵĸ���
** ���ڲ��� ��	TRUE		�����ɹ�
**			  	FALSE		����ʧ��
*********************************************************************************************************
*/
uint8 I2C1_WriteNByte(uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num)
{
    uint32 i = 0;
    if (num > 0)/* �����ȡ�ĸ���Ϊ0���򷵻ش��� */
    {
        /* ���ò��� */
        if (suba_type == 1)
        {
            /* �ӵ�ַΪ���ֽ� */
            I2C1_sla     	= sla & 0xFE;								/* �������Ĵӵ�ַ	 	*/
            I2C1_suba    	= suba;								/* �����ӵ�ַ 			*/
            I2C1_suba_num	= 1;								/* �����ӵ�ַΪ1�ֽ� 	*/
        }
        if (suba_type == 2)
        {
            /* �ӵ�ַΪ2�ֽ� */
            I2C1_sla     	= sla;								/* �������Ĵӵ�ַ 		*/
            I2C1_suba   	 	= suba;							/* �����ӵ�ַ 			*/
            I2C1_suba_num	= 2;								/* �����ӵ�ַΪ2�ֽ� 	*/
        }
        if (suba_type == 3)
        {
            /* �ӵ�ַ�ṹΪ8+X */
            I2C1_sla			= sla + ((suba >> 7 ) & 0x0e);	/* �������Ĵӵ�ַ		*/
            I2C1_suba		= suba & 0x0ff;						/* �����ӵ�ַ			*/
            I2C1_suba_num	= 1;								/* �����ӵ�ַΪ8+X	 	*/
        }

        I2C1_buf     = s;										/* ���� 				*/
        I2C1_num     = num;										/* ���ݸ��� 			*/
        I2C1_suba_en = 2;										/* ���ӵ�ַ��д���� 	*/
        I2C1_end     = 0;

        /* ���STA,SI,AA��־λ */
        I21CONCLR = (1 << 2) |	/* AA 	*/
                    (1 << 3) |	/* SI 	*/
                    (1 << 5);	/* STA 	*/

        /* ��λSTA,����I2C���� */
        I21CONSET = (1 << 5) |	/* STA 	*/
                    (1 << 6);	/* I2CEN*/

        /* �ȴ�I2C������� */
        while (I2C1_end == 0)
        {
            if (i++ > 1000000)
                return FALSE;
            //IO_FlagLingt_Flash();
        }
        return I2C1_end;
    }
    return (FALSE);
}

/*
*********************************************************************************************************
** �������� ��__irq IRQ_I2C1()
** �������� ��Ӳ��I2C1�жϷ������
** ��ڲ��� ����
** ���ڲ��� ����
** ˵��     ��ע�⴦���ӵ�ַΪ2�ֽڵ������
*********************************************************************************************************
*/
void I2C1_Exception(void)
{
    /* ��ȡI2C״̬�Ĵ���I2C1DAT */
    /* ����ȫ�ֱ��������ý��в��������������־ */
    /* ����ж��߼�,�жϷ��� */

    switch (I21STAT & 0xF8)
    {
    /* ����״̬�������Ӧ�Ĵ��� */
    case 0x08:	/* �ѷ�����ʼ���� */				/* �����ͺ������ն��� 		*/
        /* װ��SLA+W����SLA+R */
        if(I2C1_suba_en == 1)/* SLA+R */			/* ָ���ӵ�ַ�� 			*/
        {
            I21DAT = I2C1_sla & 0xFE;			/* ��д���ַ 				*/
        }
        else				/* SLA+W */
        {
            I21DAT = I2C1_sla;        				/* ����ֱ�ӷ��ʹӻ���ַ 	*/
        }
        /* ����SIλ */
        I21CONCLR =	(1 << 3) |						/* SI 						*/
                    (1 << 5);						/* STA 						*/
        break;
    case 0x10:	/*�ѷ����ظ���ʼ���� */ 			/* �����ͺ������ն��� 		*/
        /* װ��SLA+W����SLA+R */
        I21DAT = I2C1_sla;							/* �������ߺ��ط��ӵ�ַ 	*/
        I21CONCLR = 0x28;							/* ����SI,STA */
        break;
    case 0x18:
    case 0x28:	/* �ѷ���I2C1DAT�е����ݣ��ѽ���ACK */
        if (I2C1_suba_en == 0)
        {
            if (I2C1_num > 0)
            {
                I21DAT = *I2C1_buf++;
                I21CONCLR = 0x28;					/* ����SI,STA 				*/
                I2C1_num--;
            }
            else	/* û�����ݷ����� */
            {
                /* ֹͣ���� */
                I21CONSET = (1 << 4);				/* STO 						*/
                I21CONCLR = 0x28;					/* ����SI,STA 				*/
                I2C1_end = 1;						/* �����Ѿ�ֹͣ 			*/
            }
        }

        if(I2C1_suba_en == 1)	/* ����ָ����ַ������������������ 				*/
        {
            if (I2C1_suba_num == 2)
            {
                I21DAT = ((I2C1_suba >> 8) & 0xff);
                I21CONCLR = 0x28;					/* ����SI,STA 				*/
                I2C1_suba_num--;
                break;
            }

            if(I2C1_suba_num == 1)
            {
                I21DAT = (I2C1_suba & 0xff);
                I21CONCLR = 0x28;					/* ����SI,STA 				*/
                I2C1_suba_num--;
                break;
            }

            if (I2C1_suba_num == 0)
            {
                I21CONSET = 0x20;
                I21CONCLR = 0x08;
                I2C1_suba_en = 0;     				/* �ӵ�ַ������ 			*/
                break;
            }
        }

        if (I2C1_suba_en == 2)/* ָ���ӵ�ַд,�ӵ�ַ��δָ��,�����ӵ�ַ 		*/
        {
            if (I2C1_suba_num > 0)
            {
                if (I2C1_suba_num == 2)
                {
                    I21DAT = ((I2C1_suba >> 8) & 0xff);
                    I21CONCLR = 0x28;
                    I2C1_suba_num--;
                    break;
                }
                if (I2C1_suba_num == 1)
                {
                    I21DAT    = (I2C1_suba & 0xff);
                    I21CONCLR = 0x28;
                    I2C1_suba_num--;
                    I2C1_suba_en  = 0;
                    break;
                }
            }
        }
        break;
    case 0x40:	/* �ѷ���SLA+R,�ѽ���ACK */
        if (I2C1_num <= 1)	/* ��������һ���ֽ� */
        {
            I21CONCLR = 1 << 2;      				/* �´η��ͷ�Ӧ���ź� 		*/
        }
        else
        {
            I21CONSET = 1 << 2;					/* �´η���Ӧ���ź� 		*/
        }
        I21CONCLR = 0x28;							/* ����SI,STA 				*/
        break;
    case 0x20:	/* �ѷ���SLA+W,�ѽ��շ�Ӧ��              */
    case 0x30:	/* �ѷ���I2C1DAT�е����ݣ��ѽ��շ�Ӧ��   */
    case 0x38:	/* ��SLA+R/W�������ֽ��ж�ʧ�ٲ�         */
    case 0x48:	/* �ѷ���SLA+R,�ѽ��շ�Ӧ��              */
        I21CONCLR = 0x28;
        I2C1_end = 0x0;
        break;
    case 0x50:	/* �ѽ��������ֽڣ��ѷ���ACK */
        if (I2C1_buf != NULL)
            *I2C1_buf++ = I21DAT;
        I2C1_num--;
        if (I2C1_num == 1)/* �������һ���ֽ� */
        {
            I21CONCLR = 0x2C;						/* STA,SI,AA = 0 			*/
        }
        else
        {
            I21CONSET = 0x04;						/* AA=1 					*/
            I21CONCLR = 0x28;
        }
        break;
    case 0x58:	/* �ѽ��������ֽڣ��ѷ��ط�Ӧ�� */
        if (I2C1_buf != NULL)
            *I2C1_buf++ = I21DAT;     					/* ��ȡ���һ�ֽ����� 		*/
        I21CONSET = 0x10;        					/* �������� 				*/
        I21CONCLR = 0x28;
        I2C1_end = 1;
        break;
    default:
        break;
    }
    //IP_printf("I2C:%x %x",I21STAT&0xF8,I21DAT);
    //VICVectAddr = 0x00;              					/* �жϴ������ 			*/
}

//////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
BYTE AT24C16_Read(BYTE *buf, uint16 address, uint16 lg)
{
    uint8 dev_address, sub_address;
    dev_address = 0xa0 + ((address >> 7) & 0xFE);
    sub_address = address & 0xff;
    return I2C1_ReadNByte(dev_address, ONE_BYTE_SUBA, sub_address, buf, lg);
}

//////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
BYTE AT24C16_Write( BYTE *buf, uint16 address, uint16 lg)
{
    uint8 dev_address, sub_address, flag;
    uint16 tmp, tmp_add, tmp_lg;
    uint32 etime;
#if 0
    if (lg <= 16)
    {
        dev_address = 0xa0 + ((address >> 7) & 0xFE);
        sub_address = address & 0xff;
        flag = I2C1_WriteNByte(dev_address, ONE_BYTE_SUBA, sub_address, buf, lg);
        etime == 0;
        do
        {
            flag = I2C1_ReadNByte(dev_address, ONE_BYTE_SUBA, sub_address, rbuf, 1);
            if (etime++ > 1000) break;
        }
        while(flag == FALSE);
        return  flag;
    }
#endif

    tmp_lg = lg;
    tmp_add = address;
    tmp = 0;
    for (;;)
    {
        dev_address = 0xa0 + ((address >> 7) & 0xFE);
        sub_address = tmp_add & 0xff;
        if (tmp_lg >= 16)
        {
            flag = I2C1_WriteNByte(dev_address, ONE_BYTE_SUBA, sub_address, buf + tmp, 16);
            //IP_printf("dev_address=%x,ONE_BYTE_SUBA=%x,sub_address=%x,buf+tmp=%x",dev_address,ONE_BYTE_SUBA,ONE_BYTE_SUBA,buf+tmp);
            etime = 0;
            /*
            do{
             			flag = I2C1_ReadNByte(dev_address, ONE_BYTE_SUBA, sub_address,rbuf,1);
             			if (etime++ > 1000)
             				break;
            	   		IO_FlagLingt_Flash();
            	   }while(flag!=TRUE);*/
            if (tmp_lg <= 16)
                return flag;
        }
        else
        {
            flag = I2C1_WriteNByte(dev_address, ONE_BYTE_SUBA, sub_address, buf + tmp, tmp_lg);
            etime = 0;
            /*
            do{
            		flag = I2C1_ReadNByte(dev_address, ONE_BYTE_SUBA, sub_address,rbuf,1);
            		if (etime++ > 1000)
            			break;
            	}while(flag!=TRUE);
                          */
        }

        tmp += 16;
        tmp_add += 16;
        if ((tmp_lg > 16) && (flag))
            tmp_lg -= 16;
        else/**/
            return flag;

    }
}

//////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
/////////////////////////////////////////////////////////
void I2c1Init(uint32 Fi2c)
{
    if (Fi2c > 400000)
        Fi2c = 400000;

    I21SCLH = (Fpclk / Fi2c + 1) / 2;						/* �趨I2Cʱ�� 		*/
    I21SCLL = (Fpclk / Fi2c) / 2;
    I21CONCLR = 0x6C;
    I21CONSET = 0x40;									/* ʹ����I2C 		*/
    SetVICIRQ(VIC_CHANLE_I2C1, 10, (uint32)I2C1_Exception);
}

/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			main.c
** Last modified Date:  2004-09-16
** Last Version:		1.0
** Descriptions:		The main() function example template
**
**------------------------------------------------------------------------------------------------------
** Created by:			Chenmingji
** Created date:		2004-09-16
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			Chenxibing
** Modified date:		2005-02-16
** Version:
** Descriptions:
**
*******************************************************************************************************
*/
#include  "includes.h"

//#define BEEP    (1 << 7)


/* ���庯��ָ�� */
#include "LP2368_flash.h"
void (*IAP_Entry)(uint32 param_tab[], uint32 result_tab[]);

uint32  paramin[8];                         // IAP��ڲ���������
uint32  paramout[8];                        // IAP���ڲ���������


void InitIAP( void )
{
    IAP_Entry = (void(*)())IAP_ENTER_ADRR;           // ��ʼ������ָ��IAP_Entry

    //  ReadParID();                                    // ������ID
    // BootCodeID();                                   // ��Boot�汾��

}


/*
*********************************************************************************************************
** �������ƣ�SelSector()
** �������ܣ�IAP��������ѡ���������50��
** ��ڲ�����sec1        ��ʼ����
**           sec2        ��ֹ����
** ���ڲ�����IAP����״̬��
**           IAP����ֵ(paramout������)
*******************************************************************************************************
*/
uint32  SelSector(uint8 sec1, uint8 sec2)
{
    paramin[0] = IAP_SELECTOR;               // ����������
    paramin[1] = sec1;                       // ���ò���
    paramin[2] = sec2;
    (*IAP_Entry)(paramin, paramout);         // ����IAP�������

    return(paramout[0]);                     // ����״̬��
}

/*
*******************************************************************************************************
** �������ƣ�RamToFlash()
** �������ܣ�����RAM�����ݵ�FLASH���������51��
** ��ڲ�����dst        Ŀ���ַ����FLASH��ʼ��ַ����512�ֽ�Ϊ�ֽ�
**           src        Դ��ַ����RAM��ַ����ַ�����ֶ���
**           no         �����ֽڸ�����Ϊ512/1024/4096/8192
** ���ڲ�����IAP����״̬��
**           IAP����ֵ(paramout������)
*******************************************************************************************************
*/
uint32  RamToFlash(uint32 dst, uint32 src, uint32 no)
{
    paramin[0] = IAP_RAMTOFLASH;             // ����������
    paramin[1] = dst;                        // ���ò���
    paramin[2] = src;
    paramin[3] = no;
    paramin[4] = IAP_FCCLK;
    (*IAP_Entry)(paramin, paramout);         // ����IAP�������

    return(paramout[0]);                     // ����״̬��
}

/*
*******************************************************************************************************
** �������ƣ�EraseSector()
** �������ܣ������������������52��
** ��ڲ�����sec1       ��ʼ����
**           sec2       ��ֹ����
** ���ڲ�����IAP����״̬��
**           IAP����ֵ(paramout������)
*******************************************************************************************************
*/
uint32  EraseSector(uint8 sec1, uint8 sec2)
{
    paramin[0] = IAP_ERASESECTOR;            // ����������
    paramin[1] = sec1;                       // ���ò���
    paramin[2] = sec2;
    paramin[3] = IAP_FCCLK;
    (*IAP_Entry)(paramin, paramout);         // ����IAP�������

    return(paramout[0]);                     // ����״̬��
}

/*
*******************************************************************************************************
** �������ƣ�BlankCHK()
** �������ܣ�������գ��������53��
** ��ڲ�����sec1       ��ʼ����
**           sec2       ��ֹ����
** ���ڲ�����IAP����״̬��
**           IAP����ֵ(paramout������)
*******************************************************************************************************
*/
uint32  BlankCHK(uint8 sec1, uint8 sec2)
{
    paramin[0] = IAP_BLANKCHK;               // ����������
    paramin[1] = sec1;                       // ���ò���
    paramin[2] = sec2;
    (*IAP_Entry)(paramin, paramout);         // ����IAP�������

    return(paramout[0]);                     // ����״̬��
}

/*
*******************************************************************************************************
** �������ƣ�ReadParID()
** �������ܣ�������գ��������53��
** ��ڲ�������
** ���ڲ�����IAP����״̬��
**           IAP����ֵ(paramout������)
*******************************************************************************************************
*/
uint32  ReadParID(void)
{
    paramin[0] = IAP_READPARTID;             // ����������
    (*IAP_Entry)(paramin, paramout);         // ����IAP�������

    return(paramout[0]);                     // ����״̬��
}

/*
*******************************************************************************************************
** �������ƣ�BootCodeID()
** �������ܣ���ȡboot����汾�ţ��������55��
** ��ڲ�������
** ���ڲ�����IAP����״̬��
**           IAP����ֵ(paramout������)
*******************************************************************************************************
*/
uint32 BootCodeID(void)
{
    paramin[0] = IAP_BOOTCODEID;             // ����������
    (*IAP_Entry)(paramin, paramout);         // ����IAP�������

    return(paramout[0]);                     // ����״̬��
}
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
BYTE *Get_ClassInfo_Table(uint8 id)
{
    return (BYTE *)(ARM_TABLE_ADDRESS + id * 256);
}

/*
*******************************************************************************************************
** �������ƣ�Compare()
** �������ܣ�У�����ݣ��������56��
** ��ڲ�����dst        Ŀ���ַ����RAM/FLASH��ʼ��ַ����ַ�����ֶ���
**           src        Դ��ַ����FLASH/RAM��ַ����ַ�����ֶ���
**           no         �����ֽڸ����������ܱ�4����
** ���ڲ�����IAP����״̬��
**           IAP����ֵ(paramout������)
*******************************************************************************************************
*/
uint32 Compare(uint32 dst, uint32 src, uint32 no)
{
    paramin[0] = IAP_COMPARE;                // ����������
    paramin[1] = dst;                        // ���ò���
    paramin[2] = src;
    paramin[3] = no;
    (*IAP_Entry)(paramin, paramout);         // ����IAP�������

    return(paramout[0]);                     // ����״̬��
}

void Test_Flash(void)
{
    BYTE buf[256];
    uint32 resualt;
    BYTE *p;
    InitIAP();
    resualt = ReadParID();                                  // ������ID
    resualt = BootCodeID();
    // ��Boot�汾��

    resualt = SelSector(13, 13);                            // ѡ������1
    resualt = EraseSector(13, 13);                          // ��������1
    p = (BYTE *)0x30000;
    memcpy(buf, p, 256);
    //  resualt= BlankCHK(13,13) ;      // �������1
    memset(buf, 0xaa, 256);
    resualt = SelSector(13, 13);                            // ѡ������1
    resualt = RamToFlash(0x30000, (uint32)&buf[0], 256);		// д���ݵ�����1
    resualt = Compare(0x30000, (uint32)&buf[0], 256);
    memcpy(buf, p, 256);
    //	resualt=SelSector(13,13);                               // ѡ������1
    //  resualt=EraseSector(13,13);                             // ��������1
    resualt = resualt;
}

/*
*******************************************************************************************************
** �������ƣ�main()
** �������ܣ�IAP�������ã�����
*******************************************************************************************************
*/
/*
int  main(void)
{
    uint8 err;
    uint32 i;

    PINSEL0 = 0x00000000;			// �ܽ�����GPIO
    IO0DIR = BEEP;
    IO0SET = BEEP;

    IAP_Entry = (void(*)())IAP_ENTER_ADR;           // ��ʼ������ָ��IAP_Entry

    ReadParID();                                    // ������ID
    BootCodeID();                                   // ��Boot�汾��

    SelSector(1,1);                                 // ѡ������1
    EraseSector(1,1);                               // ��������1
    BlankCHK(1,1) ;                                 // �������1

    SelSector(1,1);                                 // ѡ������1
    RamToFlash(0x00001000, 0x40001000, 256);        // д���ݵ�����1
    err = Compare(0x00001000, 0x40001000, 256);     // �Ƚ�����

    if(err == CMD_SUCCESS)
    {	// �ɹ�������һ�� //
        IO0CLR = BEEP;
        for (i=0; i<0x3FFFFF; i++);
        IO0SET = BEEP;
    }
    else
    {	// ʧ�ܣ�һֱ���� //
        IO0CLR = BEEP;
    }

    while(1);
    return(0);
}
*/

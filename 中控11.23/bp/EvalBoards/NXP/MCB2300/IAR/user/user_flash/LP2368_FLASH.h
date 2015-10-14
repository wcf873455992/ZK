#ifndef _LP2368_FLASH_H_
#define _LP2368_FLASH_H_

//���� LPC23xx_us_en.pdf ��27�� table 483 ����ʹ��flash���
// 0 - 128K ��bootloader	0-10 ���� (0x0000 0000 -0x0001FFFF)
// 128K - 496K ��������	11-25���� (0x0002 0000 -0x0007BFFF)
// ���4K��������Ϣ		26-27���� (0x0007 C000 -0x0007DFFF)


#define ARM_TABLE_ADDRESS		0x30000
#define ARM_TABLE_SECTOR_START	        13
#define ARM_TABLE_SECTOR_END		14



#define IAP_ENTER_ADRR   0x7FFFFFF1  // IAP��ڵ�ַ����

/* ����IAP������ */
//   ����           ����
#define     IAP_SELECTOR        50  // ѡ������     ����ʼ�����š����������š�
#define     IAP_RAMTOFLASH      51  // ��������     ��FLASHĿ���ַ��RAMԴ��ַ��д���ֽ�����ϵͳʱ��Ƶ�ʡ�
#define     IAP_ERASESECTOR     52  // ��������     ����ʼ�����š����������š�ϵͳʱ��Ƶ�ʡ�
#define     IAP_BLANKCHK        53  // �������     ����ʼ�����š����������š�
#define     IAP_READPARTID      54  // ������ID     ���ޡ�
#define     IAP_BOOTCODEID      55  // ��Boot�汾�� ���ޡ�
#define     IAP_COMPARE         56  // �Ƚ�����     ��Flash��ʼ��ַ��RAM��ʼ��ַ����Ҫ�Ƚϵ��ֽ�����

/* ����IAP����״̬�� */
#define     CMD_SUCCESS          0
#define     INVALID_COMMAND      1
#define     SRC_ADDR_ERROR       2
#define     DST_ADDR_ERROR       3
#define     SRC_ADDR_NOT_MAPPED  4
#define     DST_ADDR_NOT_MAPPED  5
#define     COUNT_ERROR          6
#define     INVALID_SECTOR       7
#define     SECTOR_NOT_BLANK     8
#define     SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION 9
#define     COMPARE_ERROR        10
#define     BUSY                 11


/* ����CCLKֵ��С����λΪKHz */
#define  IAP_FCCLK           (Fcclk/1000)//40000// Fcclk
void InitIAP( void );

uint32  SelSector(uint8 sec1, uint8 sec2);
uint32  RamToFlash(uint32 dst, uint32 src, uint32 no);
uint32  EraseSector(uint8 sec1, uint8 sec2);
uint32  BlankCHK(uint8 sec1, uint8 sec2);
uint32  Compare(uint32 dst, uint32 src, uint32 no);
uint32  ReadParID(void);
uint32  BootCodeID(void);
BYTE *Get_ClassInfo_Table(uint8 id);
void Test_Flash(void);
#endif//_LP2368_FLASH_H_

#define ETHERNET_GLOBALS
#include "includes.h"
#include "../include/cfg_net.h"

/////////////////////////////////////////////////////////////////////
//	�������ƣ�void Ethernet_SendFrame
//	�������ܣ�IP��ARP���ݰ�����ETHERNET��ͷ�������͡�
//	��ڲ�����struct _pkst *TxdData,//�ṹָ��
//			uint8 * de_mac,			//�Է���MAC��ַָ��
//			uint8 PROTOCOL			//IPЭ���ARPЭ��
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void Ethernet_SendFrame(
    struct _pkst *TxdData,	//�ṹָ��
    uint8 *des_mac,		//�Է���MAC��ַָ��
    uint8 PROTOCOL			//IPЭ���ARPЭ��
)
{
#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register        */
    OS_CPU_SR  cpu_sr;                       /* Prevent compiler warning                        */
#endif
    ipethernet ethernet_head; //ETHERNET��������
    struct _pkst PKethernet;
    BYTE *p;
    p = ARP_Rec_Find(Get_DesIp(DEVICE_SUB_CLASS));
    memcpy((BYTE *)ethernet_head.DestMacId, des_mac, 6); //���öԷ�MAC
    memcpy((BYTE *)ethernet_head.SourceMacId, p, 6);
    if(PROTOCOL == IP_PACKED)					//�����IP��������Ϊ0X0800
        ethernet_head.NextProtocal = 0X0008;
    else if(PROTOCOL == ARP_PACKED)			//�����ARP��������Ϊ0X0806
        ethernet_head.NextProtocal = 0X0608;	//0X0806;
    PKethernet.STPTR = TxdData;				//ָ��ǰһ���ṹ����
    PKethernet.length = 14;					//ETHERNET��ͷ�ĳ���
    PKethernet.DAPTR = (uint8 EX_RAM *)&ethernet_head; //ETHERNET��ͷ��ָ��
    OS_ENTER_CRITICAL();		//����
#if DBG_TEST >0
    Send_Packet(&PKethernet);	//���͸�֡
#endif
    OS_EXIT_CRITICAL();			//�ָ�
}

////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ������ж����ݱ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint8 Ethernet_ReceivePacket(uint8 *RecData)
{
    if(((ipethernet *)RecData)->NextProtocal == 0x0608) //���Լ��ٶ����ݻ���ȡ������
    {
        RecData = RecData + 14;
        ARP_ReceivePacket(RecData);
    }
    else if(((ipethernet *)RecData)->NextProtocal == 0x0008) //���Լ��ٶ����ݻ���ȡ������
    {
        RecData = RecData + 14;

        IP_ReceivePacket(RecData);
    }
    return TRUE;
}

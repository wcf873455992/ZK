
#include "includes.h"
#include "main.h"
#include "user_debug.h"

#define TASK_UART0Send_ID        	17
#define TASK_UART0Send_PRIO   	17

#define TASK_UART1Send_ID        	18
#define TASK_UART1Send_PRIO   	18

#define TASK_UART2Send_ID        	19
#define TASK_UART2Send_PRIO   	19

#define TASK_UART3Send_ID        	20
#define TASK_UART3Send_PRIO   	20

#define TASK_UART4Send_ID        	21
#define TASK_UART4Send_PRIO   	21

#define TASK_UART5Send_ID        	22
#define TASK_UART5Send_PRIO   	22

#define TASK_UART8Send_ID        	23
#define TASK_UART8Send_PRIO   	23

#define TASK_UART9Send_ID        	24
#define TASK_UART9Send_PRIO   	24

#define TASK_UART_ID        		25
#define TASK_UART_PRIO      	25

#define TASK_START_ID       		26
#define TASK_START_PRIO   	26

#define TASK_RUN_ID       		27
#define TASK_RUN_PRIO   	27



////////////////////////////////////////////////////

#define TASK_START_STACK_SZIE	400//400
#define TASK_UART_STACK_SZIE	 300//       300
#define TASK_RUN_STACK_SZIE	  300//      300

#define TASK_UART0Send_STACK_SZIE	100
#define TASK_UART1Send_STACK_SZIE	100
#define TASK_UART2Send_STACK_SZIE	100
#define TASK_UART3Send_STACK_SZIE	100
#define TASK_UART4Send_STACK_SZIE	100
#define TASK_UART5Send_STACK_SZIE	180

#define TASK_UART8Send_STACK_SZIE	180
#define TASK_UART9Send_STACK_SZIE	180

OS_STK TASK_START_stack[TASK_START_STACK_SZIE];
OS_STK TASK_UART_stack[TASK_UART_STACK_SZIE];
OS_STK TASK_RUN_stack[TASK_RUN_STACK_SZIE];
OS_STK TASK_UART0Send_stack[TASK_UART0Send_STACK_SZIE];
OS_STK TASK_UART1Send_stack[TASK_UART1Send_STACK_SZIE];
OS_STK TASK_UART2Send_stack[TASK_UART2Send_STACK_SZIE];
OS_STK TASK_UART3Send_stack[TASK_UART3Send_STACK_SZIE];
OS_STK TASK_UART4Send_stack[TASK_UART4Send_STACK_SZIE];
OS_STK TASK_UART5Send_stack[TASK_UART5Send_STACK_SZIE];

OS_STK TASK_UART8Send_stack[TASK_UART8Send_STACK_SZIE]; //
OS_STK TASK_UART9Send_stack[TASK_UART9Send_STACK_SZIE]; //
void TASK_START(void *pdata);
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
int main (void)
{
    uint8 err;

    IO_FlagLingt_Flash();
    TargetResetInit();
    IO_FlagLingt_Flash();
    UART_printf("User main--Run\n");
    OSInit();
    memset(&TASK_START_stack[0], 0xdc, TASK_START_STACK_SZIE);
    memset(&TASK_UART_stack[0], 0xdc, TASK_UART_STACK_SZIE);
    memset(&TASK_RUN_stack[0], 0xdc, TASK_RUN_STACK_SZIE);
    memset(&TASK_UART0Send_stack[0], 0xdc, TASK_UART0Send_STACK_SZIE);
    memset(&TASK_UART1Send_stack[0], 0xdc, TASK_UART1Send_STACK_SZIE);
    memset(&TASK_UART2Send_stack[0], 0xdc, TASK_UART2Send_STACK_SZIE);
    memset(&TASK_UART3Send_stack[0], 0xdc, TASK_UART3Send_STACK_SZIE);
    memset(&TASK_UART4Send_stack[0], 0xdc, TASK_UART4Send_STACK_SZIE);
    memset(&TASK_UART5Send_stack[0], 0xdc, TASK_UART5Send_STACK_SZIE);

    memset(&TASK_UART8Send_stack[0], 0xdc, TASK_UART8Send_STACK_SZIE);
    memset(&TASK_UART9Send_stack[0], 0xdc, TASK_UART9Send_STACK_SZIE);
    /**/
    err = OSTaskCreateExt(TASK_START, (void *)0, &TASK_START_stack[TASK_START_STACK_SZIE - 1]
                          , TASK_START_PRIO, TASK_START_ID, &TASK_START_stack[0], TASK_START_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_START_PRIO, "User option", &err);

    OSStart();
    err = err;
    while(1);                                          /* Start multitasking    */
}

void TASK_START(void *pdata)
{
    /*�ڵ�1�����������ö�ʱ��0���ж�*/
    uint8 err;//,buf[16],tmp;
    uint32 i;
    pdata = pdata;
    IO_FlagLingt_Flash();
    UART_printf("User TASK_START--Run\n");
    TargetInit();
    //AVCD22M3494Reset();
    IO_FlagLingt_Flash();
    //VGACD22M3494Reset();
    User_mem_init();
    Option_init( );//��Ϣ����

#if   DBG_TEST	>	0
    InitNet();
    IO_FlagLingt_Flash();
    Initial_arp();
    for (i = 0; i < 400000; i++)
        IO_FlagLingt_Flash();
#endif
    //��ʼ������ͨѶ����
    XCOM_init();//ͨѶ����
    //UART_printf("XCOM_init\n");
    err = OSTaskCreateExt(TASK_UART, (void *)0, &TASK_UART_stack[TASK_UART_STACK_SZIE - 1]
                          , TASK_UART_PRIO, TASK_UART_ID, &TASK_UART_stack[0], TASK_UART_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_UART_PRIO, "User UART", &err);
    err = OSTaskCreateExt(TASK_UART0Send, (void *)0, &TASK_UART0Send_stack[TASK_UART0Send_STACK_SZIE - 1]
                          , TASK_UART0Send_PRIO, TASK_UART0Send_ID, &TASK_UART0Send_stack[0], TASK_UART0Send_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_UART0Send_PRIO, "TUART0_MB", &err);
    err = OSTaskCreateExt(TASK_UART1Send, (void *)0, &TASK_UART1Send_stack[TASK_UART1Send_STACK_SZIE - 1]
                          , TASK_UART1Send_PRIO, TASK_UART1Send_ID, &TASK_UART1Send_stack[0], TASK_UART1Send_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_UART1Send_PRIO, "TUART1_TYJ", &err);
    err = OSTaskCreateExt(TASK_UART2Send, (void *)0, &TASK_UART2Send_stack[TASK_UART2Send_STACK_SZIE - 1]
                          , TASK_UART2Send_PRIO, TASK_UART2Send_ID, &TASK_UART2Send_stack[0], TASK_UART2Send_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_UART2Send_PRIO, "TUART2_SOUND", &err);
    err = OSTaskCreateExt(TASK_UART3Send, (void *)0, &TASK_UART3Send_stack[TASK_UART3Send_STACK_SZIE - 1]
                          , TASK_UART3Send_PRIO, TASK_UART3Send_ID, &TASK_UART3Send_stack[0], TASK_UART3Send_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_UART3Send_PRIO, "TUART3_TB", &err);
    err = OSTaskCreateExt(TASK_UART4Send, (void *)0, &TASK_UART4Send_stack[TASK_UART4Send_STACK_SZIE - 1]
                          , TASK_UART4Send_PRIO, TASK_UART4Send_ID, &TASK_UART4Send_stack[0], TASK_UART4Send_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_UART4Send_PRIO, "TUART4_DYX", &err);
    err = OSTaskCreateExt(TASK_UART5Send, (void *)0, &TASK_UART5Send_stack[TASK_UART5Send_STACK_SZIE - 1]
                          , TASK_UART5Send_PRIO, TASK_UART5Send_ID, &TASK_UART5Send_stack[0], TASK_UART5Send_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_UART5Send_PRIO, "TUART5_SCRAME", &err);
    err = OSTaskCreateExt(TASK_UART8Send, (void *)0, &TASK_UART8Send_stack[TASK_UART8Send_STACK_SZIE - 1]
                          , TASK_UART8Send_PRIO, TASK_UART8Send_ID, &TASK_UART8Send_stack[0], TASK_UART8Send_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_UART8Send_PRIO, "TUART8_SCRAME", &err);
    err = OSTaskCreateExt(TASK_UART9Send, (void *)0, &TASK_UART9Send_stack[TASK_UART9Send_STACK_SZIE - 1]
                          , TASK_UART9Send_PRIO, TASK_UART9Send_ID, &TASK_UART9Send_stack[0], TASK_UART9Send_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_UART9Send_PRIO, "TUART9_SCRAME", &err); /**/
    //��������Ӳ���ж�
    UART_init();
    err = OSTaskCreateExt(TASK_RUN, (void *)0, &TASK_RUN_stack[TASK_RUN_STACK_SZIE - 1]
                          , TASK_RUN_PRIO, TASK_RUN_ID, &TASK_RUN_stack[0], TASK_RUN_STACK_SZIE, (void *)0, 0);
    OSTaskNameSet(TASK_RUN_PRIO, "RUN", &err);
    OS_CPU_SR_FIQ_Dis();
    User_Option();
    err = err;
    //Temperaturepro();
    for (;;);
    //IP_printf("123");
}

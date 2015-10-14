#ifndef __USER_UDP_H
#define  __USER_UDP_H
#define 	CMD_ABLE_ICII 				0x3230424c
#define	IC2_COMMAND_PORT			7101		/// ����˿ڶ���
#define	IC2_COMMAND_MAXLEN		1024		/// һ�������������ݳ���
#define	IC2_COMMAND_BUF_COUNT	20		/// �������������
#define	IC2_COMMAND_HEADLEN		16		/// ����ͷ����
#define 	CMD_AD_SENDMSG     		0x00010000		//һ������͵���Ϣ
#define 	CMD_AD_RECVMSG     		0x00010001		//���͸�һ�������Ϣ
#define	 CMD_SENDER_AD   			0x14//һ���
#define	 CMD_SENDER_ACK			0x80//
//add 2015.5.7,
#define IC2_COMMAND_PORT_K8_PLUS        9000
/**
@brief �����ֶ��壺
��������4���ֽڣ����·���ֻΪ���ڹ��������ֲ��������壬����ǿ�����ƣ��Ծ��嶨��Ϊ׼
0x00000000-0x0000FFFF  Ϊ��������ǰϵͳ�ĳ�ͻ��Ԥ�Ա���
0x00010000-0x0001FFFF  һ���Ӳ������������
0x00020000-0x0002FFFF  ���ء�������������ǽ�ȿ��Ʋ�����
0x00030000-0x0003FFFF  ��ý�塢��ʦ����Ӧ�ò�����
*/
typedef struct tagIC2CMD_CmdID
{
    DWORD	dwCommand;		/// ������
    DWORD   dwFlag;			/// �����־��Ĭ�Ͽɶ�Ϊ#define FLAG_NULL 0x00000000
    /**
    @brief  GUID
    ���ػ����ǽ�����User ID
    */
    char 	cUserID[10];
    /**
    @brief  GUID
    �����ض�һ���������Ϣʱ����GUIDΪ���ջ���һ�����GUID
    */
    char 	cGUID[10];
} IC2CMD_CmdID;


typedef struct tagIC2CMD_Header
{
    DWORD dwSyncHead;			/// ͬ��ͷ #define CMD_ABLE_ICII 0x4c423032
    DWORD	dwFromIP;			/// ԴIP
    DWORD	dwToIP;			/// Ŀ��IP
    BYTE	byteFrameIndex;		/// �����(0-255ѭ��,�ط�������)
    /**
    @brief Э�����Ͱ���������ǻظ��Ķ��壬�������ַ����ߵĽ�ɫ
    byteProtocolType&0x7f ���ֽ�ɫ
    byteProtocolType&0x80 ��������Ӧ��������
    */
    BYTE	byteProtocolType;		/// Э������
    WORD	wDataLen;			/// ��һ���ֵ����ݳ���
    IC2CMD_CmdID Cmd;
} IC2CMD_Header;
#define AUDIO_PORT 		3002
#define AUDIO_CMD_LEN 	512
#define AUDIO_DATA_LEN 	(AUDIO_CMD_LEN-8)
#define CMD_ABLE_AUDIO 0xaaa55a55
/**
@brief ����ͷ����
*/
typedef struct tagAudio_Header
{
    DWORD	dwSyncHead;			/// ͬ��ͷ
    DWORD	dwFromIP;			/// ��ƵԴIP
} Audio_Header;

//������԰���Ӧ
struct LAN_TEST_DATA
{
    uint8 flag[4];
    uint8 sip[4];

};


//////////////////////////////////////////////////////////////
void LAN_Test_Ack(uint8 *pdata, uint16 lg);
void UDP_Send_CMD(BYTE *pdata, uint16 lg, uint8 *des_ip);
void UDP_msg_rev(uint8 *p);
uint8 UDP_Read(BYTE *pdata, uint16 lg, uint8 *des_ip, uint8 WaitType, uint8 KeyWord);
uint8 Check_UDP_Paket(void *pdata, uint16 lg);
#endif

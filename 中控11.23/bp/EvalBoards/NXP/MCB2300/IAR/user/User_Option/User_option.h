#ifndef _USER_OPTION_H
#define  _USER_OPTION_H


#define Debug_MSG_Log         	1
#define Debug_Scamera_Log  	0

///////////////////////////////////////
#define DEVICE_COMMTYPE_NONE	0
#define DEVICE_COMMTYPE_HW		1
#define DEVICE_COMMTYPE_XCOM	2
#define DEVICE_COMMTYPE_UDP		3

/////////////////�������ò���///////////////////////////
#define Device_FunctionMode_YY    	0  //��������̨����
#define Device_FunctionMode_LB		1//¼������
#define Device_FunctionMode_TB    	2//ͬ��������
#define Device_FunctionMode_KBRY  	3  //��ҫ����
#define Device_FunctionMode_KBKR  	4//�����
#define Device_FunctionMode_CARD  5 //У԰һ��ͨ
/////////////////�Ͽ�״̬////////////////////////////////////
#define CLASS_OVER            		0
#define CLASS_BEGIN_KEY		1
#define CLASS_BEGIN_RF		2
#define CLASS_BEGIN_TIME		3
#define CLASS_BEGIN_UDPCMD	4
#define CLASS_BEGIN_FINGERPRINT 5        //ָ�ƻ�
/////////////////////////////////////////////////////
#define KB_TYPE_NO				    0
#define KB_TYPE_RONGYAO               1
#define KB_TYPE_KUREI			    2
#define KB_TYPE_ZHONGKONG             3



struct BASE_MSG
{
    uint8 device;
    uint8 option;
};
struct BASE_TIME
{
    uint8 hour;
    uint8 min;
};
//����ͨѶ����
//����ͨѶ����
#define COMM_POCTL_def		0
#define COMM_POCTL_YTJ		1
#define COMM_POCTL_PD		2
#define COMM_POCTL_UDP		4
#define COMM_POCTL_ICMP	8
#define COMM_POCTL_ARP		16

struct COMM_IP
{
    uint8 IP[4];		//IP
    uint8 Mask[4];
};
struct COMM_UDP
{
    uint8 IP[4];		//IP
    //	uint8 Mask[4];
    uint16 port;		//�˿�
    uint16 poctl;		//Э��
};
struct COMM_COM
{
    uint8 PortNum;
    uint8 address;
    uint16 BPS;
    uint16 PROTOCOL;
    uint8 linecontrl;
    uint8 Hbps;

};
//ͨѶ����
union COMM_VAL
{
    struct COMM_COM xcom;
    struct COMM_UDP   udp;
};

//DEV_VGA_LINK
struct DEV_VGA_LINK
{
    uint8 VGAIn;		//�豸����һ���VGA����ͨ��
    uint8 VGAOut;		//�豸����һ���VGA���ͨ��
    uint8 VGALink;		//���ͨ��Ĭ������
    uint8 vgatype;		//VGA����
};
//DEV_VIDEO_LINK
struct DEV_VIDEO_LINK
{
    uint8 VideoIn;		//�豸����һ���VIDEO����ͨ��
    uint8 VideoOut;		//�豸����һ���VIDEO���ͨ��
    uint8 VideoLink;	//���ͨ��Ĭ������
    uint8 VideoType;	//VIDEO����
};
//DEV_SOUND_LINK
struct DEV_SOUND_LINK
{
    uint8 SoundIn;		//�豸����һ���SOUND����ͨ��
    uint8 SoundOut;		//�豸����һ���SOUND���ͨ��
    uint8 SoundLink;	//���ͨ��Ĭ������
    uint8 SoundVol;		//��Ƶ����
};
//DEV_IN_LINK
struct DEV_IN_LINK
{
    uint8 in_no;		//IO����ͨ����
    uint8 in_type;		//IO����
    uint8 in_sound;	//��Ƶ��
    uint8 bak;		//����
};
//DEV_OUT_LINK
struct DEV_OUT_LINK
{
    uint8 out_no;
    uint8 out_type;
    uint8 io_sound;
    uint8 bak;
};
//DEV_POWER_LINK
struct DEV_POWER_LINK
{
    uint8	 dev_id;
    uint8 power_no;
};
//DEV_COMM_LINK
struct DEV_COMM_LINK
{
    uint16 comm_type;
    union COMM_VAL val;
};
///////�豸���Ͷ���/////////////////////////////
#define DEVICE_TYPE_CLASS_INFO      			 0
#define DEVICE_TYPE_CLASS_OPEN_INFO		 1
#define DEVICE_TYPE_CLASS_CLOSE_INFO		 2
#define DEVICE_TYPE_CLASS_TABLE_START		 3
#define DEVICE_TYPE_CLASS_TABLE_END		 9

///////////////���һ�����Ϣ///////////////////////////////
//���һ�������------��Ӧ����0ҳ
struct CLASS_INFO
{
    uint8 dev_id;				//�豸ID
    uint8 dev_type;				//�豸����
    uint8 school_name[32];		//ѧУ����
    uint8 house_name[32];		//��ѧ¥
    uint8 class_name[32];		//����
    uint16 class_type;			//���ܽ�������
    uint8 class_ID[8];			//����ý������ʶ����
    struct COMM_IP class_ip;	//����
    struct COMM_IP Gateway_ip;	//����
    struct COMM_IP Ser_ip;		//���ط�����
    struct BASE_TIME alm_on;	//��������
    struct BASE_TIME alm_off;	//��������
    uint8 configtime[4];           //���ñ����ô���
    uint8 CurrsScameraPage; //�����������豸���õ�ҳ�ţ�
    uint8 OpenType;              //���ػ�����
    uint8 BJCardIP[4];          //�༭��IP
    uint8 BMCard1IP[4];//���뿨1IP
    uint8 BMCard2IP[4];//���뿨2IP
    uint8 LMT_IP[4];     //��ý��IP
    uint8 PC_IP[4];    //��ʦPC��IP
    uint8 Def_IP[4];//������IP
    uint8 Def_1;//����
    uint8 Card_Contrl;//У԰һ��ͨ����
    uint8 YuYinCtrl;//�������ܿ���

};
//һ�����������------��Ӧ����1ҳ
struct CLASS_OPION_TYPE
{
    struct BASE_MSG msg;
    uint16  del_time;

};
struct CLASS_OPEN_INFO
{
    uint8 dev_id;					//�豸ID
    uint8 dev_type;					//�豸����
    struct BASE_MSG  msgon1;		//����������Ϣ1
    struct BASE_MSG  msgon2;		//����������Ϣ2
    struct BASE_MSG  msgon3;		//����������Ϣ3
    //struct CLASS_TABLE_REC *base;	//����������Ϣ��
};
//һ����ػ�����------��Ӧ����2ҳ
struct CLASS_CLOSE_INFO
{
    uint8 dev_id;						//�豸ID
    uint8 dev_type;						//�豸����
    struct BASE_MSG  msgoff1;			//�ػ�������Ϣ1
    struct BASE_MSG  msgoff2;			//�ػ�������Ϣ2
    struct BASE_MSG  msgoff3;			//�ػ�������Ϣ3
    //	struct CLASS_TABLE_REC *base;		//�ػ�������Ϣ��
};

//�豸������Ϣ
struct DEVICE_INFO
{
    uint8 dev_id;					//�豸ID
    uint8 dev_type;					//�豸����
    uint8 dev_name[32];				//�豸����
    uint8 dev_type_name[32];		//�豸�ͺ�
    struct DEV_VGA_LINK	 VGA;		//VGA������Ϣ
    struct DEV_VIDEO_LINK  VIDEO;	//��Ƶ�ź�������Ϣ
    struct DEV_SOUND_LINK SOUND; 	//��Ƶ�ź��ٽ���Ϣ
    struct DEV_IN_LINK	 IN;		//IO����������Ϣ
    struct DEV_OUT_LINK	OUT;		//�������
    struct DEV_POWER_LINK POWER;	//��Դ���Ӷ�i
    struct DEV_COMM_LINK  COMM;		//ͨѶ����
    struct BASE_MSG KB_MSG;			//�豸Ͷ��ѡ�������Ϣ
    struct BASE_MSG ON_MSG;			//����������Ϣ
    struct BASE_MSG OFF_MSG;		//�ػ�������Ϣ
    uint16 OFF_TIME;				//�ػ���ʱ
    uint16  prev_val;				//�豸��˽������
};
//��Ϣ�ṹ
struct MESSAGE_TYPE
{
    uint8  dev_id;					//�豸ID
    uint8 dev_type;					//�豸����
    uint8 DEVICE;					//Ŀ���豸
    struct BASE_MSG sys_msg;		//��Ϣ
    uint8 next_id;					//��չ����
    uint8 data_lg;					//���ݳ���
};
//����TASK
#define TASK_DELAY_MAX_SIZE  20
struct TASK_REC_TYPE
{
    uint8 TimeType;
    uint8 DelayTime;
    uint8 bak_time;
    uint8 repeat;
    uint16 msg_lg;
    void *pmsg;
};
struct TASK_TYPE
{
    uint8 RecCount;
    //uint8 RunPointer;
    //uint8 SavePointer;
    struct TASK_REC_TYPE DelayRec[TASK_DELAY_MAX_SIZE];
};

//��Ϣ���е�ִ��
#define CLASS_TASK_FLAG_RUN	1
#define CLASS_TASK_FLAG_OVER	(1<<1)
#define CLASS_TASK_OK		0x80
#define CLASS_TASK_ERR_ID    0x81
#define CLASS_TASK_EMPTY	0x82
#define CLASS_TASK_BUSY        0x83
struct MSG_OPTION_Q_TYPE
{
    uint8 Flag;
    uint8 Curr_id;
    struct BASE_MSG ClassOpen[3];
    struct BASE_MSG ClassClose[3];
    uint8 ClassTaskNum;
    uint8 ClassCurrNUm;
    uint8 TIME_LED_TYPE;
    uint8 TIME_LED_STATE;
};
struct DEVICE_TYJ_TYPE
{
    uint8 comm;
    uint8 curr_Type;	//��ǰͶӰ������1=VGA 0=video
    uint8 vgain;
    uint8 videoin;
};
struct POWER_STATE
{
    uint32 state;
};

struct MSG_OPTION_COMM_TYPE
{
    struct BASE_MSG msg;
    BYTE Frame;
    BYTE PortNum;
    WORD lg;
};
struct MSG_REV_TYPE
{
    uint16 Type;
    uint16 num;
    uint8 des_ip[4];

};

///Ӧ�ô��������ж�����
#define START_VIC_ADDRESS  (BYTE *)0x40000000

//EEPROM24C16��ַ
#define PROGRAM_FLAG_ADDRESS	0x0000
#define USER_ID_ADDRESS		0x0060
#define DEVICE_USE_STAT_TIME	0x0080
#define CLASS_TABLE_ADDRESS	0x0100//�γ̱�0x100-0x7ff

//Function define

//���������
#define DCS_NONE		0
#define DCS_LCJ_OC39            1//��ʿ������OC39�����

#define DCS_CONTRL_ON_OFF 	 	1
#define DCS_CONTRL_START_OVER   (1<<1)
#define DCS_JX_OPEN				(1<<2)

struct DEVICE_DCS_TYPE
{
    uint8 Type;			//���������
    uint8 Contrl_State;       //����״̬
    uint8 check_state_in;   //������źż��


};

struct gDevice_ADD
{
    uint8 IP[4];
    uint8 State;
};
/**/


uint32 Random (void);
BYTE BinToChar(BYTE data);
void Option_init(void);
void ClassInfo_init(void);
BYTE User_ID_Read(BYTE *buf, uint8 type);
uint8 Device_Rec_Scan(void);
void *Device_Rec_Find(BYTE *pdata, uint8 Find_Type);
uint8 Alarm_Link(void);
BYTE Class_Task_Start(BYTE id);
BYTE Task_Rec_Find(BYTE *pdata, BYTE lg);
BYTE Task_Rec_Del(BYTE *pdata, BYTE lg);
uint8 Task_Run_Delay(uint16 DelayTime, uint8 *pdata, uint16 lg, uint8 repeat, uint8 *prev);
uint8 Msg_send(uint8 *pdata, uint16 lg, uint8 *prev);
BYTE Msg_Option_Find(void *pdata);
void User_Option(void);
BYTE Get_Debug(uint16 flag);
void *Get_Device_Status(void);
uint8 Check_FunctionMode(uint8 FunctionMode);
void Run_Send_Flag(void);
void TASK_RUN(void *pdata);
void CameraPowerInit(void);
void BJ_Card_init(void);
void Run_Send_Flag(void);
uint8 Device_DCS_Check_in(uint8 val);
void Device_DCS_init(void);
void Device_DCS_Contrl(void);
void LMT_init(uint16 Power_no);
void LMT_Delay_Power(void);

uint16 Get_ver(void);
uint8 Get_Class_Stauts(void);



//Variable declare
extern struct DEVICE_STAT_USE_TYPE gStat_Rec;
extern struct DEVICE_STATUS_TYPE gDeviceStatus; //һ���״̬
extern struct DEVICE_REC_TYPE gDeviceTable;	  //�豸�ǼǱ�
extern struct TASK_TYPE TASK_TABLE;			 //��ʱ�����

#endif


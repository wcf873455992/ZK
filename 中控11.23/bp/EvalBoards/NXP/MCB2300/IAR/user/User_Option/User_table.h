#ifndef __USER_TABLE_H
#define __USER_TABLE_H

//////////////////////////////////////////////
#define DEV_REC_SIZE   		   256
#define DEV_REC_OPTION_ID      16
#define DEV_REC_OPTION_NOERR   0
#define DEV_REC_OPTION_NOFIND  1
#define DEV_REC_OPTION_ERR	   2
/////////////////////////////////////////////
#define DEV_FIND_ID			0
#define DEV_FIND_TYPE		1
#define DEV_FIND_SELECT		2
#define DEV_FIND_MSG		3
#define DEV_FIND_TIME		4
#define DEV_FIND_VGAIN		5
#define DEV_FIND_VGAOUT		6
#define DEV_FIND_VIDEOIN	7
#define DEV_FIND_VIDEOOUT	8
#define DEV_FIND_XCOM_PORT	9
#define DEV_FIND_XCOM_SubADDr  10
///////////////���ñ����ݼ�¼////////////////
//���ػ���Ϣ��¼
struct CLASS_OPTION_REC
{
    struct BASE_MSG MsgRec;
    uint16 OptionTime;
};
//�γ̱��¼
struct CLASS_TABLE_REC
{
    struct BASE_TIME  class_begin;	//�Ͽ�ʱ��
    struct BASE_TIME class_over;	//�¿�ʱ��
    uint8 class_name[16];			//�γ���
    uint8 teacher_name[10];			//��ʦ��
};
//���Լ�¼
struct STRATEGY_REC
{
    uint8 current_rec_frame; 		//��ǰ¼�Ƴ���
    uint8 current_use_frame;        //��ǰʹ��
    struct BASE_MSG touch_msg;    	//������Ϣ
    struct BASE_MSG decision_msg;	//������Ϣ
    uint16 msg_val;                 //������Ϣ����
    struct STRATEGY_REC *next;
};
//�������ݱ��¼
struct TRACK_REC
{
    uint16 rang; 	//����
    uint16 ZJ_zoom;	//�о�
    uint16 JJ_zoom;	//����
    uint16 TX_zoom;	//��д
    uint16 speed;	//�ٶ�
    struct TRACK_REC *next;
};
//��д����
struct TX_ZB
{
    uint16 x;	//��д����
    uint16 y;	//��д����
    uint16 d;	//��д�뾶
};
//���߻�����¼
struct HD_REC
{
    uint8 camera_id;			//Ŀ�������
    uint8 position;				//Ԥ��λ��
    struct BASE_MSG sys_msg;	//ϵͳ��Ϣ
    uint8 val;					//����λ����
    struct HD_REC *next;
};
//���鶨λ���ݼ�¼
struct BS_REC
{
    uint16 position1;		//����̽ͷ1��������
    uint16 position2;		//����̽ͷ2��������
    uint16 position3;		//����̽ͷ3��������
    uint16 position4;		//����̽ͷ4��������
    struct BS_REC *next;
};

//�γ̱������Ϣ-----��Ӧ����3-9ҳ
struct CLASS_TABLE_INFO
{
    uint8 dev_id;				//�豸ID
    uint8 dev_type;				//�豸����
    uint8 week;					//����
    //struct CLASS_TABLE_REC  rec[8];
    //struct CLASS_TABLE_REC *base;	//�γ̱������Ϣ��
};
//������Ϣ--------��Ӧ����10ҳ
struct TRACK_INFO
{
    uint8	dev_id;			//�豸ID
    uint8	dev_type;		//�豸����
    uint8	track_name[16];	//���Ŀ�������
    uint16	track_type;		//��������
    uint8  	OBJcamera_id;  	//Ŀ�������ID
    uint16	OBJcamera_x;	//Ŀ�����������X
    uint16	OBJcamera_y;	//Ŀ�����������Y
    uint16	OBJcamera_z;	//Ŀ�����������Z
    uint16	OBJcamera_H;	//Ŀ�����������ˮƽ��
    uint16	OBJcamera_V;	//Ŀ����������괹ֱ��
    struct TX_ZB tx1;		//��д1����
    struct TX_ZB tx2;		//��д2����
    struct TX_ZB tx3;		//��д3����
    struct TRACK_REC *base;	//���Ŀ��Ʊ�
};
//������ƻ�����Ϣ---��Ӧ����11ҳ-
struct BS_INFO
{
    uint8	dev_id;			//�豸ID
    uint8	dev_type;		//�豸����
    uint8   bs_name[16];	//�����������
    uint16  bs_type;		//�����������
    uint8   position;  		//�����̽ͷ����
    struct BS_REC *base;	//������Ʊ�
};
//���Ի�������Ϣ-----��Ӧ����12-13ҳ
struct STRATEGY_INFO
{
    uint8  dev_id;				//�豸ID
    uint8  dev_type;			//�豸����
    uint8  STRATEGY_name[16];	//���Ի�����
    uint16 STRATEGY_type;		//���Ի�����
    struct STRATEGY_REC *base;	//���Կ��Ʊ�
};
//���߻���������Ϣ---------��Ӧ����14-15ҳ
struct HD_INFO
{
    uint8	dev_id;			//�豸ID
    uint8	dev_type;		//�豸����
    uint8	hd_name[16];	//���߻�������
    uint16 	hd_type;		//���߻�������
    struct HD_REC *base;	//���߻������Ʊ�
};
//����ID
#define CLASS_ID_FLAG_ACTIVE	1
struct CLASS_ID_TYPE
{
    uint8  dev_id;			//�豸ID
    uint8 dev_type;			//�豸����
    uint16 flag;
    uint8 class_id[8];
};

//�豸ͳ����Ϣ����Ӧ���ñ�0xfa
struct DEVICE_STAT_REC_TYPE
{
    uint8 Device_Type;
    uint8 Device_Count;
    uint8 Device_Use_Time[6];
    uint32 Device_Stat_Time;
    uint32 Device_Stat_Count;
};

#define DEVICE_FILE_BIGIN			1
#define DEVICE_FILE_ERASESECTOR  	(1<<1)
#define DEVICE_FILE_BLANKCHK		(1<<2)
#define DEVICE_FILE_HALF			(1<<3)
struct DEVICE_REC_FILE_TYPE
{
    uint16 FileType;
    uint16 FilePage;
    uint8  FileFlag;
    uint8 FileDebug;
    uint8 *FileBuf;	//��������ָ��
};

#define DEV_DEBUG_SYSTEM	1  //0x01
#define DEV_DEBUG_MSG		(1<<1)//0x02
#define DEV_DEBUG_KEY		(1<<2) //0x04����
#define DEV_DEBUG_HW		(1<<3)  //0x08���ں��������������
#define DEV_DEBUG_XCOM 		(1<<4) //0x10���ڴ��ڲ���
#define DEV_DEBUG_POWER		(1<<5) //0x20��Դ������Ϣ���
#define DEV_DEBUG_IO 		(1<<7)//0x80����IO�����������
#define DEV_DEBUG_COMDATE 	0x40//���ڳ����������д�ӡ�����������
#define DEV_MAX_SIZE 		50
struct DEVICE_TYPE
{
    uint8 id;
    uint8 type;
    uint8 state;
};

#define DEV_FLAG_SVER_LINK   1
struct DEVICE_REC_TYPE
{
    uint8 DeviceCount;
    uint8 OptionCount;
    uint8 TB;
    uint8 Debug;
    uint16 flag_sver;
    uint16 Alarm_Type;
    uint16 Alarm_Flag;
    uint16 Device_link;
    uint8  device_link_times[4];
    struct MSG_OPTION_Q_TYPE Class;
    struct DEVICE_REC_FILE_TYPE File;
    struct DEVICE_TYJ_TYPE TYJ[2];//ͶӰ��
};

//һ�������״̬WORD
#define DEV_LINK_PCON   	1		//�μ��������
#define DEV_LINK_PCLK     	(1<<1)	//�μ������δ����
#define DEV_LINK_PCOK   	(1<<2)	//�μ������������δ����
#define DEV_LINK_LMTON 		(1<<4)	//��ý��������
#define DEV_LINK_LMTLK		(1<<5)	//��ý������δ����
#define DEV_LINK_LMTOK		(1<<6)	//��ý���������δ����
#define DEV_LINK_LIGHT		(1<<7)	//���ڵƹ�
#define DEV_LINK_KBLK		(1<<8)	//����δ����
#define DEV_LINK_BJCardLK	(1<<9)	//�Զ����Ŀ�δ����
#define DEV_LINK_TBLK		(1<<10)	//��Ƶͬ����δ����
#define DEV_LINK_YYLK		(1<<11)	//��չ������δ����
#define DEV_LINK_BM1LK		(1<<12)	//��Ƶ���뿨1δ����
#define DEV_LINK_BM2LK		(1<<13)	//��Ƶ���뿨2δ����
#define DEV_LINK_GFON       (1<<14)	//���ŵ�Դ
#define DEV_LINK_TYJON      (1<<15)	//ͶӰ����Դ
//һ�������״̬1BYTE
#define DEV_STATE_CLASSMARK		0x0f
#define DEV_STATE_CLASSOVER		00 	//�ػ�/�¿�
#define DEV_STATE_CLASSKBSTAR 	01	//�Ͽμ�����
#define DEV_STATE_CLASSRFSTAR	02	//Rf������
#define DEV_STATE_CLASSTMSTAR	03	//�α�/��ʱ����
#define DEV_STATE_CLASSYCSTAR	04	//��̨�����

#define DEV_STATE_OPTIONMARK	0xf0
#define DEV_STATE_OPTIONSD		0x00	// �ֶ�
#define DEV_STATE_OPTIONAT		0x10	//�Զ�
#define DEV_STATE_OPTIONJGDEV	0x40	//�ӹ��豸
#define DEV_STATE_OPTIONJGREC	0x80	//�ӹ�¼��
#define DEV_STATE_OPTIONJG		0xC0      //�ӹ�
//�Ͽ�״̬1BYTE
#define DEV_CLASS_START			1		//�Ͽ�
#define DEV_CLASS_GP			1<<1	//����
#define DEV_CLASS_CL			1<<2	//����
#define DEV_CLASS_HD			1<<3	//����
#define DEV_CLASS_RECSTOP		0x00	//¼��ֹͣ
#define DEV_CLASS_RECPUSH		0x10	//¼����ͣ
#define DEV_CLASS_RECSTART		0x20	//¼�ƿ�ʼ
#define DEV_CLASS_FAULT 		0x40	//���ϣ�8168δ����
//��ǰ¼������1WORD
#define DEV_REC_ML500			0x0000	//����500K
#define DEV_REC_ML1M			0x0001	//����1M
#define DEV_REC_MLZDY			0x0002	//�����Զ���
#define DEV_REC_LZVGA			0x0000	//VGA
#define DEV_REC_LZD1			0x0010	//D1
#define DEV_REC_LZZDY			0x0020	//�Զ���
#define DEV_REC_TXCG			0x0000	//����
#define DEV_REC_TXHZH			0x0100	//���л�
#define DEV_REC_TXZDY			0x0200	//�Զ���
#define DEV_REC_SG160			0x0000	//����趨160
#define DEV_REC_SG170			0x1000	//����趨��160
#define DEV_REC_SG180			0x2000	//����趨��180
//��������1BYTE
#define DEV_BJ_IN1				1		//��������1
#define DEV_BJ_IN2				1<<1	//��������2
#define DEV_BJ_IN3				1<<2	//��������3
#define DEV_BJ_IN4				1<<3	//��������4
#define DEV_BJ_IN5				1<<4	//��������5
#define DEV_BJ_IN6				1<<5	//��������6
#define DEV_BJ_TYJ				1<<6	//ͶӰ������
//�������1BYTE
#define DEV_BJ_OUT1				1		//�������1
#define DEV_BJ_OUT2				1<<1	//�������2
#define DEV_BJ_OUT3				1<<2	//�������3

struct DEVICE_STATUS_TYPE
{
    uint8 DEV_ID[8];			//һ���ʶ����
    uint16 ver;					//һ����汾��
    uint16 KB_ver;				//���Ƽ��̰汾��
    uint16 TRACK_ver;  			//�Զ����ٰ屾��
    uint8  DEV_state;   		//һ�������״̬
    uint8  CLASS_state;			//�Ͽ�״̬
    uint16 Current_Device;		//��ǰʹ���豸
    uint16 Current_REC;			//��ǰ��ý��¼�Ƴ���
    uint32 Disk_size;			//��ý��洢����
    uint16 Current_REC_set;		//��ǰ¼������
    uint8  BJ_IN;				//��������
    uint8  BJ_out;				//�������
    uint16 DEV_Link_State;		//һ�������״̬
    uint16 Curr_SCAME;			//��ǰ�������[1-6]
    uint32 power_state[4];		//��Դ��״̬
    uint8  configtime[4];		//���ߵ����ô���
    uint16 Curr_Contrl_Device; 	//��ǰ�������豸
    uint16 POWER_ver;			//���Ƶ�Դ��汾��
    uint16 MK_Vol;        		//���������D15=1 ����
    uint32 BJ_BAK;                 //��չ����
    uint8  KB_Type;               //��������
    uint8  Power_Type ;        //��Դ������
    uint16 TB_CardVer;        //ͼ�񿨰汾��
    uint16 BJ_CardVer;         //�༭���汾��
    uint16 WD_Val;               //һ��������¶�
};

//�豸ʹ��ͳ��
#define STAT_DEVICE_OVER		1
#define STAT_DEVICE_TIME		2
struct DEVICE_STAT_USE_TYPE
{
    uint16 Device;
    uint16 flag[15];
    uint32 DEVICE_STAT_Start_Time[15];
    struct DEVICE_STAT_REC_TYPE Rec[15];
};

//Function define
uint32 File_Req(uint16 type, uint16 page, uint8 *des_ip);
void File_Table_Write(uint16 page, BYTE *buf, uint8 *des_ip);
void File_Table_Read(uint16 page, uint8 *des_ip);
void File_ClassTable_Write(uint16 FilePage, uint8 *buf, uint8 *des_ip);
void File_ClassTable_Write_OneDay(uint16 FilePage, uint8 *buf, uint8 *des_ip);
void File_ClassTable_Read(uint16 FilePage, uint8 *des_ip);
BYTE File_ClassTable_Read_CurDay(void);
BYTE Class_Table_Check(void);
void Device_Stat_Option(BYTE Device, BYTE option);
void Device_Stat_Save(void);
BYTE Device_Use_Stat_Read(void);
void File_UserProgram_Write(uint16 Page, BYTE *buf, uint8 *des_ip);

#endif


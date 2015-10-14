#ifndef __USE_KEY_H
#define  __USE_KEY_H
#define LED_CHANLE_VER		0
#define LED_CHANLE_DATA	1
#define LED_CHANLE_TYPE		2
#define KEY_SOUND_VOL		3
#define KEY_VAL				4
#define KEY_TIME				5
#define KEY_RF_DATA			6
#define KEY_CONTRL			7
#define KEY_Q_SIZE	10//���̻�������С
//��һ����ʾ
#define LED_PC_POWER			0x00000001	//�μ����Ե�Դ
#define LED_TYM				0x00000002  //ͶӰ���綯Ļ
#define LED_SHOW			0x00000004 //��Ƶչ̨
#define LED_REC				0x00000008 //¼��
#define LED_MODE_ZD			0x00000010 //ģʽ/�Զ�
#define LED_METRIC_D1			0x00000020 //�ֱ���D1
#define LED_SPEED_ZDY			0x00000040//����/�Զ���
#define LED_TAIL_160			0x00000080//���/160
#define LED_SCAMERA2			0x00000100//�����2
#define LED_SCAMERA6			0x00000200//�����6
//�п�����¼�
#define LED_POWER			0x00000400//����
#define LED_REC_STOP			0x00000800//¼��ֹͣ

//�ڶ�����ʾ
#define LED_LIGHT_POWER		        0x00010001 //���ڵƹ�
#define LED_LCD_UP			0x00010002//Һ������
#define LED_DVD				0x00010004 //DVD
#define LED_KZB				0x00010008//��չB
#define LED_MODE_SD			0x00010010 //ģʽ/�ֶ�
#define LED_METRIC_ZDY			0x00010020 //�ֱ���/�Զ���
#define LED_FRAME_CG			0x00010040 //��Ч/����
#define LED_TAIL_170			0x00010080//���/170
#define LED_SCAMERA3			0x00010100//�����3
#define LED_HELP			0x00010200 //����
//��3����ʾ
#define LED_GF_POWER			0x00020001 //���ŵ�Դ
#define LED_TYDJ			0x00020002 //ͶӰ������
#define LED_RECODER			0x00020004 //�豸¼����
#define LED_PC				0x00020008 //�豸/�����
#define LED_MODE_YC			0x00020010 //ģʽ/Զ��
#define LED_SPEED_400			0x00020020 //����400
#define LED_FRAME_HZH			0x00020040 //��Ч���л�
#define LED_TAIL_180			0x00020080 //���180
#define LED_SCAMERA4			0x00020100 //�����4
#define LED_MUTE			0x00020200  //����
//��4����ʾ
#define LED_TYJ_POWER			0x00030001 //ͶӰ����Դ
#define LED_PELMET_POWER		0x00030002 //�綯����
#define LED_KZA				0x00030004//��չA
#define LED_NPC				0x00030008 //�ʼǱ�
#define LED_METRIC_XGA			0x00030010 //�ֱ���XGA
#define LED_SPEED_1M			0x00030020 //����1M
#define LED_FRAME_ZDY			0x00030040 //��Ч/�Զ���
#define LED_SCAMERA1			0x00030080//�����1
#define LED_SCAMERA5			0x00030100//�����5
//����
#define KEY_CLASS_BEGIN			0x0001//�Ͽ�
#define KEY_MIC_MUTE			0x0002 //MIC_MUTE
#define KEY_PC_POWER			0x0003 //�μ����Ե�Դ
#define KEY_LIGHT_POWER		        0x0004//�μ����Ե�Դ
#define KEY_GF_POWER			0x0005//���ŵ�Դ
#define KEY_TYJ_POWER			0x0006//ͶӰ����Դ
#define KEY_DEVICE_UP			0x0007//����
#define KEY_DEVICE_STOP			0x0008//ֹͣ
#define KEY_DEVICE_DOWN		        0x0009//�½�
#define KEY_DEVICE_SELECT		0x000a//�豸ѡ��
#define KEY_HELP			0x000b//11 //����
#define KEY_DEVICE_PC			0x010c//12//�μ������
#define KEY_DEVICE_NPC			0x010d//13//�ʼǱ�
#define KEY_DEVICE_KZA			0x010e//14//
#define KEY_REC_PUSH			0x020f//15//¼��/��ͣ
#define KEY_REC_STOP			0x0210//16 //¼��ֹͣ
#define KEY_DEVICE_SHOW		        0x0111//17//��Ƶչ̨
#define KEY_DEVICE_DVD			0x0112//18//DVD
#define KEY_DEVICE_RECODER		0x0113//19//¼����
#define KEY_DEVICE_KZB			0x0114//20//��չB
#define KEY_DEVICE_MUTE			0x0015//21//�豸����
#define KEY_MODE_SELECT		        0x0216//22 //ģʽѡ��
#define KEY_METERIC_SELECT		0x0217//23//¼�Ʒֱ���
#define KEY_FRAME_SELECT		0x0218//24//������Ч
#define KEY_SPEED_SELECT		0x0219//25//¼������
#define KEY_TAIL_SELECT			0x021a//26//����趨
#define KEY_SCAMERA_SELECT		0x021b//27 //������趨
#define KEY_SCAMERA_ZOOM1		0x021c//28//Zoom+
#define KEY_SCAMERA_UP			0x021d//29//������
#define KEY_SCAMERA_ZOOM2		0x021e//30//ZOOM-
#define KEY_SCAMERA_LEFT		0x021f//31//������
#define KEY_SCAMERA_DOWN		0x0220//32//������
#define KEY_SCAMERA_RIGHT		0x0221//33//������
#define KEY_SCAMERA_STOP		0x0230//����ͣ
#define KEY_CONTRL_BACK		        0x0122//34//����
#define KEY_CONTRL_UP			0x0123//35//������
#define KEY_CONTRL_ENTER		0x0124//36//ȷ��
#define KEY_CONTRL_LEFT			0x0125//37//������
#define KEY_CONTRL_DOWN		        0x0126//38//������
#define KEY_CONTRL_RIGHT		0x0127//39//������
#define KEY_CONTRL_SOUND		0x0128//40//����
#define KEY_CONTRL_TITLE		0x0129//41//��Ļ
#define KEY_CONTRL_YY			0x012a//42//����
#define KEY_CONTRL_PREV		        0x012b//43//��һ��
#define KEY_CONTRL_QBACK		0x012c//44//����
#define KEY_CONTRL_PLAY			0x012d//45//����/��ͣ
#define KEY_CONTRL_QUIT		        0x012e//46//���
#define KEY_CONTRL_NEXT		        0x012f//47//��һ��

//������¼����ֵ
#define KEY_SELECT_TYM			0x000c  //ͶӰĻ
#define KEY_SELECT_LCDUP		0x000d//Һ�������豸
#define KEY_SELECT_TYJUP		0x000e//ͶӰ������
#define KEY_SELECT_PELMET		0x000f//����

#define KEY_CONTRL_EJC			0x0130//DVD¼������п���
#define KEY_CONTRL_STOP		        0x0131//����ֹͣ����


#define KEY_REC_BAK                    0xE000 //��ҫ���¼�Ʊ��ü�
#define KEY_DEV_BAK                    0xF000//��ҫ����豸���ü�
//��Դ����
#define KEY_REP_PC_POWER 		1		//�����
#define KEY_REP_LIGHT_POWER	        (1<<1)	//���ڵƹ�
#define KEY_REP_GF_POWER		(1<<2)	//���ŵ�Դ
#define KEY_REP_TYJ_POWER		(1<<3)	//ͶӰ����Դ
//����/��ͣ
#define KEY_REP_PLAY_PUSH		(1<<4)
//¼��_��ͣ
#define KEY_REP_REC_PUSH		(1<<5)
//mic����
#define KEY_REP_MIC_MUTE		(1<<6)
//����
#define KEY_REP_HELP			(1<<7)
//�ӹ��豸
#define KEY_REP_JGDEV			(1<<8)
#define KEY_REP_JGREC			(1<<9)
//�豸����
#define KEY_REP_DEVICE_MUTE	        (1<<10)
//����ģʽ
#define KEY_REP_MODE			(1<<11)
//�Ͽ�
#define KEY_REP_CLASS			(1<<12)
//Ԥ¼��
#define KEY_REP_YREC			(1<<13)
//��ѧ��ϰ
#define KEY_REP_JXZX                   (1<<14)
//��������
#define KEY_CONTRL_RF			1 // RF����Ч��R��
#define KEY_CONTRL_MIC_MUTE	(1<<1)// MIC������R/W��
#define KEY_CONTRL_DEV_MUTE	(1<<2)//�豸������R/W��
#define KEY_CONTRL_UP_START	(1<<3)//����������ʼ��R
#define KEY_CONTRL_UP_OVER		(1<<4)//���������ɹ���R��
struct KEY_TYPE
{
    uint16 	key_rep;            		 //��ֵ����
    uint8 	POWER_DEV;   		//	��ǰ�����豸
    uint8       Curr_option;                 //��ǰ�������豸����
    uint8 	CURR_DEVICE; 		//��ǰ�豸
    uint8  	MODE;				//ģʽ
    uint8		PERSONAL_TALL;		//���
    uint8 	CODE1_SPEED;		//����
    uint8 	METRIC;				//�ֱ���
    uint8		FRAME;				//������Ч
    uint8 	CURR_SCAMERA;		//��ǰ�����
    uint16      CONTRL;				//
    uint8		RF_DATA[32];		//
    uint8		RF_lg;				//
    uint32	TYJ_time;
};
#define LED_DISPLAY_ON		0//��ʾ��
#define LED_DISPLAY_OFF		1//��ʾ��
#define LED_DISPLAY_FLASH	2//��˸
#define LED_ALL_CLR			3//��ʾȫ��
#define LED_ALL_SET			4//��ʾȫ��
#define LED_ALL_FLASH		5 //


#define LED_CONTRL_RF				1//RF����Ч��R��
#define LED_CONTRL_MIC_MUTE		(1<<1)// MIC������R/W��
#define LED_CONTRL_DEV_MUTE		(1<<2)//�豸������R/W��
#define LED_CONTRL_TIME_OFF		(1<<3)//ʱ�ӹر�
#define LED_CONTRL_POWER_ON	        (1<<4)//����Դ��
#define LED_CONTRL_PROGRAM_BIGEN	(1<<14)//����������ʼ
#define LED_CONTRL_PROGRAM_END	(1<<15)//���������ɹ�
struct LED_TYPE
{
    uint16 LED_data[4];
    uint16 LED_type[4];
    uint8   LED_time[8];
    uint16 Contrl;
};
void  key_init(void);
void key_msg_rev(uint8 *pdata);
void *Get_KeyData(void );
void Key_TB_state(uint16 flag, uint16 type);
void KEY_CMD(uint8 *pkey);
void LED_Set(uint32 val, uint8 type);
void LED_Time_Flash(void);
void LED_Time_Flash(void);
void LED_Contrl(uint16 Contrl, uint8 type);
void KEY_Set_Vol(uint8 Vol);
//Variable declare
extern struct KEY_TYPE KEY_DATA;
//extern void Temperaturepro(void);

#endif

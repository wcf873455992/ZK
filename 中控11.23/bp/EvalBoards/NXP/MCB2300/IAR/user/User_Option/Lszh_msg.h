#ifndef _LSZH_MSG_H
#define _LSZH_MSG_H
//��Ϣ����
#define MSG_MAX_SIZE   					20       //��Ϣ���г���

//��Ϣ����
#define MSG_UDP      						0x01
#define MSG_UART     						0x02
#define MSG_SYS       						0x04
#define MSG_KEY      						0x08
#define MSG_TIME    						0x10
//�ļ�����
#define FILE_TYPE_TABLE					0 //���ñ�
#define FILE_TYPE_USER_PRORAM			17 //�û�����
#define FILE_TYPE_CLASS_TABLE			18//�γ̱�
#define FILE_TYPE_KB_PROGRAM			19//����û�����
#define FILE_TYPE_TRACK_PROGRAM		20//���ٰ��û�����
#define FILE_TYPE_TABLE_BASEINFO		21 //���ñ������Ϣ
#define FILE_TYPE_CLASS_TABLE_ONEDAY	22//�γ̱�һ��

//�豸����
#define MSG_DEV_CLASSINFO   				0  //���һ�����Ϣ
#define MSG_DEV_OPENCLASS    			1 //��������
#define MSG_DEV_CLOSECLASS				2 //�ػ�����
#define MSG_DEV_CALSSTYPE1				3 //�γ̱�1
#define MSG_DEV_CALSSTYPE2				4
#define MSG_DEV_CALSSTYPE3				5
#define MSG_DEV_CALSSTYPE4				6
#define MSG_DEV_CALSSTYPE5				7
#define MSG_DEV_CALSSTYPE6				8
#define MSG_DEV_CALSSTYPE7				9  //�γ̱�7
#define MSG_DEV_AUTOTRACK    			10  //�Զ����ٲ���
#define MSG_DEV_FIXEDPOSITION 			11  //��λ����
#define MSG_DEV_STRATEGY1          			12  //����1
#define MSG_DEV_STRATEGY2				13
#define MSG_DEV_HD1						14 //���߻���
#define MSG_DEV_HD2						15 //���߻���
#define MSG_DEV_MAINBARD            		16  //һ���
#define MSG_DEV_DVD   					17 //
#define MSG_DEV_LYJ						18  //¼����
#define MSG_DEV_PC						19
#define MSG_DEV_NPC						20
#define MSG_DEV_SHOW					21 //չ̨
#define MSG_DEV_KZA						22 //��չ
#define MSG_DEV_KZB						23
#define MSG_DEV_LMT						24 //��ý��
#define MSG_DEV_TYJA					25 //ͶӰ��
#define MSG_DEV_TYM						26//ͶӰĻ
#define MSG_DEV_GF						27 //����
#define MSG_DEV_LIGHT					28 //���ڵƹ�
#define MSG_DEV_PELMET					29 //����
#define MSG_DEV_LCD						30 //Һ������
#define MSG_DEV_TYJUP					31//ͶӰ������
#define MSG_DEV_TRACK					32//�����豸
#define MSG_DEV_VIDEOCARD				33 //���뿨
#define MSG_DEV_POWER					35 //��Դ��
#define MSG_DEV_ALM						36 //�����豸
#define MSG_DEV_CONTRL					37 //�п��豸
#define MSG_DEV_TQCAMERA				38//��ʦ���
#define MSG_DEV_SQCAMERA				40//ѧ�����
#define MSG_DEV_BQCAMERA				41//�������
#define MSG_DEV_SCAMERA1				42 //���������1
#define MSG_DEV_SCAMERA2				43//���������2
#define MSG_DEV_SCAMERA3				44//���������3
#define MSG_DEV_TMIC					45 //��ʦ���
#define MSG_DEV_SMIC					46//ѧ�����
#define MSG_DEV_JMIC					47//�������
#define MSG_DEV_DCS						48//�����
#define MSG_DEV_KB						49//һ�����չ����
#define MSG_DEV_TB						50//һ���ͬ����
#define MSG_DEV_KZYY					51//һ�����չ������
#define MSG_DEV_TYJB					52 //ͶӰ��
#define MSG_DEV_LAN_KB					53 //�������
#define MSG_DEV_CLASS_ID				0x55

#define MSG_DEV_TEACHER				170  //������ѧ����̨
//�豸������Ϣ
#define MSG_OPTION_NONE				0x00//�޶���
#define MSG_OPTION_POWEROPEN			0x01 //�豸��/�½�()
#define MSG_OPTION_POWERDOWN		0x02 //�豸����
#define MSG_OPTION_POWERCLOSE		0x03 //�豸�ر�/ͣ()
#define MSG_OPTION_MUTEOPEN			0x04 //�豸����
#define MSG_OPTION_MUTECLOSE			0x05//�豸�����ر�
#define MSG_OPTION_VOL					0x06 //�豸����
#define MSG_OPTION_RESET				0x07 //һ�����λ
#define MSG_OPTION_OPEN_OVER			0x0b  //�豸�������
#define MSG_OPTION_CLOSE_OVER			0x0c //�豸�ػ����
#define MSG_OPTION_DEV_LINK			0x0d //�豸����
#define MSG_OPTION_DEV_BREAK			0x0e //�豸�Ͽ�
#define MSG_OPTION_ENABLE_CLOSE		0x0f  //�豸���Թر�


//���ſ�����Ϣ
#define MSG_OPTION_PLAY					0x10 //����
#define MSG_OPTION_PUSE					0x11 //��ͣ
#define MSG_OPTION_STOP					0x12 //ֹͣ
#define MSG_OPTION_GO						0x13 //ǰ��
#define MSG_OPTION_BACK					0x14//����
#define MSG_OPTION_PRE					0x15 //ǰһҳ
#define MSG_OPTION_NEXT					0x16//��һҳ
#define MSG_OPTION_UP						0x17//��
#define MSG_OPTION_DOWN					0x18//��
#define MSG_OPTION_LIFT					0x19//��
#define MSG_OPTION_RIGHT					0x1a//��
#define MSG_OPTION_ENTER					0x1b//ȷ��
#define MSG_OPTION_RETURN					0x1c//����
#define MSG_OPTION_LANGUAGE				0x1d//����
#define MSG_OPTION_TITLE					0x1e//��Ļ
#define MSG_OPTION_TRACK					0x1f//����
#define MSG_OPTION_OPEN					0x20 //����
#define MSG_OPTION_A						0x21//
#define MSG_OPTION_B						0x22
#define MSG_OPTION_ZOOM_ADD				0x23 //�豸ZOOM+
#define MSG_OPTION_ZOOM_SUB				0x24//�豸ZOOM-
#define MSG_OPTION_LT1_ON					0x25 //�豸��1��
#define MSG_OPTION_LT1_OFF				0x26//�豸��1�ر�
#define MSG_OPTION_LT2_ON					0x27//�豸��2��
#define MSG_OPTION_LT2_OFF				0x28//�豸��2�ر�
//¼�Ʋ���
#define MSG_OPTION_RECORD_START			0x40  //¼��ʼ
#define MSG_OPTION_RECORD_PAUSE			0x41 //¼����ͣ
#define MSG_OPTION_RECORD_STOP			0x42 //¼��ֹͣ
#define MSG_OPTION_CODE1_SPEED1			0x43 // ����1 500k
#define MSG_OPTION_CODE1_SPEED2			0x44// ����2 1M
#define MSG_OPTION_CODE1_SPEED3			0x45 // ����1 �Զ���
#define MSG_OPTION_METRIC1				0x46 //�ֱ���XGA
#define MSG_OPTION_METRIC2				0x47 //�ֱ���D1
#define MSG_OPTION_METRIC3				0x48 ////�ֱ����Զ���
#define MSG_OPTION_FRAME1					0x49//���л�
#define MSG_OPTION_FRAME2					0x4a//����
#define MSG_OPTION_FRAME3					0x4b //�Զ���
#define MSG_OPTION_REC_Freez				0x4c//¼��ͨ�����涳��
#define MSG_OPTION_REC_UnFreez			0x4d//¼��ͨ������ⶳ
#define MSG_OPTION_REC_ClassName			0x4e//¼�ƿγ�����
#define MSG_OPTION_REC_Teachername		0x4f//¼�ƽ�ʦ����
//�������
#define MSG_OPTION_PERSONAL_TALL1       	0x30  //����趨160
#define MSG_OPTION_PERSONAL_TALL2		0x31 //����趨170
#define MSG_OPTION_PERSONAL_TALL3		0x32//����趨180
#define MSG_OPTION_SELECT_SCAMERA1		0x33 //�����ѡ��1
#define MSG_OPTION_SELECT_SCAMERA2		0x34 //�����ѡ��2
#define MSG_OPTION_SELECT_SCAMERA3		0x35//�����ѡ��3
#define MSG_OPTION_SELECT_SCAMERA4		0x36//�����ѡ��4
#define MSG_OPTION_SELECT_SCAMERA5		0x37//�����ѡ��5
#define MSG_OPTION_SELECT_SCAMERA6		0x38//�����ѡ��6
#define MSG_OPTION_SCAMERA_ZOOMADD		0x39//�����ZOOM+
#define MSG_OPTION_SCAMERA_ZOOMSUB		0x3a//�����ZOOM-
#define MSG_OPTION_SCAMERA_UP			0x3b//�������
#define MSG_OPTION_SCAMERA_DOWN			0x3c//�������
#define MSG_OPTION_SCAMERA_LIFT			0x3d//�������
#define MSG_OPTION_SCAMERA_RIGHT		0x3e//�������
//zjj add1
#define MSG_OPTION_SCAMERA_STOP				0x3f //�����ͣ
#define MSG_OPTION_SCAMERA_LEFTUP			0x29 //���������
#define MSG_OPTION_SCAMERA_LEFTDOWN		0x2a //���������
#define MSG_OPTION_SCAMERA_RIGHTUP			0x2b //���������
#define MSG_OPTION_SCAMERA_RIGHTDOWN		0x2c //���������
#define MSG_OPTION_SCAMERA_FOCUSNEAR		0x2d //������佹Զ
#define MSG_OPTION_SCAMERA_FOCUSFAR			0x2e //������佹��
#define MSG_OPTION_SCAMERA_PREPOSITION		0x2f //�������Ԥ��λ
#define MSG_OPTION_SCAMERA_SETPREPOSITION 	0X5f//���������Ԥ��λ

//zjj add1
//¼�ƶ������ϵͳ��Ϣ
#define MSG_OPTION_MOUSE_ACTIVE		0x50 //��궯��
#define MSG_OPTION_MOUSE_NONE		0x51//����޶���
#define MSG_OPTION_KB_ACTIVE			0x52//���̶���
#define MSG_OPTION_KB_NONE			0x53//�����޶���
#define MSG_OPTION_PICTURE_ACTIVE	0x55//�����б仯
#define MSG_OPTION_PICTURE_NONE		0x56//�����ޱ仯
#define MSG_OPTION_SOUND_ACTIVE		0x57//�����б仯
#define MSG_OPTION_SOUND_NONE		0x58//�����ޱ仯
//ҵ�����
#define MSG_OPTION_CLASS_BEGIN     		0x70 //�Ͽ�
#define MSG_OPTION_CLASS_OVER		0x71//�¿�
#define MSG_OPTION_RF_ACTIVE			0x72//rf����Ч
#define MSG_OPTION_HELP_ON			0x73//������
#define MSG_OPTION_HELP_OFF			0x74//������
#define MSG_OPTION_AUTO_ON			0x75//�Զ�
#define MSG_OPTION_AUTO_OFF			0x76//�ֶ�
#define MSG_OPTION_DEV_REMOTE_ON	0x77//Զ���豸���ƿ�
#define MSG_OPTION_REC_REMOTE_ON	0x78//Զ���豸���ƿ�
#define MSG_OPTION_DEV_REMOTE_OFF	0x79//Զ���豸���ƹ�
#define MSG_OPTION_REC_REMOTE_OFF	0x7a//Զ��¼����ƹ�
#define MSG_OPTION_HW_BIGIN			0x7b//����ѧ�뿪ʼ
#define MSG_OPTION_HW_END			0x7c//����ѧ�����
//�豸ʹ��ͳ��
#define MSG_OPTION_STAT_START		0x7d //0x4c  //�豸ͳ�ƿ�ʼ
#define MSG_OPTION_STAT_END			0x7e //0x4d  //�豸ͳ�ƽ���

#define MSG_OPTION_HELP_LINK			0x80//����ͨ������
#define MSG_OPTION_HELP_BREAK			0x81//����ͨ���Ͽ�
#define MSG_OPTION_AUTOTRACK_ON		0x82//�Զ����Ŀ�
#define MSG_OPTION_AUTOTRACK_OFF		0x83//�Զ����Ĺ�
#define MSG_OPTION_STRATEGY_ON		0x84//�Զ����Կ�
#define MSG_OPTION_STRATEGY_OFF		0x85//�Զ����Թ�
#define MSG_OPTION_HD_ON				0x86//���߻�����
#define MSG_OPTION_HD_OFF				0x87//���߻�����
#define MSG_OPTION_HD_VAL				0x88//���߻�������ֵ
#define MSG_OPTION_SCAMERA_CONTRL	0x89 //����������壨������
#define MSG_OPTION_SCAMERA_BackVAL	0x8a//��������ز���
#define MSG_OPTION_REC_ReqStatus		0x8b//¼��ͨ����Ϣ��ѯ?��?
#define MSG_OPTION_REC_Contrl			0x8c//¼��ͨ������
#define MSG_OPTION_LMT_ReqStatus         0x8d//����/������ý��״̬
#define MSG_OPTION_TB_ReqStatus            0x8e //����/����ͬ�����ź�״̬

//�ŵ�ѡ�����
#define MSG_OPTION_SOUND_SELECT		0x90  //����ͨ������
#define MSG_OPTION_SOUND_VOL			0x91 //��������
#define MSG_OPTION_VIDEO_SELECT		0x92 //��Ƶ����
#define MSG_OPTION_VGA_SELECT			0x93//VGA����
#define MSG_OPTION_ARLM				0x94 //����״̬�ı�
#define MSG_OPTION_IO_OUT				0x95//����IO���
#define MSG_OPTION_JT_ON				0x96//������
#define MSG_OPTION_JT_OFF				0x97//������
#define MSG_OPTION_POWER				0x98 //��Դ������Ϣ
#define MSG_OPTION_IPPlay				0x99 //IP��Ƶ���ſ���
#define MSG_OPTION_IP_DPlay                    0x9a //ip�Խ�+��������
#define MSG_OPTION_IP_MICVol			0x9b //ip�Խ�mic ��������
#define MSG_OPTION_IP_HPVol			0x9c //ip�Խ���������������


//����
#define MSG_OPTION_FRAME_SELECT		0xa0 //����ѡ��
#define MSG_OPTION_LMT_Exit			0xa1//��ý������˳�
#define MSG_OPTION_LMT_BackPlay		0xa2 //��ý������ط�
#define MSG_OPTION_YY_Teather			0xa3 //������ѧģʽ
#define MSG_OPTION_YY_NoTeather		0xa4 //������ϰģʽ
#define MSG_OPTION_YREC_Begin			0xa5//Ԥ¼�ƿ�ʼ
#define MSG_OPTION_YREC_Stop			0xa6//Ԥ¼��ֹͣ
//����
#define MSG_OPTION_SVRLINK_REQ		0xb0  //����������
#define MSG_OPTION_SEVER_ACK			0xb1//������Ӧ��
#define MSG_OPTION_FILE_REQ			0xb2//�ļ�����
#define MSG_OPTION_FILE_SEND			0xb3//�ļ�����
#define MSG_OPTION_CLASSLINK_REQ		0xb4//��������
#define MSG_OPTION_DEBUG				0xb5//�����ź����
#define MSG_OPTION_STATUS_CHANGE		0xb6 //һ���״̬�ı�
#define MSG_OPTION_DEV_RESET                   0xb7 //�豸��λ
#define MSG_OPTION_LAN_KEY				0xb8 //�������
#define MSG_OPTION_LAN_LED				0xb9//�������led
#define MSG_OPTION_DEVICE_BUSY		0xba //һ����豸æ�����Ѿ�����IP
//zjj add
#define MSG_OPTION_REPEATPOWERCLOSE 	0xbb
#define MSG_OPTION_SERVERWRITE			0xbc
#define MSG_OPTION_SERVERWRITE_ACK		0xbd
#define MSG_OPTION_CardContrl			0xbe //У԰һ��ͨ���������֤


//��ʱ����
#define MSG_OPTION_TIMEACTIVE			0xc0//һ����Ϣ
#define MSG_OPTION_ALM_TIME			0xc1//����ʱ�䵽
#define MSG_OPTION_PBELL_OPEN			0xc2//��Դ�䱨����
#define MSG_OPTION_PBELL_COLSE		0xc3//��Դ�䱨����
#define MSG_OPTION_ASOUND_PLAY		0xc4//������Ƶ��Ƶ����
#define MSG_OPTION_ASOUND_PUSH		0xc5//������Ƶ��Ƶ��ͣ
#define MSG_OPTION_ASOUND_STOP		0xc6//������Ƶ��Ƶֹͣ
#define MSG_OPTION_COMM				0xc7//�豸ͨѶ(����ϵͳ����ʱ�˿ڷ���)
#define MSG_OPTION_HW_SEND			0xc8//���ⷢ��
#define MSG_OPTION_MSG_RUN			0xc9//��Դ������ʱ��Ϣ
#define MSG_OPTION_DEVTABLE_NEW		0xca//�豸�б����
#define MSG_OPTION_CLASSTABLE_ON		0xcb//�α�/��ʱ��
#define MSG_OPTION_CLASSTABLE_OFF		0xcc//�α�/��ʱ��
#define MSG_OPTION_POWERCARD_ONOFF   0xcd//��Դ��������
#define MSG_OPTION_TYJ_DIS_VGA		0xce//ͶӰ����ʾVGA
#define MSG_OPTION_TYJ_DIS_VIDEO		0xcf//ͶӰ����ʾVIDEO
//add wcf
//#define MSG_OPTION_Ping		0xd0    //
//#define MSG_OPTION_Chk_DeviceState		0xd1    //
#define MSG_OPTION_REC_K8               0xfb

//��ǰ�Ĳ���IO���������
#define MSG_OPTION_TEST_IO			0xf0
//���ڲ���
#define MSG_OPTION_UART_TEST			0xf1
//io��ͶӰ�����Ӳ���
#define MSG_OPTION_IO_TEST			0xf2
//�ⲿ�������������Ϣ
#define MSG_OPTION_LEDContrl			0xf3



//���ܽ�ʦ������Ϣ����
#define TCLASSINFO_ID                  		0   //�豸ID��ֵ�͡�1�ֽڡ�
#define TCLASSINFO_DeviceType    		1   //=16 �豸������ֵ�͡�1�ֽڡ�
#define TCLASSINFO_SchoolName   		2  //ѧУ�����ַ�����32�ֽڡ�
#define TCLASSINFO_HouseName   			34 //¥�������ַ�����32�ֽڡ�
#define TCLASSINFO_ClassName    			66 //���������ַ�����32�ֽڡ�
#define TCLASSINFO_ClassType     			98 //���ܽ���������ֵ��2�ֽڡ�
#define TCLASSINFO_ClassID	    			100 //���ܽ���ʶ����     ��ֵ��8�ֽڡ�
#define TCLASSINFO_ClassIP          			108  //���ܽ���IP��ַ     ��ֵ��8�ֽڡ�
#define TCLASSINFO_ClassGateIP   			116//���ܽ�������IP��ַ     ��ֵ��8�ֽڡ�
#define TCLASSINFO_JKSeverIP      		124 //���������IP��ֵ��8�ֽڡ�
#define TCLASSINFO_AlarmSet        		132 //��������������ֵʱ�䡾4�ֽڡ�
#define TCLASSINFO_ConfigTimes   		136 //���ñ����ô���  ��ֵ��4�ֽڡ�
#define TCLASSINFO_WatchNo        		140 //���ͨ������    ��ֵ��1�ֽڡ�
#define TCLASSINFO_ContrlType          		141 //������������ ��ֵ��1�ֽڡ�
#define TCLASSINFO_ContrlType_Manual    		0  //�ֶ�����
#define TCLASSINFO_ContrlType_Time        		1  //��ʱ����
#define TCLASSINFO_ContrlType_ClassTable    	2  //�γ̱���
#define TCLASSINFO_PictureEditIP      		142   //ͼ��༭����IP��ֵ��8�ֽڡ�
#define TCLASSINFO_PictureCoding1     		146  //��Ƶ���뿨��IP
#define TCLASSINFO_PictureCoding2    		150  //��Ƶ���뿨��IP
#define TCLASSINFO_LMT_IP		   	154  //��ý����IP
#define TCLASSINFO_TeacherPC_IP    		158  //��ʦ��������IP
#define TCLASSINFO_nPC_IP		   		162 //�ʼǱ���IP��δʹ�á�
#define TCLASSINFO_WatchType             	166 //����ͺ�
#define TCLASSINFO_AlarmContrl             	167 //����������������
#define TCLASSINFO_TT_Set                    	168 //��������̨TT
#define CLASSINFO_TT_Set_Def	0  //δ����
#define CLASSINFO_TT_Set_ON	1  //TT����̨��
#define CLASSINFO_TT_Set_OFF	2 //TT����̨�ر�
//�豸������ַ����
#define TDevice_ID						0//
#define TDevice_Type						1//
#define TDevice_DeviceName				2//
#define TDevice_TypeName				34//
#define TDevice_VGALink					66//
#define TDevice_VGA_Input				66//
#define TDevice_VGA_Output				67//
#define TDevice_VGA_DefLink				68//
#define TDevice_VGA_BAK					69//
#define TDevice_VideoLink					70//
#define TDevice_Video_Input				70//
#define TDevice_Video_Output				71//
#define TDevice_Video_DefLink				72//
#define TDevice_Video_BAK				73//
#define TDevice_SoundLink				74//
#define TDevice_Sound_Input				74//
#define TDevice_Sound_Output				75//
#define TDevice_Sound_DefLink			76//
#define TDevice_Sound_Vol				77//
#define TDevice_IO_InputLink				78//
#define TDevice_Input_No					78//
#define TDevice_Input_Property			79//
#define TDevice_Input_Property_High		(1<<0)//
#define TDevice_Input_Property_AlarmOn    (1<<1)//
#define TDevice_Input_Property_ALarmPlay (1<<2)//
#define TDevice_Input_Property_AlarmRep  (1<<3)//
#define TDevice_Input_Property_AlarmGF	(1<<4)//
#define TDevice_Input_LinkSoundNo		80//
#define TDevice_Input_BAK				81//
#define TDevice_IO_OutputLink			82//
#define TDevice_Output_No				82//
#define TDevice_Output_Property			83//
#define TDevice_Output_Property_JDQOn	(1<<0)//
#define TDevice_Output_Property_JDQ
#define TDevice_Output_LinkSoundNo		84//
#define TDevice_Output_BAK				85//
#define TDevice_PowerLink				86//
#define TDevice_PowerLink_SUB		86//
#define TDevice_PowerLink_No              87
#define TDevice_XCOMLink						88//
#define TDevice_XCOMLink_Type			88
#define TDevice_XCOMLink_Val				90
#define TDevice_SelectMsg						98
#define TDevice_DevicePowerOnMsg				100
#define TDevice_DevicePowerOffMsg				102
#define TDevice_DevicePowerOffTime				104

////////��ý�����״̬/////////////////////////////////////////////
#define LMT_STATUS_NONE                                 0 //0������
#define LMT_STATUS_REC_Push				  1////1����ͣ¼��
#define LMT_STATUS_REC_Stop				  2////2��ֹͣ¼��
#define LMT_STATUS_BackPlay					  3////3���ط�¼��
#define LMT_STATUS_RECing					  4//// 	¼��
#define LMT_STATUS_Pushing					  5///5��������ͣ���յ���ͣ���δִ���꣩
#define LMT_STATUS_Stoping					  6////6������ֹͣ���յ�ֹͣ���δִ���꣩
#define LMT_STATUS_Exiting					  7////7�������˳�				
#define LMT_STATUS_Exit				               8//////8���˳���ɣ��˳�ǰ���ͣ�һ����յ���״̬����ʱ1���ӹر���ý������


#define LMT_CONTRL_PWon			1
#define LMT_CONTRL_PWoff		(1<<1)
#define LMT_CONTRL_Set			(1<<2) //��ý�尲װ
#define LMT_CONTRL_EXIT			(1<<3)//��ý������˳�

#define LMT_CONTRL_time			10
#define LMT_CONTRL_Rep				3
struct LMT_CONTRL_TYPE
{
    uint8 Device;
    uint8 option;
    uint8 option_time;
    uint8 status;
    uint8 reptimes;
    uint8 LMT_status;
    uint16 power_no;//��Դͨ��
};



#endif

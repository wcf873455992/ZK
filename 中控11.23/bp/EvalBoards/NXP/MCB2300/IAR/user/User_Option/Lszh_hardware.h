#ifndef _LSZH_HARDWARE_H
#define _LSZH_HARDWARE_H
//�豸���Ӷ���----ͨѶ
#define DEV_XCOM_MB					0x00//���
#define DEV_XCOM_TYJ					0x01//ͶӰ������RS232
#define DEV_XCOM_SOUND					0x02//����������
#define DEV_XCOM_3				0x03//���1���ƴ���3

#define DEV_XCOM_DYX					0x04//��Դ�䴮��4
#define DEVICE_SUB_MB				0x08 //
#define DEVICE_SUB_POWER1			0x00//��Դ��1�ӵ�ַ
#define DEVICE_SUB_POWER2			0x01//��Դ��2�ӵ�ַ
#define DEVICE_SUB_POWER3			0x02//��Դ��3�ӵ�ַ
#define DEV_XCOM_5				0x05//������ƴ���5
//6��7������
#define DEV_XCOM_8				0x08//���3���ƴ���8
#define DEV_XCOM_9				0x09//���4���ƴ���9




//#define DEV_XCOM_TB						0x03//ͬ���崮��
//#define DEV_XCOM_SOUND					0x04//��Ƶ������
//#define DEV_XCOM_TYJ					0x05//ͶӰ������
//#define DEV_XCOM_RS485					0x06//��չRS485
//#define DEV_XCOM_SCAMERA				0x07//������ƴ���
#define DEV_XCOM_UDP					0x06 //����ͨ��
#define DEVICE_SUB_CLASS		0x16 //һ���
#define DEVICE_SUB_GATE			0x26//����
#define DEVICE_SUB_SER			0x36//���ط�����
#define DEVICE_SUB_LMT			0x46//��ý��
#define DEVICE_SUB_PC			0x56//�μ������
#define DEVICE_SUB_CMD			0x66//�ϴ�����
#define DEVICE_SUB_CAST			0x76//�㲥
#define DEVICE_SUB_TEST			0x86 //�������
#define DEVICE_SUB_BJCard		0x96 //�༭��
#define DEV_XCOM_MAX_SIZE				10
#define DEV_XCOM_DEBUG					7
#define DEV_XCOM_HW					0x81 //����ͨ��
#define DEV_XCOM_SELF					0x82 //һ�����������Ϣ
#define DEV_XCOM_RF					0x83 //RF�����򷢳�����Ϣ
#define DEV_XCOM_TIME				        0x84 //��ʱ����������Ϣ
#define DEV_XCOM_TABLE					0x85 //���ػ����з�������Ϣ
#define DEV_XCOM_FINGERPRINT                            0x90 //ָ��ʶ�𷢳�����Ϣ

//ͨѶ�˿�
#define UDP_SOUND_PORT       				8898   //��Ƶ�˿�
#define UDP_COMMAND_PORT  				8006 //����˿�
#define UDP_DEBUG_PORT            			5050
#define UDP_DEBUG_LOG_PORT            		7777 //Log 
#define UDP_DEBUG_CMD_PORT				4002 //		
#define UDP_LAN_TEST_PORT				8889

//�豸���Ӷ���----��Ƶ����ͨ��
#define DEV_SOUND_NONE					0x00 //�����޶���
#define DEV_SOUND_MIC1					0x01//���1(�����飬�쾱��ͷ��)
#define DEV_SOUND_MIC2					0x02//���2
#define DEV_SOUND_MIC3					0x03//���3
#define DEV_SOUND_MIC4					0x04//���4
#define DEV_SOUND_DVD					0x05 //DVD
#define DEV_SOUND_LYJ					0x06//¼����
#define DEV_SOUND_PC					0x07 //�����
#define DEV_SOUND_NPC					0x08//�ʼǱ�
#define DEV_SOUND_KZ					0x09//��Ƶ��չ
#define DEV_SOUND_DEV					0x0A//�豸
//�豸���Ӷ���----��Ƶ���ͨ��
#define DEV_SOUND_OUT_NONE			0x0 //��Ƶ����޶���
#define DEV_SOUND_OUT_GF				0x1 //�������
#define DEV_SOUND_OUT_LMT				0x2//��ý�����
#define DEV_SOUND_OUT_YSQ				0x3//���������
#define DEV_SOUND_OUT_BMK				0x4//���뿨���
//�豸���Ӷ���----��Ƶ����ͨ��
#define DEV_VIDEO_NONE					0x00 //��Ƶ�޶���
#define DEV_VIDEO_DVD					0x01 //DVD
#define DEV_VIDEO_LXJ					0x02//¼����
#define DEV_VIDEO_SHOW					0x03//վ̨
#define DEV_VIDEO_QSCAMERA1			0x04//��ʦ���
#define DEV_VIDEO_QSCAMERA2			0x05//ѧ�����
#define DEV_VIDEO_QSCAMERA3			0x06//�������
#define DEV_VIDEO_SCAMERA1				0x07 //����1
#define DEV_VIDEO_SCAMERA2				0x08//����2	
#define DEV_VIDEO_SCAMERA3				0x09//����3
#define DEV_VIDEO_VGATOAV				0x0a //VGATOAV
#define DEV_VIDEO_IN1					0x0b//��Ƶ����1
#define DEV_VIDEO_IN2					0x0c//��Ƶ����2
#define DEV_VIDEO_IN3					0x0d//��Ƶ����3
#define DEV_VIDEO_IN4					0x0e//��Ƶ����4
#define DEV_VIDEO_IN5					0x0f//��Ƶ����5
#define DEV_VIDEO_IN6					0x10//��Ƶ����6
//�豸���Ӷ���----��Ƶ���ͨ��
#define DEV_VIDEO_OUT_NONE			0x0 //��Ƶ����޶���
#define DEV_VIDEO_OUT_TB0				0x01//ͬ����0
#define DEV_VIDEO_OUT_TB1				0x02//ͬ����1
#define DEV_VIDEO_OUT_BM1_1			0x03//�����1-1
#define DEV_VIDEO_OUT_BM1_2			0x04//�����1-2
#define DEV_VIDEO_OUT_BM1_3			0x05//�����1-3
#define DEV_VIDEO_OUT_BM1_4			0x06//�����1-4
#define DEV_VIDEO_OUT_BM2_1			0x07//�����2-1
#define DEV_VIDEO_OUT_BM2_2			0x08//�����2-2
#define DEV_VIDEO_OUT_BM2_3			0x09//�����2-3
#define DEV_VIDEO_OUT_BM2_4			0x0a//�����2-4
#define DEV_VIDEO_OUT_FZ1				0x0b//�������1
#define DEV_VIDEO_OUT_FZ2				0x0c//�������2
#define DEV_VIDEO_OUT_FZ3				0x0d//�������3
#define DEV_VIDEO_OUT_TYJA			0x0e//�������4/ͶӰ��A
#define DEV_VIDEO_OUT_TYJB			0x0f//�������5ͶӰ��B
#define DEV_VIDEO_OUT_FZ6				0x10//�������6
//�豸���Ӷ���----VGA����ͨ��
#define DEV_VGA_NONE					0x00 //VGA�޶���
#define DEV_VGA_PC						0x01 //�����
#define DEV_VGA_DVD						0x02//DVD
#define DEV_VGA_SHOW					0x03//չ̨
#define DEV_VGA_NPC						0x04//�ʼǱ�
//�豸���Ӷ���----VGA���ͨ��
#define DEV_VGA_OUT_NONE				0x0 //VGA����޶���
#define DEV_VGA_OUT_TB0				0x1//ͬ��0
#define DEV_VGA_OUT_TB1				0x2//ͬ��1				
#define DEV_VGA_OUT_TYJ					0x3//ͶӰ��
//�豸���Ӷ���----IO����ͨ��
#define DEV_IO_NONE						0x00  //�����޶���
#define DEV_IO_IN_ALARM1				0x01	//IN_ALARM1//����1
#define DEV_IO_IN_ALARM2				0x02	//IN_ALARM2//����2
#define DEV_IO_IN_ALARM3				0x03	//IN_ALARM3//����3
#define DEV_IO_IN_ALARM4				0x04	//IN_ALARM4//����4
#define DEV_IO_IN_ALARM5				0x05	//IN_ALARM5//����5
#define DEV_IO_IN_ALARM6				0x06	//IN_ALARM6//����6
#define DEV_IO_IN_PC1POWER				0x07	//IN_PC1POWER//�μ����������
#define DEV_IO_IN_PC2POWER				0x08	//IN_PC2POWER//��ý����������
#define DEV_IO_IN_VGA					0x09	//IN_TYJVGA//ͶӰ������
//�豸���Ӷ���----IO���ͨ��
#define DEV_IO_OUT_NONE				0x00
#define DEV_IO_OUT_ALARM1				0x01	//OUT_ALARM1//�������1 
#define DEV_IO_OUT_ALARM2				0x02	//OUT_ALARM2//�������2
#define DEV_IO_OUT_ALARM3				0x03	//OUT_ALARM3//�������3
#define DEV_IO_OUT_PC1POWER			0x04	//OUT_PC1POWER//�μ����������
#define DEV_IO_OUT_PC2POWER			0x05	//OUT_PC2POWER//��ý����������
#define DEV_IO_OUT_DCS					0x06	//OUT_DCS//���������
#define DEV_IO_OUT_M3					0x07
#define DEV_IO_OUT_M4					0x08


//�豸��������
#define VOL_CH_DEVGF			0x01   //�豸����TO_GF  U37_in1
#define VOL_CH_MIC1				0x02	//MIC1	U37_IN3	
#define VOL_CH_MIC2				0x03	//MIC2	U37_IN4	
#define VOL_CH_MIC3				0x04	//MIC3	U37_IN5	
#define VOL_CH_MIC4				0x05	//MIC4	U37_IN6	
#define VOL_CH_DEVMID			0x06	//U42_IN1
#define VOL_CH_GF				0x07	//U42_IN3
#define VOL_CH_MID				0x08	//U42_IN5
#endif

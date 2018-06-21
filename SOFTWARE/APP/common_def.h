/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_DEF_H
#define __COMMON_DEF_H
////////////////////////////////////////////////////////////////////////////////
// the device work type
#define	DEVICE_TYPE_NEED_BOOK	1			//����ģʽ����ҪԤԼ
#define DEVICE_TYPE_NO_BOOK		0			//����ģʽ������ҪԤԼ
#define DEVICE_TYPE_DEFAULT		DEVICE_TYPE_NO_BOOK

// the tcp server command
#define SVR_CMD_AUTH		(0)		// �û�ˢ����֤���
#define SVR_CMD_AUTH_PC		(1)		// PC�û���֤���
#define SVR_CMD_TIME		(2)		// ͬ��������ʱ��
#define SVR_CMD_CARD_ID		(3)		// ͬ���û�����Ϣ
#define SVR_CMD_BOOK_INFO	(4)		// ͬ��ԤԼ��¼
#define SVR_CMD_SET_TYPE	(5)		// ���ù���ģʽ
#define SVR_CMD_SYNC_ACK	(6)		// 
#define SVR_CMD_ADMIN_CARD	(7)		// ���͹���
#define SVR_CMD_RFID_ORDER	(8)		// RFID������λ��

// the pc command through USART1
#define PC_CMD_HEART_BEAT	(0)		// ������
#define PC_CMD_AUTH			(1)		// PC�û���֤
#define PC_CMD_CHANGE_ID	(99)	// ����ģ��ID
#define PC_CMD_LOGOUT		(2)		// logout by pc
#define PC_CMD_ADMIN_CARD	(4)		// ��������
#define PC_CMD_SCAN			(5)		// ɨ��WIFI�ȵ�
#define PC_CMD_SET_WIFI		(6)		// ����WIFI
#define PC_CMD_SET_TCP		(7)		// ����TCP������
#define PC_CMD_DUMP			(8)		// ��ʾ״̬��Ϣ
#define PC_CMD_SETTIME		(9)		// 
#define PC_CMD_QUERY		(10)	//
#define PC_CMD_CL_OFFDATA	(11)	//
#define PC_CMD_GET_IP		(12)	// ��ʾIP��MAC
#define PC_CMD_RF_ORDER		(13)	// ����RFIDλ��
#define PC_CMD_CL_BOOK		(14)	// ���ԤԼ��Ϣ
#define PC_CMD_CL_ALL		(15)	// ���NAND FLASH��Ϣ
#define PC_CMD_SEND_FILE      (16) //�����ļ�
#define PC_CMD_ACQ_rough_weight   (17) //��ȡë��
#define PC_CMD_ACQ_WEIGHT      (18) //��ȡ��ǰ����

#define PC_CMD_RELAY_TEST	(50)	// �̵�������


// the server data
#define DATA_FIRST_SECTION	(1)		// 
#define DATA_LAST_SECTION	(2)		//

// the flag
#define FLAG_1_SEC			(1)		//
#define FLAG_10_SEC			(2)		//


// flagh
#define FLAG_ONE_SEC		(1)
#define FLAG_USART1_EVENT	(2)
#define FLAG_USART2_EVENT	(4)
#define FLAG_USART3_EVENT	(8)

#endif	/* __COMMON_DEF_H */

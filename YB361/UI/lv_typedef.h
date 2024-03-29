#ifndef LV_TYPEDEF_H
#define LV_TYPEDEF_H

#include <stdio.h>
#include <malloc.h>

#define LOG_D           printf
#define LOG_E           printf

#define RT_EOK          0

#define pvPortMalloc     malloc
#define vPortFree        free
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;


typedef struct _dev_list
{
	uint8_t *dev_appId;			/*app ID */
	uint8_t *dev_Id;			/*dev ID*/
	uint16_t elt_size;			/*Ԫ�ش�С*/
	uint16_t nelts;				/*��ԾԪ�ظ���*/
	uint16_t nalloc;			/*����Ԫ�ظ���*/
}Dev_list_t;


typedef struct _record{
	uint8_t start_time[20];		/*��ʼʱ��*/
	uint8_t end_time[20];		/*����ʱ��*/
	uint16_t duration;			/*ʱ��*/
	uint16_t regularErr;		/*���治������ʱ��*/
	uint16_t headLow;			/*��ͷʱ��*/
	uint16_t headTilt;			/*��ͷʱ��*/
}Record_t;

typedef struct _record_list
{
	Record_t *record;			/*��¼*/
	uint16_t elt_size;			/*Ԫ�ش�С*/
	uint16_t nelts;			/*��ԾԪ�ظ���*/
	uint16_t nalloc;			/*����Ԫ�ظ���*/
}Record_list_t;




typedef struct _timers
{
	uint32_t _id;			/*��ʱ��ID*/
	uint8_t name[40];          /*��ʱ������*/
	uint8_t time[20];		 	/*����ʱ��13:00*/
	uint32_t repeat;		    /*�ظ����� 0-���ظ�,1-ÿ��,2xxxxxxx,ÿ��,3xxÿ�¼���*/
	uint8_t start[20];			/*��ʼʱ��*/
	uint8_t stop[20];			/*����ʱ��*/
	uint8_t timerSwitch;		/*��ʱ������*/
	uint32_t audioId;		    /*��������ID*/
}Timer_t;

typedef struct _timer_list
{
	Timer_t *timer;				/*��ʱ��*/
	uint16_t elt_size;			/*Ԫ�ش�С*/
	uint16_t nelts;				/*��ԾԪ�ظ���*/
	uint16_t nalloc;			/*����Ԫ�ظ���*/
}Timer_list_t;

typedef struct _dev_param
{
	uint8_t remindMode;				//����ģʽ
	uint8_t volume;					//����
	uint16_t longTimeMode;			//��������ʱ��
	uint16_t shutdownMode;			//�����ػ�ʱ��
	uint8_t postureCam;				//��������ͷ����
	uint8_t longSitRemind;			//�������ѿ���
	uint8_t lightDetecter;			//�������⿪��
	uint8_t offSeat;				//������⿪��
	uint8_t micSwitch;				//����ʶ�𿪹�
	uint8_t uploadPic;				//�����ϴ�����
	uint8_t voiceWarn;				//�������ѿ���
	uint8_t vibrateWarn;			//�����ѿ���
	uint8_t lightWarn;				//�ƹ����ѿ���

	uint8_t dev_state;				//�豸״̬
	Dev_list_t dev_list;			//�豸�б�
	Record_list_t record_list;		//���˼�¼�б�
	Timer_list_t timer_list;		//��ʱ���б�
	uint8_t bind_devId[20];			//��ǰ�󶨵��豸ID

	uint8_t posestatus;				//��ǰ����״̬
	uint8_t devState;				//�豸״̬

}Dev_param_t;

extern Dev_param_t gdev_param;

void param_init(void);

uint32_t xTaskGetTickCount(void);
void add_some_data(void);
#endif

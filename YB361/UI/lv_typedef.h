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
	uint16_t elt_size;			/*元素大小*/
	uint16_t nelts;				/*活跃元素个数*/
	uint16_t nalloc;			/*申请元素个数*/
}Dev_list_t;


typedef struct _record{
	uint8_t start_time[20];		/*起始时间*/
	uint8_t end_time[20];		/*结束时间*/
	uint16_t duration;			/*时长*/
	uint16_t regularErr;		/*常规不良坐姿时长*/
	uint16_t headLow;			/*低头时长*/
	uint16_t headTilt;			/*歪头时间*/
}Record_t;

typedef struct _record_list
{
	Record_t *record;			/*记录*/
	uint16_t elt_size;			/*元素大小*/
	uint16_t nelts;			/*活跃元素个数*/
	uint16_t nalloc;			/*申请元素个数*/
}Record_list_t;




typedef struct _timers
{
	uint32_t _id;			/*定时器ID*/
	uint8_t name[40];          /*定时器名字*/
	uint8_t time[20];		 	/*提醒时间13:00*/
	uint32_t repeat;		    /*重复类型 0-不重复,1-每天,2xxxxxxx,每周,3xx每月几日*/
	uint8_t start[20];			/*起始时间*/
	uint8_t stop[20];			/*结束时间*/
	uint8_t timerSwitch;		/*定时器开关*/
	uint32_t audioId;		    /*语音提醒ID*/
}Timer_t;

typedef struct _timer_list
{
	Timer_t *timer;				/*定时器*/
	uint16_t elt_size;			/*元素大小*/
	uint16_t nelts;				/*活跃元素个数*/
	uint16_t nalloc;			/*申请元素个数*/
}Timer_list_t;

typedef struct _dev_param
{
	uint8_t remindMode;				//坐姿模式
	uint8_t volume;					//音量
	uint16_t longTimeMode;			//久坐提醒时长
	uint16_t shutdownMode;			//离座关机时长
	uint8_t postureCam;				//坐姿摄像头开关
	uint8_t longSitRemind;			//久坐提醒开关
	uint8_t lightDetecter;			//环境光检测开关
	uint8_t offSeat;				//离座监测开关
	uint8_t micSwitch;				//语音识别开关
	uint8_t uploadPic;				//拍照上传开关
	uint8_t voiceWarn;				//语音提醒开关
	uint8_t vibrateWarn;			//震动提醒开关
	uint8_t lightWarn;				//灯光提醒开关

	uint8_t dev_state;				//设备状态
	Dev_list_t dev_list;			//设备列表
	Record_list_t record_list;		//坐姿记录列表
	Timer_list_t timer_list;		//定时器列表
	uint8_t bind_devId[20];			//当前绑定的设备ID

	uint8_t posestatus;				//当前坐姿状态
	uint8_t devState;				//设备状态

}Dev_param_t;

extern Dev_param_t gdev_param;

void param_init(void);

uint32_t xTaskGetTickCount(void);
void add_some_data(void);
#endif

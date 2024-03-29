#include "lv_typedef.h"
#include <time.h>

Dev_param_t gdev_param;

int dev_info_cont = 20;/*初始申请个数*/
int dev_record_cont = 20;

uint32_t xTaskGetTickCount(void)
{
    time_t current_time = time(NULL);
    return (uint32_t)current_time;

}


/* param_init
 * 全局参数初始化
 */
void param_init(void)
{

    memset((uint8_t *)&gdev_param, 0, sizeof(gdev_param));
    memcpy(gdev_param.bind_devId,"P69R98FF0005",strlen("P69R98FF0005"));
    gdev_param.longTimeMode = 30;
    gdev_param.shutdownMode = 30;
}

/* add_device_info
 * 添加一个设备信息
 * appId:appId
 * devId:devId
 */
void add_device_info(char *appId, char *devId)
{
    int i;
    char *dev_appId;
	char *dev_Id;
    /*判断重复添加*/
	for(i = 0; i < gdev_param.dev_list.nelts; i++)
	{
		if(memcmp((uint8_t*)&gdev_param.dev_list.dev_Id[i * gdev_param.dev_list.elt_size], devId, strlen(devId)) == 0)
		{
			if(memcmp((uint8_t*)&gdev_param.dev_list.dev_appId[i * gdev_param.dev_list.elt_size], appId, strlen(appId)) == 0)
			{
				printf("there is has error");
				return;
			}
		}
	}

	/* 添加到设备信息中
	 * */
	if(gdev_param.dev_list.dev_Id == NULL || gdev_param.dev_list.dev_appId == NULL || gdev_param.dev_list.nalloc == 0)
	{
		/* 没初始化
		 * */
		gdev_param.dev_list.elt_size = 20;/*每个元素大小20字节*/
		gdev_param.dev_list.dev_appId = pvPortMalloc(gdev_param.dev_list.elt_size * dev_info_cont);
		if(gdev_param.dev_list.dev_appId == NULL)
		{
			LOG_E("there is no enough space.");
			return;
		}
		gdev_param.dev_list.dev_Id = pvPortMalloc(gdev_param.dev_list.elt_size * dev_info_cont);
		if(gdev_param.dev_list.dev_Id == NULL)
		{
			LOG_E("there is no enough space.");
			vPortFree(gdev_param.dev_list.dev_appId);
			return;
		}
		gdev_param.dev_list.nalloc = dev_info_cont;
		gdev_param.dev_list.nelts = 0;
	}

	if(gdev_param.dev_list.nelts >= gdev_param.dev_list.nalloc)
	{
		/*空间不够重新申请*/
		dev_appId = pvPortMalloc(gdev_param.dev_list.elt_size * (gdev_param.dev_list.nalloc + 10));
		if(dev_appId == NULL)
		{
			LOG_E("there is no enough space.");
            return;
		}

		dev_Id = pvPortMalloc(gdev_param.dev_list.elt_size * (gdev_param.dev_list.nalloc + 10));
		if(dev_Id == NULL)
		{
			LOG_E("there is no enough space.");
			vPortFree(dev_appId);
            return;
		}

		memcpy(dev_appId, gdev_param.dev_list.dev_appId, gdev_param.dev_list.elt_size * gdev_param.dev_list.nalloc);
		memcpy(dev_Id, gdev_param.dev_list.dev_Id, gdev_param.dev_list.elt_size * gdev_param.dev_list.nalloc);
		vPortFree(gdev_param.dev_list.dev_appId);
		vPortFree(gdev_param.dev_list.dev_Id);
		gdev_param.dev_list.dev_appId = dev_appId;
		gdev_param.dev_list.dev_Id = dev_Id;
		gdev_param.dev_list.nalloc += 10;
	}


	/* 添加到列表
	 * */
	memset(gdev_param.dev_list.dev_appId + gdev_param.dev_list.nelts * gdev_param.dev_list.elt_size, '\0', gdev_param.dev_list.elt_size);
	memset(gdev_param.dev_list.dev_Id + gdev_param.dev_list.nelts * gdev_param.dev_list.elt_size, '\0', gdev_param.dev_list.elt_size);
	memcpy(gdev_param.dev_list.dev_appId + gdev_param.dev_list.nelts * gdev_param.dev_list.elt_size, appId, strlen(appId));
	memcpy(gdev_param.dev_list.dev_Id + gdev_param.dev_list.nelts * gdev_param.dev_list.elt_size, devId, strlen(devId));
	gdev_param.dev_list.nelts ++;


}

/* add_deviv_info
 * 添加一个设备信息
 * appId:appId
 * devId:devId
 */
void add_record(char *startTime, char *endTime, int duration, int regularErr, int headLow, int headTilt)
{
    int i = 0;
    /* 添加到记录列表中
	 * */
	if(gdev_param.record_list.nalloc == 0)
	{
		/*未初始化*/
		gdev_param.record_list.elt_size = sizeof(Record_t);
		gdev_param.record_list.record = (Record_t*)pvPortMalloc(gdev_param.record_list.elt_size * dev_record_cont);
		if(gdev_param.record_list.record == NULL)
		{
			LOG_E("there is no enough space");
			return;
		}
		gdev_param.record_list.nalloc = dev_record_cont;
		gdev_param.record_list.nelts = 0;
	}

	/*如果申请的空间不够重新申请*/
	if(gdev_param.record_list.nalloc > dev_record_cont)
	{
	    #if 0
		vPortFree(gdev_param.record_list.record);
		gdev_param.record_list.elt_size = sizeof(Record_t);
		gdev_param.record_list.record = (Record_t*)pvPortMalloc(gdev_param.record_list.elt_size * arrcount);
		if(gdev_param.record_list.record == NULL)
		{
			LOG_E("there is no enough space");
			return;
		}
		gdev_param.record_list.nalloc = arrcount;
		gdev_param.record_list.nelts = 0;
    #endif // 0
        return;
	}

    i = gdev_param.record_list.nelts;
    if(i >= gdev_param.record_list.nalloc)
    {

        return;
    }
	memset(gdev_param.record_list.record[i].start_time, '\0', sizeof(gdev_param.record_list.record[i].start_time));
    memcpy(gdev_param.record_list.record[i].start_time, startTime, strlen(startTime));
    memset(gdev_param.record_list.record[i].end_time, '\0', sizeof(gdev_param.record_list.record[i].end_time));
    memcpy(gdev_param.record_list.record[i].end_time, endTime, strlen(endTime));
    gdev_param.record_list.record[i].duration = duration;
    gdev_param.record_list.record[i].regularErr = regularErr;
    gdev_param.record_list.record[i].headLow = headLow;
    gdev_param.record_list.record[i].headTilt = headTilt;
    gdev_param.record_list.nelts++;


}

/* add_timer
 * 添加定时器信息
 */
void add_timer(uint32_t _id, char * name, char *time, int repeat, char *start, char *stop, int timerSwitch, uint32_t audioId)
{
    int i = 0;
    /* 添加到记录列表中
	 * */
	if(gdev_param.timer_list.nalloc == 0)
	{
		/*未初始化*/
		gdev_param.timer_list.elt_size = sizeof(Timer_t);
		gdev_param.timer_list.timer = (Timer_t*)pvPortMalloc(gdev_param.timer_list.elt_size * dev_record_cont);
		if(gdev_param.timer_list.timer == NULL)
		{
			LOG_E("there is no enough space");
			return;
		}
		gdev_param.timer_list.nalloc = dev_record_cont;
		gdev_param.timer_list.nelts = 0;
	}

	/*如果申请的空间不够重新申请*/
	if(gdev_param.timer_list.nalloc > dev_record_cont)
	{
	    #if 0
		vPortFree(gdev_param.record_list.record);
		gdev_param.record_list.elt_size = sizeof(Record_t);
		gdev_param.record_list.record = (Record_t*)pvPortMalloc(gdev_param.record_list.elt_size * arrcount);
		if(gdev_param.record_list.record == NULL)
		{
			LOG_E("there is no enough space");
			return;
		}
		gdev_param.record_list.nalloc = arrcount;
		gdev_param.record_list.nelts = 0;
    #endif // 0
        return;
	}

    i = gdev_param.timer_list.nelts;
    if(i >= gdev_param.timer_list.nalloc)
    {

        return;
    }

    memset(gdev_param.timer_list.timer[i].name, '\0', sizeof(gdev_param.timer_list.timer[i].name));
    memcpy(gdev_param.timer_list.timer[i].name, name, strlen(name));
    memset(gdev_param.timer_list.timer[i].time, '\0', sizeof(gdev_param.timer_list.timer[i].time));
    memcpy(gdev_param.timer_list.timer[i].time, time, strlen(time));
    memset(gdev_param.timer_list.timer[i].start, '\0', sizeof(gdev_param.timer_list.timer[i].start));
    memcpy(gdev_param.timer_list.timer[i].start, start, strlen(start));
    memset(gdev_param.timer_list.timer[i].stop, '\0', sizeof(gdev_param.timer_list.timer[i].stop));
    memcpy(gdev_param.timer_list.timer[i].stop, stop, strlen(stop));
    gdev_param.timer_list.timer[i]._id = _id;
    gdev_param.timer_list.timer[i].audioId = audioId;
    gdev_param.timer_list.timer[i].repeat = repeat;
    gdev_param.timer_list.timer[i].timerSwitch = timerSwitch;
    gdev_param.timer_list.nelts++;


}


void add_some_data(void)
{
    time_t cur_time;
    char appId[20];
    char devId[20];
    int i;
    char startTime[20];
    char endTime[20];
    int duration;
    int regularErr;
    int headLow;
    int headTilt;

    for(i = 0; i < 10; i++)
    {
        sprintf(appId, "app000 0199%d", i);
        sprintf(devId, "P69R98E200F%d", i);
        add_device_info(appId, devId);
    }

    memset(startTime, '\0', sizeof(startTime));
    memcpy(startTime, "2023-011-18 8:20", strlen("2023-11-18 8:20"));
    memset(endTime, '\0', sizeof(endTime));
    memcpy(endTime, "2023-11-18 19:20", strlen("2023-11-18 19:20"));
    for(i = 0; i < 10; i++)
    {

        duration = i + 30;
        regularErr = i + 2;
        headLow = i + 3;
        headTilt = i + 4;

        add_record(startTime, endTime, duration, regularErr, headLow, headTilt);
    }

    add_timer(1, "001", "13:00", 0, "2023-11-04", "2023-11-04", 1, 0);
    add_timer(2, "002", "14:00", 331, "2023-11-05", "2023-11-05", 1, 1);
    add_timer(3, "003", "15:00", 21101010, "2023-11-06", "2023-11-06", 1, 2);
}



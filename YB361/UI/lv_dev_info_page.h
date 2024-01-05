/*
 * lv_dev_info_page.h
 *
 *  Created on: Dec 14, 2023
 *      Author: hxd
 */

#ifndef LV_DEV_INFO_PAGE_H_
#define LV_DEV_INFO_PAGE_H_

#define LV_DEVINFO_US_TABLE_SPACE			(LV_HOR_RES/6)
#define LV_DEVINFO_US_TITLE_SPACE			(45)
#define LV_DEVINFO_US_LABEL_SPACE			(5)

#define LV_DEVINFO_US_CONT_W				(300)
#define LV_DEVINFO_US_CONT_H				(90)

#define LV_FUNC_SWITCH_CONT_W				(280)
#define LV_FUNC_SWITCH_CONT_H				(50)

#define LV_DEVINFO_VOLUME_CONT_H			(70)
#define LV_DEVINFO_FUNC_CONT_H				(260)

#define LV_DEVINFO_FAST_MODE_W             	(40)
#define LV_DEVINFO_FAST_MODE_H             	(40)
#define LV_DEVINFO_FAST_MODE_POSX         	(30)
#define LV_DEVINFO_FAST_MODE_POSY          	(20)
#define LV_DEVINFO_NORMAL_MODE_POSX        	(120)
#define LV_DEVINFO_NORMAL_MODE_POSY        	(20)
#define LV_DEVINFO_HABIT_MODE_POSX         	(210)
#define LV_DEVINFO_HABIT_MODE_POSY         	(20)

#define LV_DEVINFO_MIN_VOLUME_W            	(32)
#define LV_DEVINFO_MIN_VOLUME_H            	(32)
#define LV_DEVINFO_MIN_VOLUME_POSX         	(30)
#define LV_DEVINFO_MIN_VOLUME_POSY         	(30)

#define FAST_MODE							0
#define NORMAL_MODE							1
#define HABIT_MODE							2

typedef struct _lv_devinfo{
	lv_obj_t  * bg_about;				// 背景
	lv_obj_t  * fast_mode;
	lv_obj_t  * normal_mode;
	lv_obj_t  * habit_mode;
	lv_obj_t  * volume_slider;			//音量滑块
	lv_obj_t  * pos_switch;				//坐姿摄像头开关
	lv_obj_t  * longsit_switch;			//久坐提醒开关
	lv_obj_t  * light_switch;			//环境光检测开关
	lv_obj_t  * offset_switch;			//离座监测开关
	lv_obj_t  * mic_switch;				//语音识别开关
	lv_obj_t  * upload_switch;			//图像上传开关
	lv_obj_t  * voicewarn_switch;		//语音提醒开关
	lv_obj_t  * vibrate_switch;			//震动提醒开关
	lv_obj_t  * lightwarn_switch;		//灯光提醒开关

}T_lv_devinfo, *PT_lv_devinfo;



/* some color
 * 豆沙绿		0xC7EDCC
 * 银河白		0xFFFFFF
 * 杏仁黄		0xFAF9DE
 * 秋叶褐		0xFFF2E2
 * 胭脂红		0xFDE6E0
 * 海天蓝		0xDCE2F1
 * 葛巾紫		0xE9EBFE
 * 极光灰		0xEAEAEF
 * 青草绿   	0xE3EDCD
 * 电脑			0xCCE8CF
 * WPS			0x6E7B6C
 * */
#define BACK_COLOR				(0xC7EDCC)//(0xE9EBFE)
#define RED_HEX                 (0xFF0000)
#define GREY_HEX                (0xBEBEBE)
#define LIGHTGRAY_HEX           (0xD3D3D3)
#define SNOW3_HEX               (0xCDC9C9)
#define GREEN_HEX               (0x00FF00)
#define LIME_GREEN_HEX          (0x32CD32)
#define WHITE_HEX               (0xFFFFFF)
#define BLACK_HEX               (0x000000)
#define CORNFLOWERBLUE_HEX      (0x6495ED)
#define BLUE_HEX                (0x0000FF)
#define YELLOW_HEX              (0xFFFF00)
#define PURPLE_HEX              (0xA020F0)




void lv_dev_info(void);
void lv_device_info_refresh(void);
#endif /* LV_DEV_INFO_PAGE_H_ */

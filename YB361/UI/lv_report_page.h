/*
 * lv_report_page.h
 *
 *  Created on: Dec 15, 2023
 *      Author: hxd
 */

#ifndef LV_REPORT_PAGE_H_
#define LV_REPORT_PAGE_H_

#define LV_REPORT_US_TABLE_SPACE    (LV_HOR_RES/6)
#define LV_REPORT_US_TITLE_SPACE    (45)
#define LV_REPORT_US_LABEL_SPACE    (5)

#define LV_REPORT_US_CONT_W		    (300)
#define LV_REPORT_US_CONT_H		    (240)

#define LV_REPORT_ARC_W             (140)
#define LV_REPORT_ARC_H             (140)

#define LV_REPORT_LABEL_CONT_W      (160)
#define LV_REPORT_LABEL_CONT_H      (24)

#define LV_SET_US_CONT_W            (300)
#define LV_SET_US_CONT_H            (180)


#define LV_BTM_LABEL_W              (70)
#define LV_BTM_LABEL_H              (40)

#define LV_SET_CONT_W               (280)
#define LV_SET_CONT_H               (30)

#define LV_SET_MID_LABLE_X          (50)

typedef struct _lv_report {
	lv_obj_t  * bg_about;				//背景
	lv_obj_t  * rate_arc;
	lv_obj_t  * rem_th_btn;             //提醒时间间隔
	lv_obj_t  * rem_vl_btn;             //提醒语音
	lv_obj_t  * lon_th_btn;             //久坐时长
	lv_obj_t  * lev_th_btn;             //离座时长
	lv_obj_t  * rem_ti_btn;             //定时提醒
	lv_obj_t  * rem_cd_btn;             //倒计时提醒
} T_lv_report, *PT_lv_report;


void lv_report(void);
#endif /* LV_REPORT_PAGE_H_ */

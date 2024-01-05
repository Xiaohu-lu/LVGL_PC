/*
 * lv_set_time_page.h
 *
 *  Created on: Dec 16, 2023
 *      Author: hxd
 */

#ifndef LV_LONG_TIME_PAGE_H_
#define LV_LONG_TIME_PAGE_H_


#define LV_SET_US_TABLE_SPACE   (LV_HOR_RES/6)
#define LV_SET_US_TITLE_SPACE   (45)
#define LV_SET_US_LABEL_SPACE   (5)

#define LV_LONG_BTN_W           (300)
#define LV_LONG_BTN_H           (40)

#define LV_CONFIRM_BTN_W		(280)
#define LV_CONFIRM_BTN_H	    (40)

#define LV_LONG_TEXTAREA_W      (140)
#define LV_LONG_TEXTAREA_H      (16)


#define LV_LONG_BTNM_W          (200)
#define LV_LONG_BTNM_H          (150)

typedef struct _lv_longtime {
	lv_obj_t  * bg_about;				// 背景
	lv_obj_t  * min_30_btn;				// 30分钟
	lv_obj_t  * min_40_btn;				// 40分钟
	lv_obj_t  * min_45_btn;				// 45分钟
	lv_obj_t  * usr_btn;				// 自定义时间
	lv_obj_t  * use_label;              // 分钟标签
	lv_obj_t  * sym_btn;                // 对号标签
	lv_obj_t  * confirm_btn;			// 确认按钮
	lv_obj_t  * cancel_btn;				// 取消按钮
	lv_obj_t  * usr_text;               // 自定义文本框
	lv_obj_t  * btnm;                   // 按钮矩阵
} T_lv_longtime, *PT_lv_longtime;


void lv_set_long_time(void);

#endif /* LV_LONG_TIME_PAGE_H_ */

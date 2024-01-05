/*
 * lv_shunt_time.h
 *
 *  Created on: Dec 18, 2023
 *      Author: hxd
 */

#ifndef LV_SHUNT_TIME_H_
#define LV_SHUNT_TIME_H_


#define LV_SET_US_TABLE_SPACE       (LV_HOR_RES/6)
#define LV_SET_US_TITLE_SPACE       (45)
#define LV_SET_US_LABEL_SPACE       (5)

#define LV_SHUNT_TEXTAREA_W         (140)
#define LV_SHUNT_TEXTAREA_H         (16)


typedef struct _lv_shunttime {
	lv_obj_t  * bg_about;				// 背景
	lv_obj_t  * min_10_btn;				// 10分钟
	lv_obj_t  * min_20_btn;				// 20分钟
	lv_obj_t  * min_30_btn;				// 30分钟
	lv_obj_t  * usr_btn;				// 自定义时间
	lv_obj_t  * use_label;              // 分钟标签
	lv_obj_t  * sym_btn;                // 对号标签
	lv_obj_t  * confirm_btn;			//确认按钮
	lv_obj_t  * cancel_btn;				//取消按钮
	lv_obj_t  * usr_text;               // 自定义文本框
	lv_obj_t  * btnm;                   // 按钮矩阵
} T_lv_shunttime, *PT_lv_shunttime;

#endif /* LV_SHUNT_TIME_H_ */

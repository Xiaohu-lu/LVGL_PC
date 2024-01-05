/*
 * lv_record_page.h
 *
 *  Created on: Dec 16, 2023
 *      Author: hxd
 */

#ifndef LV_RECORD_PAGE_H_
#define LV_RECORD_PAGE_H_

#define LV_RECORD_US_TABLE_SPACE    (LV_HOR_RES/6)
#define LV_RECORD_US_TITLE_SPACE    (45)
#define LV_RECORD_US_LABEL_SPACE    (5)

#define LV_RECORD_US_CONT_W		    (300)
#define LV_RECORD_US_CONT_H		    (60)//(420)

#define LV_RECORD_BTN_W			    (280)
#define LV_RECORD_BTN_H			    (40)

#define LV_RECORD_LIST_W		    (300)//(280)
#define LV_RECORD_LIST_H		    (360)

typedef struct _lv_record {
	lv_obj_t  * bg_about;				// 背景
	lv_obj_t  * find_btn;				// 查找按钮
	lv_obj_t  * dev_list;				// 设备列表
	lv_obj_t  * select_btn;				// 绑定设备按钮
} T_lv_record, *PT_lv_record;

void lv_record(void);
void lv_record_refresh(void);
uint8_t get_record_index(void);
#endif /* LV_RECORD_PAGE_H_ */

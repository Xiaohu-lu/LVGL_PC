/*
 * lv_timing_list_page.h
 *
 *  Created on: Dec 20, 2023
 *      Author: hxd
 */

#ifndef LV_TIMING_LIST_PAGE_H_
#define LV_TIMING_LIST_PAGE_H_

#define LV_TIMER_US_TITLE_SPACE     (45)

#define LV_TIMER_CONT_W             (300)
#define LV_TIMER_CONT_H             (60)

#define LV_CONT_ALIGN_SPACE         (10)

#define LV_SWITCH_W                 (72)
#define LV_SWITCH_H                 (36)

#define LV_ADD_BTN_SIZE             (36)


#define LV_ADD_TIMER_INDEX          (255)





typedef struct _lv_timinglist {
	lv_obj_t  * bg_about;				// 背景
	lv_obj_t  * *timer_list;            //定时器列表
	lv_obj_t  * *switch_list;           //开关列表
	lv_obj_t  * add_btn;                //添加定时器按钮
} T_lv_timinglist, *PT_lv_timinglist;


void lv_timing_list(void);
uint8_t get_select_timer_index(void);
#endif /* LV_TIMING_LIST_PAGE_H_ */

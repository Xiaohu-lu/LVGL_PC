/*
 * lv_main_page.h
 *
 *  Created on: Dec 12, 2023
 *      Author: hxd
 */

#ifndef LV_MAIN_PAGE_H_
#define LV_MAIN_PAGE_H_



void lv_main_page_demo(uint32_t delay);
void lv_main_page_demo_home(uint32_t delay);


lv_obj_t * add_title(lv_obj_t *obj, const char *txt);
void add_back(lv_obj_t *obj, lv_event_cb_t event_cb);
void add_confirm(lv_obj_t *obj, lv_event_cb_t event_cb);
void set_user_style(lv_obj_t *obj);
#endif /* LV_MAIN_PAGE_H_ */

/*
 * lv_record_page.c
 *
 *  Created on: Dec 16, 2023
 *      Author: hxd
 */

#include "lvgl/lvgl.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "lv_about_page.h"
#include "lv_find_dev.h"
#include "lv_dev_info_page.h"
#include "lv_record_page.h"
#include "lv_record_info_page.h"

static PT_lv_record g_pt_lv_record;  // 数据结构体




static uint8_t grecord_run_state = 0;

static uint8_t grecord_index = 0;

static void lv_record_init(void);
static void event_handler_back_to_home(lv_event_t *e);
static void event_handler_find_btn(lv_event_t *e);
static void event_handler_find_dev(lv_event_t *e);


/* lv_record
 * 坐姿记录界面
 * */
void lv_record(void)
{

	g_pt_lv_record = (T_lv_record*)pvPortMalloc(sizeof(T_lv_record));
	g_pt_lv_record->bg_about = lv_obj_create(lv_scr_act());/*创建背景*/

	lv_obj_set_size(g_pt_lv_record->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_record->bg_about, 0, 0);/*设置圆角*/
	lv_obj_align(g_pt_lv_record->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*设置padding*/
	set_user_style(g_pt_lv_record->bg_about);

	lv_obj_set_size(g_pt_lv_record->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_record->bg_about, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_record->bg_about, lv_color_hex(BACK_COLOR), 0);
	lv_record_init();/*界面初始化*/

	add_title(g_pt_lv_record->bg_about, (const char *)gdev_param.bind_devId);
	add_back(g_pt_lv_record->bg_about, event_handler_back_to_home);

}

/* lv_record_refresh
 * 坐姿记录界面刷新函数
 */
void lv_record_refresh(void)
{
	char str_buf[50];
	uint16_t i;
	if(grecord_run_state == 0)
		return;
    lv_obj_t *list_btn;
	lv_obj_clean(g_pt_lv_record->dev_list);
	for(i = 0; i < gdev_param.record_list.nelts; i++)
	{
		memset(str_buf, '\0', sizeof(str_buf));
		sprintf(str_buf, "%s<--->%s", gdev_param.record_list.record[i].start_time, gdev_param.record_list.record[i].end_time);
		list_btn = lv_list_add_btn(g_pt_lv_record->dev_list, NULL, (const char*)str_buf);
		lv_obj_set_style_bg_color(list_btn, lv_color_hex(BACK_COLOR), 0);
		lv_obj_set_style_bg_color(list_btn, lv_color_hex(RED_HEX), LV_STATE_PRESSED);/*按下的背景颜色*/
		lv_obj_add_event_cb(list_btn, event_handler_find_dev, LV_EVENT_CLICKED, NULL);
	}

}

/* get_record_index
 * 返回当前点击的列表索引
 */
uint8_t get_record_index(void)
{
	return grecord_index;
}

/* lv_record_init
 * 坐姿记录界面初始化
 * */
static void lv_record_init(void)
{
    char str_buf[50];
	uint16_t i;
    grecord_run_state = 1;

    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(BACK_COLOR));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 2);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
	lv_style_set_border_width(&style_cont, 0);
	/*创建容器 */
	lv_obj_t *cont_mode = lv_obj_create(g_pt_lv_record->bg_about);
	lv_obj_set_size(cont_mode, LV_RECORD_US_CONT_W, LV_RECORD_US_CONT_H);/*设置对象大小*/
	lv_obj_add_style(cont_mode, &style_cont, 0);/*默认状态下的style*/
	lv_obj_align(cont_mode, LV_ALIGN_TOP_MID, 0, LV_RECORD_US_TITLE_SPACE);/*设置位置,在他父对象的顶部中间*/
	lv_obj_fade_in(cont_mode, 0, 0);/*淡入*/

	/*创建两个按钮*/
	/*创建模式按钮样式*/
	static lv_style_t style_btn;
	lv_style_init(&style_btn);
	lv_style_set_bg_opa(&style_btn, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_bg_color(&style_btn, lv_color_hex(BACK_COLOR));/*设置背景颜色*/
	lv_style_set_pad_top(&style_btn,0);
	lv_style_set_pad_bottom(&style_btn, 0);
	lv_style_set_pad_left(&style_btn, 0);
	lv_style_set_pad_right(&style_btn, 0);
	lv_style_set_border_width(&style_btn, 1);
	lv_style_set_radius(&style_btn, 10);/*设置圆角*/

	lv_obj_t *btn;
	g_pt_lv_record->find_btn = lv_btn_create(cont_mode);
	btn = g_pt_lv_record->find_btn;
	lv_obj_add_style(btn, &style_btn, 0);/*添加样式*/
	lv_obj_set_size(btn, LV_RECORD_BTN_W, LV_RECORD_BTN_H);
	lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 5);
	lv_obj_fade_in(btn, 0, 0);/*淡入*/
	/*添加点击事件*/
	lv_obj_add_event_cb(g_pt_lv_record->find_btn, event_handler_find_btn, LV_EVENT_CLICKED, NULL);

	/*创建btn中的标签*/
	lv_obj_t *label_find = lv_label_create(btn);
	lv_label_set_text(label_find, "获取坐姿记录");/*设置text*/
	lv_obj_set_style_text_color(label_find, lv_color_hex(0x0), 0);/*设置字体颜色*/
	lv_obj_set_style_text_font(label_find, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_center(label_find);
	lv_obj_fade_in(label_find, 0, 0);


	/* 创建列表
	 * */
	//g_pt_lv_record->dev_list = lv_list_create(cont_mode);
	g_pt_lv_record->dev_list = lv_list_create(g_pt_lv_record->bg_about);
	set_user_style(g_pt_lv_record->dev_list);
	lv_obj_set_style_bg_color(g_pt_lv_record->dev_list, lv_color_hex(BACK_COLOR), 0);
	lv_obj_set_size(g_pt_lv_record->dev_list, LV_RECORD_LIST_W, LV_RECORD_LIST_H);
	//lv_obj_align_to(g_pt_lv_record->dev_list, g_pt_lv_record->find_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
	lv_obj_align_to(g_pt_lv_record->dev_list, cont_mode, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	lv_obj_t *list_btn;
    lv_obj_clean(g_pt_lv_record->dev_list);
	for(i = 0; i < gdev_param.record_list.nelts; i++)
	{
		memset(str_buf, '\0', sizeof(str_buf));
		sprintf(str_buf, "s:%s<-->e:%s", gdev_param.record_list.record[i].start_time, gdev_param.record_list.record[i].end_time);
		list_btn = lv_list_add_btn(g_pt_lv_record->dev_list, NULL, (const char*)str_buf);
		lv_obj_set_style_bg_color(list_btn, lv_color_hex(BACK_COLOR), 0);
		lv_obj_set_style_bg_color(list_btn, lv_color_hex(RED_HEX), LV_STATE_PRESSED);/*按下的背景颜色*/
		lv_obj_add_event_cb(list_btn, event_handler_find_dev, LV_EVENT_CLICKED, NULL);
	}
}


/* event_handler_back_to_home
 * 返回按键点击事件的回调函数
 * */
static void event_handler_back_to_home(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
		if(g_pt_lv_record->bg_about != NULL) lv_obj_del(g_pt_lv_record->bg_about);

		vPortFree(g_pt_lv_record);
		grecord_run_state = 0;
		lv_main_page_demo_home(100);
	}
}

/* event_handler_find_btn
 * 获取坐姿记录按钮时间回调函数
 * */
static void event_handler_find_btn(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//int result = RT_EOK;
	if(code == LV_EVENT_CLICKED)
	{
	    /*
		result = mqtt_send_getRecord(2);
		if(result != RT_EOK)
		{
			LOG_D("send device finding error.");
		}
		*/
	}
}

/* event_handler_find_dev
 * list列表元素点击时间回调函数
 */
static void event_handler_find_dev(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)
	{
		LOG_D("clicked :%s", lv_list_get_btn_text(g_pt_lv_record->dev_list, obj));

		grecord_index = lv_obj_get_index(obj);
		LOG_D("clicked index = %d", grecord_index);

		if (g_pt_lv_record->dev_list != NULL)      	lv_obj_del(g_pt_lv_record->dev_list);
		if (g_pt_lv_record->find_btn != NULL)      	lv_obj_del(g_pt_lv_record->find_btn);
		if (g_pt_lv_record->bg_about != NULL)      	lv_obj_del(g_pt_lv_record->bg_about);						// 删除背景
		grecord_run_state = 0;
//		/* 释放内存 */
		vPortFree(g_pt_lv_record);

		/* 清空屏幕，返回桌面 */
		lv_record_info();

	}
}

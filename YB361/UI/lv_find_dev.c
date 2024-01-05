/*
 * lv_find_dev.c
 *
 *  Created on: Dec 15, 2023
 *      Author: hxd
 */

/* lvgl
 * */
#include "lvgl/lvgl.h"
#include "UI/lv_typedef.h"
#include "UI/lv_main_page.h"
#include "UI/lv_about_page.h"
#include "UI/lv_find_dev.h"
#include "UI/lv_dev_info_page.h"


static PT_lv_find_dev g_pt_lv_find_dev;  // 数据结构体

uint8_t gfind_dev_state = 0;



char dev_id[10][20] = {"P69R98E200F0","P69R98E200F1","P69R98E200F2","P69R98E200F3","P69R98E200F4","P69R98E200F5","P69R98E200F6","P69R98E200F7","P69R98E200F8",\
                        "P69R98E200F9"};


static void lv_find_dev_init(void);
static void event_handler_back_to_home(lv_event_t *e);
static void event_handler_find_btn(lv_event_t *e);
static void event_handler_select_btn(lv_event_t *e);
static void event_handler_find_dev(lv_event_t *e);
/* lv_find_dev
 * 查找设备界面
 * */
void lv_find_dev(void)
{
	g_pt_lv_find_dev = (T_lv_find_dev*)pvPortMalloc(sizeof(T_lv_find_dev));
	g_pt_lv_find_dev->bg_about = lv_obj_create(lv_scr_act());/*创建背景*/

	lv_obj_set_size(g_pt_lv_find_dev->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_find_dev->bg_about, 0, 0);/*设置圆角*/
	lv_obj_align(g_pt_lv_find_dev->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*设置padding*/
	set_user_style(g_pt_lv_find_dev->bg_about);

	lv_obj_set_size(g_pt_lv_find_dev->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_find_dev->bg_about, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_find_dev->bg_about, lv_color_hex(BACK_COLOR), 0);
	lv_find_dev_init();/*界面初始化*/

	add_title(g_pt_lv_find_dev->bg_about, (const char *)gdev_param.bind_devId);
	add_back(g_pt_lv_find_dev->bg_about, event_handler_back_to_home);


}

/* lv_find_dev_refresh
 * 刷新查找设备界面
 * */
void lv_find_dev_refresh(void)
{
	uint16_t i;
	if(gfind_dev_state == 0)
		return;
	lv_obj_t *list_btn;
	//lv_list_clean(g_pt_lv_find_dev->dev_list);
	/*
	LOG_D("gdev_param.dev_list.nelts = %d", gdev_param.dev_list.nelts);
	for(i = 0; i < gdev_param.dev_list.nelts; i++)
	{
		list_btn = lv_list_add_btn(g_pt_lv_find_dev->dev_list, LV_SYMBOL_FILE, (const char*)gdev_param.dev_list.dev_Id + i * gdev_param.dev_list.elt_size);
		lv_obj_add_event_cb(list_btn, event_handler_find_dev, LV_EVENT_CLICKED, NULL);
	}
	*/
	for(i = 0; i < 10; i++)
    {

		list_btn = lv_list_add_btn(g_pt_lv_find_dev->dev_list, LV_SYMBOL_FILE, (const char*)dev_id[i]);
		lv_obj_set_style_bg_color(list_btn, lv_color_hex(BACK_COLOR), 0);
		lv_obj_add_event_cb(list_btn, event_handler_find_dev, LV_EVENT_CLICKED, NULL);

    }


}


/* lv_find_dev_init
 * 查找设备信息界面初始化
 * */
static void lv_find_dev_init(void)
{
	gfind_dev_state = 1;

	g_pt_lv_find_dev->last_btn = NULL;
	static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(BACK_COLOR));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 2);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
//	lv_style_set_border_width(&style_cont, 0);
	/*创建容器 */
	lv_obj_t *cont_mode = lv_obj_create(g_pt_lv_find_dev->bg_about);
	lv_obj_set_size(cont_mode, LV_FIND_US_CONT_W, LV_FIND_US_CONT_H);/*设置对象大小*/
	lv_obj_add_style(cont_mode, &style_cont, 0);/*默认状态下的style*/
	lv_obj_align(cont_mode, LV_ALIGN_TOP_MID, 0, LV_FIND_US_TITLE_SPACE);/*设置位置,在他父对象的顶部中间*/
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
	g_pt_lv_find_dev->find_btn = lv_btn_create(cont_mode);
	btn = g_pt_lv_find_dev->find_btn;
	lv_obj_add_style(btn, &style_btn, 0);/*添加样式*/
	lv_obj_set_style_bg_color(btn, lv_color_hex(RED_HEX), LV_STATE_PRESSED);/*按下颜色变为红色*/
	lv_obj_set_size(btn, LV_FIND_BTN_W, LV_FIND_BTN_H);
	lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 5);
	lv_obj_fade_in(btn, 0, 0);/*淡入*/
	/*添加点击事件*/
	lv_obj_add_event_cb(g_pt_lv_find_dev->find_btn, event_handler_find_btn, LV_EVENT_CLICKED, NULL);

	/*创建btn中的标签*/
	lv_obj_t *label_find = lv_label_create(btn);
	lv_label_set_text(label_find, "查找设备");/*设置text*/
	lv_obj_set_style_text_color(label_find, lv_color_hex(0x0), 0);/*设置字体颜色*/
	lv_obj_set_style_text_font(label_find, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_center(label_find);
	lv_obj_fade_in(label_find, 0, 0);

	g_pt_lv_find_dev->select_btn = lv_btn_create(cont_mode);
	btn = g_pt_lv_find_dev->select_btn;
	lv_obj_add_style(btn, &style_btn, 0);/*添加样式*/
	lv_obj_set_style_bg_color(btn, lv_color_hex(RED_HEX), LV_STATE_PRESSED);/*按下颜色变为红色*/
	lv_obj_set_size(btn, LV_FIND_BTN_W, LV_FIND_BTN_H);
	lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -15);
	lv_obj_fade_in(btn, 0, 0);/*淡入*/
	/*添加点击事件*/
	lv_obj_add_event_cb(g_pt_lv_find_dev->select_btn, event_handler_select_btn, LV_EVENT_CLICKED, NULL);

	/*创建btn中的标签*/
	lv_obj_t *label_select = lv_label_create(btn);
	lv_label_set_text(label_select, "绑定设备");/*设置text*/
	lv_obj_set_style_text_color(label_select, lv_color_hex(0x0), 0);/*设置字体颜色*/
	lv_obj_set_style_text_font(label_select, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_center(label_select);
	lv_obj_fade_in(label_select, 0, 0);

	/* 创建列表
	 * */
	g_pt_lv_find_dev->dev_list = lv_list_create(cont_mode);
	lv_obj_set_style_bg_color(g_pt_lv_find_dev->dev_list, lv_color_hex(BACK_COLOR), 0);
	lv_obj_set_size(g_pt_lv_find_dev->dev_list, LV_FIND_LIST_W, LV_FIND_LIST_H);
	lv_obj_align_to(g_pt_lv_find_dev->dev_list, g_pt_lv_find_dev->find_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
	lv_obj_t *list_btn;
	uint16_t i;
	#if 0
	for(i = 0; i < 10; i++)
    {

		list_btn = lv_list_add_btn(g_pt_lv_find_dev->dev_list, LV_SYMBOL_FILE, (const char*)dev_id[i]);
		lv_obj_set_style_bg_color(list_btn, lv_color_hex(BACK_COLOR), 0);
		lv_obj_add_event_cb(list_btn, event_handler_find_dev, LV_EVENT_CLICKED, NULL);

    }
	#else
	for(i = 0; i < gdev_param.dev_list.nelts; i++)
	{
		list_btn = lv_list_add_btn(g_pt_lv_find_dev->dev_list, LV_SYMBOL_FILE, (const char*)gdev_param.dev_list.dev_Id + i * gdev_param.dev_list.elt_size);
		lv_obj_set_style_bg_color(list_btn, lv_color_hex(BACK_COLOR), 0);
		lv_obj_add_event_cb(list_btn, event_handler_find_dev, LV_EVENT_CLICKED, NULL);
	}
	#endif


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
		if(g_pt_lv_find_dev->bg_about != NULL) lv_obj_del(g_pt_lv_find_dev->bg_about);

		vPortFree(g_pt_lv_find_dev);
		gfind_dev_state = 0;
		lv_main_page_demo_home(100);
	}
}

/* event_handler_find_btn
 * 查找设备按钮点击事件回调函数
 * */
static void event_handler_find_btn(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//int result = RT_EOK;
	if(code == LV_EVENT_CLICKED)
	{
	    /*
		result = mqtt_send_deviceFinding();
		if(result != RT_EOK)
		{
			LOG_D("send device finding error.");
		}
		*/
	}
}

/* event_handler_select_btn
 * 绑定设备按钮点击事件回调函数
 * */
static void event_handler_select_btn(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_CLICKED)
	{
		if (g_pt_lv_find_dev->bg_about != NULL)      	lv_obj_del(g_pt_lv_find_dev->bg_about);						// 删除背景

		gfind_dev_state = 0;

		/* 更新设备ID，重新订阅报文
		 * */
		//mqtt_subscribe_again(gdev_param.bind_devId);

		/* 释放内存 */
		vPortFree(g_pt_lv_find_dev);
		gfind_dev_state = 0;
		/* 清空屏幕，返回桌面 */
		lv_main_page_demo_home(100);

	}
}

/* event_handler_find_dev
 * 列表元素点击事件回调函数
 * */
static void event_handler_find_dev(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)
	{
		LOG_D("clicked :%s", lv_list_get_btn_text(g_pt_lv_find_dev->dev_list, obj));
		memcpy(gdev_param.bind_devId, lv_list_get_btn_text(g_pt_lv_find_dev->dev_list, obj), strlen(lv_list_get_btn_text(g_pt_lv_find_dev->dev_list, obj)));

		if(g_pt_lv_find_dev->last_btn != NULL)/*背景改变颜色*/
        {
            lv_obj_set_style_bg_color(g_pt_lv_find_dev->last_btn, lv_color_hex(BACK_COLOR), 0);

        }
		lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0000), 0);
        g_pt_lv_find_dev->last_btn = obj;/*更新上一次点击的按键*/
	}
}




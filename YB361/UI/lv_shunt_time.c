/*
 * lv_shunt_time.c
 *
 *  Created on: Dec 18, 2023
 *      Author: hxd
 */

#include "lvgl/lvgl.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "lv_about_page.h"
#include "lv_find_dev.h"
#include "lv_dev_info_page.h"
#include "lv_report_page.h"
#include "lv_report_page.h"
#include "lv_long_time_page.h"
#include "lv_shunt_time.h"
static PT_lv_shunttime g_pt_lv_shunttime;  // 数据结构体

uint8_t gshunt_time = 0;
extern const char *btnm_map[];

static void lv_set_shunt_time_init(void);
static void event_handler_back_to_home(lv_event_t *e);
static void event_handler_select_time(lv_event_t *e);
static void event_handler_confirm_btn(lv_event_t *e);
static void event_handler_cancel_btn(lv_event_t *e);
static void event_handler_textarea(lv_event_t *e);
static void event_handler_btnm(lv_event_t *e);
/* lv_set_long_time
 * 设置久坐提醒时间界面
 */
void lv_set_shunt_time(void)
{

	g_pt_lv_shunttime = (T_lv_shunttime*)pvPortMalloc(sizeof(T_lv_shunttime));
	g_pt_lv_shunttime->bg_about = lv_obj_create(lv_scr_act());/*创建背景*/

	lv_obj_set_size(g_pt_lv_shunttime->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_shunttime->bg_about, 0, 0);/*设置圆角*/
	lv_obj_align(g_pt_lv_shunttime->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*设置padding*/
	set_user_style(g_pt_lv_shunttime->bg_about);

	lv_obj_set_size(g_pt_lv_shunttime->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_shunttime->bg_about, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_shunttime->bg_about, lv_color_hex(BACK_COLOR), 0);
	lv_set_shunt_time_init();/*界面初始化*/

	add_title(g_pt_lv_shunttime->bg_about, (const char *)gdev_param.bind_devId);
	add_back(g_pt_lv_shunttime->bg_about, event_handler_back_to_home);

}

/* create_the_symbol_label
 * 创建对号标签
 * parent:在那个对象上创建
 */
static void create_the_symbol_label(lv_obj_t *parent)
{
    g_pt_lv_shunttime->sym_btn = lv_label_create(parent);
    lv_label_set_text(g_pt_lv_shunttime->sym_btn, LV_SYMBOL_OK);/*设置text*/
    lv_obj_set_style_text_color(g_pt_lv_shunttime->sym_btn, lv_color_hex(GREEN_HEX), 0);
    lv_obj_align(g_pt_lv_shunttime->sym_btn, LV_ALIGN_RIGHT_MID, -4, 0);

}

/* create_the_text_area
 * 创建文本框
 * parent:在那个对象上创建
 */
static void create_the_text_area(lv_obj_t *parent, lv_obj_t *align_to)
{
    /*创建文本框*/
	g_pt_lv_shunttime->usr_text = lv_textarea_create(parent);
	lv_obj_set_style_bg_color(g_pt_lv_shunttime->usr_text, lv_color_hex(BACK_COLOR), 0);
	lv_obj_set_style_radius(g_pt_lv_shunttime->usr_text, 5, 0);
	lv_obj_set_size(g_pt_lv_shunttime->usr_text , LV_SHUNT_TEXTAREA_W, LV_SHUNT_TEXTAREA_H);
	lv_textarea_set_one_line(g_pt_lv_shunttime->usr_text, true);/*设置为一行*/
	lv_obj_align_to(g_pt_lv_shunttime->usr_text, align_to, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
	//lv_textarea_set_accepted_chars(g_pt_lv_longtime->usr_text, "0123456789");/*设置可以接受字符列表*/
	lv_obj_set_style_border_color(g_pt_lv_shunttime->usr_text, lv_color_hex(GREY_HEX), 0);
	lv_obj_set_style_border_width(g_pt_lv_shunttime->usr_text, 1, 0);
	lv_obj_set_style_text_font(g_pt_lv_shunttime->usr_text, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_set_style_text_color(g_pt_lv_shunttime->usr_text, lv_color_hex(GREY_HEX), 0);
	lv_textarea_set_text(g_pt_lv_shunttime->usr_text, "可输入1-255");

    /*创建一个标签*/
    g_pt_lv_shunttime->use_label = lv_label_create(parent);
    lv_obj_t *label = g_pt_lv_shunttime->use_label;
    lv_label_set_text(label, "分钟");
	lv_obj_set_style_text_font(label, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align_to(label, g_pt_lv_shunttime->usr_text, LV_ALIGN_OUT_RIGHT_MID, 10, 0);/*设置位置*/
	lv_obj_fade_in(label, 0, 0);/*淡入*/

    lv_obj_add_flag(g_pt_lv_shunttime->usr_text, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(g_pt_lv_shunttime->use_label, LV_OBJ_FLAG_HIDDEN);

    /*添加事件回调函数*/
    lv_obj_add_event_cb(g_pt_lv_shunttime->usr_text, event_handler_textarea, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(g_pt_lv_shunttime->usr_text, event_handler_textarea, LV_EVENT_DEFOCUSED, NULL);
}

/* create_the_btnmatrix
 * 创建一个按钮矩阵
 */
static void create_the_btnmatrix(void)
{
    lv_obj_t *btnm;
    g_pt_lv_shunttime->btnm = lv_btnmatrix_create(g_pt_lv_shunttime->bg_about);
    btnm = g_pt_lv_shunttime->btnm;
    lv_obj_set_size(btnm, LV_LONG_BTNM_W, LV_LONG_BTNM_H);
    lv_obj_align(btnm, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_color(btnm, lv_color_hex(BACK_COLOR), 0);
    //lv_obj_set_style_border_color(btnm, lv_color_hex(GREY_HEX), 0);
    lv_obj_set_style_border_width(btnm, 0, 0);
    lv_obj_add_event_cb(btnm, event_handler_btnm, LV_EVENT_VALUE_CHANGED, g_pt_lv_shunttime->usr_text);
    lv_obj_clear_flag(btnm, LV_OBJ_FLAG_CLICK_FOCUSABLE); /*to keep the text area focused on button clicks*/
    lv_btnmatrix_set_map(btnm, btnm_map);
    lv_obj_add_flag(g_pt_lv_shunttime->btnm, LV_OBJ_FLAG_HIDDEN);/*隐藏*/

}

/* lv_set_shunt_time_init
 * 界面初始化
 * */
static void lv_set_shunt_time_init(void)
{
    gshunt_time = gdev_param.shutdownMode;
    /*设置容器样式*/
    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(BACK_COLOR));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
	lv_style_set_border_width(&style_cont, 0);

    lv_obj_t * btn;
    /*创建容器*/
    g_pt_lv_shunttime->min_10_btn = lv_obj_create(g_pt_lv_shunttime->bg_about);
    btn = g_pt_lv_shunttime->min_10_btn;
    lv_obj_set_size(btn, LV_LONG_BTN_W, LV_LONG_BTN_H);
    lv_obj_add_style(btn, &style_cont, 0);/*默认状态下的样式*/
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, LV_SET_US_TITLE_SPACE);/*设置位置,在他父对象的顶部中间*/
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    /*添加点击时间函数*/
    lv_obj_add_event_cb(btn, event_handler_select_time, LV_EVENT_CLICKED, NULL);


    /*创建标签*/
    lv_obj_t *label_10 = lv_label_create(btn);
    lv_label_set_text(label_10, "10分钟");
	lv_obj_set_style_text_font(label_10, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_10, LV_ALIGN_LEFT_MID, 5, 0);/*设置位置*/
	lv_obj_fade_in(label_10, 0, 0);/*淡入*/



    g_pt_lv_shunttime->min_20_btn = lv_obj_create(g_pt_lv_shunttime->bg_about);
    btn = g_pt_lv_shunttime->min_20_btn;
    lv_obj_set_size(btn, LV_LONG_BTN_W, LV_LONG_BTN_H);
    lv_obj_add_style(btn, &style_cont, 0);/*默认状态下的样式*/
    lv_obj_align_to(btn, g_pt_lv_shunttime->min_10_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    /*添加点击时间函数*/
    lv_obj_add_event_cb(btn, event_handler_select_time, LV_EVENT_CLICKED, NULL);

    /*创建标签*/
    lv_obj_t *label_20 = lv_label_create(btn);
    lv_label_set_text(label_20, "20分钟");
	lv_obj_set_style_text_font(label_20, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_20, LV_ALIGN_LEFT_MID, 5, 0);/*设置位置*/
	lv_obj_fade_in(label_20, 0, 0);/*淡入*/


    g_pt_lv_shunttime->min_30_btn = lv_obj_create(g_pt_lv_shunttime->bg_about);
    btn = g_pt_lv_shunttime->min_30_btn;
    lv_obj_set_size(btn, LV_LONG_BTN_W, LV_LONG_BTN_H);
    lv_obj_add_style(btn, &style_cont, 0);/*默认状态下的样式*/
    lv_obj_align_to(btn, g_pt_lv_shunttime->min_20_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    /*添加点击时间函数*/
    lv_obj_add_event_cb(btn, event_handler_select_time, LV_EVENT_CLICKED, NULL);

    /*创建标签*/
    lv_obj_t *label_30 = lv_label_create(btn);
    lv_label_set_text(label_30, "30分钟");
	lv_obj_set_style_text_font(label_30, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_30, LV_ALIGN_LEFT_MID, 5, 0);/*设置位置*/
	lv_obj_fade_in(label_30, 0, 0);/*淡入*/

    g_pt_lv_shunttime->usr_btn = lv_obj_create(g_pt_lv_shunttime->bg_about);
    btn = g_pt_lv_shunttime->usr_btn;
    lv_obj_set_size(btn, LV_LONG_BTN_W, LV_LONG_BTN_H);
    lv_obj_add_style(btn, &style_cont, 0);/*默认状态下的样式*/
    lv_obj_align_to(btn, g_pt_lv_shunttime->min_30_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    /*添加点击时间函数*/
    lv_obj_add_event_cb(btn, event_handler_select_time, LV_EVENT_CLICKED, NULL);

    /*创建标签*/
    lv_obj_t *label_usr = lv_label_create(btn);
    lv_label_set_text(label_usr, "自定义时间");
	lv_obj_set_style_text_font(label_usr, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_usr, LV_ALIGN_LEFT_MID, 5, 0);/*设置位置*/
	lv_obj_fade_in(label_usr, 0, 0);/*淡入*/
    create_the_text_area(g_pt_lv_shunttime->usr_btn, label_usr);
    create_the_btnmatrix();
    /*根据时间画对号*/
    switch(gshunt_time)
    {
    case 10:
        create_the_symbol_label(g_pt_lv_shunttime->min_10_btn);
        break;
    case 20:
        create_the_symbol_label(g_pt_lv_shunttime->min_20_btn);
        break;
    case 30:
        create_the_symbol_label(g_pt_lv_shunttime->min_30_btn);
        break;
    default:
        create_the_symbol_label(g_pt_lv_shunttime->usr_btn);
        lv_obj_clear_flag(g_pt_lv_shunttime->usr_text, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(g_pt_lv_shunttime->use_label, LV_OBJ_FLAG_HIDDEN);
        break;
    }


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

    /*创建两个按键*/
    g_pt_lv_shunttime->confirm_btn = lv_btn_create(g_pt_lv_shunttime->bg_about);
	btn = g_pt_lv_shunttime->confirm_btn;
	lv_obj_add_style(btn, &style_btn, 0);/*添加样式*/
	lv_obj_set_size(btn, LV_CONFIRM_BTN_W, LV_CONFIRM_BTN_H);
	lv_obj_set_style_bg_color(btn, lv_color_hex(LIME_GREEN_HEX), 0);
	lv_obj_align_to(btn, g_pt_lv_shunttime->usr_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
	lv_obj_fade_in(btn, 0, 0);/*淡入*/
	/*添加点击时间函数*/
    lv_obj_add_event_cb(btn, event_handler_confirm_btn, LV_EVENT_CLICKED, NULL);
	/*创建标签*/
	lv_obj_t *label_confirm = lv_label_create(btn);
    lv_label_set_text(label_confirm, "确认");
	lv_obj_set_style_text_font(label_confirm, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_center(label_confirm);
	lv_obj_fade_in(label_confirm, 0, 0);/*淡入*/


    g_pt_lv_shunttime->cancel_btn = lv_btn_create(g_pt_lv_shunttime->bg_about);
	btn = g_pt_lv_shunttime->cancel_btn;
	lv_obj_add_style(btn, &style_btn, 0);/*添加样式*/
	lv_obj_set_size(btn, LV_CONFIRM_BTN_W, LV_CONFIRM_BTN_H);
	//lv_obj_set_style_bg_color(btn, lv_color_hex(GREEN_HEX), 0);
	lv_obj_align_to(btn, g_pt_lv_shunttime->confirm_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
	lv_obj_fade_in(btn, 0, 0);/*淡入*/
	/*添加点击时间函数*/
    lv_obj_add_event_cb(btn, event_handler_cancel_btn, LV_EVENT_CLICKED, NULL);
    /*创建标签*/
	lv_obj_t *label_cancel = lv_label_create(btn);
    lv_label_set_text(label_cancel, "取消");
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(BLACK_HEX), 0);
	lv_obj_set_style_text_font(label_cancel, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_center(label_cancel);
	lv_obj_fade_in(label_cancel, 0, 0);/*淡入*/

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
		if(g_pt_lv_shunttime->bg_about != NULL) lv_obj_del(g_pt_lv_shunttime->bg_about);

		vPortFree(g_pt_lv_shunttime);
		//gfind_dev_state = 0;
		lv_report();
	}
}

/* event_handler_select_time
 * 选择时间点击回调函数
 * */
static void event_handler_select_time(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    lv_obj_del(g_pt_lv_shunttime->sym_btn);/*先删除*/
	    lv_obj_add_flag(g_pt_lv_shunttime->usr_text, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(g_pt_lv_shunttime->use_label, LV_OBJ_FLAG_HIDDEN);
        if(obj == g_pt_lv_shunttime->min_10_btn)
        {
            create_the_symbol_label(g_pt_lv_shunttime->min_10_btn);
            gshunt_time = 10;
        }
        else if(obj == g_pt_lv_shunttime->min_20_btn)
        {
            create_the_symbol_label(g_pt_lv_shunttime->min_20_btn);
            gshunt_time = 20;
        }
        else if(obj == g_pt_lv_shunttime->min_30_btn)
        {
            create_the_symbol_label(g_pt_lv_shunttime->min_30_btn);
            gshunt_time = 30;
        }
        else if(obj == g_pt_lv_shunttime->usr_btn)
        {
            create_the_symbol_label(g_pt_lv_shunttime->usr_btn);
            lv_obj_clear_flag(g_pt_lv_shunttime->usr_text, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(g_pt_lv_shunttime->use_label, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_text_color(g_pt_lv_shunttime->usr_text, lv_color_hex(GREY_HEX), 0);
            lv_textarea_set_accepted_chars(g_pt_lv_shunttime->usr_text, NULL);
            lv_textarea_set_max_length(g_pt_lv_shunttime->usr_text, 0);
            lv_textarea_set_text(g_pt_lv_shunttime->usr_text, "可输入1-255");
        }
	}

}
/* event_handler_confirm_btn
 * 确认按键点击时间回调函数
 */
static void event_handler_confirm_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	int result = RT_EOK;
	int text_time;
	if(code == LV_EVENT_CLICKED)
	{
	    /*先判断自定义时间*/
        if(lv_obj_get_parent(g_pt_lv_shunttime->sym_btn) == g_pt_lv_shunttime->usr_btn)
        {
            printf("text: %s\r\n", lv_textarea_get_text(g_pt_lv_shunttime->usr_text));
            text_time = atoi(lv_textarea_get_text(g_pt_lv_shunttime->usr_text));
            //if(text_time > 0 && text_time < 256)/*不处理范围*/
            {
                gshunt_time = text_time;
            }
            printf("number = %d\r\n", text_time);
        }

		if (g_pt_lv_shunttime->cancel_btn != NULL)      lv_obj_del(g_pt_lv_shunttime->cancel_btn);
		if (g_pt_lv_shunttime->confirm_btn != NULL)     lv_obj_del(g_pt_lv_shunttime->confirm_btn);
		if (g_pt_lv_shunttime->sym_btn != NULL)      	lv_obj_del(g_pt_lv_shunttime->sym_btn);
		if (g_pt_lv_shunttime->usr_btn != NULL)      	lv_obj_del(g_pt_lv_shunttime->usr_btn);
		if (g_pt_lv_shunttime->min_30_btn != NULL)      lv_obj_del(g_pt_lv_shunttime->min_30_btn);
		if (g_pt_lv_shunttime->min_20_btn != NULL)      lv_obj_del(g_pt_lv_shunttime->min_20_btn);
		if (g_pt_lv_shunttime->min_10_btn != NULL)      lv_obj_del(g_pt_lv_shunttime->min_10_btn);
		if (g_pt_lv_shunttime->bg_about != NULL)      	lv_obj_del(g_pt_lv_shunttime->bg_about);						// 删除背景

//		/* 释放内存 */
		vPortFree(g_pt_lv_shunttime);

		gdev_param.shutdownMode = gshunt_time;/*更新久坐提醒时长*/


		//result = mqtt_send_setting(gdev_param.longTimeMode, gdev_param.shutdownMode);
		if(result != RT_EOK)
		{
			LOG_E("mqtt_send_setting error...");
		}

		lv_report();
	}
}

/* event_handler_cancel_btn
 * 取消按钮点击回调函数
 */
static void event_handler_cancel_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_CLICKED)
	{
		if (g_pt_lv_shunttime->cancel_btn != NULL)      lv_obj_del(g_pt_lv_shunttime->cancel_btn);
		if (g_pt_lv_shunttime->confirm_btn != NULL)     lv_obj_del(g_pt_lv_shunttime->confirm_btn);
		if (g_pt_lv_shunttime->sym_btn != NULL)      	lv_obj_del(g_pt_lv_shunttime->sym_btn);
		if (g_pt_lv_shunttime->usr_btn != NULL)      	lv_obj_del(g_pt_lv_shunttime->usr_btn);
		if (g_pt_lv_shunttime->min_30_btn != NULL)      lv_obj_del(g_pt_lv_shunttime->min_30_btn);
		if (g_pt_lv_shunttime->min_20_btn != NULL)      lv_obj_del(g_pt_lv_shunttime->min_20_btn);
		if (g_pt_lv_shunttime->min_10_btn != NULL)      lv_obj_del(g_pt_lv_shunttime->min_10_btn);
		if (g_pt_lv_shunttime->bg_about != NULL)      	lv_obj_del(g_pt_lv_shunttime->bg_about);						// 删除背景

	//		/* 释放内存 */
		vPortFree(g_pt_lv_shunttime);

		/* 清空屏幕 */
		lv_report();
	}
}

/* event_handler_textarea
 * 文本框事件回调函数
 */
static void event_handler_textarea(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_FOCUSED)
	{
        lv_textarea_set_text(obj,"");
        lv_textarea_set_cursor_pos(obj, 0);
        lv_textarea_set_accepted_chars(obj, "0123456789");
        lv_textarea_set_max_length(obj, 3);
        lv_obj_clear_flag(g_pt_lv_shunttime->btnm, LV_OBJ_FLAG_HIDDEN);/*显示按钮矩阵*/
        lv_obj_set_style_text_color(g_pt_lv_shunttime->usr_text, lv_color_hex(BLACK_HEX), 0);
	}
	else if(code == LV_EVENT_DEFOCUSED)
    {
        lv_obj_add_flag(g_pt_lv_shunttime->btnm, LV_OBJ_FLAG_HIDDEN);/*隐藏按钮矩阵*/
    }
}



/* event_handler_btnm
 * 按键矩阵事件回调函数
 */
static void event_handler_btnm(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *ta = lv_event_get_user_data(e);
    const char *txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));
    if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)
    {
        lv_textarea_del_char(ta);
    }
    else if(strcmp(txt, LV_SYMBOL_LEFT) == 0)
    {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_textarea_add_text(ta, txt);
    }
}


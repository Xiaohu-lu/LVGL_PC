/*
 * lv_timing_list_page.c
 *
 *  Created on: Dec 20, 2023
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
#include "lv_timing_list_page.h"
#include "lv_timing_edit_page.h"

static PT_lv_timinglist g_pt_lv_timinglist;  // 数据结构体

static uint8_t timer_index = 0;

static void lv_timing_list_init(void);
static void event_handler_back_to_home(lv_event_t *e);
static void event_handler_switch(lv_event_t *e);
static void event_handler_edit_timer(lv_event_t *e);
/* lv_timing_list
 * 设置定时提醒界面
 */
void lv_timing_list(void)
{

	g_pt_lv_timinglist = (T_lv_timinglist*)pvPortMalloc(sizeof(T_lv_timinglist));
	g_pt_lv_timinglist->bg_about = lv_obj_create(lv_scr_act());/*创建背景*/

	lv_obj_set_size(g_pt_lv_timinglist->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_timinglist->bg_about, 0, 0);/*设置圆角*/
	lv_obj_align(g_pt_lv_timinglist->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*设置padding*/
	set_user_style(g_pt_lv_timinglist->bg_about);

	lv_obj_set_size(g_pt_lv_timinglist->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_timinglist->bg_about, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_timinglist->bg_about, lv_color_hex(GREY_HEX), 0);
	lv_timing_list_init();/*界面初始化*/

	add_title(g_pt_lv_timinglist->bg_about, (const char *)gdev_param.bind_devId);
	add_back(g_pt_lv_timinglist->bg_about, event_handler_back_to_home);

}

uint8_t get_select_timer_index(void)
{
    return timer_index;
}

/* lv_timing_list_init
 *
 */
static void lv_timing_list_init(void)
{

    /*创建一个标签*/
    lv_obj_t *label_1 = lv_label_create(g_pt_lv_timinglist->bg_about);
    lv_label_set_text(label_1, "定时列表");
	lv_obj_set_style_text_font(label_1, &song_font14, 0);/*默认状态下的字体样式*/
    lv_obj_align(label_1, LV_ALIGN_TOP_LEFT, 10, LV_TIMER_US_TITLE_SPACE);/*设置位置,在他父对象的顶部中间*/
	lv_obj_fade_in(label_1, 0, 0);/*淡入*/


    /*创建容器样式*/
    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
//	lv_style_set_border_width(&style_cont, 0);


    uint16_t i;
    lv_obj_t *cont;
    lv_obj_t *align_obj;/*对齐对象*/
    lv_obj_t *btn;
    lv_obj_t *label_big;
    lv_obj_t *label_small;
    align_obj = label_1;

    g_pt_lv_timinglist->timer_list = pvPortMalloc(sizeof(lv_obj_t*) * gdev_param.timer_list.nelts);
    //g_pt_lv_timinglist->switch_list = pvPortMalloc(sizeof(lv_obj_t*) * gdev_param.timer_list.nelts);
  #if 1
    for(i = 0; i < gdev_param.timer_list.nelts; i++)
    {
        /*创建容器*/
        g_pt_lv_timinglist->timer_list[i] = lv_obj_create(g_pt_lv_timinglist->bg_about);
        cont = g_pt_lv_timinglist->timer_list[i];
        lv_obj_add_style(cont, &style_cont, 0);/*添加默认样式*/
        lv_obj_set_size(cont, LV_TIMER_CONT_W, LV_TIMER_CONT_H);/*设置大小*/
        if(i == 0)
        {
            lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, LV_TIMER_US_TITLE_SPACE +20);
        }
        else
        {
            lv_obj_align_to(cont, align_obj, LV_ALIGN_OUT_BOTTOM_MID, 0, LV_CONT_ALIGN_SPACE);
        }
        /*设置容器可以点击*/
        lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
        lv_obj_add_event_cb(cont, event_handler_edit_timer, LV_EVENT_CLICKED, NULL);/*进入编辑定时器界面*/

        /*创建switch*/
        btn = lv_switch_create(cont);
        lv_obj_set_size(btn, LV_SWITCH_W, LV_SWITCH_H);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, 0, 0);

        /*设置switch初始状态*/
        if(gdev_param.timer_list.timer[i].timerSwitch)
        {
            lv_obj_add_state(btn, LV_STATE_CHECKED);/*设置开状态*/
        }
        else
        {
            lv_obj_clear_state(btn, LV_STATE_CHECKED);/*设置开状态*/
        }
        /*添加状态变换事件回调函数*/
        lv_obj_add_event_cb(btn, event_handler_switch, LV_EVENT_VALUE_CHANGED, NULL);


        /*创建大标签*/
        label_big = lv_label_create(cont);
        lv_label_set_text(label_big, gdev_param.timer_list.timer[i].time);
        lv_obj_set_style_text_font(label_big, &lv_font_montserrat_22, 0);/*默认状态下的字体样式*/
        lv_obj_align(label_big, LV_ALIGN_LEFT_MID, 10, -10);/*设置位置,在他父对象的顶部中间*/
        lv_obj_fade_in(label_big, 0, 0);/*淡入*/

        /*创建小标签*/
        label_small = lv_label_create(cont);
        lv_label_set_text(label_small, "每天");
        lv_obj_set_style_text_font(label_small, &song_font14, 0);/*默认状态下的字体样式*/
        lv_obj_align_to(label_small, label_big, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);/*设置位置,在他父对象的顶部中间*/
        lv_obj_fade_in(label_small, 0, 0);/*淡入*/


        align_obj = cont;
    }
#endif


    /*创建添加按钮*/

    static lv_style_t style_btn;
	lv_style_init(&style_btn);
	lv_style_set_bg_opa(&style_btn, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_bg_color(&style_btn, lv_color_hex(BLUE_HEX));/*设置背景颜色*/
	lv_style_set_pad_top(&style_btn,0);
	lv_style_set_pad_bottom(&style_btn, 0);
	lv_style_set_pad_left(&style_btn, 0);
	lv_style_set_pad_right(&style_btn, 0);
	lv_style_set_border_width(&style_btn, 0);
	lv_style_set_radius(&style_btn, 100);/*设置圆角*/

    g_pt_lv_timinglist->add_btn = lv_btn_create(g_pt_lv_timinglist->bg_about);
	lv_obj_add_style(g_pt_lv_timinglist->add_btn, &style_btn, 0);/*添加样式*/
	lv_obj_set_size(g_pt_lv_timinglist->add_btn, LV_ADD_BTN_SIZE, LV_ADD_BTN_SIZE);
	lv_obj_align(g_pt_lv_timinglist->add_btn, LV_ALIGN_BOTTOM_MID, 0, -20);
	lv_obj_fade_in(g_pt_lv_timinglist->add_btn, 0, 0);/*淡入*/
	/*添加点击事件回调函数*/
	lv_obj_add_event_cb(g_pt_lv_timinglist->add_btn, event_handler_edit_timer, LV_EVENT_CLICKED, NULL);

	/*添加标签*/
	lv_obj_t *label_2 = lv_label_create(g_pt_lv_timinglist->add_btn);
	lv_obj_set_style_text_font(label_2, &lv_font_montserrat_22, 0);
	lv_obj_set_style_text_color(label_2, lv_color_hex(WHITE_HEX), 0);
	lv_label_set_text(label_2, LV_SYMBOL_PLUS);
	lv_obj_center(label_2);
	lv_obj_fade_in(label_2, 0, 0);/*淡入*/

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
		if(g_pt_lv_timinglist->bg_about != NULL)    lv_obj_del(g_pt_lv_timinglist->bg_about);

        vPortFree(g_pt_lv_timinglist->timer_list);
		vPortFree(g_pt_lv_timinglist);

		lv_main_page_demo_home(100);
	}
}


/* event_handler_switch
 * switch值改变事件回调函数
 */
static void event_handler_switch(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_VALUE_CHANGED)
	{
		printf("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On":"Off");

	}
}

/* event_handler_edit_timer
 * 点击进入定时器编辑界面
 */
static void event_handler_edit_timer(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	uint16_t i;
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    /*判断那个容器被点击了*/
	    for(i = 0; i < gdev_param.timer_list.nelts; i++)
        {
            if(obj == g_pt_lv_timinglist->timer_list[i])
            {
                timer_index = i;
            }
        }
        if(obj == g_pt_lv_timinglist->add_btn)
        {
            timer_index = LV_ADD_TIMER_INDEX;
        }
        printf("timer_index = %d\r\n", timer_index);
		if(g_pt_lv_timinglist->bg_about != NULL)    lv_obj_del(g_pt_lv_timinglist->bg_about);

        vPortFree(g_pt_lv_timinglist->timer_list);
		vPortFree(g_pt_lv_timinglist);

		lv_timing_edit();/*进入定时器编辑界面*/
	}
}


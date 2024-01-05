/*
 * lv_report_page.c
 *
 *  Created on: Dec 15, 2023
 *      Author: hxd
 */
#include "lvgl/lvgl.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "lv_about_page.h"
#include "lv_find_dev.h"
#include "lv_dev_info_page.h"
#include "lv_report_page.h"
#include "lv_long_time_page.h"
#include "lv_shunt_time.h"
#include "lv_timing_list_page.h"
static PT_lv_report g_pt_lv_report;  // 数据结构体


static void lv_report_init(void);
static void event_handler_back_to_home(lv_event_t *e);
static void event_handler_setting_long_time(lv_event_t *e);
static void event_handler_setting_shunt_time(lv_event_t *e);
static void event_handler_setting_timing_time(lv_event_t *e);
void lv_report(void)
{

	g_pt_lv_report = (T_lv_report*)pvPortMalloc(sizeof(T_lv_report));
	g_pt_lv_report->bg_about = lv_obj_create(lv_scr_act());/*创建背景*/

	lv_obj_set_size(g_pt_lv_report->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_report->bg_about, 0, 0);/*设置圆角*/
	lv_obj_align(g_pt_lv_report->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*设置padding*/
	set_user_style(g_pt_lv_report->bg_about);

	lv_obj_set_size(g_pt_lv_report->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_report->bg_about, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_report->bg_about, lv_color_hex(BACK_COLOR), 0);
	lv_report_init();/*界面初始化*/

	add_title(g_pt_lv_report->bg_about, (const char *)gdev_param.bind_devId);
	add_back(g_pt_lv_report->bg_about, event_handler_back_to_home);

}

/* lv_report_init
 * 界面初始化
 * */
static void lv_report_init(void)
{
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
	lv_obj_t *cont_mode = lv_obj_create(g_pt_lv_report->bg_about);
	lv_obj_set_size(cont_mode, LV_REPORT_US_CONT_W, LV_REPORT_US_CONT_H);/*设置对象大小*/
	lv_obj_add_style(cont_mode, &style_cont, 0);/*默认状态下的style*/
	lv_obj_align(cont_mode, LV_ALIGN_TOP_MID, 0, LV_REPORT_US_TITLE_SPACE);/*设置位置,在他父对象的顶部中间*/
	lv_obj_fade_in(cont_mode, 0, 0);/*淡入*/


	/*创建一个标签*/
	lv_obj_t *label_mode = lv_label_create(cont_mode);
	lv_label_set_text(label_mode, "提醒模式");
	lv_obj_set_style_text_font(label_mode, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_mode, LV_ALIGN_TOP_LEFT, LV_DEVINFO_US_LABEL_SPACE, 0);/*设置位置*/
	lv_obj_fade_in(label_mode, 0, 0);/*淡入*/



	lv_obj_t *arc;
	/*创建圆弧*/
	g_pt_lv_report->rate_arc = lv_arc_create(cont_mode);
	arc = g_pt_lv_report->rate_arc;
	lv_arc_set_mode(arc, LV_ARC_MODE_REVERSE);/*设置圆弧的模式,反向模式*/

    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);/*取消旋钮*/
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);/*不可调整*/
	lv_obj_set_size(arc, LV_REPORT_ARC_W, LV_REPORT_ARC_H);
	lv_arc_set_rotation(arc, 180);
	lv_arc_set_bg_angles(arc, 0, 360);/*设置背景弧度*/
	lv_arc_set_value(arc, 20);
	lv_obj_center(arc);

    /*创建圆弧上面的标签*/
    lv_obj_t *lable_cont = lv_obj_create(cont_mode);
    set_user_style(lable_cont);/*取消padding和边框*/
    lv_obj_set_size(lable_cont, LV_REPORT_LABEL_CONT_W, LV_REPORT_LABEL_CONT_H);/*设置大小*/
    lv_obj_set_style_bg_color(lable_cont, lv_color_hex(CORNFLOWERBLUE_HEX), 0);/*设置背景颜色*/
    lv_obj_align_to(lable_cont, arc, LV_ALIGN_OUT_TOP_MID, 0, -8);/*设置位置*/
    lv_obj_set_style_radius(lable_cont, 10, 0);/*设置圆角*/
    /*创建一个标签*/
	lv_obj_t *label_1 = lv_label_create(lable_cont);
	lv_label_set_text(label_1, "很好,继续保持哦");
	lv_obj_set_style_text_color(label_1, lv_color_hex(WHITE_HEX), 0);
	lv_obj_set_style_text_font(label_1, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_center(label_1);
	lv_obj_fade_in(label_1, 0, 0);/*淡入*/

	/*创建圆弧下面的标签*/
    static lv_style_t style_cont1;/*容器样式*/
	lv_style_init(&style_cont1);
	lv_style_set_bg_opa(&style_cont1, LV_OPA_0);/*设置背景透明度*/
	lv_style_set_radius(&style_cont1, 0);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont1, lv_color_hex(BACK_COLOR));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont1, 0);
	lv_style_set_pad_bottom(&style_cont1, 0);
	lv_style_set_pad_left(&style_cont1, 0);
	lv_style_set_pad_right(&style_cont1, 0);
	lv_style_set_border_width(&style_cont1, 0);
	/*创建一个中间容器*/
    lv_obj_t *mid_cont = lv_obj_create(cont_mode);
    lv_obj_add_style(mid_cont, &style_cont1, 0);
    lv_obj_set_size(mid_cont, LV_BTM_LABEL_W, LV_BTM_LABEL_H);
    lv_obj_align_to(mid_cont, g_pt_lv_report->rate_arc, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    /*添加中间容器的标签*/
    lv_obj_t *mid_label_1 = lv_label_create(mid_cont);
	lv_label_set_text(mid_label_1, "3 分钟");
	lv_obj_set_style_text_font(mid_label_1, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(mid_label_1, LV_ALIGN_TOP_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(mid_label_1, 0, 0);/*淡入*/

	lv_obj_t *mid_label_2 = lv_label_create(mid_cont);
	lv_label_set_text(mid_label_2, "不良坐姿");
	lv_obj_set_style_text_font(mid_label_2, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(mid_label_2, LV_ALIGN_BOTTOM_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(mid_label_2, 0, 0);/*淡入*/



    /*创建一个左容器*/
    lv_obj_t *left_cont = lv_obj_create(cont_mode);
    lv_obj_add_style(left_cont, &style_cont1, 0);
    lv_obj_set_size(left_cont, LV_BTM_LABEL_W, LV_BTM_LABEL_H);
    lv_obj_align_to(left_cont, mid_cont, LV_ALIGN_OUT_LEFT_MID, -10, 0);

    /*添加左容器的标签*/
    lv_obj_t *left_label_1 = lv_label_create(left_cont);
	lv_label_set_text(left_label_1, "60 分钟");
	lv_obj_set_style_text_font(left_label_1, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(left_label_1, LV_ALIGN_TOP_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(left_label_1, 0, 0);/*淡入*/

	lv_obj_t *left_label_2 = lv_label_create(left_cont);
	lv_label_set_text(left_label_2, "学习时长");
	lv_obj_set_style_text_font(left_label_2, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(left_label_2, LV_ALIGN_BOTTOM_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(left_label_2, 0, 0);/*淡入*/



    /*创建一个右容器*/
    lv_obj_t *right_cont = lv_obj_create(cont_mode);
    lv_obj_add_style(right_cont, &style_cont1, 0);
    lv_obj_set_size(right_cont, LV_BTM_LABEL_W + 25, LV_BTM_LABEL_H);
    lv_obj_align_to(right_cont, mid_cont, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    /*添加左容器的标签*/
    lv_obj_t *right_label_1 = lv_label_create(right_cont);
	lv_label_set_text(right_label_1, "1/3 次");
	lv_obj_set_style_text_font(right_label_1, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(right_label_1, LV_ALIGN_TOP_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(right_label_1, 0, 0);/*淡入*/

	lv_obj_t *right_label_2 = lv_label_create(right_cont);
	lv_label_set_text(right_label_2, "不良/久坐 提醒");
	lv_obj_set_style_text_font(right_label_2, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(right_label_2, LV_ALIGN_BOTTOM_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(right_label_2, 0, 0);/*淡入*/




	/*创建一个容器*/
	lv_obj_t *cont_set = lv_obj_create(g_pt_lv_report->bg_about);
	lv_obj_add_style(cont_set, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_size(cont_set, LV_SET_US_CONT_W, LV_SET_US_CONT_H);
	lv_obj_align_to(cont_set, cont_mode, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	lv_obj_fade_in(cont_set, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_set = lv_label_create(cont_set);
	lv_label_set_text(label_set, "提醒设置");
	lv_obj_set_style_text_font(label_set, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_set, LV_ALIGN_TOP_LEFT, LV_DEVINFO_US_LABEL_SPACE, 0);/*设置位置*/
	lv_obj_fade_in(label_set, 0, 0);/*淡入*/

    /*创建容器模式*/

    lv_obj_t *set_cont1 = lv_obj_create(cont_set);
    lv_obj_add_style(set_cont1, &style_cont1, 0);
    lv_obj_set_size(set_cont1, LV_SET_CONT_W, LV_SET_CONT_H);
    lv_obj_align(set_cont1, LV_ALIGN_TOP_MID, 0, 18);
    /*创建左边标签*/
    lv_obj_t *cont1_left_label = lv_label_create(set_cont1);
    lv_label_set_text(cont1_left_label, "提醒时间间隔");
	lv_obj_set_style_text_font(cont1_left_label, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont1_left_label, LV_ALIGN_LEFT_MID, 5, 0);/*设置位置*/
	lv_obj_fade_in(cont1_left_label, 0, 0);/*淡入*/
    /*创建右边标签*/
    g_pt_lv_report->rem_th_btn = lv_label_create(set_cont1);
    lv_obj_t *cont1_right_label = g_pt_lv_report->rem_th_btn;
    lv_label_set_text(cont1_right_label, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(cont1_right_label, lv_color_hex(GREY_HEX), 0);/*设置颜色*/
    lv_obj_set_style_text_color(cont1_right_label, lv_color_hex(GREEN_HEX), LV_STATE_PRESSED);/*按下时的颜色*/
	lv_obj_set_style_text_font(cont1_right_label, &lv_font_montserrat_22, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont1_right_label, LV_ALIGN_RIGHT_MID, -18, 0);/*设置位置*/
	lv_obj_fade_in(cont1_right_label, 0, 0);/*淡入*/


    lv_obj_t *set_cont2 = lv_obj_create(cont_set);
    lv_obj_add_style(set_cont2, &style_cont1, 0);
    lv_obj_set_size(set_cont2, LV_SET_CONT_W, LV_SET_CONT_H);
    lv_obj_align_to(set_cont2, set_cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    /*创建左边标签*/
    lv_obj_t *cont2_left_label = lv_label_create(set_cont2);
    lv_label_set_text(cont2_left_label, "提醒语音");
	lv_obj_set_style_text_font(cont2_left_label, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont2_left_label, LV_ALIGN_LEFT_MID, 5, 0);/*设置位置*/
	lv_obj_fade_in(cont2_left_label, 0, 0);/*淡入*/
    /*创建右边标签*/
    g_pt_lv_report->rem_vl_btn = lv_label_create(set_cont2);
    lv_obj_t *cont2_right_label = g_pt_lv_report->rem_vl_btn;
    lv_label_set_text(cont2_right_label, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(cont2_right_label, lv_color_hex(GREY_HEX), 0);/*设置颜色*/
    lv_obj_set_style_text_color(cont2_right_label, lv_color_hex(GREEN_HEX), LV_STATE_PRESSED);/*按下时的颜色*/
	lv_obj_set_style_text_font(cont2_right_label, &lv_font_montserrat_22, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont2_right_label, LV_ALIGN_RIGHT_MID, -18, 0);/*设置位置*/
	lv_obj_fade_in(cont2_right_label, 0, 0);/*淡入*/


    lv_obj_t *set_cont3 = lv_obj_create(cont_set);
    lv_obj_add_style(set_cont3, &style_cont1, 0);
    lv_obj_set_size(set_cont3, LV_SET_CONT_W, LV_SET_CONT_H);
    lv_obj_align_to(set_cont3, set_cont2, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    lv_obj_add_flag(set_cont3, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
	lv_obj_add_event_cb(set_cont3, event_handler_setting_long_time, LV_EVENT_CLICKED, NULL);/*添加回调函数*/

    /*创建左边标签*/
    lv_obj_t *cont3_left_label = lv_label_create(set_cont3);
    lv_label_set_text(cont3_left_label, "久坐时长");
	lv_obj_set_style_text_font(cont3_left_label, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont3_left_label, LV_ALIGN_LEFT_MID, 5, 0);/*设置位置*/
	lv_obj_fade_in(cont3_left_label, 0, 0);/*淡入*/
    /*创建右边标签*/
    g_pt_lv_report->lon_th_btn = lv_label_create(set_cont3);
    lv_obj_t *cont3_right_label = g_pt_lv_report->lon_th_btn;
    lv_label_set_text(cont3_right_label, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(cont3_right_label, lv_color_hex(GREY_HEX), 0);/*设置颜色*/
    lv_obj_set_style_text_color(cont3_right_label, lv_color_hex(GREEN_HEX), LV_STATE_PRESSED);/*按下时的颜色*/
	lv_obj_set_style_text_font(cont3_right_label, &lv_font_montserrat_22, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont3_right_label, LV_ALIGN_RIGHT_MID, -18, 0);/*设置位置*/
	lv_obj_fade_in(cont3_right_label, 0, 0);/*淡入*/

    lv_obj_add_flag(g_pt_lv_report->lon_th_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
	lv_obj_add_event_cb(g_pt_lv_report->lon_th_btn, event_handler_setting_long_time, LV_EVENT_CLICKED, NULL);/*添加回调函数*/

	/*创建中间标签*/
    lv_obj_t *cont3_mid_label = lv_label_create(set_cont3);
    lv_label_set_text_fmt(cont3_mid_label, "%d分钟", gdev_param.longTimeMode);/*久坐时长时间*/
	lv_obj_set_style_text_font(cont3_mid_label, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align_to(cont3_mid_label, g_pt_lv_report->lon_th_btn, LV_ALIGN_LEFT_MID, -LV_SET_MID_LABLE_X, 0);/*设置位置*/
	lv_obj_fade_in(cont3_mid_label, 0, 0);/*淡入*/



    lv_obj_t *set_cont4 = lv_obj_create(cont_set);
    lv_obj_add_style(set_cont4, &style_cont1, 0);
    lv_obj_set_size(set_cont4, LV_SET_CONT_W, LV_SET_CONT_H);
    lv_obj_align_to(set_cont4, set_cont3, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_flag(set_cont4, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
	lv_obj_add_event_cb(set_cont4, event_handler_setting_shunt_time, LV_EVENT_CLICKED, NULL);/*添加回调函数*/

    /*创建左边标签*/
    lv_obj_t *cont4_left_label = lv_label_create(set_cont4);
    lv_label_set_text(cont4_left_label, "离座时长");
	lv_obj_set_style_text_font(cont4_left_label, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont4_left_label, LV_ALIGN_LEFT_MID, 5, 0);/*设置位置*/
	lv_obj_fade_in(cont4_left_label, 0, 0);/*淡入*/
    /*创建右边标签*/
    g_pt_lv_report->lev_th_btn = lv_label_create(set_cont4);
    lv_obj_t *cont4_right_label = g_pt_lv_report->lev_th_btn;
    lv_label_set_text(cont4_right_label, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(cont4_right_label, lv_color_hex(GREY_HEX), 0);/*设置颜色*/
    lv_obj_set_style_text_color(cont4_right_label, lv_color_hex(GREEN_HEX), LV_STATE_PRESSED);/*按下时的颜色*/
	lv_obj_set_style_text_font(cont4_right_label, &lv_font_montserrat_22, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont4_right_label, LV_ALIGN_RIGHT_MID, -18, 0);/*设置位置*/
	lv_obj_fade_in(cont4_right_label, 0, 0);/*淡入*/

	lv_obj_add_flag(g_pt_lv_report->lev_th_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
	lv_obj_add_event_cb(g_pt_lv_report->lev_th_btn, event_handler_setting_shunt_time, LV_EVENT_CLICKED, NULL);/*添加回调函数*/


	/*创建中间标签*/
    lv_obj_t *cont4_mid_label = lv_label_create(set_cont4);
    lv_label_set_text_fmt(cont4_mid_label, "%d分钟", gdev_param.shutdownMode);/*离座关机时长时间*/
	lv_obj_set_style_text_font(cont4_mid_label, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align_to(cont4_mid_label, g_pt_lv_report->lev_th_btn, LV_ALIGN_LEFT_MID, -LV_SET_MID_LABLE_X, 0);/*设置位置*/
	lv_obj_fade_in(cont4_mid_label, 0, 0);/*淡入*/


    lv_obj_t *set_cont5 = lv_obj_create(cont_set);
    lv_obj_add_style(set_cont5, &style_cont1, 0);
    lv_obj_set_size(set_cont5, LV_SET_CONT_W, LV_SET_CONT_H);
    lv_obj_align_to(set_cont5, set_cont4, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_flag(set_cont5, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
	lv_obj_add_event_cb(set_cont5, event_handler_setting_timing_time, LV_EVENT_CLICKED, NULL);/*添加回调函数*/

    /*创建左边标签*/
    lv_obj_t *cont5_left_label = lv_label_create(set_cont5);
    lv_label_set_text(cont5_left_label, "定时提醒");
	lv_obj_set_style_text_font(cont5_left_label, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont5_left_label, LV_ALIGN_LEFT_MID, 5, 0);/*设置位置*/
	lv_obj_fade_in(cont5_left_label, 0, 0);/*淡入*/
    /*创建右边标签*/
    g_pt_lv_report->rem_ti_btn = lv_label_create(set_cont5);
    lv_obj_t *cont5_right_label = g_pt_lv_report->rem_ti_btn;
    lv_label_set_text(cont5_right_label, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(cont5_right_label, lv_color_hex(GREY_HEX), 0);/*设置颜色*/
    lv_obj_set_style_text_color(cont5_right_label, lv_color_hex(GREEN_HEX), LV_STATE_PRESSED);/*按下时的颜色*/
	lv_obj_set_style_text_font(cont5_right_label, &lv_font_montserrat_22, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont5_right_label, LV_ALIGN_RIGHT_MID, -18, 0);/*设置位置*/
	lv_obj_fade_in(cont5_right_label, 0, 0);/*淡入*/

	lv_obj_add_flag(g_pt_lv_report->rem_ti_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
	lv_obj_add_event_cb(g_pt_lv_report->rem_ti_btn, event_handler_setting_timing_time, LV_EVENT_CLICKED, NULL);/*添加回调函数*/


    lv_obj_t *set_cont6 = lv_obj_create(cont_set);
    lv_obj_add_style(set_cont6, &style_cont1, 0);
    lv_obj_set_size(set_cont6, LV_SET_CONT_W, LV_SET_CONT_H);
    lv_obj_align_to(set_cont6, set_cont5, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    /*创建左边标签*/
    lv_obj_t *cont6_left_label = lv_label_create(set_cont6);
    lv_label_set_text(cont6_left_label, "倒计时提醒");
	lv_obj_set_style_text_font(cont6_left_label, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont6_left_label, LV_ALIGN_LEFT_MID, 5, 0);/*设置位置*/
	lv_obj_fade_in(cont6_left_label, 0, 0);/*淡入*/
    /*创建右边标签*/
    g_pt_lv_report->rem_cd_btn = lv_label_create(set_cont6);
    lv_obj_t *cont6_right_label = g_pt_lv_report->rem_cd_btn;
    lv_label_set_text(cont6_right_label, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(cont6_right_label, lv_color_hex(GREY_HEX), 0);/*设置颜色*/
    lv_obj_set_style_text_color(cont6_right_label, lv_color_hex(GREEN_HEX), LV_STATE_PRESSED);/*按下时的颜色*/
	lv_obj_set_style_text_font(cont6_right_label, &lv_font_montserrat_22, 0);/*默认状态下的字体样式*/
	lv_obj_align(cont6_right_label, LV_ALIGN_RIGHT_MID, -18, 0);/*设置位置*/
	lv_obj_fade_in(cont6_right_label, 0, 0);/*淡入*/

    /*添加可以点击属性*/
    lv_obj_add_flag(g_pt_lv_report->rem_th_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_flag(g_pt_lv_report->rem_vl_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_flag(g_pt_lv_report->lon_th_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_flag(g_pt_lv_report->lev_th_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_flag(g_pt_lv_report->rem_ti_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_flag(g_pt_lv_report->rem_cd_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/

	/*添加回调函数*/



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
		if(g_pt_lv_report->bg_about != NULL) lv_obj_del(g_pt_lv_report->bg_about);

		vPortFree(g_pt_lv_report);
		//gfind_dev_state = 0;
		lv_main_page_demo_home(100);
	}
}

/* event_handler_setting_long_time
 * 久坐时长事件回调函数
 */
static void event_handler_setting_long_time(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
		if(g_pt_lv_report->bg_about != NULL) lv_obj_del(g_pt_lv_report->bg_about);

		vPortFree(g_pt_lv_report);
		//gfind_dev_state = 0;
		lv_set_long_time();
	}
}

/* event_handler_setting_shunt_time
 * 离座时长事件回调函数
 */
static void event_handler_setting_shunt_time(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
		if(g_pt_lv_report->bg_about != NULL) lv_obj_del(g_pt_lv_report->bg_about);

		vPortFree(g_pt_lv_report);
		//gfind_dev_state = 0;
		lv_set_shunt_time();
	}
}

/* event_handler_setting_shunt_time
 * 离座时长事件回调函数
 */
static void event_handler_setting_timing_time(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
		if(g_pt_lv_report->bg_about != NULL) lv_obj_del(g_pt_lv_report->bg_about);

		vPortFree(g_pt_lv_report);
		//gfind_dev_state = 0;
		lv_timing_list();
	}
}


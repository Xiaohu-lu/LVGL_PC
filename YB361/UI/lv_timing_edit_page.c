/*
 * lv_timing_edit_page.c
 *
 *  Created on: Dec 20, 2023
 *      Author: hxd
 */
#include <stdio.h>
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
#include <time.h>

static PT_lv_timingedit g_pt_lv_timingedit;  // 数据结构体

static time_t cur_time;
static struct tm *cur_info;


static uint8_t timer_index = 0;/*定时列表传过来的定时器索引*/

static lv_calendar_date_t current_days[1];  /*设置日历时使用的变量*/

char *audio_id[] = {"0", "1", "2", "3"};

static char repeat_char[20];/*设置重复类型显示标签时使用的数组*/

static uint8_t week_select = 0;     /*周111 1111的选择*/
static uint8_t month_select = 0;    /*月选择*/

static lv_obj_t *last_audio_btn = NULL;
static uint8_t rep_type = NOT_REP;        /*当前选择的重复类型*/
static uint8_t last_rep_type = LAST_TYPE;  /*上次选择的重复类型,用来在重复类型界面改变圆圈的颜色*/

/*当前选择的情况*/
static int sel_rep_type;              /*当前选择的重复类型*/
static int sel_year, sel_mon, sel_day;/*当前选择结束重复时间*/
static int sel_audio_id;              /*当前选择的提醒语音id*/
static uint8_t sel_week;              /*当前选择的周数*/
static uint8_t sel_mon_day;           /*当前选择的月的那天*/

static void lv_timing_edit_init(void);
static void lv_setting_timer_page(void);

static void modify_the_select_bg(void);

static void event_handler_back_to_home(lv_event_t *e);
static void event_handler_confirm_to_home(lv_event_t *e);
static void event_handler_del_btn(lv_event_t *e);
static void event_handler_rep_btn(lv_event_t *e);
static void event_handler_rep_end_btn(lv_event_t *e);

static void event_handler_aduio_btn(lv_event_t *e);
static void event_handler_bgdel_btn(lv_event_t *e);
static void event_handler_notrep_btn(lv_event_t *e);
static void event_handler_everyday_btn(lv_event_t *e);
static void event_handler_everyweek_btn(lv_event_t *e);
static void event_handler_everymonth_btn(lv_event_t *e);
static void event_handler_everyyear_btn(lv_event_t *e);
static void event_handler_weekset_btn(lv_event_t *e);
static void event_handler_weekbox_btn(lv_event_t *e);
static void event_handler_week_cancel_btn(lv_event_t *e);
static void event_handler_week_confirm_btn(lv_event_t *e);
static void event_handler_month_cancel_btn(lv_event_t *e);
static void event_handler_month_confirm_btn(lv_event_t *e);
static void event_handler_stoptime_cancel_btn(lv_event_t *e);
static void event_handler_stoptime_confirm_btn(lv_event_t *e);
static void event_handler_audio_cancel_btn(lv_event_t *e);
static void event_handler_audio_confirm_btn(lv_event_t *e);
static void event_handler_cancel_btn(lv_event_t *e);
static void event_handler_calendar(lv_event_t *e);
static void event_handler_month_day_roller(lv_event_t *e);

static void event_handler_audio_btn_clicked(lv_event_t *e);
/* analysis_the_repeat
 * 分析定时器重复类型
 */
static void analysis_the_repeat(uint32_t repeat)
{

    if(repeat == 0)
    {
        rep_type = NOT_REP;/*不重复*/
        memset(repeat_char, '\0', sizeof(repeat_char));
        sprintf(repeat_char, "%s", "不重复");
        return;
    }
    else if(repeat == 1)
    {
        rep_type = EVERY_DAY;/*每天*/
        memset(repeat_char, '\0', sizeof(repeat_char));
        sprintf(repeat_char, "%s", "每日");
        return;
    }
    uint8_t len = 0;
    uint32_t dat = repeat;
    while(dat > 0)
    {
        dat /= 10;
        len++;
    }
    if(len > 8)
    {
        LOG_E("the repeat is not valid");
        rep_type = NOT_REP;/*不重复*/
        memset(repeat_char, '\0', sizeof(repeat_char));
        sprintf(repeat_char, "%s", "不重复");
        return ;
    }
    char string[10] = {0};
    itoa(repeat, string, 10);
    if(string[0] == '2')/*每周*/
    {
        /*计算是每周的那几天*/
        if(string[1] == '0')/*周一*/
        {
            week_select &= ~(1 << MONDAY);
        }
        else
        {
            week_select |= (1 << MONDAY);
        }

        if(string[2] == '0')/*周二*/
        {
            week_select &= ~(1 << TUESDAY);
        }
        else
        {
            week_select |= (1 << TUESDAY);
        }

        if(string[3] == '0')/*周三*/
        {
            week_select &= ~(1 << WEDNESDAY);
        }
        else
        {
            week_select |= (1 << WEDNESDAY);
        }

        if(string[4] == '0')/*周四*/
        {
            week_select &= ~(1 << THURSDAT);
        }
        else
        {
            week_select |= (1 << THURSDAT);
        }

        if(string[5] == '0')/*周五*/
        {
            week_select &= ~(1 << FRIDAY);
        }
        else
        {
            week_select |= (1 << FRIDAY);
        }


        if(string[6] == '0')/*周六*/
        {
            week_select &= ~(1 << SATURDAY);
        }
        else
        {
            week_select |= (1 << SATURDAY);
        }

        if(string[6] == '0')/*周日*/
        {
            week_select &= ~(1 << SUNDAY);
        }
        else
        {
            week_select |= (1 << SUNDAY);
        }
        memset(repeat_char, '\0', sizeof(repeat_char));
        sprintf(repeat_char, "%s", "每周");
        rep_type = EVERY_WEEK;/*每周*/
        sel_week = week_select;/*当前定时器是每周的*/
        return ;
    }
    else if(string[0] == '3')/*每月*/
    {
        month_select = repeat % 300;
        memset(repeat_char, '\0', sizeof(repeat_char));
        sprintf(repeat_char, "%s", "每月");
        rep_type = EVERY_MONTH;/*每月*/
        printf("month_select = %d\r\n",month_select);
        sel_mon_day = month_select;
        return ;
    }

    LOG_E("the repeat is not valid");
    rep_type = NOT_REP;/*不重复*/
    memset(repeat_char, '\0', sizeof(repeat_char));
    sprintf(repeat_char, "%s", "不重复");
    return ;

}


/* lv_timing_list
 * 设置定时提醒界面
 */
void lv_timing_edit(void)
{

	g_pt_lv_timingedit = (T_lv_timingedit*)pvPortMalloc(sizeof(T_lv_timingedit));
	g_pt_lv_timingedit->bg_about = lv_obj_create(lv_scr_act());/*创建背景*/

	lv_obj_set_size(g_pt_lv_timingedit->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_timingedit->bg_about, 0, 0);/*设置圆角*/
	lv_obj_align(g_pt_lv_timingedit->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*设置padding*/
	set_user_style(g_pt_lv_timingedit->bg_about);

	lv_obj_set_size(g_pt_lv_timingedit->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_timingedit->bg_about, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_timingedit->bg_about, lv_color_hex(LV_CONT_BG_COLOR), 0);
	lv_timing_edit_init();/*界面初始化*/

	add_title(g_pt_lv_timingedit->bg_about, (const char *)gdev_param.bind_devId);
	add_back(g_pt_lv_timingedit->bg_about, event_handler_back_to_home);
    add_confirm(g_pt_lv_timingedit->bg_about, event_handler_confirm_to_home);
}

/* lv_timing_list_init
 * 时间编辑界面初始化
 */
static void lv_timing_edit_init(void)
{
    char options[512];
    int i;
    int len = 0;
    lv_obj_t *roller;

    sel_week = 0;/*初始化时清0*/
    week_select = 0;
    month_select = 0;
    sel_mon_day = 0;
    timer_index = get_select_timer_index();

    /*获取当前时间*/
    time(&cur_time);
    cur_info = localtime(&cur_time);


    static lv_style_t style_sel;
    lv_style_init(&style_sel);
    lv_style_set_text_font(&style_sel, &lv_font_montserrat_18);
    lv_style_set_border_width(&style_sel, 0);
    lv_style_set_pad_top(&style_sel, 0);
	lv_style_set_pad_bottom(&style_sel, 0);
	lv_style_set_pad_left(&style_sel, 0);
	lv_style_set_pad_right(&style_sel, 0);
	lv_style_set_bg_color(&style_sel, lv_color_hex(LV_CONT_BG_COLOR));


	static lv_style_t style_selected;
	lv_style_init(&style_selected);
    lv_style_set_bg_color(&style_selected, lv_color_hex(LV_CONT_BG_COLOR));
    lv_style_set_text_color(&style_selected, lv_color_hex(BLUE_HEX));


    /*创建小时滚筒*/
    g_pt_lv_timingedit->hour_roller = lv_roller_create(g_pt_lv_timingedit->bg_about);
    roller = g_pt_lv_timingedit->hour_roller;

    memset(options, '\0', sizeof(options));
    for(i = 0; i < 24; i++)
    {
        if(i < 23)
        {
            len += sprintf(options + len, "%02d\n", i);
        }
        else
        {
            len += sprintf(options + len, "%02d", i);
        }

    }
    //printf("options :%s", options);

    lv_obj_add_style(roller, &style_sel, 0);
    lv_obj_add_style(roller, &style_selected, LV_PART_SELECTED);
    lv_obj_set_size(roller, LV_ROLLER_OBJ_W, LV_ROLLER_OBJ_H);
    lv_roller_set_options(g_pt_lv_timingedit->hour_roller, options, LV_ROLLER_MODE_INFINITE);/*设置为无限滚轮*/
    lv_roller_set_visible_row_count(roller, 5);
    lv_obj_align(roller, LV_ALIGN_TOP_LEFT, 10, LV_ROLLER_ALIGN_H);
    int hour_index;
    int min_index;/*需要使用int类型*/

    if(timer_index != LV_ADD_TIMER_INDEX)
    {
        printf("time = %s\n", gdev_param.timer_list.timer[timer_index].time);
        sscanf(gdev_param.timer_list.timer[timer_index].time, "%d:%d", &hour_index, &min_index);
    }
    else/*获取当前时间*/
    {
        hour_index = cur_info->tm_hour;
        min_index = cur_info->tm_min;
    }
    lv_roller_set_selected(roller, hour_index, LV_ANIM_ON);

    static lv_point_t line_points[] = {{0,0}, {300,0}};
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_line_rounded(&style_line, true);


    /*创建分钟滚筒*/
    g_pt_lv_timingedit->minute_roller = lv_roller_create(g_pt_lv_timingedit->bg_about);
    roller = g_pt_lv_timingedit->minute_roller;
    lv_obj_add_style(roller, &style_selected, LV_PART_SELECTED);
    memset(options, '\0', sizeof(options));
    len = 0;
    for(i = 0; i < 60; i++)
    {
        if(i < 59)
        {
            len += sprintf(options + len, "%02d\n", i);
        }
        else
        {
            len += sprintf(options + len, "%02d", i);
        }

    }

    lv_obj_add_style(roller, &style_sel, 0);
    lv_obj_set_size(roller, LV_ROLLER_OBJ_W, LV_ROLLER_OBJ_H);
    lv_roller_set_options(g_pt_lv_timingedit->minute_roller, options, LV_ROLLER_MODE_INFINITE);/*设置为无限滚轮*/
    lv_roller_set_visible_row_count(roller, 5);
    lv_obj_align(roller, LV_ALIGN_TOP_RIGHT, -10, LV_ROLLER_ALIGN_H);
    lv_roller_set_selected(roller, min_index, LV_ANIM_ON);


    /*create a line and apply the new style*/
    lv_obj_t *line1;
    line1 = lv_line_create(g_pt_lv_timingedit->bg_about);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align(line1, LV_ALIGN_TOP_MID, 0, LV_ROLLER_ALIGN_H +80 - 7);

    line1 = lv_line_create(g_pt_lv_timingedit->bg_about);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align(line1, LV_ALIGN_TOP_MID, 0, LV_ROLLER_ALIGN_H +80 +40 - 7);



    /*创建容器*/
    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_0);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, 0);

    static lv_style_t style_press;/*容器样式*/
	lv_style_init(&style_press);
	lv_style_set_bg_opa(&style_press, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_radius(&style_press, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_press, lv_color_hex(GREY_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_press, 0);
	lv_style_set_pad_bottom(&style_press, 0);
	lv_style_set_pad_left(&style_press, 0);
	lv_style_set_pad_right(&style_press, 0);
    lv_style_set_border_width(&style_press, 1);

    static lv_style_t style_big;/*容器样式*/
	lv_style_init(&style_big);
	lv_style_set_bg_opa(&style_big, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_big, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_big, lv_color_hex(WHITE_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_big, 0);
	lv_style_set_pad_bottom(&style_big, 0);
	lv_style_set_pad_left(&style_big, 0);
	lv_style_set_pad_right(&style_big, 0);
    lv_style_set_border_width(&style_big, 0);

    /*创建一个大容器*/
    lv_obj_t *cont_mode = lv_obj_create(g_pt_lv_timingedit->bg_about);
    lv_obj_add_style(cont_mode, &style_big, 0);

    lv_obj_set_size(cont_mode, LV_BIG_CONT_W, LV_BIG_CONT_H);
    lv_obj_align(cont_mode, LV_ALIGN_TOP_MID, 0, LV_CONT_ALIGN_H);



    /*创建容器*/
    g_pt_lv_timingedit->rep_btn = lv_obj_create(cont_mode);
    lv_obj_t *cont_1 = g_pt_lv_timingedit->rep_btn;
    lv_obj_add_style(cont_1, &style_cont, 0);
    lv_obj_add_style(cont_1, &style_press, LV_STATE_PRESSED);
    lv_obj_set_size(cont_1, LV_CONT_W, LV_CONT_H);
    lv_obj_align(cont_1, LV_ALIGN_TOP_MID, 0, 5);
    /*画线*/
    line1 = lv_line_create(g_pt_lv_timingedit->bg_about);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);


    /*添加标签*/
    lv_obj_t *label_left1 = lv_label_create(cont_1);
    lv_obj_set_style_text_font(label_left1, &song_font14, 0);
    lv_label_set_text(label_left1, "重复");
    lv_obj_align(label_left1, LV_ALIGN_LEFT_MID, 5, 0);

    /*创建右边标签*/
    lv_obj_t *label_right1 = lv_label_create(cont_1);
    lv_label_set_text(label_right1, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(label_right1, lv_color_hex(GREY_HEX), 0);
    lv_obj_align(label_right1, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_fade_in(label_right1, 0, 0);
    /*创建中间标签*/
    lv_obj_t *label_mid1 = lv_label_create(cont_1);
    lv_obj_set_style_text_font(label_mid1, &song_font14, 0);

    lv_obj_fade_in(label_mid1, 0, 0);

    /*根据定时器类型设置text内容*/
    if(timer_index != LV_ADD_TIMER_INDEX)/*编辑定时器*/
    {
        analysis_the_repeat(gdev_param.timer_list.timer[timer_index].repeat);
        lv_label_set_text(label_mid1, (const char *)repeat_char);
    }
    else/*创建定时器*/
    {
        rep_type = NOT_REP;/*不重复*/
        memset(repeat_char, '\0', sizeof(repeat_char));
        sprintf(repeat_char, "%s", "不重复");
        lv_label_set_text(label_mid1, (const char *)repeat_char);
    }
    lv_obj_align_to(label_mid1, label_right1, LV_ALIGN_OUT_LEFT_MID, -(5), 0);/*设置位置*/
    g_pt_lv_timingedit->rep_label = label_mid1;/*记录重复类型标签对象*/
    sel_rep_type = rep_type;
    last_rep_type = rep_type;

    /*第二个容器*/
    g_pt_lv_timingedit->rep_end_btn = lv_obj_create(cont_mode);
    lv_obj_t *cont_2 = g_pt_lv_timingedit->rep_end_btn;
    lv_obj_add_style(cont_2, &style_cont, 0);
    lv_obj_add_style(cont_2, &style_press, LV_STATE_PRESSED);
    lv_obj_set_size(cont_2, LV_CONT_W, LV_CONT_H);
    lv_obj_align_to(cont_2, cont_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    /*画线*/
    line1 = lv_line_create(g_pt_lv_timingedit->bg_about);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*添加标签*/
    lv_obj_t *labe1_left2 = lv_label_create(cont_2);
    lv_obj_set_style_text_font(labe1_left2, &song_font14, 0);
    lv_label_set_text(labe1_left2, "结束重复");
    lv_obj_align(labe1_left2, LV_ALIGN_LEFT_MID, 5, 0);

    /*创建右边标签*/
    lv_obj_t *label_right2 = lv_label_create(cont_2);
    lv_label_set_text(label_right2, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(label_right2, lv_color_hex(GREY_HEX), 0);
    lv_obj_align(label_right2, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_fade_in(label_right2, 0, 0);
    /*创建中间标签*/
    lv_obj_t *label_mid2 = lv_label_create(cont_2);
    lv_obj_set_style_text_font(label_mid2, &song_font14, 0);

    lv_obj_align_to(label_mid2, label_right2, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_fade_in(label_mid2, 0, 0);

    //lv_obj_set_style_border_width(label_mid2, 1, 0);
    /*根据定时器类型设置text内容*/
    if(timer_index != LV_ADD_TIMER_INDEX)/*编辑定时器*/
    {
        lv_label_set_text(label_mid2, (const char *)gdev_param.timer_list.timer[timer_index].stop);
        sscanf((const char *)gdev_param.timer_list.timer[timer_index].stop, "%d-%d-%d", &sel_year, &sel_mon, &sel_day);
    }
    else/*创建定时器*/
    {
        lv_label_set_text_fmt(label_mid2, "%d-%02d-%02d", (cur_info->tm_year + 1900), (cur_info->tm_mon + 1), cur_info->tm_mday);
        sel_year = (cur_info->tm_year + 1900);
        sel_mon = (cur_info->tm_mon + 1);
        sel_day = cur_info->tm_mday;
    }
    lv_obj_align_to(label_mid2, label_right2, LV_ALIGN_OUT_LEFT_MID, -(5), 0);/*设置位置*/
    g_pt_lv_timingedit->end_rep_label = label_mid2;/*记录结束重复时间标签对象*/


    g_pt_lv_timingedit->aduio_btn = lv_obj_create(cont_mode);
    lv_obj_t *cont_3 = g_pt_lv_timingedit->aduio_btn;
    lv_obj_add_style(cont_3, &style_cont, 0);
    lv_obj_add_style(cont_3, &style_press, LV_STATE_PRESSED);
    lv_obj_set_size(cont_3, LV_CONT_W, LV_CONT_H);
    lv_obj_align_to(cont_3, cont_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*添加标签*/
    lv_obj_t *label_left3 = lv_label_create(cont_3);
    lv_obj_set_style_text_font(label_left3, &song_font14, 0);
    lv_label_set_text(label_left3, "提醒音");
    lv_obj_align(label_left3, LV_ALIGN_LEFT_MID, 5, 0);

    /*创建右边标签*/
    lv_obj_t *label_right3 = lv_label_create(cont_3);
    lv_label_set_text(label_right3, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_color(label_right3, lv_color_hex(GREY_HEX), 0);
    lv_obj_align(label_right3, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_fade_in(label_right3, 0, 0);
    /*创建中间标签*/
    lv_obj_t *label_mid3 = lv_label_create(cont_3);
    lv_obj_set_style_text_font(label_mid3, &song_font14, 0);


    lv_obj_fade_in(label_mid3, 0, 0);

    //lv_obj_set_style_border_width(label_mid3, 1, 0);
    /*根据定时器类型设置text内容*/
    if(timer_index != LV_ADD_TIMER_INDEX)/*编辑定时器*/
    {
        lv_label_set_text_fmt(label_mid3, "语音%d", gdev_param.timer_list.timer[timer_index].audioId);
        sel_audio_id = gdev_param.timer_list.timer[timer_index].audioId;

    }
    else/*创建定时器*/
    {
        lv_label_set_text_fmt(label_mid3, "语音%s", audio_id[0]);
        sel_audio_id = atoi(audio_id[0]);
    }
    lv_obj_align_to(label_mid3, label_right3, LV_ALIGN_OUT_LEFT_MID, -(5), 0);
    g_pt_lv_timingedit->audio_label = label_mid3;/*记录语音提醒标签对象*/

    /*根据定时器索引判断是否显示删除按钮*/
    /*创建删除按钮*/
    g_pt_lv_timingedit->del_btn = lv_btn_create(g_pt_lv_timingedit->bg_about);
    lv_obj_t *btn = g_pt_lv_timingedit->del_btn;
    lv_obj_set_size(btn, LV_DEL_BTN_W, LV_DEL_BTN_H);
    lv_obj_add_style(btn, &style_big, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(SNOW3_HEX), 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_set_style_radius(btn, 30, 0);

    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -40);

    /*创建标签*/
    lv_obj_t *label_del = lv_label_create(btn);

    lv_obj_set_style_text_font(label_del, &song_font14, 0);
    lv_obj_set_style_text_color(label_del, lv_color_hex(RED_HEX), 0);
    lv_label_set_text(label_del, "删除定时");
    lv_obj_center(label_del);

    if(timer_index != LV_ADD_TIMER_INDEX)/*不是创建,显示按钮*/
    {
        lv_obj_clear_flag(g_pt_lv_timingedit->del_btn, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(g_pt_lv_timingedit->del_btn, LV_OBJ_FLAG_HIDDEN);
    }

    /*删除按钮添加事件*/
    lv_obj_add_event_cb(g_pt_lv_timingedit->del_btn, event_handler_del_btn, LV_EVENT_CLICKED, NULL);

    /*为重复容器添加点击事件*/
    lv_obj_add_flag(g_pt_lv_timingedit->rep_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(g_pt_lv_timingedit->rep_btn, event_handler_rep_btn, LV_EVENT_CLICKED, NULL);

    /*为结束重复容器添加点击事件*/
    lv_obj_add_flag(g_pt_lv_timingedit->rep_end_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(g_pt_lv_timingedit->rep_end_btn, event_handler_rep_end_btn, LV_EVENT_CLICKED, NULL);

    /*为提醒音容器添加点击事件*/
    lv_obj_add_flag(g_pt_lv_timingedit->aduio_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(g_pt_lv_timingedit->aduio_btn, event_handler_aduio_btn, LV_EVENT_CLICKED, NULL);
}

/* lv_create_setrep_page
 * 创建配置重复类型界面
 */
static void lv_create_setrep_page(void)
{
    static lv_style_t style_bg;/*容器样式*/
	lv_style_init(&style_bg);
	lv_style_set_bg_opa(&style_bg, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_radius(&style_bg, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_bg, lv_color_hex(GREY_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_bg, 0);
	lv_style_set_pad_bottom(&style_bg, 0);
	lv_style_set_pad_left(&style_bg, 0);
	lv_style_set_pad_right(&style_bg, 0);
    lv_style_set_border_width(&style_bg, 0);


    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, 0);

    static lv_point_t line_points[] = {{0,0}, {290,0}};
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_line_rounded(&style_line, true);


    /*实现重复类型选择界面*/
    /*创建大容器用来屏蔽后端的点击事件*/
    lv_obj_t *bg_cont = lv_obj_create(g_pt_lv_timingedit->bg_about);
    g_pt_lv_timingedit->rep_bg_cont = bg_cont;
    lv_obj_set_size(bg_cont, LV_HOR_RES, LV_VER_RES);/*设置大小*/
    lv_obj_add_style(bg_cont, &style_bg, 0);
    lv_obj_align(bg_cont, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_flag(bg_cont, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(bg_cont, event_handler_bgdel_btn, LV_EVENT_CLICKED, NULL);


    /*创建一个容器*/
    g_pt_lv_timingedit->rep_cont = lv_obj_create(bg_cont);
    lv_obj_t *cont_rep = g_pt_lv_timingedit->rep_cont;
    lv_obj_set_size(cont_rep, LV_REP_CONT_W, LV_REP_CONT_H);
    lv_obj_add_style(cont_rep, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep, 0, 0);
    lv_obj_align(cont_rep, LV_ALIGN_BOTTOM_MID, 0, -LV_REP_CONT_ALIGN_Y);

    /*添加标签*/
    lv_obj_t *label_1 = lv_label_create(cont_rep);

    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_obj_set_style_text_color(label_1, lv_color_hex(BLACK_HEX), 0);
    lv_label_set_text(label_1, "重复");
    lv_obj_align(label_1, LV_ALIGN_TOP_LEFT, 4, 5);

    /*创建容器  不重复*/
    lv_obj_t *cont_rep_1 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_1, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_1, 0, 0);
    lv_obj_set_style_radius(cont_rep_1, 0, 0);
    lv_obj_set_size(cont_rep_1, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align(cont_rep_1, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_style_bg_color(cont_rep_1, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_1, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_1, event_handler_notrep_btn, LV_EVENT_CLICKED, NULL);
    /*画线*/
    lv_obj_t *line1;
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_rep_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);


    /*创建标签*/
    label_1 = lv_label_create(cont_rep_1);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "不重复");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的圆圈 两个对象,两个圆圈*/
    lv_obj_t *cont_right = lv_obj_create(cont_rep_1);
    g_pt_lv_timingedit->not_rep_circle = cont_right;
    lv_obj_add_style(cont_right, &style_cont, 0);
    lv_obj_set_size(cont_right, LV_REP_BIG_CONT_SIZE, LV_REP_BIG_CONT_SIZE);
    lv_obj_set_style_radius(cont_right, 100, 0);
    lv_obj_set_style_border_width(cont_right, 1, 0);

    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_t *cont_right_l = lv_obj_create(cont_right);
    lv_obj_add_style(cont_right_l, &style_cont, 0);
    lv_obj_set_size(cont_right_l, LV_REP_SML_CONT_SIZE, LV_REP_SML_CONT_SIZE);
    lv_obj_set_style_radius(cont_right_l, 100, 0);
    lv_obj_center(cont_right_l);

    /*创建容器  每天*/
    lv_obj_t *cont_rep_2 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_2, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_2, 0, 0);
    lv_obj_set_style_radius(cont_rep_2, 0, 0);
    lv_obj_set_size(cont_rep_2, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align_to(cont_rep_2, cont_rep_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont_rep_2, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_2, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_2, event_handler_everyday_btn, LV_EVENT_CLICKED, NULL);
    /*画线*/
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_rep_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*创建标签*/
    label_1 = lv_label_create(cont_rep_2);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "每天");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的圆圈 两个对象,两个圆圈*/
    cont_right = lv_obj_create(cont_rep_2);
    g_pt_lv_timingedit->every_day_circle = cont_right;
    lv_obj_add_style(cont_right, &style_cont, 0);
    lv_obj_set_size(cont_right, LV_REP_BIG_CONT_SIZE, LV_REP_BIG_CONT_SIZE);
    lv_obj_set_style_radius(cont_right, 100, 0);
    lv_obj_set_style_border_width(cont_right, 1, 0);
    //lv_obj_set_style_bg_color(cont_right, lv_color_hex(BLUE_HEX), 0);
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    cont_right_l = lv_obj_create(cont_right);
    lv_obj_add_style(cont_right_l, &style_cont, 0);
    lv_obj_set_size(cont_right_l, LV_REP_SML_CONT_SIZE, LV_REP_SML_CONT_SIZE);
    lv_obj_set_style_radius(cont_right_l, 100, 0);
    lv_obj_center(cont_right_l);


    /*创建容器  每周*/
    lv_obj_t *cont_rep_3 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_3, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_3, 0, 0);
    lv_obj_set_style_radius(cont_rep_3, 0, 0);
    lv_obj_set_size(cont_rep_3, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align_to(cont_rep_3, cont_rep_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont_rep_3, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_3, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_3, event_handler_everyweek_btn, LV_EVENT_CLICKED, NULL);
    /*画线*/
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_rep_3, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*创建标签*/
    label_1 = lv_label_create(cont_rep_3);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "每周");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的圆圈 两个对象,两个圆圈*/
    cont_right = lv_obj_create(cont_rep_3);
    g_pt_lv_timingedit->every_week_circle = cont_right;
    lv_obj_add_style(cont_right, &style_cont, 0);
    lv_obj_set_size(cont_right, LV_REP_BIG_CONT_SIZE, LV_REP_BIG_CONT_SIZE);
    lv_obj_set_style_radius(cont_right, 100, 0);
    lv_obj_set_style_border_width(cont_right, 1, 0);
    //lv_obj_set_style_bg_color(cont_right, lv_color_hex(BLUE_HEX), 0);
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    cont_right_l = lv_obj_create(cont_right);
    lv_obj_add_style(cont_right_l, &style_cont, 0);
    lv_obj_set_size(cont_right_l, LV_REP_SML_CONT_SIZE, LV_REP_SML_CONT_SIZE);
    lv_obj_set_style_radius(cont_right_l, 100, 0);
    lv_obj_center(cont_right_l);


    /*创建容器  每月*/
    lv_obj_t *cont_rep_4 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_4, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_4, 0, 0);
    lv_obj_set_style_radius(cont_rep_4, 0, 0);
    lv_obj_set_size(cont_rep_4, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align_to(cont_rep_4, cont_rep_3, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont_rep_4, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_4, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_4, event_handler_everymonth_btn, LV_EVENT_CLICKED, NULL);
    /*画线*/
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_rep_4, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*创建标签*/
    label_1 = lv_label_create(cont_rep_4);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "每月");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的圆圈 两个对象,两个圆圈*/
    cont_right = lv_obj_create(cont_rep_4);
    g_pt_lv_timingedit->every_month_circle = cont_right;
    lv_obj_add_style(cont_right, &style_cont, 0);
    lv_obj_set_size(cont_right, LV_REP_BIG_CONT_SIZE, LV_REP_BIG_CONT_SIZE);
    lv_obj_set_style_radius(cont_right, 100, 0);
    lv_obj_set_style_border_width(cont_right, 1, 0);
    //lv_obj_set_style_bg_color(cont_right, lv_color_hex(BLUE_HEX), 0);
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    cont_right_l = lv_obj_create(cont_right);
    lv_obj_add_style(cont_right_l, &style_cont, 0);
    lv_obj_set_size(cont_right_l, LV_REP_SML_CONT_SIZE, LV_REP_SML_CONT_SIZE);
    lv_obj_set_style_radius(cont_right_l, 100, 0);
    lv_obj_center(cont_right_l);


    /*创建容器  每年*/
    lv_obj_t *cont_rep_5 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_5, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_5, 0, 0);
    lv_obj_set_style_radius(cont_rep_5, 0, 0);
    lv_obj_set_size(cont_rep_5, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align_to(cont_rep_5, cont_rep_4, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont_rep_5, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_5, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_5, event_handler_everyyear_btn, LV_EVENT_CLICKED, NULL);

    /*创建标签*/
    label_1 = lv_label_create(cont_rep_5);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "每年");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的圆圈 两个对象,两个圆圈*/
    cont_right = lv_obj_create(cont_rep_5);
    g_pt_lv_timingedit->every_year_circle = cont_right;
    lv_obj_add_style(cont_right, &style_cont, 0);
    lv_obj_set_size(cont_right, LV_REP_BIG_CONT_SIZE, LV_REP_BIG_CONT_SIZE);
    lv_obj_set_style_radius(cont_right, 100, 0);
    lv_obj_set_style_border_width(cont_right, 1, 0);
    //lv_obj_set_style_bg_color(cont_right, lv_color_hex(BLUE_HEX), 0);
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    cont_right_l = lv_obj_create(cont_right);
    lv_obj_add_style(cont_right_l, &style_cont, 0);
    lv_obj_set_size(cont_right_l, LV_REP_SML_CONT_SIZE, LV_REP_SML_CONT_SIZE);
    lv_obj_set_style_radius(cont_right_l, 100, 0);
    lv_obj_center(cont_right_l);


    /*创建取消按钮*/
    lv_obj_t *cancel_btn = lv_obj_create(cont_rep);
    lv_obj_add_flag(cancel_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_set_size(cancel_btn, LV_REP_CONT_CANCEL_BTN_W, LV_REP_CONT_CANCEL_BTN_H);
    lv_obj_add_style(cancel_btn, &style_cont, 0);
    //lv_obj_set_style_border_width(cancel_btn, 0, 0);
    lv_obj_set_style_border_color(cancel_btn, lv_color_hex(BLUE_HEX), 0);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_MID, 0, -LV_REP_CANCEL_BTN_ALIGN_Y);
    lv_obj_set_style_radius(cancel_btn, 20, 0);
    lv_obj_set_style_bg_color(cancel_btn, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_event_cb(cancel_btn, event_handler_cancel_btn, LV_EVENT_CLICKED, NULL);
    /*创建标签*/
    lv_obj_t *label_cancel = lv_label_create(cancel_btn);

    lv_obj_set_style_text_font(label_cancel, &song_font14, 0);
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(BLUE_HEX), 0);
    lv_label_set_text(label_cancel, "取消");
    lv_obj_center(label_cancel);

    rep_type = sel_rep_type;
    last_rep_type = rep_type;
    /*更新圆圈位置*/
    modify_the_select_bg();

}

/* lv_create_set_week_page
 * 创建每周配置页面
 */
static void lv_create_set_week_page(void)
{

    static lv_style_t style_bg;/*容器样式*/
	lv_style_init(&style_bg);
	lv_style_set_bg_opa(&style_bg, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_bg, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_bg, lv_color_hex(GREY_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_bg, 0);
	lv_style_set_pad_bottom(&style_bg, 0);
	lv_style_set_pad_left(&style_bg, 0);
	lv_style_set_pad_right(&style_bg, 0);
    lv_style_set_border_width(&style_bg, 0);

    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, 0);

    static lv_point_t line_points[] = {{0,0}, {290,0}};
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_line_rounded(&style_line, true);


    /*创建大容器用来屏蔽后端的点击事件*/
    lv_obj_t *bg_cont = lv_obj_create(g_pt_lv_timingedit->bg_about);
    g_pt_lv_timingedit->week_bg_cont = bg_cont;
    lv_obj_set_size(bg_cont, LV_HOR_RES, LV_VER_RES);/*设置大小*/
    lv_obj_add_style(bg_cont, &style_bg, 0);
    lv_obj_align(bg_cont, LV_ALIGN_TOP_LEFT, 0, 0);
    //lv_obj_add_flag(bg_cont, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(bg_cont, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(bg_cont, event_handler_bgdel_btn, LV_EVENT_CLICKED, NULL);

    /*week每周界面*/
    /*创建一个容器*/
    lv_obj_t *cont_rep = lv_obj_create(bg_cont);
    lv_obj_set_size(cont_rep, LV_REP_CONT_W, LV_REP_CONT_H + 60);
    lv_obj_add_style(cont_rep, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep, 0, 0);
    lv_obj_align(cont_rep, LV_ALIGN_BOTTOM_MID, 0, -LV_REP_CONT_ALIGN_Y);
    //lv_obj_add_flag(g_pt_lv_timingedit->week_cont, LV_OBJ_FLAG_HIDDEN);

    /*添加标签*/
    lv_obj_t *label_1 = lv_label_create(cont_rep);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_obj_set_style_text_color(label_1, lv_color_hex(BLACK_HEX), 0);
    lv_label_set_text(label_1, "重复");
    lv_obj_align(label_1, LV_ALIGN_TOP_LEFT, 4, 5);

    /*创建容器  周日*/
    lv_obj_t *cont_rep_1 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_1, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_1, 0, 0);
    lv_obj_set_style_radius(cont_rep_1, 0, 0);
    lv_obj_set_size(cont_rep_1, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align(cont_rep_1, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_style_bg_color(cont_rep_1, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_1, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_1, event_handler_weekset_btn, LV_EVENT_CLICKED, (void *)SUNDAY);
    /*画线*/
    lv_obj_t *line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_rep_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);


    /*创建标签*/
    label_1 = lv_label_create(cont_rep_1);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "周日");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的复选框*/
    lv_obj_t *cont_right = lv_checkbox_create(cont_rep_1);
    lv_obj_set_style_border_color(cont_right, lv_color_hex(GREY_HEX), LV_PART_INDICATOR);
    lv_checkbox_set_text(cont_right, "");
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_add_event_cb(cont_right, event_handler_weekbox_btn, LV_EVENT_VALUE_CHANGED, (void *)SUNDAY);
    /*设置复选框的初始状态 周日*/
    if(sel_week & (0x1 << SUNDAY))
    {
        lv_obj_add_state(cont_right, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(cont_right, LV_STATE_CHECKED);
    }



    /*创建容器  周一*/
    lv_obj_t *cont_rep_2 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_2, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_2, 0, 0);
    lv_obj_set_style_radius(cont_rep_2, 0, 0);
    lv_obj_set_size(cont_rep_2, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align_to(cont_rep_2, cont_rep_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont_rep_2, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_2, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_2, event_handler_weekset_btn, LV_EVENT_CLICKED, (void *)MONDAY);
    /*画线*/
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_rep_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*创建标签*/
    label_1 = lv_label_create(cont_rep_2);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "周一");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的复选框*/
    cont_right = lv_checkbox_create(cont_rep_2);
    lv_obj_set_style_border_color(cont_right, lv_color_hex(GREY_HEX), LV_PART_INDICATOR);
    lv_checkbox_set_text(cont_right, "");
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_add_event_cb(cont_right, event_handler_weekbox_btn, LV_EVENT_VALUE_CHANGED, (void *)MONDAY);
    /*设置复选框的初始状态 周一*/
    if(sel_week & (0x1 << MONDAY))
    {
        lv_obj_add_state(cont_right, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(cont_right, LV_STATE_CHECKED);
    }


    /*创建容器  周二*/
    lv_obj_t *cont_rep_3 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_3, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_3, 0, 0);
    lv_obj_set_style_radius(cont_rep_3, 0, 0);
    lv_obj_set_size(cont_rep_3, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align_to(cont_rep_3, cont_rep_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont_rep_3, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_3, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_3, event_handler_weekset_btn, LV_EVENT_CLICKED, (void *)TUESDAY);
    /*画线*/
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_rep_3, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*创建标签*/
    label_1 = lv_label_create(cont_rep_3);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "周二");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的复选框*/
    cont_right = lv_checkbox_create(cont_rep_3);
    lv_obj_set_style_border_color(cont_right, lv_color_hex(GREY_HEX), LV_PART_INDICATOR);
    lv_checkbox_set_text(cont_right, "");
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_add_event_cb(cont_right, event_handler_weekbox_btn, LV_EVENT_VALUE_CHANGED, (void *)TUESDAY);
    /*设置复选框的初始状态 周二*/
    if(sel_week & (0x1 << TUESDAY))
    {
        lv_obj_add_state(cont_right, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(cont_right, LV_STATE_CHECKED);
    }



    /*创建容器  周三*/
    lv_obj_t *cont_rep_4 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_4, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_4, 0, 0);
    lv_obj_set_style_radius(cont_rep_4, 0, 0);
    lv_obj_set_size(cont_rep_4, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align_to(cont_rep_4, cont_rep_3, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont_rep_4, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_4, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_4, event_handler_weekset_btn, LV_EVENT_CLICKED, (void *)WEDNESDAY);
    /*画线*/
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_rep_4, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*创建标签*/
    label_1 = lv_label_create(cont_rep_4);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "周三");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的复选框*/
    cont_right = lv_checkbox_create(cont_rep_4);
    lv_obj_set_style_border_color(cont_right, lv_color_hex(GREY_HEX), LV_PART_INDICATOR);
    lv_checkbox_set_text(cont_right, "");
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_add_event_cb(cont_right, event_handler_weekbox_btn, LV_EVENT_VALUE_CHANGED, (void *)WEDNESDAY);
    /*设置复选框的初始状态 周三*/
    if(sel_week & (0x1 << WEDNESDAY))
    {
        lv_obj_add_state(cont_right, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(cont_right, LV_STATE_CHECKED);
    }



    /*创建容器  周四*/
    lv_obj_t *cont_rep_5 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_5, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_5, 0, 0);
    lv_obj_set_style_radius(cont_rep_5, 0, 0);
    lv_obj_set_size(cont_rep_5, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align_to(cont_rep_5, cont_rep_4, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont_rep_5, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_5, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_5, event_handler_weekset_btn, LV_EVENT_CLICKED, (void *)THURSDAT);
    /*画线*/
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_rep_5, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*创建标签*/
    label_1 = lv_label_create(cont_rep_5);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "周四");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的复选框*/
    cont_right = lv_checkbox_create(cont_rep_5);
    lv_obj_set_style_border_color(cont_right, lv_color_hex(GREY_HEX), LV_PART_INDICATOR);
    lv_checkbox_set_text(cont_right, "");
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_add_event_cb(cont_right, event_handler_weekbox_btn, LV_EVENT_VALUE_CHANGED, (void *)THURSDAT);
    /*设置复选框的初始状态 周四*/
    if(sel_week & (0x1 << THURSDAT))
    {
        lv_obj_add_state(cont_right, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(cont_right, LV_STATE_CHECKED);
    }



    /*创建容器  周五*/
    lv_obj_t *cont_rep_6 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_6, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_6, 0, 0);
    lv_obj_set_style_radius(cont_rep_6, 0, 0);
    lv_obj_set_size(cont_rep_6, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align_to(cont_rep_6, cont_rep_5, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont_rep_6, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_6, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_6, event_handler_weekset_btn, LV_EVENT_CLICKED, (void *)FRIDAY);
    /*画线*/
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align_to(line1, cont_rep_6, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*创建标签*/
    label_1 = lv_label_create(cont_rep_6);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "周五");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的复选框*/
    cont_right = lv_checkbox_create(cont_rep_6);
    lv_obj_set_style_border_color(cont_right, lv_color_hex(GREY_HEX), LV_PART_INDICATOR);
    lv_checkbox_set_text(cont_right, "");
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_add_event_cb(cont_right, event_handler_weekbox_btn, LV_EVENT_VALUE_CHANGED, (void *)FRIDAY);
    /*设置复选框的初始状态 周五*/
    if(sel_week & (0x1 << FRIDAY))
    {
        lv_obj_add_state(cont_right, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(cont_right, LV_STATE_CHECKED);
    }


    /*创建容器  周六*/
    lv_obj_t *cont_rep_7 = lv_obj_create(cont_rep);
    lv_obj_add_style(cont_rep_7, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep_7, 0, 0);
    lv_obj_set_style_radius(cont_rep_7, 0, 0);
    lv_obj_set_size(cont_rep_7, LV_REP_CONT_L_W, LV_REP_CONT_L_H);
    lv_obj_align_to(cont_rep_7, cont_rep_6, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(cont_rep_7, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_flag(cont_rep_7, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(cont_rep_7, event_handler_weekset_btn, LV_EVENT_CLICKED, (void *)SATURDAY);

    /*创建标签*/
    label_1 = lv_label_create(cont_rep_7);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_label_set_text(label_1, "周六");
    lv_obj_align(label_1, LV_ALIGN_LEFT_MID, LV_CONT_LABEL_ALIGN_X, 0);
    /*创建后面的复选框*/
    cont_right = lv_checkbox_create(cont_rep_7);
    lv_obj_set_style_border_color(cont_right, lv_color_hex(GREY_HEX), LV_PART_INDICATOR);
    lv_checkbox_set_text(cont_right, "");
    lv_obj_align(cont_right, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_add_event_cb(cont_right, event_handler_weekbox_btn, LV_EVENT_VALUE_CHANGED, (void *)SATURDAY);
    /*设置复选框的初始状态 周六*/
    if(sel_week & (0x1 << SATURDAY))
    {
        lv_obj_add_state(cont_right, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(cont_right, LV_STATE_CHECKED);
    }



    /*添加确认和取消按钮*/
    /*创建取消按钮*/
    lv_obj_t *cancel_btn = lv_obj_create(cont_rep);
    lv_obj_add_flag(cancel_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_set_size(cancel_btn, LV_WEEK_CANCEL_BTN_W, LV_WEEK_CANCEL_BTN_H);
    lv_obj_add_style(cancel_btn, &style_cont, 0);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_LEFT, 10, -LV_REP_CANCEL_BTN_ALIGN_Y);
    lv_obj_set_style_radius(cancel_btn, 20, 0);
    lv_obj_set_style_bg_color(cancel_btn, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_event_cb(cancel_btn, event_handler_week_cancel_btn, LV_EVENT_CLICKED, NULL);
    /*创建标签*/
    lv_obj_t *label_cancel = lv_label_create(cancel_btn);
    lv_obj_set_style_text_font(label_cancel, &song_font14, 0);
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(BLUE_HEX), 0);
    lv_label_set_text(label_cancel, "取消");
    lv_obj_center(label_cancel);

    cancel_btn = lv_obj_create(cont_rep);
    lv_obj_add_flag(cancel_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_set_size(cancel_btn, LV_WEEK_CANCEL_BTN_W, LV_WEEK_CANCEL_BTN_H);
    lv_obj_add_style(cancel_btn, &style_cont, 0);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -LV_REP_CANCEL_BTN_ALIGN_Y);
    lv_obj_set_style_radius(cancel_btn, 20, 0);
    lv_obj_set_style_bg_color(cancel_btn, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_event_cb(cancel_btn, event_handler_week_confirm_btn, LV_EVENT_CLICKED, NULL);
    /*创建标签*/
    label_cancel = lv_label_create(cancel_btn);
    lv_obj_set_style_text_font(label_cancel, &song_font14, 0);
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(BLUE_HEX), 0);
    lv_label_set_text(label_cancel, "确认");
    lv_obj_center(label_cancel);

    static lv_point_t line_points_h[] = {{0,0}, {0,20}};
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points_h, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align(line1, LV_ALIGN_BOTTOM_MID, 0, -(LV_REP_CANCEL_BTN_ALIGN_Y + 5));




}


/* lv_create_set_month_page
 * 创建每月配置页面
 */
static void lv_create_set_month_page(void)
{
    char options[128];
    int i;
    int len = 0;

    static lv_style_t style_bg;/*容器样式*/
	lv_style_init(&style_bg);
	lv_style_set_bg_opa(&style_bg, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_bg, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_bg, lv_color_hex(GREY_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_bg, 0);
	lv_style_set_pad_bottom(&style_bg, 0);
	lv_style_set_pad_left(&style_bg, 0);
	lv_style_set_pad_right(&style_bg, 0);
    lv_style_set_border_width(&style_bg, 0);

    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, 0);

    static lv_point_t line_points[] = {{0,0}, {290,0}};
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_line_rounded(&style_line, true);

    /*创建大容器用来屏蔽后端的点击事件*/
    lv_obj_t *bg_cont = lv_obj_create(g_pt_lv_timingedit->bg_about);
    g_pt_lv_timingedit->month_bg_cont = bg_cont;
    lv_obj_set_size(bg_cont, LV_HOR_RES, LV_VER_RES);/*设置大小*/
    lv_obj_add_style(bg_cont, &style_bg, 0);
    lv_obj_align(bg_cont, LV_ALIGN_TOP_LEFT, 0, 0);
    //lv_obj_add_flag(bg_cont, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(bg_cont, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(bg_cont, event_handler_bgdel_btn, LV_EVENT_CLICKED, NULL);


    /*week每周界面*/
    /*创建一个容器*/

    lv_obj_t *cont_rep = lv_obj_create(bg_cont);
    lv_obj_set_size(cont_rep, LV_REP_CONT_W, LV_REP_CONT_H + 40);
    lv_obj_add_style(cont_rep, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep, 0, 0);
    lv_obj_align(cont_rep, LV_ALIGN_BOTTOM_MID, 0, -LV_REP_CONT_ALIGN_Y);
    //lv_obj_add_flag(g_pt_lv_timingedit->week_cont, LV_OBJ_FLAG_HIDDEN);

    /*添加标签*/
    lv_obj_t *label_1 = lv_label_create(cont_rep);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_obj_set_style_text_color(label_1, lv_color_hex(BLACK_HEX), 0);
    lv_label_set_text(label_1, "重复");
    lv_obj_align(label_1, LV_ALIGN_TOP_LEFT, 4, 5);


    /*创建一个滚筒*/
    static lv_style_t style_sel;
    lv_style_init(&style_sel);
    lv_style_set_text_font(&style_sel, &lv_font_montserrat_18);
    lv_style_set_border_width(&style_sel, 0);
    lv_style_set_pad_top(&style_sel, 0);
	lv_style_set_pad_bottom(&style_sel, 0);
	lv_style_set_pad_left(&style_sel, 0);
	lv_style_set_pad_right(&style_sel, 0);
	lv_style_set_bg_color(&style_sel, lv_color_hex(WHITE_HEX));


	static lv_style_t style_selected;
	lv_style_init(&style_selected);
    lv_style_set_bg_color(&style_selected, lv_color_hex(WHITE_HEX));
    lv_style_set_text_color(&style_selected, lv_color_hex(BLUE_HEX));


    /*创建day滚筒*/
    g_pt_lv_timingedit->day_roller = lv_roller_create(cont_rep);
    lv_obj_t *roller = g_pt_lv_timingedit->day_roller;

    memset(options, '\0', sizeof(options));
    for(i = 1; i < 32; i++)
    {
        if(i < 31)
        {
            len += sprintf(options + len, "%02d\n", i);
        }
        else
        {
            len += sprintf(options + len, "%02d", i);
        }

    }

    lv_obj_add_style(roller, &style_sel, 0);
    lv_obj_add_style(roller, &style_selected, LV_PART_SELECTED);
    lv_obj_set_size(roller, LV_ROLLER_OBJ_W, LV_ROLLER_OBJ_H);
    lv_roller_set_options(g_pt_lv_timingedit->day_roller, options, LV_ROLLER_MODE_INFINITE);/*设置为无限滚轮*/
    lv_roller_set_visible_row_count(roller, 5);
    lv_obj_align(roller, LV_ALIGN_TOP_MID, 0, LV_ROLLER_ALIGN_H);
    lv_obj_add_event_cb(g_pt_lv_timingedit->day_roller, event_handler_month_day_roller, LV_EVENT_VALUE_CHANGED, NULL);
    lv_roller_set_selected(roller, month_select - 1, LV_ANIM_ON);

    lv_obj_t *line1;
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align(line1, LV_ALIGN_TOP_MID, 0,  120 - 7);

    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align(line1, LV_ALIGN_TOP_MID, 0,  120 + 40 - 7);


    /*添加确认和取消按钮*/
    /*创建取消按钮*/
    lv_obj_t *cancel_btn = lv_obj_create(cont_rep);
    lv_obj_add_flag(cancel_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_set_size(cancel_btn, LV_WEEK_CANCEL_BTN_W, LV_WEEK_CANCEL_BTN_H);
    lv_obj_add_style(cancel_btn, &style_cont, 0);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_LEFT, 10, -LV_REP_CANCEL_BTN_ALIGN_Y);
    lv_obj_set_style_radius(cancel_btn, 20, 0);
    lv_obj_set_style_bg_color(cancel_btn, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_event_cb(cancel_btn, event_handler_month_cancel_btn, LV_EVENT_CLICKED, NULL);
    /*创建标签*/
    lv_obj_t *label_cancel = lv_label_create(cancel_btn);
    lv_obj_set_style_text_font(label_cancel, &song_font14, 0);
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(BLUE_HEX), 0);
    lv_label_set_text(label_cancel, "取消");
    lv_obj_center(label_cancel);

    cancel_btn = lv_obj_create(cont_rep);
    lv_obj_add_flag(cancel_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_set_size(cancel_btn, LV_WEEK_CANCEL_BTN_W, LV_WEEK_CANCEL_BTN_H);
    lv_obj_add_style(cancel_btn, &style_cont, 0);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -LV_REP_CANCEL_BTN_ALIGN_Y);
    lv_obj_set_style_radius(cancel_btn, 20, 0);
    lv_obj_set_style_bg_color(cancel_btn, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_event_cb(cancel_btn, event_handler_month_confirm_btn, LV_EVENT_CLICKED, NULL);
    /*创建标签*/
    label_cancel = lv_label_create(cancel_btn);
    lv_obj_set_style_text_font(label_cancel, &song_font14, 0);
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(BLUE_HEX), 0);
    lv_label_set_text(label_cancel, "确认");
    lv_obj_center(label_cancel);

    static lv_point_t line_points_h[] = {{0,0}, {0,20}};
    line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points_h, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align(line1, LV_ALIGN_BOTTOM_MID, 0, -(LV_REP_CANCEL_BTN_ALIGN_Y + 5));

}

/* lv_create_set_stoptime_page
 * 设置结束时间界面
 */
static void lv_create_set_stoptime_page(void)
{
    static lv_style_t style_bg;/*容器样式*/
	lv_style_init(&style_bg);
	lv_style_set_bg_opa(&style_bg, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_bg, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_bg, lv_color_hex(GREY_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_bg, 0);
	lv_style_set_pad_bottom(&style_bg, 0);
	lv_style_set_pad_left(&style_bg, 0);
	lv_style_set_pad_right(&style_bg, 0);
    lv_style_set_border_width(&style_bg, 0);

    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, 0);

    static lv_point_t line_points[] = {{0,0}, {290,0}};
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_line_rounded(&style_line, true);

    /*创建大容器用来屏蔽后端的点击事件*/
    lv_obj_t *bg_cont = lv_obj_create(g_pt_lv_timingedit->bg_about);
    g_pt_lv_timingedit->stoptime_bg_cont = bg_cont;
    lv_obj_set_size(bg_cont, LV_HOR_RES, LV_VER_RES);/*设置大小*/
    lv_obj_add_style(bg_cont, &style_bg, 0);
    lv_obj_align(bg_cont, LV_ALIGN_TOP_LEFT, 0, 0);
    //lv_obj_add_flag(bg_cont, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(bg_cont, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(bg_cont, event_handler_bgdel_btn, LV_EVENT_CLICKED, NULL);


    /*week每周界面*/
    /*创建一个容器*/

    lv_obj_t *cont_rep = lv_obj_create(bg_cont);
    lv_obj_set_size(cont_rep, LV_REP_CONT_W, LV_REP_CONT_H + 40);
    lv_obj_add_style(cont_rep, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep, 0, 0);
    lv_obj_align(cont_rep, LV_ALIGN_BOTTOM_MID, 0, -LV_REP_CONT_ALIGN_Y);
    //lv_obj_add_flag(g_pt_lv_timingedit->week_cont, LV_OBJ_FLAG_HIDDEN);

    /*添加标签*/
    lv_obj_t *label_1 = lv_label_create(cont_rep);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_obj_set_style_text_color(label_1, lv_color_hex(BLACK_HEX), 0);
    lv_label_set_text(label_1, "结束时间");
    lv_obj_align(label_1, LV_ALIGN_TOP_LEFT, 4, 5);

    /*创建一个日历*/
    lv_obj_t *calendar = lv_calendar_create(cont_rep);
    lv_obj_set_size(calendar, LV_SET_CALENDAR_W, LV_SET_CALENDAR_H);
    lv_obj_align(calendar, LV_ALIGN_TOP_MID, 0, 30);


    /*标记今天的日期*/
    lv_calendar_set_today_date(calendar, (cur_info->tm_year + 1900), (cur_info->tm_mon + 1), cur_info->tm_yday);
    /*上面显示切换按钮*/
    lv_calendar_header_arrow_create(calendar);
    lv_calendar_set_showed_date(calendar, sel_year, sel_mon);/*显示结束时间的月*/
    current_days[0].day = sel_day;
    current_days[0].month = sel_mon;
    current_days[0].year = sel_year;
    /*把结束时间高亮显示*/
    lv_calendar_set_highlighted_dates(calendar, current_days, 1);
    lv_obj_add_event_cb(calendar, event_handler_calendar, LV_EVENT_ALL, NULL);

    /*添加确认和取消按钮*/
    /*创建取消按钮*/
    lv_obj_t *cancel_btn = lv_obj_create(cont_rep);
    lv_obj_add_flag(cancel_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_set_size(cancel_btn, LV_WEEK_CANCEL_BTN_W, LV_WEEK_CANCEL_BTN_H);
    lv_obj_add_style(cancel_btn, &style_cont, 0);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_LEFT, 10, -LV_REP_CANCEL_BTN_ALIGN_Y);
    lv_obj_set_style_radius(cancel_btn, 20, 0);
    lv_obj_set_style_bg_color(cancel_btn, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_event_cb(cancel_btn, event_handler_stoptime_cancel_btn, LV_EVENT_CLICKED, NULL);
    /*创建标签*/
    lv_obj_t *label_cancel = lv_label_create(cancel_btn);
    lv_obj_set_style_text_font(label_cancel, &song_font14, 0);
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(BLUE_HEX), 0);
    lv_label_set_text(label_cancel, "取消");
    lv_obj_center(label_cancel);

    cancel_btn = lv_obj_create(cont_rep);
    lv_obj_add_flag(cancel_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_set_size(cancel_btn, LV_WEEK_CANCEL_BTN_W, LV_WEEK_CANCEL_BTN_H);
    lv_obj_add_style(cancel_btn, &style_cont, 0);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -LV_REP_CANCEL_BTN_ALIGN_Y);
    lv_obj_set_style_radius(cancel_btn, 20, 0);
    lv_obj_set_style_bg_color(cancel_btn, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_event_cb(cancel_btn, event_handler_stoptime_confirm_btn, LV_EVENT_CLICKED, NULL);
    /*创建标签*/
    label_cancel = lv_label_create(cancel_btn);
    lv_obj_set_style_text_font(label_cancel, &song_font14, 0);
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(BLUE_HEX), 0);
    lv_label_set_text(label_cancel, "确认");
    lv_obj_center(label_cancel);

    static lv_point_t line_points_h[] = {{0,0}, {0,20}};
    lv_obj_t *line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points_h, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align(line1, LV_ALIGN_BOTTOM_MID, 0, -(LV_REP_CANCEL_BTN_ALIGN_Y + 5));

}

/* lv_create_set_audioid_page
 * 设置提醒语音界面
 */
static void lv_create_set_audio_page(void)
{
    static lv_style_t style_bg;/*容器样式*/
	lv_style_init(&style_bg);
	lv_style_set_bg_opa(&style_bg, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_bg, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_bg, lv_color_hex(GREY_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_bg, 0);
	lv_style_set_pad_bottom(&style_bg, 0);
	lv_style_set_pad_left(&style_bg, 0);
	lv_style_set_pad_right(&style_bg, 0);
    lv_style_set_border_width(&style_bg, 0);

    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, 0);

    static lv_point_t line_points[] = {{0,0}, {290,0}};
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_line_rounded(&style_line, true);

    /*创建大容器用来屏蔽后端的点击事件*/
    lv_obj_t *bg_cont = lv_obj_create(g_pt_lv_timingedit->bg_about);
    g_pt_lv_timingedit->audio_bg_cont = bg_cont;
    lv_obj_set_size(bg_cont, LV_HOR_RES, LV_VER_RES);/*设置大小*/
    lv_obj_add_style(bg_cont, &style_bg, 0);
    lv_obj_align(bg_cont, LV_ALIGN_TOP_LEFT, 0, 0);
    //lv_obj_add_flag(bg_cont, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(bg_cont, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_add_event_cb(bg_cont, event_handler_bgdel_btn, LV_EVENT_CLICKED, NULL);


    /*week每周界面*/
    /*创建一个容器*/

    lv_obj_t *cont_rep = lv_obj_create(bg_cont);
    lv_obj_set_size(cont_rep, LV_REP_CONT_W, LV_REP_CONT_H + 40);
    lv_obj_add_style(cont_rep, &style_cont, 0);
    lv_obj_set_style_border_width(cont_rep, 0, 0);
    lv_obj_align(cont_rep, LV_ALIGN_BOTTOM_MID, 0, -LV_REP_CONT_ALIGN_Y);
    //lv_obj_add_flag(g_pt_lv_timingedit->week_cont, LV_OBJ_FLAG_HIDDEN);

    /*添加标签*/
    lv_obj_t *label_1 = lv_label_create(cont_rep);
    lv_obj_set_style_text_font(label_1, &song_font14, 0);
    lv_obj_set_style_text_color(label_1, lv_color_hex(BLACK_HEX), 0);
    lv_label_set_text(label_1, "提醒语音");
    lv_obj_align(label_1, LV_ALIGN_TOP_LEFT, 4, 5);

    /*创建一个列表*/
	lv_obj_t *list = lv_list_create(cont_rep);
	lv_obj_set_style_bg_color(list, lv_color_hex(WHITE_HEX), 0);
	lv_obj_set_size(list, LV_SET_LIST_W, LV_SET_LIST_H);
	lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 40);
	lv_obj_t *list_btn;
	uint16_t i;

	for(i = 0; i < 4; i++)
    {

		list_btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, (const char*)audio_id[i]);
		lv_obj_set_style_bg_color(list_btn, lv_color_hex(WHITE_HEX), 0);
		//lv_obj_set_style_bg_color()
		lv_obj_add_event_cb(list_btn, event_handler_audio_btn_clicked, LV_EVENT_CLICKED, atoi(audio_id[i]));

    }


    /*添加确认和取消按钮*/
    /*创建取消按钮*/
    lv_obj_t *cancel_btn = lv_obj_create(cont_rep);
    lv_obj_add_flag(cancel_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_set_size(cancel_btn, LV_WEEK_CANCEL_BTN_W, LV_WEEK_CANCEL_BTN_H);
    lv_obj_add_style(cancel_btn, &style_cont, 0);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_LEFT, 10, -LV_REP_CANCEL_BTN_ALIGN_Y);
    lv_obj_set_style_radius(cancel_btn, 20, 0);
    lv_obj_set_style_bg_color(cancel_btn, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_event_cb(cancel_btn, event_handler_audio_cancel_btn, LV_EVENT_CLICKED, NULL);
    /*创建标签*/
    lv_obj_t *label_cancel = lv_label_create(cancel_btn);
    lv_obj_set_style_text_font(label_cancel, &song_font14, 0);
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(BLUE_HEX), 0);
    lv_label_set_text(label_cancel, "取消");
    lv_obj_center(label_cancel);

    cancel_btn = lv_obj_create(cont_rep);
    lv_obj_add_flag(cancel_btn, LV_OBJ_FLAG_CLICKABLE);/*允许点击*/
    lv_obj_set_size(cancel_btn, LV_WEEK_CANCEL_BTN_W, LV_WEEK_CANCEL_BTN_H);
    lv_obj_add_style(cancel_btn, &style_cont, 0);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -LV_REP_CANCEL_BTN_ALIGN_Y);
    lv_obj_set_style_radius(cancel_btn, 20, 0);
    lv_obj_set_style_bg_color(cancel_btn, lv_color_hex(GREY_HEX), LV_STATE_PRESSED);
    lv_obj_add_event_cb(cancel_btn, event_handler_audio_confirm_btn, LV_EVENT_CLICKED, NULL);
    /*创建标签*/
    label_cancel = lv_label_create(cancel_btn);
    lv_obj_set_style_text_font(label_cancel, &song_font14, 0);
    lv_obj_set_style_text_color(label_cancel, lv_color_hex(BLUE_HEX), 0);
    lv_label_set_text(label_cancel, "确认");
    lv_obj_center(label_cancel);

    static lv_point_t line_points_h[] = {{0,0}, {0,20}};
    lv_obj_t *line1 = lv_line_create(cont_rep);
    lv_line_set_points(line1, line_points_h, 2);
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_align(line1, LV_ALIGN_BOTTOM_MID, 0, -(LV_REP_CANCEL_BTN_ALIGN_Y + 5));

}

/* modify_the_select_bg
 * 根据点击的容器切换圆圈的颜色
 */
static void modify_the_select_bg(void)
{

    switch(last_rep_type)
    {
    case NOT_REP:
        lv_obj_set_style_border_width(g_pt_lv_timingedit->not_rep_circle, 1, 0);
        lv_obj_set_style_bg_color(g_pt_lv_timingedit->not_rep_circle, lv_color_hex(WHITE_HEX), 0);
        break;
    case EVERY_DAY:
        lv_obj_set_style_border_width(g_pt_lv_timingedit->every_day_circle, 1, 0);
        lv_obj_set_style_bg_color(g_pt_lv_timingedit->every_day_circle, lv_color_hex(WHITE_HEX), 0);
        break;
    case EVERY_WEEK:
        lv_obj_set_style_border_width(g_pt_lv_timingedit->every_week_circle, 1, 0);
        lv_obj_set_style_bg_color(g_pt_lv_timingedit->every_week_circle, lv_color_hex(WHITE_HEX), 0);
        break;
    case EVERY_MONTH:
        lv_obj_set_style_border_width(g_pt_lv_timingedit->every_month_circle, 1, 0);
        lv_obj_set_style_bg_color(g_pt_lv_timingedit->every_month_circle, lv_color_hex(WHITE_HEX), 0);
        break;
    case EVERY_YEAR:
        lv_obj_set_style_border_width(g_pt_lv_timingedit->every_year_circle, 1, 0);
        lv_obj_set_style_bg_color(g_pt_lv_timingedit->every_year_circle, lv_color_hex(WHITE_HEX), 0);
        break;
    }

    switch(rep_type)
    {
    case NOT_REP:
        lv_obj_set_style_border_width(g_pt_lv_timingedit->not_rep_circle, 0, 0);
        lv_obj_set_style_bg_color(g_pt_lv_timingedit->not_rep_circle, lv_color_hex(BLUE_HEX), 0);
        break;
    case EVERY_DAY:
        lv_obj_set_style_border_width(g_pt_lv_timingedit->every_day_circle, 0, 0);
        lv_obj_set_style_bg_color(g_pt_lv_timingedit->every_day_circle, lv_color_hex(BLUE_HEX), 0);
        break;
    case EVERY_WEEK:
        lv_obj_set_style_border_width(g_pt_lv_timingedit->every_week_circle, 0, 0);
        lv_obj_set_style_bg_color(g_pt_lv_timingedit->every_week_circle, lv_color_hex(BLUE_HEX), 0);
        break;
    case EVERY_MONTH:
        lv_obj_set_style_border_width(g_pt_lv_timingedit->every_month_circle, 0, 0);
        lv_obj_set_style_bg_color(g_pt_lv_timingedit->every_month_circle, lv_color_hex(BLUE_HEX), 0);
        break;
    case EVERY_YEAR:
        lv_obj_set_style_border_width(g_pt_lv_timingedit->every_year_circle, 0, 0);
        lv_obj_set_style_bg_color(g_pt_lv_timingedit->every_year_circle, lv_color_hex(BLUE_HEX), 0);
        break;
    }

}


/* refresh_edit_page
 * 刷新页面主要是三个标签
 */
static void refresh_edit_page(void)
{
    memset(repeat_char, '\0', sizeof(repeat_char));
    switch(sel_rep_type)
    {
    case NOT_REP:
        sprintf(repeat_char, "%s", "不重复");
        break;
    case EVERY_DAY:
        sprintf(repeat_char, "%s", "每日");
        break;
    case EVERY_WEEK:
        sprintf(repeat_char, "%s", "每周");
        break;
    case EVERY_MONTH:
        sprintf(repeat_char, "%s", "每月");
        break;
    }
    lv_label_set_text(g_pt_lv_timingedit->rep_label, (const char *)repeat_char);

    /*结束重复时间*/
    lv_label_set_text_fmt(g_pt_lv_timingedit->end_rep_label, "%d-%02d-%02d", sel_year, sel_mon, sel_day);


    /*提醒语音*/
    lv_label_set_text_fmt(g_pt_lv_timingedit->audio_label, "语音%d", sel_audio_id);

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
		if(g_pt_lv_timingedit->bg_about != NULL)    lv_obj_del(g_pt_lv_timingedit->bg_about);

		vPortFree(g_pt_lv_timingedit);

		lv_timing_list();
	}
}

/* event_handler_back_to_home
 * 返回按键点击事件的回调函数
 * */
static void event_handler_confirm_to_home(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
		if(g_pt_lv_timingedit->bg_about != NULL)    lv_obj_del(g_pt_lv_timingedit->bg_about);

		vPortFree(g_pt_lv_timingedit);

		lv_timing_list();
	}
}

/* event_handler_del_btn
 * 删除定时器按钮点击事件回调函数
 */
static void event_handler_del_btn(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    /*删除定时器*/

		if(g_pt_lv_timingedit->bg_about != NULL)    lv_obj_del(g_pt_lv_timingedit->bg_about);

		vPortFree(g_pt_lv_timingedit);

		lv_timing_list();
	}
}

/* event_handler_rep_btn
 * 选择重复类型点击事件回调函数
 */
static void event_handler_rep_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    /*新建配置页面*/
	    lv_create_setrep_page();
	}
}

/* event_handler_rep_btn
 * 选择结束重复类型点击事件回调函数
 */
static void event_handler_rep_end_btn(lv_event_t *e)
{

    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    /*新建结束时间配置页面*/
	    lv_create_set_stoptime_page();
	}

}

/* event_handler_rep_btn
 * 选择提醒音点击事件回调函数
 */
static void event_handler_aduio_btn(lv_event_t *e)
{

    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    /*新建结束时间配置页面*/
	    lv_create_set_audio_page();
	}
}

/* event_handler_bgdel_btn
 * 删除页面
 */
static void event_handler_bgdel_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	lv_obj_t *obj = lv_event_get_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
        refresh_edit_page();/*刷新edit 页面*/
        lv_obj_del(obj);/*删除页面*/
        rep_type = sel_rep_type;
        week_select = sel_week;
        month_select = sel_mon_day;
        current_days[0].day = sel_day;
        current_days[0].month = sel_mon;
        current_days[0].year = sel_year;
        last_audio_btn = NULL;/*再次创建后地址改变*/
	}

}

/* event_handler_notrep_btn
 * 重复类型不重复点击事件
 */
static void event_handler_notrep_btn(lv_event_t *e)
{

    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    rep_type = NOT_REP;/*更新rep_type*/
        modify_the_select_bg();/*重新绘画圆圈*/
	    last_rep_type = rep_type;/*更新last点击的类型*/

        sel_rep_type = rep_type;/*当前选择的重复类型改变*/
	    refresh_edit_page();/*刷新edit 页面*/
	    /*删除配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->rep_bg_cont);

	}
}

/* event_handler_notrep_btn
 * 重复类型每天点击事件
 */
static void event_handler_everyday_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    rep_type = EVERY_DAY;/*更新rep_type*/
        modify_the_select_bg();/*重新绘画圆圈*/
	    last_rep_type = rep_type;/*更新last点击的类型*/

	    sel_rep_type = rep_type;/*当前选择的重复类型改变*/
	    refresh_edit_page();/*刷新edit 页面*/
	    /*删除配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->rep_bg_cont);
	}

}


/* event_handler_notrep_btn
 * 重复类型每周点击事件
 */
static void event_handler_everyweek_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    rep_type = EVERY_WEEK;/*更新rep_type*/
        modify_the_select_bg();/*重新绘画圆圈*/
	    last_rep_type = rep_type;/*更新last点击的类型*/

        //refresh_edit_page();/*刷新edit 页面*/
	    /*删除配置页面*/
	    lv_create_set_week_page();/*创建新页面*/
	}

}

/* event_handler_notrep_btn
 * 重复类型每月点击事件
 */
static void event_handler_everymonth_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    rep_type = EVERY_MONTH;/*更新rep_type*/
        modify_the_select_bg();/*重新绘画圆圈*/
	    last_rep_type = rep_type;/*更新last点击的类型*/

	    //refresh_edit_page();/*刷新edit 页面*/
	    /*删除配置页面*/
	    lv_create_set_month_page();/*创建新页面*/
	}

}


/* event_handler_notrep_btn
 * 重复类型吗每年点击事件
 */
static void event_handler_everyyear_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    rep_type = EVERY_YEAR;/*更新rep_type*/
        modify_the_select_bg();/*重新绘画圆圈*/
	    last_rep_type = rep_type;/*更新last点击的类型*/
	}

}

/* event_handler_weekset_btn
 * 选择周几的容器点击事件回调函数
 */
static void event_handler_weekset_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    uint8_t day_index = (uint8_t)lv_event_get_user_data(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *check_box = lv_obj_get_child(obj, 1);/*注意如果界面更新了需要注意checkbox id*/
    lv_state_t state = lv_obj_get_state(check_box);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{

	    if((state & LV_STATE_CHECKED))
        {
            lv_obj_clear_state(check_box, LV_STATE_CHECKED);
            week_select &= ~(1 << day_index);
        }
        else
        {
            lv_obj_add_state(check_box, LV_STATE_CHECKED);
            week_select |= (1 << day_index);
        }
        printf("day_inde = %d", day_index);
        printf("week_select = %x", week_select);

	}

}

/* event_handler_weekset_btn
 * 选择周几的复选框值改变事件回调函数
 */
static void event_handler_weekbox_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    uint8_t day_index = (uint8_t)lv_event_get_user_data(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	lv_obj_t *obj = lv_event_get_target(e);
    lv_state_t state = lv_obj_get_state(obj);
	if(code == LV_EVENT_VALUE_CHANGED)/*值改变*/
	{
	    if((state & LV_STATE_CHECKED))
        {
            week_select |= (1 << day_index);

        }
        else
        {
            week_select &= ~(1 << day_index);
        }
        printf("day_inde = %d", day_index);
        printf("week_select = %x", week_select);

	}

}

/* event_handler_week_cancel_btn
 * 设置周界面的取消函数,
 */
static void event_handler_week_cancel_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    /*删除每周配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->week_bg_cont);
        week_select = sel_week;
        rep_type = sel_rep_type;
	}
}

/* event_handler_week_cancel_btn
 * 设置周界面的取消函数,
 */
static void event_handler_week_confirm_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{

        sel_week = week_select;
	    sel_rep_type = rep_type;/*当前选择的重复类型改变*/
	    refresh_edit_page();/*刷新edit 页面*/
	    /*删除配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->week_bg_cont);
	    /*返回主界面*/
	    lv_obj_del(g_pt_lv_timingedit->rep_bg_cont);
        /*设置定时器参数中的定时器类型*/

	}
}

/* event_handler_month_cancel_btn
 * 设置月界面的取消函数,
 */
static void event_handler_month_cancel_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    rep_type = sel_rep_type;
	    /*删除每周配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->month_bg_cont);
        month_select = sel_mon_day;
	}
}

/* event_handler_week_cancel_btn
 * 设置月界面的取消函数,
 */
static void event_handler_month_confirm_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    sel_rep_type = rep_type;/*当前选择的重复类型改变*/
	    sel_mon_day = month_select;/*更新值*/

	    refresh_edit_page();/*刷新edit 页面*/
	    /*删除配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->month_bg_cont);
	    /*返回主界面*/
	    lv_obj_del(g_pt_lv_timingedit->rep_bg_cont);
        /*设置定时器参数中的定时器类型*/

	}
}

/* event_handler_month_cancel_btn
 * 设置月界面的取消函数,
 */
static void event_handler_stoptime_cancel_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    /*删除结束时间配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->stoptime_bg_cont);
        current_days[0].day = sel_day;
        current_days[0].month = sel_mon;
        current_days[0].year = sel_year;
	}
}

/* event_handler_stoptime_confirm_btn
 * 设置结束时间界面的取消函数,
 */
static void event_handler_stoptime_confirm_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    /*更新时间*/
        sel_day = current_days[0].day;
        sel_mon = current_days[0].month;
        sel_year = current_days[0].year;

	    refresh_edit_page();/*刷新edit 页面*/

	    /*删除结束时间配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->stoptime_bg_cont);

	}
}


/* event_handler_cancel_btn
 * 返回设置timer界面回调函数
 */
static void event_handler_cancel_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*没聚焦*/
	{
        /*删除配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->rep_bg_cont);
	}

}

/* event_handler_stoptime_confirm_btn
 * 设置结束时间界面的取消函数,
 */
static void event_handler_audio_confirm_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    last_audio_btn = NULL;/*再次创建后地址改变*/
	    refresh_edit_page();/*刷新edit 页面*/
	    /*删除结束时间配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->audio_bg_cont);
        /*设置定时器参数中的定时器类型*/

	}
}


/* event_handler_cancel_btn
 * 返回设置timer界面回调函数
 */
static void event_handler_audio_cancel_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*没聚焦*/
	{
	    last_audio_btn = NULL;/*再次创建后地址改变*/
        /*删除配置页面*/
	    lv_obj_del(g_pt_lv_timingedit->audio_bg_cont);
	}

}

static void event_handler_audio_btn_clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    int audio_id = (int)lv_event_get_user_data(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*没聚焦*/
	{
        if(last_audio_btn)
        {
            lv_obj_set_style_bg_color(last_audio_btn, lv_color_hex(WHITE_HEX), 0);
        }
        sel_audio_id = audio_id;
        lv_obj_set_style_bg_color(obj, lv_color_hex(RED_HEX), 0);

        last_audio_btn = obj;
	}
}


static void event_handler_calendar(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	//lv_obj_t *obj = lv_event_get_target(e);
	if(code == LV_EVENT_VALUE_CHANGED)/*值改变*/
	{
        lv_calendar_date_t date;
        if(lv_calendar_get_pressed_date(obj, &date))
        {
            current_days[0].day = date.day;
            current_days[0].month = date.month;
            current_days[0].year = date.year;
            lv_calendar_set_highlighted_dates(obj, current_days, 1);
            LOG_D("clicked date:%02d.%02d.%d\r\n",date.day, date.month, date.year);
        }
	}
}

static void event_handler_month_day_roller(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_target(e);
	if(code == LV_EVENT_VALUE_CHANGED)/*值改变*/
	{
        char buf[10];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        month_select = atoi(buf);
	}
}



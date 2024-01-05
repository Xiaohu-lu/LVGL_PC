/*
 * lv_dev_info_page.c
 *
 *  Created on: Dec 14, 2023
 *      Author: hxd
 */

/* lvgl
 * */

#include "lvgl/lvgl.h"
#include "UI/lv_typedef.h"
#include "UI/lv_dev_info_page.h"
#include "UI/lv_main_page.h"
#include "UI/lv_about_page.h"


static PT_lv_devinfo g_pt_lv_devinfo;		/*数据结构体*/

static uint8_t g_pose_mode = FAST_MODE;

uint8_t g_run_state = 0;




static void lv_dev_info_init(void);
static void event_handler_back_to_home(lv_event_t *e);
static void event_handler_pose_mode_btn(lv_event_t *e);
static void event_handler_volume_slider(lv_event_t *e);
static void event_handler_switch(lv_event_t *e);
/* lv_board_about
 * 板子信息界面
 * */
void lv_dev_info(void)
{
	g_pt_lv_devinfo = (T_lv_devinfo*)pvPortMalloc(sizeof(T_lv_devinfo));
	g_pt_lv_devinfo->bg_about = lv_obj_create(lv_scr_act());/*创建背景*/

	lv_obj_set_size(g_pt_lv_devinfo->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_devinfo->bg_about, 0, 0);/*设置圆角*/
	lv_obj_align(g_pt_lv_devinfo->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*设置padding*/
	set_user_style(g_pt_lv_devinfo->bg_about);

	lv_obj_set_size(g_pt_lv_devinfo->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_devinfo->bg_about, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_devinfo->bg_about, lv_color_hex(BACK_COLOR), 0);
	lv_dev_info_init();/*界面初始化*/

	add_title(g_pt_lv_devinfo->bg_about, (const char *)gdev_param.bind_devId);
	add_back(g_pt_lv_devinfo->bg_about, event_handler_back_to_home);

}

/* lv_dev_info_init
 * 设备信息界面初始化
 *
 * */
static void lv_dev_info_init(void)
{
	g_run_state = 1;

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
	lv_obj_t *cont_mode = lv_obj_create(g_pt_lv_devinfo->bg_about);
	lv_obj_set_size(cont_mode, LV_DEVINFO_US_CONT_W, LV_DEVINFO_US_CONT_H);/*设置对象大小*/
	lv_obj_add_style(cont_mode, &style_cont, 0);/*默认状态下的style*/
	lv_obj_align(cont_mode, LV_ALIGN_TOP_MID, 0, LV_DEVINFO_US_TITLE_SPACE);/*设置位置,在他父对象的顶部中间*/
	lv_obj_fade_in(cont_mode, 0, 0);/*淡入*/

	/*创建一个标签*/
	lv_obj_t *label_mode = lv_label_create(cont_mode);
	lv_label_set_text(label_mode, "提醒模式");
	lv_obj_set_style_text_font(label_mode, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_mode, LV_ALIGN_TOP_LEFT, LV_DEVINFO_US_LABEL_SPACE, 0);/*设置位置*/
	lv_obj_fade_in(label_mode, 0, 0);/*淡入*/

	/*创建模式标签按钮*/
	lv_obj_t *btn;

	/*创建模式按钮样式*/
	static lv_style_t style_btn_fast;
	lv_style_init(&style_btn_fast);
	lv_style_set_bg_opa(&style_btn_fast, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_bg_color(&style_btn_fast, lv_color_hex(0xBEBEBE));/*设置背景颜色*/
	lv_style_set_pad_top(&style_btn_fast,0);
	lv_style_set_pad_bottom(&style_btn_fast, 0);
	lv_style_set_pad_left(&style_btn_fast, 0);
	lv_style_set_pad_right(&style_btn_fast, 0);
	lv_style_set_border_width(&style_btn_fast, 1);
	lv_style_set_radius(&style_btn_fast, 100);/*设置圆角*/

	/*极速体验模式*/
	g_pt_lv_devinfo->fast_mode = lv_btn_create(cont_mode);
	btn = g_pt_lv_devinfo->fast_mode;
	lv_obj_add_style(btn, &style_btn_fast, 0);/*添加样式*/
	lv_obj_set_size(btn, LV_DEVINFO_FAST_MODE_W, LV_DEVINFO_FAST_MODE_H);
	lv_obj_align(btn, LV_ALIGN_OUT_BOTTOM_LEFT, 45, 20);
	lv_obj_fade_in(btn, 0, 0);/*淡入*/


	lv_obj_t *label_text_fast = lv_label_create(btn);
	lv_label_set_text(label_text_fast, LV_SYMBOL_LEFT);/*设置text*/
	lv_obj_set_style_text_color(label_text_fast, lv_color_hex(0xFFFFFF), 0);/*设置字体颜色*/
	lv_obj_set_style_text_font(label_text_fast, &lv_font_montserrat_22, 0);/*设置字体*/
	lv_obj_center(label_text_fast);
	lv_obj_fade_in(label_text_fast, 0, 0);


	lv_obj_t *label_fast = lv_label_create(cont_mode);
	lv_obj_set_style_text_font(label_fast, &song_font14, 0);/*默认状态下的字体样式*/
	lv_label_set_text(label_fast, "极速体验");
	lv_obj_align_to(label_fast, btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 4);
	lv_obj_fade_in(label_fast, 0, 0);

	/*习惯养成模式*/
	g_pt_lv_devinfo->normal_mode = lv_btn_create(cont_mode);
	btn = g_pt_lv_devinfo->normal_mode;
	lv_obj_add_style(btn, &style_btn_fast, 0);/*添加样式*/
	lv_obj_set_size(btn, LV_DEVINFO_FAST_MODE_W, LV_DEVINFO_FAST_MODE_H);
	lv_obj_align(btn, LV_ALIGN_OUT_BOTTOM_LEFT, 130, 20);
	lv_obj_fade_in(btn, 0, 0);/*淡入*/

	lv_obj_t *label_text_normal = lv_label_create(btn);
	lv_label_set_text(label_text_normal, LV_SYMBOL_STOP);/*设置text*/
	lv_obj_set_style_text_color(label_text_normal, lv_color_hex(0xFFFFFF), 0);/*设置字体颜色*/
	lv_obj_set_style_text_font(label_text_normal, &lv_font_montserrat_22, 0);/*设置字体*/
	lv_obj_center(label_text_normal);
	lv_obj_fade_in(label_text_normal, 0, 0);

	lv_obj_t *label_normal = lv_label_create(cont_mode);
	lv_obj_set_style_text_font(label_normal, &song_font14, 0);/*默认状态下的字体样式*/
	lv_label_set_text(label_normal, "习惯养成");
	lv_obj_align_to(label_normal, btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 4);
	lv_obj_fade_in(label_normal, 0, 0);

	/*习惯保持模式*/
	g_pt_lv_devinfo->habit_mode = lv_btn_create(cont_mode);
	btn = g_pt_lv_devinfo->habit_mode;
	lv_obj_add_style(btn, &style_btn_fast, 0);/*添加样式*/
	lv_obj_set_size(btn, LV_DEVINFO_FAST_MODE_W, LV_DEVINFO_FAST_MODE_H);
	lv_obj_align(btn, LV_ALIGN_OUT_BOTTOM_LEFT, 215, 20);
	lv_obj_fade_in(btn, 0, 0);/*淡入*/

	lv_obj_t *label_text_habit = lv_label_create(btn);
	lv_label_set_text(label_text_habit, LV_SYMBOL_RIGHT);/*设置text*/
	lv_obj_set_style_text_color(label_text_habit, lv_color_hex(0xFFFFFF), 0);/*设置字体颜色*/
	lv_obj_set_style_text_font(label_text_habit, &lv_font_montserrat_22, 0);/*设置字体*/
	lv_obj_center(label_text_habit);
	lv_obj_fade_in(label_text_habit, 0, 0);


	lv_obj_t *label_habit = lv_label_create(cont_mode);
	lv_obj_set_style_text_font(label_habit, &song_font14, 0);/*默认状态下的字体样式*/
	lv_label_set_text(label_habit, "习惯保持");
	lv_obj_align_to(label_habit, btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 4);
	lv_obj_fade_in(label_habit, 0, 0);


	/*添加点击回调函数*/
	lv_obj_add_event_cb(g_pt_lv_devinfo->fast_mode, event_handler_pose_mode_btn,LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(g_pt_lv_devinfo->normal_mode, event_handler_pose_mode_btn,LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(g_pt_lv_devinfo->habit_mode, event_handler_pose_mode_btn,LV_EVENT_CLICKED, NULL);

	/*更新g_pose_mode*/
	g_pose_mode = gdev_param.remindMode;

	switch(g_pose_mode)
	{
		case FAST_MODE:
		{
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->fast_mode, lv_color_hex(0x0000FF), 0);
			break;
		}
		case NORMAL_MODE:
		{
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->normal_mode, lv_color_hex(0x0000FF), 0);
			break;
		}
		case HABIT_MODE:
		{
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->habit_mode, lv_color_hex(0x0000FF), 0);
			break;
		}
	}

	/*创建音量容器*/
	lv_obj_t *cont_volume = lv_obj_create(g_pt_lv_devinfo->bg_about);
	lv_obj_add_style(cont_volume, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_size(cont_volume, LV_DEVINFO_US_CONT_W, LV_DEVINFO_VOLUME_CONT_H);
	lv_obj_align_to(cont_volume, cont_mode, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	lv_obj_fade_in(cont_volume, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_volume = lv_label_create(cont_volume);
	lv_label_set_text(label_volume, "提醒音量");
	lv_obj_set_style_text_font(label_volume, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_volume, LV_ALIGN_TOP_LEFT, LV_DEVINFO_US_LABEL_SPACE, 0);/*设置位置*/
	lv_obj_fade_in(label_volume, 0, 0);/*淡入*/

	/*创建滚动条*/
	lv_obj_t *slider;
	g_pt_lv_devinfo->volume_slider = lv_slider_create(cont_volume);
	slider = g_pt_lv_devinfo->volume_slider;
	lv_slider_set_range(slider, 40, 100);/*设置滚动条范围*/

	lv_obj_set_width(slider, 160);/*设置对象的宽度*/
	lv_obj_set_height(slider, 14);/*设置对象的高度*/
	lv_obj_align(slider, LV_ALIGN_CENTER, 0, 5);
	lv_slider_set_value(slider, gdev_param.volume, LV_ANIM_OFF);/*设置当前值*/

	/*设置事件回调函数*/
	//lv_obj_add_event_cb(slider, event_handler_volume_slider, LV_EVENT_VALUE_CHANGED, NULL);
	lv_obj_add_event_cb(slider, event_handler_volume_slider, LV_EVENT_RELEASED, NULL);

	/*创建音量标签的样式*/
	static lv_style_t style_volume_btn;
	lv_style_init(&style_volume_btn);
	lv_style_set_bg_opa(&style_volume_btn, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_bg_color(&style_volume_btn, lv_color_hex(BACK_COLOR));/*设置背景颜色*/
	lv_style_set_pad_top(&style_volume_btn,0);
	lv_style_set_pad_bottom(&style_volume_btn, 0);
	lv_style_set_pad_left(&style_volume_btn, 0);
	lv_style_set_pad_right(&style_volume_btn, 0);
	lv_style_set_border_width(&style_volume_btn, 0);
	lv_style_set_radius(&style_volume_btn, 0);/*设置圆角*/
	lv_style_set_text_color(&style_volume_btn, lv_color_hex(0x0));/*设置字体颜色*/
	lv_style_set_text_font(&style_volume_btn, &lv_font_montserrat_14);/*设置字体*/

	/*创建两个标签*/
	lv_obj_t *volume_btn1 = lv_label_create(cont_volume);
	lv_label_set_text(volume_btn1, LV_SYMBOL_MUTE);
	lv_obj_add_style(volume_btn1, &style_volume_btn, 0);
	//lv_obj_set_size(volume_btn1, LV_DEVINFO_MIN_VOLUME_W, LV_DEVINFO_MIN_VOLUME_H);
	lv_obj_align_to(volume_btn1, slider, LV_ALIGN_OUT_LEFT_MID, -(10 + 10), 0);

	lv_obj_t *volume_btn2 = lv_label_create(cont_volume);
	lv_label_set_text(volume_btn2, LV_SYMBOL_VOLUME_MAX);
	lv_obj_add_style(volume_btn2, &style_volume_btn, 0);
	//lv_obj_set_size(volume_btn2, LV_DEVINFO_MIN_VOLUME_W, LV_DEVINFO_MIN_VOLUME_H);
	lv_obj_align_to(volume_btn2, slider, LV_ALIGN_OUT_RIGHT_MID, 15, 0);

	/*创建功能开关容器*/
	lv_obj_t *cont_function = lv_obj_create(g_pt_lv_devinfo->bg_about);
	lv_obj_add_style(cont_function, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_size(cont_function, LV_DEVINFO_US_CONT_W, LV_DEVINFO_FUNC_CONT_H);
	lv_obj_align_to(cont_function, cont_volume, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	lv_obj_fade_in(cont_function, 0, 0);

	/*创建一个标签*/
	lv_obj_t *label_func = lv_label_create(cont_function);
	lv_label_set_text(label_func, "功能开关");
	lv_obj_set_style_text_font(label_func, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_func, LV_ALIGN_TOP_LEFT, LV_DEVINFO_US_LABEL_SPACE, 0);/*设置位置*/
	lv_obj_fade_in(label_func, 0, 0);/*淡入*/


	/*创建一个容器<--->坐姿摄像头开关*/
	lv_obj_t *cont_pos = lv_obj_create(cont_function);
	lv_obj_add_style(cont_pos, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_style_border_width(cont_pos, 0, 0);
	lv_obj_set_size(cont_pos, LV_FUNC_SWITCH_CONT_W, LV_FUNC_SWITCH_CONT_H);/*设置大小*/
	lv_obj_align(cont_pos, LV_ALIGN_TOP_MID, 0, 15);/*设置位置*/
	lv_obj_fade_in(cont_pos, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_pos = lv_label_create(cont_pos);
	lv_label_set_text(label_pos, "坐姿摄像头开关");
	lv_obj_set_style_text_font(label_pos, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_pos, LV_ALIGN_LEFT_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(label_pos, 0, 0);/*淡入*/
	/*创建一个switch*/
	g_pt_lv_devinfo->pos_switch = lv_switch_create(cont_pos);/*坐姿摄像头开关*/
	lv_obj_set_size(g_pt_lv_devinfo->pos_switch, 80, 40);
	lv_obj_align(g_pt_lv_devinfo->pos_switch, LV_ALIGN_RIGHT_MID, -5, 0);


	/*创建一个容器<--->久坐提醒开关*/
	lv_obj_t *cont_longsit = lv_obj_create(cont_function);
	lv_obj_add_style(cont_longsit, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_style_border_width(cont_longsit, 0, 0);
	lv_obj_set_size(cont_longsit, LV_FUNC_SWITCH_CONT_W, LV_FUNC_SWITCH_CONT_H);/*设置大小*/
	lv_obj_align_to(cont_longsit, cont_pos, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);/*设置位置*/
	lv_obj_fade_in(cont_longsit, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_longsit = lv_label_create(cont_longsit);
	lv_label_set_text(label_longsit, "久坐提醒开关");
	lv_obj_set_style_text_font(label_longsit, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_longsit, LV_ALIGN_LEFT_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(label_longsit, 0, 0);/*淡入*/
	/*创建一个switch*/
	g_pt_lv_devinfo->longsit_switch = lv_switch_create(cont_longsit);/*坐姿摄像头开关*/
	lv_obj_set_size(g_pt_lv_devinfo->longsit_switch, 80, 40);
	lv_obj_align(g_pt_lv_devinfo->longsit_switch, LV_ALIGN_RIGHT_MID, -5, 0);

	/*创建一个容器<--->环境光检测开关*/
	lv_obj_t *cont_light = lv_obj_create(cont_function);
	lv_obj_add_style(cont_light, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_style_border_width(cont_light, 0, 0);
	lv_obj_set_size(cont_light, LV_FUNC_SWITCH_CONT_W, LV_FUNC_SWITCH_CONT_H);/*设置大小*/
	lv_obj_align_to(cont_light, cont_longsit, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);/*设置位置*/
	lv_obj_fade_in(cont_light, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_light = lv_label_create(cont_light);
	lv_label_set_text(label_light, "环境光检测开关");
	lv_obj_set_style_text_font(label_light, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_light, LV_ALIGN_LEFT_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(label_light, 0, 0);/*淡入*/
	/*创建一个switch*/
	g_pt_lv_devinfo->light_switch = lv_switch_create(cont_light);/*坐姿摄像头开关*/
	lv_obj_set_size(g_pt_lv_devinfo->light_switch, 80, 40);
	lv_obj_align(g_pt_lv_devinfo->light_switch, LV_ALIGN_RIGHT_MID, -5, 0);

	/*创建一个容器<--->离座监测开关*/
	lv_obj_t *cont_offset = lv_obj_create(cont_function);
	lv_obj_add_style(cont_offset, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_style_border_width(cont_offset, 0, 0);
	lv_obj_set_size(cont_offset, LV_FUNC_SWITCH_CONT_W, LV_FUNC_SWITCH_CONT_H);/*设置大小*/
	lv_obj_align_to(cont_offset, cont_light, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);/*设置位置*/
	lv_obj_fade_in(cont_offset, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_offset = lv_label_create(cont_offset);
	lv_label_set_text(label_offset, "离座监测开关");
	lv_obj_set_style_text_font(label_offset, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_offset, LV_ALIGN_LEFT_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(label_offset, 0, 0);/*淡入*/
	/*创建一个switch*/
	g_pt_lv_devinfo->offset_switch = lv_switch_create(cont_offset);/*坐姿摄像头开关*/
	lv_obj_set_size(g_pt_lv_devinfo->offset_switch, 80, 40);
	lv_obj_align(g_pt_lv_devinfo->offset_switch, LV_ALIGN_RIGHT_MID, -5, 0);


	/*创建一个容器<--->语音识别开关*/
	lv_obj_t *cont_mic = lv_obj_create(cont_function);
	lv_obj_add_style(cont_mic, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_style_border_width(cont_mic, 0, 0);
	lv_obj_set_size(cont_mic, LV_FUNC_SWITCH_CONT_W, LV_FUNC_SWITCH_CONT_H);/*设置大小*/
	lv_obj_align_to(cont_mic, cont_offset, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);/*设置位置*/
	lv_obj_fade_in(cont_mic, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_mic = lv_label_create(cont_mic);
	lv_label_set_text(label_mic, "语音识别开关");
	lv_obj_set_style_text_font(label_mic, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_mic, LV_ALIGN_LEFT_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(label_mic, 0, 0);/*淡入*/
	/*创建一个switch*/
	g_pt_lv_devinfo->mic_switch = lv_switch_create(cont_mic);/*坐姿摄像头开关*/
	lv_obj_set_size(g_pt_lv_devinfo->mic_switch, 80, 40);
	lv_obj_align(g_pt_lv_devinfo->mic_switch, LV_ALIGN_RIGHT_MID, -5, 0);


	/*创建一个容器<--->图像上传开关*/
	lv_obj_t *cont_upload = lv_obj_create(cont_function);
	lv_obj_add_style(cont_upload, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_style_border_width(cont_upload, 0, 0);
	lv_obj_set_size(cont_upload, LV_FUNC_SWITCH_CONT_W, LV_FUNC_SWITCH_CONT_H);/*设置大小*/
	lv_obj_align_to(cont_upload, cont_mic, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);/*设置位置*/
	lv_obj_fade_in(cont_upload, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_upload = lv_label_create(cont_upload);
	lv_label_set_text(label_upload, "图像上传开关");
	lv_obj_set_style_text_font(label_upload, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_upload, LV_ALIGN_LEFT_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(label_upload, 0, 0);/*淡入*/
	/*创建一个switch*/
	g_pt_lv_devinfo->upload_switch = lv_switch_create(cont_upload);/*坐姿摄像头开关*/
	lv_obj_set_size(g_pt_lv_devinfo->upload_switch, 80, 40);
	lv_obj_align(g_pt_lv_devinfo->upload_switch, LV_ALIGN_RIGHT_MID, -5, 0);

	/*创建一个容器<--->语音提醒开关*/
	lv_obj_t *cont_voicewarn = lv_obj_create(cont_function);
	lv_obj_add_style(cont_voicewarn, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_style_border_width(cont_voicewarn, 0, 0);
	lv_obj_set_size(cont_voicewarn, LV_FUNC_SWITCH_CONT_W, LV_FUNC_SWITCH_CONT_H);/*设置大小*/
	lv_obj_align_to(cont_voicewarn, cont_upload, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);/*设置位置*/
	lv_obj_fade_in(cont_voicewarn, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_voicewarn = lv_label_create(cont_voicewarn);
	lv_label_set_text(label_voicewarn, "语音提醒开关");
	lv_obj_set_style_text_font(label_voicewarn, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_voicewarn, LV_ALIGN_LEFT_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(label_voicewarn, 0, 0);/*淡入*/
	/*创建一个switch*/
	g_pt_lv_devinfo->voicewarn_switch = lv_switch_create(cont_voicewarn);/*坐姿摄像头开关*/
	lv_obj_set_size(g_pt_lv_devinfo->voicewarn_switch, 80, 40);
	lv_obj_align(g_pt_lv_devinfo->voicewarn_switch, LV_ALIGN_RIGHT_MID, -5, 0);

	/*创建一个容器<--->震动提醒开关*/
	lv_obj_t *cont_vibrate = lv_obj_create(cont_function);
	lv_obj_add_style(cont_vibrate, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_style_border_width(cont_vibrate, 0, 0);
	lv_obj_set_size(cont_vibrate, LV_FUNC_SWITCH_CONT_W, LV_FUNC_SWITCH_CONT_H);/*设置大小*/
	lv_obj_align_to(cont_vibrate, cont_voicewarn, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);/*设置位置*/
	lv_obj_fade_in(cont_vibrate, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_vibrate = lv_label_create(cont_vibrate);
	lv_label_set_text(label_vibrate, "震动提醒开关");
	lv_obj_set_style_text_font(label_vibrate, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_vibrate, LV_ALIGN_LEFT_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(label_vibrate, 0, 0);/*淡入*/
	/*创建一个switch*/
	g_pt_lv_devinfo->vibrate_switch = lv_switch_create(cont_vibrate);/*坐姿摄像头开关*/
	lv_obj_set_size(g_pt_lv_devinfo->vibrate_switch, 80, 40);
	lv_obj_align(g_pt_lv_devinfo->vibrate_switch, LV_ALIGN_RIGHT_MID, -5, 0);

	/*创建一个容器<--->灯光提醒开关*/
	lv_obj_t *cont_lightwarn = lv_obj_create(cont_function);
	lv_obj_add_style(cont_lightwarn, &style_cont, 0);/*默认状态下的style*/
	lv_obj_set_style_border_width(cont_lightwarn, 0, 0);
	lv_obj_set_size(cont_lightwarn, LV_FUNC_SWITCH_CONT_W, LV_FUNC_SWITCH_CONT_H);/*设置大小*/
	lv_obj_align_to(cont_lightwarn, cont_vibrate, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);/*设置位置*/
	lv_obj_fade_in(cont_lightwarn, 0, 0);
	/*创建一个标签*/
	lv_obj_t *label_lightwarn = lv_label_create(cont_lightwarn);
	lv_label_set_text(label_lightwarn, "灯光提醒开关");
	lv_obj_set_style_text_font(label_lightwarn, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_lightwarn, LV_ALIGN_LEFT_MID, 0, 0);/*设置位置*/
	lv_obj_fade_in(label_lightwarn, 0, 0);/*淡入*/
	/*创建一个switch*/
	g_pt_lv_devinfo->lightwarn_switch = lv_switch_create(cont_lightwarn);/*坐姿摄像头开关*/
	lv_obj_set_size(g_pt_lv_devinfo->lightwarn_switch, 80, 40);
	lv_obj_align(g_pt_lv_devinfo->lightwarn_switch, LV_ALIGN_RIGHT_MID, -5, 0);

	lv_obj_add_event_cb(g_pt_lv_devinfo->pos_switch, event_handler_switch, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(g_pt_lv_devinfo->longsit_switch, event_handler_switch, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(g_pt_lv_devinfo->light_switch, event_handler_switch, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(g_pt_lv_devinfo->offset_switch, event_handler_switch, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(g_pt_lv_devinfo->mic_switch, event_handler_switch, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(g_pt_lv_devinfo->upload_switch, event_handler_switch, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(g_pt_lv_devinfo->voicewarn_switch, event_handler_switch, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(g_pt_lv_devinfo->vibrate_switch, event_handler_switch, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(g_pt_lv_devinfo->lightwarn_switch, event_handler_switch, LV_EVENT_ALL, NULL);

	/* 设置开关的初始状态
	 * */
	if(gdev_param.postureCam)/*坐姿摄像头开关*/
	{
		lv_obj_add_state(g_pt_lv_devinfo->pos_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->pos_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.longSitRemind)//久坐提醒开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->longsit_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->longsit_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.lightDetecter)//环境光检测开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->light_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->light_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.offSeat)//离座检测开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->offset_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->offset_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.micSwitch)//语音识别开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->mic_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->mic_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.uploadPic)//图像上传开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->upload_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->upload_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.voiceWarn)//语音提醒开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->voicewarn_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->voicewarn_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.vibrateWarn)//震动提醒开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->vibrate_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->vibrate_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.lightWarn)//灯光提醒开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->lightwarn_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->lightwarn_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

}

/* lv_device_info_refresh
 * 刷新界面
 *
 * */
void lv_device_info_refresh(void)
{
	if(g_run_state == 0)
		return;

	/*更新g_pose_mode*/
	g_pose_mode = gdev_param.remindMode;
	switch(g_pose_mode)
	{
		case FAST_MODE:
		{
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->fast_mode, lv_color_hex(0x0000FF), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->normal_mode, lv_color_hex(0xBEBEBE), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->habit_mode, lv_color_hex(0xBEBEBE), 0);
			break;
		}
		case NORMAL_MODE:
		{
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->fast_mode, lv_color_hex(0xBEBEBE), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->normal_mode, lv_color_hex(0x0000FF), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->habit_mode, lv_color_hex(0xBEBEBE), 0);
			break;
		}
		case HABIT_MODE:
		{
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->fast_mode, lv_color_hex(0xBEBEBE), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->normal_mode, lv_color_hex(0xBEBEBE), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->habit_mode, lv_color_hex(0x0000FF), 0);
			break;
		}
	}

	/*根据音量值,设置进度条值*/
	lv_slider_set_value(g_pt_lv_devinfo->volume_slider, gdev_param.volume, LV_ANIM_OFF);/*设置当前值*/

	/* 设置开关的初始状态
	 * */
	if(gdev_param.postureCam)/*坐姿摄像头开关*/
	{
		lv_obj_add_state(g_pt_lv_devinfo->pos_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->pos_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.longSitRemind)//久坐提醒开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->longsit_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->longsit_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.lightDetecter)//环境光检测开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->light_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->light_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.offSeat)//离座检测开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->offset_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->offset_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.micSwitch)//语音识别开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->mic_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->mic_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.uploadPic)//图像上传开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->upload_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->upload_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.voiceWarn)//语音提醒开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->voicewarn_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->voicewarn_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.vibrateWarn)//震动提醒开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->vibrate_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->vibrate_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

	if(gdev_param.lightWarn)//灯光提醒开关
	{
		lv_obj_add_state(g_pt_lv_devinfo->lightwarn_switch, LV_STATE_CHECKED);/*设置开状态*/
	}
	else
	{
		lv_obj_clear_state(g_pt_lv_devinfo->lightwarn_switch, LV_STATE_CHECKED);/*设置关状态*/
	}

}


/* event_handler_back_to_home
 * 返回按键点击事件的回调函数
 * */
static void event_handler_back_to_home(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
		if(g_pt_lv_devinfo->lightwarn_switch != NULL) lv_obj_del(g_pt_lv_devinfo->lightwarn_switch);
		if(g_pt_lv_devinfo->vibrate_switch != NULL) lv_obj_del(g_pt_lv_devinfo->vibrate_switch);
		if(g_pt_lv_devinfo->voicewarn_switch != NULL) lv_obj_del(g_pt_lv_devinfo->voicewarn_switch);
		if(g_pt_lv_devinfo->upload_switch != NULL) lv_obj_del(g_pt_lv_devinfo->upload_switch);
		if(g_pt_lv_devinfo->mic_switch != NULL) lv_obj_del(g_pt_lv_devinfo->mic_switch);
		if(g_pt_lv_devinfo->offset_switch != NULL) lv_obj_del(g_pt_lv_devinfo->offset_switch);
		if(g_pt_lv_devinfo->light_switch != NULL) lv_obj_del(g_pt_lv_devinfo->light_switch);
		if(g_pt_lv_devinfo->longsit_switch != NULL) lv_obj_del(g_pt_lv_devinfo->longsit_switch);
		if(g_pt_lv_devinfo->pos_switch != NULL) lv_obj_del(g_pt_lv_devinfo->pos_switch);
		if(g_pt_lv_devinfo->volume_slider != NULL) lv_obj_del(g_pt_lv_devinfo->volume_slider);
		if(g_pt_lv_devinfo->habit_mode != NULL) lv_obj_del(g_pt_lv_devinfo->habit_mode);
		if(g_pt_lv_devinfo->normal_mode != NULL) lv_obj_del(g_pt_lv_devinfo->normal_mode);
		if(g_pt_lv_devinfo->fast_mode != NULL) lv_obj_del(g_pt_lv_devinfo->fast_mode);
		if(g_pt_lv_devinfo->bg_about != NULL) lv_obj_del(g_pt_lv_devinfo->bg_about);

		vPortFree(g_pt_lv_devinfo);
		g_run_state = 0;
		lv_main_page_demo_home(100);
	}
}

static void event_handler_pose_mode_btn(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);

	if(code == LV_EVENT_CLICKED)
	{
		if(obj == g_pt_lv_devinfo->fast_mode)
		{
			g_pose_mode = FAST_MODE;
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->fast_mode, lv_color_hex(0x0000FF), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->normal_mode, lv_color_hex(0xBEBEBE), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->habit_mode, lv_color_hex(0xBEBEBE), 0);
		}
		else if(obj == g_pt_lv_devinfo->normal_mode)
		{
			g_pose_mode = NORMAL_MODE;
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->fast_mode, lv_color_hex(0xBEBEBE), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->normal_mode, lv_color_hex(0x0000FF), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->habit_mode, lv_color_hex(0xBEBEBE), 0);
		}
		else if(obj == g_pt_lv_devinfo->habit_mode)
		{
			g_pose_mode = HABIT_MODE;
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->fast_mode, lv_color_hex(0xBEBEBE), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->normal_mode, lv_color_hex(0xBEBEBE), 0);
			lv_obj_set_style_bg_color(g_pt_lv_devinfo->habit_mode, lv_color_hex(0x0000FF), 0);
		}
	}
}

static void event_handler_volume_slider(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	uint8_t value;
	/*
	if(code == LV_EVENT_VALUE_CHANGED)
	{
		value = lv_slider_get_value(obj);
		printf("Value : %d\n", value);

	}
	*/
	if(code == LV_EVENT_RELEASED)
	{
		value = lv_slider_get_value(obj);
		value = (value + 9)/10 * 10;
		printf("Value : %d\n", value);
	}
}

static void event_handler_switch(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_VALUE_CHANGED)
	{
		printf("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On":"Off");

		if(obj == g_pt_lv_devinfo->pos_switch)//坐姿摄像头开关
		{
			gdev_param.postureCam = lv_obj_has_state(obj, LV_STATE_CHECKED);
		}
		else if(obj == g_pt_lv_devinfo->longsit_switch)//久坐提醒提醒开关
		{
			gdev_param.longSitRemind = lv_obj_has_state(obj, LV_STATE_CHECKED);
		}
		else if(obj == g_pt_lv_devinfo->light_switch)//环境光检测开关
		{
			gdev_param.lightDetecter = lv_obj_has_state(obj, LV_STATE_CHECKED);
		}
		else if(obj == g_pt_lv_devinfo->offset_switch)//离座检测开关
		{
			gdev_param.offSeat = lv_obj_has_state(obj, LV_STATE_CHECKED);
		}
		else if(obj == g_pt_lv_devinfo->mic_switch)//语音识别开关
		{
			gdev_param.micSwitch = lv_obj_has_state(obj, LV_STATE_CHECKED);
		}
		else if(obj == g_pt_lv_devinfo->upload_switch)//图像上传开关
		{
			gdev_param.uploadPic = lv_obj_has_state(obj, LV_STATE_CHECKED);
		}
		else if(obj == g_pt_lv_devinfo->voicewarn_switch)//语音提醒开关
		{
			gdev_param.voiceWarn = lv_obj_has_state(obj, LV_STATE_CHECKED);
		}
		else if(obj == g_pt_lv_devinfo->vibrate_switch)//震动提醒开关
		{
			gdev_param.vibrateWarn = lv_obj_has_state(obj, LV_STATE_CHECKED);
		}
		else if(obj == g_pt_lv_devinfo->lightwarn_switch)//灯光提醒开关
		{
			gdev_param.lightWarn = lv_obj_has_state(obj, LV_STATE_CHECKED);
		}



	}
}



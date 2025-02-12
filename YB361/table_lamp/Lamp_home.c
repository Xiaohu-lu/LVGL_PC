/*
 * lamp_main.c
 *
 *  Created on: Nov 5, 2024
 *      Author: hxd
 */
/* lvgl
 * */
#include "lvgl/lvgl.h"
#include "UI/lv_typedef.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "Lamp_main.h"
#include "Lamp_home.h"


Lamp_home_t Lamp_home_info;


/*一些样式*/
lv_style_t bg_style;
lv_style_t bg_style_round;
lv_style_t bg_style_con;
lv_style_t con_style_on;
lv_style_t con_style_off;
lv_style_t circle_style;
lv_style_t circle_style_pitched;
lv_style_t label_con_style;
lv_style_t style_line;
lv_style_t style_line_2;

/* Lamp_Style_Init
 * 用到的样式初始化
 * */
void Lamp_Style_Init(void)
{
	/*设置背景样式*/
	lv_style_init(&bg_style_round);
	lv_style_set_bg_opa(&bg_style_round, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&bg_style_round, 40);/*设置圆角*/
	lv_style_set_bg_color(&bg_style_round, lv_color_black());/*设置背景颜色,lv_color_hex()RGB888*/
	lv_style_set_pad_top(&bg_style_round, 0);
	lv_style_set_pad_bottom(&bg_style_round, 0);
	lv_style_set_pad_left(&bg_style_round, 0);
	lv_style_set_pad_right(&bg_style_round, 0);
	lv_style_set_border_width(&bg_style_round, 0);

	lv_style_init(&bg_style);
	lv_style_set_bg_opa(&bg_style, LV_OPA_100);/*设置背景透明度,100不透明,0透明*/
	lv_style_set_radius(&bg_style, 0);/*设置圆角*/
	lv_style_set_bg_color(&bg_style, lv_color_black());/*设置背景颜色,lv_color_hex()RGB888*/
	lv_style_set_pad_top(&bg_style, 0);
	lv_style_set_pad_bottom(&bg_style, 0);
	lv_style_set_pad_left(&bg_style, 0);
	lv_style_set_pad_right(&bg_style, 0);
	lv_style_set_border_width(&bg_style, 0);

	lv_style_init(&bg_style_con);
	lv_style_set_bg_opa(&bg_style_con, LV_OPA_100);/*设置背景透明度,100不透明,0透明*/
	lv_style_set_radius(&bg_style_con, 0);/*设置圆角*/
	lv_style_set_bg_color(&bg_style_con, lv_color_white());/*设置背景颜色,lv_color_hex()RGB888*/
	lv_style_set_pad_top(&bg_style_con, 0);
	lv_style_set_pad_bottom(&bg_style_con, 0);
	lv_style_set_pad_left(&bg_style_con, 0);
	lv_style_set_pad_right(&bg_style_con, 0);
	lv_style_set_border_width(&bg_style_con, 0);


	lv_style_init(&con_style_on);
	lv_style_set_bg_opa(&con_style_on, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&con_style_on, 40);/*设置圆角*/
	lv_style_set_bg_color(&con_style_on, lv_color_hex(0xffe58f));/*设置背景颜色,lv_color_hex()RGB888*/
	lv_style_set_bg_grad_color(&con_style_on, lv_color_hex(0xffd666));
	lv_style_set_bg_grad_dir(&con_style_on, LV_GRAD_DIR_HOR);
	lv_style_set_pad_top(&con_style_on, 0);
	lv_style_set_pad_bottom(&con_style_on, 0);
	lv_style_set_pad_left(&con_style_on, 0);
	lv_style_set_pad_right(&con_style_on, 0);
	lv_style_set_border_width(&con_style_on, 0);

	lv_style_init(&con_style_off);
	lv_style_set_bg_opa(&con_style_off, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&con_style_off, 40);/*设置圆角*/
	lv_style_set_bg_color(&con_style_off, lv_color_hex(0x434343));/*设置背景颜色,lv_color_hex()RGB888*/
	lv_style_set_pad_top(&con_style_off, 0);
	lv_style_set_pad_bottom(&con_style_off, 0);
	lv_style_set_pad_left(&con_style_off, 0);
	lv_style_set_pad_right(&con_style_off, 0);
	lv_style_set_border_width(&con_style_off, 0);

	lv_style_init(&label_con_style);
	lv_style_set_bg_opa(&label_con_style, LV_OPA_0);/*设置背景透明度*/
	lv_style_set_radius(&label_con_style, 0);/*设置圆角*/
	lv_style_set_pad_top(&label_con_style, 0);
	lv_style_set_pad_bottom(&label_con_style, 0);
	lv_style_set_pad_left(&label_con_style, 0);
	lv_style_set_pad_right(&label_con_style, 0);
	lv_style_set_border_width(&label_con_style, 0);

	lv_style_init(&circle_style);
	lv_style_set_bg_opa(&circle_style, LV_OPA_0);/*设置背景透明度*/
	lv_style_set_radius(&circle_style, 100);/*设置圆角*/
	lv_style_set_pad_top(&circle_style, 0);
	lv_style_set_pad_bottom(&circle_style, 0);
	lv_style_set_pad_left(&circle_style, 0);
	lv_style_set_pad_right(&circle_style, 0);
	lv_style_set_border_width(&circle_style, 1);
	lv_style_set_border_color(&circle_style, lv_color_hex(0xFFFFFF));

    lv_style_init(&circle_style_pitched);
	lv_style_set_bg_opa(&circle_style_pitched, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&circle_style_pitched, 100);/*设置圆角*/
	lv_style_set_pad_top(&circle_style_pitched, 0);
	lv_style_set_pad_bottom(&circle_style_pitched, 0);
	lv_style_set_pad_left(&circle_style_pitched, 0);
	lv_style_set_pad_right(&circle_style_pitched, 0);
	lv_style_set_border_width(&circle_style_pitched, 0);
	lv_style_set_bg_color(&circle_style_pitched, lv_color_hex(0xFFF1B8));

	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, 1);
	lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));//6b6b6b
	lv_style_set_line_rounded(&style_line, true);

	lv_style_init(&style_line_2);
	lv_style_set_line_width(&style_line_2, 1);
	lv_style_set_line_color(&style_line_2, lv_palette_main(LV_PALETTE_GREY));//6b6b6b
	lv_style_set_line_rounded(&style_line_2, true);

}

char *week_str[7] = {"星期一","星期二","星期三","星期四","星期五","星期六","星期日"};
/* Lamp_home_update_time
 * 更新home界面时间信息
 * */
void Lamp_home_update_time(uint8_t hour, uint8_t min, uint8_t week)
{
	Lamp_home_info.hour = hour;
	Lamp_home_info.minute = min;
	Lamp_home_info.week = week;
	lv_label_set_text_fmt(Lamp_home_info.label_time, "%02d:%02d", Lamp_home_info.hour, Lamp_home_info.minute);
	lv_label_set_text(Lamp_home_info.label_week, week_str[week]);
}

/* Lamp_home_init
 * 主界面初始化
 * */
void Lamp_home_init(void)
{
	Lamp_Style_Init();
	/*初始为Home界面*/
	Lamp_home_info.func_id = FUNC_HOME;

	/*创建背景*/
	Lamp_home_info.bg_img = lv_obj_create(lv_scr_act());
	lv_obj_set_size(Lamp_home_info.bg_img, LAMP_DISP_HOR_RES, LAMP_DISP_VER_RES);
	lv_obj_center(Lamp_home_info.bg_img);
	lv_obj_add_style(Lamp_home_info.bg_img, &bg_style, LV_PART_MAIN);

	/*创建时间标签*/
	Lamp_home_info.label_time = lv_label_create(Lamp_home_info.bg_img);
	lv_obj_set_style_text_font(Lamp_home_info.label_time, &Harm_san_100, LV_PART_MAIN);
	lv_obj_set_style_text_color(Lamp_home_info.label_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align(Lamp_home_info.label_time, LV_ALIGN_TOP_LEFT, 33, 70);

	/*创建日期标签*/
	lv_obj_t *con = lv_obj_create(Lamp_home_info.bg_img);
	lv_obj_add_style(con, &bg_style_round, LV_PART_MAIN);
	lv_obj_set_style_radius(con, 20, LV_PART_MAIN);
	lv_obj_set_style_bg_color(con, lv_color_hex(0x2D2D2D), LV_PART_MAIN);

	lv_obj_set_size(con, 64, 22);
	lv_obj_align(con, LV_ALIGN_TOP_LEFT, 230, 26);
	Lamp_home_info.label_week = lv_label_create(con);
	lv_obj_set_style_text_font(Lamp_home_info.label_week, &Harm_san_16, LV_PART_MAIN);
	lv_obj_set_style_text_color(Lamp_home_info.label_week, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_center(Lamp_home_info.label_week);

	Lamp_home_update_time(7, 30, 3);


	/*划线*/

	lv_obj_t *line_1 = lv_obj_create(Lamp_home_info.bg_img);
	lv_obj_set_size(line_1, 110, 2);
	lv_obj_add_style(line_1, &bg_style, LV_PART_MAIN);
	lv_obj_set_style_bg_color(line_1, lv_color_hex(0x000000), LV_PART_MAIN);
	lv_obj_set_style_bg_grad_color(line_1, lv_color_hex(0x7D7D7D), LV_PART_MAIN);
	lv_obj_set_style_bg_grad_dir(line_1, LV_GRAD_DIR_HOR, LV_PART_MAIN);
	lv_obj_align(line_1, LV_ALIGN_TOP_LEFT, 50, 178);
	lv_obj_t *line_2 = lv_obj_create(Lamp_home_info.bg_img);
	lv_obj_set_size(line_2, 110, 2);
	lv_obj_add_style(line_2, &bg_style, LV_PART_MAIN);
	lv_obj_set_style_bg_color(line_2, lv_color_hex(0x7D7D7D), LV_PART_MAIN);
	lv_obj_set_style_bg_grad_color(line_2, lv_color_hex(0x000000), LV_PART_MAIN);
	lv_obj_set_style_bg_grad_dir(line_2, LV_GRAD_DIR_HOR, LV_PART_MAIN);
	lv_obj_align(line_2, LV_ALIGN_TOP_LEFT, 160, 178);

}

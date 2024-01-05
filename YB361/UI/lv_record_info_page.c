/*
 * lv_record_info_page.c
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

 static PT_lv_meter g_pt_lv_meter;  // 数据结构体

 static uint8_t gmeter_index = 0;

 static void lv_record_info_init(void);
 static void event_handler_back_to_home(lv_event_t *e);
 static void draw_part_event_cb(lv_event_t *e);
/* lv_record
 * 坐姿记录详细信息界面
 * */
void lv_record_info(void)
{

	g_pt_lv_meter = (T_lv_meter*)pvPortMalloc(sizeof(T_lv_meter));
	g_pt_lv_meter->bg_about = lv_obj_create(lv_scr_act());/*创建背景*/

	lv_obj_set_size(g_pt_lv_meter->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_meter->bg_about, 0, 0);/*设置圆角*/
	lv_obj_align(g_pt_lv_meter->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*设置padding*/
	set_user_style(g_pt_lv_meter->bg_about);

	lv_obj_set_size(g_pt_lv_meter->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_meter->bg_about, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_meter->bg_about, lv_color_hex(BACK_COLOR), 0);
	lv_record_info_init();/*界面初始化*/

	add_title(g_pt_lv_meter->bg_about, (const char *)gdev_param.bind_devId);
	add_back(g_pt_lv_meter->bg_about, event_handler_back_to_home);

}

/* lv_record_info_init
 * 记录详细信息界面初始化
 */
static void lv_record_info_init(void)
{
    gmeter_index = get_record_index();
    /*创建表格*/
    lv_obj_t *table;
    g_pt_lv_meter->about_table = lv_table_create(g_pt_lv_meter->bg_about);
    table = g_pt_lv_meter->about_table;
    set_user_style(table);
    lv_table_set_row_cnt(table, 6);/*设置行数*/
    lv_table_set_col_cnt(table, 2);/*设置列数*/
    lv_table_set_col_width(table, 0, LV_METER_TABLE_COL_WIDTH);/*设置第0列宽度*/
    lv_table_set_col_width(table, 1, LV_METER_TABLE_COL_WIDTH);/*设置第1列宽度*/
    lv_obj_align(table, LV_ALIGN_TOP_MID, 0, LV_METER_US_TITLE_SPACE);/*设置位置,在他父对象的顶部中间*/
    lv_obj_set_style_bg_color(table, lv_color_hex(BACK_COLOR), 0);

    /*填充表格*/
	lv_table_set_cell_value(g_pt_lv_meter->about_table, 0, 0, "startTime");/*设置单元格内容*/
	lv_table_set_cell_value(g_pt_lv_meter->about_table, 1, 0, "endTime");
	lv_table_set_cell_value(g_pt_lv_meter->about_table, 2, 0, "duration");
	lv_table_set_cell_value(g_pt_lv_meter->about_table, 3, 0, "regularErr");
	lv_table_set_cell_value(g_pt_lv_meter->about_table, 4, 0, "headLow");
	lv_table_set_cell_value(g_pt_lv_meter->about_table, 5, 0, "headTilt");

	lv_table_set_cell_value(g_pt_lv_meter->about_table, 0, 1, (const char*)gdev_param.record_list.record[gmeter_index].start_time);/*设置单元格内容*/
	lv_table_set_cell_value(g_pt_lv_meter->about_table, 1, 1, (const char*)gdev_param.record_list.record[gmeter_index].end_time);
	lv_table_set_cell_value_fmt(g_pt_lv_meter->about_table, 2, 1,"%d", gdev_param.record_list.record[gmeter_index].duration);
	lv_table_set_cell_value_fmt(g_pt_lv_meter->about_table, 3, 1,"%d", gdev_param.record_list.record[gmeter_index].regularErr);
	lv_table_set_cell_value_fmt(g_pt_lv_meter->about_table, 4, 1,"%d", gdev_param.record_list.record[gmeter_index].headLow);
	lv_table_set_cell_value_fmt(g_pt_lv_meter->about_table, 5, 1,"%d", gdev_param.record_list.record[gmeter_index].headTilt);

    lv_obj_set_height(table, LV_METER_TABLE_HEIGHT);
    /*添加绘制开始时的回调函数*/
    lv_obj_add_event_cb(table, draw_part_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);


    /*创建饼图*/
    lv_obj_t *meter;
    g_pt_lv_meter->meter = lv_meter_create(g_pt_lv_meter->bg_about);
    meter = g_pt_lv_meter->meter;
	/*remove the background and the circle from the middle*/
	lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
	lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);

	lv_obj_set_size(meter, LV_METER_SIZE, LV_METER_SIZE);
	//lv_obj_center(meter);
    lv_obj_align(meter, LV_ALIGN_BOTTOM_MID, -20, 0);
	/*add a scale first with no ticks*/
	lv_meter_scale_t *scale = lv_meter_add_scale(meter);
	lv_meter_set_scale_ticks(meter, scale, 0, 0, 0, lv_color_black());
	lv_meter_set_scale_range(meter, scale, 0, 100, 360, 0);

	/*add a three arc indicator*/
	lv_coord_t indic_w = 100;
	uint16_t duration_time = gdev_param.record_list.record[gmeter_index].duration;
	uint16_t regularErr_time = gdev_param.record_list.record[gmeter_index].regularErr;
	uint16_t headLow_time = gdev_param.record_list.record[gmeter_index].headLow;
	uint16_t headTilt_time = gdev_param.record_list.record[gmeter_index].headTilt;
	uint16_t good_time = duration_time - regularErr_time - headLow_time - headTilt_time;
    uint8_t start_value;
    uint8_t end_value;
	/*良好坐姿占比 绿色*/
	start_value = 0;
	end_value = start_value + ((good_time * 100) / duration_time);
	lv_meter_indicator_t *indic1 = lv_meter_add_arc(meter, scale, indic_w, lv_palette_main(LV_PALETTE_GREEN), 0);
	lv_meter_set_indicator_start_value(meter, indic1, 0);
	lv_meter_set_indicator_end_value(meter, indic1, end_value);

	/*常规不良坐姿占比 橙色*/
	start_value = end_value;
	end_value = start_value + (regularErr_time * 100) / duration_time;
	lv_meter_indicator_t *indic2 = lv_meter_add_arc(meter, scale, indic_w, lv_palette_main(LV_PALETTE_ORANGE), 0);
	lv_meter_set_indicator_start_value(meter, indic2, start_value);
	lv_meter_set_indicator_end_value(meter, indic2, end_value);

    /*低头坐姿占比 蓝色*/
    start_value = end_value;
	end_value = start_value + (headLow_time * 100) / duration_time;
	lv_meter_indicator_t *indic3 = lv_meter_add_arc(meter, scale, indic_w, lv_palette_main(LV_PALETTE_BLUE), 0);
	lv_meter_set_indicator_start_value(meter, indic3, start_value);
	lv_meter_set_indicator_end_value(meter, indic3, end_value);

	/*歪头坐姿占比 黄色*/
    start_value = end_value;
	end_value = 100;
	lv_meter_indicator_t *indic4 = lv_meter_add_arc(meter, scale, indic_w, lv_palette_main(LV_PALETTE_YELLOW), 0);
	lv_meter_set_indicator_start_value(meter, indic4, start_value);
	lv_meter_set_indicator_end_value(meter, indic4, end_value);

    /*创建容器类型*/
    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 0);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(BACK_COLOR));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
	lv_style_set_border_width(&style_cont, 0);

	/*创建一个小容器类型*/
	static lv_style_t style_cont_s;/*容器样式*/
	lv_style_init(&style_cont_s);
	lv_style_set_bg_opa(&style_cont_s, LV_OPA_80);/*设置背景透明度*/
	lv_style_set_radius(&style_cont_s, 0);	/*设置圆角*/
	//lv_style_set_bg_color(&style_cont_s, lv_color_hex(BACK_COLOR));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont_s, 0);
	lv_style_set_pad_bottom(&style_cont_s, 0);
	lv_style_set_pad_left(&style_cont_s, 0);
	lv_style_set_pad_right(&style_cont_s, 0);
	lv_style_set_border_width(&style_cont_s, 1);

	/*创建良好容器*/
    lv_obj_t *b_cont1 = lv_obj_create(g_pt_lv_meter->bg_about);
    lv_obj_add_style(b_cont1, &style_cont, 0);/*添加默认样式*/
    lv_obj_set_size(b_cont1, LV_BIG_CONT_W, LV_BIG_CONT_H);
    lv_obj_align_to(b_cont1, g_pt_lv_meter->about_table, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 5);
    lv_obj_t *label_1 = lv_label_create(b_cont1);
    lv_label_set_text(label_1, "良好");/*设置text*/
	lv_obj_set_style_text_color(label_1, lv_color_hex(0x0), 0);/*设置字体颜色*/
	lv_obj_set_style_text_font(label_1, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_1, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_fade_in(label_1, 0, 0);
	/*创建小容器*/
	lv_obj_t *l_cont1 = lv_obj_create(b_cont1);
	lv_obj_add_style(l_cont1, &style_cont_s, 0);/*添加默认样式*/
    lv_obj_set_size(l_cont1, LV_LIT_CONT_W, LV_LIT_CONT_H);
    lv_obj_set_style_bg_color(l_cont1, lv_palette_main(LV_PALETTE_GREEN), 0);/*设置背景颜色*/
    lv_obj_align(l_cont1, LV_ALIGN_RIGHT_MID, 0, 0);

    /*创建不良容器*/
    lv_obj_t *b_cont2 = lv_obj_create(g_pt_lv_meter->bg_about);
    lv_obj_add_style(b_cont2, &style_cont, 0);/*添加默认样式*/
    lv_obj_set_size(b_cont2, LV_BIG_CONT_W, LV_BIG_CONT_H);
    lv_obj_align_to(b_cont2, b_cont1, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 3);
    lv_obj_t *label_2 = lv_label_create(b_cont2);
    lv_label_set_text(label_2, "不良");/*设置text*/
	lv_obj_set_style_text_color(label_2, lv_color_hex(0x0), 0);/*设置字体颜色*/
	lv_obj_set_style_text_font(label_2, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_2, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_fade_in(label_2, 0, 0);
	/*创建小容器*/
	lv_obj_t *l_cont2 = lv_obj_create(b_cont2);
	lv_obj_add_style(l_cont2, &style_cont_s, 0);/*添加默认样式*/
    lv_obj_set_size(l_cont2, LV_LIT_CONT_W, LV_LIT_CONT_H);
    lv_obj_set_style_bg_color(l_cont2, lv_palette_main(LV_PALETTE_ORANGE), 0);/*设置背景颜色*/
    lv_obj_align(l_cont2, LV_ALIGN_RIGHT_MID, 0, 0);


    /*创建低头容器*/
    lv_obj_t *b_cont3 = lv_obj_create(g_pt_lv_meter->bg_about);
    lv_obj_add_style(b_cont3, &style_cont, 0);/*添加默认样式*/
    lv_obj_set_size(b_cont3, LV_BIG_CONT_W, LV_BIG_CONT_H);
    lv_obj_align_to(b_cont3, b_cont2, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 3);
    lv_obj_t *label_3 = lv_label_create(b_cont3);
    lv_label_set_text(label_3, "低头");/*设置text*/
	lv_obj_set_style_text_color(label_3, lv_color_hex(0x0), 0);/*设置字体颜色*/
	lv_obj_set_style_text_font(label_3, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_3, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_fade_in(label_3, 0, 0);
	/*创建小容器*/
	lv_obj_t *l_cont3 = lv_obj_create(b_cont3);
	lv_obj_add_style(l_cont3, &style_cont_s, 0);/*添加默认样式*/
    lv_obj_set_size(l_cont3, LV_LIT_CONT_W, LV_LIT_CONT_H);
    lv_obj_set_style_bg_color(l_cont3, lv_palette_main(LV_PALETTE_BLUE), 0);/*设置背景颜色*/
    lv_obj_align(l_cont3, LV_ALIGN_RIGHT_MID, 0, 0);


    /*创建歪头容器*/
    lv_obj_t *b_cont4 = lv_obj_create(g_pt_lv_meter->bg_about);
    lv_obj_add_style(b_cont4, &style_cont, 0);/*添加默认样式*/
    lv_obj_set_size(b_cont4, LV_BIG_CONT_W, LV_BIG_CONT_H);
    lv_obj_align_to(b_cont4, b_cont3, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 3);
    lv_obj_t *label_4 = lv_label_create(b_cont4);
    lv_label_set_text(label_4, "歪头");/*设置text*/
	lv_obj_set_style_text_color(label_4, lv_color_hex(0x0), 0);/*设置字体颜色*/
	lv_obj_set_style_text_font(label_4, &song_font14, 0);/*默认状态下的字体样式*/
	lv_obj_align(label_4, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_fade_in(label_4, 0, 0);
	/*创建小容器*/
	lv_obj_t *l_cont4 = lv_obj_create(b_cont4);
	lv_obj_add_style(l_cont4, &style_cont_s, 0);/*添加默认样式*/
    lv_obj_set_size(l_cont4, LV_LIT_CONT_W, LV_LIT_CONT_H);
    lv_obj_set_style_bg_color(l_cont4, lv_palette_main(LV_PALETTE_YELLOW), 0);/*设置背景颜色*/
    lv_obj_align(l_cont4, LV_ALIGN_RIGHT_MID, 0, 0);


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
	    if(g_pt_lv_meter->about_table != NULL) lv_obj_del(g_pt_lv_meter->about_table);
		if(g_pt_lv_meter->bg_about != NULL) lv_obj_del(g_pt_lv_meter->bg_about);

		vPortFree(g_pt_lv_meter);

		lv_record();
		//lv_main_page_demo_home(100);
	}
}

/* draw_part_event_cb
 * 开始绘制表格事件的回调函数
 */
static void draw_part_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_param(e);

    /*if the cells are drawn...*/
    if(dsc->part == LV_PART_ITEMS)
    {
        uint32_t row = dsc->id / lv_table_get_col_cnt(obj);
        uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);

        /*设置背景*/
        dsc->rect_dsc->bg_color = lv_color_hex(BACK_COLOR);
        /*In the first column align the texts to the right*/
        if(col == 0)
        {

            dsc->label_dsc->align = LV_TEXT_ALIGN_LEFT;
        }
        else if(col == 1)
        {
            dsc->label_dsc->align = LV_TEXT_ALIGN_RIGHT;

        }

    }


}

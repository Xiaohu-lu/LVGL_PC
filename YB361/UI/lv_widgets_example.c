/*
 * lv_widgets_example.c
 *
 *  Created on: Dec 12, 2023
 *      Author: hxd
 */

#include "UI/lv_typedef.h"
/* lvgl
 * */
#include "lvgl/lvgl.h"



/*line chart*/
void lv_example_chart_1(void)
{
	lv_obj_t *chart;
	chart = lv_chart_create(lv_scr_act());
	lv_obj_set_size(chart, 200, 150);
	lv_obj_center(chart);
	lv_chart_set_type(chart, LV_CHART_TYPE_LINE);/*show lines and points too*/

	/*add tow data series*/
	lv_chart_series_t *ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
	lv_chart_series_t *ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_SECONDARY_Y);

	/*set the next points on "ser1"*/
	lv_chart_set_next_value(chart, ser1, 10);
	lv_chart_set_next_value(chart, ser1, 10);
	lv_chart_set_next_value(chart, ser1, 10);
	lv_chart_set_next_value(chart, ser1, 10);
	lv_chart_set_next_value(chart, ser1, 10);
	lv_chart_set_next_value(chart, ser1, 10);
	lv_chart_set_next_value(chart, ser1, 10);
	lv_chart_set_next_value(chart, ser1, 30);
	lv_chart_set_next_value(chart, ser1, 70);
	lv_chart_set_next_value(chart, ser1, 90);

	/*directly set points on "ser2"*/
	ser2->y_points[0] = 90;
	ser2->y_points[1] = 70;
	ser2->y_points[2] = 65;
	ser2->y_points[3] = 65;
	ser2->y_points[4] = 65;
	ser2->y_points[5] = 65;
	ser2->y_points[6] = 65;
	ser2->y_points[7] = 65;
	ser2->y_points[8] = 65;
	ser2->y_points[9] = 65;
	lv_chart_refresh(chart);
}

static lv_obj_t *chart1;
static lv_chart_series_t *ser1;
static lv_chart_series_t *ser2;

static void draw_event_cb(lv_event_t *e)
{
	lv_obj_t *obj = lv_event_get_target(e);

	/*add the faded area before the lines are drawn*/
	lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
	if(dsc->part == LV_PART_ITEMS)
	{
		if(!dsc->p1 || !dsc->p2)
			return;

		/*add a line mask that keeps the area below the line*/
		lv_draw_mask_line_param_t line_mask_param;
		lv_draw_mask_line_points_init(&line_mask_param, dsc->p1->x, dsc->p1->y, dsc->p2->x, dsc->p2->y, LV_DRAW_MASK_LINE_SIDE_BOTTOM);
		int16_t line_mask_id = lv_draw_mask_add(&line_mask_param, NULL);

		/*add a fade effect. transparent bottom covering top*/
		lv_coord_t h = lv_obj_get_height(obj);
		lv_draw_mask_fade_param_t fade_mask_param;
		lv_draw_mask_fade_init(&fade_mask_param, &obj->coords, LV_OPA_COVER, obj->coords.y1 + h / 8, LV_OPA_TRANSP, obj->coords.y2);
		int16_t fade_mask_id = lv_draw_mask_add(&fade_mask_param, NULL);

		/*Draw a rectangle that will be affected by the mask*/
		lv_draw_rect_dsc_t draw_rect_dsc;
		lv_draw_rect_dsc_init(&draw_rect_dsc);
		draw_rect_dsc.bg_opa = LV_OPA_20;
		draw_rect_dsc.bg_color = dsc->line_dsc->color;

		lv_area_t a;
		a.x1 = dsc->p1->x;
		a.x2 = dsc->p2->x - 1;
		a.y1 = LV_MIN(dsc->p1->y, dsc->p2->y);
		a.y2 = obj->coords.y2;
		lv_draw_rect(dsc->draw_ctx, &draw_rect_dsc, &a);

		/*remove the masks*/
		lv_draw_mask_free_param(&line_mask_param);
		lv_draw_mask_free_param(&fade_mask_param);
		lv_draw_mask_remove_id(line_mask_id);
		lv_draw_mask_remove_id(fade_mask_id);

	}

	/*Hook the division lines too*/
	else if(dsc->part == LV_PART_MAIN)
	{
		if(dsc->line_dsc == NULL || dsc->p1 == NULL || dsc->p2 == NULL)
			return;
		/*vertical line*/
		if(dsc->p1->x == dsc->p2->x)
		{
			dsc->line_dsc->color = lv_palette_lighten(LV_PALETTE_GREY, 1);
			if(dsc->id == 3)
			{
				dsc->line_dsc->width = 2;
				dsc->line_dsc->dash_gap = 0;
				dsc->line_dsc->dash_width = 0;
			}
			else
			{
				dsc->line_dsc->width = 1;
				dsc->line_dsc->dash_gap = 6;
				dsc->line_dsc->dash_width = 6;
			}


		}
		/*horizontal line*/
		else
		{
			if(dsc->id == 2)
			{
				dsc->line_dsc->width = 2;
				dsc->line_dsc->dash_gap = 0;
				dsc->line_dsc->dash_width = 0;
			}
			else
			{
				dsc->line_dsc->width = 2;
				dsc->line_dsc->dash_gap = 6;
				dsc->line_dsc->dash_width = 6;
			}
		}

		if(dsc->id == 1 || dsc->id == 3)
		{
			dsc->line_dsc->color = lv_palette_main(LV_PALETTE_GREEN);
		}
		else
		{
			dsc->line_dsc->color = lv_palette_lighten(LV_PALETTE_BLUE_GREY, 1);
		}
	}

}

static void add_data(lv_timer_t *timer)
{
	LV_UNUSED(timer);
	static uint32_t cnt = 0;
	lv_chart_set_next_value(chart1, ser1, lv_rand(20, 90));
	if(cnt % 4 == 0)
		lv_chart_set_next_value(chart1, ser2, lv_rand(40, 60));
	cnt++;
}


void lv_example_chart_2(void)
{
	/*create a chart1*/
	chart1 = lv_chart_create(lv_scr_act());
	lv_obj_set_size(chart1, 200, 150);
	lv_obj_center(chart1);
	lv_chart_set_type(chart1, LV_CHART_TYPE_LINE);

	lv_chart_set_div_line_count(chart1, 5, 7);

	lv_obj_add_event_cb(chart1, draw_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
	lv_chart_set_update_mode(chart1, LV_CHART_UPDATE_MODE_CIRCULAR);

	/*add two data series*/
	ser1 = lv_chart_add_series(chart1, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
	ser2 = lv_chart_add_series(chart1, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_SECONDARY_Y);

	uint32_t i;
	for(i = 0; i < 10; i++)
	{
		lv_chart_set_next_value(chart1, ser1, lv_rand(20, 90));
		lv_chart_set_next_value(chart1, ser2, lv_rand(30, 70));

	}
	lv_timer_create(add_data, 200, NULL);
}

static void draw_event_cb2(lv_event_t *e)
{
	lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
	if(!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_TICK_LABEL))
		return;
	if(dsc->id == LV_CHART_AXIS_PRIMARY_X && dsc->text)
	{
		const char *month[] = {"Jan", "Febr", "March", "Apr", "May", "Jun", "July", "Aug", "Sept", "Oct",
			"Nov", "Dec"};
		lv_snprintf(dsc->text, dsc->text_length, "%s", month[dsc->value]);
	}
}

void lv_example_chart_3(void)
{
	/*create a chart*/
	lv_obj_t *chart;
	chart = lv_chart_create(lv_scr_act());
	lv_obj_set_size(chart, 200, 150);
	lv_obj_center(chart);
	lv_chart_set_type(chart, LV_CHART_TYPE_BAR);
	lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
	lv_chart_set_range(chart, LV_CHART_AXIS_SECONDARY_Y, 0, 400);

	lv_chart_set_point_count(chart, 12);
	lv_obj_add_event_cb(chart, draw_event_cb2, LV_EVENT_DRAW_PART_BEGIN, NULL);

	/*add ticks and label to every axis*/
	lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 12, 3, true, 40);
	lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 2, true, 50);
	lv_chart_set_axis_tick(chart, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 3, 4, true, 50);

	/*zoom in a little in X*/
	lv_chart_set_zoom_x(chart, 800);

	/*add two data series*/
	lv_chart_series_t *ser1 = lv_chart_add_series(chart, lv_palette_lighten(LV_PALETTE_GREEN, 2), LV_CHART_AXIS_PRIMARY_Y);
	lv_chart_series_t *ser2 = lv_chart_add_series(chart, lv_palette_darken(LV_PALETTE_GREEN, 2), LV_CHART_AXIS_SECONDARY_Y);

	/*Set the next points on "ser1"*/
	lv_chart_set_next_value(chart, ser1, 31);
	lv_chart_set_next_value(chart, ser1, 66);
	lv_chart_set_next_value(chart, ser1, 10);
	lv_chart_set_next_value(chart, ser1, 89);
	lv_chart_set_next_value(chart, ser1, 63);
	lv_chart_set_next_value(chart, ser1, 56);
	lv_chart_set_next_value(chart, ser1, 32);
	lv_chart_set_next_value(chart, ser1, 35);
	lv_chart_set_next_value(chart, ser1, 57);
	lv_chart_set_next_value(chart, ser1, 85);
	lv_chart_set_next_value(chart, ser1, 22);
	lv_chart_set_next_value(chart, ser1, 58);

	lv_coord_t *ser2_array = lv_chart_get_y_array(chart, ser2);
	/*Directly set points on "ser2"*/
	ser2_array[0] = 92;
	ser2_array[1] = 71;
	ser2_array[2] = 61;
	ser2_array[3] = 15;
	ser2_array[4] = 21;
	ser2_array[5] = 35;
	ser2_array[6] = 35;
	ser2_array[7] = 58;
	ser2_array[8] = 31;
	ser2_array[9] = 53;
	ser2_array[10] = 33;
	ser2_array[11] = 73;

	lv_chart_refresh(chart);

}



/*calendar*/
static void event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_VALUE_CHANGED)
	{
		lv_calendar_date_t date;
		if(lv_calendar_get_pressed_date(obj, &date))
		{
			LOG_D("Clicked date: %02d.%02d.%d", date.day, date.month, date.year);
		}
	}
}

void lv_example_calendar_1(void)
{
	lv_obj_t *calendar = lv_calendar_create(lv_scr_act());
	lv_obj_set_size(calendar, 185, 185);
	lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 27);
	lv_obj_add_event_cb(calendar, event_handler, LV_EVENT_ALL, NULL);

	lv_calendar_set_today_date(calendar, 2021, 02, 23);
	lv_calendar_set_showed_date(calendar, 2021, 02);

	static lv_calendar_date_t highlighted_days[3];
	highlighted_days[0].year = 2021;
	highlighted_days[0].month = 02;
	highlighted_days[0].day = 6;

	highlighted_days[1].year = 2021;
	highlighted_days[1].month = 02;
	highlighted_days[1].day = 11;

	highlighted_days[2].year = 2022;
	highlighted_days[2].month = 02;
	highlighted_days[3].day = 22;

	lv_calendar_set_highlighted_dates(calendar, highlighted_days, 3);

#if LV_USE_CALENDAR_HEADER_DROPDOWN
	lv_calendar_header_dropdown_create(calendar);
#elif LV_USE_CALENDAR_HEADER_ARROW
	lv_calendar_header_arrow_create(calendar);
#endif
	lv_calendar_set_showed_date(calendar, 2021, 10);

}




/*meter*/
void lv_example_meter_4(void)
{
	lv_obj_t *meter = lv_meter_create(lv_scr_act());

	/*remove the background and the circle from the middle*/
	lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
	lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);

	lv_obj_set_size(meter, 200, 200);
	lv_obj_center(meter);

	/*add a scale first with no ticks*/
	lv_meter_scale_t *scale = lv_meter_add_scale(meter);
	lv_meter_set_scale_ticks(meter, scale, 0, 0, 0, lv_color_black());
	lv_meter_set_scale_range(meter, scale, 0, 100, 360, 0);

	/*add a three arc indicator*/
	lv_coord_t indic_w = 100;
	lv_meter_indicator_t *indic1 = lv_meter_add_arc(meter, scale, indic_w, lv_palette_main(LV_PALETTE_ORANGE), 0);
	lv_meter_set_indicator_start_value(meter, indic1, 0);
	lv_meter_set_indicator_end_value(meter, indic1, 40);

	lv_meter_indicator_t *indic2 = lv_meter_add_arc(meter, scale, indic_w, lv_palette_main(LV_PALETTE_YELLOW), 0);
	lv_meter_set_indicator_start_value(meter, indic2, 40);
	lv_meter_set_indicator_end_value(meter, indic2, 80);

	lv_meter_indicator_t *indic3 = lv_meter_add_arc(meter, scale, indic_w, lv_palette_main(LV_PALETTE_DEEP_ORANGE), 0);
	lv_meter_set_indicator_start_value(meter, indic3, 80);
	lv_meter_set_indicator_end_value(meter, indic3, 100);

}

/*menu*/
void lv_example_menu_1(void)
{
	/*create a menu object*/
	lv_obj_t *menu = lv_menu_create(lv_scr_act());
	lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
	lv_obj_center(menu);

	lv_obj_t *cont;
	lv_obj_t *label;

	/*create a sub page*/
	lv_obj_t *sub_page = lv_menu_page_create(menu, NULL);

	cont = lv_menu_cont_create(sub_page);
	label = lv_label_create(cont);
	lv_label_set_text(label, "Hello, I am hiding here");

	/*create a main page*/
	lv_obj_t *main_page = lv_menu_page_create(menu, NULL);

	cont = lv_menu_cont_create(main_page);
	label = lv_label_create(cont);
	lv_label_set_text(label, "Item 1");

	cont = lv_menu_cont_create(main_page);
	label = lv_label_create(cont);
	lv_label_set_text(label, "Item 2");

	cont = lv_menu_cont_create(main_page);
	label = lv_label_create(cont);
	lv_label_set_text(label, "Item 3(click me!)");
	lv_menu_set_load_page_event(menu, cont, sub_page);
	lv_menu_set_page(menu, main_page);


}

static void back_event_handler(lv_event_t *e)
{
	lv_obj_t *obj = lv_event_get_target(e);
	lv_obj_t *menu = lv_event_get_user_data(e);

	if(lv_menu_back_btn_is_root(menu, obj))
	{
		lv_obj_t *mbox1 = lv_msgbox_create(NULL, "Hello", "Root back btn click.", NULL, true);
		lv_obj_center(mbox1);
	}
}


void lv_example_menu_2(void)
{
	lv_obj_t *menu = lv_menu_create(lv_scr_act());
	lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
	lv_obj_add_event_cb(menu, back_event_handler, LV_EVENT_CLICKED, menu);
	lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
	lv_obj_center(menu);

	lv_obj_t *cont;
	lv_obj_t *label;

	/*create a sub page*/
	lv_obj_t *sub_page = lv_menu_page_create(menu, NULL);

	cont = lv_menu_cont_create(sub_page);
	label = lv_label_create(cont);
	lv_label_set_text(label, "Hello, I am hiding here");

	/*create a main page*/
	lv_obj_t *main_page = lv_menu_page_create(menu, NULL);

	cont = lv_menu_cont_create(main_page);
	label = lv_label_create(cont);
	lv_label_set_text(label, "Item 1");

	cont = lv_menu_cont_create(main_page);
	label = lv_label_create(cont);
	lv_label_set_text(label, "Item 2");

	cont = lv_menu_cont_create(main_page);
	label = lv_label_create(cont);
	lv_label_set_text(label, "Item 3 (click me!)");
	lv_menu_set_load_page_event(menu, cont, sub_page);

	lv_menu_set_page(menu, main_page);

}




/*img*/

void lv_example_img_1(void)
{
	LV_IMG_DECLARE(img_lv_100ask_bg);
	lv_obj_t *img1 = lv_img_create(lv_scr_act());
	lv_img_set_src(img1, &img_lv_100ask_bg);
	lv_obj_align(img1, LV_ALIGN_CENTER, 0, -20);
	lv_obj_set_size(img1, 200, 200);

	lv_obj_t *img2 = lv_img_create(lv_scr_act());
	lv_img_set_src(img2, LV_SYMBOL_OK"Accept");
	lv_obj_align_to(img2, img1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
}



/*label*/
void lv_example_label_1(void)
{
	lv_obj_t *label1 = lv_label_create(lv_scr_act());
	/*break the long lines*/
	lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);
	/*enable re-coloring by commands in the text*/
	lv_label_set_recolor(label1, true);

	lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label,align the lines to the center"
			" and wrap long text automatically");
	/*set smaller width to make the lines wrap*/
	lv_obj_set_width(label1, 150);
	lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);

	lv_obj_t *label2 = lv_label_create(lv_scr_act());
	lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR);

	lv_obj_set_width(label2, 150);
	lv_label_set_text(label2, "It is a circularly scrolling text");
	lv_obj_align(label2, LV_ALIGN_CENTER, 0, 40);


}

void lv_example_label_2(void)
{
	/*create a style for the shadow*/
	static lv_style_t style_shadow;
	lv_style_init(&style_shadow);
	lv_style_set_text_opa(&style_shadow, LV_OPA_30);
	lv_style_set_text_color(&style_shadow, lv_color_black());

	/*create a label for the shadow first (it's in the background)*/
	lv_obj_t *shadow_label = lv_label_create(lv_scr_act());
	lv_obj_add_style(shadow_label, &style_shadow, 0);

	/*create the main label*/
	lv_obj_t *main_label = lv_label_create(lv_scr_act());
	lv_label_set_text(main_label, "A simple method to create\n"
									"shadows on a text.\n"
									"It even works with\n\n"
									"newlines and spaces.");
	/*set the same text for the shadow label*/
	lv_label_set_text(shadow_label, lv_label_get_text(main_label));

	/*position the main label*/
	lv_obj_align(main_label, LV_ALIGN_CENTER, 0, 0);

	/*shift the second label down and to the right by 2 pixel*/
	lv_obj_align_to(shadow_label, main_label, LV_ALIGN_TOP_LEFT, 2, 2);

}

void lv_example_label_3(void)
{
	lv_obj_t *ltr_label = lv_label_create(lv_scr_act());
	lv_label_set_text(ltr_label, "In modern terminology, a microcontroller is similar to a system on a chip (SoC).");
	lv_obj_set_style_text_font(ltr_label, &lv_font_montserrat_14, 0);
	lv_obj_set_width(ltr_label, 310);
	lv_obj_align(ltr_label, LV_ALIGN_TOP_LEFT, 5, 5);

	lv_obj_t *rtl_label = lv_label_create(lv_scr_act());

	lv_label_set_text(rtl_label, "CPU - Central Processing Unit).");
	lv_obj_set_style_base_dir(rtl_label, LV_BASE_DIR_RTL, 0);
	lv_obj_set_style_text_font(rtl_label, &lv_font_simsun_16_cjk, 0);
	lv_obj_set_width(rtl_label, 310);
	lv_obj_align(rtl_label, LV_ALIGN_LEFT_MID, 5, 0);

	lv_obj_t *cz_label = lv_label_create(lv_scr_act());
	lv_label_set_text(cz_label, "嵌入式系统(Embedded System), \n是一种嵌入式机械或电气系统内部、具有专一功能和实时计算性能的计算机系统。");
	//lv_obj_set_style_text_font(cz_label, &lv_font_simsun_16_cjk, 0);
	lv_obj_set_width(cz_label, 310);
	lv_obj_align(cz_label, LV_ALIGN_BOTTOM_LEFT, 5, -5);
}



/*animimg*/




/*table*/

static void draw_part_event_cb(lv_event_t *e)
{
	lv_obj_t *obj = lv_event_get_target(e);
	lv_obj_draw_part_dsc_t *dsc = lv_event_get_param(e);
	/*If the cells are drawn...*/
	if(dsc->part == LV_PART_ITEMS)
	{
		uint32_t row = dsc->id / lv_table_get_col_cnt(obj);
		uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);

		/*make the texts in the first cell center aligned*/
		if(row == 0)
		{
			dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
			dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_BLUE), dsc->rect_dsc->bg_color, LV_OPA_20);

		}
		/*In the first column align the texts to the right*/
		else if(col == 0)
		{
			dsc->label_dsc->align = LV_TEXT_ALIGN_RIGHT;
		}
		/*MAke every 2nd row grayish*/
		if((row != 0 && row % 2) == 0)
		{
			dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_GREY), dsc->rect_dsc->bg_color, LV_OPA_10);
			dsc->rect_dsc->bg_opa = LV_OPA_COVER;
		}
	}
}

void lv_example_table_1(void)
{
	lv_obj_t *table = lv_table_create(lv_scr_act());

	/*fill the first column*/
	lv_table_set_cell_value(table, 0, 0, "Name");
	lv_table_set_cell_value(table, 1, 0, "Apple");
	lv_table_set_cell_value(table, 2, 0, "Banana");
	lv_table_set_cell_value(table, 3, 0, "Lemon");
	lv_table_set_cell_value(table, 4, 0, "Grape");
	lv_table_set_cell_value(table, 5, 0, "Melon");
	lv_table_set_cell_value(table, 6, 0, "Peach");
	lv_table_set_cell_value(table, 7, 0, "Nuts");

	/*fill the second column*/
	lv_table_set_cell_value(table, 0, 1, "Price");
	lv_table_set_cell_value(table, 1, 1, "$7");
	lv_table_set_cell_value(table, 2, 1, "$4");
	lv_table_set_cell_value(table, 3, 1, "$6");
	lv_table_set_cell_value(table, 4, 1, "$2");
	lv_table_set_cell_value(table, 5, 1, "$5");
	lv_table_set_cell_value(table, 6, 1, "$1");
	lv_table_set_cell_value(table, 7, 1, "$9");

	/*set a smaller height to the table, It'll make it scrollable*/
	lv_obj_set_height(table, 200);
	lv_obj_center(table);

	/*add an event callback to to apply some custom drawing*/
	lv_obj_add_event_cb(table, draw_part_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
}

#define ITEM_CNT 200
static void draw_event_cb1(lv_event_t *e)
{
	lv_obj_t *obj = lv_event_get_target(e);
	lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
	/*If the cells are drawn...*/
	if(dsc->part == LV_PART_ITEMS)
	{
		bool chk = lv_table_has_cell_ctrl(obj, dsc->id, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
		lv_draw_rect_dsc_t rect_dsc;
		lv_draw_rect_dsc_init(&rect_dsc);
		rect_dsc.bg_color = chk ? lv_theme_get_color_primary(obj) : lv_palette_lighten(LV_PALETTE_GREY, 2);
		rect_dsc.radius = LV_RADIUS_CIRCLE;

		lv_area_t sw_area;
		sw_area.x1 = dsc->draw_area->x2 - 50;
		sw_area.x2 = sw_area.x1 + 40;
		sw_area.y1 = dsc->draw_area->y1 + lv_area_get_height(dsc->draw_area) / 2 - 10;
		sw_area.y2 = sw_area.y1 + 20;
		lv_draw_rect(dsc->draw_ctx, &rect_dsc, &sw_area);

		rect_dsc.bg_color = lv_color_white();
		if(chk)
		{
			sw_area.x2 -= 2;
			sw_area.x1 = sw_area.x2 - 16;
		}
		else
		{
			sw_area.x1 += 2;
			sw_area.x2 = sw_area.x1 + 16;
		}
		sw_area.y1 += 2;
		sw_area.y2 -= 2;
		lv_draw_rect(dsc->draw_ctx, &rect_dsc, &sw_area);
	}
}

static void change_event_cb(lv_event_t *e)
{
	lv_obj_t *obj = lv_event_get_target(e);
	uint16_t col;
	uint16_t row;
	lv_table_get_selected_cell(obj, &row, &col);
	bool chk = lv_table_has_cell_ctrl(obj, row, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
	if(chk)
		lv_table_clear_cell_ctrl(obj, row, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
	else
		lv_table_add_cell_ctrl(obj, row, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
}

static void scroll_event_cb(lv_event_t *e)
{
	lv_obj_t *obj = lv_event_get_target(e);
	uint16_t col;
	uint16_t row;
	lv_table_get_selected_cell(obj, &row, &col);
	printf("scroll col = %d, row = %d", col, row);
	bool chk = lv_table_has_cell_ctrl(obj, row, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
	if(chk)
		lv_table_clear_cell_ctrl(obj, row, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
	else
		lv_table_add_cell_ctrl(obj, row, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
}


/*a very light-wighted list created form table*/
void lv_example_table_2(void)
{
	/*measure memory usage*/
	lv_mem_monitor_t mon1;
	lv_mem_monitor(&mon1);

	uint32_t t = lv_tick_get();

	lv_obj_t *table = lv_table_create(lv_scr_act());

	/*set a smaller height to the table, It'll make it scrollable*/
	lv_obj_set_size(table, LV_SIZE_CONTENT, 200);

	lv_table_set_col_width(table, 0, 150);
	lv_table_set_row_cnt(table, ITEM_CNT);/*not required but avoids a lot of memory reallocation lv_table_set_set_value*/
	lv_table_set_col_cnt(table, 1);

	/*Don't make the cell pressed, we will draw something different in the event*/
	lv_obj_remove_style(table, NULL, LV_PART_ITEMS | LV_STATE_PRESSED);
	uint32_t i;
	for(i = 0; i < ITEM_CNT; i++)
	{
		lv_table_set_cell_value_fmt(table, i, 0, "Ttem %"LV_PRIu32, i + 1);
	}
	lv_obj_align(table, LV_ALIGN_CENTER, 0, -20);

	/*add an event callback to to apply some custom drawing*/
	lv_obj_add_event_cb(table, draw_event_cb1, LV_EVENT_DRAW_PART_END, NULL);
	lv_obj_add_event_cb(table, change_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    //lv_obj_add_event_cb(table, scroll_event_cb, LV_EVENT_SCROLL, NULL);
	lv_mem_monitor_t mon2;
	lv_mem_monitor(&mon2);

	uint32_t mem_used = mon1.free_size - mon2.free_size;
	uint32_t elaps = lv_tick_elaps(t);

	lv_obj_t *label = lv_label_create(lv_scr_act());
	lv_label_set_text_fmt(label, "%"LV_PRIu32" items were created in %"LV_PRIu32" ms\n"
									"using %"LV_PRIu32" bytes of memory", ITEM_CNT, elaps, mem_used);
	lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -10);


}

/*tabview*/
void lv_example_tabview_1(void)
{
	/*create a tab view object*/
	lv_obj_t *tabview;
	tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0);

	/*add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
	lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Tab 1");
	lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Tab 2");
	lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Tab 3");

	/*add content to the tabs*/
	lv_obj_t *label = lv_label_create(tab1);
	lv_label_set_text(label, "this the first tab\n\n"
							"If the content\n"
							"of a tab\n"
							"becomes too\n"
							"longer\n"
							"than the\n"
							"container\n"
							"then it\n"
							"automatically\n"
							"becomes\n"
							"scrollable.\n"
							"\n"
							"\n"
							"\n"
							"Can you see it?");
	label = lv_label_create(tab2);
	lv_label_set_text(label, "Second tab");

	label = lv_label_create(tab3);
	lv_label_set_text(label, "Third tab");

	lv_obj_scroll_to_view_recursive(label, LV_ANIM_ON);


}

static void scroll_begin_event(lv_event_t *e)
{
	/*Disable the scroll animations. triggered when a tab button is clicked*/
	if(lv_event_get_code(e) == LV_EVENT_SCROLL_BEGIN)
	{
		lv_anim_t *a = lv_event_get_param(e);
		if(a)
			a->time = 0;
	}
}

void lv_example_tabview_2(void)
{
	/*create a tab view object*/
	lv_obj_t *tabview;
	tabview = lv_tabview_create(lv_scr_act(), LV_DIR_LEFT, 80);
	lv_obj_add_event_cb(lv_tabview_get_content(tabview), scroll_begin_event, LV_EVENT_SCROLL_BEGIN, NULL);
	lv_obj_set_style_bg_color(tabview, lv_palette_lighten(LV_PALETTE_BLUE, 2), 0);

	lv_obj_t *tab_btns = lv_tabview_get_tab_btns(tabview);
	lv_obj_set_style_bg_color(tab_btns, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
	lv_obj_set_style_text_color(tab_btns, lv_palette_lighten(LV_PALETTE_RED, 1), 0);
	lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_RIGHT, LV_PART_ITEMS | LV_STATE_CHECKED);

	/*add 3 tab (the tabs are page(lv_page) and can be scrolled*/
	lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Tab 1");
	lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Tab 2");
	lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Tab 3");
	lv_obj_t *tab4 = lv_tabview_add_tab(tabview, "tab 4");
	lv_obj_t *tab5 = lv_tabview_add_tab(tabview, "Tab 5");

	lv_obj_set_style_bg_color(tab2, lv_palette_lighten(LV_PALETTE_AMBER, 3), 0);
	lv_obj_set_style_bg_opa(tab2, LV_OPA_COVER, 0);

	/*add content to the tabs*/
	lv_obj_t *label = lv_label_create(tab1);
	lv_label_set_text(label, "First tab");

	label = lv_label_create(tab2);
	lv_label_set_text(label, "Second tab");

	label = lv_label_create(tab3);
	lv_label_set_text(label, "Third tab");

	label = lv_label_create(tab4);
	lv_label_set_text(label, "Forth tab");

	label = lv_label_create(tab5);
	lv_label_set_text(label, "fifth tab");

	lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);


}



/*roller*/

static void event_handler_roller1(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED)
    {
        //lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), 0);
        //lv_obj_set_style_text_color(obj, lv_color_hex(0x0000ff), 0);
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        printf("selected month : %s\n", buf);
    }
}

void lv_example_roller_1(void)
{
    lv_obj_t *roller1 = lv_roller_create(lv_scr_act());
    char options[2048];
    int len = 0;
    memset(options, '\0', 512);
    for(int i = 0; i < 60 ; i++)
    {
        if(i < 59 )
        {
            len += sprintf(options + len, "%02d\n", i);
        }
        else
        {
            len += sprintf(options + len, "%02d", i);
        }
    }
    #if 0
    lv_roller_set_options(roller1, "January\n"
                                   "February\n"
                                   "March\n"
                                   "April\n"
                                   "May\n"
                                   "June\n"
                                   "July\n"
                                   "August\n"
                                   "September\n"
                                   "October\n"
                                   "November\n"
                                   "December", LV_ROLLER_MODE_INFINITE);
#else
    lv_roller_set_options(roller1, options, LV_ROLLER_MODE_INFINITE);
    lv_obj_set_style_bg_color(roller1, lv_color_hex(0xffffff), 0);
#endif
    lv_roller_set_visible_row_count(roller1, 4);
    lv_obj_center(roller1);
    lv_obj_add_event_cb(roller1, event_handler_roller1, LV_EVENT_ALL, NULL);

}


void lv_example_roller_2(void)
{
    static lv_style_t style_sel;
    lv_style_init(&style_sel);
    lv_style_set_text_font(&style_sel, &lv_font_montserrat_22);

    const char *opts = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";
    lv_obj_t *roller;

    /*a roller on the left with left aligned text, and custom width*/
    roller = lv_roller_create(lv_scr_act());
    lv_roller_set_options(roller, opts, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 2);
    lv_obj_set_width(roller, 100);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(roller, LV_ALIGN_LEFT_MID, 10, 0);

    lv_obj_add_event_cb(roller, event_handler_roller1, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 2, LV_ANIM_OFF);

    /*A roller on the middle with center aligned text, and auto (default) width*/
    roller = lv_roller_create(lv_scr_act());
    lv_roller_set_options(roller, opts, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 3);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_align(roller, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(roller, event_handler_roller1, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 5, LV_ANIM_OFF);

    /*A roller on the right with right aligned text, and custom width*/
    roller = lv_roller_create(lv_scr_act());
    lv_roller_set_options(roller, opts, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 4);
    lv_obj_set_width(roller, 80);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_align(roller, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_add_event_cb(roller, event_handler_roller1, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 8, LV_ANIM_OFF);
}

void lv_gif_example_1(void)
{
    LV_IMG_DECLARE(aixin);
    LV_IMG_DECLARE(hutao);
    LV_IMG_DECLARE(img_lv_100ask_bg);
    lv_obj_t *gif_bg = lv_gif_create(lv_scr_act());
    lv_gif_set_src(gif_bg, &hutao);
    //lv_obj_t *gif_bg = lv_img_create(lv_scr_act());
    //lv_img_set_src(gif_bg, &hutao1);
    lv_obj_center(gif_bg);
}


/*************************************************
 *  函数名称 :  anim_x_cb
 *  参    数 ： 无
 *  函数功能 ： 动画x轴位置显示回调
 *************************************************/
static void anim_x_cb(void *var,int32_t v)
{
   lv_obj_set_x(var,v);
}
/*************************************************
 *  函数名称 :  anim_size_cb
 *  参    数 ： 无
 *  函数功能 ： 动画尺寸显示回调函数
 *************************************************/
static void anim_size_cb(void *var,int32_t v)
{
   lv_obj_set_size(var,v,v);
}
/*************************************************
 *  函数名称 :  anim_show_2
 *  参    数 ： 无
 *  函数功能 ： 按键联动，实现Label动画显示
 *************************************************/
void anim_show_2(void)
{
   lv_obj_t * obj = lv_obj_create(lv_scr_act());      //创建一个对象
   lv_obj_set_style_bg_color(obj,lv_palette_main(LV_PALETTE_RED),0); //设置背景颜色
   lv_obj_set_style_radius(obj,LV_RADIUS_CIRCLE,0);   //设置样式圆角

   lv_obj_align(obj,LV_ALIGN_LEFT_MID,10,0);          //居中样式

   lv_anim_t a;                                       //创建动画样式
   lv_anim_init(&a);                                  //初始化动画
   lv_anim_set_var(&a,obj);                           //给动画设置一个变量
   lv_anim_set_values(&a,10,50);                      //设置一个动画值
   lv_anim_set_time(&a,1000);                         //设置动画时间
   lv_anim_set_playback_delay(&a,100);                //回放延时 使动画回放时，正向方向准备好了
   lv_anim_set_playback_time(&a,300);                 //回放时间
   lv_anim_set_repeat_delay(&a,500);                  //重复延时
   lv_anim_set_repeat_count(&a,LV_ANIM_REPEAT_INFINITE); //重复计数次数
   lv_anim_set_path_cb(&a,lv_anim_path_ease_in_out);  //设置动画播放路径

   lv_anim_set_exec_cb(&a,anim_size_cb);              //给动画设置一个功能 回调函数为尺寸
   lv_anim_start(&a);                                 //开始动画
   lv_anim_set_exec_cb(&a,anim_x_cb);                 //给动画设置一个功能 回调函数为x轴值
   lv_anim_set_values(&a,10,240);                     //给动画设置一个值
   lv_anim_start(&a);                                 //开始动画
}


/*************************************************
 *  函数名称 :  sw_event_cb
 *  参    数 ： 无
 *  函数功能 ： 按键回调联动显示动画
 *************************************************/
static void sw_event_cb(lv_event_t * e)
{
   lv_obj_t * sw = lv_event_get_target(e);               //获取事件最初瞄准的对象。即使事件是冒泡的，也是一样的。
   lv_obj_t * label = lv_event_get_user_data(e);         //获取在对象上注册事件时传递的user_data
   if(lv_obj_has_state(sw,LV_STATE_CHECKED)){
      lv_anim_t a;
      lv_anim_init(&a);                                  //初始化动画对象
      lv_anim_set_var(&a, label);                        //为对象设置一个动画
      lv_anim_set_values(&a, lv_obj_get_x(label), 100);  //设置一个动画的开始和结束值
      lv_anim_set_time(&a, 500);                         //设置一个动画时间
      lv_anim_set_exec_cb(&a, anim_x_cb);                //设置一个动画回调
      lv_anim_set_path_cb(&a, lv_anim_path_overshoot);   //设置一个动画的路径
      lv_anim_start(&a);                                 //开始动画
   }
   else{
      lv_anim_t a;
      lv_anim_init(&a);                                  //初始化动画对象
      lv_anim_set_var(&a, label);                        //为对象设置一个动画
      lv_anim_set_values(&a, lv_obj_get_x(label), -lv_obj_get_width(label));//设置一个动画的开始和结束值
      lv_anim_set_time(&a, 500);                         //设置一个动画时间
      lv_anim_set_exec_cb(&a, anim_x_cb);                //设置一个动画回调
      lv_anim_set_path_cb(&a, lv_anim_path_ease_in);     //设置一个动画的路径
      lv_anim_start(&a);                                 //开始动画
   }
}
/*************************************************
 *  函数名称 :  anim_show_1
 *  参    数 ： 无
 *  函数功能 ： 按键联动，实现Label动画显示
 *************************************************/
void anim_show_1()
{
   lv_obj_t * label = lv_label_create(lv_scr_act());       //创建一个Label
   lv_label_set_text(label,"Hello animations!");         //为label设置显示内容
   lv_obj_set_pos(label,100,10);                         //设置对象位置

   lv_obj_t * sw = lv_switch_create(lv_scr_act());       //创建一个开关对象
   lv_obj_center(sw);                                    //居中显示
   lv_obj_add_state(sw,LV_STATE_CHECKED);                //添加状态
   lv_obj_add_event_cb(sw,sw_event_cb,LV_EVENT_VALUE_CHANGED,label); //添加回调函数
}

void anim_spin_test1(void)
{
    LV_IMG_DECLARE(Sun2);
    lv_obj_t *img_revolve = lv_img_create(lv_scr_act());
    lv_img_set_src(img_revolve, &Sun2);
    lv_obj_align(img_revolve, LV_ALIGN_CENTER, 0, 0);

    /*创建旋转动画效果*/
    lv_anim_t anim_img;
    lv_anim_init(&anim_img);
    lv_anim_set_var(&anim_img, img_revolve);

    /*设置动画参数*/
    lv_anim_set_exec_cb(&anim_img, (lv_anim_exec_xcb_t)lv_img_set_angle);
    lv_anim_set_time(&anim_img, 900);
    lv_anim_set_values(&anim_img, 0, 3600-1);
    lv_anim_set_repeat_count(&anim_img, LV_ANIM_REPEAT_INFINITE);/*无限触发*/
    /*设置动画路径*/
    lv_anim_set_path_cb(&anim_img, lv_anim_path_ease_in_out);
    /*开始动画*/
    lv_anim_start(&anim_img);
}

static void sun_anim_x_cb(void *var, int32_t v)
{
    lv_obj_set_x(var, v);
    printf("ste x %d\r\n", v);
}

static void sun_anim_y_cb(void *var, int32_t v)
{
    lv_obj_set_y(var, v);
    printf("ste y %d\r\n", v);
}

static void sun_anim_size_cb(void *var, int32_t v)
{

    lv_obj_set_size(var, v, v);
}

void anim_sun_test1(void)
{
    LV_IMG_DECLARE(Sun2);
    /*创建图片*/
    lv_obj_t *img = lv_img_create(lv_scr_act());
    /*设置图像位置*/
    lv_obj_set_pos(img, 200, 40);
    lv_img_set_src(img, &Sun2);
    /*创建动画样式*/
    lv_anim_t anim_sun;
    lv_anim_init(&anim_sun);
    lv_anim_set_var(&anim_sun, img);
    lv_anim_set_values(&anim_sun, 40, 40);
    lv_anim_set_time(&anim_sun, 2000);
    lv_anim_set_playback_delay(&anim_sun, 1000);
    lv_anim_set_playback_time(&anim_sun, 0);
    lv_anim_set_repeat_delay(&anim_sun, 800);
    lv_anim_set_repeat_count(&anim_sun, 1);
    lv_anim_set_path_cb(&anim_sun, lv_anim_path_ease_in_out);

    //lv_anim_set_exec_cb(&anim_sun, sun_anim_size_cb);
    //lv_anim_start(&anim_sun);
    lv_anim_set_exec_cb(&anim_sun, sun_anim_x_cb);
    lv_anim_set_values(&anim_sun, 200, 210);
    lv_anim_start(&anim_sun);
    lv_anim_set_exec_cb(&anim_sun, sun_anim_y_cb);
    lv_anim_set_values(&anim_sun, 40, 280);
    lv_anim_start(&anim_sun);
}

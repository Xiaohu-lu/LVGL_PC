/*
 * lv_about_page.c
 *
 *  Created on: Dec 13, 2023
 *      Author: hxd
 */
/* lvgl
 * */
#include "lv_typedef.h"
#include "lvgl/lvgl.h"
#include "UI/lv_main_page.h"
#include "UI/lv_about_page.h"



static PT_lv_about g_pt_lv_about;		/*数据结构体*/




static void lv_boart_about_init(void);
static void event_handler_back_to_home(lv_event_t *e);

/* lv_board_about
 * 板子信息界面
 * */
void lv_board_about(void)
{
	g_pt_lv_about = (T_lv_about*)pvPortMalloc(sizeof(T_lv_about));
	g_pt_lv_about->bg_about = lv_obj_create(lv_scr_act());/*创建背景*/

	//lv_obj_clear_flag(g_pt_lv_about->bg_about, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_size(g_pt_lv_about->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_about->bg_about, 0, 0);/*设置圆角*/
	//lv_obj_set_x(g_pt_lv_about->bg_about, 0);/*设置x坐标*/
	//lv_obj_set_y(g_pt_lv_about->bg_about, 0);/*设置y坐标*/
	lv_obj_align(g_pt_lv_about->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);

	//lv_obj_remove_style_all(g_pt_lv_about->bg_about);

	/*设置padding*/
//	lv_obj_set_style_pad_top(g_pt_lv_about->bg_about, 0, 0);
//	lv_obj_set_style_pad_bottom(g_pt_lv_about->bg_about, 0, 0);
//	lv_obj_set_style_pad_left(g_pt_lv_about->bg_about, 0, 0);
//	lv_obj_set_style_pad_right(g_pt_lv_about->bg_about, 0, 0);
	set_user_style(g_pt_lv_about->bg_about);

	lv_obj_set_size(g_pt_lv_about->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_about->bg_about, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_about->bg_about, lv_color_white(), 1);
	lv_boart_about_init();/*界面初始化*/

	add_title(g_pt_lv_about->bg_about, "ABOUTS");
	add_back(g_pt_lv_about->bg_about, event_handler_back_to_home);

}

/* lv_boart_about_init
 * about界面初始
 * */
static void lv_boart_about_init(void)
{
	g_pt_lv_about->about_table = lv_table_create(g_pt_lv_about->bg_about);
	lv_table_set_col_cnt(g_pt_lv_about->about_table, 2);
	lv_table_set_row_cnt(g_pt_lv_about->about_table, 9);
	lv_table_set_col_width(g_pt_lv_about->about_table, 0, (LV_ABOUT_US_TABLE_SPACE * 2));
	lv_table_set_col_width(g_pt_lv_about->about_table, 1, (LV_ABOUT_US_TABLE_SPACE * 4));

	lv_obj_align(g_pt_lv_about->about_table, LV_ALIGN_TOP_MID, 0, LV_ABOUT_US_TITLE_SPACE);

	/*Make the cells of the first row center aligned*/
	//lv_table_set_cell_align(g_pt_lv_about->about_table, 0, 0, LV_LABEL_ALIGN_LEFT);



	lv_obj_set_size(g_pt_lv_about->about_table, 300, 240);
	lv_table_set_cell_value(g_pt_lv_about->about_table, 0, 0, "Name");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 1, 0, "Version");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 2, 0, "Board Name");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 3, 0, "Producer");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 4, 0, "Core");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 5, 0, "SRAM");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 6, 0, "Flash");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 7, 0, "SD card");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 8, 0, "CPU Usage");

	lv_table_set_cell_value(g_pt_lv_about->about_table, 0, 1, "100ASK");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 1, 1, "V1.2");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 2, 1, "100ASK_STM32F103");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 3, 1, "https://www.100ask.net");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 4, 1, "ARM 32-bit Cortex?-M3 CPU");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 5, 1, "64KB+1024KB");
	lv_table_set_cell_value(g_pt_lv_about->about_table, 6, 1, "512KB+1024KB");
	lv_table_set_cell_value_fmt(g_pt_lv_about->about_table, 7, 1, "%d MB free total %d MB", 100, 10);
	lv_table_set_cell_value(g_pt_lv_about->about_table, 8, 1, "0%");

	LV_IMG_DECLARE(img_lv_100ask_demo_logo);
	lv_obj_t *logo = lv_img_create(g_pt_lv_about->bg_about);
	lv_img_set_src(logo, &img_lv_100ask_demo_logo);
	lv_obj_align_to(logo, g_pt_lv_about->about_table, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 0);

	lv_obj_t *label_logo = lv_label_create(g_pt_lv_about->bg_about);
	lv_obj_set_style_text_font(label_logo, &lv_font_montserrat_14, 0);
	lv_label_set_text(label_logo, "https://www.100ask.net\nhttp://www.lvgl.100ask.net");
	lv_obj_align_to(label_logo, logo, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *label_info1 = lv_label_create(g_pt_lv_about->bg_about);
	lv_obj_set_style_text_font(label_info1, &lv_font_montserrat_10, 0);
	lv_label_set_text(label_info1, "100ASK");

	/*create a 100x100 QR code*/
	g_pt_lv_about->qr_official_website = lv_qrcode_create(g_pt_lv_about->bg_about, LV_ABOUT_QRCODE_AD_SIZE, lv_color_make(0x00, 0xff, 0x00), lv_color_hex3(0xeef));
	/*set data*/
	lv_qrcode_update(g_pt_lv_about->qr_official_website, "https://www.100ask.net", strlen("https://www.100ask.net"));
	//lv_obj_align_to(g_pt_lv_about->qr_official_website, label_logo, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 10);
	//lv_obj_align(g_pt_lv_about->qr_official_website, LV_ALIGN_BOTTOM_LEFT, (LV_ABOUT_QRCODE_SPACE + (LV_ABOUT_QRCODE_SIZE * 0)), -lv_obj_get_height(label_info1));  // 摆放位置
	lv_obj_align(g_pt_lv_about->qr_official_website, LV_ALIGN_BOTTOM_LEFT, (LV_ABOUT_QRCODE_SPACE + (LV_ABOUT_QRCODE_SIZE * 0)), -20);  // 摆放位置
	lv_obj_align_to(label_info1, g_pt_lv_about->qr_official_website, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

	/*微信公众号*/
	lv_obj_t *label_info2 = lv_label_create(g_pt_lv_about->bg_about);
	lv_obj_set_style_text_font(label_info2, &lv_font_montserrat_10, 0);
	lv_label_set_text(label_info2, "WeChat");

	/*Create a 100x100 QR code*/
	g_pt_lv_about->qr_wechat = lv_qrcode_create(g_pt_lv_about->bg_about, LV_ABOUT_QRCODE_AD_SIZE, lv_color_make(0x00, 0xff, 0x00), lv_color_hex3(0xeef));
	/*set data*/
	lv_qrcode_update(g_pt_lv_about->qr_wechat, "http://weixin.qq.com/r/M0hvd2-EwjK9rSsU9x0h", strlen("http://weixin.qq.com/r/M0hvd2-EwjK9rSsU9x0h"));
	//lv_obj_align_to(g_pt_lv_about->qr_wechat, g_pt_lv_about->qr_official_website, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
	//lv_obj_align(g_pt_lv_about->qr_wechat, LV_ALIGN_BOTTOM_LEFT, (LV_ABOUT_QRCODE_SPACE + (LV_ABOUT_QRCODE_SIZE * 1)), -lv_obj_get_height(label_info2));  // 摆放位置
	lv_obj_align(g_pt_lv_about->qr_wechat, LV_ALIGN_BOTTOM_LEFT, (LV_ABOUT_QRCODE_SPACE + (LV_ABOUT_QRCODE_SIZE * 1)), -20);  // 摆放位置
	lv_obj_align_to(label_info2, g_pt_lv_about->qr_wechat, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

	/*100ASK LVGL*/
	lv_obj_t *label_info3 = lv_label_create(g_pt_lv_about->bg_about);
	lv_obj_set_style_text_font(label_info3, &lv_font_montserrat_10, 0);
	lv_label_set_text(label_info3, "100ASK LVGL");

	/*Create a 100x100 QR code*/
	g_pt_lv_about->qr_100ask_lvgl = lv_qrcode_create(g_pt_lv_about->bg_about, LV_ABOUT_QRCODE_AD_SIZE, lv_color_make(0xff, 0x00, 0x00), lv_color_hex3(0xeef));
	/*set data*/
	lv_qrcode_update(g_pt_lv_about->qr_100ask_lvgl, "http://lvgl.100ask.net", strlen("http://lvgl.100ask.net"));
	//lv_obj_align_to(g_pt_lv_about->qr_100ask_lvgl, g_pt_lv_about->qr_wechat, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
	//lv_obj_align(g_pt_lv_about->qr_100ask_lvgl, LV_ALIGN_BOTTOM_LEFT, (LV_ABOUT_QRCODE_SPACE + (LV_ABOUT_QRCODE_SIZE * 2)), -lv_obj_get_height(label_info3));  // 摆放位置
	lv_obj_align(g_pt_lv_about->qr_100ask_lvgl, LV_ALIGN_BOTTOM_LEFT, (LV_ABOUT_QRCODE_SPACE + (LV_ABOUT_QRCODE_SIZE * 2)), -20);  // 摆放位置
	lv_obj_align_to(label_info3, g_pt_lv_about->qr_100ask_lvgl, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

}

static void event_handler_back_to_home(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
		if(g_pt_lv_about->qr_100ask_lvgl != NULL) lv_obj_del(g_pt_lv_about->qr_100ask_lvgl);
		if(g_pt_lv_about->qr_wechat != NULL) lv_obj_del(g_pt_lv_about->qr_wechat);
		if(g_pt_lv_about->qr_official_website != NULL) lv_obj_del(g_pt_lv_about->qr_official_website);
		if(g_pt_lv_about->about_table != NULL) lv_obj_del(g_pt_lv_about->about_table);
		if(g_pt_lv_about->bg_about != NULL) lv_obj_del(g_pt_lv_about->bg_about);

		vPortFree(g_pt_lv_about);

		lv_main_page_demo_home(100);
	}
}





































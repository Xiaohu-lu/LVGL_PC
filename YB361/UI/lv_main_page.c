/*
 * lv_main_page.c
 *
 *  Created on: Dec 12, 2023
 *      Author: hxd
 */

/* lvgl
 * */
#include "lvgl/lvgl.h"
#include "UI/lv_typedef.h"
#include "UI/lv_main_page.h"
#include "UI/lv_about_page.h"
#include "UI/lv_find_dev.h"
#include "lv_record_page.h"
#include "lv_snake_page.h"
#include "lv_music_page.h"
#include "UI/filesys_page.h"
#include "game/gomoku.h"


/*size*/
#define LV_BOTTOM_MENU_W				(LV_HOR_RES)
#define LV_BOTTOM_MENU_H				(76)
#define LV_BOTTOM_MENU_ITEM_SIZE		(52)

/*color*/
#define LV_BOTTOM_MENU_COLOR			(lv_color_hex(0xC9C9CE))
#define LV_BOTTOM_MENU_ITEM_COUNT		(4)
#define LV_MENU_ITEM_COUNT				(12)

/*Padding*/
#define LV_DEMO_TITLE_PAD				(8)
#define LV_DEMO_BACK_W					(30)
#define LV_DEMO_BACK_H					(30)
#define LV_DEMO_BACK_PAD				(8)
#define LV_TABVIEW_NAVIGATION_SPACE		(10)

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void event_handler_stm32_demo_about(lv_event_t *e);					// �򿪹�������Ӧ���¼�������
static void event_handler_stm32_demo_game_2048(lv_event_t *e);				// ��2048��ϷӦ���¼��������¼�������
static void event_handler_stm32_demo_tools_calc(lv_event_t *e);      		// �򿪼�����Ӧ���¼�������
static void event_handler_stm32_demo_game_snake(lv_event_t *e);			    // ��̰������ϷӦ���¼�������
static void event_handler_stm32_demo_tools_clock(lv_event_t *e);			// ��ʱ��Ӧ���¼�������
static void event_handler_stm32_demo_tools_regbit(lv_event_t *e);   		// �򿪼Ĵ���λ�鿴���㹤��Ӧ���¼�������
static void event_handler_stm32_demo_tools_qrcode(lv_event_t *e);   		// �򿪴�����ά��Ӧ���¼�������
static void event_handler_stm32_demo_music_player(lv_event_t *e); 			// �����ֲ�����Ӧ���¼�������
static void event_handler_stm32_demo_tools_humiture(lv_event_t *e);			// ����ʪ��Ӧ���¼�������
static void event_handler_stm32_demo_tools_settings(lv_event_t *e);			// ��ϵͳ����Ӧ���¼�������
static void event_handler_stm32_demo_tools_widget_test(lv_event_t *e);		// ��ADC��LED��BEEP����Ӧ���¼�������
static void event_handler_stm32_demo_tools_file_browser(lv_event_t *e); 	// ���ļ������Ӧ���¼�������



/* ����ͼ������ */

LV_IMG_DECLARE(img_lv_100ask_icon_2048);		// 00 LV_100ASK_MENU_ITEM_COUNT
LV_IMG_DECLARE(img_lv_100ask_icon_about_us);	// 01
LV_IMG_DECLARE(img_lv_100ask_icon_setting);	    // 02
LV_IMG_DECLARE(img_lv_100ask_icon_data);		// 03
LV_IMG_DECLARE(img_lv_100ask_icon_floder);		// 04
LV_IMG_DECLARE(img_lv_100ask_icon_music);		// 05
LV_IMG_DECLARE(img_lv_100ask_icon_QRcode);  	// 06
LV_IMG_DECLARE(img_lv_100ask_icon_snake);		// 07
LV_IMG_DECLARE(img_lv_100ask_icon_humiture);	// 08
LV_IMG_DECLARE(img_lv_100ask_icon_regbit);		// 09
LV_IMG_DECLARE(img_lv_100ask_icon_calc);		// 10
LV_IMG_DECLARE(img_lv_100ask_icon_widgets);   	// 11

/* ����ͼ������ */
#if 1
static const lv_img_dsc_t *app_icon[] = \
	{ \
	  &img_lv_100ask_icon_floder,	 &img_lv_100ask_icon_data, 	   &img_lv_100ask_icon_QRcode,	  &img_lv_100ask_icon_about_us, \
	  &img_lv_100ask_icon_humiture,  &img_lv_100ask_icon_2048,	   &img_lv_100ask_icon_music,	  &img_lv_100ask_icon_snake, \
	  &img_lv_100ask_icon_calc,      &img_lv_100ask_icon_regbit,   \
	  &img_lv_100ask_icon_setting, \
	  &img_lv_100ask_icon_widgets
	};
	#else
static const lv_img_dsc_t *app_icon[] = \
{ \

};
#endif
/* �ص�����ָ������*/
static void (*lv_event_cb[])(lv_event_t *e) = \
	{ \
	  event_handler_stm32_demo_tools_file_browser, event_handler_stm32_demo_tools_clock, event_handler_stm32_demo_tools_qrcode,  event_handler_stm32_demo_about, \
	  event_handler_stm32_demo_tools_humiture,     event_handler_stm32_demo_game_2048,   event_handler_stm32_demo_music_player,  event_handler_stm32_demo_game_snake, \
	  event_handler_stm32_demo_tools_calc,         event_handler_stm32_demo_tools_regbit, \
	  event_handler_stm32_demo_tools_settings, \
	  event_handler_stm32_demo_tools_widget_test \
	};

/* Ӧ��ͼ������ */
static const char * app_name[] = \
	{ \
	  "file",     "time",    "QRcode",  "aboutus", \
	  "humiture", "2048",    "music",  "snake", \
	  "calc",     "regbit", \
	  "camera",  \
	  "test"
	};



static lv_obj_t *bg_top;			/*����*/
static lv_obj_t *tabview_desktop;	/*ҳǩ*/
static lv_obj_t *tab_left;
static lv_obj_t *tab_main;
static lv_obj_t *tab_right;

static uint16_t tabview_desktop_id;	/*��¼ҳǩ��ǰչʾ��ҳ��*/

/* lv_main_page_demo
 * ������ҳ��
 * delay:������ҳ�����ʱ
 * */
void lv_main_page_demo(uint32_t delay)
{
	/*ȷ����Ļ�޲���*/
	lv_obj_clean(lv_scr_act());

	/*���汳��*/
	LV_IMG_DECLARE(img_lv_100ask_bg);

	bg_top = lv_img_create(lv_scr_act());

	//lv_obj_remove_style_all(bg_top);
	set_user_style(bg_top);

	lv_img_set_src(bg_top, &img_lv_100ask_bg);
	lv_obj_align(bg_top, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(bg_top, 320, 480);
	lv_obj_fade_in(bg_top, 0, delay);

	/*����Ч��*/
	lv_boot_animation(delay);

	/*���*/
	lv_obj_t *label_state = lv_label_create(bg_top);
	lv_label_set_recolor(label_state, true);/*����������ɫ*/
	/*������ɫ���� #color text# color=rgb888, text����*/
	lv_label_set_text(label_state, "#ffffff "LV_SYMBOL_BATTERY_FULL"#");
	//lv_label_set_text(label_state, "#ffffff Re-color# #ff00ff words#");
	lv_obj_align(label_state, LV_ALIGN_TOP_RIGHT, -(15*1), lv_obj_get_height(label_state)/2);
	lv_obj_fade_in(label_state, 0, delay);

	/*ʱ�� ͼ��*/
	label_state = lv_label_create(bg_top);
	lv_label_set_recolor(label_state, true);/*����������ɫ*/
	/*������ɫ���� #color text# color=rgb888, text����*/
	lv_label_set_text(label_state, "#ffffff 17:33#");
	//lv_label_set_text(label_state, "#ffffff Re-color# #ff00ff words#");
	lv_obj_align(label_state, LV_ALIGN_TOP_MID, 0, lv_obj_get_height(label_state)/2);
	lv_obj_fade_in(label_state, 0, delay);

	tabview_desktop_id = 1;

	lv_main_page_demo_home(delay);
}


/* lv_main_page_demo_home
 * ҳǩ��ʼ��
 *
 * */
void lv_main_page_demo_home(uint32_t delay)
{
	/*����һ��ҳǩ,��ť������,0�߶�*/
	tabview_desktop = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0);
	lv_obj_set_style_bg_opa(tabview_desktop, LV_OPA_0, 0);/*���ñ���͸����*/

	set_user_style(tabview_desktop);

	tab_left = lv_tabview_add_tab(tabview_desktop, "left_desktop");/*0*/
	tab_main = lv_tabview_add_tab(tabview_desktop, "main_desktop");/*1*/
	tab_right = lv_tabview_add_tab(tabview_desktop, "right_desktop");/*2*/
//	lv_obj_remove_style_all(tab_left);
//	lv_obj_remove_style_all(tab_main);
//	lv_obj_remove_style_all(tab_right);
	set_user_style(tab_left);
	set_user_style(tab_main);
	set_user_style(tab_right);

	lv_obj_set_size(tab_left, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_size(tab_main, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_size(tab_right, LV_HOR_RES, LV_VER_RES);
	lv_tabview_set_act(tabview_desktop, tabview_desktop_id, LV_ANIM_OFF);/*�л���ǰ��ʾҳǩ,����*/
	lv_obj_fade_in(tabview_desktop, 0, delay);

	/*���ҳ����ʾԲȦ*/
	static lv_style_t style_round1;
	static lv_style_t style_round2;
	lv_style_init(&style_round1);
	lv_style_set_bg_opa(&style_round1, LV_OPA_80);
	lv_style_set_radius(&style_round1, 100);
	lv_style_set_bg_color(&style_round1, lv_color_hex(0xFFFFFF));

	lv_style_init(&style_round2);
	lv_style_set_bg_opa(&style_round2, LV_OPA_40);
	lv_style_set_radius(&style_round2, 100);
	lv_style_set_bg_color(&style_round2, lv_color_hex(0xFFFFFF));

	lv_obj_t *left_round1 = lv_obj_create(tab_left);
	set_user_style(left_round1);
	lv_obj_set_size(left_round1, 8, 8);
	lv_obj_add_style(left_round1, &style_round1, 0);/*0����Ĭ��״̬��*/
	lv_obj_align(left_round1, LV_ALIGN_BOTTOM_MID, 0, -(LV_BOTTOM_MENU_H + LV_TABVIEW_NAVIGATION_SPACE));


	lv_obj_t *left_round2 = lv_obj_create(tab_left);
	set_user_style(left_round2);
	lv_obj_set_size(left_round2, 8, 8);
	lv_obj_add_style(left_round2, &style_round1, 0);/*0����Ĭ��״̬��*/
	lv_obj_align(left_round2, LV_ALIGN_BOTTOM_MID, 20, -(LV_BOTTOM_MENU_H + LV_TABVIEW_NAVIGATION_SPACE));

	lv_obj_t *left_round3 = lv_obj_create(tab_left);
	set_user_style(left_round3);
	lv_obj_set_size(left_round3, 8, 8);
	lv_obj_add_style(left_round3, &style_round2, 0);/*0����Ĭ��״̬��*/
	lv_obj_align(left_round3, LV_ALIGN_BOTTOM_MID, -20, -(LV_BOTTOM_MENU_H + LV_TABVIEW_NAVIGATION_SPACE));

	lv_obj_t *main_round1 = lv_obj_create(tab_main);
	set_user_style(main_round1);
	lv_obj_set_size(main_round1, 8, 8);
	lv_obj_add_style(main_round1, &style_round2, 0);/*0����Ĭ��״̬��*/
	lv_obj_align(main_round1, LV_ALIGN_BOTTOM_MID, 0, -(LV_BOTTOM_MENU_H + LV_TABVIEW_NAVIGATION_SPACE));

	lv_obj_t *main_round2 = lv_obj_create(tab_main);
	set_user_style(main_round2);
	lv_obj_set_size(main_round2, 8, 8);
	lv_obj_add_style(main_round2, &style_round1, 0);/*0����Ĭ��״̬��*/
	lv_obj_align(main_round2, LV_ALIGN_BOTTOM_MID, 20, -(LV_BOTTOM_MENU_H + LV_TABVIEW_NAVIGATION_SPACE));

	lv_obj_t *main_round3 = lv_obj_create(tab_main);
	set_user_style(main_round3);
	lv_obj_set_size(main_round3, 8, 8);
	lv_obj_add_style(main_round3, &style_round1, 0);/*0����Ĭ��״̬��*/
	lv_obj_align(main_round3, LV_ALIGN_BOTTOM_MID, -20, -(LV_BOTTOM_MENU_H + LV_TABVIEW_NAVIGATION_SPACE));

	lv_obj_t *right_round1 = lv_obj_create(tab_right);
	set_user_style(right_round1);
	lv_obj_set_size(right_round1, 8, 8);
	lv_obj_add_style(right_round1, &style_round1, 0);/*0����Ĭ��״̬��*/
	lv_obj_align(right_round1, LV_ALIGN_BOTTOM_MID, 0, -(LV_BOTTOM_MENU_H + LV_TABVIEW_NAVIGATION_SPACE));

	lv_obj_t *right_round2 = lv_obj_create(tab_right);
	set_user_style(right_round2);
	lv_obj_set_size(right_round2, 8, 8);
	lv_obj_add_style(right_round2, &style_round2, 0);/*0����Ĭ��״̬��*/
	lv_obj_align(right_round2, LV_ALIGN_BOTTOM_MID, 20, -(LV_BOTTOM_MENU_H + LV_TABVIEW_NAVIGATION_SPACE));

	lv_obj_t *right_round3 = lv_obj_create(tab_right);
	set_user_style(right_round3);
	lv_obj_set_size(right_round3, 8, 8);
	lv_obj_add_style(right_round3, &style_round1, 0);/*0����Ĭ��״̬��*/
	lv_obj_align(right_round3, LV_ALIGN_BOTTOM_MID, -20, -(LV_BOTTOM_MENU_H + LV_TABVIEW_NAVIGATION_SPACE));

	/*�������˵�*/
	lv_obj_t *label_app_name;	/*appͼ���·�������*/
	lv_obj_t *obj_menu_item;	/*���ڱ�����ʼ��appͼ���ָ��*/
	uint8_t row_count = 0;		/*��*/
	uint8_t col_count = 0;		/*��*/

	/*��*/
	lv_coord_t col_space = ((((LV_BOTTOM_MENU_W - (LV_BOTTOM_MENU_ITEM_SIZE * LV_BOTTOM_MENU_ITEM_COUNT)) / 5) * (col_count + 1)) + (LV_BOTTOM_MENU_ITEM_SIZE * col_count));
	/*��*/
	lv_coord_t row_space = ((((LV_BOTTOM_MENU_W - (LV_BOTTOM_MENU_ITEM_SIZE * LV_BOTTOM_MENU_ITEM_COUNT)) / 5) * (row_count + 1)) + (LV_BOTTOM_MENU_ITEM_SIZE * row_count));

	for(int i = 0; i < LV_MENU_ITEM_COUNT; i++)
	{
		if((i >= 0) && (i < 10))/*���˵�*/
		{
			obj_menu_item = lv_img_create(tab_main);
			label_app_name = lv_label_create(tab_main);
		}
		else if((i >= 10) && (i < 11))/*���˵�*/
		{
			obj_menu_item = lv_img_create(tab_left);
			label_app_name = lv_label_create(tab_left);
		}
		else/*�Ҳ�˵�*/
		{
			obj_menu_item = lv_img_create(tab_right);
			label_app_name = lv_label_create(tab_right);
		}

		/*���������Ե��*/
		lv_obj_add_flag(obj_menu_item, LV_OBJ_FLAG_CLICKABLE);
		lv_img_set_src(obj_menu_item, app_icon[i]);/*���ͼ��*/
		lv_obj_align(obj_menu_item, LV_ALIGN_TOP_LEFT, col_space, row_space);
		lv_obj_fade_in(obj_menu_item, 0, delay);
		lv_obj_add_event_cb(obj_menu_item, lv_event_cb[i], LV_EVENT_CLICKED, NULL);

		/*����APP���ֱ�ǩ*/
		lv_obj_set_style_text_font(label_app_name, &lv_font_montserrat_10, 0);
		lv_label_set_text(label_app_name, app_name[i]);
		lv_obj_align_to(label_app_name, obj_menu_item, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
		lv_obj_fade_in(label_app_name, 0, delay);

		col_count++;
		if(((i + 1) % 4) == 0)/*����*/
		{
			col_count = 0;
			row_count++;
			row_space = ((((LV_BOTTOM_MENU_W - (LV_BOTTOM_MENU_ITEM_SIZE * LV_BOTTOM_MENU_ITEM_COUNT)) / 5) * (row_count + 1)) + (LV_BOTTOM_MENU_ITEM_SIZE * row_count));
		}
		col_space = ((((LV_BOTTOM_MENU_W - (LV_BOTTOM_MENU_ITEM_SIZE * LV_BOTTOM_MENU_ITEM_COUNT)) / 5) * (col_count + 1)) + (LV_BOTTOM_MENU_ITEM_SIZE * col_count));
	}

	/*�����ײ�����������*/
	lv_obj_t *obj_bottom_menu = lv_obj_create(lv_scr_act());
	lv_obj_remove_style_all(obj_bottom_menu);
	lv_obj_set_size(obj_bottom_menu, LV_BOTTOM_MENU_W, LV_BOTTOM_MENU_H);
	lv_obj_set_style_bg_opa(obj_bottom_menu, LV_OPA_70, 0);/*���ñ���͸����*/
	lv_obj_set_style_radius(obj_bottom_menu, 0, 0);/*����Բ��*/
	lv_obj_set_style_bg_color(obj_bottom_menu, LV_BOTTOM_MENU_COLOR, 0);
	lv_obj_align(obj_bottom_menu, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_obj_fade_in(obj_bottom_menu, 0, delay);

	/*�����ײ��������ڵĲ˵�*/
	lv_obj_t *obj_bottom_menu_item;
	for(int i = 0; i < LV_BOTTOM_MENU_ITEM_COUNT; i++)
	{
		obj_bottom_menu_item = lv_img_create(obj_bottom_menu);
		lv_obj_add_flag(obj_bottom_menu_item, LV_OBJ_FLAG_CLICKABLE);/*������*/
		lv_img_set_src(obj_bottom_menu_item, app_icon[i]);/*���ͼ��*/
		lv_obj_align(obj_bottom_menu_item, LV_ALIGN_LEFT_MID, ((((LV_BOTTOM_MENU_W - (LV_BOTTOM_MENU_ITEM_SIZE * LV_BOTTOM_MENU_ITEM_COUNT)) / 5) * (i + 1)) + (LV_BOTTOM_MENU_ITEM_SIZE * i)), 0);
		lv_obj_fade_in(obj_bottom_menu_item, 0, delay);
		lv_obj_add_event_cb(obj_bottom_menu_item, lv_event_cb[i], LV_EVENT_CLICKED, NULL);
	}

}


/* event_handler_stm32_demo_about
 * aboutͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_about(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		tabview_desktop_id = lv_tabview_get_tab_act(tabview_desktop);
		lv_obj_del(tabview_desktop);
		lv_board_about();
	}

}

/* event_handler_stm32_demo_about
 * musicͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_music_player(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		tabview_desktop_id = lv_tabview_get_tab_act(tabview_desktop);
		lv_obj_del(tabview_desktop);
		lv_music_player();
	}
}

/* event_handler_stm32_demo_about
 * 2048ͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_game_2048(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		tabview_desktop_id = lv_tabview_get_tab_act(tabview_desktop);
		lv_obj_del(tabview_desktop);
		lv_block_game();
	}

}

/* event_handler_stm32_demo_about
 * regbitͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_tools_regbit(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		tabview_desktop_id = lv_tabview_get_tab_act(tabview_desktop);
		lv_obj_del(tabview_desktop);
		lv_find_dev();
	}
}

/* event_handler_stm32_demo_about
 * qrcodeͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_tools_qrcode(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		tabview_desktop_id = lv_tabview_get_tab_act(tabview_desktop);
		lv_obj_del(tabview_desktop);
		gomoku_page();
	}
}

/* event_handler_stm32_demo_about
 * calcͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_tools_calc(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		tabview_desktop_id = lv_tabview_get_tab_act(tabview_desktop);
		lv_obj_del(tabview_desktop);
		lv_report();
	}
}

/* event_handler_stm32_demo_about
 * browserͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_tools_file_browser(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		tabview_desktop_id = lv_tabview_get_tab_act(tabview_desktop);
		lv_obj_del(tabview_desktop);
		file_sys_page();
		//lv_dev_info();
	}
}

/* event_handler_stm32_demo_about
 * snakeͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_game_snake(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		tabview_desktop_id = lv_tabview_get_tab_act(tabview_desktop);
		lv_obj_del(tabview_desktop);
		lv_snake();
	}
}

/* event_handler_stm32_demo_about
 * clockͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_tools_clock(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		tabview_desktop_id = lv_tabview_get_tab_act(tabview_desktop);
		lv_obj_del(tabview_desktop);
		lv_record();
	}
}

/* event_handler_stm32_demo_about
 * humitureͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_tools_humiture(lv_event_t *e)
{

}

/* event_handler_stm32_demo_about
 * testͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_tools_widget_test(lv_event_t *e)
{

}

/* event_handler_stm32_demo_about
 * settingsͼ�����¼��ص�����
 *
 * */
static void event_handler_stm32_demo_tools_settings(lv_event_t *e)
{

}

/* add_title
 * ��ָ��������ӱ���
 * obj:Ҫ��ӱ���Ķ���
 * txt:��������
 * */
lv_obj_t * add_title(lv_obj_t *obj, const char *txt)
{
	/*��ӱ���*/
	lv_obj_t *title = lv_label_create(obj);
	/*������ɫ*/
	//lv_obj_set_style_text_color(title, lv_color_white(), 0);
	lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
	lv_label_set_text(title, txt);
	lv_obj_align(title, LV_ALIGN_TOP_MID, 0, LV_DEMO_TITLE_PAD);
	return title;
}

/* add_back
 * ���һ�����ذ�ť
 * obj:Ҫ��ӷ��ذ�ť�Ķ���
 * event_cb:������ذ�ť�Ļص�����
 * */
void add_back(lv_obj_t *obj, lv_event_cb_t event_cb)
{
	static lv_style_t style_btn;
	static lv_style_t style_btn_press;
	lv_style_init(&style_btn);
	lv_style_set_text_color(&style_btn, lv_color_hex(0x00ff00));

	lv_style_init(&style_btn_press);
	lv_style_set_text_color(&style_btn_press, lv_color_hex(0xff0000));

	/*�������ذ�ť*/
	lv_obj_t *btn = lv_label_create(obj);
	lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);/*������*/
	lv_obj_set_style_bg_opa(btn, LV_OPA_0, 0);/*����͸����*/
	lv_obj_set_style_border_opa(btn, LV_OPA_0, 0);/*���ñ߿�͸����*/
	lv_label_set_text(btn, LV_SYMBOL_LEFT);/*���������ַ�*/
	lv_obj_set_style_text_font(btn, &lv_font_montserrat_32, 0);
	lv_obj_set_size(btn, LV_DEMO_BACK_W, LV_DEMO_BACK_H);
	lv_obj_set_pos(btn, LV_DEMO_BACK_PAD + 4, LV_DEMO_BACK_PAD);
	lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
	lv_obj_add_style(btn, &style_btn, 0);
	lv_obj_add_style(btn, &style_btn_press, LV_STATE_PRESSED);

}

/* add_confirm
 * ���һ��ȷ�ϰ�ť
 * obj:Ҫ��ӷ��ذ�ť�Ķ���
 * event_cb:������ذ�ť�Ļص�����
 * */
void add_confirm(lv_obj_t *obj, lv_event_cb_t event_cb)
{
	static lv_style_t style_btn;
	static lv_style_t style_btn_press;
	lv_style_init(&style_btn);
	lv_style_set_text_color(&style_btn, lv_color_hex(0x00ff00));

	lv_style_init(&style_btn_press);
	lv_style_set_text_color(&style_btn_press, lv_color_hex(0xff0000));

	/*�������ذ�ť*/
	lv_obj_t *btn = lv_label_create(obj);
	lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);/*������*/
	lv_obj_set_style_bg_opa(btn, LV_OPA_0, 0);/*����͸����*/
	lv_obj_set_style_border_opa(btn, LV_OPA_0, 0);/*���ñ߿�͸����*/
	lv_label_set_text(btn, LV_SYMBOL_OK);/*���������ַ�*/
	lv_obj_set_style_text_font(btn, &lv_font_montserrat_32, 0);
	lv_obj_set_size(btn, LV_DEMO_BACK_W, LV_DEMO_BACK_H);
	lv_obj_set_pos(btn, LV_HOR_RES - LV_DEMO_BACK_W - 4, LV_DEMO_BACK_PAD);
	lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
	lv_obj_add_style(btn, &style_btn, 0);
	lv_obj_add_style(btn, &style_btn_press, LV_STATE_PRESSED);

}

/* set_user_style
 * ȡ�������padding��border
 * obj:Ҫȡ���Ķ���
 * */
void set_user_style(lv_obj_t *obj)
{
	/*ȡ��padding*/
	lv_obj_set_style_pad_top(obj, 0, 0);
	lv_obj_set_style_pad_bottom(obj, 0, 0);
	lv_obj_set_style_pad_left(obj, 0, 0);
	lv_obj_set_style_pad_right(obj, 0, 0);
	/*ȡ��border*/
	lv_obj_set_style_border_width(obj, 0, 0);
}











/*
 * lv_music_page.c
 *
 *  Created on: Dec 19, 2023
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
#include "lv_music_page.h"

static PT_lv_music_player g_pt_lv_music;  // ���ݽṹ��
static bool lv_music_list_init(void);
static void event_handler_back_to_home(lv_event_t *e);
static void lv_music_player_init(void);
static void event_handler_play_btn(lv_event_t *e);
static void event_handler_prev_btn(lv_event_t *e);
static void event_handler_next_btn(lv_event_t *e);
static void event_handler_volume_slider(lv_event_t *e);
/* lv_music_player
 * snake game
 */
void lv_music_player(void)
{
    g_pt_lv_music = (T_lv_music_player*)pvPortMalloc(sizeof(T_lv_music_player));
	g_pt_lv_music->bg_music_player = lv_obj_create(lv_scr_act());/*��������*/

	lv_obj_set_size(g_pt_lv_music->bg_music_player, LV_HOR_RES, LV_VER_RES);/*���ô�С*/
	lv_obj_set_style_radius(g_pt_lv_music->bg_music_player, 0, 0);/*����Բ��*/
	lv_obj_align(g_pt_lv_music->bg_music_player, LV_ALIGN_TOP_LEFT, 0, 0);


	/*����padding*/
	set_user_style(g_pt_lv_music->bg_music_player);

	lv_obj_set_size(g_pt_lv_music->bg_music_player, LV_HOR_RES, LV_VER_RES);/*���ô�С*/
	lv_obj_set_style_bg_opa(g_pt_lv_music->bg_music_player, LV_OPA_100, 0);/*���ñ���͸����*/
	lv_obj_set_style_bg_color(g_pt_lv_music->bg_music_player, lv_color_hex(BACK_COLOR), 0);

	lv_music_player_init();

	#if 0
	if(lv_music_list_init())
    {
        lv_obj_move_foreground(g_pt_lv_music->music_list);
    }
    #endif





	add_title(g_pt_lv_music->bg_music_player, "MUSIC");
	add_back(g_pt_lv_music->bg_music_player, event_handler_back_to_home);

}

/* lv_music_player_init
 * �����ʼ��
 */
static void lv_music_player_init(void)
{
    static lv_style_t style_bg;/*������ʽ*/
	lv_style_init(&style_bg);
	lv_style_set_bg_opa(&style_bg, LV_OPA_100);/*���ñ���͸����*/
	lv_style_set_radius(&style_bg, 10);	/*����Բ��*/
	lv_style_set_bg_color(&style_bg, lv_color_hex(GREY_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&style_bg, 0);
	lv_style_set_pad_bottom(&style_bg, 0);
	lv_style_set_pad_left(&style_bg, 0);
	lv_style_set_pad_right(&style_bg, 0);
    lv_style_set_border_width(&style_bg, 0);

    static lv_style_t style_cont;/*������ʽ*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_100);/*���ñ���͸����*/
	lv_style_set_radius(&style_cont, 10);	/*����Բ��*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(BACK_COLOR));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, 0);

    /*�����ײ�������*/
    lv_obj_t *bottom_con = lv_obj_create(g_pt_lv_music->bg_music_player);
    lv_obj_set_size(bottom_con, BOTTOM_CON_W, BOTTOM_CON_H);
    lv_obj_add_style(bottom_con, &style_cont, 0);
    lv_obj_set_style_border_width(bottom_con, 0, 1);
    lv_obj_align(bottom_con, LV_ALIGN_BOTTOM_MID, 0, -5);

    /*�������ź���ͣ��ť*/
    /*����Բ������*/
    lv_obj_t *ctl_con = lv_obj_create(bottom_con);
    lv_obj_set_size(ctl_con, CTL_SIZE, CTL_SIZE);
    lv_obj_add_style(ctl_con, &style_cont, 0);
    lv_obj_set_style_pad_left(ctl_con, 0, 0);
    lv_obj_set_style_radius(ctl_con, 100, 0);
    lv_obj_set_style_opa(ctl_con, LV_OPA_100, 0);
    lv_obj_set_style_border_width(ctl_con, 0, 0);
    lv_obj_set_style_bg_color(ctl_con, lv_color_hex(WHITE_HEX), 0);
    lv_obj_center(ctl_con);
    //printf("ctl_con = %x\r\n", ctl_con);
    /*����¼�*/
    lv_obj_add_flag(ctl_con, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ctl_con, event_handler_play_btn, LV_EVENT_CLICKED, NULL);

    /*������ǩ*/
    g_pt_lv_music->btn_player_ctl = lv_label_create(ctl_con);

    lv_obj_t *label = g_pt_lv_music->btn_player_ctl;
    lv_obj_set_style_text_font(label, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(GREY_HEX), 0);
    lv_label_set_text(label, LV_SYMBOL_PAUSE);
    lv_obj_center(label);

    /*������һ��*/
    lv_obj_t *pre_con = lv_obj_create(bottom_con);
    lv_obj_set_size(pre_con, CTL_SIZE, CTL_SIZE);
    lv_obj_add_style(pre_con, &style_cont, 0);
    lv_obj_set_style_pad_all(pre_con, 0, 0);
    lv_obj_set_style_radius(pre_con, 100, 0);
    lv_obj_set_style_opa(pre_con, LV_OPA_100, 0);
    lv_obj_set_style_border_width(pre_con, 0, 0);
    lv_obj_set_style_bg_color(pre_con, lv_color_hex(BACK_COLOR), 0);
    lv_obj_align_to(pre_con, ctl_con, LV_ALIGN_OUT_LEFT_MID, -10, 0);

    /*����¼�*/
    lv_obj_add_flag(pre_con, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(pre_con, event_handler_prev_btn, LV_EVENT_CLICKED, NULL);

    lv_obj_t *pre_label = lv_label_create(pre_con);
    lv_obj_set_style_text_font(pre_label, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(pre_label, lv_color_hex(GREY_HEX), 0);
    lv_label_set_text(pre_label, LV_SYMBOL_PREV);
    lv_obj_center(pre_label);

    /*������һ��*/
    lv_obj_t *next_con = lv_obj_create(bottom_con);
    lv_obj_set_size(next_con, CTL_SIZE, CTL_SIZE);
    lv_obj_add_style(next_con, &style_cont, 0);
    lv_obj_set_style_radius(next_con, 100, 0);
    lv_obj_set_style_border_width(next_con, 0, 0);
    lv_obj_align_to(next_con, ctl_con, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    /*����¼�*/
    lv_obj_add_flag(next_con, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(next_con, event_handler_next_btn, LV_EVENT_CLICKED, NULL);


    lv_obj_t *next_label = lv_label_create(next_con);
    lv_obj_set_style_text_font(next_label, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(next_label, lv_color_hex(GREY_HEX), 0);
    lv_label_set_text(next_label, LV_SYMBOL_NEXT);
    lv_obj_center(next_label);
    /*����������*/
    lv_obj_t *volume_con = lv_obj_create(bottom_con);
    lv_obj_set_size(volume_con, CTL_SIZE, CTL_SIZE);
    lv_obj_add_style(volume_con, &style_cont, 0);
    lv_obj_set_style_radius(volume_con, 100, 0);
    lv_obj_set_style_border_width(volume_con, 0, 0);
    lv_obj_align_to(volume_con, pre_con, LV_ALIGN_OUT_LEFT_MID, -10, 0);
    /*����¼�*/
    lv_obj_add_flag(volume_con, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(volume_con, event_handler_next_btn, LV_EVENT_CLICKED, NULL);


    lv_obj_t *volume_label = lv_label_create(volume_con);
    lv_obj_set_style_text_font(volume_label, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(volume_label, lv_color_hex(GREY_HEX), 0);
    lv_label_set_text(volume_label, LV_SYMBOL_VOLUME_MAX);
    lv_obj_center(volume_label);

    /*��������������*/
    lv_obj_t *slider_con = lv_obj_create(g_pt_lv_music->bg_music_player);
    lv_obj_set_size(slider_con, SLIDER_CON_W, SLIDER_CON_H);
    lv_obj_add_style(slider_con, &style_cont, 0);
    lv_obj_set_style_radius(slider_con, 80, 0);
    lv_obj_set_style_border_width(slider_con, 0, 0);
    lv_obj_set_style_bg_color(slider_con, lv_color_hex(WHITE_HEX), 0);
    lv_obj_align(slider_con, LV_ALIGN_TOP_RIGHT, -20, 40);

    /*����������*/

    lv_obj_t *volume_slider;
	volume_slider = lv_slider_create(slider_con);
	lv_slider_set_range(volume_slider, 0, 100);/*���ù�������Χ*/

	lv_obj_set_width(volume_slider, 8);/*���ö���Ŀ��*/
	lv_obj_set_height(volume_slider, 120);/*���ö���ĸ߶�*/
	lv_obj_align(volume_slider, LV_ALIGN_TOP_MID, 0, 10);
	lv_obj_set_style_bg_color(volume_slider, lv_color_hex(GREY_HEX), LV_PART_MAIN);/*�������ı���*/
	lv_obj_set_style_bg_color(volume_slider, lv_color_hex(RED_HEX), LV_PART_INDICATOR);/*��������ָʾ��*/
	lv_obj_set_style_bg_color(volume_slider, lv_color_hex(RED_HEX), LV_PART_KNOB);/*����������ť*/
	lv_slider_set_value(volume_slider, 50, LV_ANIM_OFF);/*���õ�ǰֵ*/
	/*��ӻص�����*/
	lv_obj_add_event_cb(volume_slider, event_handler_volume_slider, LV_EVENT_RELEASED, NULL);
	/*����һ����ǩ*/



}


/* lv_music_list_init
 * �����б��ʼ��
 */
static bool lv_music_list_init(void)
{
    g_pt_lv_music->music_list = lv_list_create(g_pt_lv_music->bg_music_player);
    lv_obj_set_size(g_pt_lv_music->music_list, LV_HOR_RES, LV_MUSIC_PLAYER_LIST_HIGHT);/*���ô�С*/
    lv_obj_set_style_opa(g_pt_lv_music->music_list, LV_OPA_60, 0);/*����ɫ��͸����*/
    lv_obj_set_y(g_pt_lv_music->music_list, LV_VER_RES);
    lv_obj_set_style_text_font(g_pt_lv_music->music_list, &song_font14, 0);
    lv_obj_clean(g_pt_lv_music->music_list);

    lv_obj_t *btn;
    btn = lv_list_add_btn(g_pt_lv_music->music_list, LV_SYMBOL_AUDIO, "Letgo.mp3");
    btn = lv_list_add_btn(g_pt_lv_music->music_list, LV_SYMBOL_AUDIO, "lifeline.mp3");
    g_pt_lv_music->current_btn = btn;

    return true;
}


int play_flag = 0;

/* event_handler_back_to_home
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_back_to_home(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		if(g_pt_lv_music->bg_music_player != NULL) lv_obj_del(g_pt_lv_music->bg_music_player);

		vPortFree(g_pt_lv_music);
		//gfind_dev_state = 0;
		lv_main_page_demo_home(100);
	}
}

/* event_handler_play_btn
 * ���Ű�ť����ص�����
 */
static void event_handler_play_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	//lv_obj_t *target = lv_event_get_target(e);

	//printf("current = %x\r\n", obj);
	//printf("target = %x\r\n", target);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
	    if(play_flag == 0)/*��ͣ*/
        {
            play_flag = 1;
        }
        else
        {
            play_flag = 0;
        }
        if(play_flag)/*����*/
        {

            lv_obj_set_style_pad_left(obj, 0, 0);
            lv_label_set_text(g_pt_lv_music->btn_player_ctl, LV_SYMBOL_PAUSE);
        }
        else
        {
            lv_obj_set_style_pad_left(obj, 4, 0);
            lv_label_set_text(g_pt_lv_music->btn_player_ctl, LV_SYMBOL_PLAY);

        }

	}
}

/* event_handler_prev_btn
 * ��һ�װ�ť����ص�����
 */
static void event_handler_prev_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	//lv_obj_t *target = lv_event_get_target(e);

	//printf("current = %x\r\n", obj);
	//printf("target = %x\r\n", target);
	if(code == LV_EVENT_CLICKED)/*���*/
	{

	}
}

/* event_handler_next_btn
 * ��һ�װ�ť����ص�����
 */
static void event_handler_next_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	//lv_obj_t *target = lv_event_get_target(e);

	//printf("current = %x\r\n", obj);
	//printf("target = %x\r\n", target);
	if(code == LV_EVENT_CLICKED)/*���*/
	{

	}
}

/* event_handler_volume_btn
 * ������ť����ص�����
 */
static void event_handler_volume_btn(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_current_target(e);
	//lv_obj_t *target = lv_event_get_target(e);

	//printf("current = %x\r\n", obj);
	//printf("target = %x\r\n", target);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
        /*��ʾ������*/
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


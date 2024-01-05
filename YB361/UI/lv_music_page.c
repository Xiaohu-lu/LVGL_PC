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

static PT_lv_music_player g_pt_lv_music;  // 数据结构体
static bool lv_music_list_init(void);
static void event_handler_back_to_home(lv_event_t *e);


/* lv_music_player
 * snake game
 */
void lv_music_player(void)
{
    g_pt_lv_music = (T_lv_music_player*)pvPortMalloc(sizeof(T_lv_music_player));
	g_pt_lv_music->bg_music_player = lv_obj_create(lv_scr_act());/*创建背景*/

	lv_obj_set_size(g_pt_lv_music->bg_music_player, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_music->bg_music_player, 0, 0);/*设置圆角*/
	lv_obj_align(g_pt_lv_music->bg_music_player, LV_ALIGN_TOP_LEFT, 0, 0);


	/*设置padding*/
	set_user_style(g_pt_lv_music->bg_music_player);

	lv_obj_set_size(g_pt_lv_music->bg_music_player, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_music->bg_music_player, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_music->bg_music_player, lv_color_hex(BACK_COLOR), 0);


	if(lv_music_list_init())
    {
        lv_obj_move_foreground(g_pt_lv_music->music_list);
    }


	add_title(g_pt_lv_music->bg_music_player, "MUSIC");
	add_back(g_pt_lv_music->bg_music_player, event_handler_back_to_home);

}

/* lv_music_list_init
 * 音乐列表初始化
 */
static bool lv_music_list_init(void)
{
    g_pt_lv_music->music_list = lv_list_create(g_pt_lv_music->bg_music_player);
    lv_obj_set_size(g_pt_lv_music->music_list, LV_HOR_RES, LV_MUSIC_PLAYER_LIST_HIGHT);/*设置大小*/
    lv_obj_set_style_opa(g_pt_lv_music->music_list, LV_OPA_60, 0);/*设置色彩透明度*/
    lv_obj_set_y(g_pt_lv_music->music_list, LV_VER_RES);
    lv_obj_set_style_text_font(g_pt_lv_music->music_list, &song_font14, 0);
    lv_obj_clean(g_pt_lv_music->music_list);

    lv_obj_t *btn;
    btn = lv_list_add_btn(g_pt_lv_music->music_list, LV_SYMBOL_AUDIO, "Letgo.mp3");
    btn = lv_list_add_btn(g_pt_lv_music->music_list, LV_SYMBOL_AUDIO, "时间.mp3");
    g_pt_lv_music->current_btn = btn;

    return true;
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
		if(g_pt_lv_music->bg_music_player != NULL) lv_obj_del(g_pt_lv_music->bg_music_player);

		vPortFree(g_pt_lv_music);
		//gfind_dev_state = 0;
		lv_main_page_demo_home(100);
	}
}





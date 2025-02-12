#include "lvgl/lvgl.h"
#include "UI/lv_typedef.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "Lamp_main.h"
#include "Lamp_home.h"
#include "Lamp_audio.h"

LV_IMG_DECLARE(voice);
LV_IMG_DECLARE(ico_0);
LV_IMG_DECLARE(ico_1);
LV_IMG_DECLARE(ico_2);
LV_IMG_DECLARE(ico_3);
LV_IMG_DECLARE(ico_4);
LV_IMG_DECLARE(ico_5);
LV_IMG_DECLARE(ico_6);
LV_IMG_DECLARE(ico_7);
LV_IMG_DECLARE(ico_8);
LV_IMG_DECLARE(ico_9);
LV_IMG_DECLARE(ico_10);
LV_IMG_DECLARE(ico_11);
LV_IMG_DECLARE(ico_12);
LV_IMG_DECLARE(ico_13);
LV_IMG_DECLARE(ico_14);
LV_IMG_DECLARE(ico_15);
LV_IMG_DECLARE(ico_16);
LV_IMG_DECLARE(ico_17);
LV_IMG_DECLARE(ico_18);
LV_IMG_DECLARE(ico_19);
LV_IMG_DECLARE(ico_20);

LV_IMG_DECLARE(alarm);
LV_IMG_DECLARE(clock);
LV_IMG_DECLARE(count);
LV_IMG_DECLARE(dray);
LV_IMG_DECLARE(eye);
LV_IMG_DECLARE(pose);
LV_IMG_DECLARE(record);
LV_IMG_DECLARE(remind);
LV_IMG_DECLARE(udray);
LV_IMG_DECLARE(uray);
LV_IMG_DECLARE(volume);



Lamp_audio_t Lamp_audio_info;

extern lv_style_t bg_style;
extern lv_style_t bg_style_round;
extern lv_style_t bg_style_con;
extern lv_style_t con_style_on;
extern lv_style_t con_style_off;
extern lv_style_t circle_style;
extern lv_style_t circle_style_pitched;

/* Lamp_audio_pitchAsr
 * 根据Asr状态,显示圆圈还是对号
 * */
static void Lamp_audio_pitchAsr(void)
{
	lv_obj_t *circle = lv_obj_get_child(Lamp_audio_info.asr_img, 2);
	lv_obj_t *label_c = lv_obj_get_child(circle, 0);
	if(Lamp_audio_info.AsrOn){
		lv_obj_set_size(circle, 22, 22);
		lv_obj_add_state(circle, LV_STATE_PRESSED);
		lv_obj_clear_flag(label_c, LV_OBJ_FLAG_HIDDEN);
	}
	else{
		lv_obj_set_size(circle, 14, 14);
		lv_obj_clear_state(circle, LV_STATE_PRESSED);
		lv_obj_add_flag(label_c, LV_OBJ_FLAG_HIDDEN);
	}
}

static void Lamp_audio_pitchAsrOrVme(void)
{
     lv_obj_t *img_asr = lv_obj_get_child(Lamp_audio_info.asr_img, 0);
    lv_obj_t *label_asr = lv_obj_get_child(Lamp_audio_info.asr_img, 1);
    lv_obj_t *img_vme = lv_obj_get_child(Lamp_audio_info.vme_img, 0);
    lv_obj_t *label_vme = lv_obj_get_child(Lamp_audio_info.vme_img, 1);

    Lamp_audio_pitchAsr();
    if(Lamp_audio_info.page_id == PAGE_ASR){
    	lv_obj_add_state(Lamp_audio_info.asr_img, LV_STATE_PRESSED);
        lv_obj_add_state(img_asr, LV_STATE_PRESSED);
        lv_obj_add_state(label_asr, LV_STATE_PRESSED);
        lv_obj_clear_state(Lamp_audio_info.vme_img, LV_STATE_PRESSED);
        lv_obj_clear_state(img_vme, LV_STATE_PRESSED);
        lv_obj_clear_state(label_vme, LV_STATE_PRESSED);
    }
    else if(Lamp_audio_info.page_id == PAGE_VME)
    {
        if(Lamp_audio_info.AsrOn){
        	lv_obj_add_state(Lamp_audio_info.asr_img, LV_STATE_PRESSED);
            lv_obj_add_state(img_asr, LV_STATE_PRESSED);
            lv_obj_add_state(label_asr, LV_STATE_PRESSED);
        }
        else{
        	lv_obj_clear_state(Lamp_audio_info.asr_img, LV_STATE_PRESSED);
            lv_obj_clear_state(img_asr, LV_STATE_PRESSED);
            lv_obj_clear_state(label_asr, LV_STATE_PRESSED);
        }
        lv_obj_add_state(Lamp_audio_info.vme_img, LV_STATE_PRESSED);
        lv_obj_add_state(img_vme, LV_STATE_PRESSED);
        lv_obj_add_state(label_vme, LV_STATE_PRESSED);
    }

}





/* Lamp_audio_init
 * 语音控制界面初始化
 * */
void Lamp_audio_init(void)
{
	/*创建背景*/
	Lamp_audio_info.bg_img = lv_obj_create(lv_scr_act());
	lv_obj_set_size(Lamp_audio_info.bg_img, LAMP_DISP_HOR_RES, LAMP_DISP_VER_RES);
	lv_obj_center(Lamp_audio_info.bg_img);
	lv_obj_add_style(Lamp_audio_info.bg_img, &bg_style, LV_PART_MAIN);

	Lamp_audio_info.page_id = PAGE_ASR;
Lamp_audio_info.AsrOn = 1;
	/*语音控制*/
	Lamp_audio_info.asr_img = lv_obj_create(Lamp_audio_info.bg_img);
	lv_obj_set_size(Lamp_audio_info.asr_img, 272, 38);
	lv_obj_align(Lamp_audio_info.asr_img, LV_ALIGN_TOP_LEFT, 24, 24);
	lv_obj_add_style(Lamp_audio_info.asr_img, &con_style_on, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_add_style(Lamp_audio_info.asr_img, &con_style_off, LV_PART_MAIN);

    /*图标,child[0]*/
    lv_obj_t *img_1 = lv_img_create(Lamp_audio_info.asr_img);
    lv_img_set_src(img_1, &ico_14);
    lv_obj_align(img_1, LV_ALIGN_LEFT_MID, 15, 0);
    lv_obj_set_style_img_recolor_opa(img_1, LV_OPA_100, 0);
    lv_obj_set_style_img_recolor(img_1, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_img_recolor(img_1, lv_color_hex(0x000000), LV_STATE_PRESSED);

    /*标签,child[1]*/
    lv_obj_t *label_1 = lv_label_create(Lamp_audio_info.asr_img);
    lv_obj_set_style_text_font(label_1, &Harm_san_22, LV_PART_MAIN);
	lv_obj_set_style_text_color(label_1, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_align_to(label_1, img_1, LV_ALIGN_OUT_RIGHT_MID, 3, 0);
	lv_label_set_text(label_1, "语音控制");

    /*圆圈,child[2]*/
    lv_obj_t *circle = lv_obj_create(Lamp_audio_info.asr_img);
    lv_obj_set_size(circle, 14, 14);
    lv_obj_align(circle, LV_ALIGN_RIGHT_MID, -21, 0);
    lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
    /*创建对号*/
    lv_obj_t *label_c = lv_label_create(circle);
    lv_obj_set_style_text_color(label_c, lv_color_hex(0xFFC53D),LV_PART_MAIN);
    lv_label_set_text(label_c, LV_SYMBOL_OK);
    lv_obj_center(label_c);
    lv_obj_add_flag(label_c, LV_OBJ_FLAG_HIDDEN);

	/*音量调节*/
	Lamp_audio_info.vme_img = lv_obj_create(Lamp_audio_info.bg_img);
	lv_obj_set_size(Lamp_audio_info.vme_img, 272, 38);
	lv_obj_align_to(Lamp_audio_info.vme_img, Lamp_audio_info.asr_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 13);
    lv_obj_add_style(Lamp_audio_info.vme_img, &con_style_off, LV_PART_MAIN);
    lv_obj_add_style(Lamp_audio_info.vme_img, &con_style_on, LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_t *img_2 = lv_img_create(Lamp_audio_info.vme_img);
    lv_img_set_src(img_2, &ico_13);
    lv_obj_align(img_2, LV_ALIGN_LEFT_MID, 15, 0);
    lv_obj_set_style_img_recolor_opa(img_2, LV_OPA_100, 0);
    lv_obj_set_style_img_recolor(img_2, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_img_recolor(img_2, lv_color_hex(0x000000), LV_STATE_PRESSED);

    lv_obj_t *label_2 = lv_label_create(Lamp_audio_info.vme_img);
    lv_obj_set_style_text_font(label_2, &Harm_san_22, LV_PART_MAIN);
	lv_obj_set_style_text_color(label_2, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_align_to(label_2, img_2, LV_ALIGN_OUT_RIGHT_MID, 3, 0);
	lv_label_set_text(label_2, "音量调节");

	lv_obj_t *label_3 = lv_label_create(Lamp_audio_info.vme_img);
	lv_obj_align(label_3, LV_ALIGN_RIGHT_MID, -24, 0);
	lv_obj_set_style_text_color(label_3, lv_color_hex(0xFFFFFF),LV_PART_MAIN);
    lv_label_set_text(label_3, LV_SYMBOL_RIGHT);

	Lamp_audio_pitchAsrOrVme();
}


/* Lamp_audio_volume_page
 * 音量控制界面
 * */
void Lamp_audio_volume_page(void)
{
	/*创建背景*/
	Lamp_audio_info.volume_page = lv_obj_create(lv_scr_act());
	lv_obj_set_size(Lamp_audio_info.volume_page, LAMP_DISP_HOR_RES, LAMP_DISP_VER_RES);
	lv_obj_center(Lamp_audio_info.volume_page);
	lv_obj_add_style(Lamp_audio_info.volume_page, &bg_style_round, LV_PART_MAIN);

	/*创建标签,第0个child*/
	#if 0
	lv_obj_t *label = lv_label_create(Lamp_audio_info.volume_page);
	lv_obj_set_style_text_font(label, &Lamp_font32, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_label_set_text_fmt(label, "%d\%%", Lamp_audio_info.Volume);
	lv_obj_align(label, LV_ALIGN_TOP_LEFT, 163, 128);
#endif
	/*创建圆弧,第1个child*/
	lv_obj_t *arc = lv_arc_create(Lamp_audio_info.volume_page);
	lv_obj_set_size(arc, 142, 142);
	lv_arc_set_angles(arc, 0, 360);
	lv_arc_set_bg_angles(arc, 0, 360);
	lv_obj_set_style_arc_width(arc, 12, LV_PART_INDICATOR);
	lv_obj_set_style_arc_width(arc, 12, LV_PART_MAIN);
	lv_obj_set_style_arc_color(arc, lv_color_hex(0xFFE58F), LV_PART_INDICATOR);
	lv_obj_set_style_arc_color(arc, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
	lv_arc_set_mode(arc, LV_ARC_MODE_REVERSE);
	lv_arc_set_rotation(arc, 180);
	lv_arc_set_value(arc, 100);
	lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_align(arc, LV_ALIGN_TOP_LEFT, 89, 48);

    Lamp_audio_info.Volume = 75;
	lv_obj_t *label = lv_label_create(arc);
	lv_obj_set_style_text_font(label, &Lamp_font32, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_label_set_text_fmt(label, "%d\%%", Lamp_audio_info.Volume);
	lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 54);


	/*添加图片*/
	lv_obj_t *img_1 = lv_img_create(Lamp_audio_info.volume_page);
	lv_img_set_src(img_1, &volume);
	lv_obj_align(img_1, LV_ALIGN_TOP_LEFT, 21, 21);

	/*创建圆形*/
	lv_obj_t *circle = lv_obj_create(arc);
	lv_obj_set_size(circle, 19, 19);
    lv_obj_align(circle, LV_ALIGN_BOTTOM_MID, 0, -23);
    lv_obj_add_style(circle, &circle_style_pitched, LV_PART_MAIN);
    lv_obj_set_style_bg_color(circle, lv_color_hex(0x1D1D1D), LV_PART_MAIN);
    lv_obj_clear_flag(circle, LV_OBJ_FLAG_SCROLLABLE);
    /*添加图片*/
    lv_obj_t *img_circle = lv_img_create(circle);
    lv_img_set_src(img_circle, &ico_13);
    lv_img_set_zoom(img_circle, 180);
    lv_obj_set_style_img_recolor_opa(img_circle, LV_OPA_100, 0);
    lv_obj_set_style_img_recolor(img_circle, lv_color_hex(0x8C8C8C), LV_PART_MAIN);
    lv_obj_center(img_circle);

}

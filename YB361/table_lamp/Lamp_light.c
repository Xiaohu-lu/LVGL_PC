#include "lvgl/lvgl.h"
#include "UI/lv_typedef.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "Lamp_main.h"
#include "Lamp_home.h"
#include "Lamp_audio.h"
#include "Lamp_light.h"


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

extern lv_style_t bg_style;
extern lv_style_t bg_style_round;
extern lv_style_t bg_style_con;
extern lv_style_t con_style_on;
extern lv_style_t con_style_off;
extern lv_style_t circle_style;
extern lv_style_t circle_style_pitched;

Lamp_light_t Lamp_light_info;


static const char *img_mode_ico[] = \
{
	"S:/img_src/ico/ico_5.png", "S:/img_src/ico/ico_6.png", "S:/img_src/ico/ico_7.png", "S:/img_src/ico/ico_8.png", "S:/img_src/ico/ico_9.png",
};

static const char *label_mode_txt[] = \
{
    "书写模式", "网课模式", "入座感应", "智能感光", "夜灯伴睡",
};

static void Lamp_light_add_mode_item(uint8_t mode)
{
    /*创建容器*/
    lv_obj_t *con = lv_obj_create(Lamp_light_info.bg_img);
    lv_obj_set_size(con, 272, 38);
    lv_obj_add_style(con, &con_style_on, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_add_style(con, &con_style_off, LV_PART_MAIN);
    lv_obj_align(con, LV_ALIGN_TOP_LEFT, 24, 24 + (51 * mode));

    /*图标,child[0]*/
    lv_obj_t *img_1 = lv_img_create(con);
    lv_img_set_src(img_1, img_mode_ico[mode]);
    lv_obj_align(img_1, LV_ALIGN_LEFT_MID, 15, 0);
    lv_obj_set_style_img_recolor_opa(img_1, LV_OPA_100, 0);
    lv_obj_set_style_img_recolor(img_1, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_img_recolor(img_1, lv_color_hex(0x000000), LV_STATE_PRESSED);

    /*标签,child[1]*/
	lv_obj_t *label_1 = lv_label_create(con);
	lv_obj_set_style_text_font(label_1, &Harm_san_22, LV_PART_MAIN);
	lv_obj_set_style_text_color(label_1, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_align_to(label_1, img_1, LV_ALIGN_OUT_RIGHT_MID, 3, 0);
	lv_label_set_text(label_1, label_mode_txt[mode]);
    if(mode == LIGHT_NIGHT_MODE){/*夜灯模式*/
        /*显示一个RIGHT符号*/
        lv_obj_t *label_3 = lv_label_create(con);
        lv_obj_align(label_3, LV_ALIGN_RIGHT_MID, -24, 0);
        lv_obj_set_style_text_color(label_3, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_label_set_text(label_3, LV_SYMBOL_RIGHT);
    }
    else{
        /*圆圈,child[2]*/
        lv_obj_t *circle = lv_obj_create(con);
        lv_obj_set_size(circle, 14, 14);
        lv_obj_align(circle, LV_ALIGN_RIGHT_MID, -21, 0);
        lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
        lv_obj_add_style(circle, &circle_style_pitched, LV_PART_MAIN | LV_STATE_PRESSED);

        /*创建对号*/
        lv_obj_t *label_c = lv_label_create(circle);
        lv_obj_set_style_text_color(label_c, lv_color_hex(0xFFC53D),LV_PART_MAIN);
        lv_label_set_text(label_c, LV_SYMBOL_OK);
        lv_obj_center(label_c);
        lv_obj_add_flag(label_c, LV_OBJ_FLAG_HIDDEN);

    }


}


void Lamp_light_init(void)
{
    uint8_t i;
    /*创建背景*/
	Lamp_light_info.bg_img = lv_obj_create(lv_scr_act());
	lv_obj_set_size(Lamp_light_info.bg_img, LAMP_DISP_HOR_RES, LAMP_DISP_VER_RES);
	lv_obj_center(Lamp_light_info.bg_img);
	lv_obj_add_style(Lamp_light_info.bg_img, &bg_style, LV_PART_MAIN);
	Lamp_light_info.page_id = LIGHT_MODE_PAGE;

    for(i = 0; i <= LIGHT_NIGHT_MODE; i++)
    {
       Lamp_light_add_mode_item(i);
    }
    lv_obj_set_scrollbar_mode(Lamp_light_info.bg_img, LV_SCROLLBAR_MODE_OFF);
    lv_obj_scroll_to_y(Lamp_light_info.bg_img, 24, LV_ANIM_ON);
}



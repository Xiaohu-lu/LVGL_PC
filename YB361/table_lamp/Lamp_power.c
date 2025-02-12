#include "lvgl/lvgl.h"
#include "UI/lv_typedef.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "Lamp_main.h"
#include "Lamp_home.h"
#include "Lamp_audio.h"
#include "Lamp_power.h"

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

LV_IMG_DECLARE(lum_d);
LV_IMG_DECLARE(lum_u);
LV_IMG_DECLARE(lum_m);


static const void *img_left_ico[] = \
{
	&udray, &dray, &udray,
};

extern lv_style_t bg_style;
extern lv_style_t bg_style_round;
extern lv_style_t bg_style_con;

Lamp_power_t Lamp_power_info;


/* Lamp_power_SetLightMode
 * 设置灯光模式
 * mode:0:上下发光;1:下发光;2:上发光;
 * d_value:5~100,下发光的亮度
 * u_value:5~100,上发光的亮度
 * */
void Lamp_power_SetLightMode(uint8_t mode, uint8_t d_value, uint8_t u_value)
{
	if(mode > POWER_MODE_UP){
		Lamp_power_info.Light_mode = POWER_MODE_UPDOWN;
	}
	else{
		Lamp_power_info.Light_mode = mode;
	}
	/*下发光亮度*/
	if(d_value < 5){
		Lamp_power_info.Down_value = 5;
	}
	else if(d_value > 100){
		Lamp_power_info.Down_value = 100;
	}
	else{
		Lamp_power_info.Down_value = d_value;
	}
	/*上发光亮度*/
	if(u_value < 5){
		Lamp_power_info.Up_value = 5;
	}
	else if(u_value > 100){
		Lamp_power_info.Up_value = 100;
	}
	else{
		Lamp_power_info.Up_value = u_value;
	}

}

/* Lamp_power_draw_first_level_page
 * 绘制第一级界面
 * */
static void Lamp_power_draw_first_level_page(void)
{
	/*左上角图标*/
	lv_obj_t *img_l = lv_obj_get_child(Lamp_power_info.bg_img, 0);
	/*标签容器*/
	lv_obj_t *img_b = lv_obj_get_child(Lamp_power_info.bg_img, 1);
	/*标签*/
	lv_obj_t *label = lv_obj_get_child(img_b, 0);
	/*上面亮度图片*/
	lv_obj_t *img_u = lv_obj_get_child(Lamp_power_info.bg_img, 2);
	/*下面亮度图片*/
	lv_obj_t *img_d = lv_obj_get_child(Lamp_power_info.bg_img, 3);

	/*左上图片*/
	lv_img_set_src(img_l, img_left_ico[Lamp_power_info.Light_mode]);


	switch(Lamp_power_info.Light_mode)
	{
	case POWER_MODE_UPDOWN:/*上下发光*/
		/*显示lamp_d和lamp_u*/
		lv_obj_clear_flag(img_d, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(img_u, LV_OBJ_FLAG_HIDDEN);
		lv_label_set_text(label, "上下发光");
		break;
	case POWER_MODE_DOWN:/*下发光*/
		/*显示lamp_d,不显示lamp_u*/
		lv_obj_clear_flag(img_d, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(img_u, LV_OBJ_FLAG_HIDDEN);
		lv_label_set_text(label, "下发光");
		break;
	case POWER_MODE_UP:/*上发光*/
		/*不显示lamp_d,显示lamp_u*/
		lv_obj_add_flag(img_d, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(img_u, LV_OBJ_FLAG_HIDDEN);
		lv_label_set_text(label, "上发光");
		break;
	}
}

/* Lamp_power_init
 * 开关按键界面初始化
 * */
void Lamp_power_init(void)
{
	/*创建背景*/
	Lamp_power_info.bg_img = lv_obj_create(lv_scr_act());
	lv_obj_set_size(Lamp_power_info.bg_img, LAMP_DISP_HOR_RES, LAMP_DISP_VER_RES);
	lv_obj_center(Lamp_power_info.bg_img);
	lv_obj_add_style(Lamp_power_info.bg_img, &bg_style, LV_PART_MAIN);

	Lamp_power_SetLightMode(0, 0, 0);

	/*左上角图标child[0]*/
	lv_obj_t *img_left = lv_img_create(Lamp_power_info.bg_img);
	lv_obj_align(img_left, LV_ALIGN_TOP_LEFT, 21, 21);

	/*下面标签容器child[1]*/
	lv_obj_t *img_bottom = lv_obj_create(Lamp_power_info.bg_img);
	lv_obj_set_size(img_bottom, 122, 28);
	lv_obj_align(img_bottom, LV_ALIGN_TOP_LEFT, 95, 179);
    lv_obj_add_style(img_bottom, &bg_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(img_bottom, lv_color_hex(0x2A2A2A), LV_PART_MAIN);
    lv_obj_set_style_radius(img_bottom, 40, LV_PART_MAIN);
    /*创建标签*/
    lv_obj_t *label = lv_label_create(img_bottom);
    lv_obj_set_style_text_font(label, &Harm_san_18, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(label);


	/*上面光亮度图片child[2]*/
	lv_obj_t *img_top = lv_img_create(Lamp_power_info.bg_img);
	lv_img_set_src(img_top, &lum_u);
	lv_obj_align(img_top, LV_ALIGN_TOP_LEFT, 76, 24);

	/*下面亮度图片child[3]*/
	lv_obj_t *img_b = lv_img_create(Lamp_power_info.bg_img);
	lv_img_set_src(img_b,  &lum_d);
	lv_obj_align(img_b, LV_ALIGN_TOP_LEFT, 72, 97);

	/*台灯中间图片child[4]*/
	lv_obj_t *img_m = lv_img_create(Lamp_power_info.bg_img);
	lv_img_set_src(img_m,  &lum_m);
	lv_obj_align(img_m, LV_ALIGN_TOP_LEFT, 86, 81);

	/*根据当前灯光模式*/
	Lamp_power_draw_first_level_page();
	Lamp_power_info.page_id = POWER_PAGE_MODE;
}

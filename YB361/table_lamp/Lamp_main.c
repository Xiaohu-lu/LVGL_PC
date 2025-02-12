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


LV_IMG_DECLARE(Pose_on);
LV_IMG_DECLARE(Pose_off);
LV_IMG_DECLARE(Rest_on);
LV_IMG_DECLARE(Rest_off);
LV_IMG_DECLARE(Eye_on);
LV_IMG_DECLARE(Eye_off);
LV_IMG_DECLARE(record_on);
LV_IMG_DECLARE(Record_off);

/* 桌标图标数组 */
static const lv_img_dsc_t *First_level_on[] = \
{ \
	&Pose_on,	&Rest_on,	&Eye_on,	&record_on,
};

static const lv_img_dsc_t *First_level_off[] = \
{ \
	&Pose_off,	&Rest_off,	&Eye_off,	&Record_off,
};

Lamp_Fun_t Lamp_Info;	/*界面信息*/

lv_obj_t *First_Img[4];
lv_obj_t *First_label[4];

/*圆形矩形*/

static lv_style_t bg_style;
static lv_style_t bg_style_round;
static lv_style_t con_style_on;
static lv_style_t con_style_off;
static lv_style_t con_style;

static void lamp_first_level_page_init(void)
{
	lv_obj_t *bg;
	uint8_t i;
	bg = Lamp_Info.bg_img;
	Lamp_Info.func_id = Pose_Remind_m;

	/*给背景添加样式*/
	lv_obj_add_style(bg, &bg_style_round, LV_PART_MAIN);

	#if 0
	for(i = 0; i < 4; i++){
		First_Img[i] = lv_img_create(bg);
		/*选着图片源,开启还是关闭*/
		if(Lamp_Info.func_id == i){
			lv_img_set_src(First_Img[i], First_level_on[i]);
		}
		else{
			lv_img_set_src(First_Img[i], First_level_off[i]);
		}
		/*对齐*/
		if(i == 0){
			lv_obj_align(First_Img[i], LV_ALIGN_TOP_MID, 0, 32);
		}
		else{
			lv_obj_align_to(First_Img[i], First_Img[i -1], LV_ALIGN_OUT_BOTTOM_MID, 0, 16);
		}
	}
	#endif // 0

	First_Img[0] = lv_img_create(bg);
	lv_img_set_src(First_Img[0], First_level_on[0]);
	lv_obj_align(First_Img[0], LV_ALIGN_TOP_MID, 0, 32);


	First_Img[1] = lv_obj_create(bg);
	lv_obj_set_size(First_Img[1], 340, 47);
    lv_obj_add_style(First_Img[1], &bg_style_round, LV_PART_MAIN);
    lv_obj_align_to(First_Img[1], First_Img[0], LV_ALIGN_OUT_BOTTOM_MID, 0, 16);
    lv_obj_add_style(First_Img[1], &con_style_on, LV_PART_MAIN);

#if 1
    lv_obj_t *con = lv_obj_create(First_Img[1]);
    lv_obj_set_size(con, 32 * 4, 32);
    lv_obj_add_style(con, &con_style, LV_PART_MAIN);
    lv_obj_align(con, LV_ALIGN_LEFT_MID, 52, 0);
#endif
    #if 1
    First_label[1] = lv_label_create(con);


    lv_label_set_text(First_label[1], "坐姿提醒");
    lv_obj_set_style_text_color(First_label[1], lv_color_hex(0x000000), LV_PART_MAIN);
    //lv_obj_set_style_text_font(label, &Lamp_font32, LV_PART_MAIN);
    lv_obj_set_style_text_font(First_label[1], &Lamp_font24, LV_PART_MAIN);
    lv_obj_align(First_label[1], LV_ALIGN_LEFT_MID, 0, 0);



#endif

}


static void lamp_pose_record_day(void)
{
    lv_obj_t *bg;
	uint8_t i;
	bg = Lamp_Info.bg_img;

	/*给背景添加样式*/
	lv_obj_add_style(bg, &bg_style_round, LV_PART_MAIN);

    /*下面"日报"标签*/
    lv_label_t *label_bottom = lv_label_create(bg);
    lv_label_set_text(label_bottom, "123");
    lv_obj_set_style_text_color(label_bottom, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label_bottom, &Lamp_font18, LV_PART_MAIN);
    lv_obj_align(label_bottom, LV_ALIGN_TOP_LEFT, 185, 254);

}




static void lamp_first_level_page_update(void)
{
	uint8_t i;
	for(i = 0; i < 4; i++){
		/*选着图片源,开启还是关闭*/
		if(Lamp_Info.func_id == i){
			lv_img_set_src(First_Img[i], First_level_on[i]);
		}
		else{
			lv_img_set_src(First_Img[i], First_level_off[i]);
		}
	}

}


void lamp_main_page_init(void)
{
    #if 1
	Lamp_Info.bg_img = lv_obj_create(lv_scr_act());
	lv_obj_set_size(Lamp_Info.bg_img, 400, 300);
	lv_obj_center(Lamp_Info.bg_img);
#endif
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


	/*设置背景样式*/
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

	lv_style_init(&con_style);
	lv_style_set_bg_opa(&con_style, LV_OPA_0);/*设置背景透明度*/
	lv_style_set_radius(&con_style, 0);/*设置圆角*/
	lv_style_set_bg_color(&con_style, lv_color_hex(0xffffff));/*设置背景颜色,lv_color_hex()RGB888*/
	lv_style_set_pad_top(&con_style, 0);
	lv_style_set_pad_bottom(&con_style, 0);
	lv_style_set_pad_left(&con_style, 0);
	lv_style_set_pad_right(&con_style, 0);
	lv_style_set_border_width(&con_style, 0);


	//lv_obj_add_style(Lamp_Info.bg_img, &bg_style_round, LV_PART_MAIN);
#if 0
	lv_obj_t *label_1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label_1, "中国智造");
    lv_obj_set_style_text_color(label_1, lv_color_hex(0xffd666), 0);
    lv_obj_set_style_text_font(label_1, &Lamp_font24, 0);
    lv_obj_align(label_1, LV_ALIGN_BOTTOM_MID, 0, 0);
#endif
	//lamp_first_level_page_init();
	lamp_pose_record_day();
}

/* lamp_deal_left
 * 处理左旋转
 * */
void lamp_deal_left(void)
{
	Lamp_Info.last_id = Lamp_Info.func_id;/*更新上次的id*/
	/*1级菜单*/
	if(0 == Lamp_Info.func_id){/*当前菜单的第一个页面*/
		Lamp_Info.func_id = (First_Level_end - 1);
	}
	else if(Lamp_Info.func_id < (First_Level_end)){
		Lamp_Info.func_id -= 1;
	}


	LOG_D("deal left");
	/*更新图片*/
	if(Lamp_Info.func_id < First_Level_end){/*1级菜单*/
		lamp_first_level_page_update();

	}

}

/* lamp_deal_left
 * 处理右旋转
 * */
void lamp_deal_right(void)
{
	/*1级菜单*/
	if((First_Level_end - 1) == Lamp_Info.func_id){/*当前菜单的最后一个页面*/
		Lamp_Info.func_id = 0;
	}
	else if(Lamp_Info.func_id < (First_Level_end - 1)){
		Lamp_Info.func_id += 1;
	}

	LOG_D("deal right");
	/*更新图片*/
	if(Lamp_Info.func_id < First_Level_end){/*1级菜单*/
		lamp_first_level_page_update();

	}

}

/* lamp_deal_left
 * 处理确认
 * */
void lamp_deal_affirm(void)
{

}

/* lamp_deal_left
 * 处理返回
 * */
void lamp_deal_back(void)
{

}






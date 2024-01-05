/*
 * lv_boot_animation.c
 *
 *  Created on: Dec 12, 2023
 *      Author: hxd
 */
/* lvgl
 * */
#include "UI/lv_typedef.h"
#include "lvgl/lvgl.h"




LV_IMG_DECLARE(img_lv_100ask_demo_logo);
static const lv_img_dsc_t *anim_imgs[1] = {
		&img_lv_100ask_demo_logo
};

void lv_wait_ainmation(uint32_t delay);
/* lv_boot_animation
 * 开机动画
 * delay:持续时间
 * */
void lv_boot_animation(uint32_t delay)
{
    #if 0
	LV_IMG_DECLARE(img_lv_100ask_demo_logo);
	/*创建一个图片*/
	lv_obj_t *logo = lv_img_create(lv_scr_act());
	lv_img_set_src(logo, &img_lv_100ask_demo_logo);
	/*设置对齐方式*/
	lv_obj_align(logo, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t *animimg0 = lv_animimg_create(lv_scr_act());
	lv_obj_center(animimg0);
	lv_animimg_set_src(animimg0, (lv_img_dsc_t**)anim_imgs, 1);
	lv_animimg_set_duration(animimg0, 1000);
	lv_animimg_set_repeat_count(animimg0, 10);
	lv_animimg_start(animimg0);

#endif // 0
    lv_wait_ainmation(delay);
}

static lv_obj_t *bg;
static lv_obj_t *gif_bg;
static lv_timer_t *timer;
static void lv_wait_time_cb(lv_timer_t * timer)
{
    /*删除背景,图片和定时器*/
    if(gif_bg)
    {
        lv_obj_del(gif_bg);
        gif_bg = NULL;
    }
    if(bg)
    {

        lv_obj_del(bg);
        bg = NULL;
    }
    if(timer)
    {
        lv_timer_del(timer);
        timer = NULL;
    }
}

/*
 */
void lv_wait_ainmation(uint32_t delay)
{
    LV_IMG_DECLARE(waitting);
    LV_IMG_DECLARE(aixin);
    LV_IMG_DECLARE(hutao);
    /*创建背景*/
    bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(bg, LV_HOR_RES, LV_VER_RES);/*设置大小*/
    lv_obj_set_style_bg_opa(bg, 0, 0);

    gif_bg = lv_gif_create(bg);
    lv_gif_set_src(gif_bg, &hutao);
    lv_obj_center(gif_bg);

    /*创建定时器*/
    timer = lv_timer_create(lv_wait_time_cb, delay, NULL);

}



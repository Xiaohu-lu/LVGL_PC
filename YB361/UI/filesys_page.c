/*
 * filesys_page.c
 *
 *  Created on: Mar 14, 2024
 *      Author: hxd
 */
#include "lv_typedef.h"
#include "lvgl/lvgl.h"
#include "UI/lv_main_page.h"
#include "UI/lv_about_page.h"
#include "UI/filesys_page.h"
#include "UI/lv_dev_info_page.h"

static PT_lv_filesys g_pt_lv_filesys;

static void file_sys_page_init(void);
static void event_handler_back_to_home(lv_event_t *e);
/* file_sys_page
 * �ļ�ϵͳ����
 * */
void file_sys_page(void)
{
	g_pt_lv_filesys = (T_lv_filesys*)pvPortMalloc(sizeof(T_lv_filesys));
	g_pt_lv_filesys->bg_about = lv_obj_create(lv_scr_act());/*��������*/

	lv_obj_set_style_radius(g_pt_lv_filesys->bg_about, 0, 0);/*����Բ��*/
	lv_obj_align(g_pt_lv_filesys->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);

	set_user_style(g_pt_lv_filesys->bg_about);

	lv_obj_set_size(g_pt_lv_filesys->bg_about, LV_HOR_RES, LV_VER_RES);/*���ô�С*/
	lv_obj_set_style_bg_opa(g_pt_lv_filesys->bg_about, LV_OPA_100, 0);/*���ñ���͸����*/
	lv_obj_set_style_bg_color(g_pt_lv_filesys->bg_about, lv_color_white(), 1);
	file_sys_page_init();/*�����ʼ��*/

	add_title(g_pt_lv_filesys->bg_about, "FILESYSTEM");
	add_back(g_pt_lv_filesys->bg_about, event_handler_back_to_home);

}

/* file_sys_page_init
 * �����ʼ��
 * */
static void file_sys_page_init(void)
{

	int fre_clust, fre_sect, tot_sect;


	/* ��ȡSD���ܵĿռ��С�Լ�ʣ��ռ��С
	 * ���㷽����
	 * �ܿռ��С(MB) = ������ * ÿ�ص������� / 2 / 1024
	 * ʣ��ռ��С(MB) = ���д��� * ÿ�ص������� / 2 /1024
	 * */
	tot_sect = 15 * 1024;
	fre_sect = 13 * 1024;

	printf("total = %d, fre_sect = %d\r\n", tot_sect, fre_sect);

	static lv_style_t style_cont;/*������ʽ*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_0);/*���ñ���͸����*/
	lv_style_set_radius(&style_cont, 10);	/*����Բ��*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, 1);



    /*����3��120*120������*/
    lv_obj_t *cont_pic = lv_obj_create(g_pt_lv_filesys->bg_about);
    lv_obj_add_style(cont_pic, &style_cont, 0);
    lv_obj_set_size(cont_pic, 80, 80);
    lv_obj_align(cont_pic, LV_ALIGN_TOP_LEFT, 20, 50);

    lv_obj_t *cont_vido = lv_obj_create(g_pt_lv_filesys->bg_about);
    lv_obj_add_style(cont_vido, &style_cont, 0);
    lv_obj_set_size(cont_vido, 80, 80);
    lv_obj_align_to(cont_vido, cont_pic, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    lv_obj_t *cont_audio = lv_obj_create(g_pt_lv_filesys->bg_about);
    lv_obj_add_style(cont_audio, &style_cont, 0);
    lv_obj_set_size(cont_audio, 80, 80);
    lv_obj_align_to(cont_audio, cont_vido, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
}

/* event_handler_back_to_home
 * ���淵�غ���
 * */
static void event_handler_back_to_home(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		if(g_pt_lv_filesys->bg_about != NULL) lv_obj_del(g_pt_lv_filesys->bg_about);
		vPortFree(g_pt_lv_filesys);
		lv_main_page_demo_home(100);
	}
}


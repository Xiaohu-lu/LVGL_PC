/*
 * filesys_page.h
 *
 *  Created on: Mar 14, 2024
 *      Author: hxd
 */

#ifndef FILESYS_PAGE_H_
#define FILESYS_PAGE_H_


typedef struct _lv_filesys{
	lv_obj_t *bg_about;				/*背景*/
	lv_obj_t *about_table;			/*展示文字*/
	lv_obj_t *qr_official_website;	/*二维码框(官网)*/
	lv_obj_t *qr_wechat;			/*二维码(微信公众号)*/
	lv_obj_t *qr_100ask_lvgl;		/*二维码(lvgl中文站)*/

}T_lv_filesys, *PT_lv_filesys;


void file_sys_page(void);

#endif /* FILESYS_PAGE_H_ */

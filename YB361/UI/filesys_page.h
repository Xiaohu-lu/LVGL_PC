/*
 * filesys_page.h
 *
 *  Created on: Mar 14, 2024
 *      Author: hxd
 */

#ifndef FILESYS_PAGE_H_
#define FILESYS_PAGE_H_


typedef struct _lv_filesys{
	lv_obj_t *bg_about;				/*����*/
	lv_obj_t *about_table;			/*չʾ����*/
	lv_obj_t *qr_official_website;	/*��ά���(����)*/
	lv_obj_t *qr_wechat;			/*��ά��(΢�Ź��ں�)*/
	lv_obj_t *qr_100ask_lvgl;		/*��ά��(lvgl����վ)*/

}T_lv_filesys, *PT_lv_filesys;


void file_sys_page(void);

#endif /* FILESYS_PAGE_H_ */

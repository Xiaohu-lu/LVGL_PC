/*
 * lv_about_page.h
 *
 *  Created on: Dec 13, 2023
 *      Author: hxd
 */

#ifndef LV_ABOUT_PAGE_H_
#define LV_ABOUT_PAGE_H_


#define LV_ABOUT_US_TABLE_SPACE			(LV_HOR_RES/6)
#define LV_ABOUT_US_TITLE_SPACE			(45)
#define LV_ABOUT_QRCODE_SIZE			(100)
#define LV_ABOUT_QRCODE_AD_SIZE			(80)
#define LV_ABOUT_QRCODE_SPACE			((LV_HOR_RES - (LV_ABOUT_QRCODE_AD_SIZE * 3)) / 4)


typedef struct _lv_about{
	lv_obj_t *bg_about;				/*背景*/
	lv_obj_t *about_table;			/*展示文字*/
	lv_obj_t *qr_official_website;	/*二维码框(官网)*/
	lv_obj_t *qr_wechat;			/*二维码(微信公众号)*/
	lv_obj_t *qr_100ask_lvgl;		/*二维码(lvgl中文站)*/

}T_lv_about, *PT_lv_about;


void lv_board_about(void);
#endif /* LV_ABOUT_PAGE_H_ */

/*
 * lv_record_info_page.h
 *
 *  Created on: Dec 16, 2023
 *      Author: hxd
 */

#ifndef LV_RECORD_INFO_PAGE_H_
#define LV_RECORD_INFO_PAGE_H_


#define LV_METER_US_TABLE_SPACE     (LV_HOR_RES/6)
#define LV_METER_US_TITLE_SPACE     (45)
#define LV_METER_US_LABEL_SPACE     (5)

#define LV_METER_US_CONT_W		    (300)
#define LV_METER_US_CONT_H		    (400)


#define LV_METER_TABLE_COL_WIDTH     (150)
#define LV_METER_TABLE_HEIGHT        (260)

#define LV_METER_SIZE                (180)


#define LV_BIG_CONT_W                 (60)
#define LV_BIG_CONT_H                 (20)

#define LV_LIT_CONT_W                 (24)
#define LV_LIT_CONT_H                 (14)

typedef struct _lv_meter {
	lv_obj_t  * bg_about;			// ±³¾°
	lv_obj_t  * meter;				// ÒÇ±í
	lv_obj_t  * about_table;		// ±í¸ñ
} T_lv_meter, *PT_lv_meter;


#endif /* LV_RECORD_INFO_PAGE_H_ */

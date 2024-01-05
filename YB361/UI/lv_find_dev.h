/*
 * lv_find_dev.h
 *
 *  Created on: Dec 15, 2023
 *      Author: hxd
 */

#ifndef LV_FIND_DEV_H_
#define LV_FIND_DEV_H_


#define LV_FIND_US_TABLE_SPACE  (LV_HOR_RES/6)
#define LV_FIND_US_TITLE_SPACE  (45)
#define LV_FIND_US_LABEL_SPACE  (5)

#define LV_FIND_US_CONT_W		(300)
#define LV_FIND_US_CONT_H		(420)

#define LV_FIND_BTN_W			(280)
#define LV_FIND_BTN_H			(40)

#define LV_FIND_LIST_W			(280)
#define LV_FIND_LIST_H			(300)

typedef struct _lv_find_dev {
	lv_obj_t  * bg_about;				// 背景
	lv_obj_t  * find_btn;				// 查找按钮
	lv_obj_t  * dev_list;				// 设备列表
	lv_obj_t  * select_btn;				// 绑定设备按钮
	lv_obj_t  * last_btn;
} T_lv_find_dev, *PT_lv_find_dev;




void lv_find_dev(void);
void lv_find_dev_refresh(void);

#endif /* LV_FIND_DEV_H_ */

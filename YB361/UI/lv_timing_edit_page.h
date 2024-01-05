/*
 * lv_timing_edit_page.h
 *
 *  Created on: Dec 20, 2023
 *      Author: hxd
 */

#ifndef LV_TIMING_EDIT_PAGE_H_
#define LV_TIMING_EDIT_PAGE_H_

#define LV_TIME_TITLE_SPACE         (50)

#define LV_ROLLER_OBJ_W             (140)
#define LV_ROLLER_OBJ_H             (80)

#define LV_ROLLER_ALIGN_H           (40)

#define LV_CONT_ALIGN_H             (LV_ROLLER_ALIGN_H + 140 + LV_TIME_TITLE_SPACE)
#define LV_BIG_CONT_W               (310)
#define LV_BIG_CONT_H               (130)
#define LV_CONT_W                   (300)
#define LV_CONT_H                   (40)

#define LV_DEL_BTN_W                (160)
#define LV_DEL_BTN_H                (28)

#define LV_REP_CONT_W               (300)
#define LV_REP_CONT_H               (300)
#define LV_REP_CONT_L_W             (300)
#define LV_REP_CONT_L_H             (40)

#define LV_REP_CONT_CANCEL_BTN_W    (260)
#define LV_REP_CONT_CANCEL_BTN_H    (40)

#define LV_WEEK_CANCEL_BTN_W        (120)
#define LV_WEEK_CANCEL_BTN_H        (40)

#define LV_REP_BIG_CONT_SIZE        (20)
#define LV_REP_SML_CONT_SIZE        (10)


#define LV_CONT_BG_COLOR            (LIGHTGRAY_HEX)

#define LV_CONT_LABEL_ALIGN_X       (5)

#define LV_REP_CONT_ALIGN_Y         (20)        /*rep容器对于屏幕底部偏移*/
#define LV_REP_CANCEL_BTN_ALIGN_Y   (10)        /*rep容器上cancel按钮bottom底部偏移*/

#define LV_SQUARE_SIZE              (20)



#define LV_SET_CALENDAR_W           (280)
#define LV_SET_CALENDAR_H           (260)

#define LV_SET_LIST_W               (280)
#define LV_SET_LIST_H               (240)

typedef enum
{
    NOT_REP = 0,
    EVERY_DAY,
    EVERY_WEEK,
    EVERY_MONTH,
    EVERY_YEAR,
    LAST_TYPE

}Rep_type_t;

typedef enum
{
    PAGE_REP = 0,
    PAGE_WEEK,
    LAST_PAGE
}Page_type_t;

typedef enum
{
    MONDAY = 0,
    TUESDAY,
    WEDNESDAY,
    THURSDAT,
    FRIDAY,
    SATURDAY,
    SUNDAY,
    LAST_DAT,
}Week_type_t;


typedef struct _lv_timingedit {
    /*编辑页面*/
	lv_obj_t  * bg_about;				// 背景
	lv_obj_t  * hour_roller;            //时滚筒
	lv_obj_t  * minute_roller;          //分钟滚筒
	//lv_obj_t  * year_roller;            //年滚筒
	//lv_obj_t  * month_roller;           //月滚筒
	lv_obj_t  * day_roller;             //日滚筒
	lv_obj_t  * rep_btn;                //重复类型选择
	lv_obj_t  * rep_end_btn;            //重复结束选择
	lv_obj_t  * aduio_btn;              //音频选择
    lv_obj_t  * del_btn;                //删除按钮
    lv_obj_t  * rep_label;              //重复类型标签
    lv_obj_t  * end_rep_label;          //结束重复时间标签
    lv_obj_t  * audio_label;            //结束提醒标签


    /*重复类型配置页面*/
    lv_obj_t  * rep_bg_cont;            //配置背景蒙版
    lv_obj_t  * rep_cont;               //设置重复界面
    lv_obj_t  * not_rep_circle;         //不重复圆圈
    lv_obj_t  * every_day_circle;       //每天圆圈
    lv_obj_t  * every_week_circle;      //每月圆圈
    lv_obj_t  * every_month_circle;     //每月圆圈
    lv_obj_t  * every_year_circle;      //每年圆圈

    /*重复类型每周配置页面*/
    lv_obj_t  * week_bg_cont;           //周配置背景蒙版
    /*重复类型每月配置页面*/
    lv_obj_t  * month_bg_cont;          //月配置背景
    /*结束重复时间配置页面*/
    lv_obj_t  * stoptime_bg_cont;       //结束时间配置背景
    /*语音提醒配置页面*/
    lv_obj_t  * audio_bg_cont;          //提醒语音配置背景






} T_lv_timingedit, *PT_lv_timingedit;


void lv_timing_edit(void);

#endif /* LV_TIMING_EDIT_PAGE_H_ */

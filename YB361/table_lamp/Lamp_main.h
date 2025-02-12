/*
 * lamp_main.h
 *
 *  Created on: Nov 5, 2024
 *      Author: hxd
 */

#ifndef LAMP_MAIN_H_
#define LAMP_MAIN_H_


typedef enum{
	/*1级菜单*/
	Pose_Remind_m = 0,
	Reset_Remid_m,
	Eye_exercises_m,
	Pose_Record_m,
	First_Level_end,
	/*2级菜单*/

}Lamp_page_e;


typedef struct{
	lv_obj_t *bg_img;	/*背景图片*/
	uint8_t func_id;	/*功能ID*/
	uint8_t last_id;	/*上次id*/

}Lamp_Fun_t;



void Lamp_Task(void *parameter);

#endif /* LAMP_MAIN_H_ */

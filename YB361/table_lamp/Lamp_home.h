#ifndef LAMP_HOME_H_INCLUDED
#define LAMP_HOME_H_INCLUDED


#define LAMP_DISP_HOR_RES		320
#define LAMP_DISP_VER_RES		240


typedef enum{
	FUNC_HOME = 0,
	FUNC_POWER,
	FUNC_LIGHT,
	FUNC_POSE,
	FUNC_AUDIO,
	FUNC_TIME,
}FUNC_ID;


typedef struct
{
	lv_obj_t *bg_img;	/*背景图片*/
	lv_obj_t *label_time;/*时间标签*/
	lv_obj_t *label_week;/*星期标签*/


	uint8_t func_id;	/*功能ID*/
	uint8_t hour;		/*小时*/
	uint8_t minute;		/*分钟*/
	uint8_t week;		/*星期*/


}Lamp_home_t;

void Lamp_Style_Init(void);
#endif // LAMP_HOME_H_INCLUDED

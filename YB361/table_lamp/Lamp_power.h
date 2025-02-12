#ifndef LAMP_POWER_H_INCLUDED
#define LAMP_POWER_H_INCLUDED


typedef enum{
	POWER_MODE_UPDOWN = 0,
	POWER_MODE_DOWN,
	POWER_MODE_UP,
}POWER_LIGHT_MODE_E;

typedef enum{
	POWER_PAGE_MODE = 0,
	POWER_PAGE_DOWN,
	POWER_PAGE_UP,
}POWER_ID_E;

typedef struct
{
	lv_obj_t *bg_img;	/*背景图片*/
	lv_obj_t *down_page;/*下发光亮度设置界面*/
	lv_obj_t *up_page;	/*上发光亮度设置界面*/

	uint8_t page_id;	/*页面ID*/
	uint8_t Light_mode;	/*灯光模式*/
	uint8_t Up_value;	/*上发光亮度*/
	uint8_t Down_value;	/*下发光亮度*/

}Lamp_power_t;


void Lamp_power_init(void);
#endif // LAMP_POWER_H_INCLUDED

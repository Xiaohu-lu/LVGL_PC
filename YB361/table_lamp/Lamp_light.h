#ifndef LAMP_LIGHT_H_INCLUDED
#define LAMP_LIGHT_H_INCLUDED

typedef enum
{
    LIGHT_MODE_PAGE = 0,
    LIGHT_NIGHT_PAGE,
}LIGHT_PAGE_E;

typedef enum
{
    LIGHT_WRITING_MODE = 0,
    LIGHT_INTERNET_MODE,
    LIGHT_SEATING_MODE,
    LIGHT_SMART_MODE,
    LIGHT_NIGHT_MODE,
}LIGHT_MODE_e;


typedef struct
{
	lv_obj_t *bg_img;	/*±≥æ∞Õº∆¨*/
	lv_obj_t *asr_img;	/*”Ô“Ùøÿ÷∆Õº∆¨*/
	lv_obj_t *vme_img;	/*“Ù¡øøÿ÷∆Õº∆¨*/

	uint8_t page_id;	/*µ±«∞“≥√ÊID*/
	uint8_t light_mode;
}Lamp_light_t;


void Lamp_light_init(void);
#endif // LAMP_LIGHT_H_INCLUDED

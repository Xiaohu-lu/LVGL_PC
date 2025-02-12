#ifndef LAMP_AUDIO_H_INCLUDED
#define LAMP_AUDIO_H_INCLUDED


typedef enum{
	PAGE_ASR = 0,
	PAGE_VME,
	PAGE_VOLUME,
}AUDI_PAGE_ID;

typedef struct
{
	lv_obj_t *bg_img;	/*背景图片*/
	lv_obj_t *asr_img;	/*语音控制图片*/
	lv_obj_t *vme_img;	/*音量控制图片*/
	lv_obj_t *volume_page;/*音量设置界面*/

	uint8_t page_id;	/*当前页面ID*/
	uint8_t Volume;		/*音量*/
	uint8_t AsrOn;		/*语音识别开关*/
}Lamp_audio_t;



void Lamp_audio_init(void);
void Lamp_audio_volume_page(void);
#endif // LAMP_AUDIO_H_INCLUDED

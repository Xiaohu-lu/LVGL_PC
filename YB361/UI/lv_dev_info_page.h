/*
 * lv_dev_info_page.h
 *
 *  Created on: Dec 14, 2023
 *      Author: hxd
 */

#ifndef LV_DEV_INFO_PAGE_H_
#define LV_DEV_INFO_PAGE_H_

#define LV_DEVINFO_US_TABLE_SPACE			(LV_HOR_RES/6)
#define LV_DEVINFO_US_TITLE_SPACE			(45)
#define LV_DEVINFO_US_LABEL_SPACE			(5)

#define LV_DEVINFO_US_CONT_W				(300)
#define LV_DEVINFO_US_CONT_H				(90)

#define LV_FUNC_SWITCH_CONT_W				(280)
#define LV_FUNC_SWITCH_CONT_H				(50)

#define LV_DEVINFO_VOLUME_CONT_H			(70)
#define LV_DEVINFO_FUNC_CONT_H				(260)

#define LV_DEVINFO_FAST_MODE_W             	(40)
#define LV_DEVINFO_FAST_MODE_H             	(40)
#define LV_DEVINFO_FAST_MODE_POSX         	(30)
#define LV_DEVINFO_FAST_MODE_POSY          	(20)
#define LV_DEVINFO_NORMAL_MODE_POSX        	(120)
#define LV_DEVINFO_NORMAL_MODE_POSY        	(20)
#define LV_DEVINFO_HABIT_MODE_POSX         	(210)
#define LV_DEVINFO_HABIT_MODE_POSY         	(20)

#define LV_DEVINFO_MIN_VOLUME_W            	(32)
#define LV_DEVINFO_MIN_VOLUME_H            	(32)
#define LV_DEVINFO_MIN_VOLUME_POSX         	(30)
#define LV_DEVINFO_MIN_VOLUME_POSY         	(30)

#define FAST_MODE							0
#define NORMAL_MODE							1
#define HABIT_MODE							2

typedef struct _lv_devinfo{
	lv_obj_t  * bg_about;				// ����
	lv_obj_t  * fast_mode;
	lv_obj_t  * normal_mode;
	lv_obj_t  * habit_mode;
	lv_obj_t  * volume_slider;			//��������
	lv_obj_t  * pos_switch;				//��������ͷ����
	lv_obj_t  * longsit_switch;			//�������ѿ���
	lv_obj_t  * light_switch;			//�������⿪��
	lv_obj_t  * offset_switch;			//������⿪��
	lv_obj_t  * mic_switch;				//����ʶ�𿪹�
	lv_obj_t  * upload_switch;			//ͼ���ϴ�����
	lv_obj_t  * voicewarn_switch;		//�������ѿ���
	lv_obj_t  * vibrate_switch;			//�����ѿ���
	lv_obj_t  * lightwarn_switch;		//�ƹ����ѿ���

}T_lv_devinfo, *PT_lv_devinfo;



/* some color
 * ��ɳ��		0xC7EDCC
 * ���Ӱ�		0xFFFFFF
 * ���ʻ�		0xFAF9DE
 * ��Ҷ��		0xFFF2E2
 * ��֬��		0xFDE6E0
 * ������		0xDCE2F1
 * �����		0xE9EBFE
 * �����		0xEAEAEF
 * �����   	0xE3EDCD
 * ����			0xCCE8CF
 * WPS			0x6E7B6C
 * */
#define BACK_COLOR				(0xC7EDCC)//(0xE9EBFE)
#define RED_HEX                 (0xFF0000)
#define GREY_HEX                (0xBEBEBE)
#define LIGHTGRAY_HEX           (0xD3D3D3)
#define SNOW3_HEX               (0xCDC9C9)
#define GREEN_HEX               (0x00FF00)
#define LIME_GREEN_HEX          (0x32CD32)
#define WHITE_HEX               (0xFFFFFF)
#define BLACK_HEX               (0x000000)
#define CORNFLOWERBLUE_HEX      (0x6495ED)
#define BLUE_HEX                (0x0000FF)
#define YELLOW_HEX              (0xFFFF00)
#define PURPLE_HEX              (0xA020F0)




void lv_dev_info(void);
void lv_device_info_refresh(void);
#endif /* LV_DEV_INFO_PAGE_H_ */

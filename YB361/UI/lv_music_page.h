/*
 * lv_music_page.h
 *
 *  Created on: Dec 19, 2023
 *      Author: hxd
 */

#ifndef LV_MUSIC_PAGE_H_
#define LV_MUSIC_PAGE_H_

#define LV_MUSIC_PLAYER_SPACE                   (75)
#define LV_MUSIC_PLAYER_BUFSIZE				    (1504) //1408 //(1312)	//(1248) //1024*2

#define LV_MUSIC_PLAYER_LIST_HIGHT              (LV_VER_RES - (LV_VER_RES / 3))


#define BOTTOM_CON_H                            60
#define BOTTOM_CON_W                            300
#define CTL_SIZE                                40

#define SLIDER_CON_W                            40
#define SLIDER_CON_H                            180

typedef struct _lv_100ask_music_player {
	lv_obj_t  * bg_music_player;      		/* ���� */
	lv_obj_t  * current_btn;				/* ��¼��ǰ���ŵ����� */
	lv_obj_t  * slider_label;				/* ���Ž����� */
	lv_obj_t  * label_music_name;			/* ���ŵ��ļ��� */
	lv_obj_t  * music_list;    	   			/* �����б��� */
	lv_obj_t  * btn_player_ctl; 			/* ��ͣ�����Ű�ť */
	lv_obj_t  * btn_per; 					/* ��һ�װ�ť */
	lv_obj_t  * btn_next;     				/* ��һ�װ�ť */
	lv_obj_t  * btn_shuffle; 				/* ������Ű�ť */
	lv_obj_t  * btn_list;					/* �����б�ť */
	lv_obj_t  * slider;						/* ���Ž����� */
	lv_timer_t * timer_handle;   		    /* ���������� */

	lv_fs_file_t file;						/* �ļ����� */
	uint8_t  buffer[LV_MUSIC_PLAYER_BUFSIZE];				/* ������ */
	uint32_t progress;  					/* ���Ž��� */
	bool player_state;   					/* ����״̬�� 1 - ������ ; 0 - ��ͣ���� */
} T_lv_music_player, *PT_lv_music_player;



void lv_music_player(void);


#endif /* LV_MUSIC_PAGE_H_ */

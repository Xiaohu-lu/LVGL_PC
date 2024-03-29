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
	lv_obj_t  * bg_music_player;      		/* 背景 */
	lv_obj_t  * current_btn;				/* 记录当前播放的索引 */
	lv_obj_t  * slider_label;				/* 播放进度条 */
	lv_obj_t  * label_music_name;			/* 播放的文件名 */
	lv_obj_t  * music_list;    	   			/* 播放列表句柄 */
	lv_obj_t  * btn_player_ctl; 			/* 暂停、播放按钮 */
	lv_obj_t  * btn_per; 					/* 上一首按钮 */
	lv_obj_t  * btn_next;     				/* 下一首按钮 */
	lv_obj_t  * btn_shuffle; 				/* 随机播放按钮 */
	lv_obj_t  * btn_list;					/* 播放列表按钮 */
	lv_obj_t  * slider;						/* 播放进度条 */
	lv_timer_t * timer_handle;   		    /* 播放任务句柄 */

	lv_fs_file_t file;						/* 文件对象 */
	uint8_t  buffer[LV_MUSIC_PLAYER_BUFSIZE];				/* 缓冲区 */
	uint32_t progress;  					/* 播放进度 */
	bool player_state;   					/* 播放状态： 1 - 播放中 ; 0 - 暂停播放 */
} T_lv_music_player, *PT_lv_music_player;



void lv_music_player(void);


#endif /* LV_MUSIC_PAGE_H_ */

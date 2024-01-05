/*
 * lv_snake_page.h
 *
 *  Created on: Dec 18, 2023
 *      Author: hxd
 */

#ifndef LV_SNAKE_PAGE_H_
#define LV_SNAKE_PAGE_H_

#define LV_SNAKE_TITLE_SPACE   (50)

#define LV_SNAKE_INIT_LINE     (3)
#define LV_SNAKE_SPEED         (15)
#define LV_SNAKE_SIZE          (15)
#define LV_SNAKE_HEAD_COLOR    (YELLOW_HEX)       /*蛇头颜色*/
#define LV_SNAKE_BODY_COLOR    (BLACK_HEX)        /*蛇身颜色*/
#define LV_SNAKE_RADIUS        (15)               /*蛇身圆角设置*/
#define LV_FOOD_RADIUS         (50)               /*食物圆角*/
#define LV_FOOD_COLOR          (PURPLE_HEX)       /*食物颜色*/
#define LV_FOOD_SIZE           (LV_SNAKE_SIZE)    /*大小*/

#define LV_FOOD_CHECK_SPACE    (10)               /*检测食物和蛇头*/



#define LV_FOOD_MAX_HOR        (LV_HOR_RES - LV_SNAKE_SIZE)
#define LV_FOOD_MAX_VER        (LV_VER_RES - LV_SNAKE_TITLE_SPACE)


#define LV_SNAKE_BORDER_W      (300)
#define LV_SNAKE_BORDER_H      (360)
#define LV_SNAKE_BORDER_COLOR  (BLACK_HEX)
#define LV_SNAKE_BORDER_WIDTH  (5)
#define LV_SNAKE_BORDER_Y      (100)
#define LV_SNAKE_BORDER_X      (15)
#define LV_SNAKE_RAND_RANGE_X  (LV_SNAKE_BORDER_W - LV_SNAKE_BORDER_WIDTH - LV_SNAKE_SIZE)
#define LV_SNAKE_RAND_RANGE_Y  (LV_SNAKE_BORDER_H - LV_SNAKE_BORDER_Y - LV_SNAKE_BORDER_WIDTH - LV_SNAKE_SIZE)


#define LV_CHECK_RIGHT         (LV_SNAKE_BORDER_W + 10 - 7)
#define LV_CHECK_LEFT          (10 - 2)
#define LV_CHECK_TOP           (LV_SNAKE_BORDER_Y - 1)
#define LV_CHECK_BOTTOM        (LV_SNAKE_BORDER_Y + LV_SNAKE_BORDER_H - 7)
typedef struct _lv_snake {
	lv_obj_t  * bg_about;				// 背景
    lv_obj_t  * obj_food;               //食物句柄
    lv_obj_t  * label_info;             //游戏信息
    lv_obj_t  * obj_border;             //边界
    uint16_t score;                    //得分
    uint16_t len;                      //长度
    uint8_t gesture;                   //蛇的头部方向标志
    lv_timer_t *snake_timer;           //定时器
} T_lv_snake, *PT_lv_snake;

typedef struct Link{
    lv_obj_t *obj;                  //蛇身
    lv_coord_t x;                   //x坐标
    lv_coord_t y;                   //y坐标
    struct Link *prior;            //指向直接前驱
    struct Link *next;             //指向直接后驱
}link_snake, p_link_snake;


void lv_snake(void);
#endif /* LV_SNAKE_PAGE_H_ */

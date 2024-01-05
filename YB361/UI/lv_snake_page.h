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
#define LV_SNAKE_HEAD_COLOR    (YELLOW_HEX)       /*��ͷ��ɫ*/
#define LV_SNAKE_BODY_COLOR    (BLACK_HEX)        /*������ɫ*/
#define LV_SNAKE_RADIUS        (15)               /*����Բ������*/
#define LV_FOOD_RADIUS         (50)               /*ʳ��Բ��*/
#define LV_FOOD_COLOR          (PURPLE_HEX)       /*ʳ����ɫ*/
#define LV_FOOD_SIZE           (LV_SNAKE_SIZE)    /*��С*/

#define LV_FOOD_CHECK_SPACE    (10)               /*���ʳ�����ͷ*/



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
	lv_obj_t  * bg_about;				// ����
    lv_obj_t  * obj_food;               //ʳ����
    lv_obj_t  * label_info;             //��Ϸ��Ϣ
    lv_obj_t  * obj_border;             //�߽�
    uint16_t score;                    //�÷�
    uint16_t len;                      //����
    uint8_t gesture;                   //�ߵ�ͷ�������־
    lv_timer_t *snake_timer;           //��ʱ��
} T_lv_snake, *PT_lv_snake;

typedef struct Link{
    lv_obj_t *obj;                  //����
    lv_coord_t x;                   //x����
    lv_coord_t y;                   //y����
    struct Link *prior;            //ָ��ֱ��ǰ��
    struct Link *next;             //ָ��ֱ�Ӻ���
}link_snake, p_link_snake;


void lv_snake(void);
#endif /* LV_SNAKE_PAGE_H_ */

/*
 * lv_snake_page.c
 *
 *  Created on: Dec 18, 2023
 *      Author: hxd
 */

#include "lvgl/lvgl.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "lv_about_page.h"
#include "lv_find_dev.h"
#include "lv_dev_info_page.h"
#include "lv_record_page.h"
#include "lv_record_info_page.h"
#include "lv_snake_page.h"

static PT_lv_snake g_pt_lv_snake;  // ���ݽṹ��

static link_snake *spriteSnake;
static uint8_t last_gesture;

static link_snake *lv_snake_initLink(void);
static void lv_snake_linkAddNode(void);
static void lv_snake_clear_list(link_snake *ppHeadNode);
static void lv_snake_init(void);
static void event_handler_back_to_home(lv_event_t *e);
static void event_handler_snake_gesture_cb(lv_event_t *e);
static void lv_snake_update_snake_data(void);
static void lv_timer_game_snake(lv_timer_t * timer);
/* lv_snake
 * snake game
 */
void lv_snake(void)
{
    g_pt_lv_snake = (T_lv_snake*)pvPortMalloc(sizeof(T_lv_snake));
	g_pt_lv_snake->bg_about = lv_obj_create(lv_scr_act());/*��������*/

	lv_obj_set_size(g_pt_lv_snake->bg_about, LV_HOR_RES, LV_VER_RES);/*���ô�С*/
	lv_obj_set_style_radius(g_pt_lv_snake->bg_about, 0, 0);/*����Բ��*/
	lv_obj_align(g_pt_lv_snake->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*����padding*/
	set_user_style(g_pt_lv_snake->bg_about);

	lv_obj_set_size(g_pt_lv_snake->bg_about, LV_HOR_RES, LV_VER_RES);/*���ô�С*/
	lv_obj_set_style_bg_opa(g_pt_lv_snake->bg_about, LV_OPA_100, 0);/*���ñ���͸����*/
	lv_obj_set_style_bg_color(g_pt_lv_snake->bg_about, lv_color_hex(BACK_COLOR), 0);
	lv_snake_init();/*�����ʼ��*/

    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);/*���ϲ�������*/
	lv_obj_add_event_cb(lv_layer_top(), event_handler_snake_gesture_cb, LV_EVENT_GESTURE, NULL);/*��ӻص�����*/

	add_title(g_pt_lv_snake->bg_about, "Snake game");
	add_back(lv_layer_top(), event_handler_back_to_home);

}

/* lv_snake_init
 * snake �����ʼ������
 */
static void lv_snake_init(void)
{
    spriteSnake = lv_snake_initLink();
    link_snake *head = NULL;
    head = spriteSnake;

    g_pt_lv_snake->obj_border = lv_obj_create(g_pt_lv_snake->bg_about);/*�����߽�*/
    lv_obj_set_size(g_pt_lv_snake->obj_border, LV_SNAKE_BORDER_W, LV_SNAKE_BORDER_H);/*���ô�С*/
    lv_obj_set_style_border_color(g_pt_lv_snake->obj_border, lv_color_hex(LV_SNAKE_BORDER_COLOR), 0);
    lv_obj_set_style_border_width(g_pt_lv_snake->obj_border, LV_SNAKE_BORDER_WIDTH, 0);
    lv_obj_set_style_bg_color(g_pt_lv_snake->obj_border, lv_color_hex(BACK_COLOR), 0);
    lv_obj_align(g_pt_lv_snake->obj_border, LV_ALIGN_TOP_MID, 0, LV_SNAKE_BORDER_Y);

    int i = 0;

    lv_coord_t init_x = (rand()%(LV_SNAKE_RAND_RANGE_X)) + LV_SNAKE_BORDER_X + LV_SNAKE_SIZE * 3;
    lv_coord_t init_y = (rand()%(LV_SNAKE_RAND_RANGE_Y)) + LV_SNAKE_BORDER_Y;

    while(head->next)
    {
        head = head->next;/*����ͷ�ڵ�*/
        head->obj = lv_obj_create(g_pt_lv_snake->bg_about);
        if(i == 0)
        {
            lv_obj_set_style_bg_color(head->obj, lv_color_hex(LV_SNAKE_HEAD_COLOR), 0);
        }
        else
        {
            lv_obj_set_style_bg_color(head->obj, lv_color_hex(LV_SNAKE_BODY_COLOR), 0);
        }
        lv_obj_set_style_radius(head->obj, LV_SNAKE_RADIUS, 0);/*����Բ��*/
        lv_obj_set_size(head->obj, LV_SNAKE_SIZE, LV_SNAKE_SIZE);/*���ô�С*/
        lv_obj_set_pos(head->obj, init_x - (i * LV_SNAKE_SIZE), init_y);
        //head->x = lv_obj_get_x(head->obj);
        //head->y = lv_obj_get_y(head->obj);
        head->x = init_x - (i * LV_SNAKE_SIZE);
        head->y = init_y;
        i++;
    }

    /*��ʼ��ʳ��*/
    g_pt_lv_snake->obj_food = lv_obj_create(g_pt_lv_snake->bg_about);
    lv_obj_set_size(g_pt_lv_snake->obj_food, LV_FOOD_SIZE, LV_FOOD_SIZE);/*���ô�С*/
    lv_obj_set_style_bg_color(g_pt_lv_snake->obj_food, lv_color_hex(LV_FOOD_COLOR), 0);/*���ñ�����ɫ*/
    lv_obj_set_style_radius(g_pt_lv_snake->obj_food, LV_FOOD_RADIUS, 0);
    lv_obj_set_pos(g_pt_lv_snake->obj_food, (rand()%(LV_SNAKE_RAND_RANGE_X) + LV_SNAKE_BORDER_X), (rand()%(LV_SNAKE_RAND_RANGE_Y)) + LV_SNAKE_BORDER_Y);/*����λ��*/

    g_pt_lv_snake->score = 3;
    g_pt_lv_snake->len = 3;
    g_pt_lv_snake->label_info = lv_label_create(g_pt_lv_snake->bg_about);/*����һ����ǩ*/
    lv_label_set_text_fmt(g_pt_lv_snake->label_info, "LEN: %d\nSCORE: %d", g_pt_lv_snake->len, g_pt_lv_snake->score);
    lv_obj_align(g_pt_lv_snake->label_info, LV_ALIGN_TOP_LEFT, 0, LV_SNAKE_TITLE_SPACE);


    /*����һ����ʱ��*/
    g_pt_lv_snake->snake_timer = lv_timer_create(lv_timer_game_snake, 300, NULL);
    last_gesture = 4;/*��ʼ����*/

}

/* lv_snake_initLink
 * ��ʼ�����������
 * return���������ͷ
 */
static link_snake *lv_snake_initLink(void)
{

    link_snake *head = (link_snake*)pvPortMalloc(sizeof(link_snake));/*���������һ���ڵ�*/
    head->prior = NULL;
    head->next = NULL;
    head->obj = NULL;
    head->x = 0;
    head->y = 0;
    link_snake *list = head;

    /*��������(��ʼ������)*/
    for(int i = 0; i < LV_SNAKE_INIT_LINE; i++)
    {
        link_snake *body = (link_snake*)pvPortMalloc(sizeof(link_snake));
        body->prior = NULL;
        body->next = NULL;
        body->obj = NULL;
        body->x = i;
        body->y = i;

        list->next = body;
        body->prior = list;
        list = list->next;
    }
    return head;
}

/* lv_snake_linkAddNode
 * ���һ���ڵ㵽����
 */
static void lv_snake_linkAddNode(void)
{
    link_snake *temp_list;
    /*��ʼ���½ڵ�*/
    link_snake *list_new = (link_snake*)pvPortMalloc(sizeof(link_snake));
    list_new->prior = NULL;
    list_new->next = NULL;
    list_new->x = 0;
    list_new->y = 0;
    list_new->obj = lv_obj_create(g_pt_lv_snake->bg_about);
    lv_obj_set_style_bg_color(list_new->obj, lv_color_hex(LV_SNAKE_BODY_COLOR), 0);/*���ñ�����ɫ*/
    lv_obj_set_style_radius(list_new->obj, LV_SNAKE_RADIUS, 0);/*����Բ��*/
    lv_obj_set_size(list_new->obj, LV_SNAKE_SIZE, LV_SNAKE_SIZE);/*���ô�С*/
    /*���ͷ�ڵ�*/
    if(spriteSnake == NULL)
    {
        spriteSnake = list_new;
    }
    else
    {
        temp_list = spriteSnake;
        while(temp_list->next)/*�ҵ�β��*/
        {
            temp_list = temp_list->next;
        }
        temp_list->next = list_new;
        list_new->prior = temp_list;
    }
}

/* lv_snake_clear_list
 * �����������
 */
static void lv_snake_clear_list(link_snake *ppHeadNode)
{
    link_snake *pListNodeTmp = NULL;
    if(ppHeadNode == NULL)
    {
        LOG_E("The list is empty, no need to clear.");
        return;
    }
    while(ppHeadNode->next != NULL)
    {
        pListNodeTmp = ppHeadNode->next;
        if(ppHeadNode->obj != NULL)
        {
            lv_obj_clean(ppHeadNode->obj);
        }
        vPortFree(ppHeadNode);
        ppHeadNode = pListNodeTmp;

    }
    /*ɾ�����һ���ڵ�*/
    if(ppHeadNode != NULL)
    {
        if(ppHeadNode->obj != NULL)
        {
            lv_obj_clean(ppHeadNode->obj);
        }
        vPortFree(ppHeadNode);
        ppHeadNode = NULL;
    }
    LOG_D("The list is cleared .");
}

/* lv_timer_game_snake
 * ��ʱ���ص�����
 */
static void lv_timer_game_snake(lv_timer_t * timer)
{
   lv_coord_t x = spriteSnake->next->x - lv_obj_get_x(g_pt_lv_snake->obj_food);/*��ȡx�����*/
   lv_coord_t y = spriteSnake->next->y - lv_obj_get_y(g_pt_lv_snake->obj_food);/*��ȡy��Ĳ���*/
   /*�Ե���ʳ��*/
   if(((x >= -LV_FOOD_CHECK_SPACE) && (x <= LV_FOOD_CHECK_SPACE)) && ((y >= -LV_FOOD_CHECK_SPACE) && (y <= LV_FOOD_CHECK_SPACE)))
   {
       lv_snake_linkAddNode();/*����µ�ʳ��*/
       g_pt_lv_snake->len += 1;
       g_pt_lv_snake->score += 1;

       /*����ʳ���λ��*/
       lv_obj_set_pos(g_pt_lv_snake->obj_food, (rand()%(LV_SNAKE_RAND_RANGE_X) + LV_SNAKE_BORDER_X), (rand()%(LV_SNAKE_RAND_RANGE_Y)) + LV_SNAKE_BORDER_Y);/*����λ��*/
       /*���µ÷�*/
       lv_label_set_text_fmt(g_pt_lv_snake->label_info, "LEN: %d\nSCORE: %d", g_pt_lv_snake->len, g_pt_lv_snake->score);
   }
   lv_snake_update_snake_data();/*�����ߵ�����*/
}


/* lv_snake_update_snake_data
 * �����ߵ�״̬
 */
static void lv_snake_update_snake_data(void)
{
    static lv_coord_t x = LV_SNAKE_SPEED, y = 0;    /*Ĭ��������*/


    lv_coord_t last_x, last_y;
    uint8_t mode = 0;
    link_snake *temp_list = NULL;
    temp_list = spriteSnake;
    temp_list = temp_list->next;
    last_x = lv_obj_get_x(temp_list->obj);
    last_y = lv_obj_get_y(temp_list->obj);


    /*�ұ߲�������*/
    if((last_x + LV_SNAKE_SIZE ) > LV_CHECK_RIGHT)
    {
        mode |= 4;/*��ʾ���Ҳ����ƶ�*/
    }
    if((last_x - LV_SNAKE_SIZE) < LV_CHECK_LEFT)
    {
        mode |= 8;/*��ʾ���������ƶ�*/
    }
    if((last_y - LV_SNAKE_SIZE) < LV_CHECK_TOP)
    {
        mode |= 1;/*��ʾ���������ƶ�*/
    }
    if((last_y + LV_SNAKE_SIZE) > LV_CHECK_BOTTOM)
    {
        mode |= 2;/*��ʾ���������ƶ�*/
    }

    /*�ж��Ƿ�����*/
    switch(last_gesture)
    {
    case 1:/*����*/
        if((mode & 1) == 1)/*��ʾ���������ƶ�*/
        {
            if(g_pt_lv_snake->gesture == 0)/*����û����*/
            {
                g_pt_lv_snake->gesture = 4;/*�����ƶ�*/
                if((mode & 4) == 4)/*��ʾ���������ƶ�*/
                {
                    g_pt_lv_snake->gesture = 1;/*�����ƶ�*/
                }
            }
        }

        break;
    case 2:/*����*/
        if((mode & 2) == 2)/*��ʾ���������ƶ�*/
        {
            if(g_pt_lv_snake->gesture == 0)/*����û����*/
            {
                g_pt_lv_snake->gesture = 8;/*�����ƶ�*/
                if((mode & 8) == 8)/*��ʾ���������ƶ�*/
                {
                    g_pt_lv_snake->gesture = 1;/*�����ƶ�*/
                }
            }
        }


        break;
    case 4:/*����*/
        if((mode & 4) == 4)/*��ʾ���������ƶ�*/
        {
            if(g_pt_lv_snake->gesture == 0)/*����û����*/
            {
                g_pt_lv_snake->gesture = 2;/*�����ƶ�*/
                if((mode & 2) == 2)/*��ʾ���������ƶ�*/
                {
                    g_pt_lv_snake->gesture = 1;/*�����ƶ�*/
                }
            }

        }

        break;
    case 8:/*����*/
        if((mode & 8) == 8)/*��ʾ���������ƶ�*/
        {
            if(g_pt_lv_snake->gesture == 0)/*����û����*/
            {
                g_pt_lv_snake->gesture = 1;/*�����ƶ�*/
                if((mode & 1) == 1)/*��ʾ���������ƶ�*/
                {
                    g_pt_lv_snake->gesture = 2;/*�����ƶ�*/
                }
            }
        }
        break;
    }


    temp_list = spriteSnake;
    if(((g_pt_lv_snake->gesture) & 1) == 1)/*����*/
    {
        y = -LV_SNAKE_SPEED;
        x = 0;
        last_gesture = 1;

    }
    else if(((g_pt_lv_snake->gesture) & 2) == 2)/*����*/
    {
        y = LV_SNAKE_SPEED;
        x = 0;
        last_gesture = 2;

    }
    else if(((g_pt_lv_snake->gesture) & 4) == 4)/*����*/
    {
        y = 0;
        x = LV_SNAKE_SPEED;
        last_gesture = 4;

    }
    else if(((g_pt_lv_snake->gesture) & 8) == 8)/*����*/
    {
        y = 0;
        x = -LV_SNAKE_SPEED;
        last_gesture = 8;

    }
    g_pt_lv_snake->gesture = 0;/*��ձ�־λ*/
temp_list = spriteSnake;
#if 0
    while(temp_list->next)/*�ҵ�β�ڵ�*/
    {
        temp_list = temp_list->next;
    }
    while(temp_list->prior->prior)/*��ǰ����*/
    {
        /*�Ӻ���ǰ��������ϸ��������*/
        temp_list->x = temp_list->prior->x;
        temp_list->y = temp_list->prior->y;
        lv_obj_set_pos(temp_list->obj, temp_list->x, temp_list->y);
        temp_list = temp_list->prior;
    }
    /*�����׽ڵ�*/
    temp_list->x = lv_obj_get_x(temp_list->obj) + x;
    temp_list->y = lv_obj_get_y(temp_list->obj) + y;
    lv_obj_set_pos(temp_list->obj, temp_list->x, temp_list->y);
    #else
    //lv_coord_t last_x, last_y;
    lv_coord_t temp_x, temp_y;
    /*���ƶ���ͷ*/
    temp_list = temp_list->next;/*��һ���ڵ�*/
    last_x = temp_list->x;
    last_y = temp_list->y;
    temp_list->x = last_x + x;
    temp_list->y = last_y + y;
    lv_obj_set_pos(temp_list->obj, temp_list->x, temp_list->y);
    while(temp_list->next)
    {
        temp_list = temp_list->next;
        temp_x = temp_list->x;
        temp_y = temp_list->y;
        temp_list->x = last_x;
        temp_list->y = last_y;
        last_x = temp_x;
        last_y = temp_y;
        lv_obj_set_pos(temp_list->obj, temp_list->x, temp_list->y);
    }

    #endif

}


/* event_handler_back_to_home
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_back_to_home(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{

	    lv_snake_clear_list(spriteSnake);
	    if(g_pt_lv_snake->snake_timer != NULL) lv_timer_del(g_pt_lv_snake->snake_timer);
		if(g_pt_lv_snake->bg_about != NULL) lv_obj_del(g_pt_lv_snake->bg_about);



		vPortFree(g_pt_lv_snake);
		//gfind_dev_state = 0;

		lv_obj_remove_event_cb(lv_layer_top(), event_handler_snake_gesture_cb);
		lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
		lv_obj_clean(lv_layer_top());

		lv_main_page_demo_home(100);
	}
}

/* event_handler_snake_gesture_cb
 * ���㴥���¼�������
 */
static void event_handler_snake_gesture_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_GESTURE)/*��⵽һ������*/
	{
		switch(lv_indev_get_gesture_dir(lv_indev_get_act()))/*��ǰ����������豸*/
		{
        case LV_DIR_TOP:
            if(last_gesture != 2)
            {
                g_pt_lv_snake->gesture |= 1;
            }
            break;
        case LV_DIR_BOTTOM:
            if(last_gesture != 1)
            {
                g_pt_lv_snake->gesture |= 2;
            }
            break;
        case LV_DIR_RIGHT:
            if(last_gesture != 8)
            {
                g_pt_lv_snake->gesture |= 4;
            }
            break;
        case LV_DIR_LEFT:
            if(last_gesture != 4)
            {
                 g_pt_lv_snake->gesture |= 8;
            }
            break;
		}
		//lv_snake_update_snake_data();

	}
}

/* lvgl
 * */
#include "lvgl/lvgl.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "lv_block_game.h"
#include "lv_dev_info_page.h"

static PT_lv_blockgame g_pt_lv_blockgame;  // ���ݽṹ��

static lv_style_t style_baseblock;/*������ʽ*/

static Block_t last_block;      /*��һ������Ϣ*/
static Block_t current_block;   /*��ǰ����Ϣ*/

static void lv_block_game_init(void);
static void lv_test_block_timer(lv_timer_t * timer);


static void lv_block_game_create_border(void);
static void lv_block_game_init_all_base_block(void);
static void lv_block_game_reset_current_block(void);



static void lv_block_game_new_block(Block_t *block, uint8_t block_type, uint32_t color);
static void lv_block_game_fill_block(Block_t *block);
static void lv_block_game_update_current_block(void);
static void lv_block_game_switch_block_state(Block_t *block);

static void lv_block_game_move_style_1_block_left(Block_t *block);
static void lv_block_game_move_style_1_block_right(Block_t *block);
static int lv_block_game_move_style_1_block_down(Block_t *block);

static void lv_block_game_move_style_2_block_left(Block_t *block);
static void lv_block_game_move_style_2_block_right(Block_t *block);
static int lv_block_game_move_style_2_block_down(Block_t *block);

static void lv_block_game_move_block_left(Block_t *block);
static void lv_block_game_move_block_right(Block_t *block);

static int lv_block_game_check_row_and_clear(uint8_t row);
static void lv_block_game_check_row_full(Block_t *block);
static void lv_block_game_move_block_down(Block_t *block);




static void lv_block_game_align(uint8_t index, uint8_t index_to, int *x, int *y, uint8_t align_type);
static void lv_block_game_set_style_1_state(Block_t *block);
static void lv_block_game_create_style_1_block(Block_t *block, uint32_t color);
static void lv_block_game_switch_style_1_state(Block_t *block);\

static void lv_block_game_set_style_2_state(Block_t *block);
static void lv_block_game_create_style_2_block(Block_t *block, uint32_t color);
static void lv_block_game_switch_style_2_state(Block_t *block);

static void lv_block_game_set_style_3_state(Block_t *block);
static void lv_block_game_create_style_3_block(Block_t *block, uint32_t color);
static void lv_block_game_switch_style_3_state(Block_t *block);

static void lv_block_game_set_style_4_state(Block_t *block);
static void lv_block_game_create_style_4_block(Block_t *block, uint32_t color);
static void lv_block_game_switch_style_4_state(Block_t *block);

static void lv_block_game_set_style_5_state(Block_t *block);
static void lv_block_game_create_style_5_block(Block_t *block, uint32_t color);
static void lv_block_game_switch_style_5_state(Block_t *block);

static void lv_block_game_set_style_6_state(Block_t *block);
static void lv_block_game_create_style_6_block(Block_t *block, uint32_t color);
static void lv_block_game_switch_style_6_state(Block_t *block);

static void event_handler_back_to_home(lv_event_t *e);
static void event_handler_left_move(lv_event_t *e);
static void event_handler_right_move(lv_event_t *e);
static void event_handler_switch_state(lv_event_t *e);


/* lv_block_game
 * ����˹������Ϸ
 */
void lv_block_game(void)
{
    g_pt_lv_blockgame = (T_lv_blockgame*)pvPortMalloc(sizeof(T_lv_blockgame));
	g_pt_lv_blockgame->bg_about = lv_obj_create(lv_scr_act());/*��������*/

	lv_obj_set_size(g_pt_lv_blockgame->bg_about, LV_HOR_RES, LV_VER_RES);/*���ô�С*/
	lv_obj_set_style_radius(g_pt_lv_blockgame->bg_about, 0, 0);/*����Բ��*/
	lv_obj_align(g_pt_lv_blockgame->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*����padding*/
	set_user_style(g_pt_lv_blockgame->bg_about);

	lv_obj_set_size(g_pt_lv_blockgame->bg_about, LV_HOR_RES, LV_VER_RES);/*���ô�С*/
	lv_obj_set_style_bg_opa(g_pt_lv_blockgame->bg_about, LV_OPA_100, 0);/*���ñ���͸����*/
	lv_obj_set_style_bg_color(g_pt_lv_blockgame->bg_about, lv_color_hex(WHITE_HEX), 0);
	lv_block_game_init();/*�����ʼ��*/

	add_title(g_pt_lv_blockgame->bg_about, "Block Game");
	add_back(g_pt_lv_blockgame->bg_about, event_handler_back_to_home);

}

/* lv_block_game_init
 * �����ʼ��
 */
static void lv_block_game_init(void)
{
    /*����������ʽ*/
    static lv_style_t style_cont;/*������ʽ*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_100);/*���ñ���͸����*/
	lv_style_set_radius(&style_cont, 10);	/*����Բ��*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, LV_BLOCK_GAME_VALID_CONT_BORDER_WIDTH);


	lv_style_init(&style_baseblock);
	lv_style_set_bg_opa(&style_baseblock, LV_OPA_100);/*���ñ���͸����*/
	lv_style_set_radius(&style_baseblock, 10);	/*����Բ��*/
	//lv_style_set_bg_color(&style_baseblock, lv_color_hex(RED_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_border_color(&style_baseblock, lv_color_hex(LV_BLOCK_GAME_BASE_BLOCK_BORDER_COLOR));
	lv_style_set_pad_top(&style_baseblock, 0);
	lv_style_set_pad_bottom(&style_baseblock, 0);
	lv_style_set_pad_left(&style_baseblock, 0);
	lv_style_set_pad_right(&style_baseblock, 0);
    lv_style_set_border_width(&style_baseblock, 1);
    lv_style_set_radius(&style_baseblock, 0);

    /*������Ϸ��Ч����*/
    g_pt_lv_blockgame->game_value_cont = lv_obj_create(g_pt_lv_blockgame->bg_about);
    lv_obj_t *cont = g_pt_lv_blockgame->game_value_cont;
    lv_obj_add_style(cont, &style_cont, 0);
    lv_obj_set_size(cont, LV_BLOCK_GAME_VALID_CONT_W + LV_BLOCK_GAME_VALID_CONT_BORDER_WIDTH * 2, LV_BLOCK_GAME_VALID_CONT_H + LV_BLOCK_GAME_VALID_CONT_BORDER_WIDTH * 2);
    lv_obj_align(cont, LV_ALIGN_TOP_LEFT, 4, 40);
    lv_obj_set_style_radius(cont, 0, 0);

    static lv_style_t style_btn;/*��ť��ʽ*/
	lv_style_init(&style_btn);
	lv_style_set_bg_opa(&style_btn, LV_OPA_100);/*���ñ���͸����*/
	lv_style_set_radius(&style_btn, 10);	/*����Բ��*/
	lv_style_set_bg_color(&style_btn, lv_color_hex(BLUE_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&style_btn, 0);
	lv_style_set_pad_bottom(&style_btn, 0);
	lv_style_set_pad_left(&style_btn, 0);
	lv_style_set_pad_right(&style_btn, 0);
    lv_style_set_border_width(&style_btn, 0);

    lv_obj_t *btn;
    lv_obj_t *label;
    /*������ť*/
    g_pt_lv_blockgame->left_btn = lv_btn_create(g_pt_lv_blockgame->bg_about);
    btn = g_pt_lv_blockgame->left_btn;
    lv_obj_add_style(btn, &style_btn, 0);
    lv_obj_set_size(btn, LV_BLOCK_GAME_BTN_W, LV_BLOCK_GAME_BTN_H);
    lv_obj_set_style_bg_color(btn, lv_color_hex(RED_HEX), LV_STATE_PRESSED);
    lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, -5, 40);
    label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
    lv_label_set_text(label, LV_SYMBOL_LEFT);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, event_handler_left_move, LV_EVENT_CLICKED, NULL);


    /*�����Ұ�ť*/
    g_pt_lv_blockgame->right_btn = lv_btn_create(g_pt_lv_blockgame->bg_about);
    btn = g_pt_lv_blockgame->right_btn;
    lv_obj_add_style(btn, &style_btn, 0);
    lv_obj_set_size(btn, LV_BLOCK_GAME_BTN_W, LV_BLOCK_GAME_BTN_H);
    lv_obj_set_style_bg_color(btn, lv_color_hex(RED_HEX), LV_STATE_PRESSED);
    lv_obj_align_to(btn, g_pt_lv_blockgame->left_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 40);
    label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
    lv_label_set_text(label, LV_SYMBOL_RIGHT);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, event_handler_right_move, LV_EVENT_CLICKED, NULL);

    /*�����л���ť*/
    g_pt_lv_blockgame->switch_btn = lv_btn_create(g_pt_lv_blockgame->bg_about);
    btn = g_pt_lv_blockgame->switch_btn;
    lv_obj_add_style(btn, &style_btn, 0);
    lv_obj_set_size(btn, LV_BLOCK_GAME_BTN_W, LV_BLOCK_GAME_BTN_H);
    lv_obj_set_style_bg_color(btn, lv_color_hex(RED_HEX), LV_STATE_PRESSED);
    lv_obj_align_to(btn, g_pt_lv_blockgame->right_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 40);
    label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
    lv_label_set_text(label, LV_SYMBOL_REFRESH);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, event_handler_switch_state, LV_EVENT_CLICKED, NULL);



    //lv_block_game_create_border();
    lv_block_game_init_all_base_block();

    /*��λ��ǰ���״̬*/
    lv_block_game_reset_current_block();

    /*����1������*/
    lv_block_game_new_block(&current_block, STYLE_1, RED_HEX);

    lv_block_game_switch_block_state(&current_block);

    /*����һ����ʱ��*/
    g_pt_lv_blockgame->update_timer = lv_timer_create(lv_test_block_timer, 400, NULL);

}

static void lv_test_block_timer(lv_timer_t * timer)
{
    lv_block_game_move_block_down(&current_block);
}

/* lv_block_game_create_border
 * ��������Ľ�����
 */
static void lv_block_game_create_border(void)
{
    int i;
    static lv_point_t line_points[] = {{0,0}, {250,0}};
    static lv_point_t line_points_h[] = {{0,0}, {0,410}};
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 1);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_line_rounded(&style_line, true);

    lv_obj_t *line1;
    #if 1
    for(i = 1; i < LV_BLOCK_GAME_VALID_CONT_H/ 20; i++)
    {
        line1 = lv_line_create(g_pt_lv_blockgame->game_value_cont);
        lv_line_set_points(line1, line_points, 2);
        lv_obj_add_style(line1, &style_line, 0);
        lv_obj_align(line1, LV_ALIGN_TOP_MID, 0, i * 20);

    }
    #endif

    for(i = 1; i < LV_BLOCK_GAME_VALID_CONT_W/ 20; i++)
    {
        line1 = lv_line_create(g_pt_lv_blockgame->game_value_cont);
        lv_line_set_points(line1, line_points_h, 2);
        lv_obj_add_style(line1, &style_line, 0);
        lv_obj_align(line1, LV_ALIGN_LEFT_MID, i * 20, 0);

    }


}

/* lv_block_game_init_all_base_block
 * �������������п�
 */
static void lv_block_game_init_all_base_block(void)
{
    int i, j;
    lv_obj_t *obj;
    for(i = 0; i < LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE; i++)
    {

        for(j = 0; j < LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE; j++)
        {
            g_pt_lv_blockgame->block[i][j].color = LV_BLOCK_GAME_BASE_BLOCK_DEFAULT_COLOR;
            g_pt_lv_blockgame->block[i][j].is_fill = NOT_FILL;
            g_pt_lv_blockgame->block[i][j].obj = lv_obj_create(g_pt_lv_blockgame->game_value_cont);
            obj = g_pt_lv_blockgame->block[i][j].obj;
            lv_obj_add_style(obj, &style_baseblock, 0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(g_pt_lv_blockgame->block[i][j].color), 0);
            lv_obj_set_size(obj, LV_BLOCK_GAME_BASE_CONT_SIZE , LV_BLOCK_GAME_BASE_CONT_SIZE );
            lv_obj_align(obj, LV_ALIGN_TOP_LEFT, i * 20, j * 20);
        }
    }
}

/* lv_block_game_reset_all_base_block
 * ���³�ʼ�������еı�����
 */
static void lv_block_game_reset_all_base_block(void)
{
    int i, j;
    lv_obj_t *obj;
    for(i = 0; i < LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE; i++)
    {

        for(j = 0; j < LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE; j++)
        {
            g_pt_lv_blockgame->block[i][j].color = LV_BLOCK_GAME_BASE_BLOCK_DEFAULT_COLOR;
            g_pt_lv_blockgame->block[i][j].is_fill = NOT_FILL;
            obj = g_pt_lv_blockgame->block[i][j].obj;
            lv_obj_add_style(obj, &style_baseblock, 0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(g_pt_lv_blockgame->block[i][j].color), 0);
        }
    }
}

/* lv_block_game_reset_current_block
 * ��λ��ʼ���״̬
 */
static void lv_block_game_reset_current_block(void)
{
    current_block.type = STYLE_LAST;
    last_block.type = STYLE_LAST;

}

/* lv_block_game_new_block
 * ����һ���µĿ�
 */
static void lv_block_game_new_block(Block_t *block, uint8_t block_type, uint32_t color)
{
    switch(block_type)
    {
    case STYLE_1:
        lv_block_game_create_style_1_block(block, color);
        break;
    case STYLE_2:
        lv_block_game_create_style_2_block(block, color);
        break;
    }
}

static void lv_block_game_new_the_block(void)
{
    /*�����ɫ*/
    /*�������������*/
    srand((unsigned)time(NULL));
    int color_type = rand() % BLOCK_RANGE;
    int style_type = rand() % 2;
    uint32_t color = RED_COLOR_HEX;
    switch(color_type)
    {
    case BLOCK_RED:
        color = RED_COLOR_HEX;
        break;
    case BLOCK_ORANGE:
        color = ORANGE_COLOR_HEX;
        break;
    case BLOCK_YELLOW:
        color = YELLOW_COLOR_HEX;
        break;
    case BLOCK_GREEN:
        color = GREEN_COLOR_HEX;
        break;
    case BLOCK_CANY:
        color = CANY_COLOR_HEX;
        break;
    case BLOCK_PURPLE:
        color = BLUE_COLOR_HEX;
        break;
    case BLOCK_RANGE:
        color = PURPLE_COLOR_HEX;
        break;
    }

    lv_block_game_new_block(&current_block, style_type, color);
}

/* lv_block_game_restart
 * ���¿�ʼ��Ϸ
 */
static void lv_block_game_restart(void)
{
    /*��ͣ��ʱ��*/
    lv_timer_pause(g_pt_lv_blockgame->update_timer);
    /*��λ��������Ϣ*/
    lv_block_game_reset_all_base_block();
    lv_block_game_reset_current_block();
    /*�����µĿ�*/
    lv_block_game_new_the_block();
    /*����������ʱ��*/
    lv_timer_resume(g_pt_lv_blockgame->update_timer);
}

/* lv_block_game_fill_block
 * ���һ�������ɫ
 */
static void lv_block_game_fill_block(Block_t *block)
{
    int i;
    if(last_block.type == STYLE_LAST)/*����Ч*/
    {
        return;
    }
    /*���¿����ɫ*/
    for(i = 0; i < 4; i++)
    {
        if(block->x[i] >= 0 && block->y[i] >= 0)
        {
            lv_obj_set_style_bg_color(g_pt_lv_blockgame->block[block->x[i]][block->y[i]].obj, lv_color_hex(block->color), 0);
        }
    }
}

/* lv_block_game_update_current_block
 * ��䵱ǰ�����ɫ
 */
static void lv_block_game_update_current_block(void)
{
    lv_block_game_fill_block(&last_block);
    lv_block_game_fill_block(&current_block);
    memcpy((uint8_t*)&last_block, (uint8_t*)&current_block, sizeof(last_block));
    last_block.color = LV_BLOCK_GAME_BASE_BLOCK_DEFAULT_COLOR;
}

/* lv_block_game_switch_block_state
 * �л�һ�����״̬
 */
static void lv_block_game_switch_block_state(Block_t *block)
{
    switch(block->type)
    {
    case STYLE_1:
        lv_block_game_switch_style_1_state(block);
        break;
    case STYLE_2:
        lv_block_game_switch_style_2_state(block);
        break;
    }

    lv_block_game_update_current_block();

}

/* lv_block_game_move_style_1_block_left
 * ��һ�����͵Ŀ������ƶ�
 */
static void lv_block_game_move_style_1_block_left(Block_t *block)
{
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t i;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����ұ�*/
        /*3����1����±�*/
        /*4���ڿ�����*/
        /*��Ҫ�ж�4������Ƿ�����ƶ���3������Ƿ�����ƶ�*/
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2����1����±�*/
        /*3����1������*/
        /*4���ڿ���ϱ�*/
        /*��Ҫ�ж�3������Ƿ�����ƶ���2������Ƿ�����ƶ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2����1������*/
        /*3����1����ϱ�*/
        /*4���ڿ���ұ�*/
        /*��Ҫ�ж�2������Ƿ�����ƶ�*/

        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }

        break;
    case STATE_4:
        /*2����1����ϱ�*/
        /*3����1����ұ�*/
        /*4���ڿ���±�*/
        /*��Ҫ�ж�4������Ƿ�����ƶ�*/
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    }

    /*���������ƶ�*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] -= 1;
    }

}

/* lv_block_game_move_style_1_block_right
 * ��һ�����͵Ŀ������ƶ�
 */
static void lv_block_game_move_style_1_block_right(Block_t *block)
{
    uint8_t max_x;
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t i;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����ұ�*/
        /*3����1����±�*/
        /*4���ڿ�����*/
        /*��Ҫ�ж�2���ұ��Ƿ�����ƶ���3���ұ��Ƿ�����ƶ�*/
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2����1����±�*/
        /*3����1������*/
        /*4���ڿ���ϱ�*/
        /*��Ҫ�ж�2���ұ��Ƿ�����ƶ�*/
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2����1������*/
        /*3����1����ϱ�*/
        /*4���ڿ���ұ�*/
        /*��Ҫ�ж�4���ұ��Ƿ�����ƶ���3���ұ��Ƿ�����ƶ�*/

        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }

        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }

        break;
    case STATE_4:
        /*2����1����ϱ�*/
        /*3����1����ұ�*/
        /*4���ڿ���±�*/
        /*��Ҫ�ж�3���ұ��Ƿ�����ƶ���4���ұ��Ƿ�����ƶ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    }

    /*���������ƶ�*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] += 1;
    }

}

/* lv_block_game_move_style_1_block_down
 * ��һ�����͵Ŀ������ƶ�
 */
static int lv_block_game_move_style_1_block_down(Block_t *block)
{
    uint8_t max_y;
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t i;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����ұ�*/
        /*3����1����±�*/
        /*4���ڿ�����*/
        /*��Ҫ�ж�3���±��Ƿ�����ƶ���4���±� 2���±��Ƿ�����ƶ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(y_index == max_y)/*�����ƶ�*/
        {
            /*y����ʾ��ǰ��������*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_2:
        /*2����1����±�*/
        /*3����1������*/
        /*4���ڿ���ϱ�*/
        /*��Ҫ�ж�2���±ߺ�3���±��Ƿ�����ƶ�*/
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(y_index == max_y)/*�����ƶ�*/
        {
            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_3:
        /*2����1������*/
        /*3����1����ϱ�*/
        /*4���ڿ���ұ�*/
        /*��Ҫ�ж�2���±��Ƿ�����ƶ���1���±��Ƿ�����ƶ�,4���±��Ƿ�����ƶ�*/

        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(y_index == max_y)/*�����ƶ�*/
        {
            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }

        break;
    case STATE_4:
        /*2����1����ϱ�*/
        /*3����1����ұ�*/
        /*4���ڿ���±�*/
        /*��Ҫ�ж�4���±��Ƿ�����ƶ���3���±��Ƿ�����ƶ�*/

        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(y_index == max_y)/*�����ƶ�*/
        {
            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    }

    /*���������ƶ�*/
    for(i = 0; i < 4; i++)
    {
        block->y[i] += 1;
    }
    return RT_OK;
}

/* lv_block_game_move_style_2_block_left
 * �ڶ������͵Ŀ������ƶ�
 */
static void lv_block_game_move_style_2_block_left(Block_t *block)
{
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t i;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����ϱ�*/
        /*4���ڿ�1���±�*/
        /*3����4������*/
        /*��Ҫ�ж�4������Ƿ��пռ�,1������Ƿ��пռ�,2������Ƿ��пռ�*/
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2����1����ұ�*/
        /*4���ڿ�1�����*/
        /*3����4����±�*/
        /*��Ҫ�ж�3������Ƿ��пռ�,4������Ƿ��пռ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2����1����±�*/
        /*4���ڿ�1���ϱ�*/
        /*3����4������*/
        /*��Ҫ�ж�3������Ƿ�����ƶ�,1������Ƿ�����ƶ�,2������Ƿ�����ƶ�*/

        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_4:
        /*2����1������*/
        /*4���ڿ�1���ұ�*/
        /*3����4����ϱ�*/
        /*��Ҫ�ж�2������Ƿ��пռ�,3������Ƿ��пռ�*/
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    }

    /*���������ƶ�*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] -= 1;
    }

}

/* lv_block_game_move_style_2_block_right
 * �ڶ������͵Ŀ������ƶ�
 */
static void lv_block_game_move_style_2_block_right(Block_t *block)
{
    uint8_t max_x;
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t i;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����ϱ�*/
        /*4���ڿ�1���±�*/
        /*3����4������*/
        /*��Ҫ��3���ұ��Ƿ��пռ�,1���ұ��Ƿ��пռ�,2���ұ��Ƿ��пռ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2����1����ұ�*/
        /*4���ڿ�1�����*/
        /*3����4����±�*/
        /*��Ҫ�ж�2���ұ��Ƿ��пռ�,3���ұ��Ƿ��пռ�*/
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2����1����±�*/
        /*4���ڿ�1���ϱ�*/
        /*3����4������*/
        /*��Ҫ�ж�2���ұ��Ƿ�����ƶ�,1���ұ��Ƿ�����ƶ�,4���ұ��Ƿ�����ƶ�*/

        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_4:
        /*2����1������*/
        /*4���ڿ�1���ұ�*/
        /*3����4����ϱ�*/
        /*��Ҫ�ж�4���ұ��Ƿ��пռ�,3���ұ��Ƿ��пռ�*/
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    }

    /*���������ƶ�*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] += 1;
    }

}

/* lv_block_game_move_style_2_block_down
 * �ڶ������͵Ŀ������ƶ�
 */
static int lv_block_game_move_style_2_block_down(Block_t *block)
{
    uint8_t max_y;
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t i;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����ϱ�*/
        /*4���ڿ�1���±�*/
        /*3����4������*/
        /*��Ҫ��4���±��Ƿ��пռ�,3���±��Ƿ��пռ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(y_index == max_y)/*�����ƶ�*/
        {
            /*y����ʾ��ǰ��������*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_2:
        /*2����1����ұ�*/
        /*4���ڿ�1�����*/
        /*3����4����±�*/
        /*��Ҫ��3���±��Ƿ��пռ�,1���±��Ƿ��пռ�,2���±��Ƿ��пռ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(y_index == max_y)/*�����ƶ�*/
        {
            /*y����ʾ��ǰ��������*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_3:
        /*2����1����±�*/
        /*4���ڿ�1���ϱ�*/
        /*3����4������*/
        /*��Ҫ��3���±��Ƿ��пռ�,2���±��Ƿ��пռ�*/
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(y_index == max_y)/*�����ƶ�*/
        {
            /*y����ʾ��ǰ��������*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_4:
        /*2����1������*/
        /*4���ڿ�1���ұ�*/
        /*3����4����ϱ�*/
        /*��Ҫ��2���±��Ƿ��пռ�,1���±��Ƿ��пռ�,4���±��Ƿ��пռ�*/
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(y_index == max_y)/*�����ƶ�*/
        {
            /*y����ʾ��ǰ��������*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    }

    /*���������ƶ�*/
    for(i = 0; i < 4; i++)
    {
        block->y[i] += 1;
    }

}


/* lv_block_game_move_style_3_block_left
 * ���������͵Ŀ������ƶ�
 */
static void lv_block_game_move_style_3_block_left(Block_t *block)
{
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t i;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����±�*/
        /*3����2������*/
        /*4���ڿ�1���ϱ�*/
        /*��Ҫ�ж�3������Ƿ��пռ�,1������Ƿ��пռ�,4������Ƿ��пռ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2����1������*/
        /*3����2����ϱ�*/
        /*4���ڿ�1���ұ�*/
        /*��Ҫ�ж�3������Ƿ��пռ�,2������Ƿ��пռ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2����1����ϱ�*/
        /*3����2����ұ�*/
        /*4���ڿ�1���±�*/
        /*��Ҫ�ж�4������Ƿ�����ƶ�,1������Ƿ�����ƶ�,2������Ƿ�����ƶ�*/

        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_4:
        /*2����1����ұ�*/
        /*3����2����±�*/
        /*4���ڿ�1�����*/
        /*��Ҫ�ж�4������Ƿ��пռ�,3������Ƿ��пռ�*/
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == 0)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    }

    /*���������ƶ�*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] -= 1;
    }

}



/* lv_block_game_move_style_3_block_right
 * ���������͵Ŀ������ƶ�
 */
static void lv_block_game_move_style_3_block_right(Block_t *block)
{
    uint8_t max_x;
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t i;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����±�*/
        /*3����2������*/
        /*4���ڿ�1���ϱ�*/
        /*��Ҫ��4���ұ��Ƿ��пռ�,1���ұ��Ƿ��пռ�,2���ұ��Ƿ��пռ�*/
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2����1������*/
        /*3����2����ϱ�*/
        /*4���ڿ�1���ұ�*/
        /*��Ҫ�ж�4���ұ��Ƿ��пռ�,3���ұ��Ƿ��пռ�*/
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2����1����ϱ�*/
        /*3����2����ұ�*/
        /*4���ڿ�1���±�*/
        /*��Ҫ�ж�3���ұ��Ƿ�����ƶ�,1���ұ��Ƿ�����ƶ�,4���ұ��Ƿ�����ƶ�*/

        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    case STATE_4:
        /*2����1����ұ�*/
        /*3����2����±�*/
        /*4���ڿ�1�����*/
        /*��Ҫ�ж�2���ұ��Ƿ��пռ�,3���ұ��Ƿ��пռ�*/
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == max_x)/*�����ƶ�*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return;
        }
        break;
    }

    /*���������ƶ�*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] += 1;
    }

}

/* lv_block_game_move_style_3_block_down
 * ���������͵Ŀ������ƶ�
 */
static int lv_block_game_move_style_3_block_down(Block_t *block)
{
    uint8_t max_y;
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t i;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����±�*/
        /*3����2������*/
        /*4���ڿ�1���ϱ�*/
        /*��Ҫ��2���±��Ƿ��пռ�,3���±��Ƿ��пռ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(y_index == max_y)/*�����ƶ�*/
        {
            /*y����ʾ��ǰ��������*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_2:
        /*2����1������*/
        /*3����2����ϱ�*/
        /*4���ڿ�1���ұ�*/
        /*��Ҫ��4���±��Ƿ��пռ�,1���±��Ƿ��пռ�,2���±��Ƿ��пռ�*/
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(y_index == max_y)/*�����ƶ�*/
        {
            /*y����ʾ��ǰ��������*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�2��*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_3:
        /*2����1����ϱ�*/
        /*3����2����ұ�*/
        /*4���ڿ�1���±�*/
        /*��Ҫ��4���±��Ƿ��пռ�,3���±��Ƿ��пռ�*/
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(y_index == max_y)/*�����ƶ�*/
        {
            /*y����ʾ��ǰ��������*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_4:
        /*2����1����ұ�*/
        /*3����2����±�*/
        /*4���ڿ�1�����*/
        /*��Ҫ��3���±��Ƿ��пռ�,1���±��Ƿ��пռ�,4���±��Ƿ��пռ�*/
        /*���ж�3��*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(y_index == max_y)/*�����ƶ�*/
        {
            /*y����ʾ��ǰ��������*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�1��*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        /*���ж�4��*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*�Ѿ�����䲻��������*/
        {
            return RT_NEED_NEW;
        }
        break;
    }

    /*���������ƶ�*/
    for(i = 0; i < 4; i++)
    {
        block->y[i] += 1;
    }

}

/* lv_block_game_move_block_left
 * ��һ�����������ƶ�
 */
static void lv_block_game_move_block_left(Block_t *block)
{
    switch(block->type)
    {
    case STYLE_1:
        lv_block_game_move_style_1_block_left(block);
        break;
    case STYLE_2:
        lv_block_game_move_style_2_block_left(block);
        break;
    case STYLE_3:
        lv_block_game_move_style_3_block_left(block);
        break;
    }
    lv_block_game_update_current_block();
}

/* lv_block_game_move_block_left
 * ��һ�����������ƶ�
 */
static void lv_block_game_move_block_right(Block_t *block)
{
    switch(block->type)
    {
    case STYLE_1:
        lv_block_game_move_style_1_block_right(block);
        break;
    case STYLE_2:
        lv_block_game_move_style_2_block_right(block);
        break;
    case STYLE_3:
        lv_block_game_move_style_3_block_right(block);
        break;
    }
    lv_block_game_update_current_block();

}

/* lv_block_game_check_row_and_clear
 * �жϵ�ǰ���Ƿ�����,������������ƶ�
 */
static int lv_block_game_check_row_and_clear(uint8_t row)
{
    uint8_t max_x;
    //printf("row = %d\r\n", row);
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    int i,j;
    for(i= 0; i <= max_x; i++)
    {
        if(g_pt_lv_blockgame->block[i][row].is_fill != IS_FILL)
        {
            break;
        }
    }
    //printf("i= %d\r\n", i);
    if(i == max_x + 1)/*����*/
    {
        //printf("����\r\n");
        for(i = row; i > 0; i--)
        {
            for(j = 0; j <= max_x; j++)
            {
                g_pt_lv_blockgame->block[j][i].is_fill = g_pt_lv_blockgame->block[j][i-1].is_fill;
                g_pt_lv_blockgame->block[j][i].color = g_pt_lv_blockgame->block[j][i-1].color;
                /*���¸ı���е���ɫ*/
                lv_obj_set_style_bg_color(g_pt_lv_blockgame->block[j][i].obj, lv_color_hex(g_pt_lv_blockgame->block[j][i].color), 0);
            }
        }
        /*���µ�0������*/
        for(j = 0; j <= max_x; j++)
        {
            g_pt_lv_blockgame->block[j][0].is_fill = NOT_FILL;
            g_pt_lv_blockgame->block[j][0].color = LV_BLOCK_GAME_BASE_BLOCK_DEFAULT_COLOR;
            /*���¸ı���е���ɫ*/
            lv_obj_set_style_bg_color(g_pt_lv_blockgame->block[j][0].obj, lv_color_hex(g_pt_lv_blockgame->block[j][0].color), 0);
        }

    }
}

/* lv_block_game_check_row_full
 * ���һ�����ڵĿ��Ƿ������һ����
 */
static void lv_block_game_check_row_full(Block_t *block)
{
    uint8_t max_y;
    uint8_t max_x;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    uint8_t top_y;
    int i,j;

    /*�ȴ���������ҵ��ʼ���ĸ߶�*/
    for(j = 0; j <= max_y; j++)
    {
        for(i = 0; i <= max_x; i++)
        {
            if(g_pt_lv_blockgame->block[i][j].is_fill == IS_FILL)
            {
                //printf("j = %d\r\n", j);
                top_y = j;
                break;
            }
        }
        if(i <= max_x)
        {
            break;
        }
    }
    printf("top_y = %d\r\n", top_y);
    /*�ҵ���͵���ײ��, Ȼ��һֱ���ϱ���*/
    for(i = max_y; i >= top_y; i--)
    {
        lv_block_game_check_row_and_clear(i);
    }


}



/* lv_block_game_move_block_down
 * ��һ�����������ƶ�
 */
static void lv_block_game_move_block_down(Block_t *block)
{
    int nRet;
    int i;
    int j;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t max_y;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    switch(block->type)
    {
    case STYLE_1:
        nRet = lv_block_game_move_style_1_block_down(block);
        break;
    case STYLE_2:
        nRet = lv_block_game_move_style_2_block_down(block);
        break;
    case STYLE_3:
        nRet = lv_block_game_move_style_3_block_down(block);
        break;
    }
    #if 1
    if(nRet == RT_NEED_NEW)
    {
        //printf("move down\r\n");
        /*���±��������״̬ ����ɫ*/
        for(i = 0; i < 4; i++)
        {
            x_index = block->x[i];
            y_index = block->y[i];
            g_pt_lv_blockgame->block[x_index][y_index].is_fill = IS_FILL;
            g_pt_lv_blockgame->block[x_index][y_index].color = block->color;
            //printf("(%d,%d)\r\n", x_index, y_index);
        }
        /*����Ƿ���һ������*/
        lv_block_game_check_row_full(block);

        /*��λcurrent���״̬*/
        lv_block_game_reset_current_block();
        /*����һ���µĿ�*/
        lv_block_game_new_the_block();
        /*�жϴ������Ƿ��������ײ*/
        uint8_t flag = 0;
        for(i = 0; i < 4; i++)
        {
            x_index = block->x[i];
            y_index = block->y[i];
            if(g_pt_lv_blockgame->block[x_index][y_index].is_fill == IS_FILL)
            {
                flag = 1;
                break;
            }
        }

        if(flag == 1)/*game over*/
        {

            /*���¿�ʼ��Ϸ*/
            lv_block_game_restart();


        }



    }
    #endif

    lv_block_game_update_current_block();

}

/* lv_block_game_align
 * ���������������
 */
static void lv_block_game_align(uint8_t index, uint8_t index_to, int *x, int *y, uint8_t align_type)
{
    switch(align_type)
    {
    case BL_ALIGN_TOP:      /*�϶���*/
        x[index - 1] = x[index_to - 1];
        y[index - 1] = y[index_to - 1] - 1;
        break;
     case BL_ALIGN_BOTTOM:  /*�¶���*/
        x[index - 1] = x[index_to - 1];
        y[index - 1] = y[index_to - 1] + 1;
        break;
    case BL_ALIGN_LEFT:     /*�����*/
        x[index - 1] = x[index_to - 1] - 1;
        y[index - 1] = y[index_to - 1];
        break;
    case BL_ALIGN_RIGHT:    /*�Ҷ���*/
        x[index - 1] = x[index_to - 1] + 1;
        y[index - 1] = y[index_to - 1];
        break;

    }

}

/* lv_block_game_set_style_1_state
 * ���ÿ�1��״̬
 */
static void lv_block_game_set_style_1_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����ұ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3����1����±�*/
        lv_block_game_align(3, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4���ڿ�����*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
        break;
    case STATE_2:
        /*2����1����±�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3����1������*/
        lv_block_game_align(3, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*4���ڿ���ϱ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        break;
    case STATE_3:
        /*2����1������*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*3����1����ϱ�*/
        lv_block_game_align(3, 1, block->x, block->y, BL_ALIGN_TOP);
        /*4���ڿ���ұ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_RIGHT);
        break;
    case STATE_4:
        /*2����1����ϱ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_TOP);
        /*3����1����ұ�*/
        lv_block_game_align(3, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*4���ڿ���±�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * ������1�ֿ�
 */
static void lv_block_game_create_style_1_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_1;/*��ǰ�������*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_1_state(block);
}

/* lv_block_game_switch_style_1_state
 * �л���1�ֿ��״̬
 */
static void lv_block_game_switch_style_1_state(Block_t *block)
{
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t max_x;
    uint8_t max_y;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    /*����״̬˳ʱ����ת*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1״̬�л���2״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1���ϱ��Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        lv_block_game_set_style_1_state(block);
        break;
    case STATE_2:
        /*2״̬�л���3״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*4���x����+1*/
        /*�ж�1���ұ��Ƿ���1���ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == max_x)/*�ұ�û�ռ�*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_3;
        lv_block_game_set_style_1_state(block);
        break;
    case STATE_3:
        /*3״̬�л���4״̬��Ҫ�ж�1���±��Ƿ��пռ�*/
        /*4���y����+1*/
        /*�ж�1���±��Ƿ���1���ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)/*�±�û�ռ�*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        block->state = STATE_4;
        lv_block_game_set_style_1_state(block);
        break;
    case STATE_4:
        /*4״̬�л���1״̬��Ҫ�ж�����Ƿ��пռ�*/
        /*4���x����-1*/
        /*�ж�1������Ƿ���1���ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == 0)/*���û�ռ�*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        block->state = STATE_1;
        lv_block_game_set_style_1_state(block);
        break;
    }
}

/* lv_block_game_set_style_2_state
 * ���ÿ�2��״̬
 */
static void lv_block_game_set_style_2_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����ϱ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_TOP);
        /*4���ڿ�1���±�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3����4������*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_LEFT);
        break;
    case STATE_2:
        /*2����1����ұ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*4���ڿ�1�����*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*3����4����±�*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    case STATE_3:
        /*2����1����±�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4���ڿ�1���ϱ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        /*3����4������*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_LEFT);
        break;
    case STATE_4:
        /*2����1������*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*4���ڿ�1���ұ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3����4����ϱ�*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_TOP);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * ������1�ֿ�
 */
static void lv_block_game_create_style_2_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_2;/*��ǰ�������*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_2_state(block);
}

/* lv_block_game_switch_style_2_state
 * �л���2�ֿ��״̬
 */
static void lv_block_game_switch_style_2_state(Block_t *block)
{
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t max_x;
    uint8_t max_y;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    /*����״̬˳ʱ����ת*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1״̬�л���2״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1������Ƿ���һ���ռ� 4������Ƿ���1���ռ� 1���ұ��Ƿ���1���ռ�*/
        /*�ж�1���ұ��Ƿ���1���ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1������Ƿ��пռ�*/
        if(x_index == 0)/*û�ռ�*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�4������Ƿ�ռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        lv_block_game_set_style_2_state(block);
        break;
    case STATE_2:
        /*2״̬�л���3״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1���±��Ƿ��пռ�,4���ϱ��Ƿ��пռ�,1���ϱ��Ƿ��пռ�*/
        /*�ж�1���±��Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���ϱ��Ƿ��пռ�*/
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�4���ϱ��Ƿ��пռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        block->state = STATE_3;
        lv_block_game_set_style_2_state(block);
        break;
    case STATE_3:
        /*3״̬�л���4״̬��Ҫ�ж�1���±��Ƿ��пռ�*/
        /*��Ҫ�ж�1������Ƿ��пռ�,1���ұ��Ƿ��пռ�,4���ұ��Ƿ��пռ�*/
        /*�ж�1������Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���ұ��Ƿ��пռ�*/
        if(x_index == max_x)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�4���ұ��Ƿ��пռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_4;
        lv_block_game_set_style_2_state(block);
        break;
    case STATE_4:
        /*4״̬�л���1״̬��Ҫ�ж�����Ƿ��пռ�*/
        /*��Ҫ�ж�1���ϱߺ��±��Ƿ��пռ�,4���±��Ƿ��пռ�*/
        /*�ж�1���±�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)/*�±�û�ռ�*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���ϱ��Ƿ��пռ�*/
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�4���±��Ƿ��пռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_1;
        lv_block_game_set_style_2_state(block);
        break;
    }
}


/* lv_block_game_set_style_3_state
 * ���ÿ�3��״̬
 */
static void lv_block_game_set_style_3_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����±�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3����2������*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_LEFT);
        /*4���ڿ�1���ϱ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        break;
    case STATE_2:
        /*2����1������*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*3����2����ϱ�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_TOP);
        /*4���ڿ�1���ұ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_RIGHT);
        break;
    case STATE_3:
        /*2����1����ϱ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_TOP);
        /*3����2����ұ�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_RIGHT);
        /*4���ڿ�1���±�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    case STATE_4:
        /*2����1����ұ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3����2����±�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4���ڿ�1�����*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * ������1�ֿ�
 */
static void lv_block_game_create_style_3_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_3;/*��ǰ�������*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_3_state(block);
}

/* lv_block_game_switch_style_3_state
 * �л���3�ֿ��״̬
 */
static void lv_block_game_switch_style_3_state(Block_t *block)
{
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t max_x;
    uint8_t max_y;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    /*����״̬˳ʱ����ת*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1״̬�л���2״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1���ұ��Ƿ���һ���ռ� 4������Ƿ���1���ռ� 1������Ƿ���1���ռ�*/
        /*�ж�1���ұ��Ƿ���1���ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == max_x)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1������Ƿ��пռ�*/
        if(x_index == 0)/*û�ռ�*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�4������Ƿ�ռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        break;
    case STATE_2:
        /*2״̬�л���3״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1���±��Ƿ��пռ�,4���ϱ��Ƿ��пռ�,1���ϱ��Ƿ��пռ�*/
        /*�ж�1���±��Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���ϱ��Ƿ��пռ�*/
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�4���ϱ��Ƿ��пռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        block->state = STATE_3;
        break;
    case STATE_3:
        /*3״̬�л���4״̬��Ҫ�ж�1���±��Ƿ��пռ�*/
        /*��Ҫ�ж�1������Ƿ��пռ�,1���ұ��Ƿ��пռ�,4���ұ��Ƿ��пռ�*/
        /*�ж�1������Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == 0)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���ұ��Ƿ��пռ�*/
        if(x_index == max_x)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�4���ұ��Ƿ��пռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_4;
        break;
    case STATE_4:
        /*4״̬�л���1״̬��Ҫ�ж�����Ƿ��пռ�*/
        /*��Ҫ�ж�1���ϱߺ��±��Ƿ��пռ�,4���±��Ƿ��пռ�*/
        /*�ж�1���±�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)/*�±�û�ռ�*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���ϱ��Ƿ��пռ�*/
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�4���±��Ƿ��пռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_1;
        break;
    }
    lv_block_game_set_style_3_state(block);
}

/* lv_block_game_set_style_4_state
 * ���ÿ�4��״̬
 */
static void lv_block_game_set_style_4_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����ұ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*4���ڿ�1���ϱ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        /*3����4������*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_LEFT);
        break;
    case STATE_2:
        /*2����1����±�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4���ڿ�1���ұ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3����4����ϱ�*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_TOP);
        break;
    case STATE_3:
        /*2����1������*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*4���ڿ�1���±�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3����4����ұ�*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_RIGHT);
        break;
    case STATE_4:
        /*2����1����ϱ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_TOP);
        /*4���ڿ�1�����*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*3����4����±�*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    }
}

/* lv_block_game_create_style_4_block
 * ���������ֿ�
 */
static void lv_block_game_create_style_4_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_4;/*��ǰ�������*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_4_state(block);
}

/* lv_block_game_switch_style_4_state
 * �л���4�ֿ��״̬
 */
static void lv_block_game_switch_style_4_state(Block_t *block)
{
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t max_x;
    uint8_t max_y;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    /*����״̬˳ʱ����ת*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1״̬�л���2״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1���±��Ƿ���һ���ռ� 4���ұ��Ƿ���1���ռ�*/
        /*�ж�1���±��Ƿ���1���ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�4���ұ��Ƿ�ռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        break;
    case STATE_2:
        /*2״̬�л���3״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1������Ƿ��пռ�,4���±��Ƿ��пռ�*/
        /*�ж�1������Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == 0)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        /*�ж�4���±��Ƿ��пռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        block->state = STATE_3;
        break;
    case STATE_3:
        /*3״̬�л���4״̬��Ҫ�ж�1���±��Ƿ��пռ�*/
        /*��Ҫ�ж�1���ϱ��Ƿ��пռ�,4������Ƿ��пռ�*/
        /*�ж�1���ϱ��Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        /*�ж�4������Ƿ��пռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_4;
        break;
    case STATE_4:
        /*4״̬�л���1״̬��Ҫ�ж�����Ƿ��пռ�*/
        /*��Ҫ�ж�1���ұ��Ƿ��пռ�,4���ϱ��Ƿ��пռ�*/
        /*�ж�1���ұ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == max_x)/*�ұ�û�ռ�*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�4���ϱ��Ƿ��пռ�*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_1;

        break;
    }
    lv_block_game_set_style_4_state(block);
}


/* lv_block_game_set_style_5_state
 * ���ÿ�5��״̬
 */
static void lv_block_game_set_style_5_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2����1����ϱ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_TOP);
        /*3����2����ұ�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_RIGHT);
        /*4���ڿ�1�����*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
        break;
    case STATE_2:
        /*2����1����ұ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3����2����±�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4���ڿ�1���ϱ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        break;
    case STATE_3:
        /*2����1����±�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3����2������*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_LEFT);
        /*4���ڿ�1���ұ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_RIGHT);
        break;
    case STATE_4:
        /*2����1������*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*3����2����ϱ�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_TOP);
        /*4���ڿ�1���±�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    }
}

/* lv_block_game_create_style_5_block
 * ���������ֿ�
 */
static void lv_block_game_create_style_5_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_5;/*��ǰ�������*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_5_state(block);
}

/* lv_block_game_switch_style_5_state
 * �л���5�ֿ��״̬
 */
static void lv_block_game_switch_style_5_state(Block_t *block)
{
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t max_x;
    uint8_t max_y;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    /*����״̬˳ʱ����ת*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1״̬�л���2״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1���ұ߱��Ƿ���һ���ռ� 1���ұߵ��±��Ƿ���1���ռ�*/
        /*�ж�1���ұ��Ƿ���1���ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];

        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���ұߵ��±��Ƿ�ռ�*/
        x_index = block->x[0] + 1;
        y_index = block->y[0];
        if(y_index == max_y)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        break;
    case STATE_2:
        /*2״̬�л���3״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1���±��Ƿ��пռ�,1���±ߵ�����Ƿ��пռ�*/
        /*�ж�1���±��Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        /*�ж�1���±ߵ�����Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0] + 1;
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        block->state = STATE_3;
        break;
    case STATE_3:
        /*3״̬�л���4״̬��Ҫ�ж�1���±��Ƿ��пռ�*/
        /*��Ҫ�ж�1������Ƿ��пռ�,1����ߵ��ϱ��Ƿ��пռ�*/
        /*�ж�1������Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        /*�ж�1����ߵ��ϱ��Ƿ��пռ�*/
        x_index = block->x[0] - 1;
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_4;
        break;
    case STATE_4:
        /*4״̬�л���1״̬��Ҫ�ж�����Ƿ��пռ�*/
        /*��Ҫ�ж�1���ϱ��Ƿ��пռ�,1���ϱߵ��ұ��Ƿ��пռ�*/
        /*�ж�1���ϱ�*/
        x_index = block->x[0];
        y_index = block->y[0];

        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���ϱߵ��ұ��Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0] - 1;
        if(x_index == max_x)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_1;

        break;
    }
    lv_block_game_set_style_5_state(block);
}


/* lv_block_game_set_style_6_state
 * ���ÿ�6��״̬
 */
static void lv_block_game_set_style_6_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)/*ֻ��һ��״̬*/
    {
    case STATE_1:
    case STATE_2:
    case STATE_3:
    case STATE_4:
        /*2����1����ұ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3����2����±�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4���ڿ�1���±�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    }
}

/* lv_block_game_create_style_6_block
 * ������6�ֿ�
 */
static void lv_block_game_create_style_6_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_6;/*��ǰ�������*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_6_state(block);
}

/* lv_block_game_switch_style_6_state
 * �л���6�ֿ��״̬
 */
static void lv_block_game_switch_style_6_state(Block_t *block)
{
    /*1װ״̬����Ҫ�л�*/
    return;
}


/* lv_block_game_set_style_7_state
 * ���ÿ�7��״̬
 */
static void lv_block_game_set_style_7_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)/*ֻ��������״̬*/
    {
    case STATE_1:
        /*2����1����ұ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3����2����ұ�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_RIGHT);
        /*4���ڿ�1�����*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
    case STATE_2:
        /*2����1����±�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3����2����±�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4���ڿ�1���ϱ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
    case STATE_3:
        /*2����1����ұ�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3����2����ұ�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_RIGHT);
        /*4���ڿ�1�����*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
    case STATE_4:
        /*2����1����±�*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3����2����±�*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4���ڿ�1���ϱ�*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        break;
    }
}

/* lv_block_game_create_style_7_block
 * ������7�ֿ�
 */
static void lv_block_game_create_style_7_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_7;/*��ǰ�������*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_7_state(block);
}

/* lv_block_game_switch_style_7_state
 * �л���7�ֿ��״̬
 */
static void lv_block_game_switch_style_7_state(Block_t *block)
{
    uint8_t state;
    uint8_t x_index;
    uint8_t y_index;
    uint8_t max_x;
    uint8_t max_y;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    state = block->state;
    /*����״̬˳ʱ����ת*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1״̬�л���2״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1���ϱ��Ƿ���һ���ռ� 1���±��Ƿ���1���ռ�,1���±ߵ��±��Ƿ��ҿռ�*/
        /*�ж�1���ϱ��Ƿ���1���ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];

        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���±ߵ��±����Ƿ�ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)/*1���Ѿ��������*/
        {
            break;
        }
        y_index += 1;/*1�����һ��*/
        if(y_index == max_y)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���±���û�пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        break;
    case STATE_2:
        /*2״̬�л���3״̬��Ҫ�ж��ұ��Ƿ��пռ�*/
        /*��Ҫ�ж�1������Ƿ��пռ�,1����ұ��Ƿ��ҿռ�,1���ұߵ��ұ��Ƿ��пռ�*/
        /*�ж�1������Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == 0)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        /*�ж�1���ұߵ��ұ��Ƿ��пռ�*/
        if(x_index == max_x)/*1��������ұ���*/
        {
            break;
        }
        x_index += 1;
        if(x_index == max_x)/*1����ұ������ұ���*/
        {
            break;
        }
        /*�ж�1���ұߵ��ұ��Ƿ��пռ�*/
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1����ұ��Ƿ��пռ�*/
        if(g_pt_lv_blockgame->block[x_index][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        block->state = STATE_3;
        break;
    case STATE_3:
        /*3״̬�л���4״̬��Ҫ�ж�1���±��Ƿ��пռ�*/
        /*��Ҫ�ж�1���ϱ��Ƿ���һ���ռ� 1���±��Ƿ���1���ռ�,1���±ߵ��±��Ƿ��ҿռ�*/
        /*�ж�1���ϱ��Ƿ���1���ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];

        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���±ߵ��±����Ƿ�ռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)/*1���Ѿ��������*/
        {
            break;
        }
        y_index += 1;/*1�����һ��*/
        if(y_index == max_y)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1���±���û�пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_4;
        break;
    case STATE_4:
        /*4״̬�л���1״̬��Ҫ�ж�����Ƿ��пռ�*/
        /*��Ҫ�ж�1������Ƿ��пռ�,1����ұ��Ƿ��ҿռ�,1���ұߵ��ұ��Ƿ��пռ�*/
        /*�ж�1������Ƿ��пռ�*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == 0)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        /*�ж�1���ұߵ��ұ��Ƿ��пռ�*/
        if(x_index == max_x)/*1��������ұ���*/
        {
            break;
        }
        x_index += 1;
        if(x_index == max_x)/*1����ұ������ұ���*/
        {
            break;
        }
        /*�ж�1���ұߵ��ұ��Ƿ��пռ�*/
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }
        /*�ж�1����ұ��Ƿ��пռ�*/
        if(g_pt_lv_blockgame->block[x_index][y_index].is_fill == IS_FILL)/*û�ռ�*/
        {
            break;
        }

        block->state = STATE_1;

        break;
    }
    lv_block_game_set_style_7_state(block);
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
	    if(g_pt_lv_blockgame->update_timer != NULL) lv_timer_del(g_pt_lv_blockgame->update_timer);
		if(g_pt_lv_blockgame->bg_about != NULL) lv_obj_del(g_pt_lv_blockgame->bg_about);

		vPortFree(g_pt_lv_blockgame);

		lv_main_page_demo_home(100);
	}
}


/* event_handler_left_move
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_left_move(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
        /*����*/
        lv_block_game_move_block_left(&current_block);
	}
}

/* event_handler_right_move
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_right_move(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
        /*����*/
        lv_block_game_move_block_right(&current_block);
	}
}

/* event_handler_right_move
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_switch_state(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
        /*�л�״̬*/
        lv_block_game_switch_block_state(&current_block);
	}
}

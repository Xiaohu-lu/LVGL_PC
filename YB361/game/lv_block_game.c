/* lvgl
 * */
#include "lvgl/lvgl.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "lv_block_game.h"
#include "lv_dev_info_page.h"

static PT_lv_blockgame g_pt_lv_blockgame;  // 数据结构体

static lv_style_t style_baseblock;/*容器样式*/

static Block_t last_block;      /*上一个块信息*/
static Block_t current_block;   /*当前块信息*/

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
 * 俄罗斯方块游戏
 */
void lv_block_game(void)
{
    g_pt_lv_blockgame = (T_lv_blockgame*)pvPortMalloc(sizeof(T_lv_blockgame));
	g_pt_lv_blockgame->bg_about = lv_obj_create(lv_scr_act());/*创建背景*/

	lv_obj_set_size(g_pt_lv_blockgame->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_radius(g_pt_lv_blockgame->bg_about, 0, 0);/*设置圆角*/
	lv_obj_align(g_pt_lv_blockgame->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);


	/*设置padding*/
	set_user_style(g_pt_lv_blockgame->bg_about);

	lv_obj_set_size(g_pt_lv_blockgame->bg_about, LV_HOR_RES, LV_VER_RES);/*设置大小*/
	lv_obj_set_style_bg_opa(g_pt_lv_blockgame->bg_about, LV_OPA_100, 0);/*设置背景透明度*/
	lv_obj_set_style_bg_color(g_pt_lv_blockgame->bg_about, lv_color_hex(WHITE_HEX), 0);
	lv_block_game_init();/*界面初始化*/

	add_title(g_pt_lv_blockgame->bg_about, "Block Game");
	add_back(g_pt_lv_blockgame->bg_about, event_handler_back_to_home);

}

/* lv_block_game_init
 * 界面初始化
 */
static void lv_block_game_init(void)
{
    /*创建容器样式*/
    static lv_style_t style_cont;/*容器样式*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_cont, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, LV_BLOCK_GAME_VALID_CONT_BORDER_WIDTH);


	lv_style_init(&style_baseblock);
	lv_style_set_bg_opa(&style_baseblock, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_baseblock, 10);	/*设置圆角*/
	//lv_style_set_bg_color(&style_baseblock, lv_color_hex(RED_HEX));/*设置背景颜色白色*/
	lv_style_set_border_color(&style_baseblock, lv_color_hex(LV_BLOCK_GAME_BASE_BLOCK_BORDER_COLOR));
	lv_style_set_pad_top(&style_baseblock, 0);
	lv_style_set_pad_bottom(&style_baseblock, 0);
	lv_style_set_pad_left(&style_baseblock, 0);
	lv_style_set_pad_right(&style_baseblock, 0);
    lv_style_set_border_width(&style_baseblock, 1);
    lv_style_set_radius(&style_baseblock, 0);

    /*创建游戏有效区域*/
    g_pt_lv_blockgame->game_value_cont = lv_obj_create(g_pt_lv_blockgame->bg_about);
    lv_obj_t *cont = g_pt_lv_blockgame->game_value_cont;
    lv_obj_add_style(cont, &style_cont, 0);
    lv_obj_set_size(cont, LV_BLOCK_GAME_VALID_CONT_W + LV_BLOCK_GAME_VALID_CONT_BORDER_WIDTH * 2, LV_BLOCK_GAME_VALID_CONT_H + LV_BLOCK_GAME_VALID_CONT_BORDER_WIDTH * 2);
    lv_obj_align(cont, LV_ALIGN_TOP_LEFT, 4, 40);
    lv_obj_set_style_radius(cont, 0, 0);

    static lv_style_t style_btn;/*按钮样式*/
	lv_style_init(&style_btn);
	lv_style_set_bg_opa(&style_btn, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_btn, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_btn, lv_color_hex(BLUE_HEX));/*设置背景颜色白色*/
	lv_style_set_pad_top(&style_btn, 0);
	lv_style_set_pad_bottom(&style_btn, 0);
	lv_style_set_pad_left(&style_btn, 0);
	lv_style_set_pad_right(&style_btn, 0);
    lv_style_set_border_width(&style_btn, 0);

    lv_obj_t *btn;
    lv_obj_t *label;
    /*创建左按钮*/
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


    /*创建右按钮*/
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

    /*创建切换按钮*/
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

    /*复位当前块的状态*/
    lv_block_game_reset_current_block();

    /*创建1个方块*/
    lv_block_game_new_block(&current_block, STYLE_1, RED_HEX);

    lv_block_game_switch_block_state(&current_block);

    /*创建一个定时器*/
    g_pt_lv_blockgame->update_timer = lv_timer_create(lv_test_block_timer, 400, NULL);

}

static void lv_test_block_timer(lv_timer_t * timer)
{
    lv_block_game_move_block_down(&current_block);
}

/* lv_block_game_create_border
 * 创建界面的交界线
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
 * 创建背景的所有块
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
 * 重新初始化索所有的背景块
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
 * 复位初始块的状态
 */
static void lv_block_game_reset_current_block(void)
{
    current_block.type = STYLE_LAST;
    last_block.type = STYLE_LAST;

}

/* lv_block_game_new_block
 * 创建一个新的块
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
    /*随机颜色*/
    /*设置随机数种子*/
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
 * 重新开始游戏
 */
static void lv_block_game_restart(void)
{
    /*暂停定时器*/
    lv_timer_pause(g_pt_lv_blockgame->update_timer);
    /*复位背景块信息*/
    lv_block_game_reset_all_base_block();
    lv_block_game_reset_current_block();
    /*创建新的块*/
    lv_block_game_new_the_block();
    /*重新启动定时器*/
    lv_timer_resume(g_pt_lv_blockgame->update_timer);
}

/* lv_block_game_fill_block
 * 填充一个块的颜色
 */
static void lv_block_game_fill_block(Block_t *block)
{
    int i;
    if(last_block.type == STYLE_LAST)/*块无效*/
    {
        return;
    }
    /*更新块的颜色*/
    for(i = 0; i < 4; i++)
    {
        if(block->x[i] >= 0 && block->y[i] >= 0)
        {
            lv_obj_set_style_bg_color(g_pt_lv_blockgame->block[block->x[i]][block->y[i]].obj, lv_color_hex(block->color), 0);
        }
    }
}

/* lv_block_game_update_current_block
 * 填充当前块的颜色
 */
static void lv_block_game_update_current_block(void)
{
    lv_block_game_fill_block(&last_block);
    lv_block_game_fill_block(&current_block);
    memcpy((uint8_t*)&last_block, (uint8_t*)&current_block, sizeof(last_block));
    last_block.color = LV_BLOCK_GAME_BASE_BLOCK_DEFAULT_COLOR;
}

/* lv_block_game_switch_block_state
 * 切换一个块的状态
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
 * 第一种类型的块向左移动
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
        /*2块在1块的右边*/
        /*3块在1块的下边*/
        /*4块在块的左边*/
        /*需要判断4块左边是否可以移动和3块左边是否可以移动*/
        /*先判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2块在1块的下边*/
        /*3块在1块的左边*/
        /*4块在块的上边*/
        /*需要判断3块左边是否可以移动和2块左边是否可以移动*/
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2块在1块的左边*/
        /*3块在1块的上边*/
        /*4块在块的右边*/
        /*需要判断2块左边是否可以移动*/

        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }

        break;
    case STATE_4:
        /*2块在1块的上边*/
        /*3块在1块的右边*/
        /*4块在块的下边*/
        /*需要判断4块左边是否可以移动*/
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    }

    /*可以向左移动*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] -= 1;
    }

}

/* lv_block_game_move_style_1_block_right
 * 第一种类型的块向右移动
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
        /*2块在1块的右边*/
        /*3块在1块的下边*/
        /*4块在块的左边*/
        /*需要判断2块右边是否可以移动和3块右边是否可以移动*/
        /*先判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2块在1块的下边*/
        /*3块在1块的左边*/
        /*4块在块的上边*/
        /*需要判断2块右边是否可以移动*/
        /*先判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2块在1块的左边*/
        /*3块在1块的上边*/
        /*4块在块的右边*/
        /*需要判断4块右边是否可以移动和3块右边是否可以移动*/

        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }

        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }

        break;
    case STATE_4:
        /*2块在1块的上边*/
        /*3块在1块的右边*/
        /*4块在块的下边*/
        /*需要判断3块右边是否可以移动和4块右边是否可以移动*/
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        break;
    }

    /*可以向右移动*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] += 1;
    }

}

/* lv_block_game_move_style_1_block_down
 * 第一种类型的块向下移动
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
        /*2块在1块的右边*/
        /*3块在1块的下边*/
        /*4块在块的左边*/
        /*需要判断3块下边是否可以移动和4块下边 2块下边是否可以移动*/
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(y_index == max_y)/*不能移动*/
        {
            /*y最大表示当前块碰壁了*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_2:
        /*2块在1块的下边*/
        /*3块在1块的左边*/
        /*4块在块的上边*/
        /*需要判断2块下边和3块下边是否可以移动*/
        /*先判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(y_index == max_y)/*不能移动*/
        {
            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_3:
        /*2块在1块的左边*/
        /*3块在1块的上边*/
        /*4块在块的右边*/
        /*需要判断2块下边是否可以移动和1块下边是否可以移动,4块下边是否可以移动*/

        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(y_index == max_y)/*不能移动*/
        {
            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }

        break;
    case STATE_4:
        /*2块在1块的上边*/
        /*3块在1块的右边*/
        /*4块在块的下边*/
        /*需要判断4块下边是否可以移动和3块下边是否可以移动*/

        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(y_index == max_y)/*不能移动*/
        {
            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    }

    /*可以向下移动*/
    for(i = 0; i < 4; i++)
    {
        block->y[i] += 1;
    }
    return RT_OK;
}

/* lv_block_game_move_style_2_block_left
 * 第二种类型的块向左移动
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
        /*2块在1块的上边*/
        /*4块在块1的下边*/
        /*3块在4块的左边*/
        /*需要判断4块左边是否有空间,1块左边是否有空间,2块左边是否有空间*/
        /*先判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2块在1块的右边*/
        /*4块在块1的左边*/
        /*3块在4块的下边*/
        /*需要判断3块左边是否有空间,4块左边是否有空间*/
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2块在1块的下边*/
        /*4块在块1的上边*/
        /*3块在4块的左边*/
        /*需要判断3块左边是否可以移动,1块左边是否可以移动,2块左边是否可以移动*/

        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    case STATE_4:
        /*2块在1块的左边*/
        /*4块在块1的右边*/
        /*3块在4块的上边*/
        /*需要判断2块左边是否有空间,3块左边是否有空间*/
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    }

    /*可以向左移动*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] -= 1;
    }

}

/* lv_block_game_move_style_2_block_right
 * 第二种类型的块向右移动
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
        /*2块在1块的上边*/
        /*4块在块1的下边*/
        /*3块在4块的左边*/
        /*需要判3块右边是否有空间,1块右边是否有空间,2块右边是否有空间*/
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2块在1块的右边*/
        /*4块在块1的左边*/
        /*3块在4块的下边*/
        /*需要判断2块右边是否有空间,3块右边是否有空间*/
        /*先判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2块在1块的下边*/
        /*4块在块1的上边*/
        /*3块在4块的左边*/
        /*需要判断2块右边是否可以移动,1块右边是否可以移动,4块右边是否可以移动*/

        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    case STATE_4:
        /*2块在1块的左边*/
        /*4块在块1的右边*/
        /*3块在4块的上边*/
        /*需要判断4块右边是否有空间,3块右边是否有空间*/
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    }

    /*可以向左移动*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] += 1;
    }

}

/* lv_block_game_move_style_2_block_down
 * 第二种类型的块向下移动
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
        /*2块在1块的上边*/
        /*4块在块1的下边*/
        /*3块在4块的左边*/
        /*需要判4块下边是否有空间,3块下边是否有空间*/
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(y_index == max_y)/*不能移动*/
        {
            /*y最大表示当前块碰壁了*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_2:
        /*2块在1块的右边*/
        /*4块在块1的左边*/
        /*3块在4块的下边*/
        /*需要判3块下边是否有空间,1块下边是否有空间,2块下边是否有空间*/
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(y_index == max_y)/*不能移动*/
        {
            /*y最大表示当前块碰壁了*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_3:
        /*2块在1块的下边*/
        /*4块在块1的上边*/
        /*3块在4块的左边*/
        /*需要判3块下边是否有空间,2块下边是否有空间*/
        /*先判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(y_index == max_y)/*不能移动*/
        {
            /*y最大表示当前块碰壁了*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_4:
        /*2块在1块的左边*/
        /*4块在块1的右边*/
        /*3块在4块的上边*/
        /*需要判2块下边是否有空间,1块下边是否有空间,4块下边是否有空间*/
        /*先判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(y_index == max_y)/*不能移动*/
        {
            /*y最大表示当前块碰壁了*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    }

    /*可以向下移动*/
    for(i = 0; i < 4; i++)
    {
        block->y[i] += 1;
    }

}


/* lv_block_game_move_style_3_block_left
 * 第三种类型的块向左移动
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
        /*2块在1块的下边*/
        /*3块在2块的左边*/
        /*4块在块1的上边*/
        /*需要判断3块左边是否有空间,1块左边是否有空间,4块左边是否有空间*/
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2块在1块的左边*/
        /*3块在2块的上边*/
        /*4块在块1的右边*/
        /*需要判断3块左边是否有空间,2块左边是否有空间*/
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2块在1块的上边*/
        /*3块在2块的右边*/
        /*4块在块1的下边*/
        /*需要判断4块左边是否可以移动,1块左边是否可以移动,2块左边是否可以移动*/

        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    case STATE_4:
        /*2块在1块的右边*/
        /*3块在2块的下边*/
        /*4块在块1的左边*/
        /*需要判断4块左边是否有空间,3块左边是否有空间*/
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == 0)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    }

    /*可以向左移动*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] -= 1;
    }

}



/* lv_block_game_move_style_3_block_right
 * 第三种类型的块向右移动
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
        /*2块在1块的下边*/
        /*3块在2块的左边*/
        /*4块在块1的上边*/
        /*需要判4块右边是否有空间,1块右边是否有空间,2块右边是否有空间*/
        /*先判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        break;
    case STATE_2:
        /*2块在1块的左边*/
        /*3块在2块的上边*/
        /*4块在块1的右边*/
        /*需要判断4块右边是否有空间,3块右边是否有空间*/
        /*先判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能右移了*/
        {
            return;
        }
        break;
    case STATE_3:
        /*2块在1块的上边*/
        /*3块在2块的右边*/
        /*4块在块1的下边*/
        /*需要判断3块右边是否可以移动,1块右边是否可以移动,4块右边是否可以移动*/

        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    case STATE_4:
        /*2块在1块的右边*/
        /*3块在2块的下边*/
        /*4块在块1的左边*/
        /*需要判断2块右边是否有空间,3块右边是否有空间*/
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(x_index == max_x)/*不能移动*/
        {
            return;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*已经被填充不能左移了*/
        {
            return;
        }
        break;
    }

    /*可以向左移动*/
    for(i = 0; i < 4; i++)
    {
        block->x[i] += 1;
    }

}

/* lv_block_game_move_style_3_block_down
 * 第三种类型的块向下移动
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
        /*2块在1块的下边*/
        /*3块在2块的左边*/
        /*4块在块1的上边*/
        /*需要判2块下边是否有空间,3块下边是否有空间*/
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(y_index == max_y)/*不能移动*/
        {
            /*y最大表示当前块碰壁了*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_2:
        /*2块在1块的左边*/
        /*3块在2块的上边*/
        /*4块在块1的右边*/
        /*需要判4块下边是否有空间,1块下边是否有空间,2块下边是否有空间*/
        /*先判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(y_index == max_y)/*不能移动*/
        {
            /*y最大表示当前块碰壁了*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断2块*/
        x_index = block->x[1];
        y_index = block->y[1];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_3:
        /*2块在1块的上边*/
        /*3块在2块的右边*/
        /*4块在块1的下边*/
        /*需要判4块下边是否有空间,3块下边是否有空间*/
        /*先判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(y_index == max_y)/*不能移动*/
        {
            /*y最大表示当前块碰壁了*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    case STATE_4:
        /*2块在1块的右边*/
        /*3块在2块的下边*/
        /*4块在块1的左边*/
        /*需要判3块下边是否有空间,1块下边是否有空间,4块下边是否有空间*/
        /*先判断3块*/
        x_index = block->x[2];
        y_index = block->y[2];
        if(y_index == max_y)/*不能移动*/
        {
            /*y最大表示当前块碰壁了*/

            return RT_NEED_NEW;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断1块*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        /*再判断4块*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*已经被填充不能下移了*/
        {
            return RT_NEED_NEW;
        }
        break;
    }

    /*可以向下移动*/
    for(i = 0; i < 4; i++)
    {
        block->y[i] += 1;
    }

}

/* lv_block_game_move_block_left
 * 把一个方块向左移动
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
 * 把一个方块向右移动
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
 * 判断当前行是否满了,如果满了向下移动
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
    if(i == max_x + 1)/*满了*/
    {
        //printf("满了\r\n");
        for(i = row; i > 0; i--)
        {
            for(j = 0; j <= max_x; j++)
            {
                g_pt_lv_blockgame->block[j][i].is_fill = g_pt_lv_blockgame->block[j][i-1].is_fill;
                g_pt_lv_blockgame->block[j][i].color = g_pt_lv_blockgame->block[j][i-1].color;
                /*重新改变改行的颜色*/
                lv_obj_set_style_bg_color(g_pt_lv_blockgame->block[j][i].obj, lv_color_hex(g_pt_lv_blockgame->block[j][i].color), 0);
            }
        }
        /*更新第0行数据*/
        for(j = 0; j <= max_x; j++)
        {
            g_pt_lv_blockgame->block[j][0].is_fill = NOT_FILL;
            g_pt_lv_blockgame->block[j][0].color = LV_BLOCK_GAME_BASE_BLOCK_DEFAULT_COLOR;
            /*重新改变改行的颜色*/
            lv_obj_set_style_bg_color(g_pt_lv_blockgame->block[j][0].obj, lv_color_hex(g_pt_lv_blockgame->block[j][0].color), 0);
        }

    }
}

/* lv_block_game_check_row_full
 * 检测一个碰壁的块是否造成了一行满
 */
static void lv_block_game_check_row_full(Block_t *block)
{
    uint8_t max_y;
    uint8_t max_x;
    max_y = LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    max_x = LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE - 1;
    uint8_t top_y;
    int i,j;

    /*先从上面遍历找到最开始填充的高度*/
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
    /*找到最低的碰撞点, 然后一直网上遍历*/
    for(i = max_y; i >= top_y; i--)
    {
        lv_block_game_check_row_and_clear(i);
    }


}



/* lv_block_game_move_block_down
 * 把一个方块向下移动
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
        /*更新背景的填充状态 和颜色*/
        for(i = 0; i < 4; i++)
        {
            x_index = block->x[i];
            y_index = block->y[i];
            g_pt_lv_blockgame->block[x_index][y_index].is_fill = IS_FILL;
            g_pt_lv_blockgame->block[x_index][y_index].color = block->color;
            //printf("(%d,%d)\r\n", x_index, y_index);
        }
        /*检测是否有一行满了*/
        lv_block_game_check_row_full(block);

        /*复位current块的状态*/
        lv_block_game_reset_current_block();
        /*创建一个新的块*/
        lv_block_game_new_the_block();
        /*判断创建的是否产生了碰撞*/
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

            /*重新开始游戏*/
            lv_block_game_restart();


        }



    }
    #endif

    lv_block_game_update_current_block();

}

/* lv_block_game_align
 * 对齐两个块的坐标
 */
static void lv_block_game_align(uint8_t index, uint8_t index_to, int *x, int *y, uint8_t align_type)
{
    switch(align_type)
    {
    case BL_ALIGN_TOP:      /*上对齐*/
        x[index - 1] = x[index_to - 1];
        y[index - 1] = y[index_to - 1] - 1;
        break;
     case BL_ALIGN_BOTTOM:  /*下对齐*/
        x[index - 1] = x[index_to - 1];
        y[index - 1] = y[index_to - 1] + 1;
        break;
    case BL_ALIGN_LEFT:     /*左对齐*/
        x[index - 1] = x[index_to - 1] - 1;
        y[index - 1] = y[index_to - 1];
        break;
    case BL_ALIGN_RIGHT:    /*右对齐*/
        x[index - 1] = x[index_to - 1] + 1;
        y[index - 1] = y[index_to - 1];
        break;

    }

}

/* lv_block_game_set_style_1_state
 * 设置块1的状态
 */
static void lv_block_game_set_style_1_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2块在1块的右边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3块在1块的下边*/
        lv_block_game_align(3, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4块在块的左边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
        break;
    case STATE_2:
        /*2块在1块的下边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3块在1块的左边*/
        lv_block_game_align(3, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*4块在块的上边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        break;
    case STATE_3:
        /*2块在1块的左边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*3块在1块的上边*/
        lv_block_game_align(3, 1, block->x, block->y, BL_ALIGN_TOP);
        /*4块在块的右边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_RIGHT);
        break;
    case STATE_4:
        /*2块在1块的上边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_TOP);
        /*3块在1块的右边*/
        lv_block_game_align(3, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*4块在块的下边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * 创建第1种块
 */
static void lv_block_game_create_style_1_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_1;/*当前块的类型*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_1_state(block);
}

/* lv_block_game_switch_style_1_state
 * 切换第1种块的状态
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
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1状态切换到2状态需要判断右边是否有空间*/
        /*需要判断1块上边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        lv_block_game_set_style_1_state(block);
        break;
    case STATE_2:
        /*2状态切换到3状态需要判断右边是否有空间*/
        /*4块的x坐标+1*/
        /*判断1块右边是否有1个空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == max_x)/*右边没空间*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_3;
        lv_block_game_set_style_1_state(block);
        break;
    case STATE_3:
        /*3状态切换到4状态需要判断1块下边是否有空间*/
        /*4块的y坐标+1*/
        /*判断1块下边是否有1个空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)/*下边没空间*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        block->state = STATE_4;
        lv_block_game_set_style_1_state(block);
        break;
    case STATE_4:
        /*4状态切换到1状态需要判断左边是否有空间*/
        /*4块的x坐标-1*/
        /*判断1块左边是否有1个空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == 0)/*左边没空间*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空见*/
        {
            break;
        }
        block->state = STATE_1;
        lv_block_game_set_style_1_state(block);
        break;
    }
}

/* lv_block_game_set_style_2_state
 * 设置块2的状态
 */
static void lv_block_game_set_style_2_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2块在1块的上边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_TOP);
        /*4块在块1的下边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3块在4块的左边*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_LEFT);
        break;
    case STATE_2:
        /*2块在1块的右边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*4块在块1的左边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*3块在4块的下边*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    case STATE_3:
        /*2块在1块的下边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4块在块1的上边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        /*3块在4块的左边*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_LEFT);
        break;
    case STATE_4:
        /*2块在1块的左边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*4块在块1的右边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3块在4块的上边*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_TOP);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * 创建第1种块
 */
static void lv_block_game_create_style_2_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_2;/*当前块的类型*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_2_state(block);
}

/* lv_block_game_switch_style_2_state
 * 切换第2种块的状态
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
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1状态切换到2状态需要判断右边是否有空间*/
        /*需要判断1块左边是否有一个空间 4块左边是否有1个空间 1块右边是否有1个空间*/
        /*判断1块右边是否有1个空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块左边是否有空间*/
        if(x_index == 0)/*没空间*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断4块左边是否空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        lv_block_game_set_style_2_state(block);
        break;
    case STATE_2:
        /*2状态切换到3状态需要判断右边是否有空间*/
        /*需要判断1块下边是否有空间,4块上边是否有空间,1块上边是否有空间*/
        /*判断1块下边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块上边是否有空间*/
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断4块上边是否有空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        block->state = STATE_3;
        lv_block_game_set_style_2_state(block);
        break;
    case STATE_3:
        /*3状态切换到4状态需要判断1块下边是否有空间*/
        /*需要判断1块左边是否有空间,1块右边是否有空间,4块右边是否有空间*/
        /*判断1块左边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块右边是否有空间*/
        if(x_index == max_x)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断4块右边是否有空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_4;
        lv_block_game_set_style_2_state(block);
        break;
    case STATE_4:
        /*4状态切换到1状态需要判断左边是否有空间*/
        /*需要判断1块上边和下边是否有空间,4块下边是否有空间*/
        /*判断1块下边*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)/*下边没空间*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块上边是否有空间*/
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断4块下边是否有空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_1;
        lv_block_game_set_style_2_state(block);
        break;
    }
}


/* lv_block_game_set_style_3_state
 * 设置块3的状态
 */
static void lv_block_game_set_style_3_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2块在1块的下边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3块在2块的左边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_LEFT);
        /*4块在块1的上边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        break;
    case STATE_2:
        /*2块在1块的左边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*3块在2块的上边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_TOP);
        /*4块在块1的右边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_RIGHT);
        break;
    case STATE_3:
        /*2块在1块的上边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_TOP);
        /*3块在2块的右边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_RIGHT);
        /*4块在块1的下边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    case STATE_4:
        /*2块在1块的右边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3块在2块的下边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4块在块1的左边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * 创建第1种块
 */
static void lv_block_game_create_style_3_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_3;/*当前块的类型*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_3_state(block);
}

/* lv_block_game_switch_style_3_state
 * 切换第3种块的状态
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
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1状态切换到2状态需要判断右边是否有空间*/
        /*需要判断1块右边是否有一个空间 4块左边是否有1个空间 1块左边是否有1个空间*/
        /*判断1块右边是否有1个空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == max_x)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块左边是否有空间*/
        if(x_index == 0)/*没空间*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断4块左边是否空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        break;
    case STATE_2:
        /*2状态切换到3状态需要判断右边是否有空间*/
        /*需要判断1块下边是否有空间,4块上边是否有空间,1块上边是否有空间*/
        /*判断1块下边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块上边是否有空间*/
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断4块上边是否有空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        block->state = STATE_3;
        break;
    case STATE_3:
        /*3状态切换到4状态需要判断1块下边是否有空间*/
        /*需要判断1块左边是否有空间,1块右边是否有空间,4块右边是否有空间*/
        /*判断1块左边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == 0)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块右边是否有空间*/
        if(x_index == max_x)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断4块右边是否有空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_4;
        break;
    case STATE_4:
        /*4状态切换到1状态需要判断左边是否有空间*/
        /*需要判断1块上边和下边是否有空间,4块下边是否有空间*/
        /*判断1块下边*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)/*下边没空间*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块上边是否有空间*/
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断4块下边是否有空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_1;
        break;
    }
    lv_block_game_set_style_3_state(block);
}

/* lv_block_game_set_style_4_state
 * 设置块4的状态
 */
static void lv_block_game_set_style_4_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2块在1块的右边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*4块在块1的上边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        /*3块在4块的左边*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_LEFT);
        break;
    case STATE_2:
        /*2块在1块的下边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4块在块1的右边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3块在4块的上边*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_TOP);
        break;
    case STATE_3:
        /*2块在1块的左边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*4块在块1的下边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3块在4块的右边*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_RIGHT);
        break;
    case STATE_4:
        /*2块在1块的上边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_TOP);
        /*4块在块1的左边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*3块在4块的下边*/
        lv_block_game_align(3, 4, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    }
}

/* lv_block_game_create_style_4_block
 * 创建第四种块
 */
static void lv_block_game_create_style_4_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_4;/*当前块的类型*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_4_state(block);
}

/* lv_block_game_switch_style_4_state
 * 切换第4种块的状态
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
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1状态切换到2状态需要判断右边是否有空间*/
        /*需要判断1块下边是否有一个空间 4块右边是否有1个空间*/
        /*判断1块下边是否有1个空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断4块右边是否空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        break;
    case STATE_2:
        /*2状态切换到3状态需要判断右边是否有空间*/
        /*需要判断1块左边是否有空间,4块下边是否有空间*/
        /*判断1块左边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == 0)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        /*判断4块下边是否有空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        block->state = STATE_3;
        break;
    case STATE_3:
        /*3状态切换到4状态需要判断1块下边是否有空间*/
        /*需要判断1块上边是否有空间,4块左边是否有空间*/
        /*判断1块上边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        /*判断4块左边是否有空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_4;
        break;
    case STATE_4:
        /*4状态切换到1状态需要判断左边是否有空间*/
        /*需要判断1块右边是否有空间,4块上边是否有空间*/
        /*判断1块右边*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == max_x)/*右边没空间*/
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断4块上边是否有空间*/
        x_index = block->x[3];
        y_index = block->y[3];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_1;

        break;
    }
    lv_block_game_set_style_4_state(block);
}


/* lv_block_game_set_style_5_state
 * 设置块5的状态
 */
static void lv_block_game_set_style_5_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)
    {
    case STATE_1:
        /*2块在1块的上边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_TOP);
        /*3块在2块的右边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_RIGHT);
        /*4块在块1的左边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
        break;
    case STATE_2:
        /*2块在1块的右边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3块在2块的下边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4块在块1的上边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        break;
    case STATE_3:
        /*2块在1块的下边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3块在2块的左边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_LEFT);
        /*4块在块1的右边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_RIGHT);
        break;
    case STATE_4:
        /*2块在1块的左边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_LEFT);
        /*3块在2块的上边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_TOP);
        /*4块在块1的下边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    }
}

/* lv_block_game_create_style_5_block
 * 创建第五种块
 */
static void lv_block_game_create_style_5_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_5;/*当前块的类型*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_5_state(block);
}

/* lv_block_game_switch_style_5_state
 * 切换第5种块的状态
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
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1状态切换到2状态需要判断右边是否有空间*/
        /*需要判断1块右边边是否有一个空间 1块右边的下边是否有1个空间*/
        /*判断1块右边是否有1个空间*/
        x_index = block->x[0];
        y_index = block->y[0];

        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块右边的下边是否空间*/
        x_index = block->x[0] + 1;
        y_index = block->y[0];
        if(y_index == max_y)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        break;
    case STATE_2:
        /*2状态切换到3状态需要判断右边是否有空间*/
        /*需要判断1块下边是否有空间,1块下边的左边是否有空间*/
        /*判断1块下边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        /*判断1块下边的左边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0] + 1;
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        block->state = STATE_3;
        break;
    case STATE_3:
        /*3状态切换到4状态需要判断1块下边是否有空间*/
        /*需要判断1块左边是否有空间,1块左边的上边是否有空间*/
        /*判断1块左边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        /*判断1块左边的上边是否有空间*/
        x_index = block->x[0] - 1;
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_4;
        break;
    case STATE_4:
        /*4状态切换到1状态需要判断左边是否有空间*/
        /*需要判断1块上边是否有空间,1块上边的右边是否有空间*/
        /*判断1块上边*/
        x_index = block->x[0];
        y_index = block->y[0];

        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块上边的右边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0] - 1;
        if(x_index == max_x)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_1;

        break;
    }
    lv_block_game_set_style_5_state(block);
}


/* lv_block_game_set_style_6_state
 * 设置块6的状态
 */
static void lv_block_game_set_style_6_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)/*只有一种状态*/
    {
    case STATE_1:
    case STATE_2:
    case STATE_3:
    case STATE_4:
        /*2块在1块的右边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3块在2块的下边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4块在块1的下边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        break;
    }
}

/* lv_block_game_create_style_6_block
 * 创建第6种块
 */
static void lv_block_game_create_style_6_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_6;/*当前块的类型*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_6_state(block);
}

/* lv_block_game_switch_style_6_state
 * 切换第6种块的状态
 */
static void lv_block_game_switch_style_6_state(Block_t *block)
{
    /*1装状态不需要切换*/
    return;
}


/* lv_block_game_set_style_7_state
 * 设置块7的状态
 */
static void lv_block_game_set_style_7_state(Block_t *block)
{
    uint8_t state;
    state = block->state;
    switch(state)/*只有两种种状态*/
    {
    case STATE_1:
        /*2块在1块的右边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3块在2块的右边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_RIGHT);
        /*4块在块1的左边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
    case STATE_2:
        /*2块在1块的下边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3块在2块的下边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4块在块1的上边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
    case STATE_3:
        /*2块在1块的右边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_RIGHT);
        /*3块在2块的右边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_RIGHT);
        /*4块在块1的左边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_LEFT);
    case STATE_4:
        /*2块在1块的下边*/
        lv_block_game_align(2, 1, block->x, block->y, BL_ALIGN_BOTTOM);
        /*3块在2块的下边*/
        lv_block_game_align(3, 2, block->x, block->y, BL_ALIGN_BOTTOM);
        /*4块在块1的上边*/
        lv_block_game_align(4, 1, block->x, block->y, BL_ALIGN_TOP);
        break;
    }
}

/* lv_block_game_create_style_7_block
 * 创建第7种块
 */
static void lv_block_game_create_style_7_block(Block_t *block, uint32_t color)
{

    block->type = STYLE_7;/*当前块的类型*/
    block->state = STATE_1;
    block->color = color;
    block->x[0] = LV_BLOCK_GAME_START_X_INDEX;
    block->y[0] = LV_BLOCK_GAME_START_Y_INDEX;
    lv_block_game_set_style_7_state(block);
}

/* lv_block_game_switch_style_7_state
 * 切换第7种块的状态
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
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        /*1状态切换到2状态需要判断右边是否有空间*/
        /*需要判断1块上边是否有一个空间 1块下边是否有1个空间,1块下边的下边是否右空间*/
        /*判断1块上边是否有1个空间*/
        x_index = block->x[0];
        y_index = block->y[0];

        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块下边的下边有是否空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)/*1块已经是最底了*/
        {
            break;
        }
        y_index += 1;/*1块的下一块*/
        if(y_index == max_y)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块下边有没有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        break;
    case STATE_2:
        /*2状态切换到3状态需要判断右边是否有空间*/
        /*需要判断1块左边是否有空间,1块的右边是否右空间,1块右边的右边是否有空间*/
        /*判断1块左边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == 0)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        /*判断1块右边的右边是否有空间*/
        if(x_index == max_x)/*1块就再最右边了*/
        {
            break;
        }
        x_index += 1;
        if(x_index == max_x)/*1块的右边在最右边了*/
        {
            break;
        }
        /*判断1块右边的右边是否有空间*/
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块的右边是否有空间*/
        if(g_pt_lv_blockgame->block[x_index][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        block->state = STATE_3;
        break;
    case STATE_3:
        /*3状态切换到4状态需要判断1块下边是否有空间*/
        /*需要判断1块上边是否有一个空间 1块下边是否有1个空间,1块下边的下边是否右空间*/
        /*判断1块上边是否有1个空间*/
        x_index = block->x[0];
        y_index = block->y[0];

        if(g_pt_lv_blockgame->block[x_index][y_index - 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块下边的下边有是否空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(y_index == max_y)/*1块已经是最底了*/
        {
            break;
        }
        y_index += 1;/*1块的下一块*/
        if(y_index == max_y)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块下边有没有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(g_pt_lv_blockgame->block[x_index][y_index + 1].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_4;
        break;
    case STATE_4:
        /*4状态切换到1状态需要判断左边是否有空间*/
        /*需要判断1块左边是否有空间,1块的右边是否右空间,1块右边的右边是否有空间*/
        /*判断1块左边是否有空间*/
        x_index = block->x[0];
        y_index = block->y[0];
        if(x_index == 0)
        {
            break;
        }
        if(g_pt_lv_blockgame->block[x_index - 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        /*判断1块右边的右边是否有空间*/
        if(x_index == max_x)/*1块就再最右边了*/
        {
            break;
        }
        x_index += 1;
        if(x_index == max_x)/*1块的右边在最右边了*/
        {
            break;
        }
        /*判断1块右边的右边是否有空间*/
        if(g_pt_lv_blockgame->block[x_index + 1][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }
        /*判断1块的右边是否有空间*/
        if(g_pt_lv_blockgame->block[x_index][y_index].is_fill == IS_FILL)/*没空间*/
        {
            break;
        }

        block->state = STATE_1;

        break;
    }
    lv_block_game_set_style_7_state(block);
}

/* event_handler_back_to_home
 * 返回按键点击事件的回调函数
 * */
static void event_handler_back_to_home(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
	    if(g_pt_lv_blockgame->update_timer != NULL) lv_timer_del(g_pt_lv_blockgame->update_timer);
		if(g_pt_lv_blockgame->bg_about != NULL) lv_obj_del(g_pt_lv_blockgame->bg_about);

		vPortFree(g_pt_lv_blockgame);

		lv_main_page_demo_home(100);
	}
}


/* event_handler_left_move
 * 返回按键点击事件的回调函数
 * */
static void event_handler_left_move(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
        /*左移*/
        lv_block_game_move_block_left(&current_block);
	}
}

/* event_handler_right_move
 * 返回按键点击事件的回调函数
 * */
static void event_handler_right_move(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
        /*右移*/
        lv_block_game_move_block_right(&current_block);
	}
}

/* event_handler_right_move
 * 返回按键点击事件的回调函数
 * */
static void event_handler_switch_state(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
        /*切换状态*/
        lv_block_game_switch_block_state(&current_block);
	}
}

/* lvgl
 * */
#include "lvgl/lvgl.h"
#include "lv_typedef.h"
#include "lv_main_page.h"
#include "lv_block_game.h"
#include "lv_dev_info_page.h"

static PT_lv_blockgame g_pt_lv_blockgame;  // 数据结构体

static lv_style_t style_baseblock;/*容器样式*/

static uint8_t block_array[LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE][LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE]={0};


T_lv_block block_1;
T_lv_block block_2;
T_lv_block block_3;
T_lv_block block_4;
T_lv_block block_5;
T_lv_block block_6;
T_lv_block block_7;
static void lv_block_game_init(void);
static void event_handler_back_to_home(lv_event_t *e);

static void lv_test_block_timer(lv_timer_t * timer);


static void lv_block_game_create_border(void);

static void lv_block_game_create_baseblock(T_lv_baseblock *baseblock);

static void lv_block_game_style_1_block_set_state(T_lv_block *block);
static void lv_block_game_create_style_1_block(T_lv_block *block);
static void lv_block_game_switch_style_1_block_state(T_lv_block *block);

static void lv_block_game_style_2_block_set_state(T_lv_block *block);
static void lv_block_game_create_style_2_block(T_lv_block *block);
static void lv_block_game_switch_style_2_block_state(T_lv_block *block);

static void lv_block_game_style_3_block_set_state(T_lv_block *block);
static void lv_block_game_create_style_3_block(T_lv_block *block);
static void lv_block_game_switch_style_3_block_state(T_lv_block *block);

static void lv_block_game_style_4_block_set_state(T_lv_block *block);
static void lv_block_game_create_style_4_block(T_lv_block *block);
static void lv_block_game_switch_style_4_block_state(T_lv_block *block);

static void lv_block_game_style_5_block_set_state(T_lv_block *block);
static void lv_block_game_create_style_5_block(T_lv_block *block);
static void lv_block_game_switch_style_5_block_state(T_lv_block *block);

static void lv_block_game_style_6_block_set_state(T_lv_block *block);
static void lv_block_game_create_style_6_block(T_lv_block *block);
static void lv_block_game_switch_style_6_block_state(T_lv_block *block);

static void lv_block_game_style_7_block_set_state(T_lv_block *block);
static void lv_block_game_create_style_7_block(T_lv_block *block);
static void lv_block_game_switch_style_7_block_state(T_lv_block *block);

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
    lv_style_set_border_width(&style_cont, 1);


	lv_style_init(&style_baseblock);
	lv_style_set_bg_opa(&style_baseblock, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_baseblock, 10);	/*设置圆角*/
	lv_style_set_bg_color(&style_baseblock, lv_color_hex(RED_HEX));/*设置背景颜色白色*/
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
    lv_obj_set_size(cont, LV_BLOCK_GAME_VALID_CONT_W, LV_BLOCK_GAME_VALID_CONT_H);
    lv_obj_align(cont, LV_ALIGN_TOP_LEFT, 4, 40);
    lv_obj_set_style_radius(cont, 0, 0);


    lv_block_game_create_border();

    /*创建第一个类型的块*/

    lv_block_game_create_style_1_block(&block_1);
    lv_block_game_create_style_2_block(&block_2);
    lv_block_game_create_style_3_block(&block_3);
    lv_block_game_create_style_4_block(&block_4);
    lv_block_game_create_style_5_block(&block_5);
    lv_block_game_create_style_6_block(&block_6);
    lv_block_game_create_style_7_block(&block_7);
    lv_obj_align(block_2.baseblock.block_1, LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_align(block_3.baseblock.block_1, LV_ALIGN_TOP_RIGHT, -20, 20);
    lv_obj_align(block_4.baseblock.block_1, LV_ALIGN_TOP_MID, 0, 120);
    lv_obj_align(block_5.baseblock.block_1, LV_ALIGN_TOP_LEFT, 20, 120);
    lv_obj_align(block_6.baseblock.block_1, LV_ALIGN_TOP_RIGHT, -20, 120);
    lv_obj_align(block_7.baseblock.block_1, LV_ALIGN_TOP_MID, 0, 240);

    /*创建一个定时器*/
    lv_timer_t *timer = lv_timer_create(lv_test_block_timer, 300, NULL);

}

static void lv_test_block_timer(lv_timer_t * timer)
{
    lv_block_game_switch_style_1_block_state(&block_1);
    lv_block_game_switch_style_2_block_state(&block_2);
    lv_block_game_switch_style_3_block_state(&block_3);
    lv_block_game_switch_style_4_block_state(&block_4);
    lv_block_game_switch_style_5_block_state(&block_5);
    lv_block_game_switch_style_6_block_state(&block_6);
    lv_block_game_switch_style_7_block_state(&block_7);
}


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


/* event_handler_back_to_home
 * 返回按键点击事件的回调函数
 * */
static void event_handler_back_to_home(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED)/*点击*/
	{
		if(g_pt_lv_blockgame->bg_about != NULL) lv_obj_del(g_pt_lv_blockgame->bg_about);

		vPortFree(g_pt_lv_blockgame);

		lv_main_page_demo_home(100);
	}
}


/* lv_block_game_create_baseblock
 * 创建一个4个方块
 * baseblock,要创建方块的地址
 */
static void lv_block_game_create_baseblock(T_lv_baseblock *baseblock)
{
    lv_obj_t *block;
    baseblock->block_1 = lv_obj_create(g_pt_lv_blockgame->game_value_cont);
    block = baseblock->block_1;
    lv_obj_add_style(block, &style_baseblock, 0);
    lv_obj_set_size(block, LV_BLOCK_GAME_BASE_CONT_SIZE, LV_BLOCK_GAME_BASE_CONT_SIZE);
    lv_obj_align(block, LV_ALIGN_TOP_MID, 0, 20);
    baseblock->block_2 = lv_obj_create(g_pt_lv_blockgame->game_value_cont);
    block = baseblock->block_2;
    lv_obj_add_style(block, &style_baseblock, 0);
    lv_obj_set_size(block, LV_BLOCK_GAME_BASE_CONT_SIZE, LV_BLOCK_GAME_BASE_CONT_SIZE);
    lv_obj_align(block, LV_ALIGN_TOP_MID, 0, 0);
    baseblock->block_3 = lv_obj_create(g_pt_lv_blockgame->game_value_cont);
    block = baseblock->block_3;
    lv_obj_add_style(block, &style_baseblock, 0);
    lv_obj_set_size(block, LV_BLOCK_GAME_BASE_CONT_SIZE, LV_BLOCK_GAME_BASE_CONT_SIZE);
    lv_obj_align(block, LV_ALIGN_TOP_MID, 0, 0);
    baseblock->block_4 = lv_obj_create(g_pt_lv_blockgame->game_value_cont);
    block = baseblock->block_4;
    lv_obj_add_style(block, &style_baseblock, 0);
    lv_obj_set_size(block, LV_BLOCK_GAME_BASE_CONT_SIZE, LV_BLOCK_GAME_BASE_CONT_SIZE);
    lv_obj_align(block, LV_ALIGN_TOP_MID, 0, 0);

}

/* lv_block_game_style_1_block_set_state
 * 设置第1种块的旋转状态
 */
static void lv_block_game_style_1_block_set_state(T_lv_block *block)
{
    lv_obj_t *cont1, *cont2, *cont3, *cont4;
    cont1 = block->baseblock.block_1;
    cont2 = block->baseblock.block_2;
    cont3 = block->baseblock.block_3;
    cont4 = block->baseblock.block_4;
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont3, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        break;
    case STATE_2:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont3, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        break;
    case STATE_3:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont3, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        break;
    case STATE_4:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont3, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * 创建第1种类型的方块
 *  ***
 *   *
 */
static void lv_block_game_create_style_1_block(T_lv_block *block)
{
    lv_block_game_create_baseblock(&block->baseblock);
    block->type = STYLE_1;
    /*对齐方块*/
    block->state = STATE_1;
    lv_block_game_style_1_block_set_state(block);
}

/* lv_block_game_switch_style_1_block_state
 * 切换第1种类型的旋转状态
 */
static void lv_block_game_switch_style_1_block_state(T_lv_block *block)
{
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        lv_block_game_style_1_block_set_state(block);
        break;
    case STATE_2:
        block->state = STATE_3;
        lv_block_game_style_1_block_set_state(block);
        break;
    case STATE_3:
        block->state = STATE_4;
        lv_block_game_style_1_block_set_state(block);
        break;
    case STATE_4:
        block->state = STATE_1;
        lv_block_game_style_1_block_set_state(block);
        break;
    }
}


/* lv_block_game_style_1_block_set_state
 * 设置第2种块的旋转状态
 */
static void lv_block_game_style_2_block_set_state(T_lv_block *block)
{
    lv_obj_t *cont1, *cont2, *cont3, *cont4;
    cont1 = block->baseblock.block_1;
    cont2 = block->baseblock.block_2;
    cont3 = block->baseblock.block_3;
    cont4 = block->baseblock.block_4;
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont3, cont4, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        break;
    case STATE_2:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont3, cont4, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        break;
    case STATE_3:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont3, cont4, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        break;
    case STATE_4:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont3, cont4, LV_ALIGN_OUT_TOP_MID, 0, 0);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * 创建第2种类型的方块
 *  ***
 *   *
 */
static void lv_block_game_create_style_2_block(T_lv_block *block)
{
    lv_block_game_create_baseblock(&block->baseblock);
    block->type = STYLE_2;
    /*对齐方块*/
    block->state = STATE_1;
    lv_block_game_style_2_block_set_state(block);
}

/* lv_block_game_switch_style_1_block_state
 * 切换第2种类型的旋转状态
 */
static void lv_block_game_switch_style_2_block_state(T_lv_block *block)
{
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        lv_block_game_style_2_block_set_state(block);
        break;
    case STATE_2:
        block->state = STATE_3;
        lv_block_game_style_2_block_set_state(block);
        break;
    case STATE_3:
        block->state = STATE_4;
        lv_block_game_style_2_block_set_state(block);
        break;
    case STATE_4:
        block->state = STATE_1;
        lv_block_game_style_2_block_set_state(block);
        break;
    }
}


/* lv_block_game_style_1_block_set_state
 * 设置第3种块的旋转状态
 */
static void lv_block_game_style_3_block_set_state(T_lv_block *block)
{
    lv_obj_t *cont1, *cont2, *cont3, *cont4;
    cont1 = block->baseblock.block_1;
    cont2 = block->baseblock.block_2;
    cont3 = block->baseblock.block_3;
    cont4 = block->baseblock.block_4;
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        break;
    case STATE_2:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        break;
    case STATE_3:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        break;
    case STATE_4:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * 创建第3种类型的方块
 *  ***
 *   *
 */
static void lv_block_game_create_style_3_block(T_lv_block *block)
{
    lv_block_game_create_baseblock(&block->baseblock);
    block->type = STYLE_3;
    /*对齐方块*/
    block->state = STATE_1;
    lv_block_game_style_3_block_set_state(block);
}

/* lv_block_game_switch_style_1_block_state
 * 切换第3种类型的旋转状态
 */
static void lv_block_game_switch_style_3_block_state(T_lv_block *block)
{
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        lv_block_game_style_3_block_set_state(block);
        break;
    case STATE_2:
        block->state = STATE_3;
        lv_block_game_style_3_block_set_state(block);
        break;
    case STATE_3:
        block->state = STATE_4;
        lv_block_game_style_3_block_set_state(block);
        break;
    case STATE_4:
        block->state = STATE_1;
        lv_block_game_style_3_block_set_state(block);
        break;
    }
}


/* lv_block_game_style_1_block_set_state
 * 设置第4种块的旋转状态
 */
static void lv_block_game_style_4_block_set_state(T_lv_block *block)
{
    lv_obj_t *cont1, *cont2, *cont3, *cont4;
    cont1 = block->baseblock.block_1;
    cont2 = block->baseblock.block_2;
    cont3 = block->baseblock.block_3;
    cont4 = block->baseblock.block_4;
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont3, cont4, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        break;
    case STATE_2:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont3, cont4, LV_ALIGN_OUT_TOP_MID, 0, 0);
        break;
    case STATE_3:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont3, cont4, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        break;
    case STATE_4:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont3, cont4, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * 创建第4种类型的方块
 *  ***
 *   *
 */
static void lv_block_game_create_style_4_block(T_lv_block *block)
{
    lv_block_game_create_baseblock(&block->baseblock);
    block->type = STYLE_4;
    /*对齐方块*/
    block->state = STATE_1;
    lv_block_game_style_4_block_set_state(block);
}

/* lv_block_game_switch_style_1_block_state
 * 切换第4种类型的旋转状态
 */
static void lv_block_game_switch_style_4_block_state(T_lv_block *block)
{
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        lv_block_game_style_4_block_set_state(block);
        break;
    case STATE_2:
        block->state = STATE_3;
        lv_block_game_style_4_block_set_state(block);
        break;
    case STATE_3:
        block->state = STATE_4;
        lv_block_game_style_4_block_set_state(block);
        break;
    case STATE_4:
        block->state = STATE_1;
        lv_block_game_style_4_block_set_state(block);
        break;
    }
}

/* lv_block_game_style_1_block_set_state
 * 设置第5种块的旋转状态
 */
static void lv_block_game_style_5_block_set_state(T_lv_block *block)
{
    lv_obj_t *cont1, *cont2, *cont3, *cont4;
    cont1 = block->baseblock.block_1;
    cont2 = block->baseblock.block_2;
    cont3 = block->baseblock.block_3;
    cont4 = block->baseblock.block_4;
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        break;
    case STATE_2:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        break;
    case STATE_3:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        break;
    case STATE_4:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_TOP_MID, 0, 0);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * 创建第4种类型的方块
 *  ***
 *   *
 */
static void lv_block_game_create_style_5_block(T_lv_block *block)
{
    lv_block_game_create_baseblock(&block->baseblock);
    block->type = STYLE_5;
    /*对齐方块*/
    block->state = STATE_1;
    lv_block_game_style_5_block_set_state(block);
}

/* lv_block_game_switch_style_1_block_state
 * 切换第4种类型的旋转状态
 */
static void lv_block_game_switch_style_5_block_state(T_lv_block *block)
{
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        lv_block_game_style_5_block_set_state(block);
        break;
    case STATE_2:
        block->state = STATE_3;
        lv_block_game_style_5_block_set_state(block);
        break;
    case STATE_3:
        block->state = STATE_4;
        lv_block_game_style_5_block_set_state(block);
        break;
    case STATE_4:
        block->state = STATE_1;
        lv_block_game_style_5_block_set_state(block);
        break;
    }
}

/* lv_block_game_style_1_block_set_state
 * 设置第6种块的旋转状态
 */
static void lv_block_game_style_6_block_set_state(T_lv_block *block)
{
    lv_obj_t *cont1, *cont2, *cont3, *cont4;
    cont1 = block->baseblock.block_1;
    cont2 = block->baseblock.block_2;
    cont3 = block->baseblock.block_3;
    cont4 = block->baseblock.block_4;
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        break;
    case STATE_2:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        break;
    case STATE_3:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        break;
    case STATE_4:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * 创建第4种类型的方块
 *  ***
 *   *
 */
static void lv_block_game_create_style_6_block(T_lv_block *block)
{
    lv_block_game_create_baseblock(&block->baseblock);
    block->type = STYLE_6;
    /*对齐方块*/
    block->state = STATE_1;
    lv_block_game_style_6_block_set_state(block);
}

/* lv_block_game_switch_style_1_block_state
 * 切换第4种类型的旋转状态
 */
static void lv_block_game_switch_style_6_block_state(T_lv_block *block)
{
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        lv_block_game_style_6_block_set_state(block);
        break;
    case STATE_2:
        block->state = STATE_3;
        lv_block_game_style_6_block_set_state(block);
        break;
    case STATE_3:
        block->state = STATE_4;
        lv_block_game_style_6_block_set_state(block);
        break;
    case STATE_4:
        block->state = STATE_1;
        lv_block_game_style_6_block_set_state(block);
        break;
    }
}

/* lv_block_game_style_1_block_set_state
 * 设置第7种块的旋转状态
 */
static void lv_block_game_style_7_block_set_state(T_lv_block *block)
{
    lv_obj_t *cont1, *cont2, *cont3, *cont4;
    cont1 = block->baseblock.block_1;
    cont2 = block->baseblock.block_2;
    cont3 = block->baseblock.block_3;
    cont4 = block->baseblock.block_4;
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        break;
    case STATE_2:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        break;
    case STATE_3:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        break;
    case STATE_4:
        lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_TOP_MID, 0, 0);
        lv_obj_align_to(cont4, cont1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        break;
    }
}

/* lv_block_game_create_style_1_block
 * 创建第4种类型的方块
 *  ***
 *   *
 */
static void lv_block_game_create_style_7_block(T_lv_block *block)
{
    lv_block_game_create_baseblock(&block->baseblock);
    block->type = STYLE_7;
    /*对齐方块*/
    block->state = STATE_1;
    lv_block_game_style_7_block_set_state(block);
}

/* lv_block_game_switch_style_1_block_state
 * 切换第4种类型的旋转状态
 */
static void lv_block_game_switch_style_7_block_state(T_lv_block *block)
{
    uint8_t state;
    state = block->state;
    /*根据状态顺时针旋转*/
    switch(state)
    {
    case STATE_1:
        block->state = STATE_2;
        lv_block_game_style_7_block_set_state(block);
        break;
    case STATE_2:
        block->state = STATE_3;
        lv_block_game_style_7_block_set_state(block);
        break;
    case STATE_3:
        block->state = STATE_4;
        lv_block_game_style_7_block_set_state(block);
        break;
    case STATE_4:
        block->state = STATE_1;
        lv_block_game_style_7_block_set_state(block);
        break;
    }
}

#include "lv_typedef.h"
#include "lvgl/lvgl.h"
#include "UI/lv_main_page.h"
#include "UI/lv_about_page.h"
#include "UI/filesys_page.h"
#include "UI/lv_dev_info_page.h"
#include "game/gomoku.h"
static PT_lv_gomoku g_pt_lv_gomoku;

static void gomoku_page_init(void);
static void gomoku_gameover_page_init(uint8_t winer);
static void gomoku_gameover_page(uint8_t chess_id);
static void event_handler_back_to_home(lv_event_t *e);
static void event_handler_chess_pressed(lv_event_t *e);
static void event_handler_backgame_pressed(lv_event_t *e);
static void event_handler_newgame_pressed(lv_event_t *e);
static void event_handler_playwithplayer_pressed(lv_event_t *e);
static void event_handler_playwithpc_pressed(lv_event_t *e);
/* file_sys_page
 * �ļ�ϵͳ����
 * */
void gomoku_page(void)
{
	g_pt_lv_gomoku = (T_lv_gomoku*)pvPortMalloc(sizeof(T_lv_gomoku));
	g_pt_lv_gomoku->bg_about = lv_obj_create(lv_scr_act());/*��������*/

	lv_obj_set_style_radius(g_pt_lv_gomoku->bg_about, 0, 0);/*����Բ��*/
	lv_obj_align(g_pt_lv_gomoku->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);

	set_user_style(g_pt_lv_gomoku->bg_about);

	lv_obj_set_size(g_pt_lv_gomoku->bg_about, LV_HOR_RES, LV_VER_RES);/*���ô�С*/
	lv_obj_set_style_bg_opa(g_pt_lv_gomoku->bg_about, LV_OPA_100, 0);/*���ñ���͸����*/
	lv_obj_set_style_bg_color(g_pt_lv_gomoku->bg_about, lv_color_white(), 1);
	gomoku_page_init();/*�����ʼ��*/

	add_title(g_pt_lv_gomoku->bg_about, "GOMOKU");
	add_back(g_pt_lv_gomoku->bg_about, event_handler_back_to_home);

}

/* gomoku_game_create_border
 * ��������Ľ�����
 */
static void gomoku_game_create_border(void)
{
    int i;
    static lv_point_t line_points[] = {{0,0}, {300, 0}};
    static lv_point_t line_points_h[] = {{0,0}, {0, 300}};
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 1);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_line_rounded(&style_line, true);

    lv_obj_t *line1;

    #if 1
    for(i = 0; i < 11; i++)
    {
        line1 = lv_line_create(g_pt_lv_gomoku->bg_about);
        lv_line_set_points(line1, line_points, 2);
        lv_obj_add_style(line1, &style_line, 0);
        lv_obj_align(line1, LV_ALIGN_TOP_LEFT, 10, i * 30 + GOMOKU_BOARD_Y_OFFSET);

    }
    #endif
    #if 1
    for(i = 0; i < 11; i++)
    {
        line1 = lv_line_create(g_pt_lv_gomoku->bg_about);
        lv_line_set_points(line1, line_points_h, 2);
        lv_obj_add_style(line1, &style_line, 0);
        lv_obj_align(line1, LV_ALIGN_TOP_LEFT, 10 + i *30, GOMOKU_BOARD_Y_OFFSET);

    }
    #endif

}


/* gomoku_create_checkerboard
 * �������̵�����
 */
static void gomoku_create_checkerboard(void)
{
    /*��������������ʽ*/
    static lv_style_t chess_style;/*������ʽ*/
	lv_style_init(&chess_style);
	lv_style_set_bg_opa(&chess_style, LV_OPA_100);/*���ñ���͸����*/
	lv_style_set_radius(&chess_style, 100);	/*����Բ��*/
	lv_style_set_bg_color(&chess_style, lv_color_hex(WHITE_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&chess_style, 0);
	lv_style_set_pad_bottom(&chess_style, 0);
	lv_style_set_pad_left(&chess_style, 0);
	lv_style_set_pad_right(&chess_style, 0);
    lv_style_set_border_width(&chess_style, 0);
    uint8_t i, j;
    lv_obj_t *chess_obj;
    for(i = 0; i < GOMOKU_CHESS_COUNT; i++){
        for(j = 0; j < GOMOKU_CHESS_COUNT; j++){
            chess_obj = lv_obj_create(g_pt_lv_gomoku->bg_about);
            lv_obj_add_style(chess_obj, &chess_style, 0);
            lv_obj_set_size(chess_obj, 20, 20);
            lv_obj_align(chess_obj, LV_ALIGN_TOP_LEFT, 30 * j, i * 30 + GOMOKU_BOARD_Y_OFFSET - 10);
            g_pt_lv_gomoku->Ch_array[i][j].ch_obj = chess_obj;
            g_pt_lv_gomoku->Ch_array[i][j].ch_status = CH_NONE;
            /*��ӿ��Ե���¼�*/
            lv_obj_add_flag(chess_obj, LV_OBJ_FLAG_CLICKABLE);/*������*/
            lv_obj_add_event_cb(chess_obj, event_handler_chess_pressed, LV_EVENT_CLICKED, (void *)(i * GOMOKU_CHESS_COUNT +j));
        }
    }
    g_pt_lv_gomoku->Ch_player = CH_BLACK;
    g_pt_lv_gomoku->AI_chess = 0;
    g_pt_lv_gomoku->player_chess = 0;
}

/* gomoku_clean_checkerboard
 * ��������
 */
static void gomoku_clean_checkerboard(void)
{
    uint8_t i, j;
    lv_obj_t *chess_obj;
    for(i = 0; i < GOMOKU_CHESS_COUNT; i++){
        for(j = 0; j < GOMOKU_CHESS_COUNT; j++){
            chess_obj = g_pt_lv_gomoku->Ch_array[i][j].ch_obj ;
            g_pt_lv_gomoku->Ch_array[i][j].ch_status = CH_NONE;
            lv_obj_set_style_bg_color(chess_obj, lv_color_hex(WHITE_HEX), 0);
        }
    }
    g_pt_lv_gomoku->Ch_player = CH_BLACK;

    /*��ʼ����ǰ����·��*/
    g_pt_lv_gomoku->AI_chess = 0;
    g_pt_lv_gomoku->player_chess = 0;
}

/* gomoku_check_col
 * �жϵ�ǰ�������Ƿ�����5��
 */
static uint8_t gomoku_check_col(uint8_t chess_color, uint8_t row_id, uint8_t col_id)
{
    uint8_t chess_count = 1;
    uint8_t i, j;
    /*���ж������Ϸ�*/
    i = row_id;
    j = col_id;
    while(1){
        if(i > 0){
            i -= 1;
        }
        else{
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            chess_count++;
        }
        else{
            break;
        }
    }
    /*�ж������·�*/
    i = row_id;
    while(1){
        if(i < (GOMOKU_CHESS_COUNT-1)){
            i += 1;
        }
        else{
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            chess_count++;
        }
        else{
            break;
        }
    }
    return chess_count;

}

/* gomoku_check_row
 * �жϵ�ǰ�������Ƿ�����5��
 */
static uint8_t gomoku_check_row(uint8_t chess_color, uint8_t row_id, uint8_t col_id)
{
    uint8_t chess_count = 1;
    uint8_t i, j;
    /*���ж�������*/
    i = row_id;
    j = col_id;
    while(1){
        if(j > 0){
            j -= 1;
        }
        else{
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            chess_count++;
        }
        else{
            break;
        }
    }
    /*�ж������ҷ�*/
    j = col_id;
    while(1){
        if(j < (GOMOKU_CHESS_COUNT-1)){
            j += 1;
        }
        else{
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            chess_count++;
        }
        else{
            break;
        }
    }
    return chess_count;
}

/* gomoku_check_leftbias
 * �жϵ�ǰ������б���Ƿ�����5��
 */
static uint8_t gomoku_check_leftbias(uint8_t chess_color, uint8_t row_id, uint8_t col_id)
{
    uint8_t chess_count = 1;
    uint8_t i, j;
    /*���ж��������Ϸ�*/
    i = row_id;
    j = col_id;
    while(1){
        if((j > 0) && (i > 0)){
            j -= 1;
            i -= 1;
        }
        else{
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            chess_count++;
        }
        else{
            break;
        }
    }
    /*�ж��������·�*/
    i = row_id;
    j = col_id;
    while(1){
        if((j < (GOMOKU_CHESS_COUNT-1)) && (i < (GOMOKU_CHESS_COUNT-1))){
            j += 1;
            i += 1;
        }
        else{
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            chess_count++;
        }
        else{
            break;
        }
    }
    return chess_count;

}

/* gomoku_check_rightbias
 * �жϵ�ǰ������б���Ƿ�����5��
 */
static uint8_t gomoku_check_rightbias(uint8_t chess_color, uint8_t row_id, uint8_t col_id)
{
    uint8_t chess_count = 1;
    uint8_t i, j;
    /*���ж��������Ϸ�*/
    i = row_id;
    j = col_id;
    while(1){
        if((j < GOMOKU_CHESS_COUNT-1) && (i > 0)){
            j += 1;
            i -= 1;
        }
        else{
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            chess_count++;
        }
        else{
            break;
        }
    }
    /*�ж��������·�*/
    i = row_id;
    j = col_id;
    while(1){
        if((j > 0) && (i < (GOMOKU_CHESS_COUNT-1))){
            j -= 1;
            i += 1;
        }
        else{
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            chess_count++;
        }
        else{
            break;
        }
    }
    return chess_count;
}

/* gomoku_check_Winner
 * �жϵ�ǰ��������,�Ƿ������ʤ����
 */
static uint8_t gomoku_check_Winner(uint8_t chess_id)
{
    uint8_t i, j;
    uint8_t chess_color;
    i = chess_id / GOMOKU_CHESS_COUNT;
    j = chess_id % GOMOKU_CHESS_COUNT;
    chess_color = g_pt_lv_gomoku->Ch_array[i][j].ch_status;
    if(chess_color == CH_NONE){
        return RT_FAIL;
    }
    if(gomoku_check_row(chess_color, i, j) == 5){
        return RT_OK;
    }
    if(gomoku_check_col(chess_color, i, j) == 5){
        return RT_OK;
    }
    if(gomoku_check_leftbias(chess_color, i, j) == 5){
        return RT_OK;
    }
    if(gomoku_check_rightbias(chess_color, i, j) == 5){
        return RT_OK;
    }
    return RT_FAIL;
}

/* AI_GetDecision_col
 * ���㵱ǰ����,�з����������
 */
static void AI_GetDecision_col(uint8_t chess_color, uint8_t row_id, uint8_t col_id, Dir_Chess_t * up_deci, Dir_Chess_t * down_deci)
{
    uint8_t i, j;
    /*���ж������Ϸ�*/
    up_deci->ch_count = 0;
    down_deci->ch_count = 0;
    i = row_id;
    j = col_id;
    while(1){
        if(i > 0){
            i -= 1;
        }
        else{
            up_deci->ch_flag = IS_BUSY;
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            up_deci->ch_count++;
        }
        else{
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
               up_deci->ch_flag = IS_FREE;
            }
            else{
                up_deci->ch_flag = IS_BUSY;
            }
            break;
        }
    }

    /*�ж������·�*/
    i = row_id;
    while(1){
        if(i < (GOMOKU_CHESS_COUNT-1)){
            i += 1;
        }
        else{
            down_deci->ch_flag = IS_BUSY;
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            down_deci->ch_count++;
        }
        else{
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
               down_deci->ch_flag = IS_FREE;
            }
            else{
                down_deci->ch_flag = IS_BUSY;
            }
            break;
        }
    }
}

/* AI_GetDecision_row
 * ���㵱ǰ����,�з����������
 */
static void AI_GetDecision_row(uint8_t chess_color, uint8_t row_id, uint8_t col_id, Dir_Chess_t * left_deci, Dir_Chess_t * right_deci)
{
    uint8_t i, j;
    left_deci->ch_count = 0;
    right_deci->ch_count = 0;
    /*���ж�������*/
    i = row_id;
    j = col_id;
    while(1){
        if(j > 0){
            j -= 1;
        }
        else{
            left_deci->ch_flag = IS_BUSY;
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            left_deci->ch_count++;
        }
        else{
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
               left_deci->ch_flag = IS_FREE;
            }
            else{
                left_deci->ch_flag = IS_BUSY;
            }
            break;
        }
    }
    /*�ж������ҷ�*/
    j = col_id;
    while(1){
        if(j < (GOMOKU_CHESS_COUNT-1)){
            j += 1;
        }
        else{
            right_deci->ch_flag = IS_BUSY;
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            right_deci->ch_count++;
        }
        else{
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
               right_deci->ch_flag = IS_FREE;
            }
            else{
                right_deci->ch_flag = IS_BUSY;
            }
            break;
        }
    }
}

/* AI_GetDecision_leftbias
 * ���㵱ǰ����,��б�߷����������
 */
static void AI_GetDecision_leftbias(uint8_t chess_color, uint8_t row_id, uint8_t col_id, Dir_Chess_t * lu_deci, Dir_Chess_t * rd_deci)
{
    uint8_t i, j;
    lu_deci->ch_count = 0;
    rd_deci->ch_count = 0;
    /*���ж��������Ϸ�*/
    i = row_id;
    j = col_id;
    while(1){
        if((j > 0) && (i > 0)){
            j -= 1;
            i -= 1;
        }
        else{
            lu_deci->ch_flag = IS_BUSY;
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            lu_deci->ch_count++;
        }
        else{
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
               lu_deci->ch_flag = IS_FREE;
            }
            else{
                lu_deci->ch_flag = IS_BUSY;
            }
            break;
        }
    }
    /*�ж��������·�*/
    i = row_id;
    j = col_id;
    while(1){
        if((j < (GOMOKU_CHESS_COUNT-1)) && (i < (GOMOKU_CHESS_COUNT-1))){
            j += 1;
            i += 1;
        }
        else{
            rd_deci->ch_flag = IS_BUSY;
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            rd_deci->ch_count++;
        }
        else{
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
               rd_deci->ch_flag = IS_FREE;
            }
            else{
                rd_deci->ch_flag = IS_BUSY;
            }
            break;
        }
    }


}

/* AI_GetDecision_rightbias
 * ���㵱ǰ����,��б�߷����������
 */
static void AI_GetDecision_rightbias(uint8_t chess_color, uint8_t row_id, uint8_t col_id, Dir_Chess_t * ru_deci, Dir_Chess_t * ld_deci)
{
    uint8_t i, j;
    ru_deci->ch_count = 0;
    ld_deci->ch_count = 0;
    /*���ж��������Ϸ�*/
    i = row_id;
    j = col_id;
    while(1){
        if((j < GOMOKU_CHESS_COUNT-1) && (i > 0)){
            j += 1;
            i -= 1;
        }
        else{
            ru_deci->ch_flag = IS_BUSY;
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            ru_deci->ch_count++;
        }
        else{
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
               ru_deci->ch_flag = IS_FREE;
            }
            else{
                ru_deci->ch_flag = IS_BUSY;
            }
            break;
        }
    }
    /*�ж��������·�*/
    i = row_id;
    j = col_id;
    while(1){
        if((j > 0) && (i < (GOMOKU_CHESS_COUNT-1))){
            j -= 1;
            i += 1;
        }
        else{
            ld_deci->ch_flag = IS_BUSY;
            break;
        }
        if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == chess_color){
            ld_deci->ch_count++;
        }
        else{
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
               ld_deci->ch_flag = IS_FREE;
            }
            else{
                ld_deci->ch_flag = IS_BUSY;
            }
            break;
        }
    }
}

/* AI_Down_Chess
 * AI����һ������
 */
static void AI_Down_Chess(uint8_t row, uint8_t col)
{
    uint8_t chess_id;
    lv_obj_t *obj = g_pt_lv_gomoku->Ch_array[row][col].ch_obj;
    switch(g_pt_lv_gomoku->Ch_player){
    case CH_BLACK:
        lv_obj_set_style_bg_color(obj, lv_color_hex(BLACK_HEX), 0);
        g_pt_lv_gomoku->Ch_array[row][col].ch_status = CH_BLACK;
        g_pt_lv_gomoku->Ch_player = CH_WHITE;
        break;
    case CH_WHITE:
        lv_obj_set_style_bg_color(obj, lv_color_hex(RED_HEX), 0);
        g_pt_lv_gomoku->Ch_array[row][col].ch_status = CH_WHITE;
        g_pt_lv_gomoku->Ch_player = CH_BLACK;
        break;
    }

    printf("row = %d, col = %d\r\n", row, col);
    chess_id = row * GOMOKU_CHESS_COUNT + col;
    g_pt_lv_gomoku->AI_path[g_pt_lv_gomoku->AI_chess] = chess_id;
    g_pt_lv_gomoku->AI_chess ++;

    /*�����Ϸ�Ƿ����*/
    gomoku_gameover_page(chess_id);
}

/* AI_Check_OneSide
 * �ж�4�����򵥱��пյ��������
 */
static uint8_t AI_Check_OneSide(uint8_t row, uint8_t col, uint8_t check_color, AI_Decision_t *decision, uint8_t *chess_count, uint8_t ch_count)
{
    uint8_t i;
    for(i = 0; i < 4; i++){
        if(chess_count[i] == ch_count){
            switch(i){
            case 0:/*�ж���*/
                /*�ж������Ƿ��������*/
                if(decision->ch_up.ch_flag == IS_FREE){
                    /*����,�˳�*/
                    AI_Down_Chess(row - decision->ch_up.ch_count, col);
                    return RT_OK;
                }
                /*�ж������Ƿ��������*/
                if(decision->ch_down.ch_flag == IS_FREE){
                    AI_Down_Chess(row + decision->ch_down.ch_count, col);
                    return RT_OK;
                }
                /*��������*/
                break;
            case 1:/*�ж���*/
                /*�ж������Ƿ��������*/
                if(decision->ch_left.ch_flag == IS_FREE){
                    /*����,�˳�*/
                    AI_Down_Chess(row, col - decision->ch_left.ch_count);
                    return RT_OK;
                }
                /*�ж������Ƿ��������*/
                if(decision->ch_right.ch_flag == IS_FREE){
                    AI_Down_Chess(row, col + decision->ch_right.ch_count);
                    return RT_OK;
                }
                /*��������*/
                break;
            case 2:/*�ж���б��*/
                /*�ж������Ƿ��������*/
                if(decision->ch_lu.ch_flag == IS_FREE){
                    /*����,�˳�*/
                    AI_Down_Chess(row - decision->ch_lu.ch_count, col - decision->ch_lu.ch_count);
                    return RT_OK;
                }
                /*�ж������Ƿ��������*/
                if(decision->ch_rd.ch_flag == IS_FREE){
                    AI_Down_Chess(row + decision->ch_rd.ch_count, col + decision->ch_rd.ch_count);
                    return RT_OK;
                }
                /*��������*/
                break;
            case 3:/*��б��*/
                /*�ж������Ƿ��������*/
                if(decision->ch_ld.ch_flag == IS_FREE){
                    /*����,�˳�*/
                    AI_Down_Chess(row + decision->ch_ld.ch_count, col - decision->ch_ld.ch_count);
                    return RT_OK;
                }
                /*�ж������Ƿ��������*/
                if(decision->ch_ru.ch_flag == IS_FREE){
                    AI_Down_Chess(row - decision->ch_ru.ch_count, col + decision->ch_ru.ch_count);
                    return RT_OK;
                }
                /*��������*/
                break;
            }
        }
    }
    return RT_FAIL;
}

/* AI_Check_doubleSide
 * �ж�4������˫���пյ��������
 */
static uint8_t AI_Check_doubleSide(uint8_t row, uint8_t col, uint8_t check_color, AI_Decision_t *decision, uint8_t *chess_count, uint8_t ch_count)
{
    uint8_t i;

    for(i = 0; i < 4; i++){
        if(chess_count[i] == ch_count){
            switch(i){
               case 0:/*�ж���*/
                /*�ж�����������Ƿ񶼿�������*/
                if((decision->ch_up.ch_flag == IS_FREE) && (decision->ch_down.ch_flag == IS_FREE)){
                    /*����,�˳�*/
                    /*�ж��������һ�������Ƿ�Ϊ�Է�����*/
                    /*�߽��ж�*/
                    if((row - decision->ch_up.ch_count) > 0){
                        if(g_pt_lv_gomoku->Ch_array[row - decision->ch_up.ch_count - 1][col].ch_status == check_color){
                            AI_Down_Chess(row - decision->ch_up.ch_count, col);
                            return RT_OK;
                        }
                    }
                    /*�ж��������һ�������Ƿ�Ϊ�Է�����*/
                    if((row + decision->ch_down.ch_count + 1) < GOMOKU_CHESS_COUNT){
                        if(g_pt_lv_gomoku->Ch_array[row + decision->ch_down.ch_count + 1][col].ch_status == check_color){
                            AI_Down_Chess(row + decision->ch_down.ch_count, col);
                            return RT_OK;
                        }
                    }
                    /*���߶�������,�����ϱ�*/
                    AI_Down_Chess(row - decision->ch_up.ch_count, col);
                    return RT_OK;
                }
                /*��������*/
                break;
               case 1:/*�ж���*/
               /*�ж�����������Ƿ񶼿�������*/
                if((decision->ch_left.ch_flag == IS_FREE) && (decision->ch_right.ch_flag == IS_FREE)){
                    /*����,�˳�*/
                    /*�ж��������һ�������Ƿ�Ϊ�Է�����*/
                    if((col - decision->ch_left.ch_count) > 0){
                        if(g_pt_lv_gomoku->Ch_array[row][col - decision->ch_left.ch_count - 1].ch_status == check_color){
                            AI_Down_Chess(row, col - decision->ch_left.ch_count);
                            return RT_OK;
                        }
                    }
                    /*�ж��������һ�������Ƿ�Ϊ�Է�����*/
                    if((col + decision->ch_right.ch_count + 1) < GOMOKU_CHESS_COUNT){
                        if(g_pt_lv_gomoku->Ch_array[row][col + decision->ch_right.ch_count + 1].ch_status == check_color){
                            AI_Down_Chess(row, col + decision->ch_right.ch_count);
                            return RT_OK;
                        }
                    }
                    /*���߶�������,�������*/
                    AI_Down_Chess(row, col - decision->ch_left.ch_count);
                    return RT_OK;
                }
                /*��������*/
                break;
                case 2:/*�ж���б��*/
                /*�ж����Ϻ������Ƿ��������*/
                if((decision->ch_lu.ch_flag == IS_FREE) && (decision->ch_rd.ch_flag == IS_FREE)){
                    /*����,�˳�*/
                    /*�ж������������һ�������Ƿ�Ϊ�Է�����*/
                    if(((row - decision->ch_lu.ch_count) > 0) && ((col - decision->ch_lu.ch_count) > 0)){
                       if(g_pt_lv_gomoku->Ch_array[row - decision->ch_lu.ch_count - 1][col - decision->ch_lu.ch_count - 1].ch_status == check_color){
                            AI_Down_Chess(row - decision->ch_lu.ch_count, col - decision->ch_lu.ch_count);
                            return RT_OK;
                        }
                    }
                    /*�ж������������һ�������Ƿ�Ϊ�Է�����*/
                    if(((row + decision->ch_rd.ch_count + 1) < GOMOKU_CHESS_COUNT) && ((col + decision->ch_rd.ch_count + 1) < GOMOKU_CHESS_COUNT)){
                        if(g_pt_lv_gomoku->Ch_array[row + decision->ch_rd.ch_count + 1][col + decision->ch_rd.ch_count + 1].ch_status == check_color){
                            AI_Down_Chess(row + decision->ch_rd.ch_count, col + decision->ch_rd.ch_count);
                            return RT_OK;
                        }
                    }
                    /*���߶�������,�������ϱ�*/
                    AI_Down_Chess(row - decision->ch_lu.ch_count, col - decision->ch_lu.ch_count);
                    return RT_OK;
                }
                /*��������*/
                break;
                case 3:/*��б��*/
                /*�ж����º������Ƿ��������*/
                if((decision->ch_ld.ch_flag == IS_FREE) && (decision->ch_ld.ch_flag == IS_FREE)){
                    /*����,�˳�*/
                    /*�ж������������һ�������Ƿ�Ϊ�Է�����*/
                    if(((row + decision->ch_ld.ch_count + 1) < GOMOKU_CHESS_COUNT) && ((col - decision->ch_ld.ch_count) > 0)){
                        if(g_pt_lv_gomoku->Ch_array[row + decision->ch_ld.ch_count + 1][col - decision->ch_ld.ch_count - 1].ch_status == check_color){
                            AI_Down_Chess(row + decision->ch_ld.ch_count, col - decision->ch_ld.ch_count);
                            return RT_OK;
                        }
                    }

                    /*�ж������������һ�������Ƿ�Ϊ�Է�����*/
                    if(((row - decision->ch_ru.ch_count) > 0) && ((col + decision->ch_ru.ch_count + 1) < GOMOKU_CHESS_COUNT)){
                        if(g_pt_lv_gomoku->Ch_array[row - decision->ch_ru.ch_count - 1][col + decision->ch_ru.ch_count + 1].ch_status == check_color){
                            AI_Down_Chess(row - decision->ch_ru.ch_count, col + decision->ch_ru.ch_count);
                            return RT_OK;
                        }
                    }
                    /*���߶�������,�������±�*/
                    AI_Down_Chess(row + decision->ch_ld.ch_count, col - decision->ch_ld.ch_count);
                    return RT_OK;
                }
                /*��������*/
                break;
            }
        }
    }
    return RT_FAIL;
}



/* AI_Down_OneChess
 * AI���ݵ�ǰ�Է�������λ�ã���������
 */
static void AI_Down_OneChess(uint8_t chess_id)
{
    AI_Decision_t decision;
    uint8_t i, j;
    uint8_t nRet;
    uint8_t row, col;
    uint8_t chess_color;
    uint8_t chess_count[4] = {0};/*�����ӵķ���,0:col,1:row,2:leftbias,3:rightbias*/
    uint8_t check_color;
    if(g_pt_lv_gomoku->Ch_player == CH_BLACK){
        check_color = CH_WHITE;
    }
    else{
        check_color = CH_BLACK;
    }
    row = chess_id / GOMOKU_CHESS_COUNT;
    col = chess_id % GOMOKU_CHESS_COUNT;
    chess_color = g_pt_lv_gomoku->Ch_array[row][col].ch_status;

    /*�ж��з���*/
    AI_GetDecision_col(chess_color, row, col, &decision.ch_up, &decision.ch_down);
    AI_GetDecision_row(chess_color, row, col, &decision.ch_left, &decision.ch_right);
    AI_GetDecision_leftbias(chess_color, row, col, &decision.ch_lu, &decision.ch_rd);
    AI_GetDecision_rightbias(chess_color, row, col, &decision.ch_ru, &decision.ch_ld);
    chess_count[0] = decision.ch_up.ch_count + decision.ch_down.ch_count + 1;
    chess_count[1] = decision.ch_left.ch_count + decision.ch_right.ch_count + 1;
    chess_count[2] = decision.ch_lu.ch_count + decision.ch_rd.ch_count + 1;
    chess_count[3] = decision.ch_ld.ch_count + decision.ch_ru.ch_count + 1;

    printf("up count = %d, down count = %d\r\n", decision.ch_up.ch_count, decision.ch_down.ch_count);
    printf("left count = %d, right count = %d\r\n", decision.ch_left.ch_count, decision.ch_right.ch_count);
    printf("left up count = %d, right down count = %d\r\n", decision.ch_lu.ch_count, decision.ch_rd.ch_count);
    printf("left down count = %d, right up count = %d\r\n", decision.ch_ld.ch_count, decision.ch_ru.ch_count);

    decision.ch_up.ch_count += 1;
    decision.ch_down.ch_count += 1;
    decision.ch_left.ch_count += 1;
    decision.ch_right.ch_count += 1;
    decision.ch_lu.ch_count += 1;
    decision.ch_rd.ch_count += 1;
    decision.ch_ld.ch_count += 1;
    decision.ch_ru.ch_count += 1;


    /*�ж�4��*/
    printf("pand 4 oneside\r\n");
    nRet = AI_Check_OneSide(row, col, check_color, &decision, chess_count, 4);
    if(nRet == RT_OK){
        return;
    }
    /*�ж�3��,���߶�Ϊ�յ�*/
    printf("pand 3 doubleside\r\n");
    nRet = AI_Check_doubleSide(row, col, check_color, &decision, chess_count, 3);
    if(nRet == RT_OK){
        return;
    }
    /*�ж�3��,����Ϊ��*/
    printf("pand 3 oneside\r\n");
    nRet = AI_Check_OneSide(row, col, check_color, &decision, chess_count, 3);
    if(nRet == RT_OK){
        return;
    }

    /*�ж�2��,���߶�Ϊ�յ�*/
    printf("pand 2 doubleside\r\n");
    nRet = AI_Check_doubleSide(row, col, check_color, &decision, chess_count, 2);
    if(nRet == RT_OK){
        return;
    }
    /*�ж�2��,����Ϊ��*/
    printf("pand 2 oneside\r\n");
    nRet = AI_Check_OneSide(row, col, check_color, &decision, chess_count, 2);
    if(nRet == RT_OK){
        return;
    }

    /*�ж�1��,���߶�Ϊ�յ�*/
    printf("pand 1 doubleside\r\n");
    nRet = AI_Check_doubleSide(row, col, check_color, &decision, chess_count, 1);
    if(nRet == RT_OK){
        return;
    }
    /*�ж�1��,����Ϊ��*/
    printf("pand 1 oneside\r\n");
    nRet = AI_Check_OneSide(row, col, check_color, &decision, chess_count, 1);
    if(nRet == RT_OK){
        return;
    }

    /*û�ط�����,�Ҹ��ط���һ����*/
    for(i = 0; i < GOMOKU_CHESS_COUNT; i++){
        for(j = 0; j < GOMOKU_CHESS_COUNT; j++){
            if(g_pt_lv_gomoku->Ch_array[row][col].ch_status == CH_NONE){
                AI_Down_Chess(i, j);
                return;
            }
        }
    }
    /*��������*/
    gomoku_gameover_page_init(CH_WHITE);

}

/* AI_Down_CanWin
 * ����Ƿ��п���Ӯ��4�����,��ֱ����Ӯ
 */
static uint8_t AI_Down_CanWin(uint8_t side, uint8_t ch_count)
{
    AI_Decision_t decision;
    uint8_t i;
    uint8_t nRet;
    uint8_t row, col;
    uint8_t chess_color;
    uint8_t chess_count[4] = {0};/*�����ӵķ���,0:col,1:row,2:leftbias,3:rightbias*/
    uint8_t check_color;
    row = g_pt_lv_gomoku->AI_path[0] / GOMOKU_CHESS_COUNT;
    col = g_pt_lv_gomoku->AI_path[0] % GOMOKU_CHESS_COUNT;
    chess_color = g_pt_lv_gomoku->Ch_array[row][col].ch_status;
    /*����AI���ص�ÿһ����*/
    for(i = 0; i < g_pt_lv_gomoku->AI_chess; i++){
        row = g_pt_lv_gomoku->AI_path[i] / GOMOKU_CHESS_COUNT;
        col = g_pt_lv_gomoku->AI_path[i] % GOMOKU_CHESS_COUNT;
        /*�ж��з���*/
        AI_GetDecision_col(chess_color, row, col, &decision.ch_up, &decision.ch_down);
        AI_GetDecision_row(chess_color, row, col, &decision.ch_left, &decision.ch_right);
        AI_GetDecision_leftbias(chess_color, row, col, &decision.ch_lu, &decision.ch_rd);
        AI_GetDecision_rightbias(chess_color, row, col, &decision.ch_ru, &decision.ch_ld);
        chess_count[0] = decision.ch_up.ch_count + decision.ch_down.ch_count + 1;
        chess_count[1] = decision.ch_left.ch_count + decision.ch_right.ch_count + 1;
        chess_count[2] = decision.ch_lu.ch_count + decision.ch_rd.ch_count + 1;
        chess_count[3] = decision.ch_ld.ch_count + decision.ch_ru.ch_count + 1;
        decision.ch_up.ch_count += 1;
        decision.ch_down.ch_count += 1;
        decision.ch_left.ch_count += 1;
        decision.ch_right.ch_count += 1;
        decision.ch_lu.ch_count += 1;
        decision.ch_rd.ch_count += 1;
        decision.ch_ld.ch_count += 1;
        decision.ch_ru.ch_count += 1;
        if(side == ONE_SIDE){
            nRet = AI_Check_OneSide(row, col, chess_color, &decision, chess_count, ch_count);
        }
        else{
            nRet = AI_Check_doubleSide(row, col, CH_NONE, &decision, chess_count, ch_count);
        }

        if(nRet == RT_OK){
            return RT_OK;
        }

    }
    return RT_FAIL;

}

/* AI_Down_CanWin
 * ����Ƿ��п���Ӯ��4�����,��ֱ����Ӯ
 */
static uint8_t AI_Down_block_foe(uint8_t side, uint8_t ch_count)
{
    AI_Decision_t decision;
    uint8_t i;
    uint8_t nRet;
    uint8_t row, col;
    uint8_t chess_color;
    uint8_t chess_count[4] = {0};/*�����ӵķ���,0:col,1:row,2:leftbias,3:rightbias*/
    uint8_t check_color;
    if(g_pt_lv_gomoku->Ch_player == CH_BLACK){
        check_color = CH_WHITE;
    }
    else{
        check_color = CH_BLACK;
    }
    row = g_pt_lv_gomoku->player_path[0] / GOMOKU_CHESS_COUNT;
    col = g_pt_lv_gomoku->player_path[0] % GOMOKU_CHESS_COUNT;
    chess_color = g_pt_lv_gomoku->Ch_array[row][col].ch_status;
    /*����AI���ص�ÿһ����*/
    for(i = 0; i < g_pt_lv_gomoku->player_chess; i++){
        row = g_pt_lv_gomoku->player_path[i] / GOMOKU_CHESS_COUNT;
        col = g_pt_lv_gomoku->player_path[i] % GOMOKU_CHESS_COUNT;
        /*�ж��з���*/
        AI_GetDecision_col(chess_color, row, col, &decision.ch_up, &decision.ch_down);
        AI_GetDecision_row(chess_color, row, col, &decision.ch_left, &decision.ch_right);
        AI_GetDecision_leftbias(chess_color, row, col, &decision.ch_lu, &decision.ch_rd);
        AI_GetDecision_rightbias(chess_color, row, col, &decision.ch_ru, &decision.ch_ld);
        chess_count[0] = decision.ch_up.ch_count + decision.ch_down.ch_count + 1;
        chess_count[1] = decision.ch_left.ch_count + decision.ch_right.ch_count + 1;
        chess_count[2] = decision.ch_lu.ch_count + decision.ch_rd.ch_count + 1;
        chess_count[3] = decision.ch_ld.ch_count + decision.ch_ru.ch_count + 1;
        decision.ch_up.ch_count += 1;
        decision.ch_down.ch_count += 1;
        decision.ch_left.ch_count += 1;
        decision.ch_right.ch_count += 1;
        decision.ch_lu.ch_count += 1;
        decision.ch_rd.ch_count += 1;
        decision.ch_ld.ch_count += 1;
        decision.ch_ru.ch_count += 1;
        if(side == ONE_SIDE){
            nRet = AI_Check_OneSide(row, col, check_color, &decision, chess_count, ch_count);
        }
        else{
            nRet = AI_Check_doubleSide(row, col, check_color, &decision, chess_count, ch_count);
        }

        if(nRet == RT_OK){
            return RT_OK;
        }

    }
    return RT_FAIL;

}

static void AI_Player_DownChess(void)
{
    uint8_t nRet;
    uint8_t i, j;
    /* 1.���ж���һ�����Ƿ����Ӯ
     * ����>=4������һ������Ӯ�����
     */
    if(g_pt_lv_gomoku->AI_chess >= 4){
        nRet = AI_Down_CanWin(ONE_SIDE, 4);
        if(nRet == RT_OK){
            return;
        }
       /* 2.�ж϶Է���һ�����Ƿ����Ӯ,
        * �Ѿ�����4�����
        */
        nRet = AI_Down_block_foe(ONE_SIDE, 4);
        if(nRet == RT_OK){
            return;
        }
    }

    /* 3.�ж��Լ��Ƿ�������3�������߶�Ϊ�յ����
     */
    nRet = AI_Down_CanWin(DOUBLE_SIDE, 3);
    if(nRet == RT_OK){
        return;
    }

    /* 4.�ж϶Է��Ƿ�����3�������߶�Ϊ�յ����
     */
    nRet = AI_Down_block_foe(DOUBLE_SIDE, 3);
    if(nRet == RT_OK){
        return;
    }

    /* 5.�ж��Լ��Ƿ�������3����1�߶�Ϊ�յ����
     */
    nRet = AI_Down_CanWin(ONE_SIDE, 3);
    if(nRet == RT_OK){
        return;
    }

    /* 6.�ж϶Է��Ƿ�����3����1�߶�Ϊ�յ����
     */
    nRet = AI_Down_block_foe(ONE_SIDE, 3);
    if(nRet == RT_OK){
        return;
    }

    /* 7.�ж��Լ��Ƿ�������2�������߶�Ϊ�յ����
     */
    nRet = AI_Down_CanWin(DOUBLE_SIDE, 2);
    if(nRet == RT_OK){
        return;
    }

    /* 8.�ж϶Է��Ƿ�����2�������߶�Ϊ�յ����
     */
    nRet = AI_Down_block_foe(DOUBLE_SIDE, 2);
    if(nRet == RT_OK){
        return;
    }

    /* 9.�ж��Լ��Ƿ�������2����1�߶�Ϊ�յ����
     */
    nRet = AI_Down_CanWin(ONE_SIDE, 2);
    if(nRet == RT_OK){
        return;
    }

    /* 10.�ж϶Է��Ƿ�����2����1�߶�Ϊ�յ����
     */
    nRet = AI_Down_block_foe(ONE_SIDE, 2);
    if(nRet == RT_OK){
        return;
    }

    /* 11.�ж��Լ��Ƿ�������1�������߶�Ϊ�յ����
     */
    nRet = AI_Down_CanWin(DOUBLE_SIDE, 1);
    if(nRet == RT_OK){
        return;
    }

    /* 12.�ж϶Է��Ƿ�����1�������߶�Ϊ�յ����
     */
    nRet = AI_Down_block_foe(DOUBLE_SIDE, 1);
    if(nRet == RT_OK){
        return;
    }

    /* 13.�ж��Լ��Ƿ�������1����1�߶�Ϊ�յ����
     */
    nRet = AI_Down_CanWin(ONE_SIDE, 1);
    if(nRet == RT_OK){
        return;
    }

    /* 14.�ж϶Է��Ƿ�����1����1�߶�Ϊ�յ����
     */
    nRet = AI_Down_block_foe(ONE_SIDE, 1);
    if(nRet == RT_OK){
        return;
    }

    /*����,*/
    /*û�ط�����,�Ҹ��ط���һ����*/
    for(i = 0; i < GOMOKU_CHESS_COUNT; i++){
        for(j = 0; j < GOMOKU_CHESS_COUNT; j++){
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
                AI_Down_Chess(i, j);
                return;
            }
        }
    }
    /*��������*/
    gomoku_gameover_page_init(CH_WHITE);
}

/* ���������ʼ��
 */
static void gomoku_gameover_page_init(uint8_t winer)
{
    /*������������������ʽ*/
    static lv_style_t over_style;/*������ʽ*/
	lv_style_init(&over_style);
	lv_style_set_bg_opa(&over_style, LV_OPA_50);/*���ñ���͸����*/
	lv_style_set_radius(&over_style, 10);	/*����Բ��*/
	lv_style_set_bg_color(&over_style, lv_color_hex(WHITE_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&over_style, 0);
	lv_style_set_pad_bottom(&over_style, 0);
	lv_style_set_pad_left(&over_style, 0);
	lv_style_set_pad_right(&over_style, 0);
    lv_style_set_border_width(&over_style, 1);

    /*������ť������ʽ*/
    static lv_style_t button_style;/*������ʽ*/
	lv_style_init(&button_style);
	lv_style_set_bg_opa(&button_style, LV_OPA_50);/*���ñ���͸����*/
	lv_style_set_radius(&button_style, 100);	/*����Բ��*/
	lv_style_set_bg_color(&button_style, lv_color_hex(WHITE_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&button_style, 0);
	lv_style_set_pad_bottom(&button_style, 0);
	lv_style_set_pad_left(&button_style, 0);
	lv_style_set_pad_right(&button_style, 0);
    lv_style_set_border_width(&button_style, 0);

    lv_obj_t * over_con = lv_obj_create(g_pt_lv_gomoku->bg_about);
    lv_obj_add_style(over_con, &over_style, 0);
    lv_obj_set_size(over_con, 240, 140);
    lv_obj_align(over_con, LV_ALIGN_TOP_MID, 0, 160);

    /*������ǩ*/
    lv_obj_t *label_winer = lv_label_create(over_con);
	lv_obj_set_style_text_font(label_winer, &lv_font_montserrat_18, 0);
	if(winer == CH_BLACK){
        lv_obj_set_style_text_color(label_winer, lv_color_hex(BLACK_HEX), 0);
        lv_label_set_text(label_winer, "black chess pieces win");
	}
	else{
	    lv_obj_set_style_text_color(label_winer, lv_color_hex(RED_HEX), 0);
        lv_label_set_text(label_winer, "red chess pieces win");
	}
    lv_obj_align(label_winer, LV_ALIGN_TOP_MID, 0, 10);
    /*��������*/
    lv_obj_t * new_con = lv_obj_create(over_con);
    lv_obj_add_style(new_con, &button_style, 0);
    lv_obj_set_size(new_con, 50, 50);
    lv_obj_align(new_con, LV_ALIGN_BOTTOM_LEFT, 10, 0);
    /*��ӿ��Ե���¼�*/
    lv_obj_add_flag(new_con, LV_OBJ_FLAG_CLICKABLE);/*������*/
    lv_obj_add_event_cb(new_con, event_handler_newgame_pressed, LV_EVENT_CLICKED, (void *) over_con);
    lv_obj_set_style_bg_color(new_con, lv_color_hex(RED_HEX), LV_STATE_PRESSED);
    /*����new��ǩ*/
    lv_obj_t *label_new = lv_label_create(new_con);
    lv_obj_set_style_text_font(label_new, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label_new, lv_color_hex(GREEN_HEX), 0);
    lv_label_set_text(label_new, "new");
    lv_obj_center(label_new);

    /*��������*/
    lv_obj_t * back_con = lv_obj_create(over_con);
    lv_obj_add_style(back_con, &button_style, 0);
    lv_obj_set_size(back_con, 50, 50);
    lv_obj_align(back_con, LV_ALIGN_BOTTOM_RIGHT, -10, 0);
    /*��ӿ��Ե���¼�*/
    lv_obj_add_flag(back_con, LV_OBJ_FLAG_CLICKABLE);/*������*/
    lv_obj_add_event_cb(back_con, event_handler_backgame_pressed, LV_EVENT_CLICKED, (void *) over_con);
    lv_obj_set_style_bg_color(back_con, lv_color_hex(RED_HEX), LV_STATE_PRESSED);
    /*����new��ǩ*/
    lv_obj_t *label_back = lv_label_create(back_con);
    lv_obj_set_style_text_font(label_back, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label_back, lv_color_hex(GREEN_HEX), 0);
    lv_label_set_text(label_back, "back");
    lv_obj_center(label_back);

}


/* gomoku_gameover_page
 * �жϵ�ǰ��������,��Ϸ�Ƿ����
 */
static void gomoku_gameover_page(uint8_t chess_id)
{
    uint8_t nRet;
    uint8_t i, j;
    uint8_t chess_color;
    i = chess_id / GOMOKU_CHESS_COUNT;
    j = chess_id % GOMOKU_CHESS_COUNT;
    chess_color = g_pt_lv_gomoku->Ch_array[i][j].ch_status;
    nRet = gomoku_check_Winner(chess_id);/*�ж��Ƿ����������*/
    if(nRet == RT_OK){/*��Ϸ����*/
        gomoku_gameover_page_init(chess_color);
        //gomoku_clean_checkerboard();
    }
}


static void gomoku_Start_page(void)
{
    /*����������ʽ*/
    static lv_style_t start_cont;/*������ʽ*/
	lv_style_init(&start_cont);
	lv_style_set_bg_opa(&start_cont, LV_OPA_100);/*���ñ���͸����*/
	lv_style_set_radius(&start_cont, 10);	/*����Բ��*/
	lv_style_set_bg_color(&start_cont, lv_color_hex(WHITE_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&start_cont, 0);
	lv_style_set_pad_bottom(&start_cont, 0);
	lv_style_set_pad_left(&start_cont, 0);
	lv_style_set_pad_right(&start_cont, 0);
    lv_style_set_border_width(&start_cont, 1);

    /*������ť������ʽ*/
    static lv_style_t button_style;/*������ʽ*/
	lv_style_init(&button_style);
	lv_style_set_bg_opa(&button_style, LV_OPA_50);/*���ñ���͸����*/
	lv_style_set_radius(&button_style, 10);	/*����Բ��*/
	lv_style_set_bg_color(&button_style, lv_color_hex(WHITE_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&button_style, 0);
	lv_style_set_pad_bottom(&button_style, 0);
	lv_style_set_pad_left(&button_style, 0);
	lv_style_set_pad_right(&button_style, 0);
    lv_style_set_border_width(&button_style, 1);

    /*������Ϸģʽѡ�����*/

    lv_obj_t * over_con = lv_obj_create(g_pt_lv_gomoku->bg_about);
    lv_obj_add_style(over_con, &start_cont, 0);
    lv_obj_set_size(over_con, 200, 300);
    lv_obj_align(over_con, LV_ALIGN_TOP_MID, 0, 60);

    /*��������*/
    lv_obj_t * play_con = lv_obj_create(over_con);
    lv_obj_add_style(play_con, &button_style, 0);
    lv_obj_set_size(play_con, 160, 80);
    lv_obj_align(play_con, LV_ALIGN_TOP_MID, 0, 20);
    /*��ӿ��Ե���¼�*/
    lv_obj_add_flag(play_con, LV_OBJ_FLAG_CLICKABLE);/*������*/
    lv_obj_add_event_cb(play_con, event_handler_playwithplayer_pressed, LV_EVENT_CLICKED, (void *) over_con);
    lv_obj_set_style_bg_color(play_con, lv_color_hex(RED_HEX), LV_STATE_PRESSED);
    /*����new��ǩ*/
    lv_obj_t *label_new = lv_label_create(play_con);
    lv_obj_set_style_text_font(label_new, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label_new, lv_color_hex(GREEN_HEX), 0);
    lv_label_set_text(label_new, "Play with Player");
    lv_obj_center(label_new);

    /*��������*/
    lv_obj_t * back_con = lv_obj_create(over_con);
    lv_obj_add_style(back_con, &button_style, 0);
    lv_obj_set_size(back_con, 160, 80);
    lv_obj_align_to(back_con, play_con, LV_ALIGN_OUT_BOTTOM_MID, 0, 40);
    /*��ӿ��Ե���¼�*/
    lv_obj_add_flag(back_con, LV_OBJ_FLAG_CLICKABLE);/*������*/
    lv_obj_add_event_cb(back_con, event_handler_playwithpc_pressed, LV_EVENT_CLICKED, (void *) over_con);
    lv_obj_set_style_bg_color(back_con, lv_color_hex(RED_HEX), LV_STATE_PRESSED);
    /*����new��ǩ*/
    lv_obj_t *label_back = lv_label_create(back_con);
    lv_obj_set_style_text_font(label_back, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label_back, lv_color_hex(GREEN_HEX), 0);
    lv_label_set_text(label_back, "Play with PC");
    lv_obj_center(label_back);

}


/* gomoku_page_init
 * gomoku game �����ʼ��
 */
static void gomoku_page_init(void)
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
    lv_style_set_border_width(&style_cont, 1);

    /*��������*/
    gomoku_game_create_border();
    /*��������*/
    gomoku_create_checkerboard();

    gomoku_Start_page();

    //gomoku_gameover_page_init(2);
}

/* event_handler_back_to_home
 * ���淵�غ���
 * */
static void event_handler_back_to_home(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		if(g_pt_lv_gomoku->bg_about != NULL) lv_obj_del(g_pt_lv_gomoku->bg_about);
		vPortFree(g_pt_lv_gomoku);
		lv_main_page_demo_home(100);
	}
}


static void event_handler_chess_pressed(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    uint8_t chess_id = (uint8_t)lv_event_get_user_data(e);
    uint8_t i, j;
    i = chess_id / GOMOKU_CHESS_COUNT;
    j = chess_id % GOMOKU_CHESS_COUNT;
	lv_obj_t *obj = lv_event_get_current_target(e);
	if(code == LV_EVENT_CLICKED){
        if(g_pt_lv_gomoku->Play_mode == PLAYER_MODE){/*��ҶԾ�*/
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
                switch(g_pt_lv_gomoku->Ch_player){
                case CH_BLACK:
                    lv_obj_set_style_bg_color(obj, lv_color_hex(BLACK_HEX), 0);
                    g_pt_lv_gomoku->Ch_array[i][j].ch_status = CH_BLACK;
                    g_pt_lv_gomoku->Ch_player = CH_WHITE;
                    break;
                case CH_WHITE:
                    lv_obj_set_style_bg_color(obj, lv_color_hex(RED_HEX), 0);
                    g_pt_lv_gomoku->Ch_array[i][j].ch_status = CH_WHITE;
                    g_pt_lv_gomoku->Ch_player = CH_BLACK;
                    break;
                }
                gomoku_gameover_page(chess_id);/*�����Ϸ�Ƿ����*/
            }
        }
        else{/*�˻���սģʽ*/
            if(g_pt_lv_gomoku->Ch_array[i][j].ch_status == CH_NONE){
                if(g_pt_lv_gomoku->Ch_player == CH_BLACK)/*���Ǻ���*/
                {
                    lv_obj_set_style_bg_color(obj, lv_color_hex(BLACK_HEX), 0);
                    g_pt_lv_gomoku->Ch_array[i][j].ch_status = CH_BLACK;
                    g_pt_lv_gomoku->Ch_player = CH_WHITE;
                    g_pt_lv_gomoku->player_path[g_pt_lv_gomoku->player_chess] = chess_id;
                    g_pt_lv_gomoku->player_chess ++;
                    gomoku_gameover_page(chess_id);/*�����Ϸ�Ƿ����*/
                    //AI_Down_OneChess(chess_id);
                    AI_Player_DownChess();
                }

            }
        }

	}
}

/* event_handler_newgame_pressed
 * newgame��ť����¼�����
 * */
static void event_handler_newgame_pressed(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
    lv_obj_t *over_con = (lv_obj_t *)lv_event_get_user_data(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		/*�������*/
        gomoku_clean_checkerboard();
        /*ɾ��over_con ����*/
        lv_obj_del(over_con);
	}
}

/* event_handler_backgame_pressed
 * backgame��ť����¼�����
 * */
static void event_handler_backgame_pressed(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
    lv_obj_t *over_con = (lv_obj_t *)lv_event_get_user_data(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		/*�������*/
        gomoku_clean_checkerboard();
        gomoku_Start_page();
        /*ɾ��over_con ����*/
        lv_obj_del(over_con);
	}
}



/* event_handler_playwithplayer_pressed
 * ��ҶԾ�ģʽ
 * */
static void event_handler_playwithplayer_pressed(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
    lv_obj_t *over_con = (lv_obj_t *)lv_event_get_user_data(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		/*�������*/
        gomoku_clean_checkerboard();
        g_pt_lv_gomoku->Play_mode = PLAYER_MODE;
        /*ɾ��over_con ����*/
        lv_obj_del(over_con);
	}
}

/* event_handler_playwithpc_pressed
 * �˻��Ծ�ģʽ
 * */
static void event_handler_playwithpc_pressed(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	//lv_obj_t *obj = lv_event_get_current_target(event);
    lv_obj_t *over_con = (lv_obj_t *)lv_event_get_user_data(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
		/*�������*/
        gomoku_clean_checkerboard();
        g_pt_lv_gomoku->Play_mode = AI_MODE;
        /*ɾ��over_con ����*/
        lv_obj_del(over_con);
	}
}

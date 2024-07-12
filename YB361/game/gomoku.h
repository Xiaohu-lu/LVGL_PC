#ifndef GOMOKU_H_INCLUDED
#define GOMOKU_H_INCLUDED


#define GOMOKU_GAME_VALID_CONT_W                      (300)
#define GOMOKU_GAME_VALID_CONT_H                      (300)
#define GOMOKU_VALID_CONT_BORDER_WIDTH                (5)


#define GOMOKU_BOARD_Y_OFFSET                         (100)
#define GOMOKU_CHESS_COUNT                            (11)


#define RT_OK                                          0
#define RT_FAIL                                        1

typedef enum{
    ONE_SIDE = 0,
    DOUBLE_SIDE,
}CHESK_SIDE;

typedef enum{
    PLAYER_MODE = 0,
    AI_MODE,
}Play_mode_t;

typedef enum{
    CH_NONE = 0,
    CH_BLACK,
    CH_WHITE,
}Chess_Status;

typedef enum{
    IS_FREE = 0,
    IS_BUSY,
}Chess_page;

typedef struct{
    uint8_t ch_status;
    lv_obj_t *ch_obj;
}Chess_t;

typedef struct{
    uint8_t ch_count;
    uint8_t ch_flag;
}Dir_Chess_t;

typedef struct{
    Dir_Chess_t ch_up;
    Dir_Chess_t ch_down;
    Dir_Chess_t ch_left;
    Dir_Chess_t ch_right;
    Dir_Chess_t ch_lu;
    Dir_Chess_t ch_rd;
    Dir_Chess_t ch_ru;
    Dir_Chess_t ch_ld;
}AI_Decision_t;


typedef struct _lv_gomoku{
	lv_obj_t *bg_about;				/*背景*/
    lv_obj_t  * game_value_cont;        //游戏游戏活动区域
    Chess_t Ch_array[GOMOKU_CHESS_COUNT][GOMOKU_CHESS_COUNT];/*棋子对象*/
    uint8_t player_path[61];
    uint8_t AI_path[61];
    uint8_t player_chess;
    uint8_t AI_chess;
    uint8_t Ch_player;  /*当前执棋者*/
    uint8_t Play_mode;
}T_lv_gomoku, *PT_lv_gomoku;


void gomoku_page(void);

#endif // GOMOKU_H_INCLUDED

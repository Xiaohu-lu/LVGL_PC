#ifndef LV_BLOCK_GAME_H_INCLUDED
#define LV_BLOCK_GAME_H_INCLUDED


#define LV_BLOCK_GAME_VALID_CONT_W      (260)
#define LV_BLOCK_GAME_VALID_CONT_H      (420)

#define LV_BLOCK_GAME_BASE_CONT_SIZE    (20)

typedef enum
{
    STYLE_1 = 0,
    STYLE_2,
    STYLE_3,
    STYLE_4,
    STYLE_5,
    STYLE_6,
    STYLE_7,
    STYLE_LAST
}BLOCK_TYPE;

typedef enum
{
    STATE_1 = 0,
    STATE_2,
    STATE_3,
    STATE_4,
    STATE_LAST
}BLOCK_STATE;

typedef struct _block_index
{
    uint8_t x_index;
    uint8_t y_index;
}block_pos_t;

typedef struct _base_block
{
    lv_obj_t *block_1;
    block_pos_t block_1_pos;
    lv_obj_t *block_2;
    block_pos_t block_2_pos;
    lv_obj_t *block_3;
    block_pos_t block_3_pos;
    lv_obj_t *block_4;
    block_pos_t block_4_pos;
}T_lv_baseblock;

typedef struct _block
{
    uint8_t type;       /*方块的类型*/
    uint8_t state;      /*方块旋转的状态*/
    T_lv_baseblock baseblock;
}T_lv_block;

typedef struct _lv_blockgame {
	lv_obj_t  * bg_about;				// 背景
    lv_obj_t  * game_value_cont;        //游戏游戏活动区域
} T_lv_blockgame, *PT_lv_blockgame;



void lv_block_game(void);
#endif // LV_BLOCK_GAME_H_INCLUDED

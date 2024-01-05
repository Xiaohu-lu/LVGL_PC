#ifndef LV_BLOCK_GAME_H_INCLUDED
#define LV_BLOCK_GAME_H_INCLUDED


#define LV_BLOCK_GAME_VALID_CONT_W                      (260)
#define LV_BLOCK_GAME_VALID_CONT_H                      (420)
#define LV_BLOCK_GAME_VALID_CONT_BORDER_WIDTH           (5)


#define LV_BLOCK_GAME_BASE_CONT_SIZE                    (20)

#define LV_BLOCK_GAME_BASE_BLOCK_BORDER_COLOR           (WHITE_HEX)
#define LV_BLOCK_GAME_BASE_BLOCK_DEFAULT_COLOR          (WHITE_HEX)


#define LV_BLOCK_GAME_START_X_INDEX                      (6)
#define LV_BLOCK_GAME_START_Y_INDEX                      (0)


#define LV_BLOCK_GAME_BTN_W                              (40)
#define LV_BLOCK_GAME_BTN_H                              (60)
typedef enum
{
    RT_OK = 0,          /*成功返回不需操作*/
    RT_NEED_NEW,        /*表示一个块不能移动了,需要创建新块*/
}RT_TYPE;


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

typedef enum
{
    BL_ALIGN_TOP = 0,
    BL_ALIGN_BOTTOM,
    BL_ALIGN_LEFT,
    BL_ALIGN_RIGHT

}BL_ALIGN;

typedef enum
{
    NOT_FILL = 0,
    IS_FILL,
}FILL_STATE;

#define RED_COLOR_HEX           (0xFF0000)
#define ORANGE_COLOR_HEX        (0xFFA500)
#define YELLOW_COLOR_HEX        (0xFFFF00)
#define GREEN_COLOR_HEX         (0x00FF00)
#define CANY_COLOR_HEX          (0x00FFFF)
#define BLUE_COLOR_HEX          (0x0000FF)
#define PURPLE_COLOR_HEX        (0xA020F0)

typedef enum
{
    BLOCK_RED = 0,
    BLOCK_ORANGE,
    BLOCK_YELLOW,
    BLOCK_GREEN,
    BLOCK_CANY,
    BLOCK_BLUE,
    BLOCK_PURPLE,
    BLOCK_RANGE
}BLOCK_COLOR;


typedef struct _base_block/*基础对象*/
{
    lv_obj_t *obj;
    uint8_t is_fill;
    //uint8_t x;
    //uint8_t y;
    uint32_t color;
}Base_block_t;

/* 俄罗斯方块
 * 是由4个小方块组成的
 */
typedef struct _block
{
    uint8_t type;       /*块的类型*/
    uint8_t state;      /*块当前的状态*/
    int x[4];           /*第一个块的x*/
    int y[4];           /*第一个块的y*/
    uint32_t color;     /*创建块的颜色*/
}Block_t;

typedef struct _lv_blockgame {
	lv_obj_t  * bg_about;				// 背景
    lv_obj_t  * game_value_cont;        //游戏游戏活动区域
    Base_block_t block[LV_BLOCK_GAME_VALID_CONT_W/LV_BLOCK_GAME_BASE_CONT_SIZE][LV_BLOCK_GAME_VALID_CONT_H/LV_BLOCK_GAME_BASE_CONT_SIZE];
    lv_timer_t * update_timer;          //定时器
    lv_obj_t  * left_btn;               //左移按钮
    lv_obj_t  * right_btn;              //右移按钮
    lv_obj_t  * switch_btn;             //切换按钮


} T_lv_blockgame, *PT_lv_blockgame;



void lv_block_game(void);
#endif // LV_BLOCK_GAME_H_INCLUDED

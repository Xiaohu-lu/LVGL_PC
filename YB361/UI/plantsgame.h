#ifndef PLANTSGAME_H_INCLUDED
#define PLANTSGAME_H_INCLUDED

#define MAX_SUN_COUNT       100

#define COLLECT_SUN_X       10
#define COLLECT_SUN_Y       2

#define SYS_SUN_EXI_TIME         6   /*pc是以s为单位6s*/
#define SUN_FLOWER_EXI_TIME     8   /*pc是以s为单位10s*/
#define SUN_FLOWER_GEN_INTER    16   /*产生的时间间隔*/

typedef enum
{
    SUN_SYS_GEN = 0,        /*系统产生的*/
    SUN_FLO_GEN,            /*太阳花产生的*/
}SUN_TYPE;

typedef enum
{
    PLANT_NONE = 0,
    PLANT_SUN,              /*种植的是太阳花*/
    PLANT_GAT,              /*种植的是豌豆射手*/
}PLANT_NAME;

typedef enum
{
    WAIT_CLICK = 0,         /*等待点击*/
    ALRE_CLICK,             /*已经点击过了*/
    WAIT_GEN,               /*等待生成阳光*/
}SUN_STATE;

typedef enum
{
    GAL_WAIT_GEN = 0,       /*等待产生子弹*/
    GAL_WAIT_CRASH,         /*等待碰撞*/
    GAL_NEED_DEL,           /*需要删除的子弹*/
};


typedef enum
{
    CLIK_FUNC_NONE = 0,     /*无功能*/
    CLIK_FUNC_FLOWER,       /*种植太阳花*/
    CLIK_FUNC_GATLPEA,      /*种植豌豆射手*/

    CLIK_FUNC_SHOVEL,       /*铲除植物*/
}CLICK_FUNC_TYPE;

typedef struct _lv_plant{
	lv_obj_t *bg_about;				/*背景*/
	lv_obj_t *sun_label;            /*阳光值得分标签*/
	lv_obj_t *menu_flower;          /*太阳花*/
    lv_obj_t *menu_gatl;            /*豌豆射手*/

	lv_obj_t *menu_shovel;          /*铲子*/
	lv_obj_t *bg_sod[45];			/*种植地*/
    uint8_t bg_sodstate[45];       /*背景块的用途*/
    uint8_t click_func;            /*当前点击背景块的功能*/
}T_lv_plant, *PT_lv_plant;

typedef struct _lv_sunevent{

    uint32_t gen_time;              /*生成时间戳*/
    uint32_t exi_time;              /*持续时间*/
    uint8_t sun_state;              /*阳光状态,0:等待点击,1:点击到等待动画效果*/
    lv_obj_t *obj;
    struct _lv_sunevent *next;
}Sun_event_t;


typedef struct _lv_sunflower
{

    uint32_t gen_time;              /*生成时间戳*/
    uint32_t exi_time;              /*持续时间*/
    uint8_t sun_state;              /*阳光状态,0:等待点击,1:点击到等待动画效果*/
    uint8_t block_id;               /*太阳花的位置*/
    lv_obj_t *obj;
    struct _lv_sunflower *next;
}SunFlower_event_t;


typedef struct _lv_gatlpea
{
    uint8_t gat_state;             /*射手状态*/
    uint8_t block_id;              /*射手位置*/
    struct _lv_gatlpea *next;
    lv_obj_t *gatling;              /*射手*/
    lv_obj_t *pea;                  /*豌豆子弹*/
}Gatlingpea_t;


void lv_plant_game(void);
#endif // PLANTSGAME_H_INCLUDED

#ifndef PLANTSGAME_H_INCLUDED
#define PLANTSGAME_H_INCLUDED

#define MAX_SUN_COUNT       100

#define COLLECT_SUN_X       10
#define COLLECT_SUN_Y       2

#define EAT_PLANT_COUNT     20

#define SYS_SUN_EXI_TIME         6   /*pc����sΪ��λ6s*/
#define SUN_FLOWER_EXI_TIME     8   /*pc����sΪ��λ10s*/
#define SUN_FLOWER_GEN_INTER    16   /*������ʱ����*/
#define PEA_GEN_TIME            4
typedef enum
{
    SUN_SYS_GEN = 0,        /*ϵͳ������*/
    SUN_FLO_GEN,            /*̫����������*/
}SUN_TYPE;

typedef enum
{
    PLANT_NONE = 0,
    PLANT_SUN,              /*��ֲ����̫����*/
    PLANT_GAT,              /*��ֲ�����㶹����*/
}PLANT_NAME;

typedef enum
{
    WAIT_CLICK = 0,         /*�ȴ����*/
    ALRE_CLICK,             /*�Ѿ��������*/
    WAIT_GEN,               /*�ȴ���������*/
}SUN_STATE;

typedef enum
{
    GAL_WAIT_GEN = 0,       /*�ȴ������ӵ�*/
    GAL_WAIT_CRASH,         /*�ȴ���ײ*/
    GAL_NEED_DEL,           /*��Ҫɾ�����ӵ�*/
};


typedef enum
{
    CLIK_FUNC_NONE = 0,     /*�޹���*/
    CLIK_FUNC_FLOWER,       /*��ֲ̫����*/
    CLIK_FUNC_GATLPEA,      /*��ֲ�㶹����*/

    CLIK_FUNC_SHOVEL,       /*����ֲ��*/
}CLICK_FUNC_TYPE;

typedef struct _lv_plant{
	lv_obj_t *bg_about;				/*����*/
	lv_obj_t *sun_label;            /*����ֵ�÷ֱ�ǩ*/
	lv_obj_t *menu_flower;          /*̫����*/
    lv_obj_t *menu_gatl;            /*�㶹����*/

	lv_obj_t *menu_shovel;          /*����*/
	lv_obj_t *bg_sod[45];			/*��ֲ��*/
    uint8_t bg_sodstate[45];       /*���������;*/
    uint8_t click_func;            /*��ǰ���������Ĺ���*/
}T_lv_plant, *PT_lv_plant;

typedef struct _lv_sunevent{

    uint32_t gen_time;              /*����ʱ���*/
    uint32_t exi_time;              /*����ʱ��*/
    uint8_t sun_state;              /*����״̬,0:�ȴ����,1:������ȴ�����Ч��*/
    lv_obj_t *obj;
    struct _lv_sunevent *next;
}Sun_event_t;


typedef struct _lv_sunflower
{
    uint32_t gen_time;              /*����ʱ���*/
    uint32_t exi_time;              /*����ʱ��*/
    uint8_t sun_state;              /*����״̬,0:�ȴ����,1:������ȴ�����Ч��*/
    uint8_t block_id;               /*̫������λ��*/
    lv_obj_t *obj;
    struct _lv_sunflower *next;
}SunFlower_event_t;


typedef struct _lv_gatlpea
{
    uint8_t gat_state;             /*����״̬*/
    uint8_t block_id;              /*����λ��*/
    struct _lv_gatlpea *next;
    lv_obj_t *gatling;              /*����*/
    lv_obj_t *pea;                  /*�㶹�ӵ�*/
}Gatlingpea_t;

#define SUN_SHOW_MAX            50

typedef enum
{
    SUN_STATE_FREE = 0,
    SUN_STATE_BUSY,
}SUN_STATE_E;


typedef struct _lv_sun_info
{
    lv_obj_t *obj_sun[SUN_SHOW_MAX];    /*̫������*/
    //uint8_t sun_state[SUN_SHOW_MAX];    /*̫������״̬*/
}All_Sun_Info_t;

#define PEA_SHOW_MAX           45

#define PEA_SPEED_X             10
#define ZOM_SPPED_X             2
typedef struct _lv_pea_s
{
    lv_obj_t *obj;          /*������*/
    struct _lv_pea_s *next;
    uint8_t cnt;
}Pea_Obj_Info_t;

typedef struct _lv_pea
{
    lv_obj_t *pea;              /*�ӵ�����*/
    struct _lv_pea *next;       /*��һ��*/
    uint16_t x;                 /*x����*/
    uint8_t state;              /*Ҫɾ��*/
}Pea_Info_t;    /*�㶹�ӵ���Ϣ*/


typedef struct _lv_zombie
{
    lv_obj_t *zom;              /*��ʬ����*/
    struct _lv_zombie *next;
    uint16_t x;                 /*x����*/
    uint8_t crash_count;        /*��ǰ��ײ����*/
    uint8_t crash_max;          /*�����ײ����*/
    uint8_t wait_count;         /*�ȴ��Ե�ֲ��*/
}Zom_Info_t;

typedef enum
{
    CRASH_NONE = 0,
    CRASH_NEXT,
}CRASH_E;

typedef enum
{
    MAN_STOP = 0,
    MAN_START,
}MANPEA_E;


typedef enum
{
    PEA_WAIT_GEN = 0,
    PEA_CAN_GEN,
}PEA_GEN_E;


typedef struct _lv_man_pea
{
    struct _lv_man_pea *next;
    uint32_t gen_time;          /*ʱ���*/
    uint8_t id;                 /*�㶹���ִ��ڵĿ�*/
    uint8_t state;              /*�н�ʬ,����,û��ʬ������*/
    uint8_t need_gen;           /*�Ƿ�Ҫ�����㶹*/
}Manage_pea_t;






void lv_plant_game(void);
#endif // PLANTSGAME_H_INCLUDED

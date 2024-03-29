#ifndef PLANTSGAME_H_INCLUDED
#define PLANTSGAME_H_INCLUDED

#define MAX_SUN_COUNT       100

#define COLLECT_SUN_X       10
#define COLLECT_SUN_Y       2

#define SYS_SUN_EXI_TIME         6   /*pc����sΪ��λ6s*/
#define SUN_FLOWER_EXI_TIME     8   /*pc����sΪ��λ10s*/
#define SUN_FLOWER_GEN_INTER    16   /*������ʱ����*/

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


void lv_plant_game(void);
#endif // PLANTSGAME_H_INCLUDED

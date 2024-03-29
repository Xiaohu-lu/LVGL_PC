#include "lv_typedef.h"
#include "lvgl/lvgl.h"
#include "UI/lv_main_page.h"
#include "UI/lv_about_page.h"
#include "plantsgame.h"
#include "lv_dev_info_page.h"

static PT_lv_plant g_pt_lv_plant;		/*���ݽṹ��*/

Sun_event_t gSun_event_head;            /*ϵͳ����������*/
SunFlower_event_t gSunflower_event_head;  /*̫��������������*/
Gatlingpea_t gGat_event_head;           /*�㶹����*/

static lv_timer_t *gManageFlowertimer;

uint32_t gTotal_sun = 100;

static void lv_plant_game_init(void);
static void CreateOneSun(void);
static void event_handler_sodclicked(lv_event_t *e);
static void event_handler_sunclicked(lv_event_t *e);
static void CreateOneSunFlower(uint8_t block_id);
static void event_handler_flowersunclicked(lv_event_t *e);
static void CollectSun(lv_obj_t *sun, uint8_t type);
static void event_handler_menu_flowerclicked(lv_event_t *e);
static void event_handler_menu_shovelclicked(lv_event_t *e);
static void event_handler_menu_gatlclicked(lv_event_t *e);
static void sun_anim_y_cb(void *var, int32_t v);
static void sun_anim_x_cb(void *var, int32_t v);


LV_IMG_DECLARE(background1);
LV_IMG_DECLARE(SeedBank);
LV_IMG_DECLARE(SunBank);
LV_IMG_DECLARE(Sun2);
LV_IMG_DECLARE(Sun3);
LV_IMG_DECLARE(sunflower);
LV_IMG_DECLARE(sunflowermenu);
LV_IMG_DECLARE(shovel);
LV_IMG_DECLARE(pea);
LV_IMG_DECLARE(Gatlingpea);
LV_IMG_DECLARE(Gatlingpeamenu);
/* lv_plant_game
 * ֲ���ս��ʬ��Ϸ
 * */
void lv_plant_game(void)
{
	g_pt_lv_plant = (T_lv_plant*)pvPortMalloc(sizeof(T_lv_plant));
	g_pt_lv_plant->bg_about = lv_obj_create(lv_scr_act());/*��������*/


	lv_obj_set_size(g_pt_lv_plant->bg_about, LV_VER_RES, LV_HOR_RES);/*���ô�С*/
	lv_obj_set_style_radius(g_pt_lv_plant->bg_about, 0, 0);/*����Բ��*/
	lv_obj_align(g_pt_lv_plant->bg_about, LV_ALIGN_TOP_LEFT, 0, 0);

	set_user_style(g_pt_lv_plant->bg_about);
	lv_obj_clear_flag(g_pt_lv_plant->bg_about, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_size(g_pt_lv_plant->bg_about, LV_HOR_RES, LV_VER_RES);/*���ô�С*/
	lv_obj_set_style_bg_opa(g_pt_lv_plant->bg_about, LV_OPA_100, 0);/*���ñ���͸����*/
	lv_obj_set_style_bg_color(g_pt_lv_plant->bg_about, lv_color_white(), 1);
	lv_plant_game_init();/*�����ʼ��*/

}

/* CollectEventCb
 * �ռ�ϵͳ����Ļص�����,��ʱ���ص�����
 */
static void CollectEventCb(lv_timer_t *timer)
{
    Sun_event_t *temp = &gSun_event_head;
    Sun_event_t *delnode;
    delnode = temp->next;
    uint32_t cur_time = xTaskGetTickCount();
    while(delnode)
    {
        if(delnode->sun_state == WAIT_CLICK)/*�ȴ����*/
        {
            /*�жϳ���ʱ���Ƿ���*/
            if(delnode->gen_time + delnode->exi_time < cur_time)/*����ʱ�䵽��*/
            {
                /*ֱ��ɾ������*/
                printf("del sun = %x\r\n", delnode->obj);
                lv_obj_remove_event_cb(delnode->obj, event_handler_sunclicked);/*�Ƴ��ص�����*/
                lv_obj_del(delnode->obj);/*ɾ������*/
                //lv_obj_clean(delnode->obj);
                /*��������ɾ��*/
                temp->next = delnode->next;

                vPortFree(delnode);
            }
            else
            {
                temp = delnode;
            }
        }
        else/*�����,�ж����⶯��*/
        {
            if((lv_obj_get_x(delnode->obj) == COLLECT_SUN_X) && (lv_obj_get_y(delnode->obj) == COLLECT_SUN_Y))
            {
                printf("del sun = %x\r\n", delnode->obj);
                lv_anim_del(delnode->obj, NULL);/*ɾ������*/
                //lv_anim_del(delnode->obj, NULL);/*ɾ������*/
                lv_obj_remove_event_cb(delnode->obj, event_handler_sunclicked);/*�Ƴ��ص�����*/
                lv_obj_del(delnode->obj);/*ɾ������*/
                //lv_obj_clean(delnode->obj);
                /*��������ɾ��*/
                temp->next = delnode->next;

                vPortFree(delnode);
                /*����ֵ++*/
                gTotal_sun += 25;
                lv_label_set_text_fmt(g_pt_lv_plant->sun_label, "%d", gTotal_sun);
            }
            else
            {
                temp = delnode;
            }
        }


        delnode = delnode->next;
    }

}

/* ManageSunFlowerEventCb
 * ����̫������������
 */
static void ManageSunFlowerEventCb(lv_timer_t *timer)
{
    SunFlower_event_t *temp = &gSunflower_event_head;
    SunFlower_event_t *delnode;
    delnode = temp->next;
    uint32_t cur_time = xTaskGetTickCount();

    while(delnode)
    {
        if(delnode->sun_state == WAIT_GEN)/*�ȴ���������*/
        {
            if(delnode->gen_time + SUN_FLOWER_GEN_INTER < cur_time)/*����һ��̫��*/
            {
                delnode->gen_time = cur_time;       /*����ʱ��*/
                delnode->sun_state = WAIT_CLICK;    /*�ȴ����*/
                //delnode->obj = lv_img_create(g_pt_lv_plant->bg_sod[delnode->block_id]);
                //delnode->obj = lv_img_create(g_pt_lv_plant->bg_about);
                delnode->obj = lv_img_create(lv_scr_act());
                lv_img_set_src(delnode->obj, &Sun2);
                /*����λ��*/
                if((delnode->block_id/9) == 0)
                {
                    lv_obj_set_pos(delnode->obj, 26+(delnode->block_id%9)*47 - 4, 45+(delnode->block_id/9)*47 + 14);
                }
                else if((delnode->block_id/9) == 1)
                {
                    lv_obj_set_pos(delnode->obj, 26+(delnode->block_id%9)*47 - 4, 45+(delnode->block_id/9)*47 + 14+3);
                }
                else if((delnode->block_id/9) == 2)
                {
                    lv_obj_set_pos(delnode->obj, 26+(delnode->block_id%9)*47 - 4, 45+(delnode->block_id/9)*47 + 14+3+4);
                }
                else if((delnode->block_id/9) == 3)
                {
                    lv_obj_set_pos(delnode->obj, 26+(delnode->block_id%9)*47 - 4, 45+(delnode->block_id/9)*47 + 14+3+4+5);
                }
                else if((delnode->block_id/9) == 4)
                {
                    lv_obj_set_pos(delnode->obj, 26+(delnode->block_id%9)*47 - 4, 45+(delnode->block_id/9)*47 + 14+3+4+5+6);
                }

                /*��ӿ��Ե����־*/
                lv_obj_add_flag(delnode->obj, LV_OBJ_FLAG_CLICKABLE);/*������*/
                /*��ӵ���¼��ص�����*/
                lv_obj_add_event_cb(delnode->obj, event_handler_flowersunclicked, LV_EVENT_CLICKED, NULL);
                LOG_D("sun flower create %x\r\n", delnode->obj);
            }
        }
        else if(delnode->sun_state == WAIT_CLICK)/*�ȴ��������*/
        {
            if(delnode->gen_time + SUN_FLOWER_EXI_TIME < cur_time)/*�������ʱ�䵽,ɾ������*/
            {
                lv_obj_remove_event_cb(delnode->obj, event_handler_flowersunclicked);/*�Ƴ��ص�����*/
                lv_obj_del(delnode->obj);           /*ɾ������*/
                //lv_obj_clean(delnode->obj);
                LOG_D("wait del %x\r\n", delnode->obj);
                delnode->gen_time = cur_time;       /*����ʱ��*/
                delnode->sun_state = WAIT_GEN;      /*�ȴ����*/
            }
        }
        else if(delnode->sun_state == ALRE_CLICK)/*�ȴ��ռ�����*/
        {
            if((lv_obj_get_x(delnode->obj) == COLLECT_SUN_X) && (lv_obj_get_y(delnode->obj) == COLLECT_SUN_Y))
            {
                lv_anim_del(delnode->obj, NULL);/*ɾ������*/
                //lv_anim_del(delnode->obj, NULL);/*ɾ������*/
                lv_obj_remove_event_cb(delnode->obj, event_handler_flowersunclicked);/*�Ƴ��ص�����*/
                lv_obj_del(delnode->obj);   /*ɾ������*/
                //lv_obj_clean(delnode->obj);
                LOG_D("click del %x\r\n", delnode->obj);
                delnode->gen_time = cur_time;       /*����ʱ��*/
                delnode->sun_state = WAIT_GEN;      /*�ȴ����*/
                /*����ֵ++*/
                gTotal_sun += 25;
                lv_label_set_text_fmt(g_pt_lv_plant->sun_label, "%d", gTotal_sun);
            }
        }
        temp = delnode;/*����һ��*/
        delnode = delnode->next;

    }

}

/* ManageGatEventCb
 * �����㶹����
 */
static void ManageGatEventCb(lv_timer_t *timer)
{


}

/* CreateSunCb
 * ϵͳָ��ʱ����ʱ���������,��ʱ���ص�����
 */
static void CreateSunCb(lv_timer_t *timer)
{
    CreateOneSun();
}

/* CollectSunEvent_init
 * �ռ�����Ͳ��������ʼ��,����������ʱ��
 */
static void CollectSunEvent_init(void)
{
    /*ϵͳ����������*/
    gSun_event_head.next = NULL;
    /*����һ����ʱ��*/
    lv_timer_create(CollectEventCb, 200, NULL);
    /*����һ������̫���Ķ�ʱ��*/
    lv_timer_create(CreateSunCb, 5000, NULL);

    /*̫��������������*/
    gSunflower_event_head.next = NULL;
    /*����һ������̫�����Ķ�ʱ��*/
    //gManageFlowertimer = lv_timer_create(ManageSunFlowerEventCb, 200, NULL);

    /*�㶹����*/
    gGat_event_head.next = NULL;
    /*����һ�������㶹���ֵĶ�ʱ��*/
    lv_timer_create(ManageGatEventCb, 200, NULL);
}

/* CollectSunEvent_Add
 * ���һ��ϵͳ���������⵽�����¼���
 */
static void CollectSunEvent_Add(lv_obj_t *sun)
{
    Sun_event_t *newNode = (Sun_event_t*)pvPortMalloc(sizeof(Sun_event_t));
    if(newNode == NULL)
    {
        LOG_D("not have enough space");
        return;
    }
    newNode->obj = sun;
    newNode->next = NULL;
    /*��ȡ����ʱ��*/
    newNode->gen_time = xTaskGetTickCount();
    newNode->exi_time = SYS_SUN_EXI_TIME;
    newNode->sun_state = WAIT_CLICK;/*�ȴ��������*/
    Sun_event_t *temp = &gSun_event_head;
    while(temp->next)
    {
        temp = temp->next;
    }
    /*��ӽڵ�*/
    temp->next = newNode;
}

/* Set_ClickFunc
 * ���õ���Ĺ���
 */
static void Set_ClickFunc(uint8_t func)
{
    g_pt_lv_plant->click_func = func;
}

/* lv_plant_game_init
 * ֲ���ս��ʬ�����ʼ��
 * */
static void lv_plant_game_init(void)
{
    /*����������ʽ*/
    static lv_style_t style_cont;/*������ʽ*/
	lv_style_init(&style_cont);
	lv_style_set_bg_opa(&style_cont, LV_OPA_10);/*���ñ���͸����*/
	lv_style_set_radius(&style_cont, 10);	/*����Բ��*/
	lv_style_set_bg_color(&style_cont, lv_color_hex(WHITE_HEX));/*���ñ�����ɫ��ɫ*/
	lv_style_set_pad_top(&style_cont, 0);
	lv_style_set_pad_bottom(&style_cont, 0);
	lv_style_set_pad_left(&style_cont, 0);
	lv_style_set_pad_right(&style_cont, 0);
    lv_style_set_border_width(&style_cont, 0);

    /*��ʾ����ͼƬ*/
    lv_obj_t *img_bg;
    img_bg = lv_img_create(g_pt_lv_plant->bg_about);
    lv_img_set_src(img_bg, &background1);
    lv_obj_align(img_bg, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    /*��ʾ����*/
    lv_obj_t *img_sun;
    img_sun = lv_img_create(g_pt_lv_plant->bg_about);
    lv_img_set_src(img_sun, &SunBank);
    lv_obj_align(img_sun, LV_ALIGN_TOP_LEFT, 10, 0);
    /*��ʾֲ����*/
    lv_obj_t *img_seed;
    img_seed = lv_img_create(g_pt_lv_plant->bg_about);
    lv_img_set_src(img_seed, &SeedBank);
    lv_obj_align_to(img_seed, img_sun, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);

    /*��������*/
    lv_obj_t *img_sod;
	uint8_t i, j, id;
	id = 0;
	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 9; j++)
		{
			g_pt_lv_plant->bg_sod[id] = lv_obj_create(g_pt_lv_plant->bg_about);
			img_sod = g_pt_lv_plant->bg_sod[id];
			lv_obj_set_size(img_sod, 47, 52);
            lv_obj_add_style(img_sod, &style_cont, 0);
            /*��ӿ��Ե����־*/
            lv_obj_add_flag(img_sod, LV_OBJ_FLAG_CLICKABLE);/*������*/
            /*��ӵ���¼��ص�����*/
            lv_obj_add_event_cb(img_sod, event_handler_sodclicked, LV_EVENT_CLICKED, (void*)id);
			if(j == 0)
			{
				lv_obj_align(img_sod, LV_ALIGN_TOP_LEFT, 26, 45 + i * 52);
			}
			else
			{
				lv_obj_align_to(img_sod, g_pt_lv_plant->bg_sod[id - 1], LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
			}
			id ++;
		}
	}

	/*������ǰ����ֵ����*/
    lv_obj_t *sun_label = lv_label_create(g_pt_lv_plant->bg_about);
    g_pt_lv_plant->sun_label = sun_label;
    lv_obj_set_style_text_font(sun_label, &song_font14, 0);
    lv_obj_set_style_text_color(sun_label, lv_color_hex(RED_HEX), 0);
    lv_label_set_text_fmt(sun_label, "%d", gTotal_sun);
    lv_obj_set_pos(sun_label, 20, 30);

    /*��ʼ��̫��*/
    CollectSunEvent_init();
    /*��ʼ���������ʼ״̬*/
    for(i = 0; i < 5 * 9;i++)
    {
        g_pt_lv_plant->bg_sodstate[i] = PLANT_NONE;
    }
    /*��ʼ������Ĺ���*/
    //g_pt_lv_plant->click_func = CLIK_FUNC_NONE;
    Set_ClickFunc(CLIK_FUNC_NONE);

    /*���ֲ��menu*/
    g_pt_lv_plant->menu_flower = lv_img_create(img_seed);
    lv_img_set_src(g_pt_lv_plant->menu_flower, &sunflowermenu);
    lv_obj_align(g_pt_lv_plant->menu_flower, LV_ALIGN_LEFT_MID, 0, 0);
    /*��ӿ��Ե����־*/
    lv_obj_add_flag(g_pt_lv_plant->menu_flower, LV_OBJ_FLAG_CLICKABLE);/*������*/
    /*��ӵ���¼��ص�����*/
    lv_obj_add_event_cb(g_pt_lv_plant->menu_flower, event_handler_menu_flowerclicked, LV_EVENT_CLICKED, NULL);
    /*����㶹*/

    g_pt_lv_plant->menu_gatl = lv_img_create(img_seed);
    lv_img_set_src(g_pt_lv_plant->menu_gatl, &Gatlingpeamenu);
    lv_obj_align_to(g_pt_lv_plant->menu_gatl, g_pt_lv_plant->menu_flower, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    /*��ӿ��Ե����־*/
    lv_obj_add_flag(g_pt_lv_plant->menu_gatl, LV_OBJ_FLAG_CLICKABLE);/*������*/
    /*��ӵ���¼��ص�����*/
    lv_obj_add_event_cb(g_pt_lv_plant->menu_gatl, event_handler_menu_gatlclicked, LV_EVENT_CLICKED, NULL);

    /*��Ӳ���*/
    g_pt_lv_plant->menu_shovel = lv_img_create(g_pt_lv_plant->bg_about);
    lv_img_set_src(g_pt_lv_plant->menu_shovel, &shovel);
    lv_obj_align_to(g_pt_lv_plant->menu_shovel, img_seed, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
    /*��ӿ��Ե����־*/
    lv_obj_add_flag(g_pt_lv_plant->menu_shovel, LV_OBJ_FLAG_CLICKABLE);/*������*/
    /*��ӵ���¼��ص�����*/
    lv_obj_add_event_cb(g_pt_lv_plant->menu_shovel, event_handler_menu_shovelclicked, LV_EVENT_CLICKED, NULL);

    /*����һ��̫����*/
    //CreateOneSunFlower(0);
    //CreateOneSunFlower(1);

}

/* Delete_theSunflower
 * ����λ��ɾ��̫����
 */
static void Delete_theSunflower(uint8_t block_id)
{
    SunFlower_event_t *temp = &gSunflower_event_head;
    SunFlower_event_t *need_del;
    need_del = temp->next;
    while(need_del)
    {
        if(need_del->block_id == block_id)/*�ҵ�Ҫɾ����̫����*/
        {
            if(need_del->sun_state == WAIT_CLICK)/*��Ҫɾ������*/
            {
                lv_obj_del(need_del->obj);
            }
            temp->next = need_del->next;/*ɾ������*/
            vPortFree(need_del);
            return;
        }
        temp = need_del;
        need_del = need_del->next;
    }

}

/* Delete_theGat
 * ����λ��ɾ���㶹����
 */
static void Delete_theGat(uint8_t block_id)
{
    Gatlingpea_t *temp = &gGat_event_head;
    Gatlingpea_t *need_del;
    need_del = temp->next;
    while(need_del)
    {
        if(need_del->block_id == block_id)/*�ҵ�Ҫɾ����̫����*/
        {
            if(need_del->gat_state == GAL_WAIT_CRASH)
            {

                need_del->gat_state = GAL_NEED_DEL;/*��ɾ������ȴ�,�ӵ���ײ����ɾ������*/
            }
            else
            {
                temp->next = need_del->next;/*ɾ������*/
                vPortFree(need_del);
            }
            return;
        }
        temp = need_del;
        need_del = need_del->next;
    }

}

/* Shovel_the_plant
 * ����ֲ��
 */
static void Shovel_the_plant(uint8_t block_id)
{
    lv_obj_t *child;
    if(g_pt_lv_plant->bg_sodstate[block_id] == PLANT_SUN)
    {
        Delete_theSunflower(block_id);
        /*ɾ��̫����*/
        child = lv_obj_get_child(g_pt_lv_plant->bg_sod[block_id], 0);
        lv_obj_del(child);
        g_pt_lv_plant->bg_sodstate[block_id] = PLANT_NONE;
    }
    else if(g_pt_lv_plant->bg_sodstate[block_id] == PLANT_GAT)
    {
        Delete_theGat(block_id);
        /*ɾ���㶹����*/
        child = lv_obj_get_child(g_pt_lv_plant->bg_sod[block_id], 0);
        lv_obj_del(child);
        g_pt_lv_plant->bg_sodstate[block_id] = PLANT_NONE;
    }

}



/* AddSunflowerEvent
 * ���̫�������¼���
 */
static void AddSunflowerEvent(uint8_t block_id)
{
    /*����һ������ڵ�*/
    SunFlower_event_t *newNode = (SunFlower_event_t*)pvPortMalloc(sizeof(SunFlower_event_t));
    if(newNode == NULL)
    {
        LOG_D("not have enough space");
        return;
    }
    newNode->block_id = block_id;/*λ����Ϣ*/
    newNode->next = NULL;
    newNode->gen_time = xTaskGetTickCount();/*����ʱ��*/
    newNode->exi_time = SUN_FLOWER_EXI_TIME;/*����ʱ��*/
    newNode->sun_state = WAIT_GEN;/*�ȴ���������*/
    SunFlower_event_t *temp = &gSunflower_event_head;
    printf("add sun flower\r\n");
    while(temp->next)
    {
        temp = temp->next;
    }
    /*��ӽڵ�*/
    temp->next = newNode;
}

/* CreateOneSunFlower
 * ����һ��̫����
 */
static void CreateOneSunFlower(uint8_t block_id)
{
    /*�ж�̫�������Ƿ�*/
    if(gTotal_sun < 50)
    {
        return;
    }
    if(g_pt_lv_plant->bg_sodstate[block_id] != PLANT_NONE)
    {
        printf("not is none\r\n");
        return;
    }
    lv_obj_t *flower = lv_img_create(g_pt_lv_plant->bg_sod[block_id]);
    lv_img_set_src(flower, &sunflower);
    lv_obj_center(flower);
    g_pt_lv_plant->bg_sodstate[block_id] = PLANT_SUN;
    gTotal_sun -= 50;
    lv_label_set_text_fmt(g_pt_lv_plant->sun_label, "%d", gTotal_sun);
    /*���̫�������¼���*/
    AddSunflowerEvent(block_id);
}



/**/

/* AddGatEvent
 * ����㶹���ֵ��¼���
 */
static void AddGatEvent(uint8_t block_id)
{
    /*����һ������ڵ�*/
    Gatlingpea_t *newNode = (Gatlingpea_t*)pvPortMalloc(sizeof(Gatlingpea_t));
    if(newNode == NULL)
    {
        LOG_D("not have enough space");
        return;
    }
    newNode->block_id = block_id;/*λ����Ϣ*/
    newNode->next = NULL;
    newNode->gat_state = GAL_WAIT_GEN;
    Gatlingpea_t *temp = &gGat_event_head;
    printf("add sun flower\r\n");
    while(temp->next)
    {
        temp = temp->next;
    }
    /*��ӽڵ�*/
    temp->next = newNode;
}

/* CreateOneGat
 * ����һ���㶹����
 */
static void CreateOneGat(uint8_t block_id)
{
    /*�ж�̫�������Ƿ�*/
    if(gTotal_sun < 100)
    {
        return;
    }
    if(g_pt_lv_plant->bg_sodstate[block_id] != PLANT_NONE)
    {
        printf("not is none\r\n");
        return;
    }
    lv_obj_t *gatling = lv_img_create(g_pt_lv_plant->bg_sod[block_id]);
    lv_img_set_src(gatling, &Gatlingpea);
    lv_obj_center(gatling);
    g_pt_lv_plant->bg_sodstate[block_id] = PLANT_GAT;
    gTotal_sun -= 100;
    lv_label_set_text_fmt(g_pt_lv_plant->sun_label, "%d", gTotal_sun);
    /*������ֵ�����*/
    AddGatEvent(block_id);
}

/* sun_anim_y_cb
 * �����ص�����
 */
static void sun_anim_y_cb(void *var, int32_t v)
{
    lv_obj_set_y(var, v);
}

/* sun_anim_y_cb
 * �����ص�����
 */
static void sun_anim_x_cb(void *var, int32_t v)
{
    lv_obj_set_x(var, v);
}

/* CreateOneSun
 * ϵͳ����һ������
 */
static void CreateOneSun(void)
{
    lv_obj_t *sun;

    srand((unsigned)time(NULL));
    uint16_t rand_x = rand() % (240);
    uint16_t rand_y = rand() % (460);

    //sun = lv_img_create(g_pt_lv_plant->bg_about);

    sun = lv_img_create(lv_scr_act());
    lv_img_set_src(sun, &Sun2);
    /*��ӿ��Ե����־*/
    lv_obj_add_flag(sun, LV_OBJ_FLAG_CLICKABLE);/*������*/
    /*��ӵ���¼��ص�����*/
    lv_obj_add_event_cb(sun, event_handler_sunclicked, LV_EVENT_CLICKED, NULL);

    rand_x += 20;
    rand_y += 45;
    if(rand_x > 410)
    {
        rand_x = 410;
    }
    if(rand_y > 265)
    {

        rand_y = 265;
    }
    lv_obj_set_pos(sun, rand_x, 40);
    /*��������*/
    /*����������ʽ*/
    lv_anim_t anim_sun;
    lv_anim_init(&anim_sun);
    lv_anim_set_var(&anim_sun, sun);
    lv_anim_set_time(&anim_sun, 2000);
    lv_anim_speed_to_time(10, 0, 2000);
    lv_anim_set_playback_time(&anim_sun, 0);
    lv_anim_set_repeat_count(&anim_sun, 1);
    lv_anim_set_path_cb(&anim_sun, lv_anim_path_ease_in_out);

    lv_anim_set_exec_cb(&anim_sun, sun_anim_y_cb);
    lv_anim_set_values(&anim_sun, 40, rand_y);
    lv_anim_start(&anim_sun);
    printf("create sun = %x\r\n", sun);

    CollectSunEvent_Add(sun);/*��ӵ�������*/
}





/* CollectSun
 * �ռ�һ��̫��,���һ������Ч��
 */
static void CollectSun(lv_obj_t *sun, uint8_t type)
{
    /*����������ʽ*/
    lv_anim_t anim_sun;
    lv_anim_init(&anim_sun);
    lv_anim_set_var(&anim_sun, sun);
    lv_anim_set_time(&anim_sun, 600);
    lv_anim_speed_to_time(10, 0, 600);
    lv_anim_set_playback_time(&anim_sun, 0);
    lv_anim_set_repeat_count(&anim_sun, 1);
    lv_anim_set_path_cb(&anim_sun, lv_anim_path_ease_in_out);

    lv_anim_set_exec_cb(&anim_sun, sun_anim_y_cb);
    lv_anim_set_values(&anim_sun, lv_obj_get_y(sun), COLLECT_SUN_Y);
    lv_anim_start(&anim_sun);

    lv_anim_t anim_sun_x;
    lv_anim_init(&anim_sun_x);
    lv_anim_set_var(&anim_sun_x, sun);
    lv_anim_set_time(&anim_sun_x, 600);
    lv_anim_speed_to_time(10, 0, 600);
    lv_anim_set_playback_time(&anim_sun_x, 0);
    lv_anim_set_repeat_count(&anim_sun_x, 1);

    lv_anim_set_exec_cb(&anim_sun_x, sun_anim_x_cb);
    lv_anim_set_values(&anim_sun_x, lv_obj_get_x(sun), COLLECT_SUN_X);
    lv_anim_start(&anim_sun_x);
    if(type == SUN_SYS_GEN)
    {
        /*�޸�����״̬,��Ϊ�ռ�״̬*/
        Sun_event_t *temp = gSun_event_head.next;
        while(temp)
        {
            if(temp->obj == sun)/*�ҵ�Ҫ�ռ�������*/
            {
                temp->sun_state = ALRE_CLICK;
                return;
            }
            temp = temp->next;
        }

    }
    else if(type == SUN_FLO_GEN)
    {
        lv_timer_pause(gManageFlowertimer);
        /*�޸�����״̬,��Ϊ�ռ�״̬*/
        SunFlower_event_t *temp = gSunflower_event_head.next;
        while(temp)
        {
            if(temp->obj == sun)/*�ҵ�Ҫ�ռ�������*/
            {
                temp->sun_state = ALRE_CLICK;
                lv_timer_resume(gManageFlowertimer);
                return;
            }
            temp = temp->next;
        }

    }
}




/* event_handler_right_move
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_sodclicked(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
    uint8_t sod_index = (uint8_t)lv_event_get_user_data(e);
	//lv_obj_t *obj = lv_event_get_current_target(e);

	if(code == LV_EVENT_CLICKED)/*���*/
	{
        printf("sod_index = %d\r\n", sod_index);
        //CreateOneSun();
        if(g_pt_lv_plant->click_func == CLIK_FUNC_FLOWER)
        {
            CreateOneSunFlower(sod_index);

        }
        else if(g_pt_lv_plant->click_func == CLIK_FUNC_GATLPEA)
        {

            CreateOneGat(sod_index);
        }
        else if(g_pt_lv_plant->click_func == CLIK_FUNC_SHOVEL)
        {

            Shovel_the_plant(sod_index);/*����ֲ��*/
        }

        Set_ClickFunc(CLIK_FUNC_NONE);/*��ֲ�����״̬*/
	}

}

/* event_handler_right_move
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_sunclicked(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	lv_obj_t *obj = lv_event_get_current_target(e);

	lv_obj_t *tag = lv_event_get_target(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
        //lv_obj_del(obj);
        //lv_anim_del_all();
        lv_anim_del(obj, NULL);/*ɾ������*/
        CollectSun(obj, SUN_SYS_GEN);
	}

}

/* event_handler_right_move
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_flowersunclicked(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	lv_obj_t *obj = lv_event_get_current_target(e);

	lv_obj_t *tag = lv_event_get_target(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
        //lv_obj_del(obj);
        //lv_anim_del_all();
        //lv_anim_del(obj, NULL);/*ɾ������*/
        CollectSun(obj, SUN_FLO_GEN);
	}

}


/* event_handler_right_move
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_menu_flowerclicked(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	lv_obj_t *obj = lv_event_get_current_target(e);

	lv_obj_t *tag = lv_event_get_target(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
        //lv_obj_del(obj);
        //lv_anim_del_all();
        Set_ClickFunc(CLIK_FUNC_FLOWER);

	}

}


/* event_handler_right_move
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_menu_shovelclicked(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	lv_obj_t *obj = lv_event_get_current_target(e);

	lv_obj_t *tag = lv_event_get_target(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
        //lv_obj_del(obj);
        //lv_anim_del_all();
        Set_ClickFunc(CLIK_FUNC_SHOVEL);

	}

}


/* event_handler_right_move
 * ���ذ�������¼��Ļص�����
 * */
static void event_handler_menu_gatlclicked(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	lv_obj_t *obj = lv_event_get_current_target(e);

	lv_obj_t *tag = lv_event_get_target(e);
	if(code == LV_EVENT_CLICKED)/*���*/
	{
        //lv_obj_del(obj);
        //lv_anim_del_all();
        Set_ClickFunc(CLIK_FUNC_GATLPEA);

	}

}

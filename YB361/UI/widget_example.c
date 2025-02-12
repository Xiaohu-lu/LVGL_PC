#include "UI/lv_typedef.h"
/* lvgl
 * */
#include "lvgl/lvgl.h"
#include "widget_example.h"
#include "Lamp_home.h"


extern lv_style_t bg_style;
extern lv_style_t bg_style_round;
extern lv_style_t bg_style_con;
extern lv_style_t con_style_on;
extern lv_style_t con_style_off;
extern lv_style_t circle_style;
extern lv_style_t circle_style_pitched;
extern lv_style_t label_con_style;
extern lv_style_t style_line;
extern lv_style_t style_line_2;


void example_acr_2(void)
{
    static lv_style_t bg_style_round;
    lv_style_init(&bg_style_round);
	lv_style_set_bg_opa(&bg_style_round, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&bg_style_round, 60);/*设置圆角*/
	lv_style_set_bg_color(&bg_style_round, lv_color_hex(0xffe58f));/*设置背景颜色,lv_color_hex()RGB888*/
	lv_style_set_pad_top(&bg_style_round, 0);
	lv_style_set_pad_bottom(&bg_style_round, 0);
	lv_style_set_pad_left(&bg_style_round, 0);
	lv_style_set_pad_right(&bg_style_round, 0);
	lv_style_set_border_width(&bg_style_round, 0);

	static lv_style_t style_con;
    lv_style_init(&style_con);
	lv_style_set_bg_opa(&style_con, LV_OPA_100);/*设置背景透明度*/
	lv_style_set_radius(&style_con, 60);/*设置圆角*/
	lv_style_set_bg_color(&style_con, lv_color_hex(0xffe58f));/*设置背景颜色,lv_color_hex()RGB888*/
	lv_style_set_pad_top(&style_con, 0);
	lv_style_set_pad_bottom(&style_con, 0);
	lv_style_set_pad_left(&style_con, 0);
	lv_style_set_pad_right(&style_con, 0);
	lv_style_set_border_width(&style_con, 0);
	lv_style_set_transform_angle(&style_con, 900);

	lv_obj_t *bg = lv_obj_create(lv_scr_act());
	lv_obj_set_size(bg, 320, 240);
	lv_obj_add_style(bg, &bg_style_round, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bg, lv_color_hex(0x000000), LV_PART_MAIN);

    lv_obj_t * arc = lv_arc_create(bg);
    lv_obj_set_size(arc, 142, 142);
    lv_arc_set_angles(arc, 0, 254);
    lv_arc_set_bg_angles(arc, 0, 254);
    lv_obj_set_style_arc_width(arc, 11, LV_PART_INDICATOR);/*设置前景弧宽度*/
    lv_obj_set_style_arc_width(arc, 11, LV_PART_MAIN);/*设置背景弧宽度*/
    lv_obj_set_style_arc_color(arc, lv_color_hex(0xffe58f), LV_PART_INDICATOR);/*设置前景弧颜色*/
    lv_obj_set_style_arc_color(arc, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_arc_set_mode(arc, LV_ARC_MODE_NORMAL);
    lv_arc_set_rotation(arc, 143);
    lv_arc_set_value(arc, 100);
    lv_obj_align(arc, LV_ALIGN_TOP_MID, 0, 49);

	/*child[1]*/
	lv_obj_t *con = lv_obj_create(bg);
	lv_obj_set_size(con, 7, 28);
	lv_obj_add_style(con, &bg_style_round, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_BOTTOM_MID, -50, -96);
	lv_obj_set_style_transform_angle(con, 2400, LV_PART_MAIN);

	/*child[2]*/
	con = lv_obj_create(bg);
	lv_obj_set_size(con, 7, 12);
	lv_obj_add_style(con, &bg_style_round, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_TOP_MID, 0, 62);

	/*child[3]*/
	con = lv_obj_create(bg);
	lv_obj_set_size(con, 7, 12);
	lv_obj_add_style(con, &bg_style_round, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_BOTTOM_MID, 50, -96);
	lv_obj_set_style_transform_angle(con, 1200, LV_PART_MAIN);

	/*child[3]*/
	con = lv_obj_create(bg);
	lv_obj_set_size(con, 7, 12);
	lv_obj_add_style(con, &bg_style_round, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_BOTTOM_MID, 0, -60);

	#if 0
	lv_obj_t *label = lv_label_create(bg);
	lv_obj_set_style_text_font(label, &Harm_san_34, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	//lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 100);
	//lv_obj_align(label, LV_ALIGN_TOP_LEFT, 143, 99);
	lv_obj_align(label, LV_ALIGN_TOP_LEFT, 134, 99);
	lv_label_set_text(label, "10");

	/*child[6]*/
	lv_obj_t *label_l = lv_label_create(bg);
	lv_obj_set_style_text_font(label_l, &Harm_san_24, LV_PART_MAIN);
	lv_obj_set_style_text_color(label_l, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align_to(label_l, label, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
	//lv_obj_align(label_l, LV_ALIGN_TOP_MID, 20, 100);
	lv_label_set_text(label_l, "S");
#endif // 0

#if 1
    lv_obj_t *label = lv_label_create(bg);
	lv_obj_set_style_text_font(label, &Harm_san_32, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 102);
	lv_label_set_text(label, "1档");
	#endif
}


static lv_point_t line_hor_points[] = {{0, 0}, {215, 0}};
static lv_point_t line_ver_points[] = {{0, 0}, {0, 96}};


/* Lamp_pose_draw_record_char
 * 坐姿记录画条形块
 * */
static void Lamp_pose_draw_record_char(lv_obj_t *parent, lv_coord_t x_offset, lv_coord_t y_offset, lv_color_t color, uint16_t value_unit, uint16_t value)
{
	lv_coord_t hor_size;
	float ratio;
	/*每个单位:29像素*/
	hor_size = 24 * (value / value_unit);
	ratio = (value % value_unit) / (value_unit * 1.0);
	hor_size += (lv_coord_t)(24 * ratio);
	lv_obj_t *chart = lv_obj_create(parent);
	lv_obj_add_style(chart, &bg_style, LV_PART_MAIN);
	lv_obj_set_size(chart, 10, hor_size);
	lv_obj_set_style_bg_color(chart, color, LV_PART_MAIN);
	lv_obj_align(chart, LV_ALIGN_BOTTOM_LEFT, x_offset, y_offset);
}

/* Lamp_pose_draw_record_mon
 * 绘制月报
 * */
static void Lamp_pose_draw_record_mon(lv_obj_t *parent, uint16_t *time_buf, uint16_t *count_buf, uint16_t time_unit, uint16_t count_uint)
{
	uint8_t i;
	/*第一周*/
	for(i = 0; i < 4; i++)
	{
		Lamp_pose_draw_record_char(parent, 60 + i * 58, -75, lv_color_hex(0x69b1ff), time_unit, time_buf[i]);
		Lamp_pose_draw_record_char(parent, 60 + i * 58 + 10, -75, lv_color_hex(0xffc069), count_uint, count_buf[i]);
	}
}

/* Lamp_pose_draw_record_week
 * 绘制周报
 * */
static void Lamp_pose_draw_record_week(lv_obj_t *parent, uint16_t *time_buf, uint16_t *count_buf, uint16_t time_unit, uint16_t count_uint)
{
	uint8_t i;
	for(i = 0; i < 7; i++)
	{
		Lamp_pose_draw_record_char(parent, 52 + i * 31, -75, lv_color_hex(0x69b1ff), time_unit, time_buf[i]);
		Lamp_pose_draw_record_char(parent, 52 + i * 31 + 10, -75, lv_color_hex(0xffc069), count_uint, count_buf[i]);
	}
}


static void Lamp_pose_draw_second_level_record_day(lv_obj_t *tab)
{
	/*获取第0的子对象*/
	lv_obj_t *bg = tab;
	#if 0
	/*显示标签"日报"*/
	lv_obj_t *label = lv_label_create(bg);
	lv_obj_set_style_text_font(label, &Harm_san_16, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -23);
	lv_label_set_text(label, "日报");
	/*显示圆圈*/

	lv_obj_t *circle = lv_obj_create(bg);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle, 8, 8);
	lv_obj_set_style_bg_opa(circle, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle, lv_color_hex(0xFFC53D), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
	lv_obj_align(circle, LV_ALIGN_BOTTOM_RIGHT, -72, -12);

	lv_obj_t *circle1 = lv_obj_create(bg);
	lv_obj_add_style(circle1, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle1, 8, 8);
	lv_obj_set_style_bg_opa(circle1, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle1, lv_color_hex(0x434343), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle1, 0, LV_PART_MAIN);
	lv_obj_align_to(circle1, circle, LV_ALIGN_OUT_RIGHT_BOTTOM, 6, 0);

	lv_obj_t *circle2 = lv_obj_create(bg);
	lv_obj_add_style(circle2, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle2, 8, 8);
	lv_obj_set_style_bg_opa(circle2, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle2, lv_color_hex(0x434343), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle2, 0, LV_PART_MAIN);
	lv_obj_align_to(circle2, circle1, LV_ALIGN_OUT_RIGHT_BOTTOM, 6, 0);
#endif
	/*创建大圆圈*/
	lv_obj_t *circle = lv_obj_create(bg);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle, 128, 128);
	lv_obj_set_style_border_color(circle, lv_color_hex(0x69B1FF), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle, 13, LV_PART_MAIN);
	lv_obj_align(circle, LV_ALIGN_TOP_LEFT, 37, 66);

	lv_obj_t *label = lv_label_create(circle);
	lv_obj_set_style_text_font(label, &Harm_san_26, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -54);
	lv_label_set_text(label, "3.8h");

	label = lv_label_create(circle);
	lv_obj_set_style_text_font(label, &Harm_san_18, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -30);
	lv_label_set_text(label, "学习时长");

    /*创建"请注意"标签*/
    label = lv_label_create(bg);
    lv_obj_set_style_text_font(label, &Harm_san_18, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_TOP_LEFT, 197, 68);
	lv_label_set_text(label, "请注意:");

    circle = lv_obj_create(bg);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle, 8, 8);
	lv_obj_set_style_bg_opa(circle, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle, lv_color_hex(0x95DE64), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
	lv_obj_align(circle, LV_ALIGN_TOP_LEFT, 198, 99);

    label = lv_label_create(bg);
    lv_obj_set_style_text_font(label, &Harm_san_18, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align_to(label, circle, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_label_set_text_fmt(label, "低头% 2d次", 4);


	lv_obj_t *circle1 = lv_obj_create(bg);
	lv_obj_add_style(circle1, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle1, 8, 8);
	lv_obj_set_style_bg_opa(circle1, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle1, lv_color_hex(0xFF7875), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle1, 0, LV_PART_MAIN);
	lv_obj_align_to(circle1, circle, LV_ALIGN_OUT_BOTTOM_MID, 0, 19);

	label = lv_label_create(bg);
    lv_obj_set_style_text_font(label, &Harm_san_18, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align_to(label, circle1, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_label_set_text_fmt(label, "歪头% 2d次", 5);

	circle = lv_obj_create(bg);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle, 8, 8);
	lv_obj_set_style_bg_opa(circle, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle, lv_color_hex(0xFFC069), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
	lv_obj_align_to(circle, circle1, LV_ALIGN_OUT_BOTTOM_MID, 0, 19);

	label = lv_label_create(bg);
    lv_obj_set_style_text_font(label, &Harm_san_18, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align_to(label, circle, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_label_set_text_fmt(label, "趴桌% 2d次", 2);

	circle1 = lv_obj_create(bg);
	lv_obj_add_style(circle1, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle1, 8, 8);
	lv_obj_set_style_bg_opa(circle1, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle1, lv_color_hex(0xFFF465), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle1, 0, LV_PART_MAIN);
	lv_obj_align_to(circle1, circle, LV_ALIGN_OUT_BOTTOM_MID, 0, 19);

	label = lv_label_create(bg);
    lv_obj_set_style_text_font(label, &Harm_san_18, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align_to(label, circle1, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_label_set_text_fmt(label, "不良% 2d次", 1);

}

static uint8_t Mon_Day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static void Lamp_pose_draw_second_level_record_week(lv_obj_t *tab)
{
	/*获取第0的子对象*/
	lv_obj_t *con = tab;
    /*(47,160)(47,67);(261,160)*/
	uint8_t i;
	lv_obj_t *line_h;
	lv_obj_t *line_v;
	/*增加单位设置标签*/
	lv_obj_t *label_h;
	lv_obj_t *label_c;
	uint8_t time_unit, count_unit;
	uint8_t start_mon, start_day;
	time_unit = 1;
	count_unit = 5;
	start_mon = 10;
	start_day = 28;

	/*画线*/
	for(i = 0; i < 5; i++)
	{
		line_h = lv_line_create(con);
		label_h = lv_label_create(con);
		label_c = lv_label_create(con);
		lv_obj_set_style_text_font(label_h, &Harm_san_12, LV_PART_MAIN);
		lv_obj_set_style_text_font(label_c, &Harm_san_12, LV_PART_MAIN);
		lv_obj_set_style_text_color(label_h, lv_color_hex(0xffffff), LV_PART_MAIN);
		lv_obj_set_style_text_color(label_c, lv_color_hex(0xffffff), LV_PART_MAIN);

		lv_obj_add_style(line_h, &style_line, LV_PART_MAIN);

		if(i == 0)
		{
			lv_label_set_text_fmt(label_h, "%d", time_unit * (i));
		}
		else{
			lv_label_set_text_fmt(label_h, "%dh", time_unit * (i));
		}
		lv_label_set_text_fmt(label_c, "%2d次", count_unit * (i));

		lv_line_set_points(line_h, line_hor_points, 2);

		lv_obj_align(line_h, LV_ALIGN_BOTTOM_LEFT, 47, -(75 + i * 24));

		lv_obj_align_to(label_h, line_h, LV_ALIGN_OUT_LEFT_MID, -4, 2);

        if((time_unit * i) >= 10){
            lv_obj_align_to(label_c, line_h, LV_ALIGN_OUT_RIGHT_MID, 4, 2);
        }
        else{
            lv_obj_align_to(label_c, line_h, LV_ALIGN_OUT_RIGHT_MID, 6, 2);
        }
	}
    for(i = 0; i < 7; i++){
        label_c = lv_label_create(con);
        lv_obj_set_style_text_font(label_c, &Harm_san_12, LV_PART_MAIN);
        lv_obj_set_style_text_color(label_c, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_label_set_text_fmt(label_c, "%d.%d", start_mon, start_day);
        lv_obj_align(label_c, LV_ALIGN_TOP_LEFT, 46 + i * 32 , 166);
        start_day++;
        if(start_day > Mon_Day[start_mon - 1]){
            start_day = 1;
            start_mon += 1;
        }
    }

	line_v = lv_line_create(con);
	lv_line_set_points(line_v, line_ver_points, 2);
	lv_obj_add_style(line_v, &style_line, 0);
	lv_obj_align(line_v, LV_ALIGN_BOTTOM_LEFT, 47, -75);

	line_v = lv_line_create(con);
	lv_line_set_points(line_v, line_ver_points, 2);
	lv_obj_add_style(line_v, &style_line, 0);
	lv_obj_align(line_v, LV_ALIGN_BOTTOM_LEFT, 262, -75);

	uint16_t time_buf[7] = {2, 3, 4, 3, 2, 1, 1};
	uint16_t count_buf[7] = {5, 6, 10, 11, 19, 20, 4};

	/*绘制条块*/
	Lamp_pose_draw_record_week(con, time_buf, count_buf, time_unit, count_unit);


	/*创建圆*/
	lv_obj_t *circle = lv_obj_create(con);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle, 8, 8);
	lv_obj_set_style_bg_opa(circle, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle, lv_color_hex(0x69b1ff), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
	lv_obj_align(circle, LV_ALIGN_TOP_LEFT, 49, 184);

    lv_obj_t *label = lv_label_create(con);
    lv_obj_set_style_text_font(label, &Harm_san_16, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align_to(label, circle, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_label_set_text(label, "学习时长 /h");

	circle = lv_obj_create(con);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle, 8, 8);
	lv_obj_set_style_bg_opa(circle, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle, lv_color_hex(0xffc069), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
	lv_obj_align(circle, LV_ALIGN_TOP_LEFT, 163, 184);

	label = lv_label_create(con);
    lv_obj_set_style_text_font(label, &Harm_san_16, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align_to(label, circle, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_label_set_text(label, "不良坐姿 /次");

}

static void Lamp_pose_draw_second_level_record_mon(lv_obj_t *tab)
{
	/*获取第0的子对象*/
	lv_obj_t *con = tab;
    /*(47,160)(47,67);(261,160)*/
	uint8_t i;
	lv_obj_t *line_h;
	lv_obj_t *line_v;
	/*增加单位设置标签*/
	lv_obj_t *label_h;
	lv_obj_t *label_c;
	uint8_t time_unit, count_unit;
	time_unit = 5;
	count_unit = 10;

	/*画线*/
	for(i = 0; i < 5; i++)
	{
		line_h = lv_line_create(con);
		label_h = lv_label_create(con);
		label_c = lv_label_create(con);
		lv_obj_set_style_text_font(label_h, &Harm_san_12, LV_PART_MAIN);
		lv_obj_set_style_text_font(label_c, &Harm_san_12, LV_PART_MAIN);
		lv_obj_set_style_text_color(label_h, lv_color_hex(0xffffff), LV_PART_MAIN);
		lv_obj_set_style_text_color(label_c, lv_color_hex(0xffffff), LV_PART_MAIN);

		lv_obj_add_style(line_h, &style_line, LV_PART_MAIN);

		if(i == 0)
		{
			lv_label_set_text_fmt(label_h, "%d", time_unit * (i));
		}
		else{
			lv_label_set_text_fmt(label_h, "%dh", time_unit * (i));
		}
		lv_label_set_text_fmt(label_c, "%2d次", count_unit * (i));

		lv_line_set_points(line_h, line_hor_points, 2);

		lv_obj_align(line_h, LV_ALIGN_BOTTOM_LEFT, 47, -(75 + i * 24));

		lv_obj_align_to(label_h, line_h, LV_ALIGN_OUT_LEFT_MID, -4, 2);

        if((time_unit * i) >= 10){
            lv_obj_align_to(label_c, line_h, LV_ALIGN_OUT_RIGHT_MID, 4, 2);
        }
        else{
            lv_obj_align_to(label_c, line_h, LV_ALIGN_OUT_RIGHT_MID, 6, 2);
        }
	}
    for(i = 0; i < 4; i++){
        label_c = lv_label_create(con);
        lv_obj_set_style_text_font(label_c, &Harm_san_12, LV_PART_MAIN);
        lv_obj_set_style_text_color(label_c, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        if(i == 0){
            lv_label_set_text(label_c, "第一周");
        }
        else if(i == 1){
            lv_label_set_text(label_c, "第二周");
        }
        else if(i == 2){
            lv_label_set_text(label_c, "第三周");
        }
        else if(i == 3){
            lv_label_set_text(label_c, "本周");
        }

        lv_obj_align(label_c, LV_ALIGN_TOP_LEFT, 54 + i * 58 , 166);
    }

	line_v = lv_line_create(con);
	lv_line_set_points(line_v, line_ver_points, 2);
	lv_obj_add_style(line_v, &style_line, 0);
	lv_obj_align(line_v, LV_ALIGN_BOTTOM_LEFT, 47, -75);

	line_v = lv_line_create(con);
	lv_line_set_points(line_v, line_ver_points, 2);
	lv_obj_add_style(line_v, &style_line, 0);
	lv_obj_align(line_v, LV_ALIGN_BOTTOM_LEFT, 262, -75);

	uint16_t time_buf[4] = {2, 9, 14, 20,};
	uint16_t count_buf[4] = {9, 29, 39, 40};
	/*绘制条块*/
	Lamp_pose_draw_record_mon(con, time_buf, count_buf, time_unit, count_unit);


	/*创建圆*/
	lv_obj_t *circle = lv_obj_create(con);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle, 8, 8);
	lv_obj_set_style_bg_opa(circle, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle, lv_color_hex(0x69b1ff), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
	lv_obj_align(circle, LV_ALIGN_TOP_LEFT, 49, 184);

    lv_obj_t *label = lv_label_create(con);
    lv_obj_set_style_text_font(label, &Harm_san_16, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align_to(label, circle, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_label_set_text(label, "学习时长 /h");

	circle = lv_obj_create(con);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle, 8, 8);
	lv_obj_set_style_bg_opa(circle, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle, lv_color_hex(0xffc069), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
	lv_obj_align(circle, LV_ALIGN_TOP_LEFT, 163, 184);

	label = lv_label_create(con);
    lv_obj_set_style_text_font(label, &Harm_san_16, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align_to(label, circle, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_label_set_text(label, "不良坐姿 /次");




}


void example_acr_3(void)
{


    /*线条样式*/
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, 1);
	lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));//6b6b6b
	lv_style_set_line_rounded(&style_line, true);

	lv_style_init(&style_line_2);
	lv_style_set_line_width(&style_line_2, 1);
	lv_style_set_line_color(&style_line_2, lv_palette_main(LV_PALETTE_GREY));//6b6b6b
	lv_style_set_line_rounded(&style_line_2, true);


	lv_obj_t *bg = lv_obj_create(lv_scr_act());
	lv_obj_set_size(bg, 320, 240);
	lv_obj_add_style(bg, &bg_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bg, lv_color_hex(0x000000), LV_PART_MAIN);


    /*坐姿统计标签,child[1]*/
	lv_obj_t *label = lv_label_create(bg);
	lv_obj_set_style_text_font(label, &Harm_san_20, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 28);
	lv_label_set_text(label, "坐姿统计");


	label = lv_label_create(bg);
	lv_obj_set_style_text_font(label, &Harm_san_16, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -23);
	lv_label_set_text(label, "月报");

	lv_obj_t *circle = lv_obj_create(bg);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle, 8, 8);
	lv_obj_set_style_bg_opa(circle, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle, lv_color_hex(0x434343), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
	lv_obj_align(circle, LV_ALIGN_BOTTOM_RIGHT, -72, -12);

	lv_obj_t *circle1 = lv_obj_create(bg);
	lv_obj_add_style(circle1, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle1, 8, 8);
	lv_obj_set_style_bg_opa(circle1, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle1, lv_color_hex(0x434343), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle1, 0, LV_PART_MAIN);
	lv_obj_align_to(circle1, circle, LV_ALIGN_OUT_RIGHT_BOTTOM, 6, 0);

	lv_obj_t *circle2 = lv_obj_create(bg);
	lv_obj_add_style(circle2, &circle_style, LV_PART_MAIN);
	lv_obj_set_size(circle2, 8, 8);
	lv_obj_set_style_bg_opa(circle2, LV_OPA_100, LV_PART_MAIN);
	lv_obj_set_style_bg_color(circle2, lv_color_hex(0xFFC53D), LV_PART_MAIN);
	lv_obj_set_style_border_width(circle2, 0, LV_PART_MAIN);
	lv_obj_align_to(circle2, circle1, LV_ALIGN_OUT_RIGHT_BOTTOM, 6, 0);


	/*tabview 58,200*/
	/*创建一个页签,child[2]*/
	lv_obj_t *tabview = lv_tabview_create(bg, LV_DIR_TOP, 0);
	lv_obj_add_style(tabview, &bg_style, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(tabview, LV_OPA_0, 0);

    lv_obj_t *day_tab = lv_tabview_add_tab(tabview, "day");
	lv_obj_add_style(day_tab, &bg_style, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(day_tab, LV_OPA_0, LV_PART_MAIN);
	lv_obj_set_size(day_tab, LAMP_DISP_HOR_RES, LAMP_DISP_VER_RES);

	lv_obj_t *week_tab = lv_tabview_add_tab(tabview, "week_tab");
	lv_obj_add_style(week_tab, &bg_style, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(week_tab, LV_OPA_0, LV_PART_MAIN);
	lv_obj_set_size(week_tab, LAMP_DISP_HOR_RES, LAMP_DISP_VER_RES);

	lv_obj_t *mon_tab = lv_tabview_add_tab(tabview, "mon_tab");
	lv_obj_add_style(mon_tab, &bg_style, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(mon_tab, LV_OPA_0, LV_PART_MAIN);
	lv_obj_set_size(mon_tab, LAMP_DISP_HOR_RES, LAMP_DISP_VER_RES);

	Lamp_pose_draw_second_level_record_day(day_tab);
	Lamp_pose_draw_second_level_record_week(week_tab);
	Lamp_pose_draw_second_level_record_mon(mon_tab);

	lv_tabview_set_act(tabview, 0, LV_ANIM_ON);
}




void example_chart_3(void)
{

    lv_obj_t *bg = lv_obj_create(lv_scr_act());
	lv_obj_set_size(bg, 320, 240);
	lv_obj_add_style(bg, &bg_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bg, lv_color_hex(0x000000), LV_PART_MAIN);


    lv_obj_t *chart = lv_chart_create(bg);

    lv_obj_set_size(chart, 200, 150);
    lv_obj_center(chart);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_range(chart, LV_CHART_AXIS_SECONDARY_Y, 0, 400);
    lv_chart_set_point_count(chart, 12);

    /*Add ticks and label to every axis*/
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 12, 3, true, 40);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 2, true, 50);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 3, 4, true, 50);

    /*Zoom in a little in X*/
    lv_chart_set_zoom_x(chart, 800);

}


extern Timer_List_s_t Timer_list_header;

static uint8_t id = 0;

/* Lamp_clock_second_level_Alarm_update_state
 * 更新闹钟的状态,开启还是关闭
 * */
static void Lamp_clock_second_level_Alarm_update_state(lv_obj_t *circle, uint8_t enable)
{
	lv_state_t state;
	state = lv_obj_get_state(circle);
	if(enable){
		if(!(state & LV_STATE_PRESSED)){
			lv_obj_set_size(circle, 22, 22);
			lv_obj_add_state(circle, LV_STATE_PRESSED);
			lv_obj_clear_flag(lv_obj_get_child(circle, 0), LV_OBJ_FLAG_HIDDEN);
		}
	}
	else{
		if(state & LV_STATE_PRESSED){
			lv_obj_set_size(circle, 14, 14);
			lv_obj_clear_state(circle, LV_STATE_PRESSED);
			lv_obj_add_flag(lv_obj_get_child(circle, 0), LV_OBJ_FLAG_HIDDEN);
		}
	}
}

static void Lamp_clock_second_level_Alarm_add_con(lv_obj_t *parent, uint8_t id)
{
    lv_obj_t *con = lv_obj_create(parent);
	lv_obj_set_size(con, 274, 38);
	lv_obj_add_style(con, &con_style_on, LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_add_style(con, &con_style_off, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_TOP_LEFT, 24, 4 + (51 * id));

}

static void Lamp_clock_second_level_Alarm_add_alarm(lv_obj_t *parent, uint8_t id, Timer_List_s_t *node)
{
	/*创建容器,child[0]*/
	lv_obj_t *con = lv_obj_create(parent);
	lv_obj_set_size(con, 274, 38);
	lv_obj_add_style(con, &con_style_on, LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_add_style(con, &con_style_off, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_TOP_LEFT, 24, 4 + (51 * id));
	/*时间标签,child[0]*/
	lv_obj_t *label = lv_label_create(con);
	lv_obj_set_style_text_font(label, &Harm_san_20, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_align(label, LV_ALIGN_LEFT_MID, 24, -1);
	lv_label_set_text_fmt(label, "%02d:%02d", node->timer_info.hour, node->timer_info.minute);

	/*创建圆圈,child[1]*/
	lv_obj_t *circle = lv_obj_create(con);
	lv_obj_set_size(circle, 14, 14);
	lv_obj_align(circle, LV_ALIGN_RIGHT_MID, -23, -1);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	lv_obj_add_style(circle, &circle_style_pitched, LV_PART_MAIN | LV_STATE_PRESSED);
	/*创建对号*/
	lv_obj_t *label_c = lv_label_create(circle);
	lv_obj_set_style_text_color(label_c, lv_color_hex(0xFFC53D), LV_PART_MAIN);
	lv_label_set_text(label_c, LV_SYMBOL_OK);
	lv_obj_center(label_c);
	lv_obj_add_flag(label_c, LV_OBJ_FLAG_HIDDEN);

	Lamp_clock_second_level_Alarm_update_state(circle, node->timer_info.alarm_open);

	/*设置ID*/
	node->timer_info.id = id;
}



void Lamp_alarm_page(void)
{
    lv_obj_t *bg = lv_obj_create(lv_scr_act());
	lv_obj_set_size(bg, 320, 240);
	lv_obj_add_style(bg, &bg_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bg, lv_color_hex(0x000000), LV_PART_MAIN);

    /*左上圆圈图片,child[0]*/
	lv_obj_t *circle = lv_obj_create(bg);
	lv_obj_set_size(circle, 33, 33);
	lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
	//lv_img_set_src(img_left, "S:/img_src/big_ico/count.png");
	lv_obj_align(circle, LV_ALIGN_TOP_LEFT, 21, 21);

    /*添加闹钟标签,child[1]*/
    lv_obj_t *label = lv_label_create(bg);
	lv_obj_set_style_text_font(label, &Harm_san_22, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_TOP_LEFT, 143, 28);
	lv_label_set_text(label, "闹钟");


	/* 创建一共容器,用来放闹钟信息
	 * child[2]
	 * */
	lv_obj_t *con_bg = lv_obj_create(bg);
	lv_obj_add_style(con_bg, &label_con_style, LV_PART_MAIN);
	lv_obj_set_size(con_bg, 320, 180);
	lv_obj_align(con_bg, LV_ALIGN_TOP_LEFT, 0, 60);

    lv_obj_set_scrollbar_mode(con_bg, LV_SCROLLBAR_MODE_OFF);
	Timer_List_s_t *head = Timer_list_header.next;
	while(head){
        Lamp_clock_second_level_Alarm_add_alarm(con_bg, id, head);
        id += 1;
        head = head->next;
	}


	/*添加闹钟选项*/
	lv_obj_t *con = lv_obj_create(con_bg);
	lv_obj_set_size(con, 274, 38);
	lv_obj_add_style(con, &con_style_on, LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_add_style(con, &con_style_off, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_TOP_LEFT, 24, 4 + (51 * id));
	/*时间标签,child[0]*/
	label = lv_label_create(con);
    lv_obj_center(label);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_label_set_text(label, LV_SYMBOL_PLUS);

    id += 1;
    /*返回选项*/
    con = lv_obj_create(con_bg);
	lv_obj_set_size(con, 274, 38);
	lv_obj_add_style(con, &con_style_on, LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_add_style(con, &con_style_off, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_TOP_LEFT, 24, 4 + (51 * id));
	/*时间标签,child[0]*/
	label = lv_label_create(con);
    lv_obj_center(label);
    lv_obj_set_style_text_font(label, &Harm_san_20, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_center(label);
	lv_label_set_text(label, "返回");
}

static lv_point_t line_ver_points1[] = {{0, 0}, {0, 25}};
static lv_point_t line_hor_points1[] = {{0, 0}, {206, 0}};
void Lamp_Alarm_Eidt_page(void)
{
    lv_obj_t *bg = lv_obj_create(lv_scr_act());
	lv_obj_set_size(bg, 320, 240);
	lv_obj_add_style(bg, &bg_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bg, lv_color_hex(0x000000), LV_PART_MAIN);

    /*创建标签,child[0]*/
    lv_obj_t *label = lv_label_create(bg);
    lv_obj_set_style_text_font(label, &Harm_san_36, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 83, 50);
    lv_label_set_text_fmt(label, "%02d", 17);
    /*child[1]*/
    lv_obj_t *label_l = lv_label_create(bg);
    lv_obj_set_style_text_font(label_l, &Harm_san_20, LV_PART_MAIN);
	lv_obj_set_style_text_color(label_l, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label_l, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_align_to(label_l, label, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 2);
    lv_label_set_text(label_l, "时");
    /*child[2]*/
    label = lv_label_create(bg);
    lv_obj_set_style_text_font(label, &Harm_san_36, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 175, 50);
    lv_label_set_text_fmt(label, "%02d", 30);
    /*child[3]*/
    label_l = lv_label_create(bg);
    lv_obj_set_style_text_font(label_l, &Harm_san_20, LV_PART_MAIN);
	lv_obj_set_style_text_color(label_l, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label_l, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_align_to(label_l, label, LV_ALIGN_OUT_RIGHT_BOTTOM, 2, 2);
    lv_label_set_text(label_l, "分");
    /*添加一条线*/
    /*child[4]*/
    lv_obj_t *line_v = lv_line_create(bg);
	lv_line_set_points(line_v, line_ver_points1, 2);
	lv_obj_add_style(line_v, &style_line, 0);
	lv_obj_set_style_line_width(line_v, 2, LV_PART_MAIN);
	lv_obj_set_style_line_color(line_v, lv_color_hex(0x212121), LV_PART_MAIN);
	lv_obj_align(line_v, LV_ALIGN_TOP_LEFT, 159, 51);

	/*创建容器child[5]*/
	lv_obj_t *con = lv_obj_create(bg);
	lv_obj_set_size(con, 216, 24);
	lv_obj_add_style(con, &bg_style, LV_PART_MAIN);
	lv_obj_set_style_opa(con, LV_OPA_100, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_TOP_LEFT, 55, 98);
	/*child[0]*/
	label = lv_label_create(con);
	lv_obj_set_style_text_font(label, &Harm_san_18, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFEC83E), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_align(label, LV_ALIGN_BOTTOM_LEFT, 0, -2);
    lv_label_set_text(label, "晨光唤醒");
    /*创建圆圈,chidl[1]*/
    lv_obj_t *circle = lv_obj_create(con);
    lv_obj_set_size(circle, 14, 14);
    lv_obj_align(circle, LV_ALIGN_RIGHT_MID, -12, 0);
    lv_obj_add_style(circle, &circle_style, LV_PART_MAIN);
    lv_obj_add_style(circle, &circle_style_pitched, LV_PART_MAIN | LV_STATE_PRESSED);
    /*创建对号*/
    lv_obj_t *label_c = lv_label_create(circle);
    lv_obj_set_style_text_color(label_c, lv_color_hex(0xFFC53D), LV_PART_MAIN);
    lv_label_set_text(label_c, LV_SYMBOL_OK);
    lv_obj_center(label_c);
    lv_obj_add_flag(label_c, LV_OBJ_FLAG_HIDDEN);

    /*创建线条*/
    lv_obj_t *line_h = lv_line_create(con);
	lv_line_set_points(line_h, line_hor_points1, 2);
	lv_obj_add_style(line_h, &style_line, 0);
	lv_obj_set_style_line_width(line_h, 2, LV_PART_MAIN);
	lv_obj_set_style_line_color(line_h, lv_color_hex(0x282828), LV_PART_MAIN);
	lv_obj_align(line_h, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    /*创建容器,child[6]*/
    con = lv_obj_create(bg);
	lv_obj_set_size(con, 216, 24);
	lv_obj_add_style(con, &bg_style, LV_PART_MAIN);
	lv_obj_set_style_opa(con, LV_OPA_100, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_TOP_LEFT, 55, 134);
	/*chidl[0]*/
	label = lv_label_create(con);
	lv_obj_set_style_text_font(label, &Harm_san_18, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFEC83E), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_align(label, LV_ALIGN_BOTTOM_LEFT, 0, -2);
    lv_label_set_text(label, "重复");
    /*创建RIGHT符号,child[1]*/
    lv_obj_t *label_r = lv_label_create(con);
    lv_obj_align(label_r, LV_ALIGN_RIGHT_MID, -12, 0);
    lv_obj_set_style_text_color(label_r, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_label_set_text(label_r, LV_SYMBOL_RIGHT);

    line_h = lv_line_create(con);
	lv_line_set_points(line_h, line_hor_points1, 2);
	lv_obj_add_style(line_h, &style_line, 0);
	lv_obj_set_style_line_width(line_h, 2, LV_PART_MAIN);
	lv_obj_set_style_line_color(line_h, lv_color_hex(0x282828), LV_PART_MAIN);
	lv_obj_align(line_h, LV_ALIGN_BOTTOM_LEFT, 0, 0);

	/*创建容器,child[7]*/
    con = lv_obj_create(bg);
	lv_obj_set_size(con, 216, 24);
	lv_obj_add_style(con, &bg_style, LV_PART_MAIN);
	lv_obj_set_style_opa(con, LV_OPA_100, LV_PART_MAIN);
	lv_obj_align(con, LV_ALIGN_TOP_LEFT, 55, 170);
	label = lv_label_create(con);
	lv_obj_set_style_text_font(label, &Harm_san_18, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFEC83E), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_align(label, LV_ALIGN_BOTTOM_LEFT, 0, -2);
    lv_label_set_text(label, "铃声");
    /*创建RIGHT符号,child[1]*/
    label_r = lv_label_create(con);
    lv_obj_align(label_r, LV_ALIGN_RIGHT_MID, -12, 0);
    lv_obj_set_style_text_color(label_r, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_label_set_text(label_r, LV_SYMBOL_RIGHT);


    line_h = lv_line_create(con);
	lv_line_set_points(line_h, line_hor_points1, 2);
	lv_obj_add_style(line_h, &style_line, 0);
	lv_obj_set_style_line_width(line_h, 2, LV_PART_MAIN);
	lv_obj_set_style_line_color(line_h, lv_color_hex(0x282828), LV_PART_MAIN);
	lv_obj_align(line_h, LV_ALIGN_BOTTOM_LEFT, 0, 0);


}



#define BOARD_SIZE          15
#define GRID_WIDTH          16
#define BOARD_START_X       48
#define BOARD_START_Y       8

enum GameState{
    PLAYING,
    BLACK_WIN,
    WHITE_WIN,
};

static lv_obj_t *canvas;
static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(320, 240)];
static uint8_t board[BOARD_SIZE][BOARD_SIZE] = {0};/*0:空,1:黑棋,2:白棋*/
static bool current_player = 0; /*0:黑方,1:白方*/
static enum GameState game_state = PLAYING;


/* 绘制棋盘网格
 */
void draw_board(void)
{
    uint16_t i;
    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.color = lv_color_hex(0x808080);
    line_dsc.width = 1;

    /*绘制横线*/
    for(i = 0; i < BOARD_SIZE; i++){
        lv_point_t points[2] = {
            {BOARD_START_X, BOARD_START_Y+i * GRID_WIDTH},
            {BOARD_START_X +(BOARD_SIZE - 1) * GRID_WIDTH, BOARD_START_Y + i * GRID_WIDTH}
        };
        lv_canvas_draw_line(canvas, points, 2, &line_dsc);
    }

    /*绘制竖线*/
    for(i = 0; i < BOARD_SIZE; i++){
        lv_point_t points[2] = {
            {BOARD_START_X + i * GRID_WIDTH, BOARD_START_Y},
            {BOARD_START_X + i *GRID_WIDTH, BOARD_START_Y + (BOARD_SIZE - 1) * GRID_WIDTH}
        };
        lv_canvas_draw_line(canvas, points, 2, &line_dsc);
    }
}

/* 绘制棋子
 */
void draw_piece(uint8_t x, uint8_t y, bool is_black)
{
    lv_draw_arc_dsc_t arc_dsc;
    lv_draw_arc_dsc_init(&arc_dsc);
    arc_dsc.color = is_black ? lv_color_black() : lv_color_white();
    arc_dsc.width = 2;

    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = is_black ? lv_color_black() :lv_color_white();
    rect_dsc.radius = LV_RADIUS_CIRCLE;

    lv_area_t area = {
        BOARD_START_X + x * GRID_WIDTH - GRID_WIDTH / 2,
        BOARD_START_Y + y * GRID_WIDTH - GRID_WIDTH / 2,
        BOARD_START_X + x * GRID_WIDTH + GRID_WIDTH / 2,
        BOARD_START_Y + y * GRID_WIDTH + GRID_WIDTH / 2,
    };
    lv_coord_t start_x, start_y;
    start_x =  BOARD_START_X + x * GRID_WIDTH - GRID_WIDTH / 2;
    start_y = BOARD_START_Y + y * GRID_WIDTH - GRID_WIDTH / 2;

    lv_canvas_draw_rect(canvas, start_x, start_y, GRID_WIDTH, GRID_WIDTH, &rect_dsc);
}

/* 检查胜利条件
 */
bool check_win(uint8_t x, uint8_t y)
{
    uint8_t player = board[x][y];
    int dx[4] = {1, 0, 1, 1};
    int dy[4] = {0, 1, 1, -1};

    uint8_t i;

    for(i = 0; i < 4; i++){
        int count = 1;
        int tx = x + dx[i];
        int ty = y + dy[i];

        while(tx >= 0 && tx < BOARD_SIZE && \
               ty >= 0 && ty < BOARD_SIZE && \
               board[tx][ty] == player){
            count++;
            tx += dx[i];
            ty += dy[i];
        }
        tx = x - dx[i];
        ty = y - dy[i];
        while(tx >= 0 && tx < BOARD_SIZE && \
              ty >= 0 && ty < BOARD_SIZE && \
              board[tx][ty] == player){
            count++;
            tx -= dx[i];
            ty -= dy[i];
        }
        if(count >= 5) return true;
    }
    return false;

}

/* 点击事件处理
 */
static void event_handler(lv_event_t *e)
{
    printf("cicked");
    lv_point_t p;
    if(game_state != PLAYING) return;
    lv_indev_t *indev = lv_indev_get_act();
    lv_indev_get_point(indev, &p);

    //转换坐标到棋盘位置
    int x = (p.x - BOARD_START_X + GRID_WIDTH / 2) / GRID_WIDTH;
    int y = (p.y - BOARD_START_Y + GRID_WIDTH / 2) / GRID_WIDTH;

    if(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
        return;
    if(board[x][y] != 0)
        return;

    //更新棋盘状态
    board[x][y] = current_player + 1;
    draw_piece(x, y, current_player);

    /*检查胜利*/
    if(check_win(x, y)){
        game_state = current_player ? WHITE_WIN : BLACK_WIN;
        return;
    }
    current_player = !current_player;
}

void gomoku_init(void)
{
    /*创建画布*/
    canvas = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(canvas, cbuf, 320, 240, LV_IMG_CF_TRUE_COLOR);
    lv_obj_add_flag(canvas, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(canvas, event_handler, LV_EVENT_CLICKED, NULL);

    /*初始化棋盘*/
    draw_board();

    /*初始化游戏状态*/
    memset(board, 0, sizeof(board));
    current_player = 0;
    game_state = PLAYING;
}



static lv_point_t line_points1[] = {{0, 0}, {154, 0}};

void Lamp_clock_draw_time_page(void)
{
    lv_obj_t *bg = lv_obj_create(lv_scr_act());
	lv_obj_set_size(bg, 320, 240);
	lv_obj_add_style(bg, &bg_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bg, lv_color_hex(0x000000), LV_PART_MAIN);

    /*显示星期几,child[0]*/
    lv_obj_t *label = lv_label_create(bg);
	lv_obj_set_style_text_font(label, &Harm_san_32, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFEC83E), LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 70);
	lv_label_set_text(label, "星期五");

    /*chidl[1]*/
	label = lv_label_create(bg);
	lv_obj_set_style_text_font(label, &Harm_san_36, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFEC83E), LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_align(label, LV_ALIGN_TOP_LEFT, 83, 132);
	lv_label_set_text_fmt(label, "%02d", 17);
	/*child[2]*/
	lv_obj_t *label_l = lv_label_create(bg);
	lv_obj_set_style_text_font(label_l, &Harm_san_20, LV_PART_MAIN);
	lv_obj_set_style_text_color(label_l, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label_l, lv_color_hex(0xFEC83E), LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_align_to(label_l, label, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 2);
	lv_label_set_text(label_l, "时");
	/*child[3]*/
	label = lv_label_create(bg);
	lv_obj_set_style_text_font(label, &Harm_san_36, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(0xFEC83E), LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_align(label, LV_ALIGN_TOP_LEFT, 175, 132);
	lv_label_set_text_fmt(label, "%02d", 30);
	/*child[4]*/
	label_l = lv_label_create(bg);
	lv_obj_set_style_text_font(label_l, &Harm_san_20, LV_PART_MAIN);
	lv_obj_set_style_text_color(label_l, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(label_l, lv_color_hex(0xFEC83E), LV_PART_MAIN | LV_STATE_PRESSED);
	lv_obj_align_to(label_l, label, LV_ALIGN_OUT_RIGHT_BOTTOM, 2, 2);
	lv_label_set_text(label_l, "分");

    /*创建线条child[5]*/
    lv_obj_t *line = lv_line_create(bg);
	lv_line_set_points(line, line_points1, 2);
	lv_obj_add_style(line, &style_line, 0);
	lv_obj_set_style_line_width(line, 2, LV_PART_MAIN);
	lv_obj_set_style_line_color(line, lv_color_hex(0x565656), LV_PART_MAIN);
	lv_obj_align(line, LV_ALIGN_TOP_LEFT, 83, 113);
}


void lv_example_flex_1(void)
{
    lv_obj_t *cont_row = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont_row, 300, 75);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);

    lv_obj_t *cont_col = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont_col, 200, 150);
    lv_obj_align_to(cont_col, cont_row, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);


    uint32_t i;
    for(i = 0; i < 10; i++){
        lv_obj_t *obj;
        lv_obj_t *label;

        obj = lv_btn_create(cont_row);
        lv_obj_set_size(obj, 100, LV_PCT(100));

        label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "Item: %u", i);
        lv_obj_center(label);

        obj = lv_btn_create(cont_col);
        lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);

        label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "Item: %"LV_PRIu32, i);
        lv_obj_center(label);
    }


}


void lv_example_grid_1(void)
{
    static lv_coord_t col_dsc[] = {90, 90, 90, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {100, 100, 100, LV_GRID_TEMPLATE_LAST};

    /*create a container width grid*/
    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 320, 320);
    lv_obj_center(cont);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    lv_obj_t *label;
    lv_obj_t *obj;

    uint32_t i;
    for(i = 0; i <9; i++){
        uint8_t col = i % 3;
        uint8_t row = i / 3;

        obj = lv_btn_create(cont);
        lv_obj_set_size(obj, 100, 100);
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_START, col, 1, LV_GRID_ALIGN_START, row, 1);
        label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "c%d， r%d", col, row);
        lv_obj_center(label);

    }



}


void lv_example_grid_2(void)
{

    static lv_coord_t col_dsc[] = {70, 70, 70, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {50, 50, 50, LV_GRID_TEMPLATE_LAST};

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_size(cont, 300, 220);
    lv_obj_center(cont);

    lv_obj_t *label;
    lv_obj_t *obj;
    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 0, 1);
    label = lv_label_create(obj);
    lv_label_set_text(label, "c0, r0");

    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    label = lv_label_create(obj);
    lv_label_set_text(label, "c1, r0");

    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_END, 0, 1);
    label = lv_label_create(obj);
    lv_label_set_text(label, "c2, r0");

    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 1, 2, LV_GRID_ALIGN_STRETCH, 1, 1);
    label = lv_label_create(obj);
    lv_label_set_text(label, "c1-2, r1");

    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 2);
    label = lv_label_create(obj);
    lv_label_set_text(label, "c0\nr1-2");





}






#define PUZZLE_SIZE			3	/*3*3拼图*/
#define TILE_SIZE			100	/*每个拼图块大小*/
#define GRID_GAP			5	/*网格间距*/


static lv_obj_t *tiles[PUZZLE_SIZE][PUZZLE_SIZE];
static uint8_t puzzle[PUZZLE_SIZE][PUZZLE_SIZE];
static lv_coord_t blank_row, blank_col;


/* shuffle_puzzle
 * 生成可解随机拼图
 * */
void shuffle_puzzle(void)
{
	uint16_t i;
	uint32_t j;
	uint32_t temp;
	uint8_t numbers[PUZZLE_SIZE * PUZZLE_SIZE - 1];
	for(i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE - 1; i++){
		numbers[i] = i + 1;
	}

	/*Fisher-Yates洗牌算法*/
	for(i = PUZZLE_SIZE * PUZZLE_SIZE - 2; i > 0; i--){
		j = lv_rand(0, i);
		temp = numbers[i];
		numbers[i] = numbers[j];
		numbers[j] = temp;
	}

	/*填充拼图并记录空白位置*/
	uint32_t index = 0;
	for(i = 0; i < PUZZLE_SIZE; i++){
		for(j = 0; j < PUZZLE_SIZE; j++){
			if(index < PUZZLE_SIZE * PUZZLE_SIZE - 1){
				puzzle[i][j] = numbers[index++];
				//tiles[i][j] = lv_btn_create(lv_scr_act());
			}
			else{
				puzzle[i][j] = 0;
				blank_row = i;
				blank_col = j;
				tiles[i][j] = NULL;/*空白不需要创建按钮*/
			}
		}
	}
	for(i = 0; i < PUZZLE_SIZE; i++){
        for(j = 0; j < PUZZLE_SIZE; j++){
            printf("%d ",puzzle[i][j]);
        }
        printf("\r\n");
	}
}

/* is_valid_move
 * 检查移动是否合法
 * */
static bool is_valid_move(int row, int col)
{
	return((abs(row - blank_row) == 1 && col == blank_col) ||
			(abs(col - blank_col) == 1 && row == blank_row));
}


/* check_victory
 * */
void check_victory(void)
{
	uint8_t i, j;
	int expected = 1;
	for(i = 0; i < PUZZLE_SIZE; i++){
		for(j = 0; j < PUZZLE_SIZE; j++){
			if(i == PUZZLE_SIZE - 1 && j == PUZZLE_SIZE - 1){
				if(puzzle[i][j] != 0)
					return;
			}
			else{
				if(puzzle[i][j] != expected++)
					return;
			}
		}
	}

	/*显示胜利提示*/
	lv_obj_t *msg = lv_msgbox_create(NULL, "胜利", "拼图完成", NULL, true);
	lv_obj_center(msg);
}

/* tile_event_handler
 * btn按键点击事件回调函数
 * */
static void tile_event_handler(lv_event_t *e)
{
	uint8_t i, j;
	lv_obj_t *tile = lv_event_get_target(e);
	int tile_num = (int)lv_obj_get_user_data(tile);

	/*查找点击的拼图位置*/
	int tile_row = -1, tile_col = -1;
	for(i = 0; i < PUZZLE_SIZE; i++){
		for(j = 0; j < PUZZLE_SIZE; j++){
			if(puzzle[i][j] == tile_num){
				tile_row = i;
				tile_col = j;
				break;
			}
		}
	}

	printf("tile_row = %d, tile_col = %d\r\n", tile_row, tile_col);
	if(is_valid_move(tile_row, tile_col)){
		/*交换拼图块位置*/
		puzzle[blank_row][blank_col] = tile_num;
		puzzle[tile_row][tile_col] = 0;

		/*计算目标坐标*/
		lv_coord_t target_x = 5 + blank_col * 105;
		lv_coord_t target_y = 85 + blank_row * 105;

		/*创建移动动画*/
		lv_anim_t a;
		lv_anim_init(&a);
		lv_anim_set_var(&a, tile);
		lv_anim_set_time(&a, 200);
		lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

		lv_anim_set_values(&a, lv_obj_get_x(tile), target_x);
		lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
		lv_anim_start(&a);

		lv_anim_set_values(&a, lv_obj_get_y(tile), target_y);
		lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
		lv_anim_start(&a);

		/*更新空白位置*/
		blank_row = tile_row;
		blank_col = tile_col;
		/*检查胜利条件*/
		check_victory();
	}
}




/* puzzle_init
 * 初始化拼图界面
 * */
void puzzle_init(void)
{
	uint8_t i, j;
	/*创建拼图块*/
	shuffle_puzzle();

	#if 1
	for(i = 0; i < PUZZLE_SIZE; i++){
		for(j = 0; j < PUZZLE_SIZE; j++){
			if(puzzle[i][j] != 0){
				lv_obj_t *btn = lv_btn_create(lv_scr_act());
				lv_obj_set_size(btn, TILE_SIZE, TILE_SIZE);
				lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 5 + j * 105, 85 + i * 105);
				lv_obj_add_event_cb(btn, tile_event_handler, LV_EVENT_CLICKED, NULL);
				lv_obj_set_user_data(btn, (void*)puzzle[i][j]);
				/*添加数字标签*/
				lv_obj_t *label = lv_label_create(btn);
				lv_label_set_text_fmt(label, "%d", puzzle[i][j]);
				lv_obj_center(label);
				tiles[i][j] = btn;
			}
			#if 0
			else{
                lv_obj_t *btn = lv_btn_create(lv_scr_act());
				lv_obj_set_size(btn, TILE_SIZE, TILE_SIZE);
				lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
				lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 5 + j * 105, 85 + i * 105);
				lv_obj_add_event_cb(btn, tile_event_handler, LV_EVENT_CLICKED, NULL);
				lv_obj_set_user_data(btn, (void*)puzzle[i][j]);

				tiles[i][j] = btn;
			}
			#endif
		}
	}
#endif

}




#define GRID_SIZE           4

static uint16_t grid[GRID_SIZE][GRID_SIZE];/*存储数字*/
static uint32_t score = 0;
static lv_obj_t *score_label;
static lv_obj_t * tiles1[GRID_SIZE][GRID_SIZE];

static lv_style_t tile_style;
lv_anim_t a;


/* 生成新数字(90%概率生成2,10%概率生成4)
 */
void spawn_new_tile(void)
{

    lv_coord_t pos_x, pos_y;
    do{
        pos_x = lv_rand(0, GRID_SIZE - 1);
        pos_y = lv_rand(0, GRID_SIZE - 1);
    }while(grid[pos_y][pos_x] != 0);

    uint16_t value = (lv_rand(0, 9) < 9) ? 2 : 4;
    grid[pos_y][pos_x] = value;

    /*创建LVGL对象*/
    lv_obj_t *tile = lv_label_create(board);
    lv_label_set_text_fmt(tile, "%d", value);
    lv_obj_add_style(tile, &tile_style, 0);
    lv_obj_set_size(tile, 65, 65);
    lv_obj_align(tile, LV_ALIGN_TOP_LEFT, pos_x * 70, pos_y *70);
    tiles[pos_y][pos_x] = tile;
}



void move_left(void)
{
    uint8_t y, x;

    for(y = 0; y < GRID_SIZE; y++){
        int write_pos = 0;
        uint16_t last_value = 0;
        for(x = 0; x < GRID_SIZE; x++){
            if(grid[y][x] == 0)
                continue;
            if(last_value == grid[y][x]){
                /*合并方块*/
                grid[y][write_pos-1] *= 2;
                score += grid[y][write_pos - 1];
                lv_label_set_text_fmt(tiles[y][write_pos - 1], "%d", grid[y][write_pos - 1]);
                lv_obj_add_flag(tiles[y][x], LV_OBJ_FLAG_HIDDEN);
                grid[y][x] = 0;
                last_value = 0;
            }
            else{
                /*移动方块*/
                if(x != write_pos){
                    grid[y][write_pos] = grid[y][x];
                    grid[y][x] = 0;
                    /*触发移动动画*/
                    lv_anim_set_var(&a, tiles[y][x]);
                    lv_anim_set_values(&a, lv_obj_get_x(tiles[y][x]), write_pos * 70);
                    lv_anim_start(&a);
                }
                last_value = grid[y][write_pos];
                write_pos++;
            }
        }
    }

}






void game_2048_init(void)
{

    /*创建棋盘容器*/
    lv_obj_t *board = lv_obj_create(lv_scr_act());
    lv_obj_set_size(board, 300, 300);
    lv_obj_align(board, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_set_style_bg_color(board, lv_color_hex(0xBBADA0), 0);
    lv_obj_set_style_pad_all(board, 10, 0);

    /*创建分数显示*/
    score_label = lv_label_create(lv_scr_act());
    lv_label_set_text_fmt(score_label, "Score: %d", score);
    lv_obj_align(score_label, LV_ALIGN_TOP_LEFT, 10, 10);


    /*动态生成数字块样式*/

    lv_style_init(&tile_style);
    lv_style_set_radius(&tile_style, 5);
    //lv_style_set_bg_color(&tile_style, get_tile_color(value));
    lv_style_set_text_font(&tile_style, &lv_font_montserrat_20);

    /*滑动动画配置*/

    lv_anim_init(&a);
    lv_anim_set_time(&a, 200);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

}








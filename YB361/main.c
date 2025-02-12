
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <unistd.h>

#include "lvgl/lvgl.h"
//#include "lv_100ask_teach_demos/lv_100ask_teach_demos.h"
#include "lv_demos/src/lv_demo_widgets/lv_demo_widgets.h"
#include "lv_drivers/win32drv/win32drv.h"

#include <windows.h>
#include "UI/lv_typedef.h"
#include "lv_main_page.h"
#include "Lamp_main.h"
#include "Lamp_home.h"
#include "Lamp_audio.h"
#include "Lamp_power.h"
#include "Lamp_light.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void *data);

/**********************
 *  STATIC VARIABLES
 **********************/
HINSTANCE ghInstance;
/**********************
 *      MACROS
 **********************/

void SetDispRotain(uint8_t rotain)
{
    if(rotain == 1)
    {

        lv_win32_init(ghInstance, SW_SHOWNORMAL, 480, 320, NULL);
    }
    else
    {
        lv_win32_init(ghInstance, SW_SHOWNORMAL, 320, 480, NULL);
    }
}
/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
    /*Initialize LittlevGL*/
    lv_init();

    /*Initialize the HAL for LittlevGL*/
    //lv_win32_init(hInstance, SW_SHOWNORMAL, 320, 240, NULL);
    lv_win32_init(hInstance, SW_SHOWNORMAL, 320, 480, NULL);
    printf("hInstance size = %d,\r\n", sizeof(hInstance));
    ghInstance = hInstance;
    printf("hInstanc = %x, ghInstance = %x\r\n", hInstance, ghInstance);
    /*Output prompt information to the console, you can also use printf() to print directly*/
    LV_LOG_USER("LVGL initialization completed!");
    //printf("www.100ask.net: Lvgl initialization complete!\n");

    //param_init();
    add_some_data();
    Lamp_Timer_Test();
    //anim_show_1();
    //anim_show_2();
    //lv_main_page_demo(1000);
    //lv_example_meter_4();
    //lv_example_acr_1();
    //lv_example_table_2();
    //lv_example_roller_1();
    //lv_gif_example_1();
    //lv_wait_ainmation(2000);
    //lv_plant_game();
    //anim_sun_test1();
    //anim_show_2();
    //anim_show_1();
    //lamp_main_page_init();
    //lv_example_chart_3();
    //lv_example_align_test();
    //lamp_pose_record_day_test();
    //lv_example_label_4();
    //lv_example_acr_1();
    //Lamp_home_init();
    Lamp_Style_Init();
    //Lamp_audio_volume_page();
    //Lamp_light_init();
    //lamp_pose_record_day_test();
    //lv_example_label_4();
//example_acr_3();
    //example_chart_3();
    //lv_example_acr_1();

//Lamp_Alarm_Eidt_page();
//gomoku_init();
//Lamp_clock_draw_time_page();
//lv_example_grid_2();
puzzle_init();
//game_2048_init();
    while(!lv_win32_quit_signal) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();
        usleep(10000);       /*Just to let the system breath*/
    }
    return 0;
}

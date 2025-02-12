/*******************************************************************************
 * Size: 10 px
 * Bpp: 4
 * Opts: --bpp 4 --size 10 --no-compress --font HarmonyOS_Sans_SC_Regular.ttf --symbols 0123456789. --format lvgl -o Harm_san_10.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef HARM_SAN_10
#define HARM_SAN_10 1
#endif

#if HARM_SAN_10

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+002E "." */
    0x0, 0x49,

    /* U+0030 "0" */
    0x7, 0xcc, 0x30, 0x2c, 0x2, 0xd0, 0x68, 0x0,
    0xc2, 0x77, 0x0, 0xb3, 0x68, 0x0, 0xc2, 0x2c,
    0x2, 0xd0, 0x7, 0xcc, 0x30,

    /* U+0031 "1" */
    0x2, 0xac, 0xc, 0x5c, 0x0, 0x1c, 0x0, 0x1c,
    0x0, 0x1c, 0x0, 0x1c, 0x0, 0x1c,

    /* U+0032 "2" */
    0x9, 0xcc, 0x40, 0x39, 0x1, 0xe0, 0x0, 0x1,
    0xe0, 0x0, 0xa, 0x60, 0x0, 0x98, 0x0, 0x9,
    0x90, 0x0, 0x6f, 0xcc, 0xc2,

    /* U+0033 "3" */
    0x9, 0xcc, 0x40, 0x38, 0x2, 0xd0, 0x0, 0x4,
    0xc0, 0x0, 0xbf, 0x50, 0x0, 0x1, 0xe1, 0x45,
    0x0, 0xe1, 0xa, 0xcc, 0x60,

    /* U+0034 "4" */
    0x0, 0x2c, 0x0, 0x0, 0xa4, 0x0, 0x2, 0xc1,
    0x10, 0xb, 0x46, 0x80, 0x3b, 0x6, 0x80, 0x8c,
    0xbd, 0xd6, 0x0, 0x6, 0x80,

    /* U+0035 "5" */
    0xb, 0xcb, 0x90, 0xd, 0x0, 0x0, 0xf, 0xcb,
    0x20, 0x7, 0x3, 0xd0, 0x0, 0x0, 0xd1, 0x18,
    0x1, 0xd0, 0x9, 0xcc, 0x30,

    /* U+0036 "6" */
    0x0, 0x3b, 0x0, 0x0, 0xa1, 0x0, 0x7, 0xec,
    0x40, 0x1c, 0x1, 0xe1, 0x68, 0x0, 0xb4, 0x4b,
    0x0, 0xd1, 0x8, 0xcc, 0x50,

    /* U+0037 "7" */
    0x5b, 0xbb, 0xf1, 0x0, 0x3, 0xb0, 0x0, 0xa,
    0x40, 0x0, 0x1d, 0x0, 0x0, 0x86, 0x0, 0x0,
    0xd0, 0x0, 0x6, 0x90, 0x0,

    /* U+0038 "8" */
    0x7, 0xcc, 0x40, 0x2c, 0x1, 0xd0, 0x1d, 0x2,
    0xc0, 0x9, 0xef, 0x40, 0x5a, 0x1, 0xd1, 0x69,
    0x0, 0xd2, 0xa, 0xcc, 0x60,

    /* U+0039 "9" */
    0x8, 0xcd, 0x40, 0x5a, 0x1, 0xe0, 0x86, 0x0,
    0xd1, 0x5b, 0x2, 0xc0, 0x7, 0xce, 0x30, 0x0,
    0x48, 0x0, 0x1, 0xc0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 37, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 91, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 23, .adv_w = 91, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 91, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 91, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 79, .adv_w = 91, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 91, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 121, .adv_w = 91, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 91, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 163, .adv_w = 91, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 184, .adv_w = 91, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 0, 1, 2, 3, 4, 5, 6,
    7, 8, 9, 10
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 46, .range_length = 12, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 12, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t Harm_san_10 = {
#else
lv_font_t Harm_san_10 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 7,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if HARM_SAN_10*/


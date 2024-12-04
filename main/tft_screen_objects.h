#pragma once

#include "tft_screen.h"
#include "tft_screen_objects_id.h"


struct _screen_position_scope {
    TFT_SCREEN_MAX_SCOPE_SIZE_TYPE x_start, y_start, x_end, y_end;
};

struct _screen_base_methods {
    void (*draw_pixel)(struct _screen_object *screen, TFT_SCREEN_MAX_SCOPE_SIZE_TYPE x,
                        TFT_SCREEN_MAX_SCOPE_SIZE_TYPE y, unsigned short int color);

    void (*set_window_addr)(struct _screen_object *screen, struct _screen_position_scope *pos_scope);
};

struct _screen_object {
    TFT_SCREEN_PINS_TYPE *pins;
    struct _screen_position_scope pos_scope;
    struct _screen_base_methods base_methods;
};

struct _widget_point {
    TFT_SCREEN_MAX_SCOPE_SIZE_TYPE x, y;
};

struct _widget_color {
    TFT_SCREEN_COLOR_BITS_TYPE foreground_color, background_color, outline_color;
};

struct _screen_widget_object {
    struct _screen_object *root;
    void *parent;
    TFTScreenObjectIDSize_t ob_id;
    struct _widget_color color;
};

struct _widget_frame_object {
    struct _screen_widget_object widget;
};

struct _widget_line_object {
    struct _screen_widget_object widget;
    struct _widget_point point1, point2;
};

struct _widget_square_object {
    struct _screen_widget_object widget;
    struct _widget_line_object lines[4];
};

struct _widget_label_object {
    struct _screen_widget_object widget;
    struct _widget_square_object background;
    unsigned char *text;
};

struct _widget_button_object {
    struct _screen_widget_object widget;
    unsigned char *text;
};


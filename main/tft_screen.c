#include "tft_screen_objects.h"
#include "tft_screen_objects_id.h"

#include <stdlib.h>


#define _widget_alloc(object_type) \
    (struct _screen_widget_object *)malloc(sizeof(object_type))

struct _screen_widget_object *
tft_screen_widget_line_create(struct _screen_widget_object *parent) {
    struct _screen_widget_object *widget;
    
    widget = _widget_alloc(struct _widget_line_object);

    widget->parent = parent;
    widget->root = parent->root;
    widget->ob_id = TFT_SCREEN_OBJ_ID_LINE;

    return widget;
}

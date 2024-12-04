#pragma once


#ifndef TFT_SCREEN_MAX_SCOPE_SIZE_TYPE
#define TFT_SCREEN_MAX_SCOPE_SIZE_TYPE unsigned char
#endif

#ifndef TFT_SCREEN_PINS_TYPE
#define TFT_SCREEN_PINS_TYPE void
#endif

#ifndef TFT_SCREEN_COLOR_BITS_TYPE
#define TFT_SCREEN_COLOR_BITS_TYPE unsigned short int
#endif


typedef struct _screen_object TFTScreen_ScreenObject;
typedef struct _screen_widget_object TFTScreen_WidgetObject;

TFTScreen_WidgetObject * tft_screen_widget_create(TFTScreen_ScreenObject *root);


/*
 * File: TFT_SCREEN_H
 */

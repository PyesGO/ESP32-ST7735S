#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct {
    unsigned char SCL, SDA, RES, DC, CS;
} ScreenPins;
#define PIN_NUM 5

static inline void
timesleep(unsigned int ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

/*
File: TFT_DRIVER_H
*/

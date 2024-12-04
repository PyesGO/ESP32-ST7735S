#pragma once


#include "priv_gpio.h"


void
test(void) {
    ESP32_REG_GPIO->OUT = 1ULL << 2;
}


/*
 * File: DHT22_PRIVATE_DEF_H
 */

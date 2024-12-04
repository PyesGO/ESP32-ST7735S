#pragma once


#define _contact_direct(x, y) x ## y
#define _contact(x, y) _contact_direct(x, y)
#define _ESP32_REG_RESERVE_NON_TYPE_SIGN(count) _contact(reserve_, __COUNTER__)[count]


typedef volatile unsigned int _esp32_register_t;


struct _esp32_gpio_registers {
    _esp32_register_t OUT, // 0x3FF44004
                      OUT_W1TS, // 0x3FF44008
                      OUT_W1TC, // 0x3FF4400C
                      OUT1, // 0x3FF44010
                      OUT1_W1TS, // 0x3FF44014
                      OUT1_W1TC, // 0x3FF44018
                      _ESP32_REG_RESERVE_NON_TYPE_SIGN(1), // 0x3FF4401C
                      ENABLE, // 0x3FF44020
                      ENABLE_W1TS, // 0x3FF44024
                      ENABLE_W1TC, // 0x3FF44028
                      ENABLE1, // 0x3FF4402C
                      ENABLE1_W1TS, // 0x3FF44030
                      ENABLE1_W1TC; // 0x3FF44034
} __attribute__((__packed__));

#define ESP32_REG_GPIO ((struct _esp32_gpio_registers *)0x3FF44004)


/**
 * File: PRIV_GPIO_REGS_H
 */

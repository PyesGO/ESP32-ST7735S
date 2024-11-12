#pragma once

#include "driver/gpio.h"

typedef struct {
    unsigned char SCL, SDA, DC, CS;
} softSPI_basePins;

typedef unsigned short int softSPI_maxTransmissionSize_t;

#define softSPI_write_byte(pins_addr, byte) { \
    unsigned char __count, __byte; \
    __byte = (unsigned char)(byte); \
    for (__count = 0; __count < 8; ++__count) { \
        gpio_set_level((pins_addr)->SCL, 0); \
        gpio_set_level((pins_addr)->SDA, __byte & 0x80); \
        gpio_set_level((pins_addr)->SCL, 1); \
        __byte <<= 1; \
    } \
}

#define softSPI_write_command(pins_addr, command) { \
    unsigned char __command = (unsigned char)(command); \
    gpio_set_level((pins_addr)->DC, 0); \
    softSPI_write_byte(pins_addr, __command); \
}

#define softSPI_write_rawdata(pins_addr, data_addr, count) { \
    unsigned char *__data_addr = (unsigned char *)(data_addr); \
    softSPI_maxTransmissionSize_t __count = (softSPI_maxTransmissionSize_t)(count); \
    gpio_set_level((pins_addr)->DC, 1); \
    while (__count--) { \
        softSPI_write_byte(pins_addr, *(__data_addr++)); \
    } \
}

#define softSPI_write_data(pins_addr, data) { \
    __typeof__(data) __data = data; \
    softSPI_write_rawdata(pins_addr, &__data, sizeof(__data)); \
}

#define softSPI_write_color(pins_addr, color) \
    softSPI_write_data(pins_addr, (unsigned short int)(color))

#define softSPI_enable_transmit(pins_addr) \
    gpio_set_level((pins_addr)->CS, 0)

#define softSPI_disable_transmit(pins_addr) \
    gpio_set_level((pins_addr)->CS, 1)


/*
 * File: SOFTSPI_LIB_H
 */

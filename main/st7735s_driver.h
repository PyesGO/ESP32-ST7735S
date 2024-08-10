#pragma once

#include "driver/gpio.h"
#include <stdlib.h>

// System function command list:
#define ST7735S_NOP 0x00
#define ST7735S_SWRESET 0x01
#define ST7735S_RDDID 0x04
#define ST7735S_RDDST 0x09
#define ST7735S_RDDPM 0x0A
#define ST7735S_RDDMADCTL 0x0B
#define ST7735S_RDDCOLMOD 0x0C
#define ST7735S_RDDIM 0x0D
#define ST7735S_RDDSM 0x0E
#define ST7735S_RDDSDR 0x0F
#define ST7735S_SLPIN 0x10
#define ST7735S_SLPOUT 0x11
#define ST7735S_PTLON 0x12
#define ST7735S_NORON 0x13
#define ST7735S_INVOFF 0x20
#define ST7735S_INVON 0x21
#define ST7735S_GAMSET 0x26
#define ST7735S_DISPOFF 0x28
#define ST7735S_DISPON 0x29
#define ST7735S_CASET 0x2A
#define ST7735S_RASET 0x2B
#define ST7735S_RAMWR 0x2C
#define ST7735S_RGBSET 0x2D
#define ST7735S_RAMRD 0x2E
// Partial Start/End Address Set.
#define ST7735S_PTLAR 0x30
// Scroll area set.
#define ST7735S_SCRLAR 0x33
// Tering effect line off.
#define ST7735S_TEOFF 0x34
#define ST7735S_TEON 0x35
#define ST7735S_MADCTL 0x36
#define ST7735S_VSCSAD 0x37
#define ST7735S_IDMOFF 0x38
#define ST7735S_IDMON 0x39
#define ST7735S_COLMOD 0x3A
#define ST7735S_RDID1 0xDA
#define ST7735S_RDID2 0xDB
#define ST7735S_RDID3 0xDC
// Panel function command list:
#define ST7735S_FRMCTR1 0xB1
#define ST7735S_FRMCTR2 0xB2
#define ST7735S_FRMCTR3 0xB3
#define ST7735S_INVCTR 0xB4
#define ST7735S_PWCTR1 0xC0
#define ST7735S_PWCTR2 0xC1
#define ST7735S_PWCTR3 0xC2
#define ST7735S_PWCTR4 0xC3
#define ST7735S_PWCTR5 0xC4
#define ST7735S_VMCTR1 0xC5
#define ST7735S_VMOFCTR 0xC7
#define ST7735S_WRID2 0xD1
#define ST7735S_WRID3 0xD2
#define ST7735S_NVCTR1 0xD9
#define ST7735S_NVCTR2 0xDE
#define ST7735S_NVCTR3 0xDF
#define ST7735S_GMCTRP1 0xE0
#define ST7735S_GMCTRN1 0xE1
#define ST7735S_GCV 0xFC

typedef unsigned short int st7735s_size_t;

typedef struct {
    unsigned char VCC, SCL, SDA, RES, DC, CS, BLK;
} st7735s_pins;
static const unsigned char ST7735S_PINS_NUM = sizeof(st7735s_pins);

typedef struct {
    unsigned int width, height;
} st7735s_size;

typedef struct {
    unsigned char *addr;
    unsigned int size, offset;
} st7735s_buffer;

void st7735s_init(st7735s_pins *pins, st7735s_size *size);

#define st7735s_write_byte(pins_struct_addr, byte) { \
    unsigned char __count, __byte; \
    __byte = byte; \
    for (__count = 0; __count < 8; ++__count) { \
        gpio_set_level((pins_struct_addr)->SCL, 0); \
        gpio_set_level((pins_struct_addr)->SDA, __byte & 0x80); \
        gpio_set_level((pins_struct_addr)->SCL, 1); \
        __byte <<= 1; \
    } \
}

#define st7735s_write_command(pins_struct_addr, command) { \
    __typeof__(command) __command = command; \
    gpio_set_level((pins_struct_addr)->DC, 0); \
    st7735s_write_byte(pins_struct_addr, __command); \
}

#define st7735s_write_rawdata(pins_addr, data_addr, count) { \
    unsigned char *__data_addr = data_addr; \
    st7735s_size_t __count = count; \
    gpio_set_level((pins_addr)->DC, 1); \
    while (__count--) { \
        st7735s_write_byte(pins_addr, *(__data_addr++)); \
    } \
}

#define st7735s_write_data(pins_addr, data) { \
    unsigned char __data = data; \
    st7735s_write_rawdata(pins_addr, &__data, sizeof(__data)); \
}

#define st7735s_enable_transmit(pins_struct_addr) \
    gpio_set_level((pins_struct_addr)->CS, 0)

#define st7735s_disable_transmit(pins_struct_addr) \
    gpio_set_level((pins_struct_addr)->CS, 1)

#define st7735s_blkctl(pins_struct_addr, state) \
    gpio_set_level((pins_struct_addr)->BLK, state)

#define st7735s_powerctl(pins_struct_addr, state) \
    gpio_set_level((pins_struct_addr)->VCC, state)


#define st7735s_buffer_init(bufsize) { \
    .addr = (unsigned char *)malloc(bufsize), \
    .size = bufsize, \
    .offset = 0 \
}

#define st7735s_buffer_commit(pins_addr, buffer) { \
    st7735s_write_rawdata(pins_addr, buffer.addr, buffer.offset); \
    buffer.offset = 0; \
}

#define st7735s_buffer_write(pins_addr, buffer, data) { \
    unsigned char *__bufaddr = buffer.addr, \
                  *__data = (unsigned char *)&data; \
    __bufaddr += buffer.offset; \
    st7735s_size_t __count = sizeof(data); \
    while (__count--) { \
        *(__bufaddr++) = *(__data++); \
        ++(buffer.offset); \
        if (buffer.offset == buffer.size) { \
            st7735s_buffer_commit(pins_addr, buffer); \
        } \
    } \
}

#define st7735s_buffer_clean(buffer) { \
    free(buffer.addr); \
    buffer.addr = NULL; \
    buffer.offset = buffer.size = 0; \
}

#define st7735s_set_SRAM_writable(pins_addr) \
    st7735s_write_command(pins_addr, ST7735S_RAMWR); \

void st7735s_set_window_addr(
        st7735s_pins *pins,
        unsigned char x1,
        unsigned char y1,
        unsigned char x2,
        unsigned char y2);
void st7735s_hwreset(st7735s_pins *pins);
void st7735s_draw_point(st7735s_pins *pins, unsigned char x, unsigned char y, unsigned short int color);
void st7735s_fill_screen(st7735s_pins *pins, st7735s_size *size, unsigned short int color);
void timesleep_ms(unsigned int ms);

/*
File: ST7735S_DRIVER_H
*/

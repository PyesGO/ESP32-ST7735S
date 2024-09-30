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
    unsigned char width, height;
} st7735s_size;

typedef struct {
    unsigned char *addr;
    unsigned int size, offset;
} st7735s_buffer;


#define st7735s_write_byte(pins_addr, byte) { \
    unsigned char __count, __byte; \
    __byte = (unsigned char)byte; \
    for (__count = 0; __count < 8; ++__count) { \
        gpio_set_level((pins_addr)->SCL, 0); \
        gpio_set_level((pins_addr)->SDA, __byte & 0x80); \
        gpio_set_level((pins_addr)->SCL, 1); \
        __byte <<= 1; \
    } \
}

#define st7735s_write_command(pins_addr, command) { \
    unsigned char __command = (unsigned char)command; \
    gpio_set_level((pins_addr)->DC, 0); \
    st7735s_write_byte(pins_addr, __command); \
}

#define st7735s_write_rawdata(pins_addr, data_addr, count) { \
    unsigned char *__data_addr = (unsigned char *)data_addr; \
    st7735s_size_t __count = (st7735s_size_t)count; \
    gpio_set_level((pins_addr)->DC, 1); \
    while (__count--) { \
        st7735s_write_byte(pins_addr, *(__data_addr++)); \
    } \
}

#define st7735s_write_data(pins_addr, data) { \
    __typeof__(data) __data = data; \
    st7735s_write_rawdata(pins_addr, &__data, sizeof(__data)); \
}

#define st7735s_write_color(pins_addr, color) \
    st7735s_write_data(pins_addr, (unsigned short int)(color))

#define st7735s_enable_transmit(pins_addr) \
    gpio_set_level((pins_addr)->CS, 0)

#define st7735s_disable_transmit(pins_addr) \
    gpio_set_level((pins_addr)->CS, 1)

#define st7735s_blkctl(pins_addr, state) \
    gpio_set_level((pins_addr)->BLK, state)

#define st7735s_powerctl(pins_addr, state) \
    gpio_set_level((pins_addr)->VCC, state)

#define st7735s_buffer_init(bufsize) { \
    .addr = (unsigned char *)malloc(bufsize), \
    .size = bufsize, \
    .offset = 0 \
}

#define st7735s_buffer_commit(pins_addr, buf_addr) { \
    st7735s_write_rawdata(pins_addr, (buf_addr)->addr, (buf_addr)->offset); \
    (buf_addr)->offset = 0; \
}

#define st7735s_buffer_write(pins_addr, buf_addr, data) { \
    unsigned char *__bufdata_addr = (buf_addr)->addr, \
                  *__data_addr = (unsigned char *)&(data); \
    __bufdata_addr += (buf_addr)->offset; \
    st7735s_size_t __count = sizeof(data); \
    while (__count--) { \
        *(__bufdata_addr++) = *(__data_addr++); \
        ++((buf_addr)->offset); \
        if ((buf_addr)->offset == (buf_addr)->size) { \
            st7735s_buffer_commit(pins_addr, buf_addr); \
        } \
    } \
}

#define st7735s_buffer_free(buf_addr) { \
    if ((buf_addr) != NULL) { \
        free((buf_addr)->addr); \
        (buf_addr)->addr = NULL; \
        (buf_addr)->offset = (buf_addr)->size = 0; \
    } \
}

#define st7735s_set_SRAM_writable(pins_addr) \
    st7735s_write_command(pins_addr, ST7735S_RAMWR)


typedef unsigned char screen_size_t;

typedef struct {
    screen_size_t x0, y0,
                  x1, y1,
                  dx, dy;
    unsigned char _istemp;
} st7735s_LineObject;

typedef struct {
    screen_size_t x0, y0,
                  x1, y1,
                  length,
                  width;
    unsigned char _istemp;
} st7735s_SquareObject;


#define st7735s_createObj(obj) ({ \
    (__typeof__(obj) *)malloc(sizeof(__typeof__(obj))); \
})

#define st7735s_createTempObj(obj, obj_creator) ({ \
    __typeof__(obj) *__temp_obj = obj_creator; \
    __temp_obj->_istemp = 1; \
    __temp_obj; \
})

#define st7735s_createLineObj(x0_, y0_, x1_, y1_) ({ \
    st7735s_LineObject *__obj = st7735s_createObj(st7735s_LineObject); \
    __obj->x0 = x0_; \
    __obj->y0 = y0_; \
    __obj->x1 = x1_; \
    __obj->y1 = y1_; \
    __obj->dx = ((x0_) < (x1_)) ? ((x1_) - (x0_)) : ((x0_) - (x1_)); \
    __obj->dy = ((y0_) < (y1_)) ? ((y1_) - (y0_)) : ((y0_) - (y1_)); \
    __obj->_istemp = 0; \
    __obj; \
})

#define st7735s_createTempLineObj(x0_, y0_, x1_, y1_) \
    st7735s_createTempObj(st7735s_LineObject, st7735s_createLineObj(x0_, y0_, x1_, y1_))

#define st7735s_createSquareObj(x_, y_, length_, width_) ({ \
    st7735s_SquareObject *__obj = st7735s_createObj(st7735s_SquareObject); \
    __obj->x0 = x_; \
    __obj->y0 = y_; \
    __obj->x1 = (x_) + (length_); \
    __obj->y1 = (y_) + (width_); \
    __obj->length = length_; \
    __obj->width = width_; \
    __obj->_istemp = 0; \
    __obj; \
})

#define st7735s_createTempSquareObj(x_, y_, length_, width_) \
    st7735s_createTempObj(st7735s_SquareObject, st7735s_createSquareObj(x_, y_, length_, width_))

#define st7735s_objaddr_to_type(obj_addr) \
    __typeof__(*(obj_addr))

#define st7735s_copyObj(sour_obj_addr, dest_obj_addr) { \
    st7735s_size_t __sour_obj_size = sizeof(st7735s_objaddr_to_type(sour_obj_addr)); \
    st7735s_objaddr_to_type(sour_obj_addr) *__souraddr = sour_obj_addr; \
    dest_obj_addr = (st7735s_objaddr_to_type(sour_obj_addr) *)malloc(__sour_obj_size); \
    st7735s_objaddr_to_type(sour_obj_addr) *__destaddr = dest_obj_addr; \
    while (--__sour_obj_size) { \
        *(__destaddr++) = *(__souraddr++); \
    } \
}

#define st7735s_copyNotTempObj(sour_obj_addr, dest_obj_addr) { \
    if (! (sour_obj_addr)->_istemp) { \
        st7735s_copyObj(sour_obj_addr, dest_obj_addr); \
    } \
}

#define st7735s_freeObj(obj_addr) { \
    if ((obj_addr) != NULL) { \
        free(obj_addr); \
        obj_addr = NULL; \
    } \
}

#define st7735s_swap_var(var1, var2) { \
    var1 = (var1) ^ (var2); \
    var2 = (var1) ^ (var2); \
    var1 = (var1) ^ (var2); \
}

void st7735s_init(st7735s_pins *pins, st7735s_size *size);
void st7735s_cmdlist_helper(st7735s_pins *pins, unsigned char *cmd_list);
#define st7735s_set_window_addr(pins_addr, x0, y0, x1, y1) { \
    unsigned char __cmd_list[] = { \
        2, \
        ST7735S_CASET, \
        4, \
        0x00, x0, 0x00, ((x0) == (x1)) ? (x0) : ((x1) - 1), \
        0, \
        ST7735S_RASET, \
        4, \
        0x00, y0, 0x00, ((y0) == (y1)) ? (y0) : ((y1) - 1), \
        0 \
    }; \
    st7735s_cmdlist_helper(pins_addr, __cmd_list); \
}
void st7735s_hwreset(st7735s_pins *pins);
void st7735s_fill_screen(st7735s_pins *pins, st7735s_size *size, unsigned short int color, st7735s_buffer *buffer);
#define st7735s_draw_pixel(pins_addr, x, y, color) { \
    st7735s_enable_transmit(pins_addr); \
    st7735s_set_window_addr(pins_addr, x, y, x, y); \
    st7735s_set_SRAM_writable(pins_addr); \
    st7735s_write_color(pins_addr, color); \
    st7735s_disable_transmit(pins_addr); \
}
void st7735s_draw_line(st7735s_pins *pins, st7735s_LineObject *line, unsigned short int color);
void st7735s_draw_square(st7735s_pins *pins, st7735s_SquareObject *square, unsigned short int color);
void timesleep_ms(unsigned int ms);

/*
File: ST7735S_DRIVER_H
*/

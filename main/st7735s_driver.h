#pragma once

#include "st7735s_ext_pins.h"
#include "st7735s_cmds.h"

#include "driver/gpio.h"

#include <stdlib.h>

typedef unsigned short int st7735s_size_t;


typedef struct {
    unsigned char SCL, SDA, RES, DC, CS;
} st7735s_pins;
static const unsigned char ST7735S_PINS_NUM = sizeof(st7735s_pins);

typedef struct {
    unsigned char width, height;
} st7735s_size;

typedef struct {
    st7735s_pins pins;
    st7735s_size size;
} st7735s_ScreenObj;

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

#ifdef BLK_PIN
#define st7735s_blkctl(state) \
    gpio_set_level(BLK_PIN, state)
#endif

#ifdef VCC_PIN
#define st7735s_powerctl(state) \
    gpio_set_level(VCC_PIN, state)
#endif

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


typedef unsigned char st7735s_screen_size_t;

typedef struct {
    st7735s_screen_size_t x0, y0,
                          x1, y1,
                          dx, dy;
    unsigned char _istemp;
} st7735s_LineObject;

typedef struct {
    st7735s_screen_size_t x0, y0,
                          x1, y1,
                          length,
                          width;
    unsigned char _istemp;
} st7735s_SquareObject;


#define st7735s_createObj(obj) \
    (__typeof__(obj) *)malloc(sizeof(__typeof__(obj)))

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
    __obj->dx = ((__obj->x0) <= (__obj->x1)) ? ((__obj->x1) - (__obj->x0)) : ((__obj->x0) - (__obj->x1)); \
    __obj->dy = ((__obj->y0) <= (__obj->y1)) ? ((__obj->y1) - (__obj->y0)) : ((__obj->y0) - (__obj->y1)); \
    __obj->_istemp = 0; \
    __obj; \
})

#define st7735s_createTempLineObj(x0_, y0_, x1_, y1_) \
    st7735s_createTempObj(st7735s_LineObject, st7735s_createLineObj(x0_, y0_, x1_, y1_))

#define st7735s_createSquareObj(x_, y_, length_, width_) ({ \
    st7735s_SquareObject *__obj = st7735s_createObj(st7735s_SquareObject); \
    __obj->x0 = x_; \
    __obj->y0 = y_; \
    __obj->length = length_; \
    __obj->width = width_; \
    __obj->x1 = (__obj->x0) + (__obj->length); \
    __obj->y1 = (__obj->y0) + (__obj->width); \
    __obj->_istemp = 0; \
    __obj; \
})

#define st7735s_createTempSquareObj(x_, y_, length_, width_) \
    st7735s_createTempObj(st7735s_SquareObject, st7735s_createSquareObj(x_, y_, length_, width_))

#define st7735s_objaddr_to_type(obj_addr) \
    __typeof__(*(obj_addr))

#define st7735s_copyObj(sour_obj_addr, dest_obj_addr) { \
    st7735s_objaddr_to_type(sour_obj_addr) *__souraddr = sour_obj_addr; \
    st7735s_objaddr_to_type(dest_obj_addr) **__destaddr = &(dest_obj_addr); \
    st7735s_size_t __sour_obj_size = sizeof(st7735s_objaddr_to_type(__souraddr)); \
    *(__destaddr) = (st7735s_objaddr_to_type(*(__destaddr)) *)malloc(__sour_obj_size); \
    unsigned char *__sour_byte_addr, *__dest_byte_addr; \
    __sour_byte_addr = (unsigned char *)__souraddr; \
    __dest_byte_addr = (unsigned char *)(*(__destaddr)); \
    while (--__sour_obj_size) { \
        *(__dest_byte_addr++) = *(__sour_byte_addr++); \
    } \
}

#define st7735s_copyNotTempObj(sour_obj_addr, dest_obj_addr) { \
    if (! ((sour_obj_addr)->_istemp)) { \
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
        0x00, x0, 0x00, x1, \
        0, \
        ST7735S_RASET, \
        4, \
        0x00, y0, 0x00, y1, \
        0 \
    }; \
    st7735s_cmdlist_helper(pins_addr, __cmd_list); \
}
void st7735s_hwreset(st7735s_pins *pins);
void st7735s_fill_screen(st7735s_pins *pins, st7735s_size *size, unsigned short int color);
#define st7735s_draw_pixel(pins_addr, x, y, color) { \
    st7735s_pins *__pins_addr = pins_addr; \
    st7735s_screen_size_t __x, __y; \
    __x = x; \
    __y = y; \
    st7735s_enable_transmit(__pins_addr); \
    st7735s_set_window_addr(__pins_addr, __x, __y, __x, __y); \
    st7735s_set_SRAM_writable(__pins_addr); \
    st7735s_write_color(__pins_addr, color); \
    st7735s_disable_transmit(__pins_addr); \
}
void st7735s_draw_non_slope_line(st7735s_pins *pins, st7735s_LineObject *line, unsigned short int color);
void st7735s_draw_slope_line(st7735s_pins *pins, st7735s_LineObject *line, unsigned short int color);
#define st7735s_draw_line(pins_addr, line_obj_addr, color) { \
    if ( (((line_obj_addr)->x0) == ((line_obj_addr)->x1)) || (((line_obj_addr)->y0) == ((line_obj_addr)->y1)) ) { \
        st7735s_draw_non_slope_line(pins_addr, line_obj_addr, color); \
    } else { \
        st7735s_draw_slope_line(pins_addr, line_obj_addr, color); \
    } \
}
void st7735s_draw_square(st7735s_pins *pins, st7735s_SquareObject *square, unsigned short int color);
void st7735s_test(st7735s_pins *pins, st7735s_LineObject *line);
void timesleep_ms(unsigned int ms);

/*
 * File: ST7735S_DRIVER_H
 */

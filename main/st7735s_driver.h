#pragma once

#include "st7735s_ext_pins.h"
#include "st7735s_cmds.h"
#include "softSPI_lib.h"
#include "compare_helper.h"

#include "driver/gpio.h"

#include <stdlib.h>

typedef unsigned short int st7735s_size_t;

typedef struct {
    unsigned char SCL, SDA, RES, DC, CS;
} st7735s_pins;
static const unsigned char ST7735S_PINS_NUM = sizeof(st7735s_pins);

typedef struct {
    unsigned char x_start, y_start, x_end, y_end;
} st7735s_screen_size;

typedef struct {
    st7735s_pins *pins;
    st7735s_screen_size *size;
} st7735s_Screen;

#define st7735s_createScreenObj() ({ \
    st7735s_Screen *__screen_obj = (st7735s_Screen *)malloc(sizeof(st7735s_Screen)); \
    __screen_obj; \
})

#define st7735s_initScreenPins(screen_obj_addr, SCL_, SDA_, RES_, DC_, CS_) { \
    st7735s_pins *__pins_objaddr = (st7735s_pins *)malloc(sizeof(st7735s_pins)); \
    __pins_objaddr->SCL = SCL_; \
    __pins_objaddr->SDA = SDA_; \
    __pins_objaddr->RES = RES_; \
    __pins_objaddr->DC = DC_; \
    __pins_objaddr->CS = CS_; \
    (screen_obj_addr)->pins = __pins_objaddr; \
}

#define st7735s_initScreenSize(screen_obj_addr, x_start_, y_start_, x_end_, y_end_) { \
    st7735s_screen_size *__size_objaddr = (st7735s_screen_size *)malloc(sizeof(st7735s_screen_size)); \
    __size_objaddr->x_start = x_start_; \
    __size_objaddr->y_start = y_start_; \
    __size_objaddr->x_end = x_end_; \
    __size_objaddr->y_end = y_end_; \
    (screen_obj_addr)->size = __size_objaddr; \
}

#define st7735s_freeScreenObj(screen_obj_addr) { \
    st7735s_Screen *__screen_objaddr = screen_obj_addr; \
    free(__screen_objaddr->pins); \
    free(__screen_objaddr->size); \
    free(__screen_objaddr); \
}

#ifdef BLK_PIN
#define st7735s_blkctl(state) \
    gpio_set_level(BLK_PIN, state)
#endif

#ifdef VCC_PIN
#define st7735s_powerctl(state) \
    gpio_set_level(VCC_PIN, state)
#endif

#define st7735s_set_SRAM_writable(pins_addr) \
    softSPI_write_command(pins_addr, ST7735S_RAMWR)


typedef unsigned char st7735s_screenMaxSize_t;

typedef struct {
    st7735s_screenMaxSize_t x0, y0,
                            x1, y1,
                            dx, dy;
    unsigned char _istemp;
} st7735s_LineObject;

typedef struct {
    st7735s_screenMaxSize_t x0, y0,
                            x1, y1,
                            length,
                            width;
    unsigned char _istemp;
} st7735s_SquareObject;


#define st7735s_createObj(obj) \
    (obj *)malloc(sizeof(obj))

#define st7735s_createTempObj(obj, obj_creator) ({ \
    obj *__temp_obj = obj_creator; \
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
    st7735s_objaddr_to_type(sour_obj_addr) *__souraddr = sour_obj_addr, \
                                           **__destaddr = &(dest_obj_addr); \
    unsigned char *__souraddr_byte, *__destaddr_byte; \
    unsigned int *__souraddr_4bytes, *__destaddr_4bytes; \
    st7735s_size_t __sour_split_byte_count, __sour_split_4bytes_count; \
    __sour_split_byte_count = sizeof(st7735s_objaddr_to_type(__souraddr)); \
    __sour_split_4bytes_count = __sour_split_byte_count >> 2; \
    __souraddr_byte = (unsigned char *)__souraddr; \
    __souraddr_4bytes = (unsigned int *)__souraddr; \
    *(__destaddr) = (st7735s_objaddr_to_type(__souraddr) *)malloc(sizeof(st7735s_objaddr_to_type(__souraddr))); \
    __destaddr_byte = (unsigned char *)(*(__destaddr)); \
    __destaddr_4bytes = (unsigned int *)(*(__destaddr)); \
    while (__sour_split_4bytes_count--) { \
        *(__destaddr_4bytes++) = *(__souraddr_4bytes++); \
    } \
    __sour_split_byte_count -= __sour_split_4bytes_count << 2; \
    __souraddr_byte += __sour_split_4bytes_count << 2; \
    __destaddr_byte += __sour_split_4bytes_count << 2; \
    while (__sour_split_byte_count--) { \
        *(__destaddr_byte++) = *(__souraddr_byte++); \
    } \
}

#define st7735s_copyNotTempObj(sour_obj_addr, dest_obj_addr) { \
    if (! ((sour_obj_addr)->_istemp)) { \
        st7735s_copyObj(sour_obj_addr, dest_obj_addr); \
    } \
}

#define st7735s_freeObj(obj_addr) \
    free(obj_addr)

#define st7735s_swap_var(var1, var2) { \
    var1 = (var1) ^ (var2); \
    var2 = (var1) ^ (var2); \
    var1 = (var1) ^ (var2); \
}


void st7735s_initScreen(st7735s_Screen *screen);
void st7735s_cmdlist_helper(st7735s_pins *pins, unsigned char *cmd_list);
#define st7735s_set_window_addr(screen_obj_addr, x0, y0, x1, y1) { \
    st7735s_Screen *__screen_addr; \
    st7735s_screenMaxSize_t __x0, __y0, \
                            __x1, __y1; \
    __screen_addr = screen_obj_addr; \
    __x0 = compare_helper_max(__screen_addr->size->x_start, x0); \
    __y0 = compare_helper_max(__screen_addr->size->y_start, y0); \
    __x1 = compare_helper_min(__screen_addr->size->x_end, x1); \
    __y1 = compare_helper_min(__screen_addr->size->y_end, y1); \
    unsigned char __cmd_list[] = { \
        2, \
        ST7735S_CASET, \
        4, \
        0x00, __x0, 0x00, ((__x1 > __x0) ? (__x1 - 1) : (__x0)), \
        0, \
        ST7735S_RASET, \
        4, \
        0x00, __y0, 0x00, ((__y1 > __y0) ? (__y1 - 1) : (__y0)), \
        0 \
    }; \
    st7735s_cmdlist_helper(__screen_addr->pins, __cmd_list); \
}
void st7735s_hwreset(st7735s_pins *pins);
void st7735s_fill_screen(st7735s_Screen *screen, unsigned short int color);
#define st7735s_draw_pixel(screen_obj_addr, x, y, color) { \
    st7735s_Screen *__screen; \
    st7735s_screenMaxSize_t __x, __y; \
    __screen = screen_obj_addr; \
    __x = x; \
    __y = y; \
    softSPI_enable_transmit(__screen->pins); \
    st7735s_set_window_addr(__screen, __x, __y, __x, __y); \
    st7735s_set_SRAM_writable(__screen->pins); \
    softSPI_write_color(__screen->pins, color); \
    softSPI_disable_transmit(__screen->pins); \
}
void st7735s_draw_non_slope_line(st7735s_Screen *screen, st7735s_LineObject *line, unsigned short int color);
void st7735s_draw_slope_line(st7735s_Screen *screen, st7735s_LineObject *line, unsigned short int color);
void st7735s_draw_square(st7735s_Screen *screen, st7735s_SquareObject *square, unsigned short int color);
void timesleep_ms(unsigned int ms);

/*
 * File: ST7735S_DRIVER_H
 */

#include "st7735s_driver.h"
#include "softSPI_lib.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

static void
pins_init(st7735s_pins *pins) {
    unsigned long long pins_mask;
    unsigned char count, *pins_ptr;

    pins_ptr = (unsigned char *)pins;
    pins_mask = 0;
    for (count = 0; count < ST7735S_PINS_NUM; ++count) {
        pins_mask |= 1ULL << *(pins_ptr++);
    }

    #ifdef BLK_PIN
    pins_mask |= 1ULL << BLK_PIN;
    #endif

    #ifdef VCC_PIN
    pins_mask |= 1ULL << VCC_PIN;
    #endif

    gpio_config_t conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = pins_mask
    }; gpio_config(&conf);
}

void
st7735s_cmdlist_helper(st7735s_pins *pins, unsigned char *cmd_list) {
    unsigned char num_commands,
                  num_args,
                  delayms,
                  *command_list_addr,
                  current_command,
                  current_argument;
    
    command_list_addr = cmd_list;
    num_commands = *(command_list_addr++);
    while (num_commands--) {
        current_command = *(command_list_addr++);
        softSPI_write_command(pins, current_command);
        
        num_args = *(command_list_addr++);
        if (num_args) {
            while (num_args--) {
                current_argument = *(command_list_addr++);
                softSPI_write_data(pins, current_argument);
            }
        }
        
        delayms = *(command_list_addr++);
        if (delayms) {
            timesleep_ms(delayms);
        }
    }
}

void
timesleep_ms(unsigned int ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void
st7735s_hwreset(st7735s_pins *pins) {
    gpio_set_level(pins->RES, 0);
    timesleep_ms(1);
    gpio_set_level(pins->RES, 1);
    timesleep_ms(120);
}

void
st7735s_initScreenState(st7735s_Screen *screen) {
    pins_init(screen->pins);
    st7735s_hwreset(screen->pins);

    unsigned char command_list[] = {
        7,
        ST7735S_SLPOUT,
        0,
        120,
        ST7735S_FRMCTR1,
        3,
        0x00, 0x01, 0x01,
        0,
        ST7735S_INVCTR,
        1,
        0x00,
        0,
        ST7735S_MADCTL,
        1,
        0xC0,
        0,
        ST7735S_COLMOD,
        1,
        0x05,
        0,
        ST7735S_NORON,
        0,
        0,
        ST7735S_DISPON,
        0,
        0
    };
    
    softSPI_enable_transmit(screen->pins);
    st7735s_cmdlist_helper(screen->pins, command_list);
    softSPI_disable_transmit(screen->pins);
}

void
st7735s_draw_slope_line(st7735s_Screen *screen, st7735s_LineObject *line, unsigned short int color) {
    st7735s_copyNotTempObj(line, line);

    signed short int judge_sign;
    signed char step_number;
    st7735s_screenMaxSize_t *x0_addr, *y0_addr;

    step_number = 1;
    if ( ((line->x0) < (line->x1)) && ((line->y0) > (line->y1)) ) {
        step_number = -step_number;
    }
    else if ( ((line->x0) > (line->x1)) && ((line->y0) > (line->y1)) ) {
        st7735s_swap_var(line->x0, line->x1);
        st7735s_swap_var(line->y0, line->y1);
    }
    else if ( ((line->x0) > (line->x1)) && ((line->y0) < (line->y1)) ) {
        st7735s_swap_var(line->x0, line->x1);
        st7735s_swap_var(line->y0, line->y1);
        step_number = -step_number;
    }

    if ((line->dx) < (line->dy)) {
        x0_addr = &(line->y0);
        y0_addr = &(line->x0);
        st7735s_swap_var(line->dx, line->dy);
        st7735s_swap_var(line->x0, line->y0);
        st7735s_swap_var(line->x1, line->y1);
    } else {
        x0_addr = &(line->x0);
        y0_addr = &(line->y0);
    }

    judge_sign = -(line->dx);
    for (; (line->x0) <= (line->x1); ++(line->x0)) {
        st7735s_draw_pixel(screen, *x0_addr, *y0_addr, color);
        judge_sign += ((line->dy) << 1);
        if (judge_sign > 0) {
            (line->y0) += step_number;
            judge_sign -= ((line->dx) << 1);
        }
    }

    st7735s_freeObj(line);
}

void
st7735s_draw_non_slope_line(st7735s_Screen *screen, st7735s_LineObject *line, unsigned short int color) {
    st7735s_copyNotTempObj(line, line);

    if ((line->x0) > (line->x1)) {
        st7735s_swap_var(line->x0, line->x1);
    }
    else if ((line->y0) > (line->y1)) {
        st7735s_swap_var(line->y0, line->y1);
    }

    for (; (line->x0) < (line->x1); ++(line->x0)) {
        st7735s_draw_pixel(screen, line->x0, line->y0, color);
    }
    for (; (line->y0) < (line->y1); ++(line->y0)) {
        st7735s_draw_pixel(screen, line->x0, line->y0, color);
    }

    st7735s_draw_pixel(screen, line->x1, line->y1, color);
    st7735s_freeObj(line);
}

void
st7735s_draw_square(st7735s_Screen *screen, st7735s_SquareObject *square, unsigned short int color) {
    st7735s_copyNotTempObj(square, square);

    st7735s_LineObject *lines[4] = {
        st7735s_createTempLineObj(square->x0, square->y0, square->x1, square->y0),
        st7735s_createTempLineObj(square->x0, square->y0, square->x0, square->y1),
        st7735s_createTempLineObj(square->x0, square->y1, square->x1, square->y1),
        st7735s_createTempLineObj(square->x1, square->y0, square->x1, square->y1)
    };

    unsigned char offset;
    st7735s_LineObject *line;
    for (offset = 0; offset < 4; ++offset) {
        line = *(lines + offset);
        st7735s_draw_non_slope_line(screen, line, color);
    }

    st7735s_freeObj(square);
}

void
st7735s_fill_screen(st7735s_Screen *screen, unsigned short int color) {
    st7735s_screenMaxSize_t x_start, y_start;

    x_start = screen->size->x_start;
    y_start = screen->size->y_start;

    softSPI_enable_transmit(screen->pins);
    st7735s_set_window_addr(screen, x_start, y_start, screen->size->x_end, screen->size->y_end);
    st7735s_set_SRAM_writable(screen->pins);

    for (; x_start < (screen->size->x_end); x_start++) {
        for (y_start = (screen->size->y_start); y_start < (screen->size->y_end); y_start++) {
            softSPI_write_color(screen->pins, color);
        }
    }

    softSPI_disable_transmit(screen->pins);
}

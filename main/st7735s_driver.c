// Private Includes:
#include "st7735s_driver.h"
// GPIO Includes:
#include "driver/gpio.h"
// FreeRTOS Includes:
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// Standard Includes:
#include <stdio.h>

static void
pins_init(st7735s_pins *pins, unsigned char pin_count) {
    unsigned long long pins_mask;
    unsigned char count, *pins_ptr;

    pins_ptr = (unsigned char *)pins;
    pins_mask = 0;
    for (count = 0; count < pin_count; ++count)
        pins_mask |= 1ULL << *(pins_ptr++);

    gpio_config_t conf;
    conf.mode = GPIO_MODE_OUTPUT;
    conf.pull_up_en = GPIO_PULLUP_DISABLE;
    conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    conf.intr_type = GPIO_INTR_DISABLE;
    conf.pin_bit_mask = pins_mask;
    gpio_config(&conf);
}

static void
cmd_list_helper(st7735s_pins *pins, unsigned char *cmd_list) {
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
        // printf("write command: %x\n", current_command);
        st7735s_write_command(pins, current_command);
        
        num_args = *(command_list_addr++);
        if (num_args) {
            while (num_args--) {
                current_argument = *(command_list_addr++);
                // printf("write argument: %x\n", current_argument);
                st7735s_write_data(pins, current_argument);
            }
        }
        
        delayms = *(command_list_addr++);
        if (delayms) {
            // printf("delay: %u ms\n", delayms);
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
st7735s_init(st7735s_pins *pins, st7735s_size *size, unsigned char pin_count) {
    pins_init(pins, pin_count);
    st7735s_powerctl(pins, 1);
    timesleep_ms(120);
    st7735s_hwreset(pins);

    unsigned char command_list[] = {
        8,
        ST7735S_SLPOUT,
        0,
        120,
        ST7735S_FRMCTR1,
        3,
        0x05, 0x3C, 0x3C,
        0,
        ST7735S_FRMCTR2,
        3,
        0x08, 0x3B, 0x3B,
        0,
        ST7735S_FRMCTR3,
        6,
        0x05, 0x3C, 0x3C,
        0x05, 0x3C, 0x3C,
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
        ST7735S_DISPON,
        0,
        0
    };
    
    st7735s_enable_transmit(pins);
    cmd_list_helper(pins, command_list);
    st7735s_disable_transmit(pins);
}

void
st7735s_set_window_addr(
        st7735s_pins *pins,
        unsigned char x1,
        unsigned char y1,
        unsigned char x2,
        unsigned char y2) {

    unsigned char cmd_list[] = {
        2,
        ST7735S_CASET,
        4,
        0x00, x1, 0x00, x2,
        0,
        ST7735S_RASET,
        4,
        0x00, y1, 0x00, y2,
        0
    };

    cmd_list_helper(pins, cmd_list);
}

void
st7735s_push_color(st7735s_pins *pins, unsigned short int color) {
    st7735s_write_data(pins, color >> 8);
    st7735s_write_data(pins, color);
}

void
st7735s_fill_screen(st7735s_pins *pins, st7735s_size *size, unsigned short int color) {
    unsigned char x, y, hicolor, locolor;

    hicolor = color >> 8;
    locolor = color;
    
    // printf("width: %u; height: %u\n", size->width, size->height);
    st7735s_enable_transmit(pins);
    st7735s_set_window_addr(pins, 0, 0, size->width, size->height);
    st7735s_write_command(pins, ST7735S_RAMWR);
    for (x = 0; x < (size->width); ++x) {
        for (y = 0; y < (size->height); ++y) {
            st7735s_write_data(pins, hicolor);
            st7735s_write_data(pins, locolor);
        }
    }
    st7735s_disable_transmit(pins);
}


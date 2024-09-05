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
pins_init(st7735s_pins *pins) {
    unsigned long long pins_mask;
    unsigned char count, *pins_ptr;

    pins_ptr = (unsigned char *)pins;
    pins_mask = 0;
    for (count = 0; count < ST7735S_PINS_NUM; ++count)
        pins_mask |= 1ULL << *(pins_ptr++);

    gpio_config_t conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = pins_mask
    }; gpio_config(&conf);
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
        st7735s_write_command(pins, current_command);
        
        num_args = *(command_list_addr++);
        if (num_args) {
            while (num_args--) {
                current_argument = *(command_list_addr++);
                st7735s_write_data(pins, current_argument);
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
st7735s_init(st7735s_pins *pins, st7735s_size *size) {
    pins_init(pins);
    st7735s_powerctl(pins, 1);
    timesleep_ms(120);
    st7735s_hwreset(pins);

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
    
    st7735s_enable_transmit(pins);
    cmd_list_helper(pins, command_list);
    st7735s_disable_transmit(pins);
}

void
st7735s_set_window_addr(
        st7735s_pins *pins,
        unsigned char x0,
        unsigned char y0,
        unsigned char x1,
        unsigned char y1) {

    unsigned char cmd_list[] = {
        2,
        ST7735S_CASET,
        4,
        0x00, x0, 0x00, (x1 - 1),
        0,
        ST7735S_RASET,
        4,
        0x00, y0, 0x00, (y1 - 1),
        0
    };

    cmd_list_helper(pins, cmd_list);
}

void
st7735s_putchar(st7735s_pins *pins, unsigned char char_, unsigned short int color) {
    unsigned char count;
    for (count = 0; count < 8; ++count) {
        if (char_ & 0x80) {
            st7735s_write_color(pins, color);
        } else {
            st7735s_write_color(pins, 0x0000);
        }
        char_ <<= 1;
    }
}

void
st7735s_test_putchar(st7735s_pins *pins) {
    unsigned char CHAR_A[] = {
        0x00, 0x00, 
        0x0e, 0x00, 
        0x0e, 0x00, 
        0x0b, 0x00, 
        0x1b, 0x00, 
        0x13, 0x00, 
        0x11, 0x80, 
        0x3f, 0x80, 
        0x21, 0x80, 
        0x20, 0xc0, 
        0x60, 0xc0, 
        0xf1, 0xe0, 
        0x00, 0x00, 
        0x00, 0x00, 
        0x00, 0x00, 
        0x00, 0x00
    };
    st7735s_enable_transmit(pins);
    st7735s_set_window_addr(pins, 16, 16, 32, 32);
    st7735s_set_SRAM_writable(pins);

    unsigned char count, *char_addr;
    char_addr = CHAR_A;
    for (count = 0; count < 32; ++count) {
        st7735s_putchar(pins, *(char_addr++), 0xFFFF);
    }

    st7735s_disable_transmit(pins);
}

void
st7735s_fill_screen(st7735s_pins *pins, st7735s_size *size, unsigned short int color, st7735s_buffer *buffer) {
    unsigned char x, y;

    st7735s_enable_transmit(pins);
    st7735s_set_window_addr(pins, 0, 0, (size->width), (size->height));
    st7735s_set_SRAM_writable(pins);
    for (x = 0; x < (size->width); ++x) {
        for (y = 0; y < (size->height); ++y) {
            if (buffer != NULL) {
                st7735s_buffer_write(pins, buffer, color);
            } else {
                st7735s_write_color(pins, color);
            }
        }
    }
    if (buffer != NULL) {
        st7735s_buffer_commit(pins, buffer);
    }
    st7735s_disable_transmit(pins);
}


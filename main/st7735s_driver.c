// Private Includes:
#include "st7735s_driver.h"
// GPIO Includes:
#include "driver/gpio.h"
#include "hal/gpio_types.h"
// FreeRTOS Includes:
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "portmacro.h"
// Standard Includes:
// #include <stdio.h>

static void
timesleep_ms(unsigned int ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

static void
st7735s_spi_init(st7735s_pins *pins, unsigned char pin_count) {
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

void
st7735s_init(st7735s_pins *pins, st7735s_window_size *size, unsigned char pin_count) {
    st7735s_spi_init(pins, pin_count);

    unsigned char command_list[] = {
        5,
        ST7735S_SWRESET,
        0,
        120,
        ST7735S_SLPOUT,
        0,
        120,
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
    
    unsigned char num_commands,
                  num_args,
                  delayms,
                  *command_list_addr;

    command_list_addr = command_list;
    num_commands = *(command_list_addr++);
    while (num_commands--) {
        st7735s_write_command(pins, *(command_list_addr++));
        // printf("command: %u\n", *(command_list_addr++));
        
        num_args = *(command_list_addr++);
        if (num_args) {
            while (num_args--) {
                st7735s_write_data(pins, *(command_list_addr++));
                // printf("args: %u\n", *(command_list_addr++));
            }
        }
        
        delayms = *(command_list_addr++);
        if (delayms) {
            // printf("delay: %u (ms)\n", delayms);
            timesleep_ms(delayms);
        }
    }
}

void
st7735s_blkctl(st7735s_pins *pins, unsigned char state) {
    gpio_set_level(pins->BLK, state);
}

void
st7735s_write_byte(st7735s_pins *pins, unsigned char byte) {
    unsigned char first_bit_mask = 0x80,
                  count = 0;

    for ( ; count < 8; ++count) {
        gpio_set_level(pins->SCL, 0);
        gpio_set_level(pins->SDA, byte & first_bit_mask);
        gpio_set_level(pins->SCL, 1);
        byte <<= 1;
    }
}

void
st7735s_write_data(st7735s_pins *pins, unsigned char data) {
    gpio_set_level(pins->DC, 1);
    st7735s_write_byte(pins, data);
}

void
st7735s_write_command(st7735s_pins *pins, unsigned char command) {
    gpio_set_level(pins->DC, 0);
    st7735s_write_byte(pins, command);
}

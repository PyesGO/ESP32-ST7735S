#include "st7735s_driver.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "portmacro.h"

static void
delay_ms(unsigned int ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

static void
st7735s_spi_init(st7735_pins *pins) {
    unsigned long long pins_mask;
    unsigned char count, *pins_ptr;

    pins_ptr = (unsigned char *)pins;
    pins_mask = *pins_ptr;
    count = 1;
    for ( ; count < PIN_NUM; ++count)
        pins_mask |= 1ULL << *(pins_ptr++);

    gpio_config_t conf;
    conf.mode = GPIO_MODE_OUTPUT;
    conf.pull_up_en = GPIO_PULLUP_DISABLE;
    conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    conf.pin_bit_mask = pins_mask;

    gpio_config(&conf);
}

void
st7735s_init(st7735_pins *pins) {
    st7735s_spi_init(pins);
    unsigned char command_list[] = {};
    st7735s_write_command(pins, ST7735S_SWRESET);
}

void
st7735s_write_bytes(st7735_pins *pins, unsigned char bytes) {
    unsigned char first_bit_mask = 0x80,
                  count = 0;

    for ( ; count < 8; ++count) {
        gpio_set_level(pins->SCL, 0);
        gpio_set_level(pins->SDA, bytes & first_bit_mask);
        gpio_set_level(pins->SCL, 1);
        bytes <<= 1;
    }
}

void
st7735s_write_data(st7735_pins *pins, unsigned char data) {
    gpio_set_level(pins->DC, 1);
    st7735s_write_bytes(pins, data);
}

void
st7735s_write_command(st7735_pins *pins, unsigned char command) {
    gpio_set_level(pins->DC, 0);
    st7735s_write_bytes(pins, command);
}

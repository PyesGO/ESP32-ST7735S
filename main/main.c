#include "st7735s_driver.h"

void
app_main(void) {
    st7735s_pins pins = {
        .VCC = 13,
        .SCL = 12,
        .SDA = 14,
        .RES = 27,
        .DC = 26,
        .CS = 25,
        .BLK = 33
    };
    st7735s_size size = {
        .width = 132,
        .height = 132
    };
    st7735s_size size2 = {
        .width = 16,
        .height = 16
    };
    st7735s_buffer buffer = st7735s_buffer_init(2048);

    st7735s_init(&pins, &size);
    st7735s_fill_screen(&pins, &size, 0x0000, &buffer);
    st7735s_buffer_free(&buffer);
    st7735s_blkctl(&pins, 1);
    st7735s_fill_screen(&pins, &size2, 0xFFFF, NULL);
    st7735s_test_putchar(&pins);
    timesleep_ms(6000);

    // st7735s_fill_screen(&pins, &size, 0xE321, &buffer);
    // timesleep_ms(2000);
    // st7735s_fill_screen(&pins, &size, 0xD321, &buffer);
    // st7735s_buffer_free(&buffer);
    // timesleep_ms(2000);

    st7735s_blkctl(&pins, 0);
    st7735s_powerctl(&pins, 0);
}

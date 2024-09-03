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
    st7735s_buffer buffer = st7735s_buffer_init(2048);

    st7735s_init(&pins, &size);
    st7735s_fill_screen(&pins, &size, 0x00, &buffer);
    st7735s_buffer_free(&buffer);
    st7735s_blkctl(&pins, 1);

    timesleep_ms(5000);
    st7735s_blkctl(&pins, 0);
    st7735s_powerctl(&pins, 0);
}

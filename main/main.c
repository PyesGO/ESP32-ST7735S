#include "st7735s_driver.h"

void
app_main(void) {
    st7735s_pins pins = {
        .SCL = 15,
        .SDA = 2,
        .RES = 4,
        .DC = 16,
        .CS = 17
    };
    st7735s_size size = {
        .width = 132,
        .height = 132
    };
    st7735s_buffer buffer = st7735s_buffer_init(256);

    st7735s_init(&pins, &size);
    st7735s_fill_screen(&pins, &size, 0x0000, &buffer);
    st7735s_blkctl(1);
    st7735s_buffer_free(&buffer);

    st7735s_draw_square(&pins, st7735s_createTempSquareObj(20, 20, 30, 30), 0xFFFF);

    timesleep_ms(30000);

    st7735s_blkctl(0);
}

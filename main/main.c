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
    st7735s_buffer buffer = st7735s_buffer_init(256);
    
    st7735s_init(&pins, &size);
    st7735s_fill_screen(&pins, &size, 0x0000, &buffer);
    st7735s_blkctl(&pins, 1);
    st7735s_buffer_free(&buffer);

    st7735s_draw_line(&pins, st7735s_createTempLineObj(10, 10, 70, 50), 0xFFFF);
    st7735s_draw_line(&pins, st7735s_createTempLineObj(20, 20, 50, 50), 0xFFFF);
    st7735s_draw_line(&pins, st7735s_createTempLineObj(30, 30, 40, 60), 0x6400);
    st7735s_draw_line(&pins, st7735s_createTempLineObj(120, 10, 80, 40), 0xFC00);
    st7735s_draw_line(&pins, st7735s_createTempLineObj(132, 132, 100, 120), 0xFF00);

    // st7735s_draw_square(&pins, st7735s_createTempSquareObj(20, 20, 20, 20), 0x6400);
    // st7735s_draw_square(&pins, st7735s_createTempSquareObj(20, 50, 30, 30), 0xFF00);
    // st7735s_draw_square(&pins, st7735s_createTempSquareObj(20, 90, 30, 20), 0xFC00);

    timesleep_ms(30000);

    st7735s_blkctl(&pins, 0);
    st7735s_powerctl(&pins, 0);
}

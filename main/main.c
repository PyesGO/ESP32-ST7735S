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
    st7735s_buffer buffer = st7735s_buffer_init(512);

    st7735s_init(&pins, &size);
    st7735s_fill_screen(&pins, &size, 0x0000, &buffer);
    st7735s_blkctl(&pins, 1);
    st7735s_buffer_free(&buffer);

    st7735s_LineObject *line1 = st7735s_createLineObj(20, 20, 20, 50),
                       *line2 = st7735s_createLineObj(20, 20, 50, 20);
    st7735s_draw_line(&pins, line1, 0xFFFF);
    st7735s_draw_line(&pins, line2, 0xFFFF);
    st7735s_freeObj(line1);
    st7735s_freeObj(line2);
    timesleep_ms(6000);

    st7735s_blkctl(&pins, 0);
    st7735s_powerctl(&pins, 0);
}

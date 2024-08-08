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
        .width = 16,
        .height = 16
    };

    st7735s_init(&pins, &size, ST7735S_PIN_COUNT(pins));
    st7735s_blkctl(&pins, 1);
    st7735s_fill_screen(&pins, &size, 0xF800);
    timesleep_ms(30000);
    st7735s_blkctl(&pins, 0);
}

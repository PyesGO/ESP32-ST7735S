#include "st7735s_driver.h"

void
app_main(void) {
    st7735s_pins pins = {
        .SCL = 4,
        .SDA = 16,
        .RES = 23,
        .DC = 19,
        .CS = 21,
        .BLK = 5
    };
    st7735s_window_size size = {
        .width = 128,
        .height = 128
    };

    st7735s_init(&pins, &size, ST7735S_PIN_COUNT(pins));
    st7735s_blkctl(&pins, 1);
    st7735s_fill_window(&pins, 0x07E0);
    timesleep_ms(8000);
    st7735s_blkctl(&pins, 0);
}

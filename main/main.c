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
    
    unsigned short int color;

    st7735s_init(&pins, &size, ST7735S_PIN_COUNT(pins));
    st7735s_blkctl(&pins, 1);

    color = 0;
    while (color < 0xFF) {
        st7735s_fill_screen(&pins, &size, color);
        color += 0x10;
        timesleep_ms(100);
    }

    st7735s_blkctl(&pins, 0);
    st7735s_powerctl(&pins, 0);
}

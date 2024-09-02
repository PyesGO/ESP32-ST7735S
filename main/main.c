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
    st7735s_buffer buffer = st7735s_buffer_init(4096);
    
    unsigned char count;
    unsigned short int color;

    st7735s_init(&pins, &size);
    st7735s_blkctl(&pins, 1);
    
    color = 0;
    for (count = 0; ((count < 50) && (color < 0xFFFF)); ++count) {
        st7735s_fill_screen(&pins, &size, color, &buffer);
        timesleep_ms(500);
        color += 0x20;
    }

    // st7735s_fill_screen(&pins, &size, 0x00, );
    // st7735s_draw_point(&pins, 66, 66, 0xFFFF);
    // timesleep_ms(10000);
    st7735s_buffer_free(&buffer);
    st7735s_blkctl(&pins, 0);
    timesleep_ms(120);
    st7735s_powerctl(&pins, 0);
}

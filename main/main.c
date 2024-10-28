#include "st7735s_driver.h"
#include "st7735s_fontlib.h"

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

    st7735s_init(&pins, &size);
    st7735s_blkctl(0);
    st7735s_fill_screen(&pins, &size, 0x0000);
    st7735s_blkctl(1);
    
    st7735s_LineObject *line1 = st7735s_createLineObj(10, 20, 30, 40);

    st7735s_test(&pins, line1);

    printf("main: x0: %u, x1: %u, y0: %u, y1:%u\n", line1->x0, line1->x1, line1->y0, line1->y1);

    st7735s_freeObj(line1);

    st7735s_font_write_text(&pins, 0, 0xFFFF);

    timesleep_ms(30000);

    st7735s_blkctl(0);
}

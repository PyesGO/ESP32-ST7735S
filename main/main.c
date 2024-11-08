#include "st7735s_driver.h"
#include "st7735s_fontlib.h"

void
app_main(void) {
    st7735s_Screen *screen;

    screen = st7735s_createScreenObj();
    st7735s_initScreenPins(screen, 15, 2, 4, 16, 17);
    st7735s_initScreenSize(screen, 3, 3, 128, 128);

    st7735s_initScreenState(screen);
    st7735s_blkctl(0);
    st7735s_fill_screen(screen, 0x0000);
    st7735s_blkctl(1);

    st7735s_draw_slope_line(screen, st7735s_createTempLineObj(20, 0, 40, 50), 0xFFFF);
    st7735s_draw_square(screen, st7735s_createTempSquareObj(20, 30, 40, 20), 0xFFFF);
    st7735s_font_write_text(screen, 4, 0xFFFF);

    st7735s_freeScreenObj(screen);

    timesleep_ms(30000);
    st7735s_blkctl(0);
}

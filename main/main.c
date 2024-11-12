#include "st7735s_driver.h"
#include "st7735s_font.h"


void
app_main(void) {
    st7735s_Screen *screen;
    
    screen = st7735s_createScreenObj();
    st7735s_initScreenPins(screen, 15, 2, 4, 16, 17);
    st7735s_initScreenSize(screen, 1, 1, 132, 132);

    st7735s_initScreenState(screen);
    st7735s_blkctl(0);
    st7735s_full_screen(screen, 0x0000);
    st7735s_blkctl(1);

    timesleep_ms(30000);
    st7735s_blkctl(0);
}

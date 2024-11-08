#include "st7735s_fontlib.h"
#include "st7735s_driver.h"
#include "softSPI_lib.h"

void
st7735s_font_write_text(st7735s_Screen *screen, unsigned char char_, unsigned short int color) {
    unsigned char byte_pixel, count, inter_count;
    const unsigned char *font_map_addr;

    font_map_addr = ST7735S_FONTS_MAP[char_];
    softSPI_enable_transmit(screen->pins);
    st7735s_set_window_addr(screen, 16, 16, 32, 32);
    st7735s_set_SRAM_writable(screen->pins);
    for (count = 0; count < 32; ++count) {
        byte_pixel = *(font_map_addr++);
        for (inter_count = 0; inter_count < 8; ++inter_count) {
            if (byte_pixel & 0x80) {
                softSPI_write_color(screen->pins, color);
            } else {
                softSPI_write_color(screen->pins, 0x0000);
            }
            byte_pixel <<= 1;
        }
    }
    softSPI_disable_transmit(screen->pins);
}

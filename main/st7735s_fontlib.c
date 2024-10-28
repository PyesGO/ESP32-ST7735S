#include "st7735s_fontlib.h"
#include "st7735s_driver.h"

void
st7735s_font_write_text(st7735s_pins *pins, unsigned char char_, unsigned short int color) {
    unsigned char byte_pixel, count, inter_count;
    const unsigned char *font_map_addr;

    font_map_addr = ST7735S_FONTS_MAP[char_];
    st7735s_enable_transmit(pins);
    st7735s_set_window_addr(pins, 16, 16, 31, 31);
    st7735s_set_SRAM_writable(pins);
    for (count = 0; count < 32; ++count) {
        byte_pixel = *(font_map_addr++);
        for (inter_count = 0; inter_count < 8; ++inter_count) {
            if (byte_pixel & 0x80) {
                st7735s_write_color(pins, color);
            } else {
                st7735s_write_color(pins, 0x0000);
            }
            byte_pixel <<= 1;
        }
    }
    st7735s_disable_transmit(pins);
}

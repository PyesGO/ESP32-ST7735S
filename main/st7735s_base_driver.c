struct _st7735s_pins;
typedef struct _st7735s_pins ST7735S_Pins_t;
#define TFT_SCREEN_PINS_TYPE ST7735S_Pins_t

#include "tft_screen.h"
#include "tft_screen_objects.h"

struct _st7735s_pins {
    unsigned char SCL, SDA, RES, DC, CS;
};


static void
init_screen_state(TFTScreen_ScreenObject *screen) {
}

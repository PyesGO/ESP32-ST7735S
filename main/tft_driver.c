#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "tft_driver.h"


static void
spi_init(ScreenPins *scp) {
    unsigned char count, *scp_pt;

    scp_pt = (unsigned char *)scp;

    for (count = 0; count < PIN_NUM; ++count)
        gpio_set_direction( *(scp_pt++), GPIO_MODE_OUTPUT );
    
    gpio_set_level(scp->RES, 1);
    timesleep(200);
    gpio_set_level(scp->RES, 0);
    timesleep(20);
}

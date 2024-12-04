#include "freertos/FreeRTOS.h"

void
driver_common_time_sleep_ms(unsigned int ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

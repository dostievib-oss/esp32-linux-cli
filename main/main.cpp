#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "ESP32_LINUX_CLI";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "=== ESP32 Linux CLI v0.1 ===");
    ESP_LOGI(TAG, "System Ready!");

    int counter = 0;
    while(1) {
        ESP_LOGI(TAG, "Heartbeat #%d", counter++);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

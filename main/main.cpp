#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "ESP32_LINUX_CLI";

// ASCII Art
const char* ascii_art = 
"\n"
"  _____ ___________   _____           _ \n"
" |  ___/  ___| ___ \\ |_   _|         | |\n"
" | |__ \\ `--.| |_/ /   | | ___   ___ | |\n"
" |  __| `--. \\  __/    | |/ _ \\ / _ \\| |\n"
" | |___/\\__/ / |       | | (_) | (_) | |\n"
" \\____/\\____/\\_|       \\_/\\___/ \\___/|_|\n"
"\n"
"  ========== LINUX BRIDGE ==========\n"
"\n";

extern "C" void app_main(void)
{
    // Печатаем ASCII Art
    printf("%s", ascii_art);

    ESP_LOGI(TAG, "ESP32 Linux CLI v1.0");
    ESP_LOGI(TAG, "System Ready!");
    ESP_LOGI(TAG, "Waiting for commands...");

    int counter = 0;
    while(1) {
        ESP_LOGI(TAG, "Heartbeat #%d", counter++);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

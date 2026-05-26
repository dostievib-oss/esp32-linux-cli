#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "ESP32_CLI";

#define UART_NUM UART_NUM_0
#define BUF_SIZE 128

// ASCII Art логотип
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
"  Ready to accept commands!\n"
"\n";

void handle_command(char *cmd) {
    cmd[strcspn(cmd, "\n\r")] = 0;
    
    if (strncmp(cmd, "pin", 3) == 0) {
        int pin;
        char action[10];
        sscanf(cmd, "pin %d %s", &pin, action);
        
        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << pin),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        gpio_config(&io_conf);
        
        if (strcmp(action, "on") == 0) {
            gpio_set_level((gpio_num_t)pin, 1);
            printf("OK: Pin %d set to HIGH\n", pin);
        } else if (strcmp(action, "off") == 0) {
            gpio_set_level((gpio_num_t)pin, 0);
            printf("OK: Pin %d set to LOW\n", pin);
        }
    }
    else if (strncmp(cmd, "read", 4) == 0) {
        int pin;
        sscanf(cmd, "read %d", &pin);
        
        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << pin),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        gpio_config(&io_conf);
        
        int level = gpio_get_level((gpio_num_t)pin);
        printf("OK: Pin %d is %s\n", pin, level ? "HIGH" : "LOW");
    }
    else if (strcmp(cmd, "help") == 0) {
        printf("\nCommands: pin <num> on/off, read <num>, status, help\n");
    }
    else if (strcmp(cmd, "status") == 0) {
        uint32_t free_heap = esp_get_free_heap_size();
        printf("OK: ESP32 Linux CLI v1.0 | Free heap: %lu bytes\n", (unsigned long)free_heap);
    }
    else if (strlen(cmd) > 0) {
        printf("ERROR: Unknown command: %s\n", cmd);
    }
}

extern "C" void app_main(void)
{
    printf("%s", ascii_art);
    
    // Инициализация UART
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_APB,
        .flags = 0,
    };
    
    ESP_LOGI(TAG, "Initializing UART...");
    uart_param_config(UART_NUM, &uart_config);
    uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
    ESP_LOGI(TAG, "UART initialized successfully");
    
    ESP_LOGI(TAG, "System ready! Waiting for commands...");
    
    uint8_t data[BUF_SIZE];
    
    while(1) {
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, 100 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0';
            ESP_LOGI(TAG, "Received: %s", data);
            handle_command((char*)data);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
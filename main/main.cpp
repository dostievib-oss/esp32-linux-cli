#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"

static const char *TAG = "ESP32_CLI";

#define UART_NUM UART_NUM_0
#define BUF_SIZE 128

// Массив для хранения состояния пинов (0=OFF, 1=ON)
uint8_t pin_states[40] = {0};

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

void setup_pwm(int pin, int duty) {
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer_conf);
    
    ledc_channel_config_t channel_conf = {
        .gpio_num = pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = (uint32_t)duty,
        .hpoint = 0,
    };
    ledc_channel_config(&channel_conf);
}

void handle_block_command(char *cmd) {
    char *equal = strchr(cmd, '=');
    if (!equal) return;
    
    *equal = '\0';
    char *pins_str = cmd + 5;
    char *action = equal + 1;
    
    char *pin = strtok(pins_str, ",");
    while (pin != NULL) {
        int pin_num = atoi(pin);
        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << pin_num),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        gpio_config(&io_conf);
        
        if (strcmp(action, "on") == 0) {
            gpio_set_level((gpio_num_t)pin_num, 1);
            pin_states[pin_num] = 1;
            printf("OK: Pin %d set to HIGH\n", pin_num);
        } else if (strcmp(action, "off") == 0) {
            gpio_set_level((gpio_num_t)pin_num, 0);
            pin_states[pin_num] = 0;
            printf("OK: Pin %d set to LOW\n", pin_num);
        }
        pin = strtok(NULL, ",");
    }
}

void handle_command(char *cmd) {
    cmd[strcspn(cmd, "\n\r")] = 0;
    
    if (strncmp(cmd, "pins", 4) == 0) {
        handle_block_command(cmd);
    }
    else if (strncmp(cmd, "pwm", 3) == 0) {
        int pin, duty;
        sscanf(cmd, "pwm %d %d", &pin, &duty);
        if (duty < 0) duty = 0;
        if (duty > 1023) duty = 1023;
        setup_pwm(pin, duty);
        printf("OK: PWM on pin %d set to %d/1023\n", pin, duty);
    }
    else if (strncmp(cmd, "pin", 3) == 0) {
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
            pin_states[pin] = 1;
            printf("OK: Pin %d set to HIGH\n", pin);
        } else if (strcmp(action, "off") == 0) {
            gpio_set_level((gpio_num_t)pin, 0);
            pin_states[pin] = 0;
            printf("OK: Pin %d set to LOW\n", pin);
        }
    }
    else if (strncmp(cmd, "read", 4) == 0) {
        int pin;
        sscanf(cmd, "read %d", &pin);
        // Читаем из сохранённого состояния, а не из gpio_get_level
        printf("OK: Pin %d is %s\n", pin, pin_states[pin] ? "HIGH" : "LOW");
    }
    else if (strcmp(cmd, "status") == 0) {
        uint32_t free_heap = esp_get_free_heap_size();
        printf("OK: ESP32 Linux CLI v1.1 | Free heap: %lu bytes | PWM: ON\n", (unsigned long)free_heap);
    }
    else if (strcmp(cmd, "help") == 0) {
        printf("\n=== ESP32 Linux CLI Commands ===\n");
        printf("  pin <num> on/off      - Set single GPIO\n");
        printf("  pins <n>,<n>=on/off   - Set multiple GPIOs\n");
        printf("  pwm <num> <0-1023>    - Set PWM duty cycle\n");
        printf("  read <num>            - Read GPIO state\n");
        printf("  status                - Show system info\n");
        printf("  help                  - Show this help\n");
        printf("================================\n");
    }
    else if (strlen(cmd) > 0) {
        printf("ERROR: Unknown command. Type 'help'\n");
    }
}

extern "C" void app_main(void)
{
    printf("%s", ascii_art);
    
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    
    uart_param_config(UART_NUM, &uart_config);
    uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
    
    ESP_LOGI(TAG, "System ready!");
    
    uint8_t data[BUF_SIZE];
    
    while(1) {
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, 100 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0';
            handle_command((char*)data);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
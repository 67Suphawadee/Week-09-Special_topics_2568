#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// กำหนดค่าคงที่สำหรับ LED 3 ดวง
#define LED1_GPIO   GPIO_NUM_2  // LSB (Least Significant Bit)
#define LED2_GPIO   GPIO_NUM_4
#define LED3_GPIO   GPIO_NUM_5  // MSB (Most Significant Bit)

#define COUNT_DELAY_MS 1000

static const char *TAG = "BINARY_COUNTER";

/**
 * @brief กำหนดค่าเริ่มต้นสำหรับ GPIOs
 */
void leds_init(void) {
    ESP_LOGI(TAG, "Initializing LEDs for Binary Counter");
    
    gpio_config_t io_conf = {0};
    io_conf.pin_bit_mask = (1ULL << LED1_GPIO) | (1ULL << LED2_GPIO) | (1ULL << LED3_GPIO);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    
    // ตั้งค่าเริ่มต้นให้ LED ดับทั้งหมด
    gpio_set_level(LED1_GPIO, 0);
    gpio_set_level(LED2_GPIO, 0);
    gpio_set_level(LED3_GPIO, 0);
    
    ESP_LOGI(TAG, "LEDs initialization completed");
}

/**
 * @brief Task สำหรับนับเลขฐานสองด้วย LED
 */
void binary_counter_task(void *pvParameters) {
    int count = 0;
    while (1) {
        // วนลูปการนับจาก 0 ถึง 7
        for (count = 0; count < 8; count++) {
            // ตั้งค่า LED ตามบิตของตัวเลข
            gpio_set_level(LED1_GPIO, (count & 1) ? 1 : 0); // Bit 0 (LSB)
            gpio_set_level(LED2_GPIO, (count & 2) ? 1 : 0); // Bit 1
            gpio_set_level(LED3_GPIO, (count & 4) ? 1 : 0); // Bit 2 (MSB)
            
            ESP_LOGI(TAG, "Count: %d | Binary: %d%d%d", 
                     count, 
                     (count & 4) >> 2, 
                     (count & 2) >> 1, 
                     (count & 1));

            vTaskDelay(pdMS_TO_TICKS(COUNT_DELAY_MS));
        }
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "ESP32 Binary Counter Demo Started");
    
    // Initialize LEDs
    leds_init();
    
    // Create Binary Counter task
    xTaskCreate(binary_counter_task, "binary_counter_task", 2048, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "Binary Counter task created");
}

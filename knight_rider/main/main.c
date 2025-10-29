#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// กำหนดค่าคงที่สำหรับ LED 3 ดวง
#define LED1_GPIO   GPIO_NUM_2
#define LED2_GPIO   GPIO_NUM_4
#define LED3_GPIO   GPIO_NUM_5
#define SCAN_DELAY_MS 150

static const char *TAG = "KNIGHT_RIDER";

// อาร์เรย์ของ LED GPIOs สำหรับสร้างลำดับการวิ่ง
static const gpio_num_t leds[] = {LED1_GPIO, LED2_GPIO, LED3_GPIO, LED2_GPIO};
static const int num_leds_in_sequence = sizeof(leds) / sizeof(leds[0]);

/**
 * @brief กำหนดค่าเริ่มต้นสำหรับ GPIOs
 */
void leds_init(void) {
    ESP_LOGI(TAG, "Initializing LEDs for Knight Rider effect");
    
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
 * @brief Task สำหรับสร้างเอฟเฟกต์ไฟวิ่ง Knight Rider
 */
void knight_rider_task(void *pvParameters) {
    while (1) {
        // วนลูปตามลำดับในอาร์เรย์
        for (int i = 0; i < num_leds_in_sequence; i++) {
            gpio_set_level(leds[i], 1); // เปิด LED ดวงปัจจุบัน
            ESP_LOGI(TAG, "LED on GPIO %d is ON", leds[i]);
            vTaskDelay(pdMS_TO_TICKS(SCAN_DELAY_MS));
            gpio_set_level(leds[i], 0); // ปิด LED ดวงปัจจุบัน
        }
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "ESP32 Knight Rider Demo Started");
    
    // Initialize LEDs
    leds_init();
    
    // Create Knight Rider task
    xTaskCreate(knight_rider_task, "knight_rider_task", 2048, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "Knight Rider task created");
}

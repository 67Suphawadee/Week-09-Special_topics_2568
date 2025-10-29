#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// กำหนดค่าคงที่สำหรับ LED 3 ดวง
#define LED1_GPIO   GPIO_NUM_2
#define LED2_GPIO   GPIO_NUM_4
#define LED3_GPIO   GPIO_NUM_5
#define BLINK_PERIOD 1000

static const char *TAG = "MULTIPLE_LEDS";

/**
 * @brief กำหนดค่าเริ่มต้นสำหรับ GPIOs
 */
void leds_init(void) {
    ESP_LOGI(TAG, "Initializing LEDs on GPIO %d, %d, %d", LED1_GPIO, LED2_GPIO, LED3_GPIO);
    
    gpio_config_t io_conf = {0};
    // กำหนด pin ที่ต้องการใช้ (bit mask)
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
 * @brief Task สำหรับกระพริบ LED ทั้ง 3 ดวงพร้อมกัน
 */
void leds_blink_task(void *pvParameters) {
    bool led_state = false;
    
    while (1) {
        // สลับสถานะ LED
        led_state = !led_state;
        gpio_set_level(LED1_GPIO, led_state);
        gpio_set_level(LED2_GPIO, led_state);
        gpio_set_level(LED3_GPIO, led_state);
        
        ESP_LOGI(TAG, "LEDs are %s", led_state ? "ON" : "OFF");
        
        // หน่วงเวลา
        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD));
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "ESP32 Multiple LEDs Control Demo Started");
    
    // Initialize LEDs
    leds_init();
    
    // Create LED blink task
    xTaskCreate(leds_blink_task, "leds_blink_task", 2048, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "LEDs blink task created");
}

/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"

#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "driver/i2c.h"

void app_main(void);

// EEPROM storage
#include "nvs_flash.h"
#include "nvs.h"
// ESP-NOW
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_mac.h"
// Motor DC Drive and control
#include "MX1508.h"
// Servo control
#include "Servo.h"
// MPU9250 module
#include "ahrs.h"
#include "mpu9250.h"
#include "calibrate.h"
#include "common.h"

#include "LEDs.h"

/* Test functions */
void leda(void *arg);
void ledb(void *arg);

void app_main(void)
{
    printf("Hello world!\n");

    ConfigLEDs();
    
    xTaskCreatePinnedToCore(&leda, "dfa", 4096, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(&ledb, "sdg", 4096, NULL, 5, NULL, 1);

    //while (1)
    //{        
        // gpio_set_level(LED_RED_PIN, HIGH);
        // gpio_set_level(LED_GREEN_PIN, LOW);
        // gpio_set_level(LED_BLUE_PIN, LOW);

        // gpio_set_level(LED_RED_PIN, LOW);
        // gpio_set_level(LED_GREEN_PIN, HIGH);
        // gpio_set_level(LED_BLUE_PIN, LOW);

        // gpio_set_level(LED_RED_PIN, LOW);
        // gpio_set_level(LED_GREEN_PIN, LOW);
        // gpio_set_level(LED_BLUE_PIN, HIGH);
    //}
}

void leda(void *arg)
{
    while (true)
    {
        ESP_ERROR_CHECK(gpio_set_level(LED_HEADLIGHT_PIN, gpio_get_level(LED_HEADLIGHT_PIN) ^ 1));
        ESP_ERROR_CHECK(gpio_set_level(LED_RIGHT_PIN, gpio_get_level(LED_RIGHT_PIN) ^ 1));
        ESP_ERROR_CHECK(gpio_set_level(LED_LEFT_PIN, gpio_get_level(LED_LEFT_PIN) ^ 1));
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void ledb(void *arg)
{
    uint32_t i = 0;

    while (true)
    {
        ESP_ERROR_CHECK(ledc_set_duty(SPEED_MODE, BRAKE_CHANNEL, i));
        ESP_ERROR_CHECK(ledc_update_duty(SPEED_MODE, BRAKE_CHANNEL));
        
        i += 25;
        if (i >= 256) i = 0;

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

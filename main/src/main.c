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
/* User Libraries */
// EEPROM storage
#include "nvs_flash.h"
#include "nvs.h"
// ESP-NOW
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_mac.h"
// lib
#include "LEDs.h"
#include "acc_gyr.h"
#include "OtherThings.h"
#include "iot_servo.h"

/*********************************************************************************/
//#define CHIP_INFO

static const char *TAG = "DriftCar";
static const char *VERSION = "0.0.0";

void app_main(void);

/* Test functions */
void led_test(void *arg);
void mpu_test(void *arg);
void SOC_test(void *arg);
void servo_test(void *arg);
void motor_test(void *arg);

void app_main(void)
{
    ESP_LOGI(TAG, "Project Version: %s", VERSION);    

    #ifdef CHIP_INFO
        /* Get Chip Information */
        esp_chip_info_t chip_info;
        uint32_t flash_size;
        esp_chip_info(&chip_info);

        printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
               CONFIG_IDF_TARGET,
               chip_info.cores,
               (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
               (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
               (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
               (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");
        unsigned major_rev = chip_info.revision / 100;
        unsigned minor_rev = chip_info.revision % 100;
        printf("silicon revision v%d.%d, ", major_rev, minor_rev);
        if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
            printf("Get flash size failed");
        printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
        (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
        printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
        fflush(stdout);
    #endif

    //Start_MotorDC();    

    xTaskCreatePinnedToCore(&led_test, "dfa", 4096, NULL, 3, NULL, 1);
    //xTaskCreatePinnedToCore(&mpu_test, "sfg", 4096, NULL, 3, NULL, 1);
    //xTaskCreatePinnedToCore(&SOC_test, "fgd", 4096, NULL, 5, NULL, 1);
    //xTaskCreatePinnedToCore(&servo_test, "sds", 4096, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(&motor_test, "motor", 4096, NULL, 5, NULL, 1);
}

void led_test(void *arg)
{
    int i = 0;

    ConfigLEDs();

    while (true)
    {
        ESP_ERROR_CHECK(gpio_set_level(LED_HEADLIGHT_PIN, gpio_get_level(LED_HEADLIGHT_PIN) ^ 1));
        ESP_ERROR_CHECK(gpio_set_level(LED_RIGHT_PIN, gpio_get_level(LED_RIGHT_PIN) ^ 1));
        ESP_ERROR_CHECK(gpio_set_level(LED_LEFT_PIN, gpio_get_level(LED_LEFT_PIN) ^ 1));
        
        ESP_ERROR_CHECK(ledc_set_duty(SPEED_MODE, BRAKE_CHANNEL, i));
        ESP_ERROR_CHECK(ledc_update_duty(SPEED_MODE, BRAKE_CHANNEL));

        i += 25;
        if (i >= 256)
            i = 0;

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void mpu_test(void *arg)
{
    calibration_t cal = {
        .mag_offset = {.x = 25.183594, .y = 57.519531, .z = -62.648438},
        .mag_scale = {.x = 1.513449, .y = 1.557811, .z = 1.434039},
        .accel_offset = {.x = 0.020900, .y = 0.014688, .z = -0.002580},
        .accel_scale_lo = {.x = -0.992052, .y = -0.990010, .z = -1.011147},
        .accel_scale_hi = {.x = 1.013558, .y = 1.011903, .z = 1.019645},
        .gyro_bias_offset = {.x = 0.303956, .y = -1.049768, .z = -0.403782}};

    //calibrate_gyro();
    //calibrate_accel();
    //calibrate_mag();

    vTaskDelay(pdMS_TO_TICKS(100));

    i2c_mpu9250_init(&cal);
    ahrs_init(SAMPLE_FREQ_Hz, 0.8);

    vTaskDelay(pdMS_TO_TICKS(100));

    while (true)
    {
        vector_t va, vg, vm;

        // Get the Accelerometer, Gyroscope and Magnetometer values.
        ESP_ERROR_CHECK(get_accel_gyro_mag(&va, &vg, &vm));
        //ESP_LOGI(TAG, "Before transform, accx: %2.3f, accy: %2.3f, accz: %2.3f\r\n", va.x, va.y, va.z);
        //ESP_LOGI(TAG, "Before transform, gyrx: %2.3f, gyry: %2.3f, gyrz: %2.3f\r\n", vg.x, vg.y, vg.z);
        //ESP_LOGI(TAG, "Before transform, magx: %2.3f, magy: %2.3f, magz: %2.3f\r\n", vm.x, vm.y, vm.z);
    
        // Transform these values to the orientation of our device.
        transform_accel_gyro(&va);
        transform_accel_gyro(&vg);
        transform_mag(&vm);

        //ESP_LOGI(TAG, "After transform, accx: %2.3f, accy: %2.3f, accz: %2.3f\r\n", va.x, va.y, va.z);
        //ESP_LOGI(TAG, "After transform, gyrx: %2.3f, gyry: %2.3f, gyrz: %2.3f\r\n", vg.x, vg.y, vg.z);
        //ESP_LOGI(TAG, "After transform, magx: %2.3f, magy: %2.3f, magz: %2.3f\r\n", vm.x, vm.y, vm.z);

        // Apply the AHRS algorithm
        ahrs_update(DEG2RAD(vg.x), DEG2RAD(vg.y), DEG2RAD(vg.z), va.x, va.y, va.z, vm.x, vm.y, vm.z);

        float temp;
        ESP_ERROR_CHECK(get_temperature_celsius(&temp));
        //printf("Temp: %f\r\n", temp);

        float heading, pitch, roll;
        ahrs_get_euler_in_degrees(&heading, &pitch, &roll);
        //ESP_LOGI(TAG, "heading: %2.3f°, pitch: %2.3f°, roll: %2.3f°, Temp %2.3f°C", heading, pitch, roll, temp);
    
        //println();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void SOC_test(void *arg)
{
    const int SAMPLES = 50;
    const float CALIBRATION_FACTOR = 1.068;

    adc1_config_width(SOC_WIDTH);
    adc1_config_channel_atten(SOC_ADC_CHANNEL, SOC_ATTENUATION);
    //adc_set_data_inv(ADC_UNIT_1, true);

    int val = 0;

    while (true)
    {
        for (int i = 0; i < SAMPLES; i++)
            val += adc1_get_raw(SOC_ADC_CHANNEL);
        val /= SAMPLES;
        
        uint16_t v = val;
        //uint16_t v = (uint16_t)~adc1_get_raw(SOC_ADC_CHANNEL) & 0b0000111111111111;
        //uint16_t v = (uint16_t)adc1_get_raw(SOC_ADC_CHANNEL);
        float vol = ((v * 8.4) / 4095.0) * CALIBRATION_FACTOR;
        uint8_t por = (int)((vol * 100) / 8.4);

        printf("Read: %d\r\n", v);
        printf("Volt: %.2f\r\n", vol);
        printf("Porc: %d\r\n", por);
        println();
        
        vTaskDelay(pdMS_TO_TICKS(750));
    } 
}

void servo_test(void *arg)
{
    servo_config_t Servos = {
        .max_angle    = MAX_ANGLE,
        .min_width_us = MIN_PULSEWIDTH,
        .max_width_us = MAX_PULSEWIDTH,
        .freq         = FREQ_SERVO,
        .timer_number = SERVO_TIMER,
        .channels = {
            .servo_pin = {
                SERVO_STEERING_WHELL,
                SERVO_HEAD_LIGHT,
            },
            .ch = {
                STEERING_CHANNEL,
                HEADLIGHT_CHANNEL,
            },
        },
        .channel_number = 2
    };

    if (iot_servo_init(SPEED_MODE, &Servos) == ESP_OK)
        printf("DEU BOM");

    while (true)
    {
        float read_angle1, read_angle2;
        for (int i = 0; i < 180; i++)
        {
            iot_servo_write_angle(SPEED_MODE, &Servos, STEERING_CHANNEL, i);
            iot_servo_write_angle(SPEED_MODE, &Servos, HEADLIGHT_CHANNEL, i);
            vTaskDelay(20 / portTICK_PERIOD_MS);
            iot_servo_read_angle(SPEED_MODE, &Servos, STEERING_CHANNEL, &read_angle1);
            iot_servo_read_angle(SPEED_MODE, &Servos, HEADLIGHT_CHANNEL, &read_angle2);
            ESP_LOGI(TAG, "[%d|%.2f|%.2f]", i, read_angle1, read_angle2);
        }
        //iot_servo_deinit(SPEED_MODE, &Servos);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void motor_test(void *arg)
{
    while (true)
    {
        printf("Test DC Motor");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

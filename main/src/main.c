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

#define println() (printf("\r\n"))
static const char *TAG = "MPU";

/**
 * Transformation:
 *  - Rotate around Z axis 180 degrees
 *  - Rotate around X axis -90 degrees
 * @param  {object} s {x,y,z} sensor
 * @return {object}   {x,y,z} transformed
 */
static void transform_accel_gyro(vector_t *v);

/**
 * Transformation: to get magnetometer aligned
 * @param  {object} s {x,y,z} sensor
 * @return {object}   {x,y,z} transformed
 */
static void transform_mag(vector_t *v);

/* Test functions */
void leda(void *arg);
void ledb(void *arg);
void mpu_test(void *arg);

void app_main(void)
{
    printf("Hello world!\n");

    ConfigLEDs();

    xTaskCreatePinnedToCore(&leda, "dfa", 4096, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(&ledb, "sdg", 4096, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(&mpu_test, "sfg", 4096, NULL, 5, NULL, 1);

    // while (1)
    //{
    //  gpio_set_level(LED_RED_PIN, HIGH);
    //  gpio_set_level(LED_GREEN_PIN, LOW);
    //  gpio_set_level(LED_BLUE_PIN, LOW);

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
        if (i >= 256)
            i = 0;

        vTaskDelay(pdMS_TO_TICKS(1000));
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

        ESP_LOGI(TAG, "After transform, accx: %2.3f, accy: %2.3f, accz: %2.3f\r\n", va.x, va.y, va.z);
        ESP_LOGI(TAG, "After transform, gyrx: %2.3f, gyry: %2.3f, gyrz: %2.3f\r\n", vg.x, vg.y, vg.z);
        ESP_LOGI(TAG, "After transform, magx: %2.3f, magy: %2.3f, magz: %2.3f\r\n", vm.x, vm.y, vm.z);

        // Apply the AHRS algorithm
        ahrs_update(DEG2RAD(vg.x), DEG2RAD(vg.y), DEG2RAD(vg.z), va.x, va.y, va.z, vm.x, vm.y, vm.z);

        float temp;
        ESP_ERROR_CHECK(get_temperature_celsius(&temp));
        //printf("Temp: %f\r\n", temp);

        float heading, pitch, roll;
        ahrs_get_euler_in_degrees(&heading, &pitch, &roll);
        ESP_LOGI(TAG, "heading: %2.3f°, pitch: %2.3f°, roll: %2.3f°, Temp %2.3f°C", heading, pitch, roll, temp);
    
        println();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void transform_accel_gyro(vector_t *v)
{
  float x = v->x;
  float y = v->y;
  float z = v->z;

  v->x = -x;
  v->y = -z;
  v->z = -y;
}

static void transform_mag(vector_t *v)
{
  float x = v->x;
  float y = v->y;
  float z = v->z;

  v->x = -y;
  v->y =  z;
  v->z = -x;
}

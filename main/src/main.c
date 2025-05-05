/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
/* User Libraries */
#include "DriftCar.h"
#include "LEDs.h"
#include "ACC.h"
#include "iot_servo.h"
#include "MX1508.h"

/*********************************** GLOBAL VARIABLES ***********************************/

// Project TAG and Version
static const char TAG[] = "DriftCar";
static const char VERSION[] = "0.0.0";

static DriftCar_ctx_t *DriftCar;

uint8_t broadcast_mac[ESP_NOW_ETH_ALEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/***************************************************************************************/


/*********************************** GLOBAL FUNCTIONS ***********************************/

/* Callbacks */
static void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status);
static void espnow_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len);

/* Test functions */
void led_test(void *arg);
void mpu_test(void *arg);
void servo_test(void *arg);
void motor_test(void *arg);
void espnow_test(void *arg);

/*****************************************************************************************/


void app_main(void)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);
    ESP_LOGI(TAG, "Project Version: %s", VERSION);

#if CONFIG_CHIP_INFO
    SHOW_CHIP_INFO();
#endif

    DriftCar = driftcar_create_app();

    // xTaskCreatePinnedToCore(&led_test, "led_test", 4096, NULL, 3, NULL, 1);
    // xTaskCreatePinnedToCore(&mpu_test, "sfg", 4096, NULL, 3, NULL, 1);
    // xTaskCreatePinnedToCore(&servo_test, "sds", 4096, NULL, 5, NULL, 1);
    // xTaskCreatePinnedToCore(&motor_test, "motor", 4096, NULL, 5, NULL, 1);
    // xTaskCreatePinnedToCore(&espnow_test, "wifi", 4096, NULL, 5, NULL, 0);

#if CONFIG_DEBUG_MEMORY
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        TaskHandle_t handler = soc_task_handler(); // Call your task handler to see the memory management
        SHOW_TASK_MEMORY(handler);
    }
#endif
}

void led_test(void *arg)
{
    uint32_t i = 0;
    uint8_t state = 0;
    LEDs_t *leds = config_LEDs();
    // LEDs_t leds = *(LEDs_t*)arg;

    while (true)
    {
        switch (state)
        {
            case 0:
            {
                printf("State 0: %ld\r\n", i);
                leds->blinky(leds->HEADLIGHT_PIN);
                leds->blinky(leds->RIGHT_PIN);
                leds->blinky(leds->LEFT_PIN);
        
                update_brake_duty(leds, i);
        
                i += 255;
                if (i >= (1UL << leds->duty_resolution))
                {
                    i = 0;
                    state++;
                }
                vTaskDelay(pdMS_TO_TICKS(100));
                break;
            }

            case 1:
            {                
                printf("State 1\r\n");
                leds->on(leds->HEADLIGHT_PIN);
                leds->on(leds->RIGHT_PIN);
                leds->on(leds->LEFT_PIN);
                brake_on(leds);
                vTaskDelay(pdMS_TO_TICKS(2000));
                state++;
                break;
            }

            case 2:
            {
                printf("State 2\r\n");
                leds->off(leds->HEADLIGHT_PIN);
                leds->off(leds->RIGHT_PIN);
                leds->off(leds->LEFT_PIN);
                brake_off(leds);
                vTaskDelay(pdMS_TO_TICKS(2000));
                state = 0;
                break;
            }
        }
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

    // calibrate_gyro();
    // calibrate_accel();
    // calibrate_mag();

    vTaskDelay(pdMS_TO_TICKS(100));

    i2c_mpu9250_init(&cal);
    ahrs_init(SAMPLE_FREQ_Hz, 0.8);

    vTaskDelay(pdMS_TO_TICKS(100));

    while (true)
    {
        vector_t va, vg, vm;

        // Get the Accelerometer, Gyroscope and Magnetometer values.
        ESP_ERROR_CHECK(get_accel_gyro_mag(&va, &vg, &vm));
        // ESP_LOGI(TAG, "Before transform, accx: %2.3f, accy: %2.3f, accz: %2.3f\r\n", va.x, va.y, va.z);
        // ESP_LOGI(TAG, "Before transform, gyrx: %2.3f, gyry: %2.3f, gyrz: %2.3f\r\n", vg.x, vg.y, vg.z);
        // ESP_LOGI(TAG, "Before transform, magx: %2.3f, magy: %2.3f, magz: %2.3f\r\n", vm.x, vm.y, vm.z);

        // Transform these values to the orientation of our device.
        transform_accel_gyro(&va);
        transform_accel_gyro(&vg);
        transform_mag(&vm);

        // ESP_LOGI(TAG, "After transform, accx: %2.3f, accy: %2.3f, accz: %2.3f\r\n", va.x, va.y, va.z);
        // ESP_LOGI(TAG, "After transform, gyrx: %2.3f, gyry: %2.3f, gyrz: %2.3f\r\n", vg.x, vg.y, vg.z);
        // ESP_LOGI(TAG, "After transform, magx: %2.3f, magy: %2.3f, magz: %2.3f\r\n", vm.x, vm.y, vm.z);

        // Apply the AHRS algorithm
        ahrs_update(DEG2RAD(vg.x), DEG2RAD(vg.y), DEG2RAD(vg.z), va.x, va.y, va.z, vm.x, vm.y, vm.z);

        float temp;
        ESP_ERROR_CHECK(get_temperature_celsius(&temp));
        // printf("Temp: %f\r\n", temp);

        float heading, pitch, roll;
        ahrs_get_euler_in_degrees(&heading, &pitch, &roll);
        // ESP_LOGI(TAG, "heading: %2.3f°, pitch: %2.3f°, roll: %2.3f°, Temp %2.3f°C", heading, pitch, roll, temp);

        // println();
        vTaskDelay(pdMS_TO_TICKS(500));
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
        .resolution   = SERVO_RESOLUTION,
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

    if (iot_servo_init(&Servos) == ESP_OK)
        printf("DEU BOM");

    while (true)
    {
        float read_angle1, read_angle2;
        for (int i = 0; i < 180; i++)
        {
            iot_servo_write_angle(&Servos, STEERING_CHANNEL, i);
            iot_servo_write_angle(&Servos, HEADLIGHT_CHANNEL, i);
            vTaskDelay(20 / portTICK_PERIOD_MS);
            iot_servo_read_angle(&Servos, STEERING_CHANNEL, &read_angle1);
            iot_servo_read_angle(&Servos, HEADLIGHT_CHANNEL, &read_angle2);
            ESP_LOGI(TAG, "[%d|%.2f|%.2f]", i, read_angle1, read_angle2);
        }
        // iot_servo_deinit(SPEED_MODE, &Servos);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void motor_test(void *arg)
{
    /*
    * @note: Tanto no sentido horario quanto no sentido anti-horario, o motor começa
    * a rotacionar somente a partir dos 300-400 de duty, ou, 2550-3400 RPM  
    */

    MX1508_config_t dc_motor = {
        .freq         = 2500,
        .timer_number = MOTOR_TIMER,
        .resolution   = MOTOR_RESOLUTION,
        .channels = {
            .motor_pin = {
                MOTOR_FOWARD_PIN,
                MOTOR_REVERSE_PIN,
            },
            
            .ch = {
                MOTOR_FOWARD_CHANNEL,
                MOTOR_REVERSE_CHANNEL,
            },
        },
        .channel_number = MX1508_UNIT,
    };

    if (MX1508_init(&dc_motor) == ESP_OK)
        printf("Deu bom");
    println();

    printf("%d - %d\r\n", DUTY_TO_RPM(300), DUTY_TO_RPM(400));

    while (true)
    {
        motorGo(RPM_TO_DUTY(2550));
        delay(5000);

        motorGo(RPM_TO_DUTY(3400));
        delay(5000);

        uint32_t read_duty1, read_duty2;
        for (int i = 0; i < (1 << MOTOR_RESOLUTION) - 1; i += 50)
        {
            motorGo(i);
            vTaskDelay(pdMS_TO_TICKS(1000));
            MX1508_read_duty(MOTOR_FOWARD_CHANNEL, &read_duty1);
            MX1508_read_duty(MOTOR_REVERSE_CHANNEL, &read_duty2);
            ESP_LOGI(TAG, "[%d|%ld|%ld]", i, read_duty1, read_duty2);
        }

        motorStop();
        vTaskDelay(pdMS_TO_TICKS(200));

        for (int i = 0; i < (1 << MOTOR_RESOLUTION) - 1; i += 50)
        {
            motorRev(i);
            vTaskDelay(pdMS_TO_TICKS(1000));
            MX1508_read_duty(MOTOR_FOWARD_CHANNEL, &read_duty1);
            MX1508_read_duty(MOTOR_REVERSE_CHANNEL, &read_duty2);
            ESP_LOGI(TAG, "[%d|%ld|%ld]", i, read_duty1, read_duty2);
        }

        motorBrake();
        vTaskDelay(pdMS_TO_TICKS(200));

        uint32_t read_duty;
        ESP_LOGI(TAG, "RPM em Duty: [%d]", RPM_TO_DUTY(MOTOR_MAX_RPM));
        motorGo(RPM_TO_DUTY(MOTOR_MAX_RPM));
        MX1508_read_duty(MOTOR_FOWARD_CHANNEL, &read_duty);
        ESP_LOGI(TAG, "Duty em RPM: [%d]", DUTY_TO_RPM(read_duty));

        // MX1508_deinit(&dc_motor);
        vTaskDelay(pdMS_TO_TICKS(5000));
        motorStop();
    }
}

/* Core 0 */
void espnow_test(void *arg)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_channel(CONFIG_ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE));

#if CONFIG_ESPNOW_ENABLE_LONG_RANGE
    ESP_ERROR_CHECK(esp_wifi_set_protocol(ESPNOW_WIFI_IF,
                                          WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
#endif

    uint8_t mac[6];
    esp_err_t mac_ret = esp_read_mac(mac, ESP_MAC_WIFI_STA);

    if (mac_ret == ESP_OK)
        printf("MAC: [0x%x 0x%x 0x%x 0x%x 0x%x 0x%x]\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    else
        printf("Failed to get MAC address, error: %d\n", ret);

    /* Initialize ESPNOW and register sending and receiving callback function. */
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(espnow_send_cb));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_recv_cb));

#if CONFIG_ESPNOW_ENABLE_POWER_SAVE
    ESP_ERROR_CHECK(esp_now_set_wake_window(CONFIG_ESPNOW_WAKE_WINDOW));
    ESP_ERROR_CHECK(esp_wifi_connectionless_module_set_wake_interval(CONFIG_ESPNOW_WAKE_INTERVAL));
#endif

    /* Add broadcast peer information to peer list. */
    esp_now_peer_info_t *peer = malloc(sizeof(esp_now_peer_info_t));
    if (peer == NULL)
    {
        ESP_LOGE(TAG, "Malloc peer information fail");
        esp_now_deinit();
    }

    memset(peer, 0, sizeof(esp_now_peer_info_t));
    peer->channel = CONFIG_ESPNOW_CHANNEL;
    peer->ifidx = ESPNOW_WIFI_IF;
    peer->encrypt = false;
    peer->priv = NULL;
    memcpy(peer->peer_addr, broadcast_mac, ESP_NOW_ETH_ALEN);
#if ESP_NOW_PMK
    /* Set primary master key. */
    peer->encrypt = true;
    uint8_t lmk[ESP_NOW_KEY_LEN] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                    0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00};
    ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)lmk));
    memcpy(peer->lmk, lmk, ESP_NOW_KEY_LEN);
#endif

    ESP_ERROR_CHECK(esp_now_add_peer(peer));
    free(peer);

    /* If MAC address does not exist in peer list, add it to peer list. */
    if (esp_now_is_peer_exist(broadcast_mac) == false)
    {
        esp_now_peer_info_t *_peer = malloc(sizeof(esp_now_peer_info_t));
        if (_peer == NULL)
        {
            ESP_LOGE(TAG, "Malloc peer information fail");
            vTaskDelete(NULL);
        }
        memset(_peer, 0, sizeof(esp_now_peer_info_t));
        _peer->channel = CONFIG_ESPNOW_CHANNEL;
        _peer->ifidx = ESPNOW_WIFI_IF;
        _peer->encrypt = false;
#if ESP_NOW_PMK
        /* Set primary master key. */
        _peer->encrypt = true;
        uint8_t _lmk[ESP_NOW_KEY_LEN] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                         0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00};
        ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)_lmk));
        memcpy(_peer->lmk, _lmk, ESP_NOW_KEY_LEN);
#endif
        memcpy(_peer->peer_addr, broadcast_mac, ESP_NOW_ETH_ALEN);
        ESP_ERROR_CHECK(esp_now_add_peer(_peer));
        free(_peer);
    }

    for (;;)
    {
        uint64_t time = esp_timer_get_time() / 1000;
        uint8_t len = sizeof(time);

        if (esp_now_send(broadcast_mac, (uint8_t *)&time, len) == ESP_OK)
            printf("Enviado mensagem [%lld] de tamanho [%d]\r\n", time, len);
        else
            ESP_LOGE(TAG, "ESP-NOW Send Error");

        delay(1000);
    }
}

/* Callbacks */
static void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if (mac_addr == NULL)
    {
        ESP_LOGE(TAG, "Send cb arg error");
        return;
    }

    ESP_LOGI(TAG, "Send ESP_NOW status: [%d]\r\n", status == ESP_NOW_SEND_SUCCESS ? 1 : 0);
}

static void espnow_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len)
{
    uint64_t rec_data = 0;
    uint8_t *mac_addr = recv_info->src_addr;
    uint8_t *des_addr = recv_info->des_addr;

    memcpy(&rec_data, (uint64_t *)data, sizeof(rec_data));

    if (mac_addr == NULL || data == NULL || len <= 0)
    {
        ESP_LOGE(TAG, "Receive cb arg error");
        return;
    }

    printf("DATA: [ ");
    for (int i = 0; i < len; i++)
        printf("0x%x ", *(data + i));
    printf("]\r\n");

    printf("DES_ADDr: [0x%x 0x%x 0x%x 0x%x 0x%x 0x%x]", des_addr[0], des_addr[1], des_addr[2],
           des_addr[3], des_addr[4], des_addr[5]);
    println();
    printf("SRC_ADDr: [0x%x 0x%x 0x%x 0x%x 0x%x 0x%x]", mac_addr[0], mac_addr[1], mac_addr[2],
           mac_addr[3], mac_addr[4], mac_addr[5]);
    println();

    ESP_LOGI(TAG, "Receive sucess, mesage: [%lld] | len message: [%d]\r\n", rec_data, len);
}

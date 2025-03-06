#ifndef DRIVER_DEFS_H
#define DRIVER_DEFS_H

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_random.h"
#include "esp_timer.h"
#include "math.h"
#include "driver/ledc.h"

// EEPROM storage
#include "nvs_flash.h"
#include "nvs.h"
// ESP-NOW
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_now.h"

#define println() (printf("\r\n"))
#define delay(ms) (vTaskDelay(pdMS_TO_TICKS(ms)))
#define millis()  (esp_timer_get_time() / 1000)

/* LEDC defines */
#define SPEED_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_CLOCK       LEDC_AUTO_CLK
#define LEDC_INTR_TYPE   LEDC_INTR_DISABLE

/* Motor timers and channels */
#define MOTOR_MAX_RPM    8700 // in 5 volts
#define MOTOR_TIMER      LEDC_TIMER_1
#define MOTOR_RESOLUTION LEDC_TIMER_10_BIT
#define RPM_TO_DUTY(r)   ((int)((r * (pow(2, MOTOR_RESOLUTION) - 1)) / MOTOR_MAX_RPM))
#define DUTY_TO_RPM(t)   ((int)((t * MOTOR_MAX_RPM) / (pow(2, MOTOR_RESOLUTION) - 1)))

#define MOTOR_FOWARD_CHANNEL  LEDC_CHANNEL_1
#define MOTOR_REVERSE_CHANNEL LEDC_CHANNEL_2

/* Servo timers and channels */
#define FREQ_SERVO        50
#define MAX_ANGLE         180
#define MAX_PULSEWIDTH    2500
#define MIN_PULSEWIDTH    500
#define SERVO_TIMER       LEDC_TIMER_2
#define SERVO_RESOLUTION  LEDC_TIMER_10_BIT

#define STEERING_CHANNEL  LEDC_CHANNEL_3
#define HEADLIGHT_CHANNEL LEDC_CHANNEL_4

/* ESP-NOW Definitions */
#if CONFIG_ESPNOW_WIFI_MODE_STATION
    #define ESPNOW_WIFI_MODE WIFI_MODE_STA
    #define ESPNOW_WIFI_IF   ESP_IF_WIFI_STA
#else
    #define ESPNOW_WIFI_MODE WIFI_MODE_AP
    #define ESPNOW_WIFI_IF   ESP_IF_WIFI_AP
#endif

#if CONFIG_ESPNOW_ENABLE_POWER_SAVE
    #define CONFIG_ESPNOW_WAKE_WINDOW   1000
    #define CONFIG_ESPNOW_WAKE_INTERVAL 1000
#endif  

#define CONFIG_ESPNOW_CHANNEL 1

#endif

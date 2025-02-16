#ifndef DRIVER_DEFS_H
#define DRIVER_DEFS_H

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/adc.h"
#include "esp_random.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "math.h"
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

/* GPIO defines */
#define GPIO_MODE        GPIO_MODE_INPUT_OUTPUT
#define PULLUP_MODE      GPIO_PULLUP_DISABLE
#define PULLDOWN_MODE    GPIO_PULLDOWN_DISABLE
#define GPIO_INTR_TYPE   GPIO_INTR_DISABLE

/* LEDC defines */
#define SPEED_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_CLOCK       LEDC_AUTO_CLK
#define LEDC_INTR_TYPE   LEDC_INTR_DISABLE

/* LEDs timers and channels */
#define FREQ_LED         50
#define LED_TIMER        LEDC_TIMER_0
#define LED_RESOLUTION   LEDC_TIMER_16_BIT

#define BRAKE_CHANNEL    LEDC_CHANNEL_0
// #define RED_CHANNEL      LEDC_CHANNEL_1 <----- DEPRECATED
// #define GREEN_CHANNEL    LEDC_CHANNEL_2 <----- DEPRECATED
// #define BLUE_CHANNEL     LEDC_CHANNEL_3 <----- DEPRECATED

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

/* ADC channel */
#define SOC_ADC_CHANNEL  ADC1_CHANNEL_6
#define SOC_WIDTH        ADC_WIDTH_BIT_12
#define SOC_ATTENUATION  ADC_ATTEN_DB_12

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

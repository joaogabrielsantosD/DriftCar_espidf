#ifndef LEDS_H
#define LEDS_H

#pragma once

#include "hardware_defs.h"
#include "driver_defs.h"
#include "driver/gpio.h"

/* GPIO defines */
#define GPIO_MODE GPIO_MODE_INPUT_OUTPUT
#define PULLUP_MODE GPIO_PULLUP_DISABLE
#define PULLDOWN_MODE GPIO_PULLDOWN_DISABLE
#define GPIO_INTR_TYPE GPIO_INTR_DISABLE

/* LEDs timers and channels */
#define FREQ_LED 50
#define LED_TIMER LEDC_TIMER_0
#define LED_RESOLUTION LEDC_TIMER_16_BIT

#define BRAKE_CHANNEL LEDC_CHANNEL_0
// #define RED_CHANNEL      LEDC_CHANNEL_1 <----- DEPRECATED
// #define GREEN_CHANNEL    LEDC_CHANNEL_2 <----- DEPRECATED
// #define BLUE_CHANNEL     LEDC_CHANNEL_3 <----- DEPRECATED

typedef esp_err_t (*LED_function)(gpio_num_t);

typedef struct LEDs
{
    /* Pins */
    uint64_t HEADLIGHT_PIN;
    uint64_t RIGHT_PIN;
    uint64_t LEFT_PIN;
    uint64_t BRAKE_PIN;

    /* Channels */
    ledc_channel_t brake_channel;

    /* GPIO Configuration */
    gpio_config_t LED_config;
    ledc_timer_config_t LEDs_ledc_timer;
    ledc_channel_config_t brake_ledc_channel;

    /* Funções */
    LED_function blinky;
    LED_function on;
    LED_function off;

} LEDs_t;

/**/
void brake_on(LEDs_t *leds);
/**/
void brake_off(LEDs_t *leds);
/**/
esp_err_t update_ledc_duty(LEDs_t *leds, int duty);

LEDs_t config_LEDs(void);
esp_err_t blinky_led(gpio_num_t gpio_num);
esp_err_t on_led(gpio_num_t gpio_num);
esp_err_t off_led(gpio_num_t gpio_num);

#endif // LEDS_H
#ifndef LEDS_H
#define LEDS_H

#include "hardware_defs.h"
#include "driver_defs.h"
#include "driver/gpio.h"

// GPIO defines
#define LED_GPIO_MODE      GPIO_MODE_INPUT_OUTPUT
#define LED_PULLUP_MODE    GPIO_PULLUP_DISABLE
#define LED_PULLDOWN_MODE  GPIO_PULLDOWN_DISABLE
#define LED_GPIO_INTR_TYPE GPIO_INTR_DISABLE

// LEDC defines
#define LED_SPEED_MODE     LEDC_LOW_SPEED_MODE
#define LED_LEDC_CLOCK     LEDC_AUTO_CLK
#define LED_LEDC_INTR_TYPE LEDC_INTR_DISABLE

// PWM defines
#define LED_FREQ           CONFIG_DRIFT_CAR_LED_FREQUENCY
#define LED_TIMER          LEDC_TIMER_0
#define LED_RESOLUTION     LEDC_TIMER_8_BIT

// LEDs PWM channels
#define BRAKE_CHANNEL      LEDC_CHANNEL_0
// #define RED_CHANNEL     LEDC_CHANNEL_1 <----- DEPRECATED
// #define GREEN_CHANNEL   LEDC_CHANNEL_2 <----- DEPRECATED
// #define BLUE_CHANNEL    LEDC_CHANNEL_3 <----- DEPRECATED

#define LED_BLINK(GPIO) blinky_led(GPIO)
#define LED_ON(GPIO)    on_led(GPIO)
#define LED_OFF(GPIO)   off_led(GPIO)


typedef struct
{
    /* Pins */
    gpio_num_t HEADLIGHT_PIN;
    gpio_num_t RIGHT_PIN;
    gpio_num_t LEFT_PIN;
    gpio_num_t BRAKE_PIN;

    /* Channels */
    ledc_channel_t brake_channel;

} led_handle_t;

led_handle_t *led_create(void);
void led_brake_on(led_handle_t *handle);
void led_brake_off(led_handle_t *handle);

#endif // LEDS_H

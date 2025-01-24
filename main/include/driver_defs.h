#ifndef LEDC_DEFS_H
#define LEDC_DEFS_H

#include "driver/gpio.h"
#include "driver/ledc.h"
#include <driver/adc.h>

#define println() (printf("\r\n"))

/* GPIO defines */
#define GPIO_MODE        GPIO_MODE_INPUT_OUTPUT
#define PULLUP_MODE      GPIO_PULLUP_DISABLE
#define PULLDOWN_MODE    GPIO_PULLDOWN_DISABLE
#define GPIO_INTR_TYPE   GPIO_INTR_DISABLE

/* LEDC defines */
#define LED_RESOLUTION   LEDC_TIMER_8_BIT
#define MOTOR_RESOLUTION LEDC_TIMER_16_BIT
#define FREQ_LED         5000
#define SPEED_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_CLOCK       LEDC_AUTO_CLK
#define LEDC_INTR_TYPE   LEDC_INTR_DISABLE

/* LEDs timers and channels */
#define LED_TIMER        LEDC_TIMER_0

#define BRAKE_CHANNEL    LEDC_CHANNEL_0
//#define RED_CHANNEL      LEDC_CHANNEL_1 <----- DEPRECATED
//#define GREEN_CHANNEL    LEDC_CHANNEL_2 <----- DEPRECATED
//#define BLUE_CHANNEL     LEDC_CHANNEL_3 <----- DEPRECATED

/* Motor timers and channels */
#define MOTOR_MODE       GPIO_MODE_OUTPUT
#define MOTOR_TIMER      LEDC_TIMER_1

#define MOTOR_FOWARD_CHANNEL  LEDC_CHANNEL_1
#define MOTOR_REVERSE_CHANNEL LEDC_CHANNEL_2

/* ADC channel */
#define SOC_ADC_CHANNEL  ADC1_CHANNEL_6

#endif

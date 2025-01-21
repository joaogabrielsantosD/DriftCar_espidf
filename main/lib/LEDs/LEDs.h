#ifndef LEDS_H
#define LEDS_H

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "hardware_defs.h"
#include "LEDC_defs.h"

/* GPIO defines */
#define GPIO_MODE      GPIO_MODE_INPUT_OUTPUT
#define PULLUP_MODE    GPIO_PULLUP_DISABLE
#define PULLDOWN_MODE  GPIO_PULLDOWN_DISABLE
#define GPIO_INTR_TYPE GPIO_INTR_DISABLE

/* LEDC defines */
#define LED_RESOLUTION LEDC_TIMER_8_BIT
#define FREQ_LED       5000

void ConfigLEDs(void);

#endif

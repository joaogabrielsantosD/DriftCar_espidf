#ifndef LEDC_DEFS_H
#define LEDC_DEFS_H

#include "driver/ledc.h"

/* Default settings */
#define SPEED_MODE     LEDC_LOW_SPEED_MODE
#define CLK_FOR_LED    LEDC_AUTO_CLK
#define LEDC_INTR_TYPE LEDC_INTR_DISABLE

/* LEDs timers and channels */
#define LED_TIMER      LEDC_TIMER_0

#define BRAKE_CHANNEL  LEDC_CHANNEL_0
//#define RED_CHANNEL    LEDC_CHANNEL_1 <----- DEPRECATED
//#define GREEN_CHANNEL  LEDC_CHANNEL_2 <----- DEPRECATED
//#define BLUE_CHANNEL   LEDC_CHANNEL_3 <----- DEPRECATED

#endif

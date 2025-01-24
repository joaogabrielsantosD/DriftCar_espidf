#include "OtherThings.h"

MX1508_config_t mx1508;

void Start_MotorDC()
{
    mx1508.pinIN1     = MOTOR_FOWARD_PIN;
    mx1508.pinIN2     = MOTOR_REVERSE_PIN;
    mx1508.ledCH1     = MOTOR_FOWARD_CHANNEL;
    mx1508.ledCH2     = MOTOR_REVERSE_CHANNEL;
    mx1508.frequecy   = FREQ_LED;
    mx1508.resolution = MOTOR_RESOLUTION;


    gpio_config_t output_pin = {
        .pin_bit_mask = BIT(mx1508.pinIN1) | BIT(mx1508.pinIN1),
        .mode         = MOTOR_MODE,
        .pull_up_en   = PULLUP_MODE,
        .pull_down_en = PULLDOWN_MODE,
        .intr_type    = GPIO_INTR_TYPE
    };

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t motor_timer = {
        .speed_mode      = SPEED_MODE,
        .duty_resolution = mx1508.resolution,
        .timer_num       = MOTOR_TIMER,
        .freq_hz         = mx1508.frequecy,
        .clk_cfg         = LEDC_CLOCK,
        .deconfigure     = false
    };

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel1 = {
        .gpio_num   = (int)mx1508.pinIN1,
        .speed_mode = SPEED_MODE,
        .channel    = mx1508.ledCH1,
        .timer_sel  = MOTOR_TIMER,
    };

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel2 = {
        .gpio_num   = (int)mx1508.pinIN2,
        .speed_mode = SPEED_MODE,
        .channel    = mx1508.ledCH2,
        .timer_sel  = MOTOR_TIMER,
    };

    MX1508_init(&output_pin, &motor_timer, &ledc_channel1, &ledc_channel2);
}


#include "MX1508.h"

PWM_values_t values;

void MX1508_init(gpio_config_t *outputpin, ledc_timer_config_t *timer_config,  \
                ledc_channel_config_t *ch1, ledc_channel_config_t *ch2)
{
    ESP_ERROR_CHECK(gpio_config(outputpin));

    ESP_ERROR_CHECK(ledc_timer_config(timer_config));

    ESP_ERROR_CHECK(ledc_channel_config(ch1));

    ESP_ERROR_CHECK(ledc_channel_config(ch2));

    values._CH1 = ch1->channel;
    values._CH2 = ch2->channel;

    switch (timer_config->duty_resolution)
    {
        case LEDC_TIMER_16_BIT:
            values._maxpwm = 65535;
            break;

        case LEDC_TIMER_12_BIT:
            values._maxpwm = 4095;
            break;

        case LEDC_TIMER_8_BIT:
        default:
            values._maxpwm = 255;
            break;
    }
}

void motorGo(long pwmSpeed) 
{
    values._pwmVal = pwmSpeed;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH1, values._pwmVal);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH2, 0);

    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH1);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH2);
}

void motorRev(long pwmSpeed) 
{
    values._pwmVal = pwmSpeed;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH1, 0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH2, values._pwmVal);

    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH1);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH2);
}

void motorBrake() 
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH1, values._maxpwm);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH2, values._maxpwm);

    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH1);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH2);
}

void motorStop()  { stopMotor(); }

void stopMotor() 
{                      // Kept for backwards compatibility
    ledc_stop(LEDC_LOW_SPEED_MODE, values._CH1, 0);
    ledc_stop(LEDC_LOW_SPEED_MODE, values._CH2, 0);
}

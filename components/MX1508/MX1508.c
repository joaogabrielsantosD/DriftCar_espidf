#include "MX1508.h"

PWM_values_t values;

void MX1508_init(MX1508_config_t *mx1508)
{
    gpio_config_t output_pin1 = {
        .pin_bit_mask = BIT(mx1508->pinIN1),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };

    gpio_config_t output_pin2 = {
        .pin_bit_mask = BIT(mx1508->pinIN2),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&output_pin1));
    ESP_ERROR_CHECK(gpio_config(&output_pin2));

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer1 = {
        .speed_mode      = LEDC_LOW_SPEED_MODE,
        .duty_resolution = mx1508->resolution,
        .timer_num       = LEDC_TIMER_0,
        .freq_hz         = mx1508->frequecy,
        .clk_cfg         = LEDC_AUTO_CLK,
        .deconfigure     = false
    };

    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer1));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel1 = {
        .gpio_num   = (int)mx1508->pinIN1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = mx1508->ledCH1,
        .timer_sel  = LEDC_TIMER_0,
    };

    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel1));

        // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer2 = {
        .speed_mode      = LEDC_LOW_SPEED_MODE,
        .duty_resolution = mx1508->resolution,
        .timer_num       = LEDC_TIMER_0,
        .freq_hz         = mx1508->frequecy,
        .clk_cfg         = LEDC_AUTO_CLK,
        .deconfigure     = false
    };

    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer2));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel2 = {
        .gpio_num   = (int)mx1508->pinIN2,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = mx1508->ledCH2,
        .timer_sel  = LEDC_TIMER_0,
    };

    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel2));

    values._CH1 = mx1508->ledCH1;
    values._CH2 = mx1508->ledCH2;

    switch (mx1508->resolution)
    {
        case LEDC_TIMER_16_BIT:
            values._maxpwm = 65535;
            break;

        case LEDC_TIMER_12_BIT:
            values._maxpwm = 4096;
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
    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH1);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH2, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH2);
}

void motorRev(long pwmSpeed) 
{
    values._pwmVal = pwmSpeed;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH1, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH1);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH2, values._pwmVal);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH2);
}

void motorBrake() 
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH1, values._maxpwm);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH1);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, values._CH2, values._maxpwm);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, values._CH2);
}

void motorStop()  { stopMotor(); }

void stopMotor() 
{                      // Kept for backwards compatibility
    ledc_stop(LEDC_LOW_SPEED_MODE, values._CH1, 0);
    ledc_stop(LEDC_LOW_SPEED_MODE, values._CH2, 0);
}

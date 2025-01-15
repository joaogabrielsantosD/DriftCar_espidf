#include "Servo.h"

void setup_pwm(uint8_t SERVO_PIN)
{
    // Configure the LEDC timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode      = SPEED_MODE,
        .duty_resolution = DUTY_RESOLUTION,
        .timer_num       = TIMER_NUM,
        .freq_hz         = FREQUENCY,
        .clk_cfg         = CLK_CONFIG,
        .deconfigure     = false
    };

    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Configure the LEDC channel
    ledc_channel_config_t ledc_channel = {
        .gpio_num   = SERVO_PIN,
        .speed_mode = SPEED_MODE,
        .channel    = CHANNEL,
        .timer_sel  = TIMER_NUM,
        .intr_type  = INTR_TYPE,
        .duty       = 0, // Initial duty cycle
        .hpoint     = 0,
        .flags = {.output_invert = 1}
    };
        
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void set_servo_angle(int angle)
{
    // Convert angle to duty cycle
    int pulsewidth = SERVO_MIN_PULSEWIDTH + ((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * angle) / SERVO_MAX_DEGREE;
    int duty = (pulsewidth * LEDC_TIMER_12_BIT * LEDC_TIMER_12_BIT) / 20000; // 8192 is 2^13 for 13-bit resolution
    ledc_set_duty(SPEED_MODE, CHANNEL, duty);
    ledc_update_duty(SPEED_MODE, CHANNEL);
}

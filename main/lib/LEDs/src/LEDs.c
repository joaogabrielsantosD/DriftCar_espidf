#include "LEDs.h"

const char *TAG    = "LEDs";
const char *STATUS = "TEST";

LEDs_t config_LEDs(void)
{
    ESP_LOGI(TAG, "LEDS Configuration: [%s]", STATUS);	
    LEDs_t leds_conf;

    leds_conf.HEADLIGHT_PIN = LED_HEADLIGHT_PIN;
    leds_conf.RIGHT_PIN     = LED_RIGHT_PIN;
    leds_conf.LEFT_PIN      = LED_LEFT_PIN;
    leds_conf.BRAKE_PIN     = LED_BRAKE_LIGHT_PIN;
    
    leds_conf.brake_channel = BRAKE_CHANNEL;

    /* Configure the GPIO */
    leds_conf.LED_config.pin_bit_mask = BIT(leds_conf.HEADLIGHT_PIN) | BIT(leds_conf.RIGHT_PIN) | BIT(leds_conf.LEFT_PIN);
    leds_conf.LED_config.mode         = GPIO_MODE;
    leds_conf.LED_config.pull_up_en   = PULLUP_MODE;
    leds_conf.LED_config.pull_down_en = PULLDOWN_MODE;
    leds_conf.LED_config.intr_type    = GPIO_INTR_TYPE;
    ESP_ERROR_CHECK(gpio_config(&leds_conf.LED_config));

    /* Configure the Timer */
    leds_conf.LEDs_ledc_timer.duty_resolution = LED_RESOLUTION;
    leds_conf.LEDs_ledc_timer.freq_hz         = FREQ_LED;
    leds_conf.LEDs_ledc_timer.speed_mode      = SPEED_MODE;
    leds_conf.LEDs_ledc_timer.timer_num       = LED_TIMER;
    leds_conf.LEDs_ledc_timer.clk_cfg         = LEDC_CLOCK;
    ESP_ERROR_CHECK(ledc_timer_config(&leds_conf.LEDs_ledc_timer));

    /* Configure the LED channel */
    leds_conf.brake_ledc_channel.gpio_num   = leds_conf.BRAKE_PIN;
    leds_conf.brake_ledc_channel.channel    = leds_conf.brake_channel;
    leds_conf.brake_ledc_channel.speed_mode = SPEED_MODE;
    leds_conf.brake_ledc_channel.timer_sel  = LED_TIMER;
    leds_conf.brake_ledc_channel.intr_type  = LEDC_INTR_TYPE;
    leds_conf.brake_ledc_channel.duty       = 0;
    leds_conf.brake_ledc_channel.hpoint     = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&leds_conf.brake_ledc_channel));

    /* Funções */
    leds_conf.blinky = blinky_led;
    leds_conf.on     = on_led;
    leds_conf.off    = off_led;

    return leds_conf;
};

esp_err_t blinky_led(gpio_num_t gpio_num)
{
    return gpio_set_level(gpio_num, gpio_get_level(gpio_num) ^ 1);
}

esp_err_t on_led(gpio_num_t gpio_num)
{
    return gpio_set_level(gpio_num, HIGH);
}

esp_err_t off_led(gpio_num_t gpio_num)
{
    return gpio_set_level(gpio_num, LOW);
}

void brake_on(LEDs_t *leds)
{
    ledc_set_duty(SPEED_MODE, leds->brake_channel, 255);
    ledc_update_duty(SPEED_MODE, leds->brake_channel);
}

void brake_off(LEDs_t *leds)
{
    ledc_set_duty(SPEED_MODE, leds->brake_channel, 120);
    ledc_update_duty(SPEED_MODE, leds->brake_channel);
}

esp_err_t update_ledc_duty(LEDs_t *leds, int duty)
{
    if (duty < 0 || duty > 255)
        return ESP_ERR_INVALID_ARG;

    if (ledc_set_duty(SPEED_MODE, leds->brake_channel, duty) == ESP_OK)
        return ledc_update_duty(SPEED_MODE, leds->brake_channel);       
    
    return ESP_FAIL;
}

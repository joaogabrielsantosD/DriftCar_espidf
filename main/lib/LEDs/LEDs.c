#include "LEDs.h"

static const char TAG[] = "LEDs";

static const uint16_t DUTY = (1UL << LED_RESOLUTION) - 1;
static led_handle_t leds_conf;

static esp_err_t blinky_led(gpio_num_t gpio_num);
static esp_err_t on_led(gpio_num_t gpio_num);
static esp_err_t off_led(gpio_num_t gpio_num);

led_handle_t *led_create(void)
{
    esp_log_level_set(TAG, ESP_LOG_VERBOSE);

    leds_conf.HEADLIGHT_PIN = LED_HEADLIGHT_PIN;
    leds_conf.RIGHT_PIN     = LED_RIGHT_PIN;
    leds_conf.LEFT_PIN      = LED_LEFT_PIN;
    leds_conf.BRAKE_PIN     = LED_BRAKE_LIGHT_PIN;
    
    leds_conf.brake_channel = BRAKE_CHANNEL;

    /* Configure the GPIO */
    gpio_config_t led_config = {
        .pin_bit_mask = BIT64(leds_conf.HEADLIGHT_PIN) | BIT64(leds_conf.RIGHT_PIN) | BIT64(leds_conf.LEFT_PIN),
        .mode         = LED_GPIO_MODE,
        .pull_up_en   = LED_PULLUP_MODE,
        .pull_down_en = LED_PULLDOWN_MODE,
        .intr_type    = LED_GPIO_INTR_TYPE
    };
    ESP_ERROR_CHECK(gpio_config(&led_config));

    /* Configure the Timer */
    ledc_timer_config_t LEDs_ledc_timer = {
        .duty_resolution = LED_RESOLUTION,
        .freq_hz         = LED_FREQ,
        .speed_mode      = LED_SPEED_MODE,
        .timer_num       = LED_TIMER,
        .clk_cfg         = LED_LEDC_CLOCK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&LEDs_ledc_timer));

    /* Configure the LED channel */
    ledc_channel_config_t brake_ledc_channel = {
        .gpio_num   = leds_conf.BRAKE_PIN,
        .channel    = leds_conf.brake_channel,
        .speed_mode = LED_SPEED_MODE,
        .timer_sel  = LED_TIMER,
        .intr_type  = LED_LEDC_INTR_TYPE,
        .duty       = 0,
        .hpoint     = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&brake_ledc_channel));

    ESP_LOGV(TAG, "LEDS Configuration: [%s]", "OK");
    return &leds_conf;
};

void led_brake_on(led_handle_t *handle)
{
    ledc_set_duty(LED_SPEED_MODE, handle->brake_channel, DUTY); // 100% duty
    ledc_update_duty(LED_SPEED_MODE, handle->brake_channel);
}

void led_brake_off(led_handle_t *handle)
{
    ledc_set_duty(LED_SPEED_MODE, handle->brake_channel, DUTY / 4); // 25% duty
    ledc_update_duty(LED_SPEED_MODE, handle->brake_channel);
}

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

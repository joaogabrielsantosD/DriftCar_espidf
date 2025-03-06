#include "LEDs.h"

static const char *TAG    = "LEDs";
static const char *STATUS = "TEST";

static LEDs_t leds_conf = {0};

static esp_err_t blinky_led(gpio_num_t gpio_num);
static esp_err_t on_led(gpio_num_t gpio_num);
static esp_err_t off_led(gpio_num_t gpio_num);

LEDs_t *config_LEDs(void)
{
    static bool init = false;
    ESP_LOGI(TAG, "LEDS Configuration: [%s]", STATUS);	

    if (init)
        return &leds_conf;

    leds_conf.HEADLIGHT_PIN = LED_HEADLIGHT_PIN;
    leds_conf.RIGHT_PIN     = LED_RIGHT_PIN;
    leds_conf.LEFT_PIN      = LED_LEFT_PIN;
    leds_conf.BRAKE_PIN     = LED_BRAKE_LIGHT_PIN;
    
    leds_conf.brake_channel   = BRAKE_CHANNEL;
    leds_conf.duty_resolution = LED_RESOLUTION; 

    /* Configure the GPIO */
    leds_conf.LED_config.pin_bit_mask = BIT64(leds_conf.HEADLIGHT_PIN) | BIT64(leds_conf.RIGHT_PIN) | BIT64(leds_conf.LEFT_PIN);
    leds_conf.LED_config.mode         = GPIO_MODE;
    leds_conf.LED_config.pull_up_en   = PULLUP_MODE;
    leds_conf.LED_config.pull_down_en = PULLDOWN_MODE;
    leds_conf.LED_config.intr_type    = GPIO_INTR_TYPE;
    ESP_ERROR_CHECK(gpio_config(&leds_conf.LED_config));

    /* Configure the Timer */
    leds_conf.LEDs_ledc_timer.duty_resolution = leds_conf.duty_resolution;
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

    init = true;
    return &leds_conf;
};

inline void brake_on(LEDs_t *leds)
{
    ledc_set_duty(SPEED_MODE, leds->brake_channel, (1 << leds->duty_resolution) - 1); // 100% duty
    ledc_update_duty(SPEED_MODE, leds->brake_channel);
}

inline void brake_off(LEDs_t *leds)
{
    ledc_set_duty(SPEED_MODE, leds->brake_channel, (1 << leds->duty_resolution) / 4); // 25% duty
    ledc_update_duty(SPEED_MODE, leds->brake_channel);
}

esp_err_t update_brake_duty(LEDs_t *leds, uint32_t duty)
{
    if (duty >= (1UL << leds->duty_resolution))
        return ESP_ERR_INVALID_ARG;

    ledc_set_duty(SPEED_MODE, leds->brake_channel, duty);
    return ledc_update_duty(SPEED_MODE, leds->brake_channel);
}

inline static esp_err_t blinky_led(gpio_num_t gpio_num)
{
    return gpio_set_level(gpio_num, gpio_get_level(gpio_num) ^ 1);
}

inline static esp_err_t on_led(gpio_num_t gpio_num)
{
    return gpio_set_level(gpio_num, HIGH);
}

inline static esp_err_t off_led(gpio_num_t gpio_num)
{
    return gpio_set_level(gpio_num, LOW);
}

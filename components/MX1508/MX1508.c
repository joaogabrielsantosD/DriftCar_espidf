#include "MX1508.h"

static const char *TAG = "MX1508";

#define MX1508_CHECK(a, str, ret_val)                             \
    if (!(a))                                                     \
    {                                                             \
        ESP_LOGE(TAG, "%s(%d): %s", __FUNCTION__, __LINE__, str); \
        return (ret_val);                                         \
    }
// #define MOTOR_FREQ_MIN 1000  // 1khz
// #define MOTOR_FREQ_MAX 20000 // 20kHz
PWM_values_t values;

esp_err_t MX1508_init(const MX1508_config_t *config)
{
    esp_err_t ret;
    MX1508_CHECK(NULL != config, "Pointer of config is invalid", ESP_ERR_INVALID_ARG);   

    MX1508_CHECK(config->channel_number > 0 && config->channel_number <= MX1508_UNIT,
    "Servo channel number out the range", ESP_ERR_INVALID_ARG);

    // MX1508_CHECK(config->freq <= MOTOR_FREQ_MAX && config->freq >= MOTOR_FREQ_MIN,
    // "Servo pwm frequency out the range", ESP_ERR_INVALID_ARG);

    uint64_t pin_mask = 0;
    uint32_t ch_mask = 0;
    for (size_t i = 0; i < config->channel_number; i++)
    {
        uint64_t _pin_mask = 1ULL << config->channels.motor_pin[i];
        uint32_t _ch_mask = 1UL << config->channels.ch[i];
        MX1508_CHECK(!(pin_mask & _pin_mask), "servo gpio has a duplicate", ESP_ERR_INVALID_ARG);
        MX1508_CHECK(!(ch_mask & _ch_mask), "servo channel has a duplicate", ESP_ERR_INVALID_ARG);
        MX1508_CHECK(GPIO_IS_VALID_OUTPUT_GPIO(config->channels.motor_pin[i]), "servo gpio invalid", ESP_ERR_INVALID_ARG);
        pin_mask |= _pin_mask;
        ch_mask |= _ch_mask;
    }

    ledc_timer_config_t ledc_timer = {
        .clk_cfg         = LEDC_AUTO_CLK,
        .duty_resolution = config->resolution,   // resolution of PWM duty
        .freq_hz         = config->freq,         // frequency of PWM signal
        .speed_mode      = SPEED_MODE,           // timer mode
        .timer_num       = config->timer_number  // timer index
    };

    ret = ledc_timer_config(&ledc_timer);
    MX1508_CHECK(ESP_OK == ret, "ledc timer configuration failed", ESP_FAIL);
    
    for (size_t i = 0; i < config->channel_number; i++)
    {
        ledc_channel_config_t ledc_ch = {
            .intr_type  = LEDC_INTR_DISABLE,
            .channel    = config->channels.ch[i],
            .duty       = 0,
            .gpio_num   = config->channels.motor_pin[i],
            .speed_mode = SPEED_MODE,
            .timer_sel  = config->timer_number,
            .hpoint     = 0
        };
        ret = ledc_channel_config(&ledc_ch);
        MX1508_CHECK(ESP_OK == ret, "ledc channel configuration failed", ESP_FAIL);
    }

    values._CH1    = config->channels.ch[0];
    values._CH2    = config->channels.ch[1];
    values._maxpwm = (1 << config->resolution) - 1;
        
    return ESP_OK;
}

esp_err_t MX1508_deinit(const MX1508_config_t *config)
{
    for (size_t i = 0; i < config->channel_number; i++)
        ledc_stop(SPEED_MODE, config->channels.ch[i], 0);
    ledc_timer_rst(SPEED_MODE, config->timer_number);
    values._maxpwm = 0;
    return ESP_OK;
}

esp_err_t MX1508_read_duty(uint8_t channel, uint32_t *duty)
{
    MX1508_CHECK(channel < LEDC_CHANNEL_MAX, "LEDC channel number too large", ESP_ERR_INVALID_ARG);
    *duty = ledc_get_duty(SPEED_MODE, channel);
    return ESP_OK;
}

esp_err_t motorGo(long pwmSpeed) 
{
    MX1508_CHECK(pwmSpeed < values._maxpwm, "SPEED is bigger than resolution", ESP_ERR_INVALID_ARG);
    values._pwmVal = pwmSpeed;

    ledc_set_duty(SPEED_MODE, values._CH1, values._pwmVal);
    ledc_set_duty(SPEED_MODE, values._CH2, 0);

    ledc_update_duty(SPEED_MODE, values._CH1);
    ledc_update_duty(SPEED_MODE, values._CH2);

    return ESP_OK;
}

esp_err_t motorRev(long pwmSpeed) 
{
    MX1508_CHECK(pwmSpeed < values._maxpwm, "SPEED is bigger than resolution", ESP_ERR_INVALID_ARG);
    values._pwmVal = pwmSpeed;

    ledc_set_duty(SPEED_MODE, values._CH1, 0);
    ledc_set_duty(SPEED_MODE, values._CH2, values._pwmVal);

    ledc_update_duty(SPEED_MODE, values._CH1);
    ledc_update_duty(SPEED_MODE, values._CH2);

    return ESP_OK;
}

void motorBrake() 
{
    ledc_set_duty(SPEED_MODE, values._CH1, values._maxpwm);
    ledc_set_duty(SPEED_MODE, values._CH2, values._maxpwm);

    ledc_update_duty(SPEED_MODE, values._CH1);
    ledc_update_duty(SPEED_MODE, values._CH2);
}

void motorStop()  { stopMotor(); }

void stopMotor() 
{   // Kept for backwards compatibility
    ledc_stop(SPEED_MODE, values._CH1, 0);
    ledc_stop(SPEED_MODE, values._CH2, 0);
}

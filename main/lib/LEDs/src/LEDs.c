#include "LEDs.h"

void StartLEDs()
{
    gpio_config_t LED_config = {
        .pin_bit_mask = LED_HEADLIGHT_BIT | LED_RIGHT_BIT | LED_LEFT_BIT,
        .mode         = GPIO_MODE_INPUT_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&LED_config));

    
    ledc_timer_config_t brake_ledc_timer = {         
        .duty_resolution = LEDC_TIMER_8_BIT,     
        .freq_hz         = 4000,                 
        .speed_mode      = LEDC_LOW_SPEED_MODE,  
        .timer_num       = LEDC_TIMER_0,        
        .clk_cfg         = LEDC_AUTO_CLK         
    };                               

    ESP_ERROR_CHECK(ledc_timer_config(&brake_ledc_timer));

    ledc_channel_config_t brake_ledc_channel = {   
        .speed_mode     = LEDC_LOW_SPEED_MODE, 
        .channel        = LEDC_CHANNEL_0,      
        .timer_sel      = LEDC_TIMER_0,        
        .intr_type      = LEDC_INTR_DISABLE,   
        .gpio_num       = LED_BRAKE_LIGHT_PIN,   
        .duty           = 0, // Set duty to 0% 
        .hpoint         = 0                    
    };                                

    ESP_ERROR_CHECK(ledc_channel_config(&brake_ledc_channel));

    ledc_channel_config_t RGB_ledc_channel[3];
    for (size_t i = 0; i < 3; i++)
    {
        RGB_ledc_channel[i].speed_mode   = LEDC_LOW_SPEED_MODE;
        RGB_ledc_channel[i].timer_sel    = LEDC_TIMER_0;
        RGB_ledc_channel[i].intr_type    = LEDC_INTR_DISABLE;
        RGB_ledc_channel[i].duty         = 0; // Set duty to 0% 
        RGB_ledc_channel[i].hpoint       = 0;
        
        if (i == 0)
        {
            RGB_ledc_channel[i].channel  = LEDC_CHANNEL_1;
            RGB_ledc_channel[i].gpio_num = LED_RED_PIN;
        }

        else if (i == 1)
        {
            RGB_ledc_channel[i].channel  = LEDC_CHANNEL_2;
            RGB_ledc_channel[i].gpio_num = LED_GREEN_PIN;
        }

        else if (i == 2)
        {
            RGB_ledc_channel[i].channel  = LEDC_CHANNEL_3;
            RGB_ledc_channel[i].gpio_num = LED_BLUE_PIN;
        }

        ESP_ERROR_CHECK(ledc_channel_config(&RGB_ledc_channel[i]));
    };   
}

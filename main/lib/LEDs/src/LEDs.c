#include "LEDs.h"

void ConfigLEDs()
{
/*================================LED CONFIG====================================================*/
    gpio_config_t LED_config = {
        .pin_bit_mask = BIT(LED_HEADLIGHT_PIN) | BIT(LED_RIGHT_PIN) | BIT(LED_LEFT_PIN),
        .mode         = GPIO_MODE,
        .pull_up_en   = PULLUP_MODE,
        .pull_down_en = PULLDOWN_MODE,
        .intr_type    = GPIO_INTR_TYPE
    };

    ESP_ERROR_CHECK(gpio_config(&LED_config));
/*=============================================================================================*/
    
/*=================================LEDC CONFIG==================================================*/
    ledc_timer_config_t LEDs_ledc_timer = {         
        .duty_resolution = LED_RESOLUTION,     
        .freq_hz         = FREQ_LED,               
        .speed_mode      = SPEED_MODE,
        .timer_num       = LED_TIMER,        
        .clk_cfg         = CLK_FOR_LED         
    };                               

    ESP_ERROR_CHECK(ledc_timer_config(&LEDs_ledc_timer));

    ledc_channel_config_t brake_ledc_channel = {   
        .speed_mode     = SPEED_MODE, 
        .channel        = BRAKE_CHANNEL,      
        .timer_sel      = LED_TIMER,        
        .intr_type      = LEDC_INTR_TYPE,   
        .gpio_num       = LED_BRAKE_LIGHT_PIN,   
        .duty           = 0, // Set duty to 0% 
        .hpoint         = 0                    
    };                                

    ESP_ERROR_CHECK(ledc_channel_config(&brake_ledc_channel));
 /*=============================================================================================*/
}

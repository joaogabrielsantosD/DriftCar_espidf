#ifndef ESP32MX1508_h
#define ESP32MX1508_h

#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "esp_err.h"

typedef struct
{
  gpio_num_t pinIN1, pinIN2;     // Pin to MX1508
  ledc_channel_t ledCH1, ledCH2; // ESP32 ledc Channel for PWM  
  ledc_timer_bit_t resolution;   // PWM Resolution
  uint32_t frequecy;             // PWM Freq

} MX1508_config_t;

typedef struct
{
  ledc_channel_t _CH1, _CH2;
  
	long _pwmVal;                  // PWM Value (speed)
  long _maxpwm;                  // Max PWM Value of the Motor
} PWM_values_t;

void MX1508_init(MX1508_config_t *mx1508);

void motorGo(long pwmVal); 
void motorRev(long pwmVal);
void motorBrake(void);                // Sets boths signals to high to hard brake the motor 
void motorStop(void);                 // Sets both signals to low to allow motor to spin down
void stopMotor(void);                 // Kept for backwards compatibility

#endif

#ifndef ESP32MX1508_h
#define ESP32MX1508_h

#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

#define MX1508_UNIT 2
#define SPEED_MODE LEDC_LOW_SPEED_MODE

/**
 * @brief Configuration of DC motor channel
 * 
 */
typedef struct
{
  gpio_num_t motor_pin[MX1508_UNIT]; /**< Pin number of pwm output */
  ledc_channel_t ch[MX1508_UNIT];    /**< The ledc channel which used */
} MX1508_channel_t;

/**
 * @brief Configuration of DC motor
 * 
 */
typedef struct
{
  uint32_t freq;             /**< PWM frequency */
  ledc_timer_t timer_number; /**< Timer number of ledc */
  ledc_timer_t resolution;   /**< Channel resolution */
  MX1508_channel_t channels; /**< Channels to use */
  uint8_t channel_number;    /**< Total channel number */
} MX1508_config_t;

/**
 * @brief Auxiliar struct for save the PWM values and Pins/Channel
 * 
 */
typedef struct
{
  ledc_channel_t _CH1, _CH2;
  
	long _pwmVal;                  // PWM Value (speed)
  long _maxpwm;                  // Max PWM Value of the Motor
} PWM_values_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize ledc to control the Motor
 *
 * @param config Pointer of Motor configure struct
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 *     - ESP_FAIL Configure ledc failed
 */
esp_err_t MX1508_init(const MX1508_config_t *config);

/**
 * @brief Deinitialize ledc for motor
 *
 * @param config Pointer of Motor configure struct
 *
 * @return
 *     - ESP_OK Success
 */
esp_err_t MX1508_deinit(const MX1508_config_t *config);

/**
 * @brief Read current duty of one channel
 *
 * @param channel LEDC channel, select from ledc_channel_t
 * @param duty Current duty of the channel
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t MX1508_read_duty(uint8_t channel, uint32_t *duty);

/**
 * @brief Set the motor to a certain spee
 *
 * @attention motorGo is foward and motorRev is reverse
 * 
 * @param channel LEDC channel, select from ledc_channel_t
 * @param duty Current duty of the channel
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t motorGo(long pwmVal); 
esp_err_t motorRev(long pwmVal); // Reverse

/**
 * @brief Hard stop
 */
void motorBrake(void);                // Sets boths signals to high to hard brake the motor 

/**
 * @brief Soft stop
 */
void motorStop(void);                 // Sets both signals to low to allow motor to spin down
void stopMotor(void);                 // Kept for backwards compatibility

#ifdef __cplusplus
}
#endif

#endif

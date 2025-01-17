#ifndef HARDWARE_DEFS_H
#define HARDWARE_DEFS_H

#include <driver/gpio.h>

/* Accelerometer/Gyroscope pins */
#define MPU_SCU              GPIO_NUM_22
#define MPU_SDA              GPIO_NUM_21

/* Servo pins */
#define SERVO_HEAD_LIGHT     GPIO_NUM_18
#define SERVO_STEERING_WHELL GPIO_NUM_19

/* Motor DC driver pins */
#define MOTOR_FOWARD_PIN     GPIO_NUM_15
#define MOTOR_REVERSE_PIN    GPIO_NUM_13

/* Analog pin to read the voltage */
#define SOC_PIN              GPIO_NUM_34

/* LEDs pins */
#define LED_LEFT_PIN         GPIO_NUM_4
#define LED_RIGHT_PIN        GPIO_NUM_27
#define LED_BRAKE_LIGHT_PIN  GPIO_NUM_32
#define LED_HEADLIGHT_PIN    GPIO_NUM_2
#define LED_RED_PIN          GPIO_NUM_23
#define LED_GREEN_PIN        GPIO_NUM_25
#define LED_BLUE_PIN         GPIO_NUM_26

#endif

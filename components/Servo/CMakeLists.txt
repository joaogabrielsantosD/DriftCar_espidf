idf_component_register(SRCS         "iot_servo.c"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_driver_ledc esp_driver_gpio)
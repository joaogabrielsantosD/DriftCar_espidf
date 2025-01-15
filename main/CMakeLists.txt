idf_component_register(SRCS "src/main.c" 
                       INCLUDE_DIRS "."
                       REQUIRES driver spi_flash MX1508 Servo ahrs mpu9250)

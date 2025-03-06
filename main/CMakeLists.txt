idf_component_register(SRCS "src/main.c" "lib/ACC_GYR/src/acc_gyr.c" "lib/LEDs/src/LEDs.c" "lib/SOC/src/SOC.c"
                       INCLUDE_DIRS "." "include" "lib/ACC_GYR" "lib/LEDs" "lib/SOC"
                       REQUIRES driver spi_flash nvs_flash esp_event esp_netif esp_wifi esp_adc 
                       MX1508 ahrs mpu9250 Servo esp_system esp_timer)

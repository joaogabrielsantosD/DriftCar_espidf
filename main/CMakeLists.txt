idf_component_register(SRCS "src/main.c" "lib/ACC_GYR/src/acc_gyr.c"
                       INCLUDE_DIRS "." "include" "lib/ACC_GYR"
                       REQUIRES driver spi_flash nvs_flash esp_event esp_netif esp_wifi 
                       MX1508 ahrs mpu9250 Servo esp_system)

idf_component_register(SRCS "src/main.c" "lib/LEDs/src/LEDs.c" "lib/ACC_GYR/src/acc_gyr.c"
                    "lib/OtherThings/src/OtherThings.c"
                       INCLUDE_DIRS "." "include" "lib/LEDs" "lib/ACC_GYR" "lib/OtherThings"
                       REQUIRES driver spi_flash nvs_flash esp_event esp_netif esp_wifi 
                       MX1508 ahrs mpu9250 Servo esp_system)

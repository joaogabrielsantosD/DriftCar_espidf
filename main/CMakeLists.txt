idf_component_register(SRCS "src/main.c" "lib/LEDs/src/LEDs.c"
                       INCLUDE_DIRS "." "include" "lib/LEDs"
                       REQUIRES driver spi_flash nvs_flash esp_event esp_netif esp_wifi MX1508 Servo ahrs mpu9250)

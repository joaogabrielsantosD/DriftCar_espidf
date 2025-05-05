#ifndef __DRIFTCAR__
#define __DRIFTCAR__

#include "sdkconfig.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"

void print_info(void);
void print_memory(const TaskHandle_t *tk);


#endif // __DRIFTCAR__
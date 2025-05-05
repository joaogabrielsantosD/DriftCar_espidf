#ifndef _SOC_MODULE_H_
#define _SOC_MODULE_H_

#include "esp_adc/adc_oneshot.h"
#include "driver_defs.h"

// ADC configuration
#define SOC_ADC_UNIT     ADC_UNIT_1
#define SOC_ADC_CHANNEL  ADC_CHANNEL_6
#define SOC_WIDTH        ADC_BITWIDTH_12
#define SOC_ATTENUATION  ADC_ATTEN_DB_12

// Battery voltage divider (R1 / R2 in kΩ)
#define R1               47
#define R2               30

// Battery voltage reference
#define VREF             8.40f
#define VREF_MIN         6.00f

typedef struct {
    int adc_value;
    float voltage_value;
    uint8_t percentage_value;

    uint32_t adc_resolution;

    adc_oneshot_unit_handle_t adc1_handle;
} soc_handle_t;

soc_handle_t *soc_create(void);
esp_err_t soc_destroy(soc_handle_t *handle);
uint8_t soc_read(soc_handle_t *handle);

esp_err_t soc_start_task(soc_handle_t *handle);
void soc_stop_task(void);
TaskHandle_t soc_task_handler(void);

#endif // _SOC_MODULE_H_
